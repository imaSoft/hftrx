/* $Id$ */
/* board-specific CPU attached signals */
//
// ������ HF Dream Receiver (�� ������� �����)
// ����� ���� ����������� mgs2001@mail.ru
// UA1ARN
//

#ifndef ARM_CTLSTYLE_V7_H_INCLUDED
#define ARM_CTLSTYLE_V7_H_INCLUDED 1
	/* ������ ������������ - ����� ������������������ */

#if 1
	/* ������ ��������� ���� - � ��������������� "������" */
	//#define FQMODEL_73050		1	// 1-st IF=73.050, 2-nd IF=0.455 MHz
	//#define FQMODEL_73050_IF0P5		1	// 1-st IF=73.050, 2-nd IF=0.5 MHz
	//#define FQMODEL_80455		1	// 1-st IF=80.455, 2-nd IF=0.455 MHz
	//#define FQMODEL_64455		1	// 1-st IF=64.455, 2-nd IF=0.455 MHz
	#define FQMODEL_64455_IF0P5		1	// 1-st IF=64.455, 2-nd IF=0.5 MHz
	//#define FQMODEL_70455		1	// 1-st IF=70.455, 2-nd IF=0.455 MHz
	//#define FQMODEL_70200		1	// 1-st if = 70.2 MHz, 2-nd IF-200 kHz
	#define BANDSELSTYLERE_UPCONV56M	1	/* Up-conversion with working band .030..56 MHz */

#else
	/* ������ ��������� ���� - � ��������������� "������" */
	//#define FQMODEL_45_IF8868	1	// SW2011
	//#define FQMODEL_45_IF0P5	1	// 1-st IF=45MHz, 2-nd IF=500 kHz
	#define FQMODEL_45_IF455	1	// 1-st IF=45MHz, 2-nd IF=455 kHz
	#define BANDSELSTYLERE_UPCONV36M	1	/* Up-conversion with working band 0.1 MHz..36 MHz */
	//#define BANDSELSTYLERE_UPCONV32M	1	/* Up-conversion with working band 0.1 MHz..32 MHz */
#endif


	#define	FONTSTYLE_ITALIC	1	//

	#define CTLREGMODE_V7_ARM 1
	// --- �������� ��������, ������������� ��� ������ ����

	// +++ �������� ��������, ������������� ��� ���� �� ATMega
	// --- �������� ��������, ������������� ��� ���� �� ATMega

	// +++ ������ �������� ������������ ������ �� ����������
	//#define KEYB_RA1AGG	1	/* ������������ ������ ��� �������, ���������� RA1AGG - ��� ������ �� ������ ������� ������ ���� �� ������ � ���� ���. */
	//#define KEYB_UA1CEI	1	/* ������������ ������ ��� UA1CEI */
	// --- ������ �������� ������������ ������ �� ����������

	// +++ ���� �� ���� ����� ���������� ��� �������, ��� �������� ������������� ��������
	//#define LCDMODE_HARD_SPI	1		/* LCD over SPI line */
	//#define LCDMODE_WH1602	1	/* ��� ������������ ���������� 16*2 */
	#define LCDMODE_WH2002	1	/* ��� ������������ ���������� 20*2, �������� ������ � LCDMODE_HARD_SPI */
	//#define LCDMODE_WH2004	1	/* ��� ������������ ���������� 20*4 */
	// --- ���� �� ���� ����� ���������� ��� �������, ��� �������� ������������� ��������

	#define ENCRES_DEFAULT ENCRES_128
	//#define ENCRES_DEFAULT ENCRES_100
	//#define ENCRES_DEFAULT ENCRES_24
	#define WITHDIRECTFREQENER	1 //(! CTLSTYLE_SW2011ALL && ! CTLSTYLE_UA3DKC)
	#define	WITHENCODER	1	/* ��� ��������� ������� ������� ������� */
	#define WITHNESTEDINTERRUPTS	1	/* ������������ ��� ������� real-time �����. */

	// +++ ��� ������ ����� ���������, �������� ���������������� �������� �������
	#define WITHTX		1	/* �������� ���������� ������������ - ���������, ����������� ����. */
	//#define WITHPBT		1	/* ������������ PBT */
	#define WITHCAT		1	/* ������������ CAT */
	#define WITHVOX		1	/* ������������ VOX */
	#define WITHSWRMTR	1	/* ���������� ��� */
	//#define WITHPWRMTR	1	/* ��������� �������� �������� ��� */
	//#define WITHVOLTLEVEL	1	/* ����������� ���������� ��� */
	#define WITHBARS	1	/* ����������� S-����� � SWR-����� */
	//#define WITHSWLMODE	1	/* ��������� ����������� ��������� ������ � swl-mode */

	#define WITHMENU 	1	/* ���������������� ���� ����� ���� ��������� - ���� ����������� ������ */
	//#define WITHDEBUG		1	/* ���������� ������ ����� COM-����. ��� CAT (WITHCAT) */
	#define WITHVIBROPLEX	1	/* ����������� �������� �������� ������������ */
	#define WITHSLEEPTIMER	1	/* ��������� ��������� � ����� ����� �� ���������� ���������� ������� */
	// --- ��� ������ ����� ���������, �������� ���������������� �������� �������

	//#define LO1PHASES	1		/* ������ ������ ������� ���������� ����� DDS � ��������������� ������� ���� */
	//#define WITHONLYBANDS 1		/* ����������� ����� ���� ���������� ������������� ����������� */
	#define WITHBCBANDS	1		/* � ������� ���������� ������������ ����������� ��������� */
	#define WITHWARCBANDS	1	/* � ������� ���������� ������������ HF WARC ��������� */
	#define DEFPREAMPSTATE 	1	/* ��� �� ��������� ������� (1) ��� �������� (0) */
	#define REQUEST_BA	1		/* ��� �.�. - ������ �� ��������� ������� WITH2K4SSB */
	#define WITH2K4SSB	1		/* �� ����� 6 ��� ������� ����� 2.7 ��� ��� */
	#define WITHMODESETFULLNFM 1	/* ������������ FM */

	/* ��� �� ������ �������� � ������ ������������ � ����������� */
	//#define NVRAM_TYPE NVRAM_TYPE_FM25XXXX	// SERIAL FRAM AUTODETECT
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L04	// ��� �� ��� ������������� FM25040A - 5 �����, 512 ����
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L16
	//#define NVRAM_TYPE NVRAM_TYPE_FM25L64
	#define NVRAM_TYPE NVRAM_TYPE_FM25L256	// FM25L256, FM25W256
	//#define NVRAM_TYPE NVRAM_TYPE_CPUEEPROM

	//#define NVRAM_TYPE NVRAM_TYPE_AT25040A
	//#define NVRAM_TYPE NVRAM_TYPE_AT25L16
	//#define NVRAM_TYPE NVRAM_TYPE_AT25256A


	// End of NVRAM definitions section

/* �����, �� ������� ������ DDS - ��� AD9834, ������ - AD9951. ����� ���, ��� FM ��������� �� ������ �� */


#define FTW_RESOLUTION 32	/* ����������� FTW ���������� DDS */

/* Board hardware configuration */
#define DDS1_TYPE DDS_TYPE_AD9951
#define DDS2_TYPE DDS_TYPE_AD9834
#define PLL1_TYPE PLL_TYPE_ADF4001	// ��������������, ���� ������ ��������� ��� ������ DDS ��� ��������.
//#define PLL2_TYPE PLL_TYPE_ADF4001
#define DDS1_CLK_DIV	1		/* �������� ������� ������� ����� ������� � DDS1 */
#define DDS2_CLK_DIV	1		/* �������� ������� ������� ����� ������� � DDS2 */
#define HYBRID_NVFOS 6				/* ���������� ��� ������� ���������� */
#define HYBRID_PLL1_POSITIVE 1		/* � ����� ���� ��� �������������� ��������� */
//#define HYBRID_PLL1_NEGATIVE 1		/* � ����� ���� ���� ������������� ��������� */

/* ���������� ������� �� SPI ���� */
#define targetdds1 SPI_CSEL0 	/* DDS1 - AD9951 */
#define targetdds2 SPI_CSEL1 	/* DDS2 - AD9834 LO3 output */
#define targetpll1 SPI_CSEL2 	/* ADF001 after DDS1 - divide by r1 and scale to n1. ��� ������� DDS ������� ���������� - ������ DDS */
//#define targetpll2 SPI_CSEL3 	/* ADF4002 - fixed 2-nd LO generate */ 
#define targetrxc1 SPI_CSEL5 	/* RX control register */
#define targetnvram SPI_CSEL6 	/* nvram serial nvram */
#define targetctldac1 SPI_CSEL7	/* control register and DAC */

/* ���� ������ ����������� ���������� ������� ��� ��� �������� */
#define BOARD_DETECTOR_MUTE 0x02
#define BOARD_DETECTOR_SSB 0x00
#define BOARD_DETECTOR_AM 0x01
#define BOARD_DETECTOR_FM 0x03
#define BOARD_DETECTOR_TUNE 0x00	/* ������������ ����� ��� ������ TUNE (CWZ �� ��������) */

/* ���� �������� ������ ��, ���������� �� ����������� */
#define BOARD_FILTER_0P5	0x03	/* 0.5 or 0.3 kHz filter */
//#define BOARD_FILTER_WIDE9	0x00	/* 9 kHz filter - �� ����� ������ ������ */
//#define BOARD_FILTER_SEMINARROW	0x01	/* 1.8 kHz filter - �� ����� ������ ������ */
#define BOARD_FILTER_3P1		0x06	/* 3.1 or 2.75 kHz filter */
#define BOARD_FILTER_6P0	0x02	/* 6 kHz filter (2.75 kHz filter if WITH2K4SSB defined) */
#define BOARD_FILTER_8P0	0x04	/* only 1-st IF filter used (6, 8 or 15 khz width) */

	//#define WITHMODESETSMART 1	/* � ����������� �� ������� �������� �������, ������������ ������ � WITHFIXEDBFO */
	#if FQMODEL_64455_IF0P5 || FQMODEL_73050_IF0P5
		//#define WITHMODESETFULL 1
		#define WITHMODESETFULLNFM 1
		//#define WITHWFM	1			/* ������������ WFM */
		#define IF3_FMASK (IF3_FMASK_8P0 | IF3_FMASK_6P0 | IF3_FMASK_3P1 | IF3_FMASK_0P5)
		#define IF3_FHAVE (IF3_FMASK_8P0 | IF3_FMASK_3P1 | IF3_FMASK_0P5)
		#define IF3_FMASKTX	(IF3_FMASK_3P1)
	#elif FQMODEL_64455 || FQMODEL_73050
		//#define WITHMODESETFULL 1
		#define WITHMODESETFULLNFM 1
		//#define WITHWFM	1			/* ������������ WFM */
		#define IF3_FMASK (IF3_FMASK_8P0 | IF3_FMASK_6P0  | IF3_FMASK_3P1 | IF3_FMASK_0P5)
		#define IF3_FHAVE (IF3_FMASK_8P0 | IF3_FMASK_6P0  | IF3_FMASK_3P1 | IF3_FMASK_0P5)
		#define IF3_FMASKTX	(IF3_FMASK_3P1)
	#else
		//#define WITHMODESETFULL 1
		#define WITHMODESETFULLNFM 1
		//#define WITHWFM	1			/* ������������ WFM */
		#define IF3_FMASK (IF3_FMASK_15P0 | IF3_FMASK_3P1 | IF3_FMASK_0P5)
		#define IF3_FHAVE (IF3_FMASK_15P0 | IF3_FMASK_3P1 | IF3_FMASK_0P5)
		#define IF3_FMASKTX	(IF3_FMASK_3P1)
	#endif

#define WITHSPLIT	1	/* ���������� �������� ���������� ����� ������� */
//#define WITHSPLITEX	1	/* ������������� ���������� �������� ���������� */
#define WITHCATEXT	1	/* ����������� ����� ������ CAT */
#define WITHELKEY	1
#define WITHKBDENCODER 1	// ����������� ������� ��������
#define WITHKEYBOARD 1	/* � ������ ���������� ���� ���������� */
#define KEYBOARD_USE_ADC	1	/* �� ����� ����� �����������  ������  �������. �� vref - 6.8K, ����� 2.2�, 4.7� � 13K. */
// ���������� ������ ��� ����������.
enum 
{ 
#if WITHVOX
	VOXIX = 2, AVOXIX = 1,	// VOX
#endif
	SMETERIX = 0,	// S-meter
	PWRI = 4,
	FWD = 4, REF = 3,	// SWR-meter
	KI0 = 5, KI1 = 6, KI2 = 7	// ����������
};
#define KI_COUNT 3	// ���������� ������������ ��� ���������� ������ ���
	#define KI_LIST	KI2, KI1, KI0,	// �������������� ��� ������� �������������

#endif /* ARM_CTLSTYLE_V7_H_INCLUDED */
