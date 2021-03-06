/* $Id$ */
//
// ������ HF Dream Receiver (�� ������� �����)
// ����� ���� ����������� mgs2001@mail.ru
// UA1ARN
//

#include "hardware.h"	/* ��������� �� ���������� ������� ������ � ������� */
#include "pio.h"

#include "audio.h"

#include "formats.h"	// for debug prints

//#include <string.h>		// memcpy() for debug

typedef struct codechw
{
	void (* initialize_rx)(void);	/* ������������� ��������� ���������� ��� ����� ������ �� ������ */
	void (* initialize_tx)(void);	/* ������������� ��������� ���������� ��� �������� ������ � ����� */
	void (* initializedma_rx)(void);	/* ������������� DMA ���������� ��� ����� ������ �� ������ */
	void (* initializedma_tx)(void);	/* ������������� DMA ���������� ��� �������� ������ � ����� */
	void (* enable_rx)(void);	/* ���������� ����� ������ �� ������ */
	void (* enable_tx)(void);	/* ���������� �������� ������ � ����� */
	const char * label;
} codechw_t;

static void hardware_dummy_initialize(void)
{

}

static void hardware_dummy_enable(void)
{

}

#if WITHINTEGRATEDDSP

#if WITHRTS192 && ! WITHSAI2HW
	#error WITHRTS192 require WITHSAI2HW
#endif /* WITHRTS192 && ! WITHSAI2HW */

// ������ � ��� ������ ����� ������ �� DMA
static uintptr_t 
dma_invalidate16rx(uintptr_t addr)
{
	//arm_hardware_invalidate(addr, DMABUFFSIZE16 * sizeof (uint16_t));
	arm_hardware_flush_invalidate(addr, DMABUFFSIZE16 * sizeof (uint16_t));
	return addr;
}

// ������ ��� ������ ����� ������������ �� DMA ����-��
// ����� ���������� �� ���������
static uintptr_t 
dma_flush16tx(uintptr_t addr)
{
	arm_hardware_flush_invalidate(addr, DMABUFFSIZE16 * sizeof (uint16_t));
	return addr;
}

// ������ � ��� ������ ����� ������ �� DMA
static uintptr_t
dma_invalidate192rts(uintptr_t addr)
{
	//arm_hardware_invalidate(addr, DMABUFFSIZE192RTS * sizeof (uint8_t));
	arm_hardware_flush_invalidate(addr, DMABUFFSIZE192RTS * sizeof (uint8_t));
	return addr;
}

// ������ ��� ������ ����� ������������ �� DMA ����-��
// ����� ���������� �� ���������
static uintptr_t
dma_flushxrtstx(uintptr_t addr, unsigned long size)
{
	//arm_hardware_invalidate(addr, size);
	arm_hardware_flush_invalidate(addr, size);
	return addr;
}
// ������ � ��� ������ ����� ������ �� DMA
static uintptr_t 
dma_invalidate32rx(uintptr_t addr)
{
	//arm_hardware_invalidate(addr, DMABUFFSIZE32RX * sizeof (uint32_t));
	arm_hardware_flush_invalidate(addr, DMABUFFSIZE32RX * sizeof (uint32_t));
	return addr;
}

// ������ ��� ������ ����� ������������ �� DMA ����-��
// ����� ���������� �� ���������
static uintptr_t dma_flush32tx(uintptr_t addr)
{
	arm_hardware_flush_invalidate(addr, DMABUFFSIZE32TX * sizeof (uint32_t));
	return addr;
}

#if CPUSTYLE_STM32F

enum
{
#if WITHSAI1HW

	NBSLOT_SAI1 = WITHSAI1_FRAMEBITS / 32,	
	// ���������� ����� � SLOTEN_TX_xxx � SLOTEN_RX_xxx ������ ���� �����
	// ��������� DMABUFSTEP32RX � DMABUFSTEP32TX ��������������.
	#if WITHSAI1_FRAMEBITS == 256
		// FPGA ������
		//#define DMABUFSTEP32RX	(WITHSAI1_FRAMEBITS / 32) //8
		SLOTEN_RX_SAI1 = 0x00FF,

		// �� �������� �� ���� ������� FPGA ������������ ���� � ��� �� ����
		// � ������ �������� ������ ������������ ������ ���� (������ 32 ���� ����� ������������ WS)
		#define DMABUFSTEP32TX	2		// 2 - ������� ������ ������������� ��� ����� � DMA ������	- I/Q
		SLOTEN_TX_SAI1 = 0x0011,	

	#elif WITHSAI1_FRAMEBITS == 64
		//#if WITHRTS96 || WITHUSEDUALWATCH
		//	#error WITHRTS96 or WITHUSEDUALWATCH unsupported with 64-bit frame length
		//#endif /* WITHRTS96 || WITHUSEDUALWATCH */
		//#define DMABUFSTEP32RX	(WITHSAI1_FRAMEBITS / 32) //2	// 2 - ������� ������ ������������� ��� ����� � DMA ������	- I/Q
		// ���������� �����
		SLOTEN_RX_SAI1 = 0x0003,
		SLOTEN_TX_SAI1 = 0x0003,
	#else
		#error Unexpected WITHSAI1_FRAMEBITS value
	#endif
#endif /* WITHSAI1HW */

#if WITHSAI2HW

	NBSLOT_SAI2 = WITHSAI2_FRAMEBITS / 32,
	// ���������� ����� � SLOTEN_TX_xxx � SLOTEN_RX_xxx ������ ���� �����
	// ��������� DMABUFSTEP32RX � DMABUFSTEP32TX ��������������.
	#if WITHSAI2_FRAMEBITS == 256
		// FPGA ������
		#if WITHUSEDUALWATCH
			SLOTEN_RX_SAI2 = 0x00FF,
		#else /* WITHUSEDUALWATCH */
			SLOTEN_RX_SAI2 = 0x00FF,
		#endif /* WITHUSEDUALWATCH */

		// �� �������� �� ���� ������� FPGA ������������ ���� � ��� �� ����
		// � ������ �������� ������ ������������ ������ ���� (������ 32 ���� ����� ������������ WS)
		SLOTEN_TX_SAI2 = 0x0011,

	#elif WITHSAI2_FRAMEBITS == 64
		// ���������� �����
		SLOTEN_RX_SAI2 = 0x0003,
		SLOTEN_TX_SAI2 = 0x0003,
	#else
		#error Unexpected WITHSAI2_FRAMEBITS value
	#endif
#endif /* WITHSAI2HW */
		enum_finish
};


//
#if WITHI2SCLOCKFROMPIN
	static uint_fast32_t 
	NOINLINEAT
	calcdivround_exti2s(
		uint_fast32_t freq		/* ��������� ������� �� ������ ��������, � ������. */
		)
	{
		return (EXTI2S_FREQ + freq / 2) / freq;
	}
#else /* WITHI2SCLOCKFROMPIN */
	static uint_fast32_t 
	NOINLINEAT
	calcdivround_plli2s(
		uint_fast32_t freq		/* ��������� ������� �� ������ ��������, � ������. */
		)
	{
		return (PLLI2S_FREQ_OUT + freq / 2) / freq;
	}
#endif /* WITHI2SCLOCKFROMPIN */

#if WITHI2SHW

// ���������� ���������� DMA �� ������ I2S - I2S2_EXT
// Use arm_hardware_invalidate
void RAMFUNC_NONILINE DMA1_Stream3_IRQHandler(void)
{
	// �������� ������� ����� ������������� ��� ���������� ����������� ������
	if ((DMA1->LISR & DMA_LISR_TCIF3) != 0)
	{
		const uint_fast8_t b = (DMA1_Stream3->CR & DMA_SxCR_CT) != 0;
		if (b != 0)
		{
			processing_dmabuffer16rx(DMA1_Stream3->M0AR);
			DMA1_Stream3->M0AR = dma_invalidate16rx(allocate_dmabuffer16());
		}
		else
		{
			processing_dmabuffer16rx(DMA1_Stream3->M1AR);
			DMA1_Stream3->M1AR = dma_invalidate16rx(allocate_dmabuffer16());
		}
		DMA1->LIFCR = DMA_LIFCR_CTCIF3;	// Clear TC interrupt flag
	}
}

// ���������� ���������� DMA �� ������ I2S - I2S3
// Use arm_hardware_invalidate
void RAMFUNC_NONILINE DMA1_Stream0_IRQHandler(void)
{
	if ((DMA1->LISR & DMA_LISR_TCIF0) != 0)
	{
		const uint_fast8_t b = (DMA1_Stream0->CR & DMA_SxCR_CT) != 0;
		if (b != 0)
		{
			processing_dmabuffer16rx(DMA1_Stream0->M0AR);
			DMA1_Stream0->M0AR = dma_invalidate16rx(allocate_dmabuffer16());
		}
		else
		{
			processing_dmabuffer16rx(DMA1_Stream0->M1AR);
			DMA1_Stream0->M1AR = dma_invalidate16rx(allocate_dmabuffer16());
		}
		DMA1->LIFCR = DMA_LIFCR_CTCIF0;	// Clear TC interrupt flag
	}
}

// ���������� ���������� DMA �� �������� I2S2
// Use arm_hardware_flush
void RAMFUNC_NONILINE DMA1_Stream4_IRQHandler(void)
{
	if ((DMA1->HISR & DMA_HISR_TCIF4) != 0)
	{
		const uint_fast8_t b = (DMA1_Stream4->CR & DMA_SxCR_CT) != 0;
		if (b != 0)
		{
			release_dmabuffer16(DMA1_Stream4->M0AR);
			DMA1_Stream4->M0AR = dma_flush16tx(getfilled_dmabuffer16phones());
		}
		else
		{
			release_dmabuffer16(DMA1_Stream4->M1AR);
			DMA1_Stream4->M1AR = dma_flush16tx(getfilled_dmabuffer16phones());
		}
		DMA1->HIFCR = DMA_HIFCR_CTCIF4;	// Clear TC interrupt flag ��������������� stream
	}
}

// ������������� DMA �� �������� I2S2
// Use arm_hardware_flush
static void 
DMA_I2S2_TX_initialize(void)
{
	/* SPI2_TX - Stream4, Channel0 */ 
	/* DMA ��� �������� �� I2S2*/
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;//������� DMA1 
	__DSB();

#if CPUSTYLE_STM32H7XX
	// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
	// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
	enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
	DMAMUX1_Channel4->CCR = 40 * DMAMUX_CxCR_DMAREQ_ID_0;	// SPI2_TX
	DMA1_Stream4->PAR = (uint32_t) & SPI2->TXDR;
#else /* CPUSTYLE_STM32H7XX */
	const uint_fast8_t ch = 0;
	DMA1_Stream4->PAR = (uint32_t) & SPI2->DR;
#endif /* CPUSTYLE_STM32H7XX */

    DMA1_Stream4->M0AR = dma_flush16tx(allocate_dmabuffer16());
    DMA1_Stream4->M1AR = dma_flush16tx(allocate_dmabuffer16());
	DMA1_Stream4->NDTR = (DMA1_Stream4->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE16 * DMA_SxNDT_0);

	DMA1_Stream4->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA1_Stream4->CR =
		ch * DMA_SxCR_CHSEL_0 | // �����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		1 * DMA_SxCR_DIR_0 |	// ����������� - ������ - ���������
		1 * DMA_SxCR_MINC |		// ��������� ������
		1 * DMA_SxCR_MSIZE_0 |	// ����� � ������ - 16�
		1 * DMA_SxCR_PSIZE_0 |	// ����� � SPI_DR- 16�
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT |		// M0AR selected
		1 * DMA_SxCR_DBM |		// double buffer mode seelcted
		0;

	DMA1->HIFCR = DMA_HISR_TCIF4;	// Clear TC interrupt flag ��������������� stream
	DMA1_Stream4->CR |= DMA_SxCR_TCIE;	// ��������� ���������� �� DMA

	NVIC_SetPriority(DMA1_Stream4_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA1_Stream4_IRQn);	// DMA1_Stream4_IRQHandler() enable
	DMA1_Stream4->CR |= DMA_SxCR_EN;
}

#if defined (I2S2ext)
/* ������������� DMA ��� ���� �� I2S2ext*/
// Use arm_hardware_invalidate
static void 
DMA_I2S2ext_rx_init(void)
{
	const uint_fast8_t ch = 3;
	/* I2S2_EXT_RX - Stream3, Channel3 */ 
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;//������� DMA1 
	__DSB();

	DMA1_Stream3->PAR = (uint32_t) & I2S2ext->DR;
    DMA1_Stream3->M0AR = dma_invalidate16rx(allocate_dmabuffer16());
    DMA1_Stream3->M1AR = dma_invalidate16rx(allocate_dmabuffer16());
	DMA1_Stream3->NDTR = (DMA1_Stream3->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE16 * DMA_SxNDT_0);

	DMA1_Stream3->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA1_Stream3->CR =
		ch * DMA_SxCR_CHSEL_0 | // �����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_DIR_0 |	// 00: Peripheral-to-memory
		1 * DMA_SxCR_MINC |		//��������� ������
		1 * DMA_SxCR_MSIZE_0 |	//����� � ������ - 16�
		1 * DMA_SxCR_PSIZE_0 |	//����� � SPI_DR- 16�
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT |		// M0AR selected
		1 * DMA_SxCR_DBM |		// double buffer mode seelcted
		0;

	DMA1->LIFCR = DMA_LISR_TCIF3;	// Clear TC interrupt flag
	DMA1_Stream3->CR |= DMA_SxCR_TCIE;	// ��������� ���������� �� DMA

	NVIC_SetPriority(DMA1_Stream3_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA1_Stream3_IRQn);	// DMA1_Stream3_IRQHandler() enable

	DMA1_Stream3->CR |= DMA_SxCR_EN;
}

#endif /* defined (I2S2ext) */

/* ������������� DMA ��� ���� �� I2S3 */
// Use arm_hardware_invalidate
static void 
DMA_I2S3_RX_initialize(void)
{
	/* I2S3_RX - DMA1, Stream0, Channel0 */ 
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;	//������� DMA1 
	__DSB();

#if CPUSTYLE_STM32H7XX
	// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
	// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
	enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
	DMAMUX1_Channel0->CCR = 61 * DMAMUX_CxCR_DMAREQ_ID_0;	// SPI3_RX
	DMA1_Stream0->PAR = (uint32_t) & SPI3->RXDR;
#else /* CPUSTYLE_STM32H7XX */
	const uint_fast8_t ch = 0;
	DMA1_Stream0->PAR = (uint32_t) & SPI3->DR;
#endif /* CPUSTYLE_STM32H7XX */

    DMA1_Stream0->M0AR = dma_invalidate16rx(allocate_dmabuffer16());
    DMA1_Stream0->M1AR = dma_invalidate16rx(allocate_dmabuffer16());
	DMA1_Stream0->NDTR = (DMA1_Stream0->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE16 * DMA_SxNDT_0);

	DMA1_Stream0->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA1_Stream0->CR =
		ch * DMA_SxCR_CHSEL_0 | // �����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_DIR_0 |	// 00: Peripheral-to-memory
		1 * DMA_SxCR_MINC |		//��������� ������
		1 * DMA_SxCR_MSIZE_0 |	//����� � ������ - 16�
		1 * DMA_SxCR_PSIZE_0 |	//����� � SPI_DR- 16�
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT |		// M0AR selected
		1 * DMA_SxCR_DBM |		// double buffer mode seelcted
		0;

	DMA1->LIFCR = DMA_LISR_TCIF0;	// Clear TC interrupt flag
	DMA1_Stream0->CR |= DMA_SxCR_TCIE;	// ��������� ���������� �� DMA

	NVIC_SetPriority(DMA1_Stream0_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA1_Stream0_IRQn);	// DMA1_Stream0_IRQHandler() enable

	DMA1_Stream0->CR |= DMA_SxCR_EN;
}

#if defined (I2S2ext)
// ��������� � �� ������
static void 
hardware_i2s2_master_fullduplex_initialize(void)		/* ������������� I2S �� STM32F429 */
{
	// SPI2 - master transmit
	// I2S2ext - slave receive

#if WITHI2SCLOCKFROMPIN
	// �������� ������� �� SPI2 (I2S) �������� � �������� ����������, � ��������� �������� ����� MCK ������ ����������
	#if defined (STM32F446xx)
		//RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_I2S2SRC)) |
		//	1 * RCC_DCKCFGR_I2S2SRC_0 |	// 01: I2S APB2 clock frequency = I2S_CKIN Alternate function input frequency
		//	0;
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_I2S1SRC)) |
			1 * RCC_DCKCFGR_I2S1SRC_0 |	// 01: I2S APB1 clock frequency = I2S_CKIN Alternate function input frequency
			0;
	#else /* defined (STM32F446xx) */
		RCC->CFGR |= RCC_CFGR_I2SSRC;	// �� STM32F446xx ����� ���� ���
	#endif /* defined (STM32F446xx) */

	//arm_hardware_pioc_altfn50(1UL << 9, AF_SPI2);	// PC9 - MCLK source - I2S_CKIN signal - ���������� � �����������-��������� header.
#else /* WITHI2SCLOCKFROMPIN */

	// �������� ������������ ������ ����� � MCLK=256*Fs
	//
	// MCK: Master Clock (mapped separately) is used, when the I2S is configured in master
	// mode (and when the MCKOE bit in the SPI_I2SPR register is set), to output this
	// additional clock generated at a preconfigured frequency rate equal to 256 * FS, where
	// FS is the audio sampling frequency.

	#if defined (STM32F446xx)
		#error Reviev STM32F446xx registers programming
	#else /* defined (STM32F446xx) */
		// ������� ��������� �� �� �����, ��� � � �������� PLL
		// PLLI2SR (at 28) = output divider of VCO frequency
		RCC->PLLI2SCFGR = (RCC->PLLI2SCFGR & ~ (RCC_PLLI2SCFGR_PLLI2SN | RCC_PLLI2SCFGR_PLLI2SR)) |
			((PLLI2SN_MUL << RCC_PLLI2SCFGR_PLLI2SN_Pos) & RCC_PLLI2SCFGR_PLLI2SN) |	// PLLI2SN bits = multiplier, freq=192..432 MHz, vale = 2..432
			2 * RCC_PLLI2SCFGR_PLLI2SR_0 |		// PLLI2SR bits - output divider, 2..7 - ��������� � calcdivround_plli2s().
			0;
			
		RCC->CR |= RCC_CR_PLLI2SON;				// ������� PLL
		while ((RCC->CR & RCC_CR_PLLI2SRDY) == 0)	// ���� ���������� PLL
			;
	#endif /* defined (STM32F446xx) */

#endif /* WITHI2SCLOCKFROMPIN */

	// ������ �������� ������ SPI.
#if CPUSTYLE_STM32H7XX
	RCC->APB1LENR |= RCC_APB1LENR_SPI2EN; // ������ ������������   
	__DSB();
#else /* CPUSTYLE_STM32H7XX */
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // ������ ������������   
	__DSB();
#endif /* CPUSTYLE_STM32H7XX */
	        
	const portholder_t i2smode = 
		SPI_I2SCFGR_I2SMOD | 
		0 * SPI_I2SCFGR_CHLEN |		// 0: 16-bit wide audio channel
		0 * SPI_I2SCFGR_DATLEN_0 |	// 00: 16-bit data length, 01: 24-bit data length, 10: 32-bit data length
#if WITHI2S_FORMATI2S_PHILIPS
		0 * SPI_I2SCFGR_I2SSTD_0 |	// 00: I2S Phillips standard, 01: MSB justified standard (left justified)
#else /* WITHI2S_FORMATI2S_PHILIPS */
		1 * SPI_I2SCFGR_I2SSTD_0 |	// 00: I2S Phillips standard, 01: MSB justified standard (left justified)
#endif /* WITHI2S_FORMATI2S_PHILIPS */
		//1 * SPI_I2SCFGR_CKPOL |		// �� ��� H7: �� ����� ���� �� �������: ������ �� ������ �������� �� ���������� ������
		0;

 	I2S2ext->I2SCFGR = i2smode | 1 * SPI_I2SCFGR_I2SCFG_0;	// 10: Master - transmit, 11: Master - receive, 01: Slave - receive
 	SPI2->I2SCFGR = i2smode | 2 * SPI_I2SCFGR_I2SCFG_0; // 10: Master - transmit, 11: Master - receive
#if WITHI2SCLOCKFROMPIN
	const portholder_t i2sdivider = calcdivround_exti2s(ARMI2SMCLK);
#else /* WITHI2SCLOCKFROMPIN */
	const portholder_t i2sdivider = calcdivround_plli2s(ARMI2SMCLK);
#endif /* WITHI2SCLOCKFROMPIN */
	const portholder_t i2soddv = (i2sdivider % 2) != 0;
	const portholder_t i2sdiv = i2sdivider / 2;

	// note: SPI_I2SPR_I2SDIV should be >= 2
#if WITHI2SCLOCKFROMPIN
	debug_printf_P(PSTR("hardware_i2s2_master_fullduplex_initialize: 1 I2S i2sdivider=%lu, ARMI2SMCLK=%lu, EXTI2S_FREQ=%lu\n"), (unsigned long) calcdivround_exti2s(ARMI2SMCLK), (unsigned long) ARMI2SMCLK, (unsigned long) EXTI2S_FREQ);
#else /* WITHI2SCLOCKFROMPIN */
	debug_printf_P(PSTR("hardware_i2s2_master_fullduplex_initialize: 2 I2S i2sdivider=%lu, ARMI2SMCLK=%lu, PLLI2S_FREQ_OUT=%lu\n"), (unsigned long) calcdivround_plli2s(ARMI2SMCLK), (unsigned long) ARMI2SMCLK, (unsigned long) PLLI2S_FREQ_OUT);
#endif /* WITHI2SCLOCKFROMPIN */

	const portholder_t i2spr = 
		((i2sdiv << SPI_I2SPR_I2SDIV_Pos) & SPI_I2SPR_I2SDIV) | 
		(SPI_I2SPR_ODD * i2soddv) | 
		SPI_I2SPR_MCKOE |
		0;
	I2S2ext->I2SPR = i2spr;
	SPI2->I2SPR = i2spr;
	// ���������� I2S � ������� ����������
	I2S2HW_INITIALIZE();
}

#endif /* defined (I2S2ext) */

#if WITHI2SHWTXSLAVE

// ��������� � �� ������
static void 
hardware_i2s2_slave_tx_initialize(void)		/* ������������� I2S2, STM32F4xx */
{
#if CPUSTYLE_STM32H7XX
	RCC->APB1LENR |= RCC_APB1LENR_SPI2EN; // ������ ������������   
	__DSB();
#else /* CPUSTYLE_STM32H7XX */
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // ������ ������������   
	__DSB();
#endif /* CPUSTYLE_STM32H7XX */
	        
	const portholder_t i2smode = 
		SPI_I2SCFGR_I2SMOD | 
		0 * SPI_I2SCFGR_CHLEN |		// 0: 16-bit wide audio channel
		0 * SPI_I2SCFGR_DATLEN_0 |	// 00: 16-bit data length, 01: 24-bit data length, 10: 32-bit data length
#if WITHI2S_FORMATI2S_PHILIPS
		0 * SPI_I2SCFGR_I2SSTD_0 |	// 00: I2S Phillips standard, 01: MSB justified standard (left justified)
#else /* WITHI2S_FORMATI2S_PHILIPS */
		1 * SPI_I2SCFGR_I2SSTD_0 |	// 00: I2S Phillips standard, 01: MSB justified standard (left justified)
#endif /* WITHI2S_FORMATI2S_PHILIPS */
		//1 * SPI_I2SCFGR_CKPOL |		// �� ��� H7: �� ����� ���� �� �������: ������ �� ������ �������� �� ���������� ������
		0;

 	SPI2->I2SCFGR = i2smode | 0 * SPI_I2SCFGR_I2SCFG_0; // 00: Slave - transmit
#if CPUSTYLE_STM32H7XX
	SPI2->CFG2 |= SPI_CFG2_IOSWP;
#endif /* CPUSTYLE_STM32H7XX */

	// ���������� I2S � ������� ����������
	I2S2HW_INITIALIZE();
}

#else /* WITHI2SHWTXSLAVE */

// ��������� � �� ������
static void 
hardware_i2s2_master_tx_initialize(void)		/* ������������� I2S2, STM32F4xx */
{
#if WITHI2SCLOCKFROMPIN
	// �������� ������� �� SPI2 (I2S) �������� � �������� ����������, � ��������� �������� ����� MCK ������ ����������
	#if defined (STM32F446xx)
		//RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_I2S2SRC)) |
		//	1 * RCC_DCKCFGR_I2S2SRC_0 |	// 01: I2S APB2 clock frequency = I2S_CKIN Alternate function input frequency
		//	0;
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_I2S1SRC)) |
			1 * RCC_DCKCFGR_I2S1SRC_0 |	// 01: I2S APB1 clock frequency = I2S_CKIN Alternate function input frequency
			0;
	#else /* defined (STM32F446xx) */
		RCC->CFGR |= RCC_CFGR_I2SSRC;
	#endif /* defined (STM32F446xx) */
	//arm_hardware_pioc_altfn50(1UL << 9, AF_SPI2);	// PC9 - MCLK source - I2S_CKIN signal - ���������� � �����������-��������� header.
#else /* WITHI2SCLOCKFROMPIN */

	// �������� ������������ ������ ����� � MCLK=256*Fs
	#if defined (STM32F446xx)
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_I2S2SRC)) |
			0 * RCC_DCKCFGR_I2S2SRC_0 |		 // 00: I2S2 clock frequency = f(PLLI2S_R)
			//1 * RCC_DCKCFGR_I2S2SRC_0 |	 // 01: I2S2 clock frequency = I2S_CKIN Alternate function input frequency
			//2 * RCC_DCKCFGR_I2S2SRC_0 |	 // 10: I2S2 clock frequency = HSI/HSE depends on PLLSRC bit (PLLCFGR[22])
			0;
		// ������� ��������� �� �� �����, ��� � � �������� PLL
		// PLLI2SR (at 28) = output divider of VCO frequency
		RCC->PLLI2SCFGR = (RCC->PLLI2SCFGR & ~ (RCC_PLLI2SCFGR_PLLI2SN | RCC_PLLI2SCFGR_PLLI2SR)) |
			((PLLI2SN_MUL << RCC_PLLI2SCFGR_PLLI2SN_Pos) & RCC_PLLI2SCFGR_PLLI2SN) |	// PLLI2SN bits = multiplier, freq=192..432 MHz, vale = 2..432
			2 * RCC_PLLI2SCFGR_PLLI2SR_0 |		// PLLI2SR bits - output divider, 2..7 - ��������� � calcdivround_plli2s().
			0;
			
		RCC->CR |= RCC_CR_PLLI2SON;				// ������� PLL
		while ((RCC->CR & RCC_CR_PLLI2SRDY) == 0)	// ���� ���������� PLL
			;
	#else /* defined (STM32F446xx) */
		//
		// MCK: Master Clock (mapped separately) is used, when the I2S is configured in master
		// mode (and when the MCKOE bit in the SPI_I2SPR register is set), to output this
		// additional clock generated at a preconfigured frequency rate equal to 256 * FS, where
		// FS is the audio sampling frequency.


		// ������� ��������� �� �� �����, ��� � � �������� PLL
		// PLLI2SR (at 28) = output divider of VCO frequency
		RCC->PLLI2SCFGR = (RCC->PLLI2SCFGR & ~ (RCC_PLLI2SCFGR_PLLI2SN | RCC_PLLI2SCFGR_PLLI2SR)) |
			((PLLI2SN_MUL << RCC_PLLI2SCFGR_PLLI2SN_Pos) & RCC_PLLI2SCFGR_PLLI2SN) |	// PLLI2SN bits = multiplier, freq=192..432 MHz, vale = 2..432
			2 * RCC_PLLI2SCFGR_PLLI2SR_0 |		// PLLI2SR bits - output divider, 2..7 - ��������� � calcdivround_plli2s().
			0;
			
		RCC->CR |= RCC_CR_PLLI2SON;				// ������� PLL
		while ((RCC->CR & RCC_CR_PLLI2SRDY) == 0)	// ���� ���������� PLL
			;
	#endif /* defined (STM32F446xx) */

#endif /* WITHI2SCLOCKFROMPIN */

	// ������ �������� ������ SPI.
#if CPUSTYLE_STM32H7XX
	RCC->APB1LENR |= RCC_APB1LENR_SPI2EN; // ������ ������������   
	__DSB();
#else /* CPUSTYLE_STM32H7XX */
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // ������ ������������   
	__DSB();
#endif /* CPUSTYLE_STM32H7XX */
	        
	const portholder_t i2smode = 
		SPI_I2SCFGR_I2SMOD | 
		0 * SPI_I2SCFGR_CHLEN |		// 0: 16-bit wide audio channel
		0 * SPI_I2SCFGR_DATLEN_0 |	// 00: 16-bit data length, 01: 24-bit data length, 10: 32-bit data length
#if WITHI2S_FORMATI2S_PHILIPS
		0 * SPI_I2SCFGR_I2SSTD_0 |	// 00: I2S Phillips standard, 01: MSB justified standard (left justified)
#else /* WITHI2S_FORMATI2S_PHILIPS */
		1 * SPI_I2SCFGR_I2SSTD_0 |	// 00: I2S Phillips standard, 01: MSB justified standard (left justified)
#endif /* WITHI2S_FORMATI2S_PHILIPS */
		//1 * SPI_I2SCFGR_CKPOL |		// �� ��� H7: �� ����� ���� �� �������: ������ �� ������ �������� �� ���������� ������
		0;

 	SPI2->I2SCFGR = i2smode | 2 * SPI_I2SCFGR_I2SCFG_0; // 10: Master - transmit, 11: Master - receive
#if WITHI2SCLOCKFROMPIN
	const portholder_t i2sdivider = calcdivround_exti2s(ARMI2SMCLK);
#else /* WITHI2SCLOCKFROMPIN */
	const portholder_t i2sdivider = calcdivround_plli2s(ARMI2SMCLK);
#endif /* WITHI2SCLOCKFROMPIN */
	const portholder_t i2soddv = (i2sdivider % 2) != 0;
	const portholder_t i2sdiv = i2sdivider / 2;

	// note: SPI_I2SPR_I2SDIV should be >= 2
#if WITHI2SCLOCKFROMPIN
	debug_printf_P(PSTR("hardware_i2s2_master_tx_initialize: 1 I2S i2sdivider=%lu, ARMI2SMCLK=%lu, EXTI2S_FREQ=%lu\n"), (unsigned long) calcdivround_exti2s(ARMI2SMCLK), (unsigned long) ARMI2SMCLK, (unsigned long) EXTI2S_FREQ);
#else /* WITHI2SCLOCKFROMPIN */
	debug_printf_P(PSTR("hardware_i2s2_master_tx_initialize: 2 I2S i2sdivider=%lu, ARMI2SMCLK=%lu, PLLI2S_FREQ_OUT=%lu\n"), (unsigned long) calcdivround_plli2s(ARMI2SMCLK), (unsigned long) ARMI2SMCLK, (unsigned long) PLLI2S_FREQ_OUT);
#endif /* WITHI2SCLOCKFROMPIN */

	const portholder_t i2spr = 
		((i2sdiv << SPI_I2SPR_I2SDIV_Pos) & SPI_I2SPR_I2SDIV) | 
		(SPI_I2SPR_ODD * i2soddv) | 
		SPI_I2SPR_MCKOE |
		0;
	SPI2->I2SPR = i2spr;
	// ���������� I2S � ������� ����������
	I2S2HW_INITIALIZE();
}

#endif /* WITHI2SHWTXSLAVE */

#if WITHI2SHWRXSLAVE

// ��������� � �� ������
static void 
hardware_i2s3_slave_rx_initialize(void)		/* ������������� I2S3 STM32F4xx */
{
	debug_printf_P(PSTR("hardware_i2s3_slave_rx_initialize\n"));
#if CPUSTYLE_STM32H7XX
	RCC->APB1LENR |= RCC_APB1LENR_SPI3EN; // ������ ������������   
	__DSB();
#else /* CPUSTYLE_STM32H7XX */
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN; // ������ ������������   
	__DSB();
#endif /* CPUSTYLE_STM32H7XX */
	        
	const portholder_t i2smode = 
		SPI_I2SCFGR_I2SMOD | 
		0 * SPI_I2SCFGR_CHLEN |		// 0: 16-bit wide audio channel
		0 * SPI_I2SCFGR_DATLEN_0 |	// 00: 16-bit data length, 01: 24-bit data length, 10: 32-bit data length
#if WITHI2S_FORMATI2S_PHILIPS
		0 * SPI_I2SCFGR_I2SSTD_0 |	// 00: I2S Phillips standard, 01: MSB justified standard (left justified)
#else /* WITHI2S_FORMATI2S_PHILIPS */
		1 * SPI_I2SCFGR_I2SSTD_0 |	// 00: I2S Phillips standard, 01: MSB justified standard (left justified)
#endif /* WITHI2S_FORMATI2S_PHILIPS */
		//1 * SPI_I2SCFGR_CKPOL |		// �� ��� H7: �� ����� ���� �� �������: ������ �� ������ �������� �� ���������� ������
		0;

 	SPI3->I2SCFGR = i2smode | 1 * SPI_I2SCFGR_I2SCFG_0;	// 10: Master - transmit, 11: Master - receive, 01: Slave - receive
#if CPUSTYLE_STM32H7XX
	//SPI3->CFG2 |= SPI_CFG2_IOSWP;
#endif /* CPUSTYLE_STM32H7XX */

	// ���������� I2S � ������� ����������
	I2S2HW_INITIALIZE();

	debug_printf_P(PSTR("hardware_i2s3_slave_rx_initialize done\n"));
}

#endif /* WITHI2SHWRXSLAVE */

#if defined (I2S2ext)

/* ���������� ������ I2S �� STM32F4xx */
// ��������� � �� ������
static void 
hardware_i2s2_fullduplex_enable(void)
{


#if CPUSTYLE_STM32H7XX

	I2S2ext->CFG1 |= SPI_CFG1_RXDMAEN; // DMA �� ������ (slave)
	SPI2->CFG1 |= SPI_CFG1_TXDMAEN; // DMA �� ��������
	I2S2ext->I2SCFGR |= SPI_I2SCFGR_I2SE;		// I2S enable (slave enabled first)
	I2S2ext->CR1 |= SPI_CR1_CSTART;	// I2S run
	__DSB();
	SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;		// I2S enable
	__DSB();
	SPI2->CR1 |= SPI_CR1_CSTART;	// I2S run

#else /* CPUSTYLE_STM32H7XX */

	I2S2ext->CR2 |= SPI_CR2_RXDMAEN; // DMA �� ������ (slave)
	SPI2->CR2 |= SPI_CR2_TXDMAEN; // DMA �� ��������
	I2S2ext->I2SCFGR |= SPI_I2SCFGR_I2SE;		// I2S enable (slave enabled first)
	__DSB();
	SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;		// I2S enable
	__DSB();

#endif /* CPUSTYLE_STM32H7XX */
}

#endif /* defined (I2S2ext) */

/* ���������� ������ I2S �� STM32F4xx */
// ��������� � �� ������
static void 
hardware_i2s2_tx_enable(void)
{
#if CPUSTYLE_STM32H7XX
	SPI2->CFG1 |= SPI_CFG1_TXDMAEN; // DMA �� ��������
	SPI2->CR1 |= SPI_CR1_SPE;		// I2S enable
	SPI2->CR1 |= SPI_CR1_CSTART;	// I2S run
	__DSB();
#else /* CPUSTYLE_STM32H7XX */
	SPI2->CR2 |= SPI_CR2_TXDMAEN; // DMA �� ��������
	SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;		// I2S enable
	__DSB();
#endif /* CPUSTYLE_STM32H7XX */
}

/* ���������� ������ I2S �� STM32F4xx */
// ��������� � �� ������
static void 
hardware_i2s3_rx_enable(void)
{
#if CPUSTYLE_STM32H7XX

	SPI3->CFG1 |= SPI_CFG1_RXDMAEN; // DMA �� �����
	SPI3->CR1 |= SPI_CR1_SPE;		// I2S enable
	SPI3->CR1 |= SPI_CR1_CSTART;	// I2S run
	__DSB();

#else /* CPUSTYLE_STM32H7XX */

	SPI3->CR2 |= SPI_CR2_RXDMAEN; // DMA �� �����
	SPI3->I2SCFGR |= SPI_I2SCFGR_I2SE;		// I2S enable
	__DSB();

#endif /* CPUSTYLE_STM32H7XX */
}



#if WITHI2SFULLDUPLEXHW
// �����, ��� ������������ DMA_I2S2 � DMA_I2S2ext
static const codechw_t audiocodechw =
{
	hardware_i2s2_master_fullduplex_initialize,
	hardware_dummy_initialize,
	DMA_I2S2ext_rx_init,			// DMA �� ����� ����� 3
	DMA_I2S2_TX_initialize,				// DMA �� �������� ����� 0
	hardware_i2s2_fullduplex_enable,
	hardware_dummy_enable,
	"i2s2-audiocodechw"
};

#else /* WITHI2SFULLDUPLEXHW */

static const codechw_t audiocodechw =
{
	#if WITHI2SHWRXSLAVE
		hardware_i2s3_slave_rx_initialize,	/* ��������� � �� ������ - �������� */
	#else /* WITHI2SHWRXSLAVE */
		hardware_dummy_initialize,			/* ��������� � �� ������ - �������� */
	#endif /* WITHI2SHWRXSLAVE */
	#if WITHI2SHWTXSLAVE
		hardware_i2s2_slave_tx_initialize,	/* ��������� � �� ������ - �������� */
	#else /* WITHI2SHWTXSLAVE */
		hardware_i2s2_master_tx_initialize,	/* ��������� � �� ������ - �������� */
	#endif /* WITHI2SHWTXSLAVE */
	DMA_I2S3_RX_initialize,					// DMA �� ����� SPI3_RX - DMA1, Stream0, Channel0
	DMA_I2S2_TX_initialize,					// DMA �� �������� ����� 0
	hardware_i2s3_rx_enable,
	hardware_i2s2_tx_enable,
	"i2s2-i2s3-audiocodechw"
};

#endif /* WITHI2SFULLDUPLEXHW */


#endif /* WITHI2SHW */

#if WITHSAI1HW || WITHSAI2HW

	#if WITHSAICLOCKFROMPIN
		static uint_fast32_t 
		NOINLINEAT
		calcdivround_extsai(
			uint_fast32_t freq		/* ��������� ������� �� ������ ��������, � ������. */
			)
		{
			return (EXTSAI_FREQ + freq / 2) / freq;
		}
	#elif WITHSAICLOCKFROMI2S

	#else
		static uint_fast32_t 
		NOINLINEAT
		calcdivround_pllsai(
			uint_fast32_t freq		/* ��������� ������� �� ������ ��������, � ������. */
			)
		{
			return (PLLSAI_FREQ_OUT + freq / 2) / freq;
		}
	#endif


	/*
		RM0090, SAI xConfiguration register 1 (SAI_xCR1) (chapter 29.17.1).
		MCKDIV[3:0] value description:

		0000: Divides by 1 the master clock input.
		Otherwise, The Master clock frequency is calculated accordingly to the following formula:
		MCLK_x = SAI_CK_x / (MCKDIV[3:0] * 2)
	*/
	static uint32_t mckdivform(uint32_t divider)
	{
		if (divider == 1)
			return 0;
		return divider / 2;
	}


static void hardware_sai1_sai2_clock_selection(void)
{
	debug_printf_P(PSTR("hardware_sai1_sai2_clock_selection\n"));

#if WITHSAICLOCKFROMPIN
	
	// �������� ������� �� SAI1 �������� � �������� ����������, � ��������� �������� 
	// ����� MCK ������ ����������.
	// SAI2 ���������������� �� SAI1

	#if defined (STM32F446xx)
		// clock sources:
		// RCC_DCKCFGR_SAI1SRC:
		// 0: SAI1 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI1 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI1 clock frequency = f(PLL_R)
		// 3: I2S_CKIN Alternate function input frequency
		// RCC_DCKCFGR_SAI2SRC
		// 0: SAI2 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI2 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI2 clock frequency = f(PLL_R)
		// 3: SAI2 clock frequency = HSI/HSE depends on PLLSRC (PLLCFGR[22])

		// SAI part of DCKCFGR
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_SAI1SRC | RCC_DCKCFGR_SAI2SRC)) |
			(3 * RCC_DCKCFGR_SAI1SRC_0) |	// I2S APB1 clock source selection
			(3 * RCC_DCKCFGR_SAI2SRC_0) |	// I2S APB2 clock source selection
			0;
	#elif CPUSTYLE_STM32F7XX
		//#warning TODO: implement for CPUSTYLE_STM32F7XX
	#elif CPUSTYLE_STM32H7XX
		//#warning TODO: implement for CPUSTYLE_STM32H7XX
	#else /* defined (STM32F446xx) */
		// clock sources:
		// 0: SAI1-B clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI1-B clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI1-B clock frequency = Alternate function input frequency

		// SAI part of DCKCFGR
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_SAI1ASRC | RCC_DCKCFGR_SAI1BSRC)) |
			(2 * RCC_DCKCFGR_SAI1ASRC_0) |	// SAI1-A clock source selection
			(2 * RCC_DCKCFGR_SAI1BSRC_0) |	// SAI1-B clock source selection
			0;
	#endif /* defined (STM32F446xx) */

#elif WITHSAICLOCKFROMI2S

	#if defined (STM32F446xx)
		// clock sources:
		// RCC_DCKCFGR_SAI1SRC:
		// 0: SAI1 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI1 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI1 clock frequency = f(PLL_R)
		// 3: I2S_CKIN Alternate function input frequency
		// RCC_DCKCFGR_SAI2SRC
		// 0: SAI2 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI2 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI2 clock frequency = f(PLL_R)
		// 3: SAI2 clock frequency = HSI/HSE depends on PLLSRC (PLLCFGR[22])

		//#error Reviev STM32F446xx registers programming
		// SAI part of DCKCFGR
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_SAI1SRC | RCC_DCKCFGR_SAI2SRC)) |
			(1 * RCC_DCKCFGR_SAI1SRC_0) |	// I2S APB1 clock source selection
			(1 * RCC_DCKCFGR_SAI2SRC_0) |	// I2S APB2 clock source selection
			0;

	#elif CPUSTYLE_STM32F7XX
		// clock sources:
		// RCC_DCKCFGR_SAI1SRC:
		// 0: SAI1 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI1 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI1 clock frequency = f(PLL_R)
		// 3: I2S_CKIN Alternate function input frequency
		// RCC_DCKCFGR_SAI2SRC
		// 0: SAI2 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI2 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI2 clock frequency = f(PLL_R)
		// 3: SAI2 clock frequency = HSI/HSE depends on PLLSRC (PLLCFGR[22])

		//#error Reviev STM32F446xx registers programming
		// SAI part of DCKCFGR
		RCC->DCKCFGR1 = (RCC->DCKCFGR1 & ~ (RCC_DCKCFGR1_SAI1SEL | RCC_DCKCFGR1_SAI2SEL)) |
			(1 * RCC_DCKCFGR1_SAI1SEL_0) |	// SAI1 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
			(1 * RCC_DCKCFGR1_SAI2SEL_0) |	// SAI2 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
			0;

	#else /* defined (STM32F446xx) */
		RCC->PLLI2SCFGR = 
			(RCC->PLLI2SCFGR & ~ (RCC_PLLI2SCFGR_PLLI2SQ)) |
			(2 * RCC_PLLI2SCFGR_PLLI2SQ_0) |	// PLLI2SQ: PLLI2S division factor for SAI1 clock - 2..7 - ��� ��������� ������������ � calcdivround_pllsai().
			0;
		// clock sources:
		// 0: SAI1-B clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI1-B clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI1-B clock frequency = Alternate function input frequency
		// SAI part of DCKCFGR
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_SAI1ASRC | RCC_DCKCFGR_SAI1BSRC)) |
			(1 * RCC_DCKCFGR_SAI1ASRC_0) |	// SAI1-A clock source selection
			(1 * RCC_DCKCFGR_SAI1BSRC_0) |	// SAI1-B clock source selection
			0;
	#endif /* defined (STM32F446xx) */

#else /* WITHSAICLOCKFROMI2S */
	// ��� SAI ������������ ��������� PLL
	// ������� ��������� �� �� �����, ��� � � �������� PLL
	// PLLSAIDIVQ
	#if defined (STM32F446xx)
		// clock sources:
		// RCC_DCKCFGR_SAI1SRC:
		// 0: SAI1 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI1 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI1 clock frequency = f(PLL_R)
		// 3: I2S_CKIN Alternate function input frequency
		// RCC_DCKCFGR_SAI2SRC
		// 0: SAI2 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI2 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI2 clock frequency = f(PLL_R)
		// 3: SAI2 clock frequency = HSI/HSE depends on PLLSRC (PLLCFGR[22])
		//#error Reviev STM32F446xx registers programming

		// SAI part of DCKCFGR
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_SAI1SRC | RCC_DCKCFGR_SAI2SRC)) |
			(0 * RCC_DCKCFGR_SAI1SRC_0) |	// I2S APB1 clock source selection
			(0 * RCC_DCKCFGR_SAI2SRC_0) |	// I2S APB2 clock source selection
			0;

	#elif CPUSTYLE_STM32F7XX
		// setup PLLSAI
		RCC->PLLSAICFGR = 
			(RCC->PLLSAICFGR & ~ (RCC_PLLSAICFGR_PLLSAIN | RCC_PLLSAICFGR_PLLSAIQ)) |
			((SAIREF1_MUL << RCC_PLLSAICFGR_PLLSAIN_Pos) & RCC_PLLSAICFGR_PLLSAIN) |	// PLLI2SN bits = multiplier, freq=192..432 MHz, vale = 2..432
			((2uL << RCC_PLLSAICFGR_PLLSAIQ_Pos) & RCC_PLLSAICFGR_PLLSAIQ) |	// PLLSAI division factor for SAI1 clock: 2..15 - ��� ��������� ������������ � calcdivround_pllsai().
			0;
		// clock sources:
		// RCC_DCKCFGR_SAI1SRC:
		// 0: SAI1 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI1 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI1 clock frequency = f(PLL_R)
		// 3: I2S_CKIN Alternate function input frequency
		// RCC_DCKCFGR_SAI2SRC
		// 0: SAI2 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI2 clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI2 clock frequency = f(PLL_R)
		// 3: SAI2 clock frequency = HSI/HSE depends on PLLSRC (PLLCFGR[22])
		//#error Reviev STM32F446xx registers programming

		// SAI part of DCKCFGR
		RCC->DCKCFGR1 = (RCC->DCKCFGR1 & ~ (RCC_DCKCFGR1_SAI1SEL | RCC_DCKCFGR1_SAI2SEL)) |
			(0 * RCC_DCKCFGR1_SAI1SEL_0) |	// SAI1 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ 
			(0 * RCC_DCKCFGR1_SAI2SEL_0) |	// SAI2 clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ 
			0;

		RCC->CR |= RCC_CR_PLLSAION;				// ������� PLL
		while ((RCC->CR & RCC_CR_PLLSAIRDY) == 0)	// ���� ���������� PLL
			;

	#else /* defined (STM32F446xx) */
		// setup PLLSAI
		RCC->PLLSAICFGR = 
			(RCC->PLLSAICFGR & ~ (RCC_PLLSAICFGR_PLLSAIN | RCC_PLLSAICFGR_PLLSAIQ)) |
			((SAIREF1_MUL << RCC_PLLSAICFGR_PLLSAIN_Pos) & RCC_PLLSAICFGR_PLLSAIN) |	// PLLI2SN bits = multiplier, freq=192..432 MHz, vale = 2..432
			((2uL << RCC_PLLSAICFGR_PLLSAIQ_Pos) & RCC_PLLSAICFGR_PLLSAIQ) |	// PLLSAI division factor for SAI1 clock: 2..15 - ��� ��������� ������������ � calcdivround_pllsai().
			0;
		// clock sources:
		// 0: SAI1-B clock frequency = f(PLLSAI_Q) / PLLSAIDIVQ
		// 1: SAI1-B clock frequency = f(PLLI2S_Q) / PLLI2SDIVQ
		// 2: SAI1-B clock frequency = Alternate function input frequency
		// SAI part of DCKCFGR
		RCC->DCKCFGR = (RCC->DCKCFGR & ~ (RCC_DCKCFGR_PLLSAIDIVQ | RCC_DCKCFGR_SAI1ASRC | RCC_DCKCFGR_SAI1BSRC)) |
			((0UL << 8) & RCC_DCKCFGR_PLLSAIDIVQ) |	// PLLSAI division factor for SAI1 clock 0..31, dividers = 1..32
			(0 * RCC_DCKCFGR_SAI1ASRC_0) |	// SAI1-A clock source selection
			(0 * RCC_DCKCFGR_SAI1BSRC_0) |	// SAI1-B clock source selection
			0;

		RCC->CR |= RCC_CR_PLLSAION;				// ������� PLL
		while ((RCC->CR & RCC_CR_PLLSAIRDY) == 0)	// ���� ���������� PLL
			;
	#endif /* defined (STM32F446xx) */
#endif /* WITHSAICLOCKFROMI2S */
	debug_printf_P(PSTR("hardware_sai1_sai2_clock_selection done\n"));
}

#endif /* WITHSAI1HW || WITHSAI2HW */

#if WITHSAI1HW


// DMA �� ������ SAI1 - ���������� ����������
// RX	SAI1_B	DMA2	Stream 5	Channel 0
// Use arm_hardware_invalidate

void RAMFUNC_NONILINE DMA2_Stream5_IRQHandler(void)
{
	// �������� ������� ����� ������������� ��� ���������� ����������� ������
	if ((DMA2->HISR & DMA_HISR_TCIF5) != 0)
	{
		const uint_fast8_t b = (DMA2_Stream5->CR & DMA_SxCR_CT) != 0;
		if (b != 0)
		{
			processing_dmabuffer32rx(DMA2_Stream5->M0AR);
			DMA2_Stream5->M0AR = dma_invalidate32rx(allocate_dmabuffer32rx());
		}
		else
		{
			processing_dmabuffer32rx(DMA2_Stream5->M1AR);
			DMA2_Stream5->M1AR = dma_invalidate32rx(allocate_dmabuffer32rx());
		}
		DMA2->HIFCR = DMA_HIFCR_CTCIF5;	// Clear TC interrupt flag ��������������� stream
	}
/*
	if ((DMA2->HISR & DMA_HISR_TEIF5) != 0)
	{
		DMA2->HIFCR = DMA_HIFCR_CTEIF5;	// Clear ERROR interrupt flag ��������������� stream
		DMA2_Stream5->CR &= ~ DMA_SxCR_EN;
		while (DMA2_Stream5->CR & DMA_SxCR_EN)
			;
		DMA2_Stream5->CR |= DMA_SxCR_EN;
	}
*/
}
// DMA �� �������� SAI1 - ���������� ����������
// TX	SAI1_A	DMA2	Stream 1	Channel 0
// Use arm_hardware_flush
void DMA2_Stream1_IRQHandler(void)
{
	if ((DMA2->LISR & DMA_LISR_TCIF1) != 0)
	{
		const uint_fast8_t b = (DMA2_Stream1->CR & DMA_SxCR_CT) != 0;
		if (b != 0)
		{
			release_dmabuffer32tx(DMA2_Stream1->M0AR);
			DMA2_Stream1->M0AR = dma_flush32tx(getfilled_dmabuffer32tx_main());
		}
		else
		{
			release_dmabuffer32tx(DMA2_Stream1->M1AR);
			DMA2_Stream1->M1AR = dma_flush32tx(getfilled_dmabuffer32tx_main());
		}
		DMA2->LIFCR = DMA_LIFCR_CTCIF1;	// Clear TC interrupt flag
	}
}


// DMA �� �������� SAI1 - �������������
// TX	SAI1_A	DMA2	Stream 1	Channel 0
// Use arm_hardware_flush
static void DMA_SAI1_A_TX_initialize(void)
{
	/* SAI1_A - Stream1, Channel0 */ 
	/* DMA ��� �������� �� I2S2*/
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//������� DMA2 
	__DSB();

#if CPUSTYLE_STM32H7XX
	// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
	// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
	enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
	DMAMUX1_Channel9->CCR = 87 * DMAMUX_CxCR_DMAREQ_ID_0;	// SAI1_A
#else /* CPUSTYLE_STM32H7XX */
	const uint_fast8_t ch = 0;
#endif /* CPUSTYLE_STM32H7XX */

	DMA2_Stream1->PAR = (uint32_t) & SAI1_Block_A->DR;
	DMA2_Stream1->M0AR = dma_flush32tx(allocate_dmabuffer32tx());
	DMA2_Stream1->M1AR = dma_flush32tx(allocate_dmabuffer32tx());
	DMA2_Stream1->NDTR = (DMA2_Stream1->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE32TX * DMA_SxNDT_0);

	DMA2_Stream1->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA2_Stream1->CR =
		ch * DMA_SxCR_CHSEL_0 | //�����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		1 * DMA_SxCR_DIR_0 | //����������� - ������ - ���������
		1 * DMA_SxCR_MINC | //��������� ������
		2 * DMA_SxCR_MSIZE_0 | //����� � ������ - 32 bit
		2 * DMA_SxCR_PSIZE_0 | //����� � DR - 32 bit
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT | // M0AR selected
		1 * DMA_SxCR_DBM | // double buffer mode seelcted
		0;

	DMA2->LIFCR = DMA_LISR_TCIF1;	// Clear TC interrupt flag
	DMA2_Stream1->CR |= DMA_SxCR_TCIE;	// ��������� ���������� �� DMA

	NVIC_SetPriority(DMA2_Stream1_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA2_Stream1_IRQn);	// DMA2_Stream1_IRQHandler() enable

	DMA2_Stream1->CR |= DMA_SxCR_EN;

}

/* DMA ��� ���� �� SAI_1_B  - ������������� */
// RX	SAI1_B	DMA2	Stream 5	Channel 0
// Use arm_hardware_invalidate
static void DMA_SAI1_B_RX_initialize(void)
{
	/* SAI1_B - Stream5, Channel0 */ 
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//������� DMA2 
	__DSB();

#if CPUSTYLE_STM32H7XX
	enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
	//const uint_fast8_t muxi = 88;	// SAI1_B
	//stm32h7xx_dma2mux(muxi, 0x05);
	// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
	// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
	DMAMUX1_Channel13->CCR = 88 * DMAMUX_CxCR_DMAREQ_ID_0;
#else /* CPUSTYLE_STM32H7XX */
	const uint_fast8_t ch = 0;
#endif /* CPUSTYLE_STM32H7XX */

	DMA2_Stream5->PAR = (uint32_t) & SAI1_Block_B->DR;
	DMA2_Stream5->M0AR = dma_invalidate32rx(allocate_dmabuffer32rx());
	DMA2_Stream5->M1AR = dma_invalidate32rx(allocate_dmabuffer32rx());
	DMA2_Stream5->NDTR = (DMA2_Stream5->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE32RX * DMA_SxNDT_0);

	DMA2_Stream5->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA2_Stream5->CR =
		ch * DMA_SxCR_CHSEL_0 | // �����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_DIR_0 |	// 00: Peripheral-to-memory
		1 * DMA_SxCR_MINC |		//��������� ������
		2 * DMA_SxCR_MSIZE_0 | //����� � ������ - 32 bit
		2 * DMA_SxCR_PSIZE_0 | //����� � DR - 32 bit
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT |	// M0AR selected
		1 * DMA_SxCR_DBM |	 // double buffer mode seelcted
		0;

	DMA2->HIFCR = (DMA_HIFCR_CTCIF5 /* | DMA_HIFCR_CTEIF5 */);	// Clear TC interrupt flag ��������������� stream
	DMA2_Stream5->CR |= (DMA_SxCR_TCIE /* | DMA_SxCR_TEIE */);	// ���������� �� DMA �� TC � TE

	NVIC_SetPriority(DMA2_Stream5_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA2_Stream5_IRQn);	// DMA2_Stream5_IRQHandler() enable

	DMA2_Stream5->CR |= DMA_SxCR_EN;
}

#if WITHSAI1HWTXRXMASTER

static void hardware_sai1_master_fullduplex_initialize(void)		/* ������������� SAI1 �� STM32F4xx */
{
	hardware_sai1_sai2_clock_selection();

	// ������ �������� ������ SAI.
	RCC->APB2ENR |= RCC_APB2ENR_SAI1EN; //������ ������������ 
	__DSB();
	

	SAI1_Block_A->CR1 &= ~ SAI_xCR1_SAIEN;
	SAI1_Block_B->CR1 &= ~ SAI_xCR1_SAIEN;

	// extclock = 24.576 MHz
	// if SAI_xCR1_NODIV == 1, ws=384 kHz
	//const uint32_t mckval = 0;	// divider = 1, ws = 96 kHz
	//const uint32_t mckval = 1;	// divider = 2, ws = 48 kHz
	//const uint32_t mckval = 2;	// divider = 4, ws = 24 kHz
	//const uint32_t mckval = 3;	// divider = 6, ws = 16 kHz
	//const uint32_t mckval = 4;	// divider = 8, ws = 12 kHz

	// SAI_xCR1_CKSTR:
	// 0: Signals generated by the SAI change on SCK rising edge, 
	//		while signals received by the SAI are sampled on the SCK falling edge.
	// 1: Signals generated by the SAI change on SCK falling edge, 
	//		while signals received by the SAI are sampled on the SCK rising edge.

	const uint_fast8_t nodiv = 0;
#if WITHSAICLOCKFROMPIN
	const uint_fast8_t mckdiv = mckdivform(calcdivround_extsai(ARMSAIMCLK));
#elif WITHSAICLOCKFROMI2S
	const uint_fast8_t mckdiv = mckdivform(calcdivround_plli2s(ARMSAIMCLK));
#else
		// from own PLL
	const uint_fast8_t mckdiv = mckdivform(calcdivround_pllsai(ARMSAIMCLK));
#endif
		
	// CR1 value
	const portholder_t commoncr1 =
		(1 * SAI_xCR1_OUTDRIV) |	//
		(0 * SAI_xCR1_MONO) |	// stereo MODE - with IQ pairs - see DMABUFFSIZE32
		(1 * SAI_xCR1_DMAEN) |	// 1: DMA enable 
		(1 * SAI_xCR1_CKSTR) |	// 0: ������ �� ������ �������� �� ������������ ������, 1: �� ����������
		(7 * SAI_xCR1_DS_0) |	// 6: data size - 24 bit, 7: 32 bit, 4: 16 bit
		(0 * SAI_xCR1_PRTCFG_0) |	// 0: free protocol
		(nodiv * SAI_xCR1_NODIV) |	// 1: no MCKDIV value
		((mckdiv * SAI_xCR1_MCKDIV_0) & SAI_xCR1_MCKDIV) |	// MCKDIV vale = 4 bits
		0;

#if WITHSAICLOCKFROMPIN
	debug_printf_P(PSTR("hardware_sai1_master_fullduplex_initialize: 1 SAI1 MCKDIV=%lu, ARMSAIMCLK=%lu, EXTSAI_FREQ=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) EXTSAI_FREQ);
#elif WITHSAICLOCKFROMI2S
	debug_printf_P(PSTR("hardware_sai1_master_fullduplex_initialize: 2 SAI1 MCKDIV=%lu, ARMSAIMCLK=%lu, PLLI2S_FREQ_OUT=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) PLLI2S_FREQ_OUT);
#else
	debug_printf_P(PSTR("hardware_sai1_master_fullduplex_initialize: 3 SAI1 MCKDIV=%lu, ARMSAIMCLK=%lu, PLLSAI_FREQ_OUT=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) PLLSAI_FREQ_OUT);
#endif

	SAI1_Block_A->CR1 = 
		commoncr1 |
		(0 * SAI_xCR1_SYNCEN_0) |	// SYNChronization ENable: 0: audio sub-block in asynchronous mode.
		(0 * SAI_xCR1_MODE_0) |	// 0: Master transmitter, 1: Master receiver, 2: Slave transmitter, 3: Slave receiver
		0;
	SAI1_Block_B->CR1 = 
		commoncr1 |
		(1 * SAI_xCR1_SYNCEN_0) |	// SYNChronization ENable: audio sub-block is synchronous with the other internal audio sub-block. In this case, the audio sub-block must be configured in slave mode
		(3 * SAI_xCR1_MODE_0) |		// 0: Master transmitter, 1: Master receiver, 2: Slave transmitter, 3: Slave receiver
		0;

	// CR2 value
	const portholder_t commoncr2 =
		//(2 * SAI_xCR2_FTH_0) |
		0;
	SAI1_Block_A->CR2 = 
		commoncr2 |
		0;
	SAI1_Block_B->CR2 = 
		commoncr2 |
		0;
	// SLOTR value
	const portholder_t commonslotr =
		(0 * SAI_xSLOTR_SLOTSZ_0) |	// 00: The slot size is equivalent to the data size (specified in DS[3:0] in the SAI_xCR1 register).
		((NBSLOT_SAI1 - 1) * SAI_xSLOTR_NBSLOT_0) | // Number of slots in audio Frame
		//SAI_xSLOTR_SLOTEN |			// all slots enabled
		//(3UL << 16) |
		//(1 * SAI_xSLOTR_FBOFF_0) | // slot offset - "1" for I2S 24 bit in 32 bit slot
		0;
	SAI1_Block_A->SLOTR = 
		((SLOTEN_TX_SAI1 * MASK2LSB(SAI_xSLOTR_SLOTEN)) & SAI_xSLOTR_SLOTEN) |			// TX slots enabled
		commonslotr |
		0;
	SAI1_Block_B->SLOTR = 
		((SLOTEN_RX_SAI1 * MASK2LSB(SAI_xSLOTR_SLOTEN)) & SAI_xSLOTR_SLOTEN) |			// RX slots enabled
		commonslotr |
		0;

	// FRCR value
	const portholder_t comm_frcr =
		((WITHSAI1_FRAMEBITS - 1) * SAI_xFRCR_FRL_0) |
		(1 * SAI_xFRCR_FSDEF) |		/* FS signal is a start of frame signal + channel side identification - must be set for I2S */
		((WITHSAI1_FRAMEBITS / 2 - 1) * SAI_xFRCR_FSALL_0) |
#if WITHSAI1_FORMATI2S_PHILIPS
		(1 * SAI_xFRCR_FSOFF) |		/* 1: FS is asserted one bit before the first bit of the slot 0. Classic I2S form Philips Semiconductors. "0" value for Sony I2S specs. */
		(0 * SAI_xFRCR_FSPO) |	/* 0: ����� � �������� 0 ���������� ��� "0" �� WS - ������. */
#endif /* WITHSAI1_FORMATI2S_PHILIPS */
		0;
	// FRAME CONTROL REGISTER
	SAI1_Block_A->FRCR =
		comm_frcr |
		0;
	SAI1_Block_B->FRCR =
		comm_frcr |
		0;

	SAI1HW_INITIALIZE();
}

#else /* WITHSAI1HWTXRXMASTER */

static void hardware_sai1_slave_fullduplex_initialize(void)		/* ������������� SAI1 �� STM32F4xx */
{
	hardware_sai1_sai2_clock_selection();

	// ������ �������� ������ SAI.
	RCC->APB2ENR |= RCC_APB2ENR_SAI1EN; //������ ������������ 
	__DSB();
	
	SAI1_Block_A->CR1 &= ~ SAI_xCR1_SAIEN;
	SAI1_Block_B->CR1 &= ~ SAI_xCR1_SAIEN;

	// extclock = 24.576 MHz
	// if SAI_xCR1_NODIV == 1, ws=384 kHz
	//const uint32_t mckval = 0;	// divider = 1, ws = 96 kHz
	//const uint32_t mckval = 1;	// divider = 2, ws = 48 kHz
	//const uint32_t mckval = 2;	// divider = 4, ws = 24 kHz
	//const uint32_t mckval = 3;	// divider = 6, ws = 16 kHz
	//const uint32_t mckval = 4;	// divider = 8, ws = 12 kHz
	//
	// SAI_xCR1_CKSTR:
	// 0: Signals generated by the SAI change on SCK rising edge, 
	//		while signals received by the SAI are sampled on the SCK falling edge.
	// 1: Signals generated by the SAI change on SCK falling edge, 
	//		while signals received by the SAI are sampled on the SCK rising edge.
	//const uint_fast8_t nodiv = 0;
#if WITHSAICLOCKFROMPIN
	//const uint_fast8_t mckdiv = mckdivform(calcdivround_extsai(ARMSAIMCLK));
#elif WITHSAICLOCKFROMI2S
	//const uint_fast8_t mckdiv = mckdivform(calcdivround_plli2s(ARMSAIMCLK));
#else
		// from own PLL
	//const uint_fast8_t mckdiv = mckdivform(calcdivround_pllsai(ARMSAIMCLK));
#endif

	// CR1 value
	const portholder_t commoncr1 =
		(1 * SAI_xCR1_OUTDRIV) |	//
		(0 * SAI_xCR1_MONO) |	// stereo MODE - with IQ pairs - see DMABUFFSIZE32
		(1 * SAI_xCR1_DMAEN) |	// 1: DMA enable 
		(1 * SAI_xCR1_CKSTR) |	// 0: ������ �� ������ �������� �� ������������ ������, 1: �� ����������
		(7 * SAI_xCR1_DS_0) |	// 6: data size - 24 bit, 7: 32 bit, 4: 16 bit
		(0 * SAI_xCR1_PRTCFG_0) |	// 0: free protocol
		//(nodiv * SAI_xCR1_NODIV) |	// 1: no MCKDIV value
		//((mckdiv * SAI_xCR1_MCKDIV_0) & SAI_xCR1_MCKDIV) |	// MCKDIV vale = 4 bits
		0;

#if WITHSAICLOCKFROMPIN
	//debug_printf_P(PSTR("hardware_sai1_slave_fullduplex_initialize: 1 SAI1 MCKDIV=%lu, ARMSAIMCLK=%lu, EXTSAI_FREQ=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) EXTSAI_FREQ);
#elif WITHSAICLOCKFROMI2S
	//debug_printf_P(PSTR("hardware_sai1_slave_fullduplex_initialize: 2 SAI1 MCKDIV=%lu, ARMSAIMCLK=%lu, PLLI2S_FREQ_OUT=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) PLLI2S_FREQ_OUT);
#else
	//debug_printf_P(PSTR("hardware_sai1_slave_fullduplex_initialize: 3 SAI1 MCKDIV=%lu, ARMSAIMCLK=%lu, PLLSAI_FREQ_OUT=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) PLLSAI_FREQ_OUT);
#endif

	SAI1_Block_A->CR1 = 
		commoncr1 |
		(0 * SAI_xCR1_SYNCEN_0) |	// SYNChronization ENable: 0: audio sub-block in asynchronous mode.
		(2 * SAI_xCR1_MODE_0) |	// 0: Master transmitter, 1: Master receiver, 2: Slave transmitter, 3: Slave receiver
		0;
	SAI1_Block_B->CR1 = 
		commoncr1 |
		(1 * SAI_xCR1_SYNCEN_0) |	// SYNChronization ENable: audio sub-block is synchronous with the other internal audio sub-block. In this case, the audio sub-block must be configured in slave mode
		(3 * SAI_xCR1_MODE_0) |		// 0: Master transmitter, 1: Master receiver, 2: Slave transmitter, 3: Slave receiver
		0;

	// CR2 value
	const portholder_t commoncr2 =
		//(2 * SAI_xCR2_FTH_0) |
		0;
	SAI1_Block_A->CR2 = 
		commoncr2 |
		0;
	SAI1_Block_B->CR2 = 
		commoncr2 |
		0;
	// SLOTR value
	const portholder_t commonslotr =
		(0 * SAI_xSLOTR_SLOTSZ_0) |	// 00: The slot size is equivalent to the data size (specified in DS[3:0] in the SAI_xCR1 register).
		((NBSLOT_SAI1 - 1) * SAI_xSLOTR_NBSLOT_0) | // Number of slots in audio Frame
		//SAI_xSLOTR_SLOTEN |			// all slots enabled
		//(3UL << 16) |
		//(1 * SAI_xSLOTR_FBOFF_0) | // slot offset - "1" for I2S 24 bit in 32 bit slot
		0;
	SAI1_Block_A->SLOTR = 
		((SLOTEN_TX_SAI1 * MASK2LSB(SAI_xSLOTR_SLOTEN)) & SAI_xSLOTR_SLOTEN) |			// TX slots enabled
		commonslotr |
		0;
	SAI1_Block_B->SLOTR = 
		((SLOTEN_RX_SAI1 * MASK2LSB(SAI_xSLOTR_SLOTEN)) & SAI_xSLOTR_SLOTEN) |			// RX slots enabled
		commonslotr |
		0;

	// FRCR value
	const portholder_t comm_frcr =
		((WITHSAI1_FRAMEBITS - 1) * SAI_xFRCR_FRL_0) |
		(1 * SAI_xFRCR_FSDEF) |		/* FS signal is a start of frame signal + channel side identification - must be set for I2S */
		((WITHSAI1_FRAMEBITS / 2 - 1) * SAI_xFRCR_FSALL_0) |
#if WITHSAI1_FORMATI2S_PHILIPS
		(1 * SAI_xFRCR_FSOFF) |		/* 1: FS is asserted one bit before the first bit of the slot 0. Classic I2S form Philips Semiconductors. "0" value for Sony I2S specs. */
		(0 * SAI_xFRCR_FSPO) |	/* 0: ����� � �������� 0 ���������� ��� "0" �� WS - ������. */
#endif /* WITHSAI1_FORMATI2S_PHILIPS */
		0;
	// FRAME CONTROL REGISTER
	SAI1_Block_A->FRCR =
		comm_frcr |
		0;
	SAI1_Block_B->FRCR =
		comm_frcr |
		0;

	//debug_printf_P(PSTR("SAI1_Block_A->CR1=0X%08lX;\n"), SAI1_Block_A->CR1); 
	//debug_printf_P(PSTR("SAI1_Block_A->CR2=0X%08lX;\n"), SAI1_Block_A->CR2); 
	//debug_printf_P(PSTR("SAI1_Block_A->SLOTR=0X%08lX;\n"), SAI1_Block_A->SLOTR); 
	//debug_printf_P(PSTR("SAI1_Block_A->FRCR=0X%08lX;\n"), SAI1_Block_A->FRCR); 

	//debug_printf_P(PSTR("SAI1_Block_B->CR1=0X%08lX;\n"), SAI1_Block_B->CR1); 
	//debug_printf_P(PSTR("SAI1_Block_B->CR2=0X%08lX;\n"), SAI1_Block_B->CR2); 
	//debug_printf_P(PSTR("SAI1_Block_B->SLOTR=0X%08lX;\n"), SAI1_Block_B->SLOTR); 
	//debug_printf_P(PSTR("SAI1_Block_B->FRCR=0X%08lX;\n"), SAI1_Block_B->FRCR); 

	SAI1HW_INITIALIZE();
}

#endif /* WITHSAI1HWTXRXMASTER */

static void hardware_sai1_enable(void)		/* ���������� ������ SAI1 �� STM32F4xx */
{
	SAI1_Block_B->CR1 |= SAI_xCR1_SAIEN;
	SAI1_Block_A->CR1 |= SAI_xCR1_SAIEN;
}

#endif /* WITHSAI1HW */

#if WITHSAI2HW

#if ! WITHSAI1HW
	//#error WITHSAI1HW must be enabled
#endif /* ! WITHSAI1HW */

// DMA �� ������ SAI2 - ���������� ����������
// RX	SAI2_B	DMA2	Stream7	Channel 3
// Use arm_hardware_invalidate
void RAMFUNC_NONILINE DMA2_Stream7_IRQHandler(void)
{
	// �������� ������� ����� ������������� ��� ���������� ����������� ������
	if ((DMA2->HISR & DMA_HISR_TCIF7) != 0)
	{
		const uint_fast8_t b = (DMA2_Stream7->CR & DMA_SxCR_CT) != 0;

#if WITHSUSBSPKONLY
		if (b != 0)
		{
			processing_dmabuffer32rx(DMA2_Stream7->M0AR);
			DMA2_Stream7->M0AR = dma_invalidate32rx(allocate_dmabuffer32rx());
		}
		else
		{
			processing_dmabuffer32rx(DMA2_Stream7->M1AR);
			DMA2_Stream7->M1AR = dma_invalidate32rx(allocate_dmabuffer32rx());
		}
#else /* WITHSUSBSPKONLY */
		if (b != 0)
		{
			processing_dmabuffer32wfm(DMA2_Stream7->M0AR);
			DMA2_Stream7->M0AR = dma_invalidate32rx(allocate_dmabuffer32rx());
		}
		else
		{
			processing_dmabuffer32wfm(DMA2_Stream7->M1AR);
			DMA2_Stream7->M1AR = dma_invalidate32rx(allocate_dmabuffer32rx());
		}
#endif /* WITHSUSBSPKONLY */
		DMA2->HIFCR = DMA_HIFCR_CTCIF7;	// Clear TC interrupt flag ��������������� stream
	}
/*
	if ((DMA2->HISR & DMA_HISR_TEIF7) != 0)
	{
		DMA2->HIFCR = DMA_HIFCR_CTEIF7;	// Clear ERROR interrupt flag ��������������� stream
		DMA2_Stream7->CR &= ~ DMA_SxCR_EN;
		while (DMA2_Stream7->CR & DMA_SxCR_EN)
			;
		DMA2_Stream7->CR |= DMA_SxCR_EN;
	}
*/
}

// TX	SAI2_A	DMA2	Stream 4	Channel 3
// Use arm_hardware_flush
void DMA2_Stream4_IRQHandler(void)
{
	// �������� ������� ����� ������������� ��� ���������� ����������� ������
	if ((DMA2->HISR & DMA_HISR_TCIF4) != 0)
	{
		const uint_fast8_t b = (DMA2_Stream4->CR & DMA_SxCR_CT) != 0;
#if WITHSUSBSPKONLY
		if (b != 0)
		{
			release_dmabuffer32tx(DMA2_Stream4->M0AR);
			DMA2_Stream4->M0AR = dma_flush32tx(getfilled_dmabuffer32tx_main());
		}
		else
		{
			release_dmabuffer32tx(DMA2_Stream4->M1AR);
			DMA2_Stream4->M1AR = dma_flush32tx(getfilled_dmabuffer32tx_main());
		}
#else /* WITHSUSBSPKONLY */
		if (b != 0)
		{
			release_dmabuffer32tx(DMA2_Stream4->M0AR);
			DMA2_Stream4->M0AR = dma_flush32tx(getfilled_dmabuffer32tx_sub());
		}
		else
		{
			release_dmabuffer32tx(DMA2_Stream4->M1AR);
			DMA2_Stream4->M1AR = dma_flush32tx(getfilled_dmabuffer32tx_sub());
		}
#endif /* WITHSUSBSPKONLY */
		DMA2->HIFCR = DMA_HIFCR_CTCIF4;	// Clear TC interrupt flag ��������������� stream
	}
}

// TX	SAI2_A	DMA2	Stream 4	Channel 3
// Use arm_hardware_flush
static void DMA_SAI2_A_TX_initializeXXX(void)
{

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//������� DMA2 
	__DSB();

	#if CPUSTYLE_STM32H7XX
		// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
		// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
		enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
		DMAMUX1_Channel12->CCR = 89 * DMAMUX_CxCR_DMAREQ_ID_0;	// SAI2_A
	#else /* CPUSTYLE_STM32H7XX */
		const uint_fast8_t ch = 3;
	#endif /* CPUSTYLE_STM32H7XX */

	DMA2_Stream4->PAR = (uint32_t) & SAI2_Block_A->DR;
	DMA2_Stream4->M0AR = dma_flush32tx(allocate_dmabuffer32tx());
	DMA2_Stream4->M1AR = dma_flush32tx(allocate_dmabuffer32tx());
	DMA2_Stream4->NDTR = (DMA2_Stream4->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE32TX * DMA_SxNDT_0);

	DMA2_Stream4->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA2_Stream4->CR =
		ch * DMA_SxCR_CHSEL_0 | //�����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		1 * DMA_SxCR_DIR_0 | //����������� - ������ - ���������
		1 * DMA_SxCR_MINC | //��������� ������
		2 * DMA_SxCR_MSIZE_0 | //����� � ������ - 32 bit
		2 * DMA_SxCR_PSIZE_0 | //����� � DR - 32 bit
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT | // M0AR selected
		1 * DMA_SxCR_DBM | // double buffer mode seelcted
		0;

	DMA2->HIFCR = DMA_HIFCR_CTCIF4;	// Clear TC interrupt flag ��������������� stream
	DMA2_Stream4->CR |= DMA_SxCR_TCIE;	// ��������� ���������� �� DMA

	NVIC_SetPriority(DMA2_Stream4_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA2_Stream4_IRQn);	// DMA2_Stream4_IRQHandler() enable

	DMA2_Stream4->CR |= DMA_SxCR_EN;
}

// TX	SAI2_A	DMA2	Stream 4	Channel 3
// Use arm_hardware_flush
static void DMA_SAI2_A_TX_initializeAUDIO48(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//������� DMA2 
	__DSB();

	#if CPUSTYLE_STM32H7XX
		// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
		// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
		enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
		DMAMUX1_Channel12->CCR = 89 * DMAMUX_CxCR_DMAREQ_ID_0;	// SAI2_A
	#else /* CPUSTYLE_STM32H7XX */
		const uint_fast8_t ch = 3;
	#endif /* CPUSTYLE_STM32H7XX */

	DMA2_Stream4->PAR = (uint32_t) & SAI2_Block_A->DR;
	DMA2_Stream4->M0AR = dma_flush32tx(allocate_dmabuffer32tx());
	DMA2_Stream4->M1AR = dma_flush32tx(allocate_dmabuffer32tx());
	DMA2_Stream4->NDTR = (DMA2_Stream4->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE32TX * DMA_SxNDT_0);

	DMA2_Stream4->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA2_Stream4->CR =
		ch * DMA_SxCR_CHSEL_0 | //�����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		1 * DMA_SxCR_DIR_0 | //����������� - ������ - ���������
		1 * DMA_SxCR_MINC | //��������� ������
		2 * DMA_SxCR_MSIZE_0 | //����� � ������ - 32 bit
		2 * DMA_SxCR_PSIZE_0 | //����� � DR - 32 bit
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT | // M0AR selected
		1 * DMA_SxCR_DBM | // double buffer mode seelcted
		0;

	DMA2->HIFCR = DMA_HIFCR_CTCIF4;	// Clear TC interrupt flag ��������������� stream
	DMA2_Stream4->CR |= DMA_SxCR_TCIE;	// ��������� ���������� �� DMA

	NVIC_SetPriority(DMA2_Stream4_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA2_Stream4_IRQn);	// DMA2_Stream4_IRQHandler() enable

	DMA2_Stream4->CR |= DMA_SxCR_EN;
}

/* DMA ��� ���� �� SAI_2_B  - ������������� */
//	RX	SAI2_B	DMA2	Stream7	Channel 0	
// Use arm_hardware_invalidate
static void DMA_SAI2_B_RX_initializeRTS96(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//������� DMA2 
	__DSB();

	#if CPUSTYLE_STM32H7XX
		// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
		// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
		enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
		DMAMUX1_Channel15->CCR = 90 * DMAMUX_CxCR_DMAREQ_ID_0;	// SAI2_B
	#else /* CPUSTYLE_STM32H7XX */
		/* DMA2	Stream7	Channel 0 */ 
		const uint_fast8_t ch = 0;
	#endif /* CPUSTYLE_STM32H7XX */

	DMA2_Stream7->PAR = (uintptr_t) & SAI2_Block_B->DR;
	DMA2_Stream7->M0AR = dma_invalidate192rts(allocate_dmabuffer192rts());
	DMA2_Stream7->M1AR = dma_invalidate192rts(allocate_dmabuffer192rts());
	DMA2_Stream7->NDTR = (DMA2_Stream7->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE192RTS * DMA_SxNDT_0);

	DMA2_Stream7->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA2_Stream7->CR =
		ch * DMA_SxCR_CHSEL_0 | // �����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_DIR_0 |	// 00: Peripheral-to-memory
		1 * DMA_SxCR_MINC |		//��������� ������
		2 * DMA_SxCR_MSIZE_0 | //����� � ������ - 32 bit
		2 * DMA_SxCR_PSIZE_0 | //����� � DR - 32 bit
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT |	// M0AR selected
		1 * DMA_SxCR_DBM |	 // double buffer mode seelcted
		0;

	DMA2->HIFCR = (DMA_HIFCR_CTCIF7 /*| DMA_HIFCR_CTEIF7 */);	// Clear TC interrupt flag ��������������� stream
	DMA2_Stream7->CR |= (DMA_SxCR_TCIE /* | DMA_SxCR_TEIE */);	// ���������� �� DMA �� TC � TE

	NVIC_SetPriority(DMA2_Stream7_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);	// DMA2_Stream7_IRQHandler() enable

	DMA2_Stream7->CR |= DMA_SxCR_EN;
}

/* DMA ��� ���� �� SAI_2_B  - ������������� */
//	RX	SAI2_B	DMA2	Stream7	Channel 0	
// Use arm_hardware_invalidate
static void DMA_SAI2_B_RX_initializeAUDIO48(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//������� DMA2 
	__DSB();

	#if CPUSTYLE_STM32H7XX
		// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
		// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
		enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
		DMAMUX1_Channel15->CCR = 90 * DMAMUX_CxCR_DMAREQ_ID_0;	// SAI2_B
	#else /* CPUSTYLE_STM32H7XX */
		/* DMA2	Stream7	Channel 0 */ 
		const uint_fast8_t ch = 0;
	#endif /* CPUSTYLE_STM32H7XX */

	DMA2_Stream7->PAR = (uintptr_t) & SAI2_Block_B->DR;
	DMA2_Stream7->M0AR = dma_invalidate32rx(allocate_dmabuffer32rx());
	DMA2_Stream7->M1AR = dma_invalidate32rx(allocate_dmabuffer32rx());
	DMA2_Stream7->NDTR = (DMA2_Stream7->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE32RX * DMA_SxNDT_0);

	DMA2_Stream7->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA2_Stream7->CR =
		ch * DMA_SxCR_CHSEL_0 | // �����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_DIR_0 |	// 00: Peripheral-to-memory
		1 * DMA_SxCR_MINC |		//��������� ������
		2 * DMA_SxCR_MSIZE_0 | //����� � ������ - 32 bit
		2 * DMA_SxCR_PSIZE_0 | //����� � DR - 32 bit
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT |	// M0AR selected
		1 * DMA_SxCR_DBM |	 // double buffer mode seelcted
		0;

	DMA2->HIFCR = (DMA_HIFCR_CTCIF7 /*| DMA_HIFCR_CTEIF7 */);	// Clear TC interrupt flag ��������������� stream
	DMA2_Stream7->CR |= (DMA_SxCR_TCIE /* | DMA_SxCR_TEIE */);	// ���������� �� DMA �� TC � TE

	NVIC_SetPriority(DMA2_Stream7_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);	// DMA2_Stream7_IRQHandler() enable

	DMA2_Stream7->CR |= DMA_SxCR_EN;
}

/* ������������� SAI2 �� STM32F4xx */
// ����� 24-������ � 32-� ������ ����� (��� ������ ����� � ������ �������� ������) =
// ����������� ������� � �������� ����� - ��� ������ �������� � USB AUDIO
static void hardware_sai2_slave_fullduplex_initialize(void)		
{
	debug_printf_P(PSTR("hardware_sai2_slave_fullduplex_initialize start\n"));
	hardware_sai1_sai2_clock_selection();

	// ������ �������� ������ SAI.
	RCC->APB2ENR |= RCC_APB2ENR_SAI2EN; //������ ������������ 
	__DSB();
	

	SAI2_Block_A->CR1 &= ~ SAI_xCR1_SAIEN;
	SAI2_Block_B->CR1 &= ~ SAI_xCR1_SAIEN;

	// SAI_xCR1_CKSTR:
	// 0: Signals generated by the SAI change on SCK rising edge, 
	//		while signals received by the SAI are sampled on the SCK falling edge.
	// 1: Signals generated by the SAI change on SCK falling edge, 
	//		while signals received by the SAI are sampled on the SCK rising edge.

	// CR1 value (������������� � SAI1)
	const portholder_t commoncr1 =
		(1 * SAI_xCR1_OUTDRIV) |	//
		(0 * SAI_xCR1_MONO) |	// stereo MODE - with IQ pairs - see DMABUFFSIZE32
		(1 * SAI_xCR1_DMAEN) |	// 1: DMA enable 
		(1 * SAI_xCR1_CKSTR) |	// 0: ������ �� ������ �������� �� ������������ ������, 1: �� ����������
		(7 * SAI_xCR1_DS_0) |	// 6: data size - 24 bit, 7: 32 bit, 4: 16 bit
		(0 * SAI_xCR1_PRTCFG_0) |	// 0: free protocol
		//(1 * SAI_xCR1_NODIV) |	// 1: no MCKDIV value
		0;

	// SAI_xCR1_SYNCEN values:
	// 0: audio sub-block in asynchronous mode.
	// 1: audio sub-block is synchronous with the other internal audio sub-block. In this case, the audio 	sub-block must be configured in slave mode
	// 2: audio sub-block is synchronous with an external SAI embedded peripheral. In this case the audio sub-block should be configured in Slave mode.
	// 3: Reserved

	SAI2_Block_A->CR1 = 
		commoncr1 |
		(2 * SAI_xCR1_SYNCEN_0) |	// SYNChronization ENable: 2: audio sub-block is synchronous with an external SAI embedded peripheral. In this case the audio sub-block should be configured in Slave mode.
		(2 * SAI_xCR1_MODE_0) |	// 0: Master transmitter, 1: Master receiver, 2: Slave transmitter, 3: Slave receiver
		0;
	SAI2_Block_B->CR1 = 
		commoncr1 |
		(1 * SAI_xCR1_SYNCEN_0) |	// SYNChronization ENable: audio sub-block is synchronous with the other internal audio sub-block. In this case, the audio sub-block must be configured in slave mode
		(3 * SAI_xCR1_MODE_0) |		// 0: Master transmitter, 1: Master receiver, 2: Slave transmitter, 3: Slave receiver
		0;

	// CR2 value
	const portholder_t commoncr2 =
		//(2 * SAI_xCR2_FTH_0) |
		0;
	SAI2_Block_A->CR2 = 
		commoncr2 |
		0;
	SAI2_Block_B->CR2 = 
		commoncr2 |
		0;
	// SLOTR value
	const portholder_t commonslotr =
		(0 * SAI_xSLOTR_SLOTSZ_0) |	// 00: The slot size is equivalent to the data size (specified in DS[3:0] in the SAI_xCR1 register).
		((NBSLOT_SAI2 - 1) * SAI_xSLOTR_NBSLOT_0) | // Number of slots in audio Frame
		//SAI_xSLOTR_SLOTEN |			// all slots enabled
		//(3UL << 16) |
		//(1 * SAI_xSLOTR_FBOFF_0) | // slot offset - "1" for I2S 24 bit in 32 bit slot
		0;
	SAI2_Block_A->SLOTR = 
		((SLOTEN_TX_SAI2 * MASK2LSB(SAI_xSLOTR_SLOTEN)) & SAI_xSLOTR_SLOTEN) |			// TX slots enabled
		commonslotr |
		0;
	SAI2_Block_B->SLOTR = 
		((SLOTEN_RX_SAI2 * MASK2LSB(SAI_xSLOTR_SLOTEN)) & SAI_xSLOTR_SLOTEN) |			// RX slots enabled
		commonslotr |
		0;

	// FRCR value
	const portholder_t comm_frcr =
		((WITHSAI2_FRAMEBITS - 1) * SAI_xFRCR_FRL_0) |
		(1 * SAI_xFRCR_FSDEF) |		/* FS signal is a start of frame signal + channel side identification - must be set for I2S */
		((WITHSAI2_FRAMEBITS / 2 - 1) * SAI_xFRCR_FSALL_0) |
#if WITHSAI2_FORMATI2S_PHILIPS
		(1 * SAI_xFRCR_FSOFF) |		/* 1: FS is asserted one bit before the first bit of the slot 0. Classic I2S form Philips Semiconductors. "0" value for Sony I2S specs. */
		(0 * SAI_xFRCR_FSPO) |	/* 0: ����� � �������� 0 ���������� ��� "0" �� WS - ������. */
#endif /* WITHSAI2_FORMATI2S_PHILIPS */
		0;
	// FRAME CONTROL REGISTER
	SAI2_Block_A->FRCR =
		comm_frcr |
		0;
	SAI2_Block_B->FRCR =
		comm_frcr |
		0;

	// ��������� ������������� ��� SAI2
	// SAI_GCR_SYNCOUT: 1: Block A used for further synchronization for others SAI
	SAI1->GCR = (SAI1->GCR & ~ (SAI_GCR_SYNCIN | SAI_GCR_SYNCOUT)) |
		0 * SAI_GCR_SYNCIN_0 |
		1 * SAI_GCR_SYNCOUT_0 |
		0;

	// �������� ������������� �� SAI1
	// SAI2 can select the synchronization from SAI1 by setting SAI2 SYNCIN to 0
	SAI2->GCR = (SAI2->GCR & ~ (SAI_GCR_SYNCIN | SAI_GCR_SYNCOUT)) |
		0 * SAI_GCR_SYNCIN_0 |
		0 * SAI_GCR_SYNCOUT_0 |
		0;

	SAI2HW_INITIALIZE();
	debug_printf_P(PSTR("hardware_sai2_slave_fullduplex_initialize done\n"));
}

#if WITHSUSBSPKONLY

static void hardware_sai2_master_fullduplex_initialize(void)		/* ������������� SAI2 �� STM32F4xx */
{
	hardware_sai1_sai2_clock_selection();

	// ������ �������� ������ SAI.
	RCC->APB2ENR |= RCC_APB2ENR_SAI2EN; //������ ������������ 
	__DSB();
	

	SAI2_Block_A->CR1 &= ~ SAI_xCR1_SAIEN;
	SAI2_Block_B->CR1 &= ~ SAI_xCR1_SAIEN;

	// extclock = 24.576 MHz
	// if SAI_xCR1_NODIV == 1, ws=384 kHz
	//const uint32_t mckval = 0;	// divider = 1, ws = 96 kHz
	//const uint32_t mckval = 1;	// divider = 2, ws = 48 kHz
	//const uint32_t mckval = 2;	// divider = 4, ws = 24 kHz
	//const uint32_t mckval = 3;	// divider = 6, ws = 16 kHz
	//const uint32_t mckval = 4;	// divider = 8, ws = 12 kHz

	// SAI_xCR1_CKSTR:
	// 0: Signals generated by the SAI change on SCK rising edge, 
	//		while signals received by the SAI are sampled on the SCK falling edge.
	// 1: Signals generated by the SAI change on SCK falling edge, 
	//		while signals received by the SAI are sampled on the SCK rising edge.
	const uint_fast8_t nodiv = 0;
#if WITHSAICLOCKFROMPIN
	const uint_fast8_t mckdiv = mckdivform(calcdivround_extsai(ARMSAIMCLK));
#elif WITHSAICLOCKFROMI2S
	const uint_fast8_t mckdiv = mckdivform(calcdivround_plli2s(ARMSAIMCLK));
#else
		// from own PLL
	const uint_fast8_t mckdiv = mckdivform(calcdivround_pllsai(ARMSAIMCLK));
#endif

	// CR1 value
	const portholder_t commoncr1 =
		(1 * SAI_xCR1_OUTDRIV) |	//
		(0 * SAI_xCR1_MONO) |	// stereo MODE - with IQ pairs - see DMABUFFSIZE32
		(1 * SAI_xCR1_DMAEN) |	// 1: DMA enable 
		(1 * SAI_xCR1_CKSTR) |	// 0: ������ �� ������ �������� �� ������������ ������, 1: �� ����������
		(7 * SAI_xCR1_DS_0) |	// 6: data size - 24 bit, 7: 32 bit, 4: 16 bit
		(0 * SAI_xCR1_PRTCFG_0) |	// 0: free protocol
		(nodiv * SAI_xCR1_NODIV) |	// 1: no MCKDIV value
		((mckdiv * SAI_xCR1_MCKDIV_0) & SAI_xCR1_MCKDIV) |	// MCKDIV vale = 4 bits
		0;

#if WITHSAICLOCKFROMPIN
	debug_printf_P(PSTR("hardware_sai2_master_fullduplex_initialize: 1 SAI2 MCKDIV=%lu, ARMSAIMCLK=%lu, EXTSAI_FREQ=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) EXTSAI_FREQ);
#elif WITHSAICLOCKFROMI2S
	debug_printf_P(PSTR("hardware_sai2_master_fullduplex_initialize: 2 SAI2 MCKDIV=%lu, ARMSAIMCLK=%lu, PLLI2S_FREQ_OUT=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) PLLI2S_FREQ_OUT);
#else
	debug_printf_P(PSTR("hardware_sai2_master_fullduplex_initialize: 3 SAI2 MCKDIV=%lu, ARMSAIMCLK=%lu, PLLSAI_FREQ_OUT=%lu\n"), (unsigned long) mckdiv, (unsigned long) ARMSAIMCLK, (unsigned long) PLLSAI_FREQ_OUT);
#endif

	SAI2_Block_A->CR1 = 
		commoncr1 |
		(0 * SAI_xCR1_SYNCEN_0) |	// SYNChronization ENable: 0: audio sub-block in asynchronous mode.
		(0 * SAI_xCR1_MODE_0) |	// 0: Master transmitter, 1: Master receiver, 2: Slave transmitter, 3: Slave receiver
		0;
	SAI2_Block_B->CR1 = 
		commoncr1 |
		(1 * SAI_xCR1_SYNCEN_0) |	// SYNChronization ENable: audio sub-block is synchronous with the other internal audio sub-block. In this case, the audio sub-block must be configured in slave mode
		(3 * SAI_xCR1_MODE_0) |		// 0: Master transmitter, 1: Master receiver, 2: Slave transmitter, 3: Slave receiver
		0;

	// CR2 value
	const portholder_t commoncr2 =
		//(2 * SAI_xCR2_FTH_0) |
		0;
	SAI2_Block_A->CR2 = 
		commoncr2 |
		0;
	SAI2_Block_B->CR2 = 
		commoncr2 |
		0;
	// SLOTR value
	const portholder_t commonslotr =
		(0 * SAI_xSLOTR_SLOTSZ_0) |	// 00: The slot size is equivalent to the data size (specified in DS[3:0] in the SAI_xCR1 register).
		((NBSLOT_SAI2 - 1) * SAI_xSLOTR_NBSLOT_0) | // Number of slots in audio Frame
		//SAI_xSLOTR_SLOTEN |			// all slots enabled
		//(3UL << 16) |
		//(1 * SAI_xSLOTR_FBOFF_0) | // slot offset - "1" for I2S 24 bit in 32 bit slot
		0;
	SAI2_Block_A->SLOTR = 
		((SLOTEN_TX_SAI2 * MASK2LSB(SAI_xSLOTR_SLOTEN)) & SAI_xSLOTR_SLOTEN) |			// TX slots enabled
		commonslotr |
		0;
	SAI2_Block_B->SLOTR = 
		((SLOTEN_RX_SAI2 * MASK2LSB(SAI_xSLOTR_SLOTEN)) & SAI_xSLOTR_SLOTEN) |			// RX slots enabled
		commonslotr |
		0;

	// FRCR value
	const portholder_t comm_frcr =
		((WITHSAI2_FRAMEBITS - 1) * SAI_xFRCR_FRL_0) |
		(1 * SAI_xFRCR_FSDEF) |		/* FS signal is a start of frame signal + channel side identification - must be set for I2S */
		((WITHSAI2_FRAMEBITS / 2 - 1) * SAI_xFRCR_FSALL_0) |
#if WITHSAI2_FORMATI2S_PHILIPS
		(1 * SAI_xFRCR_FSOFF) |		/* 1: FS is asserted one bit before the first bit of the slot 0. Classic I2S form Philips Semiconductors. "0" value for Sony I2S specs. */
		(0 * SAI_xFRCR_FSPO) |	/* 0: ����� � �������� 0 ���������� ��� "0" �� WS - ������. */
#endif /* WITHSAI2_FORMATI2S_PHILIPS */
		0;
	// FRAME CONTROL REGISTER
	SAI2_Block_A->FRCR =
		comm_frcr |
		0;
	SAI2_Block_B->FRCR =
		comm_frcr |
		0;

	SAI2HW_INITIALIZE();
}

#endif /* WITHSUSBSPKONLY */

static void hardware_sai2_enable(void)		/* ���������� ������ SAI2 �� STM32F4xx */
{
	// ��� dual watch ������������ SAI2, ��
	// ����� ���� �� ���������� ������.
	// ��� ������ ������������� ���������� ��� ����� - � ���� � �������� - � SAI2

	////+++
	//DMA_SAI2_A_TX_initialize();	// SAI_xCR1_DMAEN in CR already set
	////---
	//DMA_SAI2_B_RX_initialize();	// SAI_xCR1_DMAEN in CR already set

	SAI2_Block_B->CR1 |= SAI_xCR1_SAIEN;
	SAI2_Block_A->CR1 |= SAI_xCR1_SAIEN;
}

/* DMA ��� ���� �� SAI_2_B  - ������������� */
//	RX	SAI2_B	DMA2	Stream7	Channel 0	
// Use arm_hardware_invalidate
static void DMA_SAI2_B_RX_initializeWFM(void)
{
	debug_printf_P(PSTR("DMA_SAI2_B_RX_initializeWFM start.\n"));

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;//������� DMA2 
	__DSB();

	#if CPUSTYLE_STM32H7XX
		// DMAMUX1 channels 0 to 7 are connected to DMA1 channels 0 to 7
		// DMAMUX1 channels 8 to 15 are connected to DMA2 channels 0 to 7
		enum { ch = 0, DMA_SxCR_CHSEL_0 = 0 };
		DMAMUX1_Channel15->CCR = 90 * DMAMUX_CxCR_DMAREQ_ID_0;	// SAI2_B
	#else /* CPUSTYLE_STM32H7XX */
		/* DMA2	Stream7	Channel 0 */ 
		const uint_fast8_t ch = 0;
	#endif /* CPUSTYLE_STM32H7XX */

	DMA2_Stream7->PAR = (uintptr_t) & SAI2_Block_B->DR;
	DMA2_Stream7->M0AR = dma_invalidate32rx(allocate_dmabuffer32rx());
	DMA2_Stream7->M1AR = dma_invalidate32rx(allocate_dmabuffer32rx());
	DMA2_Stream7->NDTR = (DMA2_Stream7->NDTR & ~ DMA_SxNDT) |
		(DMABUFFSIZE32RX * DMA_SxNDT_0);

	DMA2_Stream7->FCR &= ~ DMA_SxFCR_DMDIS;	// use direct mode
	DMA2_Stream7->CR =
		ch * DMA_SxCR_CHSEL_0 | // �����
		0 * DMA_SxCR_MBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_PBURST_0 |	// 0: single transfer
		0 * DMA_SxCR_DIR_0 |	// 00: Peripheral-to-memory
		1 * DMA_SxCR_MINC |		//��������� ������
		2 * DMA_SxCR_MSIZE_0 | //����� � ������ - 32 bit
		2 * DMA_SxCR_PSIZE_0 | //����� � DR - 32 bit
		2 * DMA_SxCR_PL_0 |		// Priority level - High
		0 * DMA_SxCR_CT |	// M0AR selected
		1 * DMA_SxCR_DBM |	 // double buffer mode seelcted
		0;

	DMA2->HIFCR = (DMA_HIFCR_CTCIF7 /*| DMA_HIFCR_CTEIF7 */);	// Clear TC interrupt flag ��������������� stream
	DMA2_Stream7->CR |= (DMA_SxCR_TCIE /* | DMA_SxCR_TEIE */);	// ���������� �� DMA �� TC � TE

	NVIC_SetPriority(DMA2_Stream7_IRQn, ARM_REALTIME_PRIORITY);
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);	// DMA2_Stream7_IRQHandler() enable

	DMA2_Stream7->CR |= DMA_SxCR_EN;
	debug_printf_P(PSTR("DMA_SAI2_B_RX_initializeWFM done.\n"));
}

#if WITHSUSBSPKONLY
static const codechw_t audiocodechw =
{
	hardware_sai2_master_fullduplex_initialize,	/* ��������� � �� ������ - �������� */
	hardware_dummy_initialize,
	DMA_SAI2_B_RX_initializeAUDIO48,					// DMA �� ����� SPI3_RX - DMA1, Stream0, Channel0
	DMA_SAI2_A_TX_initializeAUDIO48,					// DMA �� �������� ����� TX	SAI2_A	DMA2	Stream 4	Channel 3
	hardware_sai2_enable,
	hardware_dummy_enable,
	"sai2-audiocodechw"
};
#else
	// other CPUs
static const codechw_t fpgaspectrumhw =
{
	hardware_sai2_slave_fullduplex_initialize,	// added...
	hardware_dummy_initialize,
	DMA_SAI2_B_RX_initializeWFM,
	hardware_dummy_initialize,
	hardware_sai2_enable,
	hardware_dummy_enable,
	"sai2-fpga spectrum for WFM"
};

#endif /* WITHSUSBSPKONLY */


#endif /* WITHSAI2HW */

#if WITHSAI1HW

#if WITHSAI1HWTXRXMASTER

static const codechw_t fpgacodechw =
{
	hardware_sai1_master_fullduplex_initialize,
	hardware_dummy_initialize,
	DMA_SAI1_B_RX_initialize,
	DMA_SAI1_A_TX_initialize,
	hardware_sai1_enable,
	hardware_dummy_enable,
	"sai1-audiocodechw"
};

#else /* WITHSAI1HWTXRXMASTER */

static const codechw_t fpgacodechw =
{
	hardware_sai1_slave_fullduplex_initialize,
	hardware_dummy_initialize,
	DMA_SAI1_B_RX_initialize,
	DMA_SAI1_A_TX_initialize,
	hardware_sai1_enable,
	hardware_dummy_enable,
	"sai1-audiocodechw"
};

#endif /* WITHSAI1HWTXRXMASTER */

#endif /* WITHSAI1HW */

#elif CPUSTYLE_R7S721
	
enum
{
	R7S721_SSIF_CKDIV1 = 0, // CKDV	0000: AUDIO�/1: 12,288 -> 12,288 (48 kS, 128 bit, stereo)
	R7S721_SSIF_CKDIV2 = 1,
	R7S721_SSIF_CKDIV4 = 2,	// CKDV	0010: AUDIO�/4: 12,288 -> 3.072 (48 kS, 32 bit, stereo)
	R7S721_SSIF_CKDIV6 = 8,
	R7S721_SSIF_CKDIV8 = 3,
	R7S721_SSIF_CKDIV12 = 9,
	R7S721_SSIF_CKDIV16 = 4,
	R7S721_SSIF_CKDIV24 = 10,
	R7S721_SSIF_CKDIV32 = 5,
	R7S721_SSIF_CKDIV48 = 11,
	R7S721_SSIF_CKDIV64 = 6,
	R7S721_SSIF_CKDIV96 = 12,
	R7S721_SSIF_CKDIV128 = 7,
};

#if WITHI2SHW

// audio codec
// DMA �� ������ SSIF0 - ���������� ����������

static RAMFUNC_NONILINE void r7s721_ssif0_rxdma(void)
{
	DMAC0.CHCFG_n |= DMAC0_CHCFG_n_REN;	// REN bit
	// SR (bt 7)
	// Indicates the register set currently selected in register mode.
	// 0: Next0 Register Set
	// 1: Next1 Register Set
	const uint_fast8_t b = (DMAC0.CHSTAT_n & (1U << DMAC0_CHSTAT_n_SR_SHIFT)) != 0;	// SR
	if (b != 0)
	{
		processing_dmabuffer16rx(DMAC0.N0DA_n);
		DMAC0.N0DA_n = dma_invalidate16rx(allocate_dmabuffer16());
	}
	else
	{
		processing_dmabuffer16rx(DMAC0.N1DA_n);
		DMAC0.N1DA_n = dma_invalidate16rx(allocate_dmabuffer16());
	}
}

// audio codec
// DMA �� �������� SSIF0 - ���������� ����������
// Use arm_hardware_flush

static void r7s721_ssif0_txdma(void)
{
	DMAC1.CHCFG_n |= DMAC1_CHCFG_n_REN;	// REN bit
	// SR (bt 7)
	// Indicates the register set currently selected in register mode.
	// 0: Next0 Register Set
	// 1: Next1 Register Set
	const uint_fast8_t b = (DMAC1.CHSTAT_n & DMAC1_CHSTAT_n_SR) != 0;	// SR
	if (b != 0)
	{
		release_dmabuffer16(DMAC1.N0SA_n);
		DMAC1.N0SA_n = dma_flush16tx(getfilled_dmabuffer16phones());
	}
	else
	{
		release_dmabuffer16(DMAC1.N1SA_n);
		DMAC1.N1SA_n = dma_flush16tx(getfilled_dmabuffer16phones());
	}
}


// audio codec
// DMA �� ����� SSIF0

static void r7s721_ssif0_dmarx_initialize(void)
{
	const uint_fast8_t id = 0;		// 0: DMAC0
	// DMAC0
   /* Set Source Start Address */
    DMAC0.N0SA_n = (uint32_t) & SSIF0.SSIFRDR;	// Fixed source address
    DMAC0.N1SA_n = (uint32_t) & SSIF0.SSIFRDR;	// Fixed source address

    /* Set Destination Start Address */
	DMAC0.N0DA_n = dma_invalidate16rx(allocate_dmabuffer16());
	DMAC0.N1DA_n = dma_invalidate16rx(allocate_dmabuffer16());

    /* Set Transfer Size */
    DMAC0.N0TB_n = DMABUFFSIZE16 * sizeof (uint16_t);	// ������ � ������
    DMAC0.N1TB_n = DMABUFFSIZE16 * sizeof (uint16_t);	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// SSIRXI0 (receive data full)
	const uint_fast8_t mid = 0x38;	
	const uint_fast8_t rid = 2;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 0;

	DMAC0.CHCFG_n =
		0 * (1U << 31) |	// DMS	0: Register mode
		1 * (1U << 30) |	// REN	1: Continues DMA transfers.
		1 * (1U << 29) |	// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << 28) |	// RSEL	0: Executes the Next0 Register Set
		0 * (1U << 27) |	// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << 24) |	// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << 22) |	// TM	0: Single transfer mode - ������ �� Table 9.4
		0 * (1U << 21) |	// DAD	0: Increment destination address
		1 * (1U << 20) |	// SAD	1: Fixed source address
		2 * (1U << 16) |	// DDS	2: 32 bits, 1: 16 bits (Destination Data Size)
		2 * (1U << 12) |	// SDS	2: 32 bits, 1: 16 bits (Source Data Size)
		am * (1U << 8) |	// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << 6) |	// LVL	1: Detects based on the level.
		1 * (1U << 5) |		// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << 3) |	// REQD		Request Direction
		(id & 0x07) * (1U << 0) |		// SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC01.DMARS = (DMAC01.DMARS & ~ (0x1FFul << dmarsshift)) |
		mid * (1U << (2 + dmarsshift)) |		// MID
		rid * (1U << (0 + dmarsshift)) |		// RID
		0;

    DMAC07.DCTRL_0_7 = (DMAC07.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT0_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_ssif0_rxdma);
		GIC_SetPriority(int_id, ARM_REALTIME_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC0.CHCTRL_n = 1 * (1U << 3);		// SWRST
	DMAC0.CHCTRL_n = 1 * (1U << 17);	// CLRINTMSK
	DMAC0.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

// audio codec
// DMA �� �������� SSIF0
// Use arm_hardware_flush

static void r7s721_ssif0_dmatx_initialize(void)
{
	const uint_fast8_t id = 1;	// 1: DMAC1
	// DMAC1
	/* Set Source Start Address */
	DMAC1.N0SA_n = dma_flush16tx(allocate_dmabuffer16());
	DMAC1.N1SA_n = dma_flush16tx(allocate_dmabuffer16());

    /* Set Destination Start Address */
    DMAC1.N0DA_n = (uint32_t) & SSIF0.SSIFTDR;	// Fixed destination address
    DMAC1.N1DA_n = (uint32_t) & SSIF0.SSIFTDR;	// Fixed destination address

    /* Set Transfer Size */
    DMAC1.N0TB_n = DMABUFFSIZE16 * sizeof (uint16_t);	// ������ � ������
    DMAC1.N1TB_n = DMABUFFSIZE16 * sizeof (uint16_t);	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// SSITXI0 (transmit data empty)
	const uint_fast8_t mid = 0x38;	
	const uint_fast8_t rid = 1;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 1;	

	DMAC1.CHCFG_n =
		0 * (1U << 31) |	// DMS	0: Register mode
		1 * (1U << 30) |	// REN	1: Continues DMA transfers.
		1 * (1U << 29) |	// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << 28) |	// RSEL	0: Executes the Next0 Register Set
		0 * (1U << 27) |	// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << 24) |	// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << 22) |	// TM	0: Single transfer mode - ������ �� Table 9.4
		1 * (1U << 21) |	// DAD	1: Fixed destination address
		0 * (1U << 20) |	// SAD	0: Increment source address
		2 * (1U << 16) |	// DDS	2: 32 bits, 1: 16 bits (Destination Data Size)
		2 * (1U << 12) |	// SDS	2: 32 bits, 1: 16 bits (Source Data Size)
		am * (1U << 8) |	// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << 6) |	// LVL	1: Detects based on the level.
		1 * (1U << 5) |		// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << 3) |	// REQD		Request Direction
		(id & 0x07) * (1U << 0) |		// SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC01.DMARS = (DMAC01.DMARS & ~ (0x1FFul << dmarsshift)) |
		mid * (1U << (2 + dmarsshift)) |		// MID
		rid * (1U << (0 + dmarsshift)) |		// RID
		0;

    DMAC07.DCTRL_0_7 = (DMAC07.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT1_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_ssif0_txdma);
		GIC_SetPriority(int_id, ARM_REALTIME_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC1.CHCTRL_n = 1 * (1U << 3);		// SWRST
	DMAC1.CHCTRL_n = 1 * (1U << 17);	// CLRINTMSK
	DMAC1.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

#define R7S721_MASTER 1
// ��������, ������� ������� �� master - �� ���� ������ �������� � ��������� (FIFO) � FPGA
// ���� ������� ������� ���: AUDIO_X1 ����������� ������ ����� ��������� i2s_ready - �� ��������
// ������, � � SLAVE ������ ������� ��� �������� - ����� ���� ������ �������� �� ������ ���������� �������.

// AUDIO CODEC I2S INTERFACE
static void r7s721_ssif0_fullduplex_initialize(void)
{
	const uint_fast8_t master = R7S721_MASTER;
    /* ---- Supply clock to the SSIF(channel 0) ---- */
	CPG.STBCR11 &= ~ (1U << 5);	// Module Stop 115 0: Channel 0 of the serial sound interface runs.
	(void) CPG.STBCR11;			/* Dummy read */

	// I2S compatible
	// SCKP = 0, SWSP = 0, DEL = 0, CHNL = 00
	// System word length = data word length

	// Control Register (SSICR)
	SSIF0.SSICR = 
		R7S721_USE_AUDIO_CLK * (1UL << 30) |		// CKS 1: AUDIO_CLK input 0: AUDIO_X1 input
		0 * (1UL << 22) |		// CHNL		00: Having one channel per system word (I2S complaint)
		1 * (1UL << 19) |		// DWL		1: 16 bit	
		1 * (1UL << 16) |		// SWL		1: 16 bit	
		master * (1UL << 15) |		// SCKD		1: Serial bit clock is output, master mode.
		master * (1UL << 14) |		// SWSD		1: Serial word select is output, master mode.
		0 * (1UL << 13) |		// SCKP		0: ������ �� ������ �������� �� ���������� ������ (I2S complaint)
		0 * (1UL << 12) |		// SWSP		0: SSIWS is low for the 1st channel, high for the 2nd channel.  (I2S complaint)
		0 * (1UL << 11) |		// SPDP		0: Padding bits are low.	
		0 * (1UL << 10) |		// SDTA	
		0 * (1UL << 9) |		// PDTA		1: 16 ��� ������� ������ - ���� 31..16 ��� ������/������ �������� ������
#if WITHI2S_FORMATI2S_PHILIPS
		0 * (1UL << 8) |		// DEL	0: 1 clock cycle delay between SSIWS and SSIDATA
#else /* WITHI2S_FORMATI2S_PHILIPS */
		1 * (1UL << 8) |		// DEL	1: No delay between SSIWS and SSIDATA
#endif /* WITHI2S_FORMATI2S_PHILIPS */
		master * R7S721_SSIF_CKDIV8 * (1UL << 4) |		// CKDV	0011: AUDIO�/8: 12,288 -> 1.536 (48 kS, 16 bit, stereo)
		0;

	// FIFO Control Register (SSIFCR)
	SSIF0.SSIFCR = 
		2 * (1uL << 6) |	// TTRG Transmit Data Trigger Number
		2 * (1uL << 4) |	// RTRG Receive Data Trigger Number
		1 * (1UL << 3) |		// TIE	Transmit Interrupt Enable
		1 * (1UL << 2) |		// RIE	Receive Interrupt Enable	
		//1 * (1UL << 1) |		// TFRST Transmit FIFO Data Register Reset
		//1 * (1UL << 0) |		// RFRST Receive FIFO Data Register Reset	
		0;

	HARDWARE_SSIF0_INITIALIZE();	// ����������� ��������� ������������� ����� � ������� ����������
}

static void r7s721_ssif0_fullduplex_enable(void)
{
	SSIF0.SSICR |= 
		1 * (1UL << 1) |		// TEN	
		1 * (1UL << 0) |		// REN	
		0;
	//local_delay_ms(20);
	//SSIF0.SSISR = ~ (1U << 28);	// TOIRQ
}

static const codechw_t audiocodechw =
{
	r7s721_ssif0_fullduplex_initialize,
	hardware_dummy_initialize,
	r7s721_ssif0_dmarx_initialize,
	r7s721_ssif0_dmatx_initialize,
	r7s721_ssif0_fullduplex_enable,
	hardware_dummy_enable,
	"ssif0-audiocodechw"
};
#endif /* WITHI2SHW */

#if WITHSAI1HW

// FPGA/IF codec
// DMA �� �������� SSIF1 - ���������� ����������
// Use arm_hardware_flush

static void r7s721_ssif1_txdma(void)
{
	DMAC3.CHCFG_n |= DMAC3_CHCFG_n_REN;	// REN bit
	// SR (bt 7)
	// Indicates the register set currently selected in register mode.
	// 0: Next0 Register Set
	// 1: Next1 Register Set
	const uint_fast8_t b = (DMAC3.CHSTAT_n & (1U << DMAC3_CHSTAT_n_SR_SHIFT)) != 0;	// SR
	if (b != 0)
	{
		release_dmabuffer32tx(DMAC3.N0SA_n);
		DMAC3.N0SA_n = dma_flush32tx(getfilled_dmabuffer32tx_main());
	}
	else
	{
		release_dmabuffer32tx(DMAC3.N1SA_n);
		DMAC3.N1SA_n = dma_flush32tx(getfilled_dmabuffer32tx_main());
	}
}

// FPGA/IF codec
// DMA �� ������ SSIF1 - ���������� ����������
// Use dma_invalidate32rx

static RAMFUNC_NONILINE void r7s721_ssif1_rxdma(void)
{
	DMAC2.CHCFG_n |= DMAC2_CHCFG_n_REN;	// REN bit
	// SR (bt 7)
	// Indicates the register set currently selected in register mode.
	// 0: Next0 Register Set
	// 1: Next1 Register Set
	const uint_fast8_t b = (DMAC2.CHSTAT_n & (1U << DMAC2_CHSTAT_n_SR_SHIFT)) != 0;	// SR
	if (b != 0)
	{
		processing_dmabuffer32rx(DMAC2.N0DA_n);
		DMAC2.N0DA_n = dma_invalidate32rx(allocate_dmabuffer32rx());
	}
	else
	{
		processing_dmabuffer32rx(DMAC2.N1DA_n);
		DMAC2.N1DA_n = dma_invalidate32rx(allocate_dmabuffer32rx());
	}
}

// FPGA/IF codec
// DMA �� ����� SSIF1
// Use dma_invalidate32rx

static void r7s721_ssif1_dmarx_initialize(void)
{
	const uint_fast8_t id = 2;	// 2: DMAC2
	// DMAC2
	/* Set Source Start Address */
    DMAC2.N0SA_n = (uint32_t) & SSIF1.SSIFRDR;	// Fixed source address
    DMAC2.N1SA_n = (uint32_t) & SSIF1.SSIFRDR;	// Fixed source address

    /* Set Destination Start Address */
	DMAC2.N0DA_n = dma_invalidate32rx(allocate_dmabuffer32rx());
	DMAC2.N1DA_n = dma_invalidate32rx(allocate_dmabuffer32rx());

	/* Set Transfer Size */
	DMAC2.N0TB_n = DMABUFFSIZE32RX * sizeof (int32_t);	// ������ � ������
	DMAC2.N1TB_n = DMABUFFSIZE32RX * sizeof (int32_t);	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// SSIRXI1 (receive data full)
	const uint_fast8_t mid = 0x39;	
	const uint_fast8_t rid = 2;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 0;

	DMAC2.CHCFG_n =
		0 * (1U << 31) |	// DMS	0: Register mode
		1 * (1U << 30) |	// REN	1: Continues DMA transfers.
		1 * (1U << 29) |	// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << 28) |	// RSEL	0: Executes the Next0 Register Set
		0 * (1U << 27) |	// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << 24) |	// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << 22) |	// TM	0: Single transfer mode - ������ �� Table 9.4
		0 * (1U << 21) |	// DAD	0: Increment destination address
		1 * (1U << 20) |	// SAD	1: Fixed source address
		2 * (1U << 16) |	// DDS	2: 32 bits (Destination Data Size)
		2 * (1U << 12) |	// SDS	2: 32 bits (Source Data Size)
		am * (1U << 8) |	// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << 6) |	// LVL	1: Detects based on the level.
		1 * (1U << 5) |		// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << 3) |	// REQD		Request Direction
		(id & 0x07) * (1U << 0) |		// SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC23.DMARS = (DMAC23.DMARS & ~ (0x1FFul << dmarsshift)) |
		mid * (1U << (2 + dmarsshift)) |		// MID
		rid * (1U << (0 + dmarsshift)) |		// RID
		0;

    DMAC07.DCTRL_0_7 = (DMAC07.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT2_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_ssif1_rxdma);
		GIC_SetPriority(int_id, ARM_REALTIME_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC2.CHCTRL_n = 1 * (1U << 3);		// SWRST
	DMAC2.CHCTRL_n = 1 * (1U << 17);	// CLRINTMSK
	DMAC2.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

// FPGA/IF codec
// DMA �� �������� SSIF1
// Use arm_hardware_flush

static void r7s721_ssif1_dmatx_initialize(void)
{
	enum { id = 3 };	// 3: DMAC3
	// DMAC3
   /* Set Source Start Address */
	DMAC3.N0SA_n = dma_flush32tx(allocate_dmabuffer32tx());
	DMAC3.N1SA_n = dma_flush32tx(allocate_dmabuffer32tx());

    /* Set Destination Start Address */
    DMAC3.N0DA_n = (uint32_t) & SSIF1.SSIFTDR;	// Fixed destination address
    DMAC3.N1DA_n = (uint32_t) & SSIF1.SSIFTDR;	// Fixed destination address

    /* Set Transfer Size */
    DMAC3.N0TB_n = DMABUFFSIZE32TX * sizeof (int32_t);	// ������ � ������
    DMAC3.N1TB_n = DMABUFFSIZE32TX * sizeof (int32_t);	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// SSITXI1 (transmit data empty)
	const uint_fast8_t mid = 0x39;	
	const uint_fast8_t rid = 1;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 1;	

	DMAC3.CHCFG_n =
		0 * (1U << 31) |	// DMS	0: Register mode
		1 * (1U << 30) |	// REN	1: Continues DMA transfers.
		1 * (1U << 29) |	// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << 28) |	// RSEL	0: Executes the Next0 Register Set
		0 * (1U << 27) |	// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << 24) |	// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << 22) |	// TM	0: Single transfer mode - ������ �� Table 9.4
		1 * (1U << 21) |	// DAD	1: Fixed destination address
		0 * (1U << 20) |	// SAD	0: Increment source address
		2 * (1U << 16) |	// DDS	2: 32 bits (Destination Data Size)
		2 * (1U << 12) |	// SDS	2: 32 bits (Source Data Size)
		am * (1U << 8) |	// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << 6) |	// LVL	1: Detects based on the level.
		1 * (1U << 5) |		// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << 3) |	// REQD		Request Direction
		(id & 0x07) * (1U << 0) |		// SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC23.DMARS = (DMAC23.DMARS & ~ (0x1FFul << dmarsshift)) |
		mid * (1U << (2 + dmarsshift)) |		// MID
		rid * (1U << (0 + dmarsshift)) |		// RID
		0;

    DMAC07.DCTRL_0_7 = (DMAC07.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT3_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_ssif1_txdma);
		GIC_SetPriority(int_id, ARM_REALTIME_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC3.CHCTRL_n = 1 * (1U << 3);		// SWRST
	DMAC3.CHCTRL_n = 1 * (1U << 17);	// CLRINTMSK
	DMAC3.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

// FGA I2S INTERFACE #1
static void r7s721_ssif1_fullduplex_initialize(void)
{
	const uint_fast8_t master = R7S721_MASTER;
    /* ---- Supply clock to the SSIF(channel 1) ---- */
	CPG.STBCR11 &= ~ (1U << 4);	// Module Stop 114	- 0: Channel 1 of the serial sound interface runs.
	(void) CPG.STBCR11;			/* Dummy read */

	// I2S compatible
	// SCKP = 0, SWSP = 0, DEL = 0, CHNL = 00
	// System word length = data word length

	// Control Register (SSICR)
	SSIF1.SSICR = 
		R7S721_USE_AUDIO_CLK * (1UL << 30) |		// CKS 1: AUDIO_CLK input 0: AUDIO_X1 input
		((WITHSAI1_FRAMEBITS / 64) - 1) * (1UL << 22) |		// CHNL		00: Having one channel per system word (I2S complaint)
		6 * (1UL << 19) |		// DWL 6: 32 bit	
#if WITHSAI1_FRAMEBITS == 64
		3 * (1UL << 16) |		// SWL 3: 32 bit, 6: 128 bit, 7: 256 bit
#elif WITHSAI1_FRAMEBITS == 256
		6 * (1UL << 16) |		// SWL 3: 32 bit, 6: 128 bit, 7: 256 bit
#endif /*  */
		master * (1UL << 15) |		// SCKD	1: Serial bit clock is output, master mode.
		master * (1UL << 14) |		// SWSD	1: Serial word select is output, master mode.
		0 * (1UL << 13) |		// SCKP	0: ������ �� ������ �������� �� ���������� ������ (I2S complaint)
		0 * (1UL << 12) |		// SWSP	0: SSIWS is low for the 1st channel, high for the 2nd channel.  (I2S complaint)
		0 * (1UL << 11) |		// SPDP 0: Padding bits are low.	
		0 * (1UL << 10) |		// SDTA	
		0 * (1UL << 9) |		// PDTA	
#if WITHSAI1_FORMATI2S_PHILIPS
		0 * (1UL << 8) |		// DEL	0: 1 clock cycle delay between SSIWS and SSIDATA
#else /* WITHSAI1_FORMATI2S_PHILIPS */
		1 * (1UL << 8) |		// DEL	1: No delay between SSIWS and SSIDATA
#endif /* WITHSAI1_FORMATI2S_PHILIPS */
		master * R7S721_SSIF_CKDIV1 * (1UL << 4) |		// CKDV	0000: AUDIO�/4: 12,288 -> 12,288 (48 kS, 128 bit, stereo)
		0;

	// FIFO Control Register (SSIFCR)
	SSIF1.SSIFCR = 
		2 * (1uL << 6) |	// TTRG Transmit Data Trigger Number
		2 * (1uL << 4) |	// RTRG Receive Data Trigger Number
		1 * (1UL << 3) |		// TIE	Transmit Interrupt Enable
		1 * (1UL << 2) |		// RIE	Receive Interrupt Enable	
		//1 * (1UL << 1) |		// TFRST Transmit FIFO Data Register Reset
		//1 * (1UL << 0) |		// RFRST Receive FIFO Data Register Reset	
		0;


	HARDWARE_SSIF1_INITIALIZE();	// ����������� ��������� ������������� ����� � ������� ����������
}

static void r7s721_ssif1_fullduplex_enable(void)
{
	SSIF1.SSICR |= 
		1 * (1UL << 1) |		// TEN	
		1 * (1UL << 0) |		// REN	
		0;
	//local_delay_ms(20);
	//SSIF1.SSISR = ~ (1U << 28);	// TOIRQ
}

static const codechw_t fpgacodechw =
{
	r7s721_ssif1_fullduplex_initialize,
	hardware_dummy_initialize,
	r7s721_ssif1_dmarx_initialize,
	r7s721_ssif1_dmatx_initialize,
	r7s721_ssif1_fullduplex_enable,
	hardware_dummy_enable,
	"ssif1-audiocodechw"
};

#endif /* WITHSAI1HW */

#if WITHSAI2HW

// FPGA/spectrum channel
// DMA �� ������ SSIF2 - ���������� ����������

static RAMFUNC_NONILINE void r7s721_ssif2_rxdma_handler(void)
{
	DMAC4.CHCFG_n |= DMAC4_CHCFG_n_REN;	// REN bit
	// SR (bt 7)
	// Indicates the register set currently selected in register mode.
	// 0: Next0 Register Set
	// 1: Next1 Register Set
	const uint_fast8_t b = (DMAC4.CHSTAT_n & (1U << DMAC4_CHSTAT_n_SR_SHIFT)) != 0;	// SR
	if (b != 0)
	{
		processing_dmabuffer32rts(DMAC4.N0DA_n);
		DMAC4.N0DA_n = dma_invalidate192rts(allocate_dmabuffer192rts());
	}
	else
	{
		processing_dmabuffer32rts(DMAC4.N1DA_n);
		DMAC4.N1DA_n = dma_invalidate192rts(allocate_dmabuffer192rts());
	}
}

// FPGA/spectrum channel
// DMA �� ����� SSIF2

static void r7s721_ssif2_dmarx_initialize(void)
{
	enum { id = 4 };	// 4: DMAC4
	// DMAC4
	/* Set Source Start Address */
    DMAC4.N0SA_n = (uint32_t) & SSIF2.SSIFRDR;	// Fixed source address
    DMAC4.N1SA_n = (uint32_t) & SSIF2.SSIFRDR;	// Fixed source address

    /* Set Destination Start Address */
	DMAC4.N0DA_n = dma_invalidate192rts(allocate_dmabuffer192rts());
	DMAC4.N1DA_n = dma_invalidate192rts(allocate_dmabuffer192rts());

    /* Set Transfer Size */
    DMAC4.N0TB_n = DMABUFFSIZE192RTS * sizeof (uint8_t);	// ������ � ������
    DMAC4.N1TB_n = DMABUFFSIZE192RTS * sizeof (uint8_t);	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// SSIRTI2 (receive data full)
	const uint_fast8_t mid = 0x3a;	
	const uint_fast8_t rid = 3;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 0;

	DMAC4.CHCFG_n =
		0 * (1U << 31) |	// DMS	0: Register mode
		1 * (1U << 30) |	// REN	1: Continues DMA transfers.
		1 * (1U << 29) |	// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << 28) |	// RSEL	0: Executes the Next0 Register Set
		0 * (1U << 27) |	// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << 24) |	// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << 22) |	// TM	0: Single transfer mode - ������ �� Table 9.4
		0 * (1U << 21) |	// DAD	0: Increment destination address
		1 * (1U << 20) |	// SAD	1: Fixed source address
		2 * (1U << 16) |	// DDS	2: 32 bits (Destination Data Size)
		2 * (1U << 12) |	// SDS	2: 32 bits (Source Data Size)
		am * (1U << 8) |	// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << 6) |	// LVL	1: Detects based on the level.
		1 * (1U << 5) |		// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << 3) |	// REQD		Request Direction
		(id & 0x07) * (1U << 0) |		// SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC45.DMARS = (DMAC45.DMARS & ~ (0x1FFul << dmarsshift)) |
		mid * (1U << (2 + dmarsshift)) |		// MID
		rid * (1U << (0 + dmarsshift)) |		// RID
		0;

    DMAC07.DCTRL_0_7 = (DMAC07.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT4_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_ssif2_rxdma_handler);
		GIC_SetPriority(int_id, ARM_REALTIME_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC4.CHCTRL_n = 1 * (1U << 3);		// SWRST
	DMAC4.CHCTRL_n = 1 * (1U << 17);	// CLRINTMSK
	DMAC4.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

// FGA I2S INTERFACE #2
// FPGA/spectrum channel
static void r7s721_ssif2_rx_initialize(void)
{
	const uint_fast8_t master = R7S721_MASTER;
    /* ---- Supply clock to the SSIF(channel 1) ---- */
	CPG.STBCR11 &= ~ (1U << 3);	// Module Stop 113	- 0: Channel 2 of the serial sound interface runs.
	(void) CPG.STBCR11;			/* Dummy read */

	// I2S compatible
	// SCKP = 0, SWSP = 0, DEL = 0, CHNL = 00
	// System word length = data word length

	// Control Register (SSICR)
	SSIF2.SSICR = 
		R7S721_USE_AUDIO_CLK * (1UL << 30) |		// CKS 1: AUDIO_CLK input 0: AUDIO_X1 input
		((WITHSAI2_FRAMEBITS / 64) - 1) * (1UL << 22) |		// CHNL		00: Having one channel per system word (I2S complaint)
		6 * (1UL << 19) |		// DWL 6: 32 bit	
#if WITHSAI2_FRAMEBITS == 64
		3 * (1UL << 16) |		// SWL 3: 32 bit, 6: 128 bit, 7: 256 bit
#elif WITHSAI2_FRAMEBITS == 256
		6 * (1UL << 16) |		// SWL 3: 32 bit, 6: 128 bit, 7: 256 bit
#endif /*  */
		master * (1UL << 15) |		// SCKD	1: Serial bit clock is output, master mode.
		master * (1UL << 14) |		// SWSD	1: Serial word select is output, master mode.
		0 * (1UL << 13) |		// SCKP	0: ������ �� ������ �������� �� ���������� ������ (I2S complaint)
		0 * (1UL << 12) |		// SWSP	0: SSIWS is low for the 1st channel, high for the 2nd channel.  (I2S complaint)
		0 * (1UL << 11) |		// SPDP 0: Padding bits are low.	
		0 * (1UL << 10) |		// SDTA	
		0 * (1UL << 9) |		// PDTA	
#if WITHSAI2_FORMATI2S_PHILIPS
		0 * (1UL << 8) |		// DEL	0: 1 clock cycle delay between SSIWS and SSIDATA
#else /* WITHSAI1_FORMATI2S_PHILIPS */
		1 * (1UL << 8) |		// DEL	1: No delay between SSIWS and SSIDATA
#endif /* WITHSAI1_FORMATI2S_PHILIPS */
		master * R7S721_SSIF_CKDIV1 * (1UL << 4) |		// CKDV	0000: AUDIO�/4: 12,288 -> 12,288 (48 kS, 128 bit, stereo)
		0;

	// FIFO Control Register (SSIFCR)
	SSIF2.SSIFCR = 
		//2 * (1uL << 6) |	// TTRG Transmit Data Trigger Number
		2 * (1uL << 4) |	// RTRG Receive Data Trigger Number
		//1 * (1UL << 3) |		// TIE	Transmit Interrupt Enable
		1 * (1UL << 2) |		// RIE	Receive Interrupt Enable	
		//1 * (1UL << 1) |		// TFRST Transmit FIFO Data Register Reset
		//1 * (1UL << 0) |		// RFRST Receive FIFO Data Register Reset	
		0;


	HARDWARE_SSIF2_INITIALIZE();	// ����������� ��������� ������������� ����� � ������� ����������
}

// FPGA/spectrum channel
static void r7s721_ssif2_rx_enable(void)
{
	SSIF2.SSICR |= 
		1 * (1UL << 0) |		// REN	
		0;
}

static const codechw_t fpgaspectrumhw =
{
	r7s721_ssif2_rx_initialize,
	hardware_dummy_initialize,
	r7s721_ssif2_dmarx_initialize,
	hardware_dummy_initialize,
	r7s721_ssif2_rx_enable,
	hardware_dummy_enable,
	"spectrumhw"
};

#endif /* WITHSAI2HW */

#else
	// other CPUs
static const codechw_t fpgaspectrumhw =
{
	hardware_sai2_slave_fullduplex_initialize,	// added...
	hardware_dummy_initialize,
	DMA_SAI2_B_RX_initializeWFM,
	hardware_dummy_initialize,
	hardware_sai2_enable,
	hardware_dummy_enable,
	"sai2-fpga spectrum for WFM"
};

#endif /* CPUSTYLE_STM32F */


#if WITHI2SHW

// ��������� � �� ������
// ���������� ������, ���������� ��� ����������� �����������
void hardware_audiocodec_enable(void)
{
	audiocodechw.initializedma_rx();
	audiocodechw.initializedma_tx();
	audiocodechw.enable_rx();
	audiocodechw.enable_tx();
}

// ��������� � �� ������
void hardware_audiocodec_initialize(void)
{
	debug_printf_P(PSTR("hardware_audiocodec_initialize start\n"));

	audiocodechw.initialize_rx();
	audiocodechw.initialize_tx();

	debug_printf_P(PSTR("hardware_audiocodec_initialize done\n"));
}

#endif /* WITHI2SHW */

#if WITHSAI1HW

// ��������� � �� ������
void hardware_fpgacodec_enable(void)
{
	fpgacodechw.initializedma_rx();
	fpgacodechw.initializedma_tx();
	fpgacodechw.enable_rx();
	fpgacodechw.enable_tx();
}

// ������������� ���� ������� ���������� � FPGA ��� � IF CODEC
// ��������� � �� ������
// ���� ���� WITHSAICLOCKFROMI2S	- ��� ���������������� ����� I2S
void hardware_fpgacodec_initialize(void)
{
	debug_printf_P(PSTR("hardware_fpgacodec_initialize start\n"));

	fpgacodechw.initialize_rx();
	fpgacodechw.initialize_tx();

	debug_printf_P(PSTR("hardware_fpgacodec_initialize done\n"));
}

#endif /* WITHSAI1HW */


#if WITHSAI2HW

#if WITHSUSBSPKONLY

// ��������� � �� ������
// ���������� ������, ���������� ��� ����������� �����������
void hardware_fpgaspectrum_enable(void)
{
	debug_printf_P(PSTR("hardware_fpgaspectrum_enable start: %s\n"), audiocodechw.label);
	audiocodechw.initializedma_rx();
	audiocodechw.initializedma_tx();
	audiocodechw.enable_rx();
	audiocodechw.enable_tx();
	debug_printf_P(PSTR("hardware_fpgaspectrum_enable done\n"));
}

// ��������� � �� ������
void hardware_fpgaspectrum_initialize(void)
{
	debug_printf_P(PSTR("hardware_fpgaspectrum_initialize start: %s\n"), audiocodechw.label);

	audiocodechw.initialize_rx();
	audiocodechw.initialize_tx();

	debug_printf_P(PSTR("hardware_fpgaspectrum_initialize done\n"));
}

#else /* WITHSUSBSPKONLY */
// ��������� � ��������� ������ � �������
void hardware_fpgaspectrum_enable(void)
{
	fpgaspectrumhw.initializedma_rx();
	//fpgaspectrumhw.initializedma_tx();
	fpgaspectrumhw.enable_rx();
	//fpgaspectrumhw.enable_tx();
}

// ������������� ���� ������� ���������� � FPGA ��� � IF CODEC
// ��������� � ��������� ������ � �������
// ���� ���� WITHSAICLOCKFROMI2S	- ��� ���������������� ����� I2S
void hardware_fpgaspectrum_initialize(void)
{
	debug_printf_P(PSTR("hardware_fpgaspectrum_initialize start: %s\n"), fpgaspectrumhw.label);
	fpgaspectrumhw.initialize_rx();
	//fpgaspectrumhw.initialize_tx();
	debug_printf_P(PSTR("hardware_fpgaspectrum_initialize done\n"));
}
#endif /* WITHSUSBSPKONLY */

#endif /* WITHSAI2HW */

#endif /* WITHINTEGRATEDDSP */

#if WITHCPUDACHW

void hardware_dac_initialize(void)		/* ������������� DAC �� STM32F4xx */
{
	debug_printf_P(PSTR("hardware_dac_initialize start\n"));
#if CPUSTYLE_STM32H7XX

	RCC->APB1LENR |= RCC_APB1LENR_DAC12EN; //������ ������������ 
	__DSB();

#elif CPUSTYLE_STM32F

	RCC->APB1ENR |= RCC_APB1ENR_DACEN; //������ ������������ 
	__DSB();

#endif
	HARDWARE_DAC_INITIALIZE();	/* �������� ������ ������ */
	debug_printf_P(PSTR("hardware_dac_initialize done\n"));
}
// ����� 12-������� �������� �� ��� - ����� 1
void hardware_dac_ch1_setvalue(uint_fast16_t v)
{
	DAC1->DHR12R1 = v;
}

// ����� 12-������� �������� �� ��� - ����� 2
void hardware_dac_ch2_setvalue(uint_fast16_t v)
{
	DAC1->DHR12R2 = v;
}

#else

void hardware_dac_initialize(void)		/* ������������� DAC �� STM32F4xx */
{
}
// ����� 12-������� �������� �� ��� - ����� 1
void hardware_dac_ch1_setvalue(uint_fast16_t v)
{
}
// ����� 12-������� �������� �� ��� - ����� 2
void hardware_dac_ch2_setvalue(uint_fast16_t v)
{
}
#endif /* WITHCPUDACHW */

#if WITHUSBHW

#if CPUSTYLE_R7S721

#if WITHUSBUAC

// USB AUDIO
// ����� DMA ��� ����� - ��������� � �������, ����� �������� ������ ����� getfilled_dmabufferx
void refreshDMA_uacin(void)
{
	if ((DMAC12.CHSTAT_n & DMAC12_CHSTAT_n_EN) != 0)
	{
		// ����� DMA ��� ����� - ����� ������ �� ���������.
		return;
	}
	if (DMAC12.N0SA_n != 0)
	{
		// �������� - ���������� �� ����� �������� �������� ������,
		// �� ��� �� ����������.
		return;
	}

	// ��� ������� ���������� ����� - ��������� ��������
	uint_fast16_t size;
	const uintptr_t addr = getfilled_dmabufferx(& size);	// ��� �������� � ��������� - ����� ������� 0
	if (addr != 0)
	{
		const uint_fast8_t pipe = HARDWARE_USBD_PIPE_ISOC_IN;	// PIPE2
		WITHUSBHW_DEVICE->PIPESEL = pipe << USB_PIPESEL_PIPESEL_SHIFT;
		while ((WITHUSBHW_DEVICE->PIPESEL & USB_PIPESEL_PIPESEL) != (pipe << USB_PIPESEL_PIPESEL_SHIFT))
			;
		WITHUSBHW_DEVICE->PIPEMAXP = size << USB_PIPEMAXP_MXPS_SHIFT;
		WITHUSBHW_DEVICE->PIPESEL = 0;

		DMAC12.N0SA_n = dma_flushxrtstx(addr, size);
		DMAC12.N0TB_n = size;	// ������ � ������

		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_SWRST;		// SWRST
		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_CLRINTMSK;	// CLRINTMSK
		DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_SETEN;		// SETEN
	}
	else
	{
		//DMAC12.N0SA_n = 0;
		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_CLREN;		// CLREN
		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_SWRST;		// SWRST
		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_CLRINTMSK;	// CLRINTMSK
	}
}

// USB AUDIO
// DMA �� �������� USB0 DMA1 - ���������� ����������
// DMA �� �������� USB1 DMA1 - ���������� ����������
// Use arm_hardware_flush
// ARM_REALTIME_PRIORITY

void r7s721_usbX_dma1_dmatx_handler(void)
{
	ASSERT(DMAC12.N0SA_n != 0);
	release_dmabufferx(DMAC12.N0SA_n);

	// ��� ������� ���������� ����� - ��������� ��������
	uint_fast16_t size;
	const uintptr_t addr = getfilled_dmabufferx(& size);	// ��� �������� � ��������� - ����� ������� 0
	if (addr != 0)
	{
		const uint_fast8_t pipe = HARDWARE_USBD_PIPE_ISOC_IN;	// PIPE2
		WITHUSBHW_DEVICE->PIPESEL = pipe << USB_PIPESEL_PIPESEL_SHIFT;
		while ((WITHUSBHW_DEVICE->PIPESEL & USB_PIPESEL_PIPESEL) != (pipe << USB_PIPESEL_PIPESEL_SHIFT))
				;
		WITHUSBHW_DEVICE->PIPEMAXP = size << USB_PIPEMAXP_MXPS_SHIFT;
		WITHUSBHW_DEVICE->PIPESEL = 0;

		DMAC12.N0SA_n = dma_flushxrtstx(addr, size);
		DMAC12.N0TB_n = size;	// ������ � ������

		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_SWRST;		// SWRST
		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_CLRINTMSK;	// CLRINTMSK
		DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_SETEN;		// SETEN
	}
	else
	{
		DMAC12.N0SA_n = 0;
		DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_CLREN;		// CLREN
		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_SWRST;		// SWRST
		//DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_CLRINTMSK;	// CLRINTMSK
	}
}


// audio codec
// DMA �� �������� USB0 DMA1
// Use arm_hardware_flush

static void r7s721_usb0_dma1_dmatx_initialize(void)
{
	const uint_fast8_t id = 12;	// 12: DMAC12
	// DMAC12
	/* Set Source Start Address */

    /* Set Destination Start Address */
    DMAC12.N0DA_n = (uint32_t) & WITHUSBHW_DEVICE->D1FIFO.UINT32;	// Fixed destination address
    //DMAC12.N1DA_n = (uint32_t) & WITHUSBHW_DEVICE->D1FIFO.UINT32;	// Fixed destination address

    /* Set Transfer Size */
    //DMAC12.N0TB_n = DMABUFFSIZE16 * sizeof (int16_t);	// ������ � ������
    //DMAC12.N1TB_n = DMABUFFSIZE16 * sizeof (int16_t);	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// USB0_DMA1 (channel 1 transmit FIFO empty)
	const uint_fast8_t mid = 0x21;	
	const uint_fast8_t rid = 3;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 1;	
	const uint_fast8_t hien = 1;	

	DMAC12.CHCFG_n =
		0 * (1U << DMAC12_CHCFG_n_DMS_SHIFT) |		// DMS	0: Register mode
		0 * (1U << DMAC12_CHCFG_n_REN_SHIFT) |		// REN	0: Does not continue DMA transfers.
		0 * (1U << DMAC12_CHCFG_n_RSW_SHIFT) |		// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << DMAC12_CHCFG_n_RSEL_SHIFT) |		// RSEL	0: Executes the Next0 Register Set
		0 * (1U << DMAC12_CHCFG_n_SBE_SHIFT) |		// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << DMAC12_CHCFG_n_DEM_SHIFT) |		// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << DMAC12_CHCFG_n_TM_SHIFT) |		// TM	0: Single transfer mode - ������ �� Table 9.4
		1 * (1U << DMAC12_CHCFG_n_DAD_SHIFT) |		// DAD	1: Fixed destination address
		0 * (1U << DMAC12_CHCFG_n_SAD_SHIFT) |		// SAD	0: Increment source address
		2 * (1U << DMAC12_CHCFG_n_DDS_SHIFT) |		// DDS	2: 32 bits, 1: 16 bits (Destination Data Size)
		2 * (1U << DMAC12_CHCFG_n_SDS_SHIFT) |		// SDS	2: 32 bits, 1: 16 bits (Source Data Size)
		am * (1U << DMAC12_CHCFG_n_AM_SHIFT) |		// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << DMAC12_CHCFG_n_LVL_SHIFT) |	// LVL	1: Detects based on the level.
		hien * (1U << DMAC12_CHCFG_n_HIEN_SHIFT) |	// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << DMAC12_CHCFG_n_REQD_SHIFT) |	// REQD		Request Direction
		(id & 0x07) * (1U << DMAC12_CHCFG_n_SEL_SHIFT) | // SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC1213.DMARS = (DMAC1213.DMARS & ~ ((DMAC1213_DMARS_CH12_MID | DMAC1213_DMARS_CH12_RID) << dmarsshift)) |
		mid * (1U << (DMAC1213_DMARS_CH12_MID_SHIFT + dmarsshift)) |		// MID
		rid * (1U << (DMAC1213_DMARS_CH12_RID_SHIFT + dmarsshift)) |		// RID
		0;

    DMAC815.DCTRL_0_7 = (DMAC815.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT12_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_usbX_dma1_dmatx_handler);
		GIC_SetPriority(int_id, ARM_REALTIME_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_SWRST;		// SWRST
	DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_CLRINTMSK;	// CLRINTMSK
	//DMAC12.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

static USBALIGN_BEGIN uint8_t uacoutbuff0 [VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT] USBALIGN_END;
static USBALIGN_BEGIN uint8_t uacoutbuff1 [VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT] USBALIGN_END;

// USB AUDIO
// DMA �� ������ USB0 DMA0 - ���������� ����������
// DMA �� ������ USB1 DMA0 - ���������� ����������
// �������� �� ARM_SYSTEM_PRIORITY

static RAMFUNC_NONILINE void r7s721_usbX_dma0_dmarx_handler(void)
{
	// Enable switch to next regidters set
	DMAC13.CHCFG_n |= DMAC13_CHCFG_n_REN;	// REN bit
	// SR (bt 7)
	// Indicates the register set currently selected in register mode.
	// 0: Next0 Register Set
	// 1: Next1 Register Set
	const uint_fast8_t b = (DMAC13.CHSTAT_n & DMAC13_CHSTAT_n_SR) != 0;	// SR
	// ���� � ������ ������ ���������� �� ����������� �� �������� � ����� (������� r7s721_ssif0_txdma). 
	// ����� � ����������������� ������ ����� �� �����������.
	if (b == 0)
	{
		uacout_buffer_save(uacoutbuff0, VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT);
		arm_hardware_flush_invalidate((uintptr_t) uacoutbuff0, VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT);
	}
	else
	{
		uacout_buffer_save(uacoutbuff1, VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT);
		arm_hardware_flush_invalidate((uintptr_t) uacoutbuff1, VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT);
	}
}


// USB AUDIO
// DMA �� ����� usb0_dma0

static void r7s721_usb0_dma0_dmarx_initialize(void)
{
	arm_hardware_flush_invalidate((uintptr_t) uacoutbuff0, VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT);
	arm_hardware_flush_invalidate((uintptr_t) uacoutbuff1, VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT);

	const uint_fast8_t id = 13;		// 13: DMAC13
	// DMAC13
	/* Set Source Start Address */
	/* �������� USB PIPE (HARDWARE_USBD_PIPE_ISOC_OUT) */
    DMAC13.N0SA_n = (uint32_t) & WITHUSBHW_DEVICE->D0FIFO.UINT32;	// Fixed source address
    DMAC13.N1SA_n = (uint32_t) & WITHUSBHW_DEVICE->D0FIFO.UINT32;	// Fixed source address

	/* Set Destination Start Address */
	DMAC13.N0DA_n = (uintptr_t) uacoutbuff0;
	DMAC13.N1DA_n = (uintptr_t) uacoutbuff1;

    /* Set Transfer Size */
    DMAC13.N0TB_n = VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT;	// ������ � ������
    DMAC13.N1TB_n = VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT;	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// USB0_DMA0 (channel 0 receive FIFO full)
	const uint_fast8_t mid = 0x20;	
	const uint_fast8_t rid = 3;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 0;
	const uint_fast8_t hien = 1;	

	DMAC13.CHCFG_n =
		0 * (1U << 31) |	// DMS	0: Register mode
		1 * (1U << 30) |	// REN	1: Continues DMA transfers.
		1 * (1U << 29) |	// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << 28) |	// RSEL	0: Executes the Next0 Register Set
		0 * (1U << 27) |	// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << 24) |	// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << 22) |	// TM	0: Single transfer mode - ������ �� Table 9.4
		0 * (1U << 21) |	// DAD	0: Increment destination address
		1 * (1U << 20) |	// SAD	1: Fixed source address
		2 * (1U << 16) |	// DDS	2: 32 bits, 1: 16 bits (Destination Data Size)
		2 * (1U << 12) |	// SDS	2: 32 bits, 1: 16 bits (Source Data Size)
		am * (1U << 8) |	// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << 6) |	// LVL	1: Detects based on the level.
		hien * (1U << 5) |	// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << 3) |	// REQD		Request Direction
		(id & 0x07) * (1U << 0) |		// SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC1213.DMARS = (DMAC1213.DMARS & ~ (0x1FFul << dmarsshift)) |
		mid * (1U << (2 + dmarsshift)) |		// MID
		rid * (1U << (0 + dmarsshift)) |		// RID
		0;

    DMAC815.DCTRL_0_7 = (DMAC815.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT13_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_usbX_dma0_dmarx_handler);
		GIC_SetPriority(int_id, ARM_SYSTEM_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC13.CHCTRL_n = 1 * (1U << 3);		// SWRST
	DMAC13.CHCTRL_n = 1 * (1U << 17);	// CLRINTMSK
	DMAC13.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

static void r7s721_usb0_dma0_dmarx_enable(void)
{
	// ������������� ����� ������� � ��������� FIFO
	//const uint_fast8_t pipe = HARDWARE_USBD_PIPE_ISOC_OUT;
}

static void r7s721_usb0_dma0_dmatx_enable(void)
{
	// ������������� ����� ������� � ��������� FIFO
	//const uint_fast8_t pipe = HARDWARE_USBD_PIPE_ISOC_IN;
}

// audio codec
// DMA �� �������� USB1 DMA1
// Use arm_hardware_flush

static void r7s721_usb1_dma1_dmatx_initialize(void)
{
	const uint_fast8_t id = 12;	// 12: DMAC12
	// DMAC12
	/* Set Source Start Address */

    /* Set Destination Start Address */
    DMAC12.N0DA_n = (uint32_t) & WITHUSBHW_DEVICE->D1FIFO.UINT32;	// Fixed destination address
    //DMAC12.N1DA_n = (uint32_t) & WITHUSBHW_DEVICE->D1FIFO.UINT32;	// Fixed destination address

    /* Set Transfer Size */
    //DMAC12.N0TB_n = DMABUFFSIZE16 * sizeof (int16_t);	// ������ � ������
    //DMAC12.N1TB_n = DMABUFFSIZE16 * sizeof (int16_t);	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// USB1_DMA1 (channel 1 transmit FIFO empty)
	const uint_fast8_t mid = 0x23;	
	const uint_fast8_t rid = 3;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 1;	
	const uint_fast8_t hien = 1;	

	DMAC12.CHCFG_n =
		0 * (1U << DMAC12_CHCFG_n_DMS_SHIFT) |		// DMS	0: Register mode
		0 * (1U << DMAC12_CHCFG_n_REN_SHIFT) |		// REN	0: Does not continue DMA transfers.
		0 * (1U << DMAC12_CHCFG_n_RSW_SHIFT) |		// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << DMAC12_CHCFG_n_RSEL_SHIFT) |		// RSEL	0: Executes the Next0 Register Set
		0 * (1U << DMAC12_CHCFG_n_SBE_SHIFT) |		// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << DMAC12_CHCFG_n_DEM_SHIFT) |		// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << DMAC12_CHCFG_n_TM_SHIFT) |		// TM	0: Single transfer mode - ������ �� Table 9.4
		1 * (1U << DMAC12_CHCFG_n_DAD_SHIFT) |		// DAD	1: Fixed destination address
		0 * (1U << DMAC12_CHCFG_n_SAD_SHIFT) |		// SAD	0: Increment source address
		2 * (1U << DMAC12_CHCFG_n_DDS_SHIFT) |		// DDS	2: 32 bits, 1: 16 bits (Destination Data Size)
		2 * (1U << DMAC12_CHCFG_n_SDS_SHIFT) |		// SDS	2: 32 bits, 1: 16 bits (Source Data Size)
		am * (1U << DMAC12_CHCFG_n_AM_SHIFT) |		// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << DMAC12_CHCFG_n_LVL_SHIFT) |	// LVL	1: Detects based on the level.
		hien * (1U << DMAC12_CHCFG_n_HIEN_SHIFT) |	// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << DMAC12_CHCFG_n_REQD_SHIFT) |	// REQD		Request Direction
		(id & 0x07) * (1U << DMAC12_CHCFG_n_SEL_SHIFT) | // SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC1213.DMARS = (DMAC1213.DMARS & ~ ((DMAC1213_DMARS_CH12_MID | DMAC1213_DMARS_CH12_RID) << dmarsshift)) |
		mid * (1U << (DMAC1213_DMARS_CH12_MID_SHIFT + dmarsshift)) |		// MID
		rid * (1U << (DMAC1213_DMARS_CH12_RID_SHIFT + dmarsshift)) |		// RID
		0;

    DMAC815.DCTRL_0_7 = (DMAC815.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT12_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_usbX_dma1_dmatx_handler);
		GIC_SetPriority(int_id, ARM_REALTIME_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_SWRST;		// SWRST
	DMAC12.CHCTRL_n = DMAC12_CHCTRL_n_CLRINTMSK;	// CLRINTMSK
	//DMAC12.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

// USB AUDIO
// DMA �� ����� usb0_dma0

static void r7s721_usb1_dma0_dmarx_initialize(void)
{
	arm_hardware_flush_invalidate((uintptr_t) uacoutbuff0, VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT);
	arm_hardware_flush_invalidate((uintptr_t) uacoutbuff1, VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT);

	const uint_fast8_t id = 13;		// 13: DMAC13
	// DMAC13
	/* Set Source Start Address */
	/* �������� USB PIPE (HARDWARE_USBD_PIPE_ISOC_OUT) */
    DMAC13.N0SA_n = (uint32_t) & WITHUSBHW_DEVICE->D0FIFO.UINT32;	// Fixed source address
    DMAC13.N1SA_n = (uint32_t) & WITHUSBHW_DEVICE->D0FIFO.UINT32;	// Fixed source address

	/* Set Destination Start Address */
	DMAC13.N0DA_n = (uintptr_t) uacoutbuff0;
	DMAC13.N1DA_n = (uintptr_t) uacoutbuff1;

    /* Set Transfer Size */
    DMAC13.N0TB_n = VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT;	// ������ � ������
    DMAC13.N1TB_n = VIRTUAL_AUDIO_PORT_DATA_SIZE_OUT;	// ������ � ������

	// Values from Table 9.4 On-Chip Peripheral Module Requests
	// USB1_DMA0 (channel 0 receive FIFO full)
	const uint_fast8_t mid = 0x22;	
	const uint_fast8_t rid = 3;		
	const uint_fast8_t tm = 0;		
	const uint_fast8_t am = 2;		
	const uint_fast8_t lvl = 1;		
	const uint_fast8_t reqd = 0;
	const uint_fast8_t hien = 1;	

	DMAC13.CHCFG_n =
		0 * (1U << 31) |	// DMS	0: Register mode
		1 * (1U << 30) |	// REN	1: Continues DMA transfers.
		1 * (1U << 29) |	// RSW	1: Inverts RSEL automatically after a DMA transaction.
		0 * (1U << 28) |	// RSEL	0: Executes the Next0 Register Set
		0 * (1U << 27) |	// SBE	0: Stops the DMA transfer without sweeping the buffer (initial value).
		0 * (1U << 24) |	// DEM	0: Does not mask the DMA transfer end interrupt - ���������� ������ ��� ����� TC
		tm * (1U << 22) |	// TM	0: Single transfer mode - ������ �� Table 9.4
		0 * (1U << 21) |	// DAD	0: Increment destination address
		1 * (1U << 20) |	// SAD	1: Fixed source address
		2 * (1U << 16) |	// DDS	2: 32 bits, 1: 16 bits (Destination Data Size)
		2 * (1U << 12) |	// SDS	2: 32 bits, 1: 16 bits (Source Data Size)
		am * (1U << 8) |	// AM	1: ACK mode: Level mode (active until the transfer request from an on-chip peripheral module
		lvl * (1U << 6) |	// LVL	1: Detects based on the level.
		hien * (1U << 5) |	// HIEN	1: When LVL = 1: Detects a request when the signal is at the High level.
		reqd * (1U << 3) |	// REQD		Request Direction
		(id & 0x07) * (1U << 0) |		// SEL	0: CH0/CH8
		0;

	enum { dmarsshift = (id & 0x01) * 16 };
	DMAC1213.DMARS = (DMAC1213.DMARS & ~ (0x1FFul << dmarsshift)) |
		mid * (1U << (2 + dmarsshift)) |		// MID
		rid * (1U << (0 + dmarsshift)) |		// RID
		0;

    DMAC815.DCTRL_0_7 = (DMAC815.DCTRL_0_7 & ~ (/*(1U << 1) | */(1U << 0))) |
		//1 * (1U << 1) |		// LVINT	1: Level output
		1 * (1U << 0) |		// PR		1: Round robin mode
		0;

	{
		const uint16_t int_id = DMAINT13_IRQn;
		r7s721_intc_registintfunc(int_id, r7s721_usbX_dma0_dmarx_handler);
		GIC_SetPriority(int_id, ARM_SYSTEM_PRIORITY);
		GIC_EnableIRQ(int_id);
	}

	DMAC13.CHCTRL_n = 1 * (1U << 3);		// SWRST
	DMAC13.CHCTRL_n = 1 * (1U << 17);	// CLRINTMSK
	DMAC13.CHCTRL_n = 1 * (1U << 0);		// SETEN
}

static void r7s721_usb1_dma0_dmarx_enable(void)
{
	// ������������� ����� ������� � ��������� FIFO
	//const uint_fast8_t pipe = HARDWARE_USBD_PIPE_ISOC_OUT;
}

static void r7s721_usb1_dma0_dmatx_enable(void)
{
	// ������������� ����� ������� � ��������� FIFO
	//const uint_fast8_t pipe = HARDWARE_USBD_PIPE_ISOC_IN;
}

#endif /* WITHUSBUAC */

#elif CPUSTYLE_STM32F4XX || CPUSTYLE_STM32F7XX || CPUSTYLE_STM32H7XX

// ����� DMA ��� ����� - ��������� � �������, ����� �������� ������ ����� getfilled_dmabufferx � ������ ��������� � host
void refreshDMA_uacin(void)
{
}

#else
	#error Unsupported USB hardware 
#endif


/* ���������� ��� ����������� �����������. */
void hardware_usbd_dma_initialize(void)
{
#if CPUSTYLE_R7S721
#if WITHUSBUAC
	if (WITHUSBHW_DEVICE == & USB200)
	{
		r7s721_usb0_dma0_dmarx_initialize();
		r7s721_usb0_dma1_dmatx_initialize();
	}
	else if (WITHUSBHW_DEVICE == & USB201)
	{
		r7s721_usb1_dma0_dmarx_initialize();
		r7s721_usb1_dma1_dmatx_initialize();
	}
#endif /* WITHUSBUAC */
#elif CPUSTYLE_STM32F4XX || CPUSTYLE_STM32F7XX || CPUSTYLE_STM32H7XX

#else
	#error Unsupported USB hardware 
#endif
}

/* ���������� ��� ����������� �����������. */
void hardware_usbd_dma_enable(void)
{
#if CPUSTYLE_R7S721
#if WITHUSBUAC
	r7s721_usb0_dma0_dmarx_enable();
	r7s721_usb0_dma0_dmatx_enable();
#endif /* WITHUSBUAC */
#elif CPUSTYLE_STM32F4XX || CPUSTYLE_STM32F7XX || CPUSTYLE_STM32H7XX

#else
	#error Unsupported USB hardware 
#endif
}

#endif /* WITHUSBHW */
