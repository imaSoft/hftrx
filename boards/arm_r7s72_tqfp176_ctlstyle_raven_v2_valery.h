/* $Id$ */
/* board-specific CPU attached signals */
//
// ������ HF Dream Receiver (�� ������� �����)
// ����� ���� ����������� mgs2001@mail.ru
// UA1ARN
//
// ��������� � DSP ���������� "Storch" �� ���������� Renesas R7S721020VCFP � ������� NAU8822L � FPGA EP4CE22E22I7N
// SD-card
// Rmainunit_v7cm.pcb

#ifndef ARM_R7S72_TQFP176_CTLSTYLE_RAVEN_V2_H_INCLUDED
#define ARM_R7S72_TQFP176_CTLSTYLE_RAVEN_V2_H_INCLUDED 1

	//#define WITHSAICLOCKFROMI2S 1	/* ���� SAI1 ����������� �� PLL I2S */
	#define WITHI2SCLOCKFROMPIN 1	// �������� ������� �� SPI2 (I2S) �������� � �������� ����������, � ��������� �������� ����� MCK ������ ����������
	#define WITHSAICLOCKFROMPIN 1	// �������� ������� �� SAI1 �������� � �������� ����������, � ��������� �������� ����� MCK ������ ����������

	#define WITHCPUXTAL 12000000uL			/* �� ���������� ���������� ����� 12.000 ��� */

	#if WITHI2SCLOCKFROMPIN
		#define FPGADECIMATION 2560
		#define FPGADIVIDERATIO 5
		#define EXTI2S_FREQ (REFERENCE_FREQ * DDS1_CLK_MUL / FPGADIVIDERATIO)
		#define EXTSAI_FREQ (REFERENCE_FREQ * DDS1_CLK_MUL / FPGADIVIDERATIO)

		#define ARMI2SMCLK	(REFERENCE_FREQ * DDS1_CLK_MUL / (FPGADECIMATION / 256))
		#define ARMSAIMCLK	(REFERENCE_FREQ * DDS1_CLK_MUL / (FPGADECIMATION / 256))
	#else /* WITHI2SCLOCKFROMPIN */
		#define PLLI2SN_MUL 336		// 344.064 (192 <= PLLI2SN <= 432)
		#define SAIREF1_MUL 240		// 245.76 / 1.024 = 240 (49 <= PLLSAIN <= 432)
		// ������� ����������� �����������
		#define ARMI2SMCLK	(12288000uL)
		#define ARMSAIMCLK	(12288000uL)
	#endif /* WITHI2SCLOCKFROMPIN */

	/* ������ ������������ - ����� ������������������ */

	/* ������ ��������� ���� - � ��������������� "������" */
	//#define FQMODEL_45_IF8868_UHF430	1	// SW2011
	//#define FQMODEL_73050		1	// 1-st IF=73.050, 2-nd IF=0.455 MHz
	//#define FQMODEL_73050_IF0P5		1	// 1-st IF=73.050, 2-nd IF=0.5 MHz
	//#define FQMODEL_80455		1	// 1-st IF=80.455, 2-nd IF=0.455 MHz
	//#define FQMODEL_64455_IF0P5		1	// 1-st IF=64.455, 2-nd IF=0.5 MHz
	//#define FQMODEL_70455		1	// 1-st IF=70.455, 2-nd IF=0.455 MHz
	//#define FQMODEL_70200		1	// 1-st if = 70.2 MHz, 2-nd IF-200 kHz
	//#define FQMODEL_60700_IF05	1	// 60.7 -> 10.7 -> 0.5
	//#define FQMODEL_60725_IF05	1	// 60.725 -> 10.725 -> 0.5
	//#define FQMODEL_60700_IF02	1	// 60.7 -> 10.7 -> 0.2
	//#define FQMODEL_70200		1	// 1-st if = 70.2 MHz, 2-nd IF-200 kHz

	#define FQMODEL_FPGA		1	// FPGA + IQ over I2S
	#if 0
		#define DIRECT_80M0_X1		1	/* �������� ��������� �� ����� 80.0 ��� */	
		#define BANDSELSTYLERE_UPCONV56M_36M	1	/* Up-conversion with working band .030..36 MHz */
	#elif 0
		#define DIRECT_100M0_X1		1	/* �������� ��������� �� ����� 100.0 ��� */	
		#define BANDSELSTYLERE_UPCONV56M_45M	1	/* Up-conversion with working band .030..45 MHz */
	#else
		//#define DIRECT_125M0_X1		1	/* �������� ��������� �� ����� 125.0 ��� */	
		#define DIRECT_122M88_X1	1	/* �������� ��������� 122.880 ��� */	
		#define BANDSELSTYLERE_UPCONV56M	1	/* Up-conversion with working band .030..56 MHz */
	#endif
	// --- �������� ��������, ������������� ��� ������ ����

	#define CTLREGMODE_RAVENDSP_V2	1	/* renesas */

	#define WITHLCDBACKLIGHT	1	// ������� ���������� ���������� ������� 
	#define WITHLCDBACKLIGHTMIN	0	// ������ ������ ����������� (������������ �� �������)
	#define WITHLCDBACKLIGHTMAX	3	// ������� ������ ����������� (������������ �� �������)
	#define WITHKBDBACKLIGHT	1	// ������� ���������� ���������� ���������� 

	/* ���� ������ ����������� ���������� ������� ��� ��� �������� */
	#define BOARD_DETECTOR_SSB 	0		// ��������

	// +++ �������� ��� ���� � DSP ����������
	#define	BOARD_AGCCODE_ON	0x00
	#define	BOARD_AGCCODE_OFF	0x01

	/* ���� �������� ������ ��, ���������� �� ����������� */
	#define BOARD_FILTER_0P5		1	/* 0.5 or 0.3 kHz filter */
	#define BOARD_FILTER_3P1		0	/* 3.1 or 2.75 kHz filter */
	#define BOARD_FILTER_6P0		0	/* 6.0 kHz filter */
	#define BOARD_FILTER_8P0		0	/* 6.0 kHz filter */
	// --- �������� ��� ���� � DSP ����������

	#define WITHPREAMPATT2_6DB		1	// ���������� ��� � ������������� ������������ � ����������� 0 - 6 - 12 - 18 dB */
	#define DEFPREAMPSTATE 	0	/* ��� �� ��������� ������� (1) ��� �������� (0) */
	//#define WITHATT2_6DB		1		// LTC2217 ���������� ������������� ������������ � ����������� 0 - 6 - 12 - 18 dB ��� ���

	#define WITHAGCMODEONOFF	1	// ��� ���/����
	#define	WITHMIC1LEVEL		1	// ��������� �������� ���������

	//#define DSTYLE_UR3LMZMOD	1	// ������������ ��������� ������ � ����������� UR3LMZ
	#define	FONTSTYLE_ITALIC	1	// ������������ �������������� �����

	// +++ ������ �������� ������������ ������ �� ����������
	//#define KEYB_FPAMEL20_V0A	1	/* 20 ������ �� 5 ����� - ����� rfrontpanel_v0 + LCDMODE_UC1608 � ���������� ������������ */
	#define KEYB_RAVEN20_V5	1		/* 5 ����� ����������: ������������ ������ ��� ������� � DSP ���������� */
	// --- ������ �������� ������������ ������ �� ����������
	#define WITHSPLIT	1	/* ���������� �������� ���������� ����� ������� */
	//#define WITHSPLITEX	1	/* ������������� ���������� �������� ���������� */

	// +++ ���� �� ���� ����� ���������� ��� �������, ��� �������� ������������� ��������
	//#define LCDMODE_HARD_SPI	1	/* LCD over SPI line */
	//#define LCDMODE_LTDC	1		/* Use STM32F4xxx with LCD-TFT Controller (LTDC), also need LCDMODE_ILI9341 */
	//#define LCDMODE_LTDC_L8	1	/* ������������ 8 ��� �� ������� ������������� ������. ����� - 16 ���. */
	//#define LCDMODE_LTDCSDRAMBUFF	1	/* ������������ ������� �������� SDRAM ��� �������� framebuffer */
	//#define LCDMODE_WH2002	1	/* ��� ������������ ���������� 20*2, �������� ������ � LCDMODE_HARD_SPI */
	#define LCDMODE_WH1602	1	/* ��� ������������ ���������� 16*2 */
	//#define LCDMODE_WH1604	1	/* ��� ������������ ���������� 16*4 */
	//#define LCDMODE_WH2004	1	/* ��� ������������ ���������� 20*4 */
	//#define LCDMODE_RDX0077	1	/* ��������� 128*64 � ������������ UC1601.  */
	//#define LCDMODE_RDX0154	1	/* ��������� 132*64 � ������������ UC1601. */
	//#define LCDMODE_RDX0120	1	/* ��������� 64*32 � ������������ UC1601.  */
	//#define LCDMODE_UC1601S_XMIRROR	1	/* ��������� 132*64 � ������������ UC1601.  */
	//#define LCDMODE_UC1601S_TOPDOWN	1	/* LCDMODE_RDX0154 - ����������� ����������� */
	//#define LCDMODE_UC1601S_EXTPOWER	1	/* LCDMODE_RDX0154 - ��� ����������� ��������������� */
	//#define LCDMODE_TIC154	1	/* ��������� 133*65 � ������������ PCF8535 */
	//#define LCDMODE_TIC218	1	/* ��������� 133*65 � ������������ PCF8535 */
	//#define LCDMODE_PCF8535_TOPDOWN	1	/* ���������� PCF8535 - ����������� ����������� */
	//#define LCDMODE_LS020 	1	/* ��������� 176*132 Sharp LS020B8UD06 � ������������ LR38826 */
	//#define LCDMODE_LS020_TOPDOWN	1	/* LCDMODE_LS020 - ����������� ����������� */
	//#define LCDMODE_LPH88		1	/* ��������� 176*132 LPH8836-2 � ������������ Hitachi HD66773 */
	//#define LCDMODE_LPH88_TOPDOWN	1	/* LCDMODE_LPH88 - ����������� ����������� */
	//#define LCDMODE_ILI9163	1	/* ��������� LPH9157-2 176*132 � ������������ ILITEK ILI9163 - ����� ������� ������, � ������ �����. */
	//#define LCDMODE_ILI9163_TOPDOWN	1	/* LCDMODE_ILI9163 - ����������� ����������� (��� ������� ������, ����� ������� �����) */
	//#define LCDMODE_L2F50	1	/* ��������� 176*132 � ������������ Epson L2F50126 */
	//#define LCDMODE_L2F50_TOPDOWN	1	/* ��������� ���������� � � ������ LCDMODE_L2F50 */
	//#define LCDMODE_S1D13781_NHWACCEL 1	/* �������������� ���������� ������������ EPSON S1D13781 ��� ������ ������� */
	//#define LCDMODE_S1D13781	1	/* ���������� 480*272 � ������������ Epson S1D13781 */
	//#define LCDMODE_S1D13781_TOPDOWN	1	/* LCDMODE_S1D13781 - ����������� ����������� */
	//#define LCDMODE_ILI9225	1	/* ��������� 220*176 SF-TC220H-9223A-N_IC_ILI9225C_2011-01-15 � ������������ ILI9225� */
	//#define LCDMODE_ILI9225_TOPDOWN	1	/* LCDMODE_ILI9225 - ����������� ����������� (��� ������� ����� �� ������) */
	//#define LCDMODE_UC1608	1		/* ��������� 240*128 � ������������ UC1608.- ����������� */
	//#define LCDMODE_UC1608_TOPDOWN	1	/* LCDMODE_UC1608 - ����������� ����������� (��� ������� ������) */
	//#define LCDMODE_ST7735	1	/* ��������� 160*128 � ������������ Sitronix ST7735 - TFT ������ 160 * 128 HY-1.8-SPI */
	//#define LCDMODE_ST7735_TOPDOWN	1	/* LCDMODE_ST7735 - ����������� ����������� (��� ������� ������) */
	//#define LCDMODE_ST7565S	1	/* ��������� WO12864C2-TFH# 128*64 � ������������ Sitronix ST7565S */
	//#define LCDMODE_ST7565S_TOPDOWN	1	/* LCDMODE_ST7565S - ����������� ����������� (��� ������� ������) */
	//#define LCDMODE_ILI9320	1	/* ��������� 248*320 � ������������ ILI9320 */
	//#define LCDMODE_ILI9341	1	/* 320*240 SF-TC240T-9370-T � ������������ ILI9341 - STM32F4DISCO */
	//#define LCDMODE_ILI9341_TOPDOWN	1	/* LCDMODE_ILI9341 - ����������� ����������� (��� ������� ������) */
	//#define LCDMODE_LQ043T3DX02K 1	/* LQ043T3DX02K panel (272*480) - SONY PSP-1000 display */
	// --- ���� �� ���� ����� ���������� ��� �������, ��� �������� ������������� ��������

	#define ENCRES_DEFAULT ENCRES_128
	//#define ENCRES_DEFAULT ENCRES_24
	#define WITHDIRECTFREQENER	1 //(! CTLSTYLE_SW2011ALL && ! CTLSTYLE_UA3DKC)
	#define	WITHENCODER	1	/* ��� ��������� ������� ������� ������� */

	/* Board hardware configuration */
	#define CODEC1_TYPE CODEC_TYPE_NAU8822L
	#define CODEC_TYPE_NAU8822_USE_SPI	1
	//#define	CODEC1_TYPE	CODEC_TYPE_TLV320AIC23B
	//#define CODEC_TYPE_TLV320AIC23B_USE_SPI	1
	//#define CODEC_TYPE_NAU8822_USE_8KS	1	/* ����� �������� � sample rate 8 kHz */

	#define CODEC2_TYPE	CODEC_TYPE_FPGAV1	/* ���������� �������� �� FPGA */

	#define WITHI2S_FORMATI2S_PHILIPS 1	// �������� ������������� ��� �������� ������ � �����, ������������ � ��������� � ���������

	#define WITHSAI1_FORMATI2S_PHILIPS 1	// ��������� ��� ��������� ������ �� FPGA
	#define WITHSAI1_FRAMEBITS 256	// ������ ������ ������ ��� ���� ��������� - ����� ����������

	//#define WITHRTS192	1		// ���� ����� ������������������ - �� ������ �������� WITHSAI2HW
	#define WITHRTS96		1		/* ������ ��������� ������������ �������� ��������� �� USB */

	#define WITHSAI2_FORMATI2S_PHILIPS 1	// ��������� ��� ��������� ������ �� FPGA
	#define WITHSAI2_FRAMEBITS 64	// ������ ������ ������ ��� ���� ��������� �� 24 ���� - ����� ������������������

	#define WITHNESTEDINTERRUPTS	1	/* ������������ ��� ������� real-time �����. */
	#define WITHINTEGRATEDDSP		1	/* � ��������� �������� ������������� � ������ DSP �����. */
	#define WITHIFDACWIDTH	32		// 1 ��� ���� � 31 ��� ��������
	#define WITHIFADCWIDTH	32		// 1 ��� ���� � 31 ��� ��������
	#define WITHAFADCWIDTH	16		// 1 ��� ���� � 15 ��� ��������
	#define WITHAFDACWIDTH	16		// 1 ��� ���� � 15 ��� ��������
	//#define WITHDACOUTDSPAGC		1	/* ��� ����������� ��� ����� ��� �� ���������� �����. */
	//#define WITHEXTERNALDDSP		1	/* ������� ���������� ������� DSP ������. */
	//#define WITHLOOPBACKTEST	1
	#define WITHDSPEXTDDC 1			/* ���������� ���������� ������� ����������� */
	#define WITHDSPEXTFIR 1			/* ���������� ��������� �������������� ������� ����������� */
	#define WITHIF4DSP	1	// "�����"

	#if 0
		#define WITHUSBHEADSET 1	/* ��������� �������� USB ���������� ��� ��������� - ����� ������������ */
		#define WITHBBOX	1	// Black Box mode - ���������� ��� ������� ����������
		#define	WITHBBOXMIKESRC	BOARD_TXAUDIO_USB
	#elif 0
		#define WITHBBOX	1	// Black Box mode - ���������� ��� ������� ����������
		#define	WITHBBOXFREQ	26985000L		// ������� ����� ���������
		//#define	WITHBBOXFREQ	(26985000L - 260)		// ������� ����� ��������� - 135 ��������� ������� ����� �  122.88 ��� ��������� � ���� ����� � 100 ��� ����������� ��� ���������
		//#define	WITHBBOXFREQ	(26985000L - 1600)		// ������� ����� ���������
		//#define	WITHBBOXFREQ	(14070000L - 1000)		// ������������� BPSK ������
		//#define	WITHBBOXFREQ	(14065000L - 135)		// ������� ����� ��������� - 135 ��������� ������� ����� �  122.88 ��� ��������� � ���� ����� � 100 ��� ����������� ��� ���������
		//#define	WITHBBOXFREQ	14065000L		// ������� ����� ���������
		//#define	WITHBBOXFREQ	(14065000L - 1000)		// ������� ����� ���������
		//#define	WITHBBOXSUBMODE	SUBMODE_USB	// ������������ ����� ������
		#define	WITHBBOXSUBMODE	SUBMODE_BPSK	// ������������ ����� ������
		//#define	WITHBBOXFREQ	27100000L		// ������� ����� ���������
		//#define	WITHBBOXSUBMODE	SUBMODE_CW	// ������������ ����� ������
		//#define	WITHBBOXTX		1		// �������������� ������� �� ��������
	#elif 0
		#define WITHBBOX	1	// Black Box mode - ���������� ��� ������� ����������
		#define	WITHBBOXFREQ	136000L		// ������� ����� ���������
		#define	WITHBBOXSUBMODE	SUBMODE_USB	// ������������ ����� ������
		#define WITHBBOXREC	1		// �������������� ��������� ������ ������ ����� ������ �������
	#elif 0
		#define WITHBBOX	1	// Black Box mode - ���������� ��� ������� ����������
		#define	WITHBBOXFREQ	7030000L		// ������� ����� ���������
		#define	WITHBBOXSUBMODE	SUBMODE_LSB	// ������������ ����� ������
		#define	WITHBBOXTX		1		// �������������� ������� �� ��������
		#define	WITHBBOXMIKESRC	BOARD_TXAUDIO_2TONE	// 2: 2tone, 1: noise 
	#endif
	#define WITHUSESDCARD 1			// ��������� ��������� SD CARD
	#define WITHUSEAUDIOREC	1	// ������ ����� �� SD CARD
	#define WITHUSEAUDIORECCLASSIC	1	// ����������� ������ ������, ��� "���"
	//#define WITHUSEAUDIOREC2CH	1	// ������ ����� �� SD CARD � ������

	#define WITHUSEDUALWATCH	1	// ������ ��������
	#define WITHFQMETER	1	/* ���� ����� ��������� ������� �������, �� �������� PPS */

	// FPGA section
	//#define	WITHFPGAWAIT_AS	1	/* FPGA ����������� �� ����������� ���������� ���������� - ��������� ��������� �������� ����� �������������� SPI � ���������� */
	#define	WITHFPGALOAD_PS	1	/* FPGA ����������� ����������� � ������� SPI */

	// +++ ��� ������ ����� ���������, �������� ���������������� �������� �������
	//#define WITHRFSG	1	/* �������� ���������� �� ������-�����������. */
	#define WITHTX		1	/* �������� ���������� ������������ - ���������, ����������� ����. */
	//#define WITHAUTOTUNER	1	/* ���� ������� ���������� */
	#define WITHIFSHIFT	1	/* ������������ IF SHIFT */
	//#define WITHIFSHIFTOFFSET	(-250)	/* ��������� �������� IF SHIFT */
	//#define WITHPBT		1	/* ������������ PBT (���� LO3 ����) */
	#define WITHCAT			1	/* ������������ CAT (����� USB CDC) */
	//#define WITHDEBUG		1	/* ���������� ������ ����� COM-����. */
	//#define WITHNMEA		1	/* ������������ NMEA parser */
	//#define WITHMODEM		1	/* ���������� �������� ��� ���������� � ���������������� ����������� */
	//#define WITHFREEDV	1	/* ��������� ������ FreeDV - http://freedv.org/ */ 
	//#define WITHBEACON	1	/* ������������ ����� ����� */
	#define WITHVOX			1	/* ������������ VOX */
	////#define WITHSHOWSWRPWR 1	/* �� ������� ������������ ������������ SWR-meter � PWR-meter */
	////#define WITHSWRMTR	1		/* ���������� ��� */
	//#define WITHPWRMTR	1	/* ��������� �������� �������� ��� */
	//#define WITHPWRLIN	1	/* ��������� �������� �������� ���������� ���������� � �� �������� */
	#define WITHBARS		1	/* ����������� S-����� � SWR-����� */
	//#define WITHVOLTLEVEL	1	/* ����������� ���������� ��� */
	//#define WITHCURRLEVEL	1	/* ����������� ���� ���������� ������� */
	//#define WITHSWLMODE	1	/* ��������� ����������� ��������� ������ � swl-mode */
	#define WITHVIBROPLEX	1	/* ����������� �������� �������� ������������ */

	// ���� �� ����������� ���������� ���������������
	////#define WITHPOTWPM		1	/* ������������ ����������� �������� �������� � ��������� �������������� */
	#define WITHPOTIFGAIN		1	/* ���������� �������� �� �� �������������� */
	#define WITHPOTAFGAIN		1	/* ���������� �������� �� �� �������������� */
	#define WITHSLEEPTIMER	1	/* ��������� ��������� � ����� ����� �� ���������� ���������� ������� */

	#define WITHMENU 	1	/* ���������������� ���� ����� ���� ��������� - ���� ����������� ������ */

	//#define WITHONLYBANDS 1		/* ����������� ����� ���� ���������� ������������� ����������� */
	//#define WITHBCBANDS	1		/* � ������� ���������� ������������ ����������� ��������� */
	#define WITHWARCBANDS	1	/* � ������� ���������� ������������ HF WARC ��������� */
	//#define WITHLFM		1	/* LFM MODE */
	//#define WITHTEMPSENSOR	1	/* ����������� ������ � ������� ����������� */
	#define WITHDIRECTBANDS 1	/* ������ ������� � ���������� �� �������� �� ���������� */
	#define WITHSPKMUTE		1	/* ���������� ����������� �������� */
	#define WITHDATAMODE	1	/* ���������� � ���������� ��������� � USB AUDIO ������ */
	// --- ��� ������ ����� ���������, �������� ���������������� �������� �������

	/* ��� �� ������ �������� � ������ ������������ � ����������� */
	//#define NVRAM_TYPE NVRAM_TYPE_FM25XXXX	// SERIAL FRAM AUTODETECT
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L04	// ��� �� ��� ������������� FM25040A - 5 �����, 512 ����
	#define NVRAM_TYPE NVRAM_TYPE_FM25L16
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L64
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L256	// FM25L256, FM25W256
	//#define NVRAM_TYPE NVRAM_TYPE_CPUEEPROM

	//#define NVRAM_TYPE NVRAM_TYPE_AT25040A
	//#define NVRAM_TYPE NVRAM_TYPE_AT25L16		// demo board with atxmega128a4u
	//#define NVRAM_TYPE NVRAM_TYPE_AT25256A
	//#define NVRAM_TYPE NVRAM_TYPE_NOTHING	// ��� NVRAM
	//#define HARDWARE_IGNORENONVRAM	1		// ������� �� ������ ��� ��� �������� NVRAM

	// End of NVRAM definitions section
	#define FTW_RESOLUTION 32	/* ����������� FTW ���������� DDS */

	#define MODEL_DIRECT	1	/* ������������ ������ ������, � �� ��������� */
	/* Board hardware configuration */
	#define DDS1_TYPE DDS_TYPE_FPGAV1
	//#define PLL1_TYPE PLL_TYPE_SI570
	//#define PLL1_FRACTIONAL_LENGTH	28	/* Si570: lower 28 bits is a fractional part */
	//#define DDS1_TYPE DDS_TYPE_AD9951
	//#define PLL1_TYPE PLL_TYPE_ADF4001
	//#define DDS2_TYPE DDS_TYPE_AD9834
	#define RTC1_TYPE RTC_TYPE_DS1305	/* MAXIM DS1305EN RTC clock chip with SPI interface */
	//#define TSC1_TYPE TSC_TYPE_TSC2046	/* Resistive touch screen controller TI TSC2046 */
	//#define DAC1_TYPE	99999		/* ������� ��� ��� ���������� �������� ������� */

	#define targetlcd	targetext1 	/* LCD over SPI line devices control */ 
	#define targetuc1608 targetext1	/* LCD with positive chip select signal	*/

	#define DDS1_CLK_DIV	1		/* �������� ������� ������� ����� ������� � DDS1 */

	#define WITHMODESETFULLNFM 1

	//#define WITHWFM	1			/* ������������ WFM */
	/* ��� ��������� � ������ ������������ ������� */
	#define IF3_FMASK	(IF3_FMASK_0P5 | IF3_FMASK_3P1 /* | IF3_FMASK_6P0 | IF3_FMASK_8P0*/)
	/* ��� ��������� � ������ ������������ ������� ��� �������� */
	#define IF3_FMASKTX	(IF3_FMASK_3P1 /*| IF3_FMASK_6P0 */)
	/* �������, ��� ������� ����� ������� HAVE */
	#define IF3_FHAVE	( IF3_FMASK_0P5 | IF3_FMASK_3P1 /*| IF3_FMASK_6P0 | IF3_FMASK_8P0*/)

	#define WITHCATEXT	1	/* ����������� ����� ������ CAT */
	#define WITHELKEY	1
	#define WITHKBDENCODER 1	// ����������� ������� ��������
	#define WITHKEYBOARD 1	/* � ������ ���������� ���� ���������� */
	#define KEYBOARD_USE_ADC	1	/* �� ����� ����� �����������  ������  �������. �� vref - 6.8K, ����� 2.2�, 4.7� � 13K. */

	// ���������� ������ ��� ����������.
	enum 
	{ 
	#if WITHPOTIFGAIN
		POTIFGAIN = 0,		// ADC0 IF GAIN
	#endif /* WITHPOTIFGAIN */
	#if WITHPOTAFGAIN
		POTAFGAIN = 1,		// ADC1 AF GAIN
	#endif /* WITHPOTAFGAIN */

	#if WITHPOTWPM
		POTWPM = 2,		// ADC2 ������������ ���������� ��������� �������� � ���������
	#endif /* WITHPOTWPM */

	#if WITHSWRMTR
		PWRI = 0,			// PB1
		FWD = 0, REF = 1,	// PB0	SWR-meter
	#endif /* WITHSWRMTR */

		KI0 = 3, KI1 = 4, KI2 = 5, KI3 = 6, KI4 = 7		// ����������
	};

	//#define VOLTLEVEL_UPPER		47	// 4.7 kOhm - ������� �������� �������� ������� ����������
	//#define VOLTLEVEL_LOWER		10	// 1.0 kOhm - ������ ��������

	#define KI_COUNT 5	// ���������� ������������ ��� ���������� ������ ���
	#define KI_LIST	KI4, KI3, KI2, KI1, KI0,	// �������������� ��� ������� �������������

#endif /* ARM_R7S72_TQFP176_CTLSTYLE_RAVEN_V2_H_INCLUDED */
