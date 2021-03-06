/* $Id$ */
/* board-specific CPU attached signals */
//
// ������ HF Dream Receiver (�� ������� �����)
// ����� ���� ����������� mgs2001@mail.ru
// UA1ARN
//

/* ����� STM32F429I-DISCO � ����������� STM32F429ZIT6	*/

/*

FPGA I/Q data interface:
	PE3 - SAI1_SD_B I2S data from FPGA
	PE4 - SAI1_FS_A WS from FPGA
	PE5 - SAI1_SCK_A SCK
	PE6 - SAI1_SD_A I2S data to FPGA

SPI:
	PF7	- SCLK
	PF8	- MISO
	PF9	- MOSI

CHIP SELECTS:
	PC1 - L3GD20 (on board)
	PC2 - TFT LCD (on board)
	PC8 - spare cs
	PC11 - FPGA
	PC12 - CODEC1 CS4272
	PC13 - FRAM FM25L16B

Encoder inputs: 
	PB7 - PHASE A
	PB4 - PHASE B

PTT:
	PA0 - user button, "1" active

ELKEY:
	PA9 - left
	PA10 - right

KEYBOARD ADC:
	PA5 - KI0
	PC3 - KI1
	PC5 - KI2


Free:
	PB4
	PE2
	PD2
	PD4
	PD5
	PD7
	PE2
	PF6
	PG2
	PG3
	PG9
*/

#ifndef ARM_STM32FXXX_TQFP144_CPUSTYLE_V8C_H_INCLUDED
#define ARM_STM32FXXX_TQFP144_CPUSTYLE_V8C_H_INCLUDED 1

//#define HARDWARE_ARM_USEUSART0 1		// US0: PA5/PA6 pins
//#define HARDWARE_ARM_USEUSART1 1		// US1: PA21/PA22 pins

//#define WITHSPI16BIT	1		/* �������� ������������� 16-�� ������ ���� ��� ������ �� SPI */
//#define WITHSPIHW 		1	/* ������������� ����������� ����������� SPI */
//#define WITHSPIHWDMA 	1	/* ������������� DMA ��� ������ �� SPI */
#define WITHSPISW 	1	/* ������������� ������������ ���������� SPI. ������ ������� ��� ������ - ��������� ����� ���������� ��-�� ��������� � I2C */
//#define SPI_BIDIRECTIONAL 1	// ���� ������ �� SPI ���� ����� ��� �� ����� pin, ��� � �����

//#define WITHTWIHW 	1	/* ������������� ����������� ����������� TWI (I2C) */
#define WITHTWISW 	1	/* ������������� ������������ ����������� TWI (I2C) */
#define WITHCPUADCHW 	1	/* ������������� ADC */

//#define WITHI2SHW	1	/* ������������� I2S - ����������	*/
#define WITHSAI1HW	1	/* ������������� SAI1 - FPGA ��� IF codec	*/

#if 1
	#define WITHUSBHW	1	/* ������������ ���������� � ��������� ��������� USB */
	#define WITHUSBHWVBUSSENSE	1	/* ������������ ���������������� ����� VBUS_SENSE */
	#define WITHUSBHWHIGHSPEED	1	/* ������������ ���������� � ��������� ��������� USB HS */
	//#define WITHUSBHWHIGHSPEEDDESC	1	/* ��������� ����������� ����������� ��� ��� HIGH SPEED */
	#define WITHUSBHW_DEVICE	USB_OTG_HS	/* �� ���� ���������� �������������� ���������������� DEVUCE	*/

	#define WITHUSBUAC		1	/* ������������ ����������� �������� ����� �� USB ���������� */
	#define WITHUSBUAC3		1	/* ����������� ��� ������ �������� ����� */

	#define WITHUSBCDC		1	/* ������������ ����������� ���������������� ���� �� USB ���������� */
	//#define WITHUSBCDCEEM	1	/* EEM ������������ Ethernet Emulation Model �� USB ���������� */
	//#define WITHUSBCDCECM	1	/* ECM ������������ Ethernet Control Model �� USB ���������� */

	#define WITHCAT_CDC		1	/* ������������ ����������� ���������������� ���� �� USB ���������� */
	#define WITHMODEM_CDC	1
#else
	//#define WITHUART1HW	1	/* ������������ ������������ ���������� ����������������� ����� #1 */
	//#define WITHUART2HW	1	/* ������������ ������������ ���������� ����������������� ����� #2 */
	#define WITHCAT_USART1		1
	#define WITHDEBUG_USART1	1
	#define WITHMODEM_USART1	1
	#define WITHNMEA_USART1		1
#endif



//#define BSRR_S(v) ((v) * GPIO_BSRR_BS_0)	/* �������������� �������� ��� ��������� ���� � �������� */
//#define BSRR_C(v) ((v) * GPIO_BSRR_BR_0)	/* �������������� �������� ��� ������ ���� � �������� */

#define LS020_RS_INITIALIZE() \
	do { \
		arm_hardware_piod_outputs2m(LS020_RS, LS020_RS); \
	} while (0)

#define LS020_RESET_INITIALIZE() \
	do { \
		arm_hardware_piod_outputs2m(LS020_RST, LS020_RST); \
	} while (0)


#if LCDMODE_SPI_RN
	// ��� ����������� ������� ������ RESET
	//#define LS020_RESET_PORT_S(v)		do { GPIOD->BSRR = BSRR_S(v); __DSB(); } while (0)
	//#define LS020_RESET_PORT_C(v)		do { GPIOD->BSRR = BSRR_C(v); __DSB(); } while (0)
	//#define LS020_RST			(1u << 8)			// D6 signal

#elif LCDMODE_SPI_RA
	// ��� ����������� ������� RESET � RS
	//#define LS020_RESET_PORT_S(v)		do { GPIOD->BSRR = BSRR_S(v); __DSB(); } while (0)
	//#define LS020_RESET_PORT_C(v)		do { GPIOD->BSRR = BSRR_C(v); __DSB(); } while (0)
	//#define LS020_RST			(1u << 8)			// D6 signal
	#define LS020_RS_PORT_S(v)		do { GPIOD->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RS_PORT_C(v)		do { GPIOD->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RS			(1u << 13)			// PD13 signal

#elif LCDMODE_SPI_RA
	// ��� ����������� ������� ������ RS
	#define LS020_RS_PORT_S(v)		do { GPIOD->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RS_PORT_C(v)		do { GPIOD->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RS			(1u << 13)			// PD13 signal
#endif

#if WITHENCODER

	// ������ ����������� ��������

	// Encoder inputs: PB7 - PHASE A, PB4 = PHASE B
	#define ENCODER_INPUT_PORT			(GPIOB->IDR)
	#define ENCODER_BITA (1u << 7)		// PB7
	#define ENCODER_BITB (1u << 4)		// PB4
	#define ENCODER_BITS (ENCODER_BITA | ENCODER_BITB)
	//#define ENCODER_SHIFT 4

	#define ENCODER_INITIALIZE() \
		do { \
			arm_hardware_piob_inputs(ENCODER_BITS); \
			arm_hardware_piob_updown(ENCODER_BITS, 0); \
			arm_hardware_piob_onchangeinterrupt(ENCODER_BITS, ENCODER_BITS, ENCODER_BITS, ARM_OVERREALTIME_PRIORITY); \
		} while (0)

#endif

/* ������������� ����� � ARM ������������ */



#if WITHCAT || WITHNMEA
	// CAT control lines
	#define FROMCAT_TARGET_PIN_RTS	sss	GPIOA->IDR // was PINA 
	#define FROMCAT_BIT_RTS			sss	(1u << 11)	/* ������ RTS �� FT232RL	*/

	/* ����������� �� ����� RS-232, ������ PPS �� GPS/GLONASS/GALILEO ������ */
	#define FROMCAT_TARGET_PIN_DTR	sss	GPIOA->IDR // was PINA 
	#define FROMCAT_BIT_DTR			sss	(1u << 12)	/* ������ DTR �� FT232RL	*/

	/* ����������� �� ����� RS-232 */
	#define FROMCAT_DTR_INITIALIZEXXX() \
		do { \
			arm_hardware_pioa_inputs(FROMCAT_BIT_DTR); \
			arm_hardware_pioa_updown(FROMCAT_BIT_DTR, 0); \
		} while (0)

	/* ������� �� �������� �� ����� RS-232 */
	#define FROMCAT_RTS_INITIALIZEXXX() \
		do { \
			arm_hardware_pioa_inputs(FROMCAT_BIT_RTS); \
			arm_hardware_pioa_updown(FROMCAT_BIT_RTS, 0); \
		} while (0)
	/* ������ PPS �� GPS/GLONASS/GALILEO ������ */
	#define NMEA_INITIALIZEXXX() \
		do { \
			arm_hardware_pioa_inputs(FROMCAT_BIT_DTR); \
			arm_hardware_pioa_onchangeinterrupt(FROMCAT_BIT_DTR, FROMCAT_BIT_DTR, FROMCAT_BIT_DTR, ARM_SYSTEM_PRIORITY); \
		} while (0)

	#define FROMCAT_DTR_INITIALIZE() \
		do { \
		} while (0)

	/* ������� �� �������� �� ����� RS-232 */
	#define FROMCAT_RTS_INITIALIZE() \
		do { \
		} while (0)

	/* ������ PPS �� GPS/GLONASS/GALILEO ������ */
	#define NMEA_INITIALIZE() \
		do { \
		} while (0)
#endif

#if  WITHTX

	// txpath outputs

	// ������� "1" - ���������� ����� ������� � �����,
	// ������� ������� ��������� "��������".
	#define TXPATH_TARGET_PORT_S(v)		do { GPIOG->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TXPATH_TARGET_PORT_C(v)		do { GPIOG->BSRR = BSRR_S(v); __DSB(); } while (0)
	#if 1

		// ���������� ������������ - ������� TXPATH_ENABLE (PG13) � TXPATH_ENABLE_CW (PG14) - ������� ��� ���� �� ������.
		#define TXPATH_BIT_ENABLE_SSB		(1u << 13)
		#define TXPATH_BIT_ENABLE_CW		(1u << 14)
		#define TXPATH_BITS_ENABLE	(TXPATH_BIT_ENABLE_SSB | TXPATH_BIT_ENABLE_CW)

		// �������������� ����������� �����
		#define TXGFV_RX		0
		#define TXGFV_TRANS		0			// ������� ����� �������� ����� � ��������
		#define TXGFV_TX_SSB	TXPATH_BIT_ENABLE_SSB
		#define TXGFV_TX_CW		TXPATH_BIT_ENABLE_CW
		#define TXGFV_TX_AM		TXPATH_BIT_ENABLE_CW
		#define TXGFV_TX_NFM	TXPATH_BIT_ENABLE_CW

		// ���������� ������������ - ������� TXPATH_ENABLE (PA11) � TXPATH_ENABLE_CW (PA10) - ������� ��� ���� �� ������.
		// ��������� ���������� ��������� �������
		#define TXPATH_INITIALIZE() \
			do { \
				arm_hardware_piog_outputs(TXPATH_BITS_ENABLE, 0); \
			} while (0)

	#else
		// ���������� ������������ - ������������ ������ ���������� ������
		#define TXPATH_BIT_GATE (1u << 13)	// �������� ������ �� ���������� - ���������� ������������.
		//#define TXPATH_BIT_GATE_RX TXPATH_BIT_GATE	// ������ tx2 - ���������� ������������. ��� ����� �������
		#define TXPATH_BIT_GATE_RX 0	// ������ tx2 - ���������� ������������. ��� ����� �� �������

		// �������������� ����������� �����
		#define TXGFV_RX		TXPATH_BIT_GATE_RX // TXPATH_BIT_GATE ��� �������� � ������ ��������� �� ���� � ��������
		#define TXGFV_TRANS		0			// ������� ����� �������� ����� � ��������
		#define TXGFV_TX_SSB	TXPATH_BIT_GATE
		#define TXGFV_TX_CW		TXPATH_BIT_GATE
		#define TXGFV_TX_AM		TXPATH_BIT_GATE
		#define TXGFV_TX_NFM	TXPATH_BIT_GATE
		// ���������� ������������ - ������ TXPATH_BIT_GATE
		// ��������� ���������� ��������� �������
		#define TXPATH_INITIALIZE() \
			do { \
				/*arm_hardware_piog_outputs2m(TXPATH_BIT_GATE, 0); */\
			} while (0)

	#endif

	// PTT input - PA0
	#define PTT_TARGET_PIN				(GPIOA->IDR)
	#define PTT_BIT_PTT					(1U << 0)		// PA0

	// �� ���� ����� PTT �������� ������� � "1"
	#define HARDWARE_GET_PTT() ((PTT_TARGET_PIN & PTT_BIT_PTT) != 0)
	#define PTT_INITIALIZE() do { \
			arm_hardware_pioa_inputs(PTT_BIT_PTT); \
			arm_hardware_pioa_updown(0, PTT_BIT_PTT); \
		} while (0)

#endif /* WITHTX */


#if WITHELKEY

	// Electronic key inputs
	#define ELKEY_TARGET_PIN			(GPIOA->IDR)
	#define ELKEY_BIT_LEFT				(1u << 9)	// PA9
	#define ELKEY_BIT_RIGHT				(1u << 10)	// PA10

	#define ELKEY_INITIALIZE() do { \
			arm_hardware_pioa_inputs(ELKEY_BIT_LEFT | ELKEY_BIT_RIGHT); \
		} while (0)

#endif

/* ������������� ������� IOUPDATE �� DDS */
/* ��������� ��������� = 1 */
#define SPI_IOUPDATE_INITIALIZE() \
	do { \
	} while (0)
/* ������������� ������� IOUPDATE �� DDS */
/* ��������� ��������� = 0 */
#define SPI_IORESET_INITIALIZE() \
	do { \
	} while (0)

#if 1
	// ����� ����������� ��� ������ ��� �������� �����������
	#define SPI_ALLCS_PORT_S(v)	do { GPIOC->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define SPI_ALLCS_PORT_C(v)	do { GPIOC->BSRR = BSRR_C(v); __DSB(); } while (0)
	

	#define SPI_CSEL0	(1U << 1)		/* PC1 - L3GD20 */
	#define SPI_CSEL1	(1U << 2)		/* PC2 - TFT LCD */ 
	#define SPI_CSEL2	(1U << 11)		/* PC11 - FPGA */
	#define SPI_CSEL3	(1U << 12)		/* PC12 - CODEC1 */
	#define SPI_CSEL4	(1U << 13)		/* PC13 - FRAM */
	#define SPI_CSEL5	(1U << 8)		/* PC8 - spare */
	// ����� ������ ���� ����������� ��� ���� ������������ CS  ����������.
	#define SPI_ALLCS_BITS (SPI_CSEL0 | SPI_CSEL1 | SPI_CSEL2 | SPI_CSEL3 | SPI_CSEL4 | SPI_CSEL5)
	#define SPI_ALLCS_BITSNEG 0		// ������, �������� ��� "1"

	/* ������������� ���� ������ ������������ ��������� */
	#define SPI_ALLCS_INITIALIZE() \
		do { \
			arm_hardware_pioc_outputs2m(SPI_ALLCS_BITS, SPI_ALLCS_BITS); \
		} while (0)
	/* ������������� �������� ����������� ������������ CS */
	#define SPI_ADDRESS_AEN_INITIALIZE() \
		do { \
		} while (0)
	/* ������������� �������� ����������� ������������ CS */
	#define SPI_ADDRESS_NAEN_INITIALIZE() \
		do { \
		} while (0)

#else
	// ���� ������� ���������� �� ���� ������ SPI 

	// ���� ������ ������ ��������� �����������
	#define SPI_ADDRESS_PORT_S(v)	do { GPIOC->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define SPI_ADDRESS_PORT_C(v)	do { GPIOC->BSRR = BSRR_C(v); __DSB(); } while (0)
	//#define SPI_ADDRESS_DDR_S			(GPIOC->PIO_OER)	// was DDRA
	//#define SPI_ADDRESS_DDR_C			(GPIOC->PIO_ODR)	// was DDRA

	#define SPI_A0 ((1u << 10))			// ���� ������ ��� ����������� SPI
	#define SPI_A1 ((1u << 11))
	#define SPI_A2 ((1u << 12))


	#define SPI_NAEN_PORT_S(v)	do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define SPI_NAEN_PORT_C(v)	do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
	//#define SPI_NAEN_DDR_S			(GPIOA->PIO_OER)	// was DDRA
	//#define SPI_NAEN_DDR_C			(GPIOA->PIO_ODR)	// was DDRA

	#define SPI_NAEN_BIT (1u << 15)		// PA15 used
	#define SPI_ALLCS_BITS	0		// ��������� ��� �������� ����, ��� ������ � ������ ������� CS (��� �����������) �� ���������

	/* ������������� �������� ����������� ������������ CS */
	#define SPI_ADDRESS_NAEN_INITIALIZE() \
		do { \
			arm_hardware_pioc_outputs2m(SPI_ADDRESS_BITS, 0); \
			arm_hardware_pioa_outputs2m(SPI_NAEN_BIT, SPI_NAEN_BIT); \
		} while (0)

	/* ������������� �������� ����������� ������������ CS */
	#define SPI_ADDRESS_AEN_INITIALIZE() \
		do { \
		} while (0)
	/* ������������� ���� ������ ������������ ��������� */
	#define SPI_ALLCS_INITIALIZE() \
		do { \
		} while (0)
#endif


// SPI control pins
// SPI1 hardware used.

// MOSI & SCK port
// STM32F303: SPI1_NSS can be placed on PA4 or PA15
#define SPI_TARGET_SCLK_PORT_C(v)	do { GPIOF->BSRR = BSRR_C(v); __DSB(); } while (0)
#define SPI_TARGET_SCLK_PORT_S(v)	do { GPIOF->BSRR = BSRR_S(v); __DSB(); } while (0)
#define SPI_TARGET_MOSI_PORT_C(v)	do { GPIOF->BSRR = BSRR_C(v); __DSB(); } while (0)
#define SPI_TARGET_MOSI_PORT_S(v)	do { GPIOF->BSRR = BSRR_S(v); __DSB(); } while (0)

// SPI5 hardware pins: PF6: SPI5_NSS, PF7: SPI5_SCK, PF8: SPI5_MISO, PF9: SPI5_MOSI (or PF11: SPI5_MOSI)
#define	SPI_SCLK_BIT			(1u << 7)	// PF7 ���, ����� ������� ���� ������������� SPI
#define	SPI_MISO_BIT			(1u << 8)	// PF8 ���, ����� ������� ���� ����
#define	SPI_MOSI_BIT			(1u << 9)	// PF9 ���, ����� ������� ���� �����

#if SPI_BIDIRECTIONAL

	#define SPI_TARGET_MOSI_PIN		(GPIOF->IDR)
	#define SPIIO_INITIALIZE() do { \
			arm_hardware_piof_outputs(SPI_MOSI_BIT | SPI_SCLK_BIT, SPI_MOSI_BIT | SPI_SCLK_BIT); \
		} while (0)
	#define SPIIO_MOSI_TO_INPUT() do { \
		arm_hardware_piof_inputs(SPI_MOSI_BIT);	/* ����������� ���� �� ������ � ������� */ \
		} while (0)
	#define SPIIO_MOSI_TO_OUTPUT() do { \
		arm_hardware_piof_outputs(SPI_MOSI_BIT, SPI_MOSI_BIT);	/* ������� ������ ����� */ \
		} while (0)

#else /* SPI_BIDIRECTIONAL */

	#define SPI_TARGET_MISO_PIN		(GPIOF->IDR)
	#define	SPI_MISO_BIT			(1u << 8)	// PF8 ���, ����� ������� ���� ���� � SPI.

	#define SPIIO_INITIALIZE() do { \
			arm_hardware_piof_outputs(SPI_MOSI_BIT | SPI_SCLK_BIT, SPI_MOSI_BIT | SPI_SCLK_BIT); \
			arm_hardware_piof_inputs(SPI_MISO_BIT); \
		} while (0)

#endif /* SPI_BIDIRECTIONAL */



//#define SIDETONE_TARGET_BIT		(1u << 8)	// output TIM4_CH3 (PB8, base mapping)

#if 1 // WITHTWISW
//#if CPUSTYLE_ATMEGA
	//#define TARGET_TWI_PORT PORTC
	//#define TARGET_TWI_DDR DDRC
	//#define TARGET_TWI_TWCK	(1U << PC0)
	//#define TARGET_TWI_TWD	(1U << PC1)
//#elif CPUSTYLE_ARM
	#define TARGET_TWI_TWCK_PORT_C(v) do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_TWI_TWCK_PORT_S(v) do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_TWI_TWCK_PIN		(GPIOA->IDR)
	#define TARGET_TWI_TWCK			(1u << 8)	// PA8

	#define TARGET_TWI_TWD_PORT_C(v) do { GPIOC->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_TWI_TWD_PORT_S(v) do { GPIOC->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_TWI_TWD_PIN		(GPIOC->IDR)
	#define TARGET_TWI_TWD			(1u << 9)	// PC9
//#endif

	// ������������� ����� ������ �����-������ ��� ����������� ���������� I2C
	#define	TWISOFT_INITIALIZE() do { \
			enum { WORKMASK = TARGET_TWI_TWCK | TARGET_TWI_TWD };		/* ������� �����, ���������� ����� ������� �������� */ \
			arm_hardware_pioa_opendrain(TARGET_TWI_TWCK, TARGET_TWI_TWCK); \
			arm_hardware_pioc_opendrain(TARGET_TWI_TWD, TARGET_TWI_TWD); \
		} while (0) 

	// ������������� ����� ������ �����-������ ��� ���������� ���������� I2C
	// ������������� ������� � ������������� ����������
	#if CPUSTYLE_STM32F1XX

		#define	TWIHARD_INITIALIZE() do { \
				enum { WORKMASK = TARGET_TWI_TWCK | TARGET_TWI_TWD };		/* ������� �����, ���������� ����� ������� �������� */ \
				arm_hardware_piob_periphopendrain_altfn2(TARGET_TWI_TWCK | TARGET_TWI_TWD, 255);	/* AF=4 */ \
			} while (0) 

	#elif CPUSTYLE_STM32F4XX || CPUSTYLE_STM32F7XX

		#define	TWIHARD_INITIALIZE() do { \
				enum { WORKMASK = TARGET_TWI_TWCK | TARGET_TWI_TWD };		/* ������� �����, ���������� ����� ������� �������� */ \
				arm_hardware_piob_periphopendrain_altfn2(TARGET_TWI_TWCK | TARGET_TWI_TWD, AF_I2C1);	/* AF=4 */ \
			} while (0) 

	#endif

#endif

#if WITHCPUADCHW
	#define HARDWARE_ADC_INITIALIZE(ainmask) do { \
			arm_hardware_pioa_analoginput(((ainmask) >> 0) & 0xff);	/* ADC12_IN0..ADC12_IN7 */ \
			arm_hardware_piob_analoginput(((ainmask) >> 8) & 0x03);	/* ADC12_IN8..ADC12_IN0 */ \
			arm_hardware_pioc_analoginput(((ainmask) >> 10) & 0x3f);	/* ADC12_IN10..ADC12_IN15 */ \
		} while (0)
#endif /* WITHCPUADCHW */

#if WITHSAI1HW
	#define SAI1HW_INITIALIZE()	do { \
		/*arm_hardware_pioe_altfn20(1U << 2, AF_SAI); */	/* PE2 - SAI1_MCK_A - 12.288 MHz	*/ \
		arm_hardware_pioe_altfn2(1U << 3, AF_SAI);			/* PE3 - SAI1_SD_B	(i2s data from codec)	*/ \
		arm_hardware_pioe_altfn2(1U << 4, AF_SAI);			/* PE4 - SAI1_FS_A	- 48 kHz	*/ \
		arm_hardware_pioe_altfn20(1U << 5, AF_SAI);			/* PE5 - SAI1_SCK_A	*/ \
		arm_hardware_pioe_altfn2(1U << 6, AF_SAI);			/* PE6 - SAI1_SD_A	(i2s data to codec)	*/ \
	} while (0)
#endif /* WITHSAI1HW */


// ���������� ������ ��� ����������.
enum 
{ 
#if WITHREFSENSOR
	VREFIX = 17,		// Reference voltage
#endif /* WITHREFSENSOR */
//#if WITHTEMPSENSOR
//	TEMPIX = 16,		// ADC1->CR2 |= ADC_CR2_TSVREFE;	// ��� ������
//#endif
//#if WITHVOLTLEVEL 
//	VOLTSOURCE = 0,		// ������� ����� �������� ����������, ��� ���
//#endif /* WITHVOLTLEVEL */

//#if WITHSWRMTR
//	PWRI = 0,
//	FWD = 0, REF = 0,	// SWR-meter
//#endif
	KI0 = 5, KI1 = 13, KI2 = 15	// PA5, PC3, PC5 ����������
};

#define KI_COUNT 3	// ���������� ������������ ��� ���������� ������ ���
#define KI_LIST	KI2, KI1, KI0,	// �������������� ��� ������� �������������

#if WITHUSBHW
	/**USB_OTG_HS GPIO Configuration    
	PB12  OTG_HS_ID
	PB13  USB_OTG_HS_VBUS
	PB14  USB_OTG_HS_DM
	PB15  USB_OTG_HS_DP 
	*/
	#define	USBD_HS_FS_INITIALIZE() do { \
		arm_hardware_piob_altfn50((1U << 14) | (1U << 15), AF_OTGHS_FS);			/* PB14, PB15 - USB_OTG_HS	*/ \
		arm_hardware_piob_inputs(1U << 13);		/* PB13 - USB_OTG_HS_VBUS */ \
		arm_hardware_piob_updownoff((1U << 13) | (1U << 14) | (1U << 15)); \
		} while (0)

	#define	USBD_FS_INITIALIZE() do { \
		} while (0)
	#define	USBD_HS_ULPI_INITIALIZE() do { \
		} while (0)

#endif /* WITHUSBHW */


	#define HARDWARE_SPI_CONNECT() do { \
			arm_hardware_piob_altfn50(SPI_MOSI_BIT | SPI_SCLK_BIT | SPI_MISO_BIT, AF_SPI1); /* � ���� ����������� � ����� � ������ ������������� �� ALT FN */ \
		} while (0)
	#define HARDWARE_SPI_DISCONNECT() do { \
			arm_hardware_piob_outputs(SPI_SCLK_BIT | SPI_MOSI_BIT, SPI_SCLK_BIT | SPI_MOSI_BIT); /* connect back to GPIO */ \
			arm_hardware_piob_inputs(SPI_MISO_BIT); /* connect back to GPIO */ \
		} while (0)
	#define HARDWARE_SPI_CONNECT_MOSI() do { \
			arm_hardware_piob_altfn50(SPI_MOSI_BIT, AF_SPI1);	/* PIO disable for MOSI bit (SD CARD read support) */ \
		} while (0)
	#define HARDWARE_SPI_DISCONNECT_MOSI() do { \
			arm_hardware_piob_outputs(SPI_MOSI_BIT, SPI_MOSI_BIT);	/* PIO enable for MOSI bit (SD CARD read support)  */ \
		} while (0)
	#define HARDWARE_SIDETONE_INITIALIZE() do { \
		arm_hardware_piob_altfn2(SIDETONE_TARGET_BIT, AF_TIM4);	/* output TIM4_CH3 (PB8, base mapping) */ \
		} while (0)

	#if KEYBOARD_USE_ADC
		#define HARDWARE_KBD_INITIALIZE() do { \
			} while (0)
	#else
		#define HARDWARE_KBD_INITIALIZE() do { \
			arm_hardware_pioa_inputs(KBD_MASK); \
			} while (0)
	#endif

	#if LCDMODE_LTDC
		enum
		{
			GPIO_AF_LTDC = 14,  /* LCD-TFT Alternate Function mapping */
			GPIO_AF_LTDC9 = 9  /* LCD-TFT Alternate Function mapping */
		};
		/* demode values: 0: static signal, 1: DE controlled */
		#define HARDWARE_LTDC_INITIALIZE(demode) do { \
				arm_hardware_pioa_altfn20((1U << 3) | (1U << 4) | (1U << 6) | (1U << 11) | (1U << 12), GPIO_AF_LTDC); \
				arm_hardware_piob_altfn20((1U << 8) | (1U << 9) | (1U << 10) | (1U << 11), GPIO_AF_LTDC); \
				arm_hardware_pioc_altfn20((1U << 6) | (1U << 7) | (1U << 12), GPIO_AF_LTDC); \
				arm_hardware_piod_altfn20((1U << 3) | (1U << 6), GPIO_AF_LTDC); \
				arm_hardware_piof_altfn20((1U << 10), GPIO_AF_LTDC); \
				arm_hardware_piog_altfn20((1U << 6) | (1U << 7) | (1U << 11), GPIO_AF_LTDC); \
				arm_hardware_piob_altfn20((1U << 0) | (1U << 1), GPIO_AF_LTDC9); \
				arm_hardware_piog_altfn20((1U << 10) | (1U << 12), GPIO_AF_LTDC9); \
			} while (0)
		/* ���������� ���������� ������� DISP ������ */
		/* demode values: 0: static signal, 1: DE controlled */
		#define HARDWARE_LTDC_SET_DISP(demode, state) do { \
			if (demode != 0) break; \
			} while (0)

	#endif /* LCDMODE_LTDC */

#endif /* ARM_STM32FXXX_TQFP144_CPUSTYLE_V8C_H_INCLUDED */
