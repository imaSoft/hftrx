/* $Id$ */
/* SW2012	*/
/* board-specific CPU attached signals */
//
// ������ HF Dream Receiver (�� ������� �����)
// ����� ���� ����������� mgs2001@mail.ru
// UA1ARN
//

// ATMEGA644
// �������� ��� ���������� SW2012SF - � ������� �������� SF-TC220H-9223A-N � ��������������� ����������� �������� ���������.

#ifndef ATMEGA_CTLSTYLE_V9SF_US2IT_H_INCLUDED
#define ATMEGA_CTLSTYLE_V9SF_US2IT_H_INCLUDED 1
	
	#define CTLSTYLE_SW2011ALL	1

	#if F_CPU != 8000000
		#error Set F_CPU right value in project file
	#endif
	/* ������ ������������ - ����� ������������������ */
	
	#define FQMODEL_45_IF6000_UHF144	1	// SW2011
	//#define FQMODEL_TRX8M		1	// ������ ������ �� 8 ���, 6 ���, 5.5 ��� � ������
	#define	MODEL_DIRECT	1	/* ������������ ������ ������, � �� ��������� */
	#define CTLREGSTYLE_SW2013SF_V3	1	// 7-bit tuner for US2IT ����������� �������� c ������������ ��������� (��� ���������� �����������)

	//#define WITHCAT_CDC		1	/* ������������ ����������� ���������������� ���� �� USB ���������� */
	#define WITHCAT_USART1		1
	#define WITHDEBUG_USART1	1
	#define WITHMODEM_USART1	1
	#define WITHNMEA_USART1		1

	#define DSTYLE_UR3LMZMOD	1
	//#define	FONTSTYLE_ITALIC	1	//
	// --- �������� ��������, ������������� ��� ������ ����

	// +++ �������� ��������, ������������� ��� ���� �� ATMega
	// --- �������� ��������, ������������� ��� ���� �� ATMega

	// +++ ������ �������� ������������ ������ �� ����������
	//#define KEYB_RA1AGG	1	/* ������������ ������ ��� �������, ���������� RA1AGG - ��� ������ �� ������ ������� ������ ���� �� ������ � ���� ���. */
	//#define KEYB_UA1CEI	1	/* ������������ ������ ��� UA1CEI */
	#define KEYBSTYLE_SW2013SF_US2IT	1
	// --- ������ �������� ������������ ������ �� ����������

	#define SHORTSET7 1
	#define WITHPOWERLPHP		1	// ������� ������������ � ����������� HP / LP
	#define WITHPOWERTRIMMIN	0	// ������ ������ �����������
	#define WITHPOWERTRIMMAX	1	// ������� ������ �����������
	#define	WITHMUTEALL			1	// ���������� ��������� �� ���� �������
	//#define WITHONEATTONEAMP	1	/* ������ ���� ��������� ����������� � ��� - "�� �����". */
	#define WITHATT1PRE1		1	// ���������� ��� � ��������� ������������
	#define WITHLCDBACKLIGHT	1	// ������� ���������� ���������� ������� 
	#define WITHLCDBACKLIGHTMIN	1	// ������ ������ ����������� (������������ �� �������)
	#define WITHLCDBACKLIGHTMAX	4	// ������� ������ ����������� (������������ �� �������)

	//#define	BOARD_AGCCODE_0		0x00
	//#define	BOARD_AGCCODE_1		0x01
	#define WITHAGCMODENONE		1	/* �������� ��� �� ��������� */
	#define BOARD_AGCCODE_OFF	0

	/* ���� ������ ����������� ���������� ������� ��� ��� �������� */
	#define BOARD_DETECTOR_MUTE 0x00
	#define BOARD_DETECTOR_SSB 0x00
	#define BOARD_DETECTOR_AM 0x00
	#define BOARD_DETECTOR_FM 0x00
	#define BOARD_DETECTOR_WFM 0x00
	#define BOARD_DETECTOR_TUNE 0x00	/* ������������ ����� ��� ������ TUNE (CWZ �� ��������) */

	/* ���� �������� ������ ��, ���������� �� ����������� */
	#define	BOARD_FILTERCODE_0	0x00
	#define	BOARD_FILTERCODE_1	0x01

	#define	WITHAUTOTUNER	1	/* ���� ������� ���������� */


	// +++ ���� �� ���� ����� ���������� ��� �������, ��� �������� ������������� ��������
	//#define LCDMODE_HARD_SPI	1		/* LCD over SPI line */
	//#define LCDMODE_WH2002	1	/* ��� ������������ ���������� 20*2, �������� ������ � LCDMODE_HARD_SPI */
	//#define LCDMODE_WH1602	1	/* ��� ������������ ���������� 16*2 */
	//#define LCDMODE_WH2004	1	/* ��� ������������ ���������� 20*4 */
	//#define LCDMODE_RDX0077	1	/* ��������� 128*64 � ������������ UC1601.  */
	//#define LCDMODE_RDX0154	1	/* ��������� 132*64 � ������������ UC1601.  */
	//#define LCDMODE_UC1601S_TOPDOWN	1	/* LCDMODE_RDX0154 - ����������� ����������� */
	//#define LCDMODE_UC1601S_EXTPOWER	1	/* LCDMODE_RDX0154 - ��� ����������� ��������������� */
	//#define LCDMODE_RDX0120	1	/* ��������� 64*32 � ������������ UC1601.  */
	//#define LCDMODE_TIC154	1	/* ��������� 133*65 � ������������ PCF8535 */
	//#define LCDMODE_TIC218	1	/* ��������� 133*65 � ������������ PCF8535 */
	//#define LCDMODE_PCF8535_TOPDOWN	1	/* ���������� PCF8535 - ����������� ����������� */
	//#define LCDMODE_LS020 	1	/* ��������� 176*132 Sharp LS020B8UD06 � ������������ LR38826 */
	//#define LCDMODE_LS020_TOPDOWN	1	/* LCDMODE_LS020 - ����������� ����������� */
	//#define LCDMODE_LPH88		1	/* ��������� 176*132 LPH8836-2 � ������������ Hitachi HD66773 */
	//#define LCDMODE_LPH88_TOPDOWN	1	/* LCDMODE_LPH88 - ����������� ����������� */
	//#define LCDMODE_S1D13781	1	/* ���������� 480*272 � ������������ Epson S1D13781 */
	//#define LCDMODE_S1D13781_TOPDOWN	1	/* LCDMODE_S1D13781 - ����������� ����������� */
	#define LCDMODE_ILI9225	1	/*! ��������� 220*176 SF-TC220H-9223A-N_IC_ILI9225C_2011-01-15 � ������������ ILI9225� */
	//#define LCDMODE_ILI9225_TOPDOWN	1	/* LCDMODE_ILI9225 - ����������� ����������� (��� ������� ����� �� ������) */
	//#define LCDMODE_UC1608	1		/* ��������� 240*128 � ������������ UC1608.- ����������� */
	//#define LCDMODE_UC1608_TOPDOWN	1	/* LCDMODE_UC1608 - ����������� ����������� (��� ������� ������) */
	//#define LCDMODE_ST7735	1	/* ��������� 160*128 � ������������ Sitronix ST7735 */
	//#define LCDMODE_ST7735_TOPDOWN	1	/* LCDMODE_ST7735 - ����������� ����������� (��� ������� ������) */
	//#define LCDMODE_ST7565S	1	/* ��������� WO12864C2-TFH# 128*64 � ������������ Sitronix ST7565S */
	//#define LCDMODE_ST7565S_TOPDOWN	1	/* LCDMODE_ST7565S - ����������� ����������� (��� ������� ������) */
	//#define LCDMODE_ILI9163	1	/* ��������� LPH9157-2 176*132 � ������������ ILITEK ILI9163 - ����� ������� ������, � ������ �����. */
	//#define LCDMODE_ILI9163_TOPDOWN	1	/* LCDMODE_ILI9163 - ����������� ����������� (��� ������� ������, ����� ������� �����) */
	//#define LCDMODE_ILI9320	1	/* ��������� 248*320 � ������������ ILI9320 */
	//#define LCDMODE_ILI9320_TOPDOWN	1	/* LCDMODE_ILI9320 - ����������� ����������� (��� �������� � ������� ������) */
	// --- ���� �� ���� ����� ���������� ��� �������, ��� �������� ������������� ��������

	#define ENCRES_DEFAULT ENCRES_128
	//#define ENCRES_DEFAULT ENCRES_24
	#define	WITHENCODER	1	/* ��� ��������� ������� ������� ������� */

	#define ENCODER_REVERSE	1	/* ������ � ������� ������� */

	// +++ ��� ������ ����� ���������, �������� ���������������� �������� �������
	#define WITHTX		1	/* �������� ���������� ������������ - ���������, ����������� ����. */
	//#define WITHPBT		1	/* ������������ PBT */
	#define WITHIFSHIFT	1	/* ������������ IF SHIFT */
	#define WITHIFSHIFTOFFSET	(-250)	/* ��������� �������� IF SHIFT */
	#define WITHCAT		1	/* ������������ CAT */
	//#define WITHVOX		1	/* ������������ VOX */
	#if 1
		#define WITHSHOWSWRPWR 1	/* �� ������� ������������ ������������ SWR-meter � PWR-meter */
		#define WITHSWRMTR	1	/* ���������� ��� */
		#define WITHPWRLIN	1	/* ��������� �������� �������� ���������� ���������� � �� �������� */
	#else
		#define WITHPWRMTR	1	/*! ��������� �������� �������� ��� */
		#define WITHPWRLIN	1	/* ��������� �������� �������� ���������� ���������� � �� �������� */
	#endif
	#define WITHBARS	1	/* ����������� S-����� � SWR-����� */
	#define WITHVOLTLEVEL	1	/* ����������� ���������� ��� */
	//#define WITHSWLMODE	1	/* ��������� ����������� ��������� ������ � swl-mode */
	//#define WITHPOTWPM		1	/* ������������ ����������� �������� �������� � ��������� �������������� */
	#define WITHVIBROPLEX	1	/* ����������� �������� �������� ������������ */

	#define WITHMENU 	1	/* ���������������� ���� ����� ���� ��������� - ���� ����������� ������ */
	//#define WITHDEBUG		1	/* ���������� ������ ����� COM-����. ��� CAT (WITHCAT) */
	#define WITHSAMEBFO	1	/* ������������� ����� �������� BFO ��� ����� � �������� */
	#define WITHWARCBANDS	1	/* � ������� ���������� ������������ HF WARC ��������� */
	//#define WITHONLYBANDS 1		/* ����������� ����� ���� ���������� ������������� ����������� */

	//#define WITHSLEEPTIMER	1	/* ��������� ��������� � ����� ����� �� ���������� ���������� ������� */
	// --- ��� ������ ����� ���������, �������� ���������������� �������� �������

	//#define WITHWFM	1			/* ������������ WFM */
	//#define LO1PHASES	1		/* ������ ������ ������� ���������� ����� DDS � ��������������� ������� ���� */
	#define DEFPREAMPSTATE 	0	/* ��� �� ��������� ������� (1) ��� �������� (0) */

	// ���������������� �������� ������������.
	#define BANDSELSTYLERE_SW20XX 1
	//#define BANDSELSTYLERE_RX3QSP 1	/* ����������� ������� ����������� �������� �� ������� RX3QSP */
	//#define BANDSELSTYLERE_R3PAV 1        /* 8 ����������� ��� ��� 12� � 10� ���������� ��� ����������� 74HCT238 ftp://shodtech.net/SW2012/%D0%A4%D0%9D%D0%A7/ */

	/* ��� �� ������ �������� � ������ ������������ � ����������� */
	//#define NVRAM_TYPE NVRAM_TYPE_FM25XXXX	// SERIAL FRAM AUTODETECT
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L04	// ��� �� ��� ������������� FM25040A - 5 �����, 512 ����
	#define NVRAM_TYPE NVRAM_TYPE_FM25L16
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L64
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L256	// FM25L256, FM25W256
	//#define NVRAM_TYPE NVRAM_TYPE_CPUEEPROM

	//#define NVRAM_TYPE NVRAM_TYPE_AT25040A
	//#define NVRAM_TYPE NVRAM_TYPE_AT25L16
	//#define NVRAM_TYPE NVRAM_TYPE_AT25256A

	// End of NVRAM definitions section

	//#define RTC1_TYPE RTC_TYPE_M41T81	/* ST M41T81M6/M41T81SM6F RTC clock chip with I2C interface */
	#define RTC1_TYPE RTC_TYPE_DS1307	/* MAXIM DS1307/DS3231 RTC clock chip with I2C interface */

	/* ���������� ����������� SW2011 */
	//#define WITHLO1LEVELADJ		1	/* �������� ���������� ������� (����������) LO1 */

	#define FTW_RESOLUTION 32	/* ����������� FTW ���������� DDS */

	/* Board hardware configuration */
	//#define DDS1_TYPE DDS_TYPE_AD9951	// UT0IS/D0ISM 
	#define PLL1_TYPE PLL_TYPE_SI570	// US2IT/D0ITC
	//#define PLL1_FRACTIONAL_LENGTH	28	/* Si570: lower 28 bits is a fractional part */
	//#define DDS1_TYPE DDS_TYPE_ATTINY2313	1
	//#define DDS1_TYPE DDS_TYPE_AD9834	1
	//#define DDS2_TYPE DDS_TYPE_AD9835
	#define DDS2_TYPE DDS_TYPE_AD9834
	//#define PLL1_TYPE PLL_TYPE_ADF4001
	//#define PLL2_TYPE PLL_TYPE_ADF4001	/* �������� ��������� ���������� ��� ��������� ��������� ��������� */

	//#define DDS1_CLK_DIV	7		/* �������� ������� ������� ����� ������� � DDS1 ATTINY2313 */
	#define DDS1_CLK_DIV	1		/* �������� ������� ������� ����� ������� � DDS1 */
	#define DDS2_CLK_DIV	1		/* �������� ������� ������� ����� ������� � DDS2 */

	/* ���������� ������� �� SPI ���� */
	#define targetdds1 SPI_CSEL0	/* DDS1 - LO1 generator */
	#define targetdds2 SPI_CSEL1	/* DDS2 - LO3 generator */
	//#define targetpll1 SPI_CSEL2	/* ADF4001 after DDS1 - divide by r1 and scale to n1. ��� ������� DDS ������� ���������� - ������ DDS */
	#define targetlcd SPI_CSEL2 	/* LCD over SPI line devices control */
	#define targetuc1608 SPI_CSEL255	/* LCD with positive chip select signal	*/
	//#define targetpll2 SPI_CSEL3	/* ADF4001 - fixed 2-nd LO generate or LO1 divider */ 
	#define targetctl1 SPI_CSEL4	/* control register */
	#define targetnvram SPI_CSEL5 	/* serial nvram */


	/* ���� �������� ������ ��, ���������� �� ����������� */
	#define BOARD_FILTER_0P5		BOARD_FILTERCODE_1	/* 0.5 or 0.3 kHz filter */
	#define BOARD_FILTER_1P8		BOARD_FILTERCODE_1	/* 1.8 kHz filter - ���������� ������ ������� 0.5 */
	#define BOARD_FILTER_2P7		BOARD_FILTERCODE_0	/* 3.1 or 2.75 kHz filter */
	//#define BOARD_FILTER_WFM		2	/* 3.1 or 2.75 kHz filter */

	#define WITHMODESETMIXONLY3 1

	#define IF3_FMASK (IF3_FMASK_2P7 | /*IF3_FMASK_1P8 | */IF3_FMASK_0P5)
	#define IF3_FMASKTX (IF3_FMASK_2P7)
	#define IF3_FHAVE (IF3_FMASK_2P7 | IF3_FMASK_0P5)

	#define WITHSPLIT	1	/* ���������� �������� ���������� ����� ������� */
	//#define WITHSPLITEX	1	/* ������������� ���������� �������� ���������� */
	#define WITHCATEXT	1	/* ����������� ����� ������ CAT */
	#define WITHELKEY	1
	//#define WITHKBDENCODER 1	// ����������� ������� ��������
	#define WITHKEYBOARD 1	/* � ������ ���������� ���� ���������� */
	#define KEYBOARD_USE_ADC	1	/* �� ����� ����� �����������  ������  �������. �� vref - 6.8K, ����� 2.2�, 4.7� � 13K. */

	// ���������� ������ ��� ����������.
	enum 
	{ 
	#if WITHBARS
		SMETERIX = 1,	// S-meter
		#if WITHPWRMTR
			PWRI = 0,		// ��������� �������� �����������
		#endif
		#if WITHSWRMTR && 0	// ������� �������
			PWRI = 0,		// ��������� �������� �����������
			FWD = 4, REF = 3,	// SWR-meter
		#endif
		#if WITHSWRMTR && 1	// US2IT
			PWRI = 4,		// ��������� �������� �����������
			FWD = 4, REF = 3,	// SWR-meter
		#endif
	#endif /* WITHBARS */

	#if WITHVOLTLEVEL 
		VOLTSOURCE = 2, // ������� ����� �������� ����������, ��� ���
	#endif /* WITHVOLTLEVEL */


	#if WITHVOX
		// �� ���� �� ADC2 � ADC3. (c) US2IT
		VOXIX = 2, AVOXIX = 3,	// VOX
	#endif
	#if WITHPOTWPM
		POTWPM = 4,		// ������������ ���������� ��������� �������� � ���������
	#endif
		 KI0 = 5, KI1 = 6, KI2 = 7	// ����������
	};

	#define VOLTLEVEL_UPPER		47	// 4.7 kOhm - ������� �������� �������� ������� ����������
	#define VOLTLEVEL_LOWER		10	// 1.0 kOhm - ������ ��������

	#define KI_COUNT 3	// ���������� ������������ ��� ���������� ������ ���

#endif /* ATMEGA_CTLSTYLE_V9SF_US2IT_H_INCLUDED */
