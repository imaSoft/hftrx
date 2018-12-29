/* $Id$ */
//
// ������ HF Dream Receiver (�� ������� �����)
// ����� ���� ����������� mgs2001@mail.ru
// UA1ARN
//
// STM32F42XX LCD-TFT Controller (LTDC)
// RENESAS Video Display Controller 5
//	Video Display Controller 5 (5): Image Synthesizer
//	Video Display Controller 5 (7): Output Controller

#include "hardware.h"

#include "display.h"
#include <stdint.h>
#include <string.h>

#include "formats.h"	// for debug prints
#include "spifuncs.h"

#if LCDMODE_LTDC


#if LCDMODE_LQ043T3DX02K
	// Sony PSP-1000 display panel
	// LQ043T3DX02K panel (272*480)
	// RK043FN48H-CT672B  panel (272*480) - ����� STM32F746G-DISCO
	/** 
	  * @brief  RK043FN48H Size  
	  */    
	enum
	{
		WIDTH = 480,				/* LCD PIXEL WIDTH            */
		HEIGHT = 272,			/* LCD PIXEL HEIGHT           */
		/** 
		  * @brief  RK043FN48H Timing  
		  */     
		HSYNC = 41,				/* Horizontal synchronization */
		HBP = 2,				/* Horizontal back porch      */
		HFP = 2,				/* Horizontal front porch     */

		VSYNC = 10,				/* Vertical synchronization   */
		VBP = 2,					/* Vertical back porch        */
		VFP = 2,					/* Vertical front porch       */
	};
#elif LCDMODE_ILI8961
	// HHT270C-8961-6A6 (320*240)
	enum
	{
		WIDTH = 320 * 3,				/* LCD PIXEL WIDTH            */
		HEIGHT = 240,			/* LCD PIXEL HEIGHT           */

		/** 
		  * @brief  RK043FN48H Timing  
		  */     
		HSYNC = 1,				/* Horizontal synchronization */
		HBP = 2,				/* Horizontal back porch      */
		HFP = 2,				/* Horizontal front porch     */

		VSYNC = 1,				/* Vertical synchronization   */
		VBP = 2,					/* Vertical back porch        */
		VFP = 2,					/* Vertical front porch       */
	};
#elif LCDMODE_ILI9341
	// SF-TC240T-9370-T (320*240)
	enum
	{

		WIDTH = 240,				/* LCD PIXEL WIDTH            */
		HEIGHT = 320,			/* LCD PIXEL HEIGHT           */

		/** 
		  * @brief  RK043FN48H Timing  
		  */     
		HSYNC = 10,				/* Horizontal synchronization */
		HBP = 20,				/* Horizontal back porch      */
		HFP = 10,				/* Horizontal front porch     */

		VSYNC = 2,				/* Vertical synchronization   */
		VBP = 4,					/* Vertical back porch        */
		VFP = 4,					/* Vertical front porch       */
	};
#else
	#error Unsupported LCDMODE_xxx
#endif

#if CPUSTYLE_R7S721

/* Set value at specified position of I/O register */
#define SETREG32(reg, iwidth, pos, ival) do { \
	uint_fast32_t val = (ival); \
	uint_fast8_t width = (iwidth); \
	uint_fast32_t mask = 0; \
	while (width --) \
		mask = (mask << 1) | 1U; \
	mask <<= pos; \
	val <<= pos; \
	ASSERT((val & mask) == val); \
	* reg = (* reg & ~ (mask)) | (val & mask); \
	(void) * reg;	/* dummy read */ \
	/*ASSERT(((* reg) & mask) == val); */ \
} while (0)

void
arm_hardware_ltdc_initialize(void)
{
	debug_printf_P(PSTR("arm_hardware_ltdc_initialize start, WIDTH=%d, HEIGHT=%d\n"), WIDTH, HEIGHT);


	/* ---- Supply clock to the video display controller 5  ---- */
	CPG.STBCR9 &= ~ CPG_STBCR9_MSTP91;	// Module Stop 91 0: The video display controller 5 runs.
	(void) CPG.STBCR9;			/* Dummy read */

	/* Configure the LCD Control pins */
	HARDWARE_LTDC_INITIALIZE();	// ����������� � ������� ���������� �������� ������������� �����������


	// I/O Clock Frequency (MHz) = 60 MHz
	VDC50.SYSCNT_PANEL_CLK =
		(3 << 12) |	/* Divided Clock Source Select: 3: Peripheral clock 1 */
		(calcdivround2(P1CLOCK_FREQ, LTDC_DOTCLK) << 0) | /* Clock Frequency Division Ratio - Table 35.5 I/O Clock Frequency and Divisors */
		(1 << 8) |	/* ICKEN */
		0;

	/* hardware-dependent control signals */
	// LCD0_TCON4 - VSYNC P7_5
	// LCD0_TCON5 - HSYNC P7_6
	// LCD0_TCON6 - DE P7_7

	////////////////////////////////////////////////////////////////
	// OUT
	SETREG32(& VDC50.OUT_UPDATE, 1, 0, 1);

	SETREG32(& VDC50.OUT_SET, 2, 12, 0x02);	// OUT_FORMAT Output Format Select 2: RGB565

	SETREG32(& VDC50.OUT_UPDATE, 1, 0, 1);

	////////////////////////////////////////////////////////////////
	// TCON
	SETREG32(& VDC50.TCON_UPDATE, 1, 0, 1);	// TCON_VEN

	//modreg32(& VDC50.TCON_TIM_POLA2, (3uL << 12), (0uL << 12));	// TCON_POLA_MD
	//modreg32(& VDC50.TCON_TIM_POLA2, (1 << 4), (0uL << 4));	// TCON_POLA_INV

	SETREG32(& VDC50.TCON_TIM, 11, 16, WIDTH / 2);	// TCON_HALF
	// Horisontal sync generation parameters

	SETREG32(& VDC50.TCON_TIM_POLA2, 2, 12, 0x00);	// TCON_POLA_MD
	// HSYNC signal
	SETREG32(& VDC50.TCON_TIM_STH1, 11, 16,	0);	// TCON_STH_HS
	SETREG32(& VDC50.TCON_TIM_STH1, 11, 0, HSYNC);	// TCON_STH_HW
	// Source strobe signal
	SETREG32(& VDC50.TCON_TIM_STB1, 11, 16, 0);		// TCON_STB_HS
	SETREG32(& VDC50.TCON_TIM_STB1, 11, 0, WIDTH);	// TCON_STB_HW
	// Hsymc polarity
	SETREG32(& VDC50.TCON_TIM_STH2, 1, 4, 0x01);	// TCON_STH_INV

	// Vertical sync generation parameters

	// Vertical enable signal
	SETREG32(& VDC50.TCON_TIM_STVB1, 11, 16, VSYNC + VBP);	// TCON_STVB_VS
	SETREG32(& VDC50.TCON_TIM_STVB1, 11, 0, HEIGHT);	// TCON_STVB_VW

	// VSYNC signal
	SETREG32(& VDC50.TCON_TIM_STVA1, 11, 16, 0);	// TCON_STVA_VS
	SETREG32(& VDC50.TCON_TIM_STVA1, 11, 0, VSYNC);	// TCON_STVA_VW
	// VSYNC polarity
	SETREG32(& VDC50.TCON_TIM_STVA2, 1, 4, 0x01);	// TCON_STVA_INV

	// Hardware-depemdent procedure
	// Output pins route
	//SETREG32(& VDC50.TCON_TIM_STVA2, 3, 0, 0xXX);	// Output Signal Select for LCD_TCON0 pin - 
	//SETREG32(& VDC50.TCON_TIM_STVB2, 3, 0, 0xXX);	// Output Signal Select for LCD_TCON1 pin - 
	//SETREG32(& VDC50.TCON_TIM_STH2, 3, 0, 0xXX);	// Output Signal Select for LCD_TCON2 pin - 
	SETREG32(& VDC50.TCON_TIM_CPV2, 3, 0, 0x00);	// TCON_CPV_SEL 0: STVA/VS Output Signal Select for LCD_TCON4 Pin - VSYNC
	SETREG32(& VDC50.TCON_TIM_POLA2, 3, 0, 0x02);	// 2: STH/SP/HS TCON_POLA_SEL Output Signal Select for LCD_TCON5 Pin - HSYNC
	SETREG32(& VDC50.TCON_TIM_POLB2, 3, 0, 0x07);	// Output Signal Select for LCD_TCON6 Pin - DE

	SETREG32(& VDC50.TCON_UPDATE, 1, 0, 1);	// TCON_VEN

	////////////////////////////////////////////////////////////////
	// SC0
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 13, 1);	// SC0_SCL0_VEN_D	Scaling-Up Control and Frame Buffer Read Control Register Update
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 12, 1);	// SC0_SCL0_VEN_C	Scaling-Down Control and Frame Buffer Read Control Register Update
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 8, 1);		// SC0_SCL0_UPDATE	SYNC Control Register Update
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 4, 1);		// SC0_SCL0_VEN_B	Synchronization Control and Scaling-up Control Register Update
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 0, 1);		// SC0_SCL0_VEN_A	Scaling-Down Control Register Update

	SETREG32(& VDC50.SC0_SCL0_FRC3, 1, 0, 0x01);	// SC0_RES_VS_SEL Vsync Signal Output Select 1: Internally generated free-running Vsync signal

	SETREG32(& VDC50.SC0_SCL0_FRC5, 1, 8, 0x00);	// SC0_RES_FLD_DLY_SEL

	SETREG32(& VDC50.SC0_SCL0_FRC4, 11, 16, HEIGHT + VSYNC + VBP + VFP - 1);// SC0_RES_FV Free-Running Vsync Period Setting
	SETREG32(& VDC50.SC0_SCL0_FRC4, 11, 0, WIDTH + HSYNC + HBP + HFP - 1);	// SC0_RES_HV Hsync Period Setting

	SETREG32(& VDC50.SC0_SCL0_FRC6, 11, 16, VSYNC + VBP - 1);				// SC0_RES_F_VS
	SETREG32(& VDC50.SC0_SCL0_FRC6, 11, 0, HEIGHT);							// SC0_RES_F_VW

	SETREG32(& VDC50.SC0_SCL0_FRC7, 11, 16, HSYNC + HBP - 1);			// SC0_RES_F_HS
	SETREG32(& VDC50.SC0_SCL0_FRC7, 11, 0, WIDTH);						// SC0_RES_F_HW

	debug_printf_P(PSTR("VDC50.SC0_SCL0_DS1=%08lX s1\n"), VDC50.SC0_SCL0_DS1);
	debug_printf_P(PSTR("VDC50.SC0_SCL0_US1=%08lX s1\n"), VDC50.SC0_SCL0_US1);
	// down-scaler off
	// depend on SC0_SCL0_VEN_A
	SETREG32(& VDC50.SC0_SCL0_DS1, 1, 4, 0);	// SC0_RES_DS_V_ON Vertical Scale Down On/Off 0: Off
	SETREG32(& VDC50.SC0_SCL0_DS1, 1, 0, 0);	// SC0_RES_DS_H_ON

	// up-scaler off
	// depend on SC0_SCL0_VEN_B
	SETREG32(& VDC50.SC0_SCL0_US1, 1, 4, 0);	// SC0_RES_US_V_ON
	SETREG32(& VDC50.SC0_SCL0_US1, 1, 0, 0);	// SC0_RES_US_V_ON

	debug_printf_P(PSTR("VDC50.SC0_SCL0_DS1=%08lX s2\n"), VDC50.SC0_SCL0_DS1);
	debug_printf_P(PSTR("VDC50.SC0_SCL0_US1=%08lX s2\n"), VDC50.SC0_SCL0_US1);

	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 13, 1);	// SC0_SCL0_VEN_D	Scaling-Up Control and Frame Buffer Read Control Register Update
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 12, 1);	// SC0_SCL0_VEN_C	Scaling-Down Control and Frame Buffer Read Control Register Update
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 8, 1);		// SC0_SCL0_UPDATE	SYNC Control Register Update
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 4, 1);		// SC0_SCL0_VEN_B	Synchronization Control and Scaling-up Control Register Update
	SETREG32(& VDC50.SC0_SCL0_UPDATE, 1, 0, 1);		// SC0_SCL0_VEN_A	Scaling-Down Control Register Update

	////////////////////////////////////////////////////////////////
	// SC1
#if 0
	SETREG32(& VDC50.SC0_SCL1_UPDATE, 1, 20, 1);	// SC0_SCL1_UPDATE_B
	SETREG32(& VDC50.SC0_SCL1_UPDATE, 1, 16, 1);	// SC0_SCL1_UPDATE_A
	SETREG32(& VDC50.SC0_SCL1_UPDATE, 1, 4, 1);		// SC0_SCL1_VEN_B
	SETREG32(& VDC50.SC0_SCL1_UPDATE, 1, 0, 1);		// SC0_SCL1_VEN_A
#endif

	////////////////////////////////////////////////////////////////
	// GR0
#if 1
	SETREG32(& VDC50.GR0_UPDATE, 1, 8, 1);	// GR0_UPDATE Frame Buffer Read Control Register Update
	SETREG32(& VDC50.GR0_UPDATE, 1, 4, 1);	// GR0_P_VEN Graphics Display Register Update
	SETREG32(& VDC50.GR0_UPDATE, 1, 0, 1);	// GR0_IBUS_VEN Frame Buffer Read Control Register Update

	SETREG32(& VDC50.GR0_FLM_RD, 1, 0, 0);	// GR0_R_ENB Frame Buffer Read Enable
	SETREG32(& VDC50.GR0_FLM2, 32, 0, (uintptr_t) & framebuff);	// GR0_BASE
	SETREG32(& VDC50.GR0_FLM3, 15, 16, 2uL * WIDTH);	// GR0_LN_OFF TODO: sizeof must me used
	SETREG32(& VDC50.GR0_FLM4, 23, 0, 2uL * WIDTH * HEIGHT);	// TODO: sizeof must me used
	SETREG32(& VDC50.GR0_FLM5, 11, 16, HEIGHT - 1);	// GR0_FLM_LNUM Sets the number of lines in a frame
	SETREG32(& VDC50.GR0_FLM5, 11, 0, HEIGHT - 1);	// GR0_FLM_LOOP
	SETREG32(& VDC50.GR0_FLM6, 11, 16, WIDTH - 1);	// GR0_HW Sets the width of the horizontal valid period.
	SETREG32(& VDC50.GR0_FLM6, 4, 28, 0x00);		// GR0_FORMAT 0: RGB565
	SETREG32(& VDC50.GR0_AB1, 2, 0,	0x00);	// GR0_DISP_SEL 0: background color

	SETREG32(& VDC50.GR0_UPDATE, 1, 8, 1);	// GR0_UPDATE Frame Buffer Read Control Register Update
	SETREG32(& VDC50.GR0_UPDATE, 1, 4, 1);	// GR0_P_VEN Graphics Display Register Update
	SETREG32(& VDC50.GR0_UPDATE, 1, 0, 1);	// GR0_IBUS_VEN Frame Buffer Read Control Register Update
#endif
	////////////////////////////////////////////////////////////////
	// GR2

	SETREG32(& VDC50.GR2_UPDATE, 1, 8, 1);	// GR2_UPDATE Frame Buffer Read Control Register Update
	SETREG32(& VDC50.GR2_UPDATE, 1, 4, 1);	// GR2_P_VEN Graphics Display Register Update
	SETREG32(& VDC50.GR2_UPDATE, 1, 0, 1);	// GR2_IBUS_VEN Frame Buffer Read Control Register Update

	SETREG32(& VDC50.GR2_FLM_RD, 1, 0, 0);	// GR2_R_ENB Frame Buffer Read Enable
	SETREG32(& VDC50.GR2_FLM2, 32, 0, (uintptr_t) & framebuff);	// GR2_BASE
	SETREG32(& VDC50.GR2_FLM3, 15, 16, 2uL * WIDTH);	// GR0_LN_OFF TODO: sizeof must me used
	SETREG32(& VDC50.GR2_FLM4, 23, 0, 2uL * WIDTH * HEIGHT);	// TODO: sizeof must me used
	SETREG32(& VDC50.GR2_FLM5, 11, 16, HEIGHT - 1);	// GR2_FLM_LNUM Sets the number of lines in a frame
	SETREG32(& VDC50.GR2_FLM5, 11, 0, HEIGHT - 1);	// GR2_FLM_LOOP Sets the number of lines in a frame
	SETREG32(& VDC50.GR2_FLM6, 11, 16, WIDTH - 1);	// GR2_HW Sets the width of the horizontal valid period.
	SETREG32(& VDC50.GR2_FLM6, 4, 28, 0x00);		// GR2_FORMAT 0: RGB565
	SETREG32(& VDC50.GR2_AB1, 2, 0,	0x00);	// GR2_DISP_SEL 0: Background color display

	SETREG32(& VDC50.GR2_UPDATE, 1, 8, 1);	// GR2_UPDATE Frame Buffer Read Control Register Update
	SETREG32(& VDC50.GR2_UPDATE, 1, 4, 1);	// GR2_P_VEN Graphics Display Register Update
	SETREG32(& VDC50.GR2_UPDATE, 1, 0, 1);	// GR2_IBUS_VEN Frame Buffer Read Control Register Update

	////////////////////////////////////////////////////////////////
	// GR3

	SETREG32(& VDC50.GR3_UPDATE, 1, 8, 1);	// GR3_UPDATE Frame Buffer Read Control Register Update
	SETREG32(& VDC50.GR3_UPDATE, 1, 4, 1);	// GR3_P_VEN Graphics Display Register Update
	SETREG32(& VDC50.GR3_UPDATE, 1, 0, 1);	// GR3_IBUS_VEN Frame Buffer Read Control Register Update

	SETREG32(& VDC50.GR3_FLM_RD, 1, 0, 1);	// GR3_R_ENB Frame Buffer Read Enable
	SETREG32(& VDC50.GR3_FLM2, 32, 0, (uintptr_t) & framebuff);	// GR3_BASE
	SETREG32(& VDC50.GR3_FLM3, 15, 16, 2uL * WIDTH);	// GR0_LN_OFF TODO: sizeof must me used
	SETREG32(& VDC50.GR0_FLM4, 23, 0, 2uL * WIDTH * HEIGHT);	// TODO: sizeof must me used
	SETREG32(& VDC50.GR3_FLM5, 11, 16, HEIGHT - 1);	// GR3_FLM_LNUM Sets the number of lines in a frame
	SETREG32(& VDC50.GR3_FLM5, 11, 0, HEIGHT - 1);	// GR3_FLM_LOOP Sets the number of lines in a frame
	SETREG32(& VDC50.GR3_FLM6, 11, 16, WIDTH - 1);	// GR3_HW Sets the width of the horizontal valid period.
	SETREG32(& VDC50.GR3_FLM6, 4, 28, 0x00);		// GR3_FORMAT 0: RGB565
	SETREG32(& VDC50.GR3_AB1, 2, 0,	0x02);	// GR3_DISP_SEL 2: Current graphics display

	SETREG32(& VDC50.GR3_UPDATE, 1, 8, 1);	// GR3_UPDATE Frame Buffer Read Control Register Update
	SETREG32(& VDC50.GR3_UPDATE, 1, 4, 1);	// GR3_P_VEN Graphics Display Register Update
	SETREG32(& VDC50.GR3_UPDATE, 1, 0, 1);	// GR3_IBUS_VEN Frame Buffer Read Control Register Update


	debug_printf_P(PSTR("arm_hardware_ltdc_initialize done\n"));
}

#elif CPUSTYLE_STM32F

/** @defgroup LTDC_Pixelformat 
  * @{
  */
#define LTDC_Pixelformat_ARGB8888                  ((uint32_t)0x00000000)
#define LTDC_Pixelformat_RGB888                    ((uint32_t)0x00000001)
#define LTDC_Pixelformat_RGB565                    ((uint32_t)0x00000002)
#define LTDC_Pixelformat_ARGB1555                  ((uint32_t)0x00000003)
#define LTDC_Pixelformat_ARGB4444                  ((uint32_t)0x00000004)
#define LTDC_Pixelformat_L8                        ((uint32_t)0x00000005)
#define LTDC_Pixelformat_AL44                      ((uint32_t)0x00000006)
#define LTDC_Pixelformat_AL88                      ((uint32_t)0x00000007)
/** @defgroup LTDC_BlendingFactor1 
  * @{
  */

/*
This register defines the blending factors F1 and F2.
The general blending formula is: BC = BF1 x C + BF2 x Cs
� BC = Blended color
� BF1 = Blend Factor 1
� C = Current layer color
� BF2 = Blend Factor 2
� Cs = subjacent layers blended color
*/
#define LTDC_BlendingFactor1_CA                       (4 << LTDC_LxBFCR_BF1_Pos)	//((uint32_t)0x00000400)
#define LTDC_BlendingFactor1_PAxCA                    (6 << LTDC_LxBFCR_BF1_Pos)	//((uint32_t)0x00000600)

/**
  * @}
  */
      
/** @defgroup LTDC_BlendingFactor2
  * @{
  */

#define LTDC_BlendingFactor2_CA                       (5 << LTDC_LxBFCR_BF2_Pos)	//((uint32_t)0x00000005)
#define LTDC_BlendingFactor2_PAxCA                    (7 << LTDC_LxBFCR_BF2_Pos)	//((uint32_t)0x00000007)

/**
  * @}
  */

/** @defgroup LTDC_Reload 
  * @{
  */
#define LTDC_IMReload                     LTDC_SRCR_IMR                         /*!< Immediately Reload. */
#define LTDC_VBReload                     LTDC_SRCR_VBR                         /*!< Vertical Blanking Reload. */

/**
  * @}
  */
  
/** @defgroup LTDC_HSPolarity 
  * @{
  */
#define LTDC_HSPolarity_AL                0                /*!< Horizontal Synchronization is active low. */
#define LTDC_HSPolarity_AH                LTDC_GCR_HSPOL                        /*!< Horizontal Synchronization is active high. */

/**
  * @}
  */
  
/** @defgroup LTDC_VSPolarity 
  * @{
  */
#define LTDC_VSPolarity_AL                0                /*!< Vertical Synchronization is active low. */
#define LTDC_VSPolarity_AH                LTDC_GCR_VSPOL                        /*!< Vertical Synchronization is active high. */

/**
  * @}
  */
  
/** @defgroup LTDC_DEPolarity 
  * @{
  */
// 0: Not Data Enable polarity is active low
// 1: Not Data Enable polarity is active high 
#define LTDC_DEPolarity_AL                0                /*!< Data Enable, is active low. */
#define LTDC_DEPolarity_AH                LTDC_GCR_DEPOL                        /*!< Data Enable, is active high. */

/**
  * @}
  */

/** @defgroup LTDC_PCPolarity 
  * @{
  */
#define LTDC_PCPolarity_IPC               0                /*!< input pixel clock. */
#define LTDC_PCPolarity_IIPC              LTDC_GCR_PCPOL                        /*!< inverted input pixel clock. */


/* Exported types ------------------------------------------------------------*/
 
/** 
  * @brief  LTDC Init structure definition  
  */

typedef struct
{
  uint32_t LTDC_HSPolarity;                 /*!< configures the horizontal synchronization polarity.
                                                 This parameter can be one value of @ref LTDC_HSPolarity */

  uint32_t LTDC_VSPolarity;                 /*!< configures the vertical synchronization polarity.
                                                 This parameter can be one value of @ref LTDC_VSPolarity */

  uint32_t LTDC_DEPolarity;                 /*!< configures the data enable polarity. This parameter can
                                                 be one of value of @ref LTDC_DEPolarity */

  uint32_t LTDC_PCPolarity;                 /*!< configures the pixel clock polarity. This parameter can
                                                 be one of value of @ref LTDC_PCPolarity */

  uint32_t LTDC_HorizontalSync;             /*!< configures the number of Horizontal synchronization 
                                                 width. This parameter must range from 0x000 to 0xFFF. */

  uint32_t LTDC_VerticalSync;               /*!< configures the number of Vertical synchronization 
                                                 heigh. This parameter must range from 0x000 to 0x7FF. */

  uint32_t LTDC_AccumulatedHBP;             /*!< configures the accumulated horizontal back porch width.
                                                 This parameter must range from LTDC_HorizontalSync to 0xFFF. */

  uint32_t LTDC_AccumulatedVBP;             /*!< configures the accumulated vertical back porch heigh.
                                                 This parameter must range from LTDC_VerticalSync to 0x7FF. */
            
  uint32_t LTDC_AccumulatedActiveW;         /*!< configures the accumulated active width. This parameter 
                                                 must range from LTDC_AccumulatedHBP to 0xFFF. */

  uint32_t LTDC_AccumulatedActiveH;         /*!< configures the accumulated active heigh. This parameter 
                                                 must range from LTDC_AccumulatedVBP to 0x7FF. */

  uint32_t LTDC_TotalWidth;                 /*!< configures the total width. This parameter 
                                                 must range from LTDC_AccumulatedActiveW to 0xFFF. */

  uint32_t LTDC_TotalHeigh;                 /*!< configures the total heigh. This parameter 
                                                 must range from LTDC_AccumulatedActiveH to 0x7FF. */
            

  PACKEDCOLOR_T LTDC_BackgroundColor;         /*!< configures the background  */

} LTDC_InitTypeDef;

/** 
  * @brief  LTDC Layer structure definition  
  */

typedef struct
{
  uint32_t LTDC_HorizontalStart;            /*!< Configures the Window Horizontal Start Position.
                                                 This parameter must range from 0x000 to 0xFFF. */
            
  uint32_t LTDC_HorizontalStop;             /*!< Configures the Window Horizontal Stop Position.
                                                 This parameter must range from 0x0000 to 0xFFFF. */
  
  uint32_t LTDC_VerticalStart;              /*!< Configures the Window vertical Start Position.
                                                 This parameter must range from 0x000 to 0xFFF. */

  uint32_t LTDC_VerticalStop;               /*!< Configures the Window vaertical Stop Position.
                                                 This parameter must range from 0x0000 to 0xFFFF. */
  
  uint32_t LTDC_PixelFormat;                /*!< Specifies the pixel format. This parameter can be 
                                                 one of value of @ref LTDC_Pixelformat */

  uint32_t LTDC_ConstantAlpha;              /*!< Specifies the constant alpha used for blending.
                                                 This parameter must range from 0x00 to 0xFF. */

  PACKEDCOLOR_T LTDC_DefaultColor;           /*!< Configures the default color value.. */


  uint32_t LTDC_BlendingFactor_1;           /*!< Select the blending factor 1. This parameter 
                                                 can be one of value of @ref LTDC_BlendingFactor1 */

  uint32_t LTDC_BlendingFactor_2;           /*!< Select the blending factor 2. This parameter 
                                                 can be one of value of @ref LTDC_BlendingFactor2 */
            
  uint32_t LTDC_CFBStartAdress;             /*!< Configures the color frame buffer address */

  uint32_t LTDC_CFBLineLength;              /*!< Configures the color frame buffer line length. 
                                                 This parameter must range from 0x0000 to 0x1FFF. */

  uint32_t LTDC_CFBPitch;                   /*!< Configures the color frame buffer pitch in bytes.
                                                 This parameter must range from 0x0000 to 0x1FFF. */
                                                 
  uint32_t LTDC_CFBLineNumber;              /*!< Specifies the number of line in frame buffer. 
                                                 This parameter must range from 0x000 to 0x7FF. */
} LTDC_Layer_InitTypeDef;

#if LCDMODE_LTDC_L24


// ������ ������� ����������� ������ ���������� �������� 
static void
fillLUT_L24(
	LTDC_Layer_TypeDef* LTDC_Layerx
	)
{
	unsigned color;

	for (color = 0; color < 256; ++ color)
	{
		uint_fast8_t r = color, g = color, b = color;
		
		/* ������ �������� � �������� ������� */
		LTDC_Layerx->CLUTWR = 
			((color << 24) & LTDC_LxCLUTWR_CLUTADD) |
			((r << 16) & LTDC_LxCLUTWR_RED) |
			((g << 8) & LTDC_LxCLUTWR_GREEN) |
			((b << 0) & LTDC_LxCLUTWR_BLUE);
	}

	LTDC_Layerx->CR |= LTDC_LxCR_CLUTEN;
}

#elif LCDMODE_LTDC_L8

static void
fillLUT_L8(
	LTDC_Layer_TypeDef* LTDC_Layerx
	)
{
	unsigned color;

	for (color = 0; color < 256; ++ color)
	{
#define XRGB(zr,zg,zb) do { r = (zr), g = (zg), b = (zb); } while (0)
		uint_fast8_t r, g, b;

		switch (color)
		{
		case TFTRGB(0, 0, 0)			/*COLOR_BLACK*/:		XRGB(0, 0, 0);			break;	// 0x00 ������
		case TFTRGB(255, 0, 0)			/*COLOR_RED*/:			XRGB(255, 0, 0);		break; 	// 0xE0 �������
		case TFTRGB(0, 255, 0)			/*COLOR_GREEN*/:		XRGB(0, 255, 0);		break; 	// 0x1C �������
		case TFTRGB(0, 0, 255)			/*COLOR_BLUE*/:			XRGB(0, 0, 255);		break; 	// 0x03 �����
		case TFTRGB(128, 0, 0)			/*COLOR_DARKRED*/:		XRGB(128, 0, 0);		break; 	// 
		case TFTRGB(0, 128, 0)			/*COLOR_DARKGREEN*/:	XRGB(0, 128, 0);		break; 	// 
		case TFTRGB(0, 0, 128)			/*COLOR_DARKBLUE*/:		XRGB(0, 0, 128);		break; 	// 
		case TFTRGB(255, 255, 0)		/*COLOR_YELLOW*/:		XRGB(255, 255, 0);		break; 	// 0xFC ������
		case TFTRGB(255, 0, 255)		/*COLOR_MAGENTA*/:		XRGB(255, 0, 255);		break; 	// 0x83 ���������
		case TFTRGB(0, 255, 255)		/*COLOR_CYAN*/:			XRGB(0, 255, 255);		break; 	// 0x1F �������
		case TFTRGB(255, 255, 255)		/*COLOR_WHITE*/:		XRGB(255, 255, 255);	break;  // 0xff	�����
		case TFTRGB(128, 128, 128)		/*COLOR_GRAY*/:			XRGB(128, 128, 128);	break; 	// �����
		case TFTRGB(0xa5, 0x2a, 0x2a)	/*COLOR_BROWN*/:		XRGB(0xa5, 0x2a, 0x2a);	break; 	// 0x64 ����������
		case TFTRGB(0xff, 0xd7, 0x00)	/*COLOR_GOLD*/:			XRGB(0xff, 0xd7, 0x00);	break; 	// 0xF4 ������
		case TFTRGB(0xd1, 0xe2, 0x31)	/*COLOR_PEAR*/:			XRGB(0xd1, 0xe2, 0x31);	break; 	// 0xDC ��������
#undef XRGB
		default:
			r = ((color & 0xe0) << 0) | ((color & 0x80) ? 0x1f : 0);	// red
			g = ((color & 0x1c) << 3) | ((color & 0x10) ? 0x1f : 0);	// green
			b = ((color & 0x03) << 6) | ((color & 0x02) ? 0x3f : 0);	// blue
			break;
		}
		/* ������ �������� � �������� ������� */
		LTDC_Layerx->CLUTWR = 
			((color << LTDC_LxCLUTWR_CLUTADD_Pos) & LTDC_LxCLUTWR_CLUTADD) |
			((r << LTDC_LxCLUTWR_RED_Pos) & LTDC_LxCLUTWR_RED) |
			((g << LTDC_LxCLUTWR_GREEN_Pos) & LTDC_LxCLUTWR_GREEN) |
			((b << LTDC_LxCLUTWR_BLUE_Pos) & LTDC_LxCLUTWR_BLUE) |
			0;
	}

	LTDC_Layerx->CR |= LTDC_LxCR_CLUTEN;
}
#else

#endif /* LCDMODE_LTDC_L8 */


/**
  * @brief  Initializes the LTDC Layer according to the specified parameters
  *         in the LTDC_LayerStruct.
  * @note   This function can be used only when the LTDC is disabled.
  * @param  LTDC_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: LTDC_Layer1, LTDC_Layer2    
  * @param  LTDC_LayerStruct: pointer to a LTDC_LayerTypeDef structure that contains
  *         the configuration information for the specified LTDC peripheral.
  * @retval None
  */

static void 
LTDC_LayerInit(LTDC_Layer_TypeDef* LTDC_Layerx, const LTDC_Layer_InitTypeDef* LTDC_Layer_InitStruct)
{

	uint32_t whsppos = 0;
	uint32_t wvsppos = 0;
	uint32_t cfbp = 0;

	/* Configures the horizontal start and stop position */
	whsppos = LTDC_Layer_InitStruct->LTDC_HorizontalStop << 16;
	LTDC_Layerx->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
	LTDC_Layerx->WHPCR |= (LTDC_Layer_InitStruct->LTDC_HorizontalStart | whsppos);

	/* Configures the vertical start and stop position */
	wvsppos = LTDC_Layer_InitStruct->LTDC_VerticalStop << 16;
	LTDC_Layerx->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
	LTDC_Layerx->WVPCR |= (LTDC_Layer_InitStruct->LTDC_VerticalStart | wvsppos);

	/* Specifies the pixel format */
	LTDC_Layerx->PFCR &= ~(LTDC_LxPFCR_PF);
	LTDC_Layerx->PFCR |= (LTDC_Layer_InitStruct->LTDC_PixelFormat);

	/* Configures the default color values */
	LTDC_Layerx->DCCR = LTDC_Layer_InitStruct->LTDC_DefaultColor;

	/* Specifies the constant alpha value */      
	// alpha ����� ���� � ����������� �� �������� �������� � ������ �������
	LTDC_Layerx->CACR = (LTDC_Layerx->CACR & ~ (LTDC_LxCACR_CONSTA)) |
		(LTDC_Layer_InitStruct->LTDC_ConstantAlpha << LTDC_LxCACR_CONSTA_Pos) |
		0;

	/* Specifies the blending factors */
	LTDC_Layerx->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);
	LTDC_Layerx->BFCR |= (LTDC_Layer_InitStruct->LTDC_BlendingFactor_1 | LTDC_Layer_InitStruct->LTDC_BlendingFactor_2);

	/* Configures the color frame buffer start address */
	LTDC_Layerx->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
	LTDC_Layerx->CFBAR |= (LTDC_Layer_InitStruct->LTDC_CFBStartAdress);

	/* Configures the color frame buffer pitch in byte */
	cfbp = (LTDC_Layer_InitStruct->LTDC_CFBPitch << 16);
	LTDC_Layerx->CFBLR  &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);
	LTDC_Layerx->CFBLR  |= (LTDC_Layer_InitStruct->LTDC_CFBLineLength | cfbp);

	/* Configures the frame buffer line number */
	LTDC_Layerx->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
	LTDC_Layerx->CFBLNR  |= (LTDC_Layer_InitStruct->LTDC_CFBLineNumber);

}
/**
  * @brief  Initializes the LTDC peripheral according to the specified parameters
  *         in the LTDC_InitStruct.
  * @note   This function can be used only when the LTDC is disabled.
  * @param  LTDC_InitStruct: pointer to a LTDC_InitTypeDef structure that contains
  *         the configuration information for the specified LTDC peripheral.
  * @retval None
  */

static void LTDC_Init(LTDC_InitTypeDef* LTDC_InitStruct)
{
	uint32_t horizontalsync = 0;
	uint32_t accumulatedHBP = 0;
	uint32_t accumulatedactiveW = 0;
	uint32_t totalwidth = 0;

	/* Sets Synchronization size */
	LTDC->SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);
	horizontalsync = (LTDC_InitStruct->LTDC_HorizontalSync << LTDC_SSCR_HSW_Pos);
	LTDC->SSCR |= (horizontalsync | LTDC_InitStruct->LTDC_VerticalSync);

	/* Sets Accumulated Back porch */
	LTDC->BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);
	accumulatedHBP = (LTDC_InitStruct->LTDC_AccumulatedHBP << LTDC_BPCR_AHBP_Pos);
	LTDC->BPCR |= (accumulatedHBP | LTDC_InitStruct->LTDC_AccumulatedVBP);

	/* Sets Accumulated Active Width */
	LTDC->AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);
	accumulatedactiveW = (LTDC_InitStruct->LTDC_AccumulatedActiveW << LTDC_AWCR_AAW_Pos);
	LTDC->AWCR |= (accumulatedactiveW | LTDC_InitStruct->LTDC_AccumulatedActiveH);

	/* Sets Total Width */
	LTDC->TWCR &= ~ (LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);
	totalwidth = (LTDC_InitStruct->LTDC_TotalWidth << LTDC_TWCR_TOTALW_Pos);
	LTDC->TWCR |= (totalwidth | LTDC_InitStruct->LTDC_TotalHeigh);

	LTDC->GCR = 0;
	LTDC->GCR |=  (uint32_t)(LTDC_InitStruct->LTDC_HSPolarity | LTDC_InitStruct->LTDC_VSPolarity |
			   LTDC_InitStruct->LTDC_DEPolarity | LTDC_InitStruct->LTDC_PCPolarity);

	/* sets the background color value */
	LTDC->BCCR = (LTDC->BCCR & ~ (LTDC_BCCR_BCBLUE_Msk | LTDC_BCCR_BCGREEN_Msk | LTDC_BCCR_BCRED_Msk)) |
		LTDC_InitStruct->LTDC_BackgroundColor |
		0;
}

static void LCD_LayerInit(
	LTDC_Layer_TypeDef* LTDC_Layerx, 
	unsigned hs,	// same as AccumulatedHBP + 1
	unsigned vs,		// same as LTDC_AccumulatedVBP + 1
	const pipparams_t * wnd,
	uint32_t LTDC_PixelFormat,
	unsigned scale_h,
	unsigned pixelsize	// ��� ������� ������� ������ � ������
	)
{
	const unsigned rowsize = (pixelsize * wnd->w);	// ������ ����� ������ � ������

	LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct; 
	/* Windowing configuration */
	/* In this case all the active display area is used to display a picture then :
	Horizontal start = horizontal synchronization + Horizontal back porch = 30 
	Horizontal stop = Horizontal start + window width -1 = 30 + 240 -1
	Vertical start   = vertical synchronization + vertical back porch     = 4
	Vertical stop   = Vertical start + window height -1  = 4 + 320 -1      */      
	LTDC_Layer_InitStruct.LTDC_HorizontalStart = hs + wnd->x * scale_h;
	LTDC_Layer_InitStruct.LTDC_HorizontalStop = hs + wnd->x * scale_h + wnd->w * scale_h - 1; 
	LTDC_Layer_InitStruct.LTDC_VerticalStart = vs + wnd->y;
	LTDC_Layer_InitStruct.LTDC_VerticalStop = vs + wnd->y + wnd->h - 1;

	/* Pixel Format configuration*/
	LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_PixelFormat;
	/* Alpha constant (255 totally opaque = ������������) */
	LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
	/* Default Color configuration (configure A,R,G,B component values) */          
	LTDC_Layer_InitStruct.LTDC_DefaultColor = 0; // transparent=���������� black color. outside active layer area        
	/* Configure blending factors */       
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA; // ���������� ��� ������� �� �������� ����
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA; // ���������� ��� ������� �� �������������� ���� ����

	/* the length of one line of pixels in bytes + 3 then :
	Line Lenth = Active high width x number of bytes per pixel + 3 
	Active high width         = DIM_SECOND 
	number of bytes per pixel = 2    (pixel_format : RGB565) 
	number of bytes per pixel = 1    (pixel_format : L8) 
	*/
	LTDC_Layer_InitStruct.LTDC_CFBLineLength = rowsize + 3; //((DIM_SECOND * 2) + 3);
	//LTDC_Layer1->CFBLR = ((rowsize << 16) & LTDC_LxCFBLR_CFBP) | (((rowsize + 3) << 0) & LTDC_LxCFBLR_CFBLL);
	/* the pitch is the increment from the start of one line of pixels to the 
	start of the next line in bytes, then :
	Pitch = Active high width x number of bytes per pixel */ 
	LTDC_Layer_InitStruct.LTDC_CFBPitch = rowsize; // (DIM_SECOND * 2);

	/* Configure the number of lines */  
	LTDC_Layer_InitStruct.LTDC_CFBLineNumber = wnd->h;

	/* Start Address configuration : the LCD Frame buffer is defined on SDRAM */    
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uintptr_t) & framebuff;
	//LTDC_Layer1->CFBAR = (uint32_t) & framebuff;

	/* Initialize LTDC layer 1 */
	LTDC_LayerInit(LTDC_Layerx, & LTDC_Layer_InitStruct);

	/* Enable foreground & background Layers */
	//LTDC_LayerCmd(LTDC_Layer1, ENABLE); 
	//LTDC_LayerCmd(LTDC_Layer2, ENABLE);
	/* Enable LTDC_Layer by setting LEN bit */
	LTDC_Layerx->CR |= LTDC_LxCR_LEN;

	/* LTDC configuration reload */  
}


//#define WITHUSELTDCTRANSPARENCY	1

#if WITHUSELTDCTRANSPARENCY

// ������ � ������������� ��� ����� PIP
#define LAYER_PIP	LTDC_Layer1		// PIP layer
#define LAYER_MAIN	LTDC_Layer2

#else /* WITHUSELTDCTRANSPARENCY */

// PIP ����������� ���� ��� ���
#define LAYER_PIP	LTDC_Layer2		// PIP layer = RGB565 format
#define LAYER_MAIN	LTDC_Layer1		// L8 or RGB565 format

#endif /* WITHUSELTDCTRANSPARENCY */

/* ��������� �������� ��� ������ ���� ��� "��������" ��� ������������� ��������� */
static void LCD_LayerInitMain(
	LTDC_Layer_TypeDef* LTDC_Layerx
	)
{
	// �������������� �� ������������ ������� RGB565 �� �������� pfc LTDC
	// � ����������� RGB888
	const unsigned long key = COLOR_KEY;
	const unsigned long keyr = (key >> 11) & 0x1F;
	const unsigned long keyg = (key >> 6) & 0x3F;
	const unsigned long keyb = (key >> 0) & 0x1F;
	const unsigned long keyrpfc = ((keyr << 3) | (keyr >> 3)) & 0xFF;
	const unsigned long keygpfc = ((keyg << 2) | (keyg >> 4)) & 0xFF;
	const unsigned long keybpfc = ((keyb << 3) | (keyb >> 3)) & 0xFF;

	LTDC_Layerx->CKCR = 
		(keyrpfc << LTDC_LxCKCR_CKRED_Pos) |
		(keygpfc << LTDC_LxCKCR_CKGREEN_Pos) |
		(keybpfc << LTDC_LxCKCR_CKBLUE_Pos) |
		0;

#if WITHUSELTDCTRANSPARENCY
	LTDC_Layerx->CR |= LTDC_LxCR_COLKEN;	
#endif /* WITHUSELTDCTRANSPARENCY */

#if 1
	// alpha ����� ���� � ����������� �� �������� �������� � ������ �������
	LTDC_Layerx->CACR = (LTDC_Layerx->CACR & ~ (LTDC_LxCACR_CONSTA)) |
		(255 << LTDC_LxCACR_CONSTA_Pos) |	/* Alpha constant (255 totally opaque=������������) */
		0;
#endif
}

/* ��������� �������� ��� ������ ���� ��� "�������" ��� ������������� ��������� */
static void LCD_LayerInitPIP(
	LTDC_Layer_TypeDef* LTDC_Layerx
	)
{
#if 1
	// alpha ����� ���� � ����������� �� �������� �������� � ������ �������
	LTDC_Layerx->CACR = (LTDC_Layerx->CACR & ~ (LTDC_LxCACR_CONSTA)) |
		(255 << LTDC_LxCACR_CONSTA_Pos) |	/* Alpha constant (255 totally opaque=������������) */
		0;
#endif
}

void
arm_hardware_ltdc_initialize(void)
{
	debug_printf_P(PSTR("arm_hardware_ltdc_initialize start, WIDTH=%d, HEIGHT=%d\n"), WIDTH, HEIGHT);

	//const unsigned rowsize = sizeof framebuff [0];	// ������ ����� ������ � ������
	//const unsigned rowsize2 = (sizeof (PACKEDCOLOR_T) * DIM_SECOND);
	//ASSERT(rowsize == rowsize2);
	debug_printf_P(PSTR("arm_hardware_ltdc_initialize: framebuff=%p\n"), framebuff);

	/* Initialize the LCD */

#if CPUSTYLE_STM32H7XX
	/* Enable the LTDC Clock */
	RCC->APB3ENR |= RCC_APB3ENR_LTDCEN;	/* LTDC clock enable */
	__DSB();

	/* Enable the DMA2D Clock */
	RCC->AHB3ENR |= RCC_AHB3ENR_DMA2DEN;	/* DMA2D clock enable */
	__DSB();
#else /* CPUSTYLE_STM32H7XX */
	/* Enable the LTDC Clock */
	RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;	/* LTDC clock enable */
	__DSB();

	/* Enable the DMA2D Clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;	/* DMA2D clock enable */
	__DSB();
#endif /* CPUSTYLE_STM32H7XX */

	/* Configure the LCD Control pins */
	HARDWARE_LTDC_INITIALIZE();	// ����������� � ������� ���������� �������� ������������� �����������

	/* LTDC Initialization -------------------------------------------------------*/
	LTDC_InitTypeDef LTDC_InitStruct;

	pipparams_t mainwnd = { 0, 0, DIM_SECOND, DIM_FIRST };
	pipparams_t pipwnd;
	display2_getpipparams(& pipwnd);

	debug_printf_P(PSTR("arm_hardware_ltdc_initialize: pip: x/y=%u/%u, w/h=%u/%u\n"), pipwnd.x, pipwnd.y, pipwnd.w, pipwnd.h);

	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
	//LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AH;     
	/* Initialize the vertical synchronization polarity as active low */  
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
	//LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AH;     
	/* Initialize the data enable polarity as active low */ 
	//LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AH;		// While VSYNC is low, do not change DISP signal "Low" or "High"
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;		// While VSYNC is low, do not change DISP signal "Low" or "High"
	/* Initialize the pixel clock polarity as input pixel clock */ 
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

	/* Timing configuration */
	/* Configure horizontal synchronization width */     
	LTDC_InitStruct.LTDC_HorizontalSync = (HSYNC - 1);
	/* Configure accumulated horizontal back porch */
	LTDC_InitStruct.LTDC_AccumulatedHBP = (HSYNC + HBP - 1);
	/* Configure accumulated active width */  
	LTDC_InitStruct.LTDC_AccumulatedActiveW = (WIDTH + HSYNC + HBP - 1);
	/* Configure total width */
	LTDC_InitStruct.LTDC_TotalWidth = (WIDTH + HSYNC + HBP + HFP - 1);

	/* Configure vertical synchronization height */
	LTDC_InitStruct.LTDC_VerticalSync = (VSYNC - 1);
	/* Configure accumulated vertical back porch */
	LTDC_InitStruct.LTDC_AccumulatedVBP = (VSYNC + VBP - 1);
	/* Configure accumulated active height */
	LTDC_InitStruct.LTDC_AccumulatedActiveH = (HEIGHT + VSYNC + VBP - 1);
	/* Configure total height */
	LTDC_InitStruct.LTDC_TotalHeigh = (HEIGHT + VSYNC + VBP + VFP - 1);

	/* Configure R,G,B component values for LCD background color */                   
	LTDC_InitStruct.LTDC_BackgroundColor = 0;		// all 0 - black

	LTDC_Init(&LTDC_InitStruct);

	LTDC_Init(& LTDC_InitStruct);


	/* LTDC initialization end ---------------------------------------------------*/

	// Top layer - LTDC_Layer2
	// Bottom layer - LTDC_Layer1
#if LCDMODE_LTDC_L24

	LCD_LayerInit(LAYER_MAIN, HSYNC + HBP, VSYNC + VBP, & mainwnd, LTDC_Pixelformat_L8, 3, sizeof (PACKEDCOLOR_T));

#elif LCDMODE_LTDC_L8

	LCD_LayerInit(LAYER_MAIN, HSYNC + HBP, VSYNC + VBP, & mainwnd, LTDC_Pixelformat_L8, 1, sizeof (PACKEDCOLOR_T));

#else
	/* ��� ������� */
	LCD_LayerInit(LAYER_MAIN, HSYNC + HBP, VSYNC + VBP, & mainwnd, LTDC_Pixelformat_RGB565, 1, sizeof (PACKEDCOLOR_T));

#endif /* LCDMODE_LTDC_L8 */

#if LCDMODE_LTDC_PIP16

	LCD_LayerInitMain(LAYER_MAIN);	// ������� �������������

	// Bottom layer
	LCD_LayerInit(LAYER_PIP, HSYNC + HBP, VSYNC + VBP, & pipwnd, LTDC_Pixelformat_RGB565, 1, sizeof (uint16_t));
	LCD_LayerInitPIP(LAYER_PIP);	// ������� �������������

#endif /* LCDMODE_LTDC_PIP16 */

	LTDC->SRCR = LTDC_SRCR_IMR;	/*!< Immediately Reload. */

	/* Enable the LTDC */
	LTDC->GCR |= LTDC_GCR_LTDCEN;

#if LCDMODE_LTDC_L24
	fillLUT_L24(LAYER_MAIN);	// ������ ���������� ���� ������ �� ������ �� �����. �������� ������� - ����� ����� �� Reload
#elif LCDMODE_LTDC_L8
	fillLUT_L8(LAYER_MAIN);	// �������� ������� - ����� ����� �� Reload
#endif /* LCDMODE_LTDC_L8 */

	/* LTDC reload configuration */  
	LTDC->SRCR = LTDC_SRCR_IMR;	/* Immediately Reload. */

	// LQ043T3DX02K rules: While �VSYNC� is �Low�, don�t change �DISP� signal �Low� to �High�. 
	HARDWARE_LTDC_SET_DISP(0);
	local_delay_ms(150);
	HARDWARE_LTDC_SET_DISP(1);

	debug_printf_P(PSTR("arm_hardware_ltdc_initialize done\n"));
}

/* set bottom buffer start */
void arm_hardware_ltdc_pip_set(uintptr_t p)
{
#if LCDMODE_LTDC_PIP16
	LAYER_PIP->CFBAR = p;
	LAYER_PIP->CR |= LTDC_LxCR_LEN;
	LTDC->SRCR = LTDC_SRCR_VBR;	/* Vertical Blanking Reload. */
#endif /* LCDMODE_LTDC_PIP16 */
}

void arm_hardware_ltdc_pip_off(void)	// set PIP framebuffer address
{
#if LCDMODE_LTDC_PIP16
	LAYER_PIP->CR &= ~ LTDC_LxCR_LEN;
	LTDC->SRCR = LTDC_SRCR_VBR;	/* Vertical Blanking Reload. */
#endif /* LCDMODE_LTDC_PIP16 */
}
#endif /* CPUSTYLE_STM32F */

#endif /* LCDMODE_LTDC */
