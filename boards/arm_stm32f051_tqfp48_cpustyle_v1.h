/* $Id$ */
/* board-specific CPU attached signals */
//
// ������ HF Dream Receiver (�� ������� �����)
// ����� ���� ����������� mgs2001@mail.ru
// UA1ARN
//

// ����� � ����������� STM32F051C6T6 (TQFP-48)

#ifndef ARM_STM32F051_TQFP48_CPUSTYLE_V1_H_INCLUDED
#define ARM_STM32F051_TQFP48_CPUSTYLE_V1_H_INCLUDED 1

//#define HARDWARE_ARM_USEUSART0 1		// US0: PA5/PA6 pins
//#define HARDWARE_ARM_USEUSART1 1		// US1: PA21/PA22 pins
#define HARDWARE_IGNORENONVRAM	1		// ������� �� ������ ��� ��� �������� NVRAM

#define WITHSPI16BIT	1		/* �������� ������������� 16-�� ������ ���� ��� ������ �� SPI */
#define WITHSPIHW 		1	/* ������������� ����������� ����������� SPI */
//#define WITHSPIHWDMA 	1	/* ������������� DMA ��� ������ �� SPI */
#define WITHSPISW 	1	/* ������������� ������������ ���������� SPI. ������ ������� ��� ������ - ��������� ����� ���������� ��-�� ��������� � I2C */

//#define	WITHCPUDACHW	1
//#define WITHTWIHW 	1	/* ������������� ����������� ����������� TWI (I2C) */
//#define WITHTWISW 	1	/* ������������� ������������ ����������� TWI (I2C) */
#define WITHCPUADCHW 	1	/* ������������� ADC */

//#define WITHUART1HW	1	/* ������������ ������������ ���������� ����������������� ����� #1 */
//#define WITHUART2HW	1	/* ������������ ������������ ���������� ����������������� ����� #2 */

//#define WITHCAT_CDC		1	/* ������������ ����������� ���������������� ���� �� USB ���������� */
#define WITHCAT_USART1		1
#define WITHDEBUG_USART1	1
#define WITHMODEM_USART1	1

//#define BSRR_S(v) ((v) * GPIO_BSRR_BS_0)	/* �������������� �������� ��� ��������� ���� � �������� */
//#define BSRR_C(v) ((v) * GPIO_BSRR_BR_0)	/* �������������� �������� ��� ������ ���� � �������� */

#define LCD_DATAS_BIT_LOW	0		// PB0..PB7
#define LCD_DATAS_BITS		0xFF	// PB0..PB7

#define DISPLAY_BUS_DATA_GET() ((LCD_DATA_INPUT & LCD_DATAS_BITS) >> LCD_DATAS_BIT_LOW) /* �������� ������ � ���� LCD */
#define DISPLAY_BUS_DATA_SET(v) do { /* ������ ������ (�� ���������) */ \
		const portholder_t t = (portholder_t) (v) << LCD_DATAS_BIT_LOW; \
		GPIOB->BSRR = BSRR_S(t & LCD_DATAS_BITS) | BSRR_C(~ t & LCD_DATAS_BITS); \
		__DSB(); \
	} while (0)
#define DISPLAY_BUS_NWR_S(v)	do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
#define DISPLAY_BUS_NWR_C(v)	do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
#define DISPLAY_BUS_NWR			(1UL << 11)	// PB11
#define DISPLAY_BUS_NRD_S(v)	do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
#define DISPLAY_BUS_NRD_C(v)	do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
#define DISPLAY_BUS_NRD			(1UL << 10)	// PB10

#define DISPLAY_BUS_INITIALIZE() do { \
		arm_hardware_piob_outputs2m( \
			LCD_DATAS_BIT_LOW | DISPLAY_BUS_NWR | DISPLAY_BUS_NRD, \
			DISPLAY_BUS_NWR | DISPLAY_BUS_NRD); \
	} while (0)
#define LS020_RS_INITIALIZE() \
	do { \
		arm_hardware_piob_outputs2m(LS020_RS, LS020_RS); \
	} while (0)

#define LS020_RESET_INITIALIZE() \
	do { \
		arm_hardware_piob_outputs2m(LS020_RST, LS020_RST); \
	} while (0)


#if LCDMODE_SPI_RN
	// ��� ����������� ������� ������ RESET
	#define LS020_RESET_PORT_S(v)		do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RESET_PORT_C(v)		do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RST			(1u << 8)			// PB8

#elif LCDMODE_SPI_NA
	// ��� ����������� ������� ������ RS
	#define LS020_RS_PORT_S(v)		do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RS_PORT_C(v)		do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RS			(1u << 12)			// PB12

#elif LCDMODE_SPI_RA
	// ��� ����������� ������� RESET � RS
	#define LS020_RESET_PORT_S(v)		do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RESET_PORT_C(v)		do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RST			(1u << 8)			// PB8

	#define LS020_RS_PORT_S(v)		do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LS020_RS_PORT_C(v)		do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LS020_RS			(1u << 12)			// PB12

#elif LCDMODE_HD44780 && (LCDMODE_SPI == 0)

	// ������ ����������� ��� ���������� WH2002 ��� ������������ HD44780.
	//#define LCD_DATA_PORT			GPIOA->ODR	
	#define LCD_DATA_PORT_S(v)		do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LCD_DATA_PORT_C(v)		do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define LCD_DATA_INPUT			(GPIOA->IDR)

	#define LCD_STROBE_PORT_S(v)		do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LCD_STROBE_PORT_C(v)		do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)

	// RS & WE bits
	#define LCD_RS_WE_PORT_S(v)		do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define LCD_RS_WE_PORT_C(v)		do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)

	#define LCD_STROBE_BIT			(1u << 13)

	#define WRITEE_BIT				(1u << 11)
	//#define WRITEE_BIT_ZERO				GPIO_ODR_ODR11
	
	#define ADDRES_BIT				(1u << 10)

	#define LCD_DATAS_BITS			((1u << 9) | (1u << 8) | (1u << 7) | (1u << 6))
	#define LCD_DATAS_BIT_LOW		6		// ����� ��� ������ ������� � ����� ��������� � �����


	/* ������������� ����������� ������� ���������� ��� ���������� HD44780 - ������ ����� ������� */
	#define LCD_CONTROL_INITIALIZE() \
		do { \
			arm_hardware_pioa_outputs2m(LCD_STROBE_BIT | WRITEE_BIT | ADDRES_BIT, 0); \
		} while (0)
	/* ������������� ����������� ������� ���������� ��� ���������� HD44780 - WE=0 */
	#define LCD_CONTROL_INITIALIZE_WEEZERO() \
		do { \
			arm_hardware_pioa_outputs2m(LCD_STROBE_BIT | WRITEE_BIT_ZERO | ADDRES_BIT, 0); \
		} while (0)
	/* ������������� ����������� ������� ���������� ��� ���������� HD44780 - WE ����������� - ������ � ���������� ������� */
	#define LCD_CONTROL_INITIALIZE_WEENONE() \
		do { \
			arm_hardware_pioa_outputs2m(LCD_STROBE_BIT | ADDRES_BIT, 0); \
		} while (0)

	#define LCD_DATA_INITIALIZE_READ() \
		do { \
			arm_hardware_pioa_inputs(LCD_DATAS_BITS);	/* ����������� ���� �� ������ � ������� */ \
		} while (0)

	#define LCD_DATA_INITIALIZE_WRITE(v) \
		do { \
			arm_hardware_pioa_outputs2m(LCD_DATAS_BITS, (v) << LCD_DATAS_BIT_LOW);	/* ������� ������ ����� */ \
		} while (0)

#endif

#if LCDMODE_UC1608
	// ��� ����������� ������� RESET � RS, � ��� �� ������������� ��������� (����������� �� ��� �� ������ ��� � EE ��44780
	#define UC1608_CSP_PORT_S(v)		do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define UC1608_CSP_PORT_C(v)		do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define UC1608_CSP			(1u << 13)			// E signal in HD44780 socket
	#define SPI_CSEL255			255				// �� ����� ���������� �������� ������������� ��������

	#define UC1608_CSP_INITIALIZE() do { \
			arm_hardware_pioa_outputs(UC1608_CSP, 0); \
		} while (0)
#endif



#if WITHENCODER

	// ������ ����������� ��������

	// Encoder inputs: PB15 - PHASE A, PB14 = PHASE B
	#define ENCODER_INPUT_PORT			(GPIOA->IDR)
	#define ENCODER_BITS ((1u << 15) | (1u << 14))
	#define ENCODER_SHIFT 14

	#define ENCODER_INITIALIZE() \
		do { \
			arm_hardware_pioa_inputs(ENCODER_BITS); \
			arm_hardware_pioa_updown(ENCODER_BITS, 0); \
			arm_hardware_pioa_onchangeinterrupt(ENCODER_BITS, ENCODER_BITS, ENCODER_BITS, ARM_OVERREALTIME_PRIORITY); \
		} while (0)

#endif

/* ������������� ����� � ARM ������������ */



#if WITHCAT || WITHNMEA
	// CAT control lines
	#define FROMCAT_TARGET_PIN_RTS		(GPIOA->IDR) // was PINA 
	#define FROMCAT_BIT_RTS				(0u << 11)	/* ������ RTS �� FT232RL	*/

	/* ����������� �� ����� RS-232, ������ PPS �� GPS/GLONASS/GALILEO ������ */
	#define FROMCAT_TARGET_PIN_DTR		(GPIOA->IDR) // was PINA 
	#define FROMCAT_BIT_DTR				(0u << 12)	/* ������ DTR �� FT232RL	*/

	/* ����������� �� ����� RS-232 */
	#define FROMCAT_DTR_INITIALIZE() \
		do { \
			arm_hardware_pioa_inputs(FROMCAT_BIT_DTR); \
			arm_hardware_pioa_updown(FROMCAT_BIT_DTR, 0); \
		} while (0)

	/* ������� �� �������� �� ����� RS-232 */
	#define FROMCAT_RTS_INITIALIZE() \
		do { \
			arm_hardware_pioa_inputs(FROMCAT_BIT_RTS); \
			arm_hardware_pioa_updown(FROMCAT_BIT_RTS, 0); \
		} while (0)

	/* ������ PPS �� GPS/GLONASS/GALILEO ������ */
	#define NMEA_INITIALIZE() \
		do { \
			arm_hardware_pioa_inputs(FROMCAT_BIT_DTR); \
			arm_hardware_pioa_onchangeinterrupt(FROMCAT_BIT_DTR, FROMCAT_BIT_DTR, FROMCAT_BIT_DTR, ARM_SYSTEM_PRIORITY); \
		} while (0)

#endif

#if WITHTX

	// txpath outputs

	//#define TXPATH_TARGET_DDR_S			GPIOF->PIO_OER	// was DDRA
	//#define TXPATH_TARGET_DDR_C		GPIOD->PIO_ODR	// was DDRA
	#define TXPATH_TARGET_PORT_S(v)		do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TXPATH_TARGET_PORT_C(v)		do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
	#if 1

		// ���������� ������������ - ������� TXPATH_ENABLE (PC12) � TXPATH_ENABLE_CW (PC13) - ������� ��� ���� �� ������.
		#define TXPATH_BIT_ENABLE_SSB		(1u << 12)
		#define TXPATH_BIT_ENABLE_CW		(1u << 13)
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
				arm_hardware_pioa_opendrain(TXPATH_BITS_ENABLE, TXPATH_BITS_ENABLE); \
			} while (0)

	#else
		// ���������� ������������ - ������������ ������ ���������� ������
		#define TXPATH_BIT_GATE (1u << 12)	// �������� ������ �� ���������� - ���������� ������������.
		//#define TXPATH_BIT_GATE_RX TXPATH_BIT_GATE	// ������ tx2 - ���������� ������������. ��� ����� �������
		#define TXPATH_BIT_GATE_RX 0	// ������ tx2 - ���������� ������������. ��� ����� �� �������

		// �������������� ����������� �����
		#define TXGFV_RX		TXPATH_BIT_GATE_RX
		#define TXGFV_TRANS		0			// ������� ����� �������� ����� � ��������
		#define TXGFV_TX_SSB	TXPATH_BIT_GATE
		#define TXGFV_TX_CW		TXPATH_BIT_GATE
		#define TXGFV_TX_AM		TXPATH_BIT_GATE
		#define TXGFV_TX_NFM	TXPATH_BIT_GATE
		// ���������� ������������ - ������ TXPATH_BIT_GATE
		// ��������� ���������� ��������� �������
		#define TXPATH_INITIALIZE() \
			do { \
				arm_hardware_pioa_outputs2m(TXPATH_BIT_GATE, 0); \
			} while (0)

	#endif

	// PTT input

	#define PTT_TARGET_PIN				(GPIOA->IDR) // was PINA 
	#define PTT_BIT_PTT					(1u << 9)

	#define HARDWARE_GET_PTT() ((PTT_TARGET_PIN & PTT_BIT_PTT) == 0)
	#define PTT_INITIALIZE() \
		do { \
			arm_hardware_pioa_inputs(PTT_BIT_PTT); \
			arm_hardware_pioa_updown(PTT_BIT_PTT, 0); \
		} while (0)

#endif /* WITHTX */


#if WITHELKEY
	// Electronic key inputs
	#define ELKEY_TARGET_PIN			(GPIOA->IDR)
	#define ELKEY_BIT_LEFT				(1u << 10)
	#define ELKEY_BIT_RIGHT				(1u << 11)

	#define ELKEY_INITIALIZE() \
		do { \
			arm_hardware_pioa_inputs(ELKEY_BIT_LEFT | ELKEY_BIT_RIGHT); \
			arm_hardware_pioa_updown(ELKEY_BIT_LEFT | ELKEY_BIT_RIGHT, 0); \
		} while (0)

#endif

// IOUPDATE = PA15
//#define SPI_IOUPDATE_PORT_C(v)	do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
//#define SPI_IOUPDATE_PORT_S(v)	do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
//#define SPI_IOUPDATE_BIT		(1U << 15)	// * PA15

#if 1
	// ����� ����������� ��� ������ ��� �������� �����������
	//#define SPI_ADDRESS_PORT_S(v)	do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	//#define SPI_ADDRESS_PORT_C(v)	do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)

	#define SPI_CSEL4	(1u << 9)	/* PB9 - LCD */ 
	#define SPI_CSEL5	(1u << 13)	/* PB13 - SD CARD */ 
	#define SPI_CSEL6	(1u << 14)	/* PB14 */ 
	#define SPI_CSEL7	(1u << 15)	/* PB15 */ 
	// ����� ������ ���� ����������� ��� ���� ������������ CS  ����������.
	#define SPI_ALLCS_BITS (SPI_CSEL4 | SPI_CSEL5 | SPI_CSEL6 | SPI_CSEL7)

	// ����� ����������� ��� ������ ��� �������� �����������
	#define SPI_ALLCS_PORT_S(v)	do { GPIOB->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define SPI_ALLCS_PORT_C(v)	do { GPIOB->BSRR = BSRR_C(v); __DSB(); } while (0)

#else
	// ���� ������� ���������� �� ���� ������ SPI 

	// ���� ������ ������ ��������� �����������
	#define SPI_ADDRESS_PORT_S(v)	do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define SPI_ADDRESS_PORT_C(v)	do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)

	#define SPI_A0 ((1u << 10))			// ���� ������ ��� ����������� SPI
	#define SPI_A1 ((1u << 11))
	#define SPI_A2 ((1u << 12))


	#define SPI_NAEN_PORT_S(v)	do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define SPI_NAEN_PORT_C(v)	do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)

	#define SPI_NAEN_BIT (1u << 15)		// PA15 used
	#define SPI_ALLCS_BITS	0		// ��������� ��� �������� ����, ��� ������ � ������ ������� CS (��� �����������) �� ���������
#endif

/* ������������� ���� ������ ������������ ��������� */
#define SPI_ALLCS_INITIALIZE() \
	do { \
		arm_hardware_piob_outputs2m(SPI_ALLCS_BITS, SPI_ALLCS_BITS); \
	} while (0)
/* ������������� �������� ����������� ������������ CS */
#define SPI_ADDRESS_AEN_INITIALIZE() \
	do { \
	} while (0)
/* ������������� �������� ����������� ������������ CS */
#define SPI_ADDRESS_NAEN_INITIALIZE() \
	do { \
	} while (0)
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

// SPI control pins
// SPI1 hardware used.

// MOSI & SCK port
// STM32F303: SPI1_NSS can be placed on PA4 or PA15
#define SPI_TARGET_SCLK_PORT_C(v)	do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
#define SPI_TARGET_SCLK_PORT_S(v)	do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
#define SPI_TARGET_MOSI_PORT_C(v)	do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
#define SPI_TARGET_MOSI_PORT_S(v)	do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)

#define	SPI_SCLK_BIT			(1u << 5)	// PA5 ���, ����� ������� ���� ������������� SPI
#define	SPI_MOSI_BIT			(1u << 7)	// PA7 ���, ����� ������� ���� ����� (��� ���� � ������ ���������������� SPI).

#define SPI_TARGET_MISO_PIN		(GPIOA->IDR)		// was PINA 
#define	SPI_MISO_BIT			(1u << 6)	// PB4 ���, ����� ������� ���� ���� � SPI.

#define SPIIO_INITIALIZE() do { \
		arm_hardware_pioa_outputs(SPI_MOSI_BIT | SPI_SCLK_BIT, SPI_MOSI_BIT | SPI_SCLK_BIT); \
		arm_hardware_pioa_inputs(SPI_MISO_BIT); \
	} while (0)


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
	#define TARGET_TWI_TWD_PORT_C(v) do { GPIOA->BSRR = BSRR_C(v); __DSB(); } while (0)
	#define TARGET_TWI_TWD_PORT_S(v) do { GPIOA->BSRR = BSRR_S(v); __DSB(); } while (0)
	#define TARGET_TWI_PIN		(GPIOA->IDR)
	#define TARGET_TWI_TWCK		(1u << 6)
	#define TARGET_TWI_TWD		(1u << 7)
//#endif
	// ������������� ����� ������ �����-������ ��� ����������� ���������� I2C
	#define	TWISOFT_INITIALIZE() do { \
			enum { WORKMASK = TARGET_TWI_TWCK | TARGET_TWI_TWD };		/* ������� �����, ���������� ����� ������� �������� */ \
			arm_hardware_pioa_opendrain(WORKMASK, WORKMASK); \
		} while (0) 
	// ������������� ����� ������ �����-������ ��� ���������� ���������� I2C
	// ������������� ������� � ������������� ����������
	#if CPUSTYLE_STM32F1XX

		#define	TWIHARD_INITIALIZE() do { \
				enum { WORKMASK = TARGET_TWI_TWCK | TARGET_TWI_TWD };		/* ������� �����, ���������� ����� ������� �������� */ \
				arm_hardware_pioa_periphopendrain_altfn2(TARGET_TWI_TWCK | TARGET_TWI_TWD, 255);	/* AF=4 */ \
			} while (0) 

	#elif CPUSTYLE_STM32F30X

		#define	TWIHARD_INITIALIZE() do { \
				enum { WORKMASK = TARGET_TWI_TWCK | TARGET_TWI_TWD };		/* ������� �����, ���������� ����� ������� �������� */ \
				arm_hardware_pioa_periphopendrain_altfn2(TARGET_TWI_TWCK | TARGET_TWI_TWD, AF_I2C1);	/* AF=4 */ \
			} while (0) 

	#elif CPUSTYLE_STM32F4XX

		#define	TWIHARD_INITIALIZE() do { \
				enum { WORKMASK = TARGET_TWI_TWCK | TARGET_TWI_TWD };		/* ������� �����, ���������� ����� ������� �������� */ \
				arm_hardware_pioa_periphopendrain_altfn2(TARGET_TWI_TWCK | TARGET_TWI_TWD, AF_I2C1);	/* AF=4 */ \
			} while (0) 

	#endif
#endif

#define HARDWARE_SPI_CONNECT() do { \
		arm_hardware_pioa_altfn50(SPI_MOSI_BIT | SPI_SCLK_BIT | SPI_MISO_BIT, 0 /* AF_0 */); /* � ���� ����������� � ����� � ������ ������������� �� ALT FN */ \
	} while (0)
#define HARDWARE_SPI_DISCONNECT() do { \
		arm_hardware_pioa_outputs(SPI_SCLK_BIT | SPI_MOSI_BIT, SPI_SCLK_BIT | SPI_MOSI_BIT); /* connect back to GPIO */ \
		arm_hardware_pioa_inputs(SPI_MISO_BIT); /* connect back to GPIO */ \
	} while (0)

#define HARDWARE_SPI_CONNECT_MOSI() do { \
		arm_hardware_pioa_altfn50(SPI_MOSI_BIT, 0 /* AF_0 */);	/* PIO disable for MOSI bit (SD CARD read support) */ \
	} while (0)
#define HARDWARE_SPI_DISCONNECT_MOSI() do { \
		arm_hardware_pioa_outputs(SPI_MOSI_BIT, SPI_MOSI_BIT);	/* PIO enable for MOSI bit (SD CARD read support)  */ \
	} while (0)

#define HARDWARE_USART1_INITIALIZE() do { \
		arm_hardware_pioa_altfn2((1U << 9) | (1U << 10), 1 /* AF_1 */); /* PA9: TX DATA line (2 MHz), PA10: RX data line */ \
	} while (0)

	#if KEYBOARD_USE_ADC
		#define HARDWARE_KBD_INITIALIZE() do { \
			} while (0)
	#else
		#define KBD_MASK ((1UL << 12) | (1UL << 11) | (1UL << 10) | (1UL << 9))

		#define HARDWARE_KBD_INITIALIZE() do { \
				arm_hardware_pioa_inputs(KBD_MASK); \
			} while (0)
	#endif

#if WITHCPUADCHW
	#define HARDWARE_ADC_INITIALIZE(ainmask) do { \
			arm_hardware_pioa_analoginput(((ainmask) >> 0) & 0xff);	/* ADC12_IN0..ADC12_IN7 */ \
			arm_hardware_piob_analoginput(((ainmask) >> 8) & 0x03);	/* ADC12_IN8..ADC12_IN0 */ \
			arm_hardware_pioc_analoginput(((ainmask) >> 10) & 0x3f);	/* ADC12_IN10..ADC12_IN15 */ \
		} while (0)
#endif /* WITHCPUADCHW */

#endif /* ARM_STM32F051_TQFP48_CPUSTYLE_V1_H_INCLUDED */
