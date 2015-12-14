/*
 * =================================================================
 *
 *       Filename:  smart_mtp_s6e3.h
 *
 *    Description:  Smart dimming algorithm implementation
 *
 *        Company:  Samsung Electronics
 *
 * ================================================================
 */
/*
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) 2012, Samsung Electronics. All rights reserved.

*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
*/
#ifndef _SS_DSI_SMART_DIMMING_S6E3HA2_AMS509ES01_H_
#define _SS_DSI_SMART_DIMMING_S6E3HA2_AMS509ES01_H_

#include "../ss_dsi_panel_common.h"
#include "../ss_dsi_smart_dimming_common.h"

#define MAX_TABLE_HMT 29

enum {
	GAMMA_CURVE_1P9 = 0,
	GAMMA_CURVE_2P15,
	GAMMA_CURVE_2P2,
};

#define LUMINANCE_MAX 65
#define GAMMA_SET_MAX 35
#define BIT_SHIFT 22
/*
	it means BIT_SHIFT is 22.  pow(2,BIT_SHIFT) is 4194304.
	BIT_SHIFT is used for right bit shfit
*/
#define BIT_SHFIT_MUL 4194304

#define GRAY_SCALE_MAX 256

/*6.4*4194304 */
#define VREG0_REF_6P4 26843546

/*6.3*4194304 */
#define VREG0_REF_6P3 26424115

/*6.2*4194304 */
#define VREG0_REF_6P2 26004685

/*V0,V3,V11,V23,V35,V51,V87,V151,V203,V255*/
#define ARRAY_MAX 11

/* PANEL DEPENDENT THINGS */
#define MIN_CANDELA	2

/*
*	ID 0x20
*/
#define V255_300CD_R_MSB_20 0x01
#define V255_300CD_R_LSB_20 0x00

#define V255_300CD_G_MSB_20 0x01
#define V255_300CD_G_LSB_20 0x00

#define V255_300CD_B_MSB_20 0x01
#define V255_300CD_B_LSB_20 0x00

#define V203_300CD_R_20 0x80
#define V203_300CD_G_20 0x80
#define V203_300CD_B_20 0x80

#define V151_300CD_R_20 0x80
#define V151_300CD_G_20 0x80
#define V151_300CD_B_20 0x80

#define V87_300CD_R_20 0x80
#define V87_300CD_G_20 0x80
#define V87_300CD_B_20 0x80

#define V51_300CD_R_20 0x80
#define V51_300CD_G_20 0x80
#define V51_300CD_B_20 0x80

#define V35_300CD_R_20 0x80
#define V35_300CD_G_20 0x80
#define V35_300CD_B_20 0x80

#define V23_300CD_R_20 0x80
#define V23_300CD_G_20 0x80
#define V23_300CD_B_20 0x80

#define V11_300CD_R_20 0x80
#define V11_300CD_G_20 0x80
#define V11_300CD_B_20 0x80

#define V3_300CD_R_20 0x80
#define V3_300CD_G_20 0x80
#define V3_300CD_B_20 0x80

#define V0_300CD_R_20 0x00
#define V0_300CD_G_20 0x00
#define V0_300CD_B_20 0x00

#define VT_300CD_R_20 0x00
#define VT_300CD_G_20 0x00
#define VT_300CD_B_20 0x00

#define PANEL_REVA 0x11
#define PANEL_REVB 0x12
#define PANEL_REVD 0x13
#define PANEL_REVE 0x14
#define PANEL_YM4 0x01

#define AOR_FIX_CD		172
#define AOR_ADJUST_CD		111
#define AOR_DIM_BASE_CD		116
#define AOR_DIM_BASE_CD_REVA	110
#define AOR_DIM_BASE_CD_REVB	112
#define AOR_DIM_BASE_CD_REVD	114

#define AOR_DIM_BASE_CD_REVC	117
#define AOR_DIM_BASE_CD_REVF	119
#define AOR_DIM_BASE_CD_REVH	113


/* PANEL DEPENDENT THINGS END*/

enum {
	V1_INDEX = 0,
	V3_INDEX = 1,
	V11_INDEX = 2,
	V23_INDEX = 3,
	V35_INDEX = 4,
	V51_INDEX = 5,
	V87_INDEX = 6,
	V151_INDEX = 7,
	V203_INDEX = 8,
	V255_INDEX = 9,
};

struct GAMMA_LEVEL {
	int level_0;
	int level_1;
	int level_3;
	int level_11;
	int level_23;
	int level_35;
	int level_51;
	int level_87;
	int level_151;
	int level_203;
	int level_255;
} __packed;

struct RGB_OUTPUT_VOLTARE {
	struct GAMMA_LEVEL R_VOLTAGE;
	struct GAMMA_LEVEL G_VOLTAGE;
	struct GAMMA_LEVEL B_VOLTAGE;
} __packed;

struct GRAY_VOLTAGE {
	/*
		This voltage value use 14bit right shit
		it means voltage is divied by 16384.
	*/
	int R_Gray;
	int G_Gray;
	int B_Gray;
} __packed;

struct GRAY_SCALE {
	struct GRAY_VOLTAGE TABLE[GRAY_SCALE_MAX];
	struct GRAY_VOLTAGE VT_TABLE;
} __packed;

/*V0,V1,V3,V11,V23,V35,V51,V87,V151,V203,V255*/

struct MTP_SET {
	char OFFSET_255_MSB;
	char OFFSET_255_LSB;
	char OFFSET_203;
	char OFFSET_151;
	char OFFSET_87;
	char OFFSET_51;
	char OFFSET_35;
	char OFFSET_23;
	char OFFSET_11;
	char OFFSET_3;
	char OFFSET_0;
	char OFFSET_T;
} __packed;

struct MTP_SET_R {
	char OFFSET_255_MSB;
	char OFFSET_255_LSB;
	char OFFSET_203;
	char OFFSET_151;
	char OFFSET_87;
	char OFFSET_51;
	char OFFSET_35;
	char OFFSET_23;
	char OFFSET_11;
	char OFFSET_3;
	char OFFSET_0;
} __packed;

struct MTP_OFFSET {
	struct MTP_SET_R R_OFFSET;
	struct MTP_SET G_OFFSET;
	struct MTP_SET B_OFFSET;
} __packed;

struct illuminance_table {
	int lux;
	char gamma_setting[GAMMA_SET_MAX];
} __packed;

struct SMART_DIM {
	struct MTP_OFFSET MTP_ORIGN;
	struct MTP_OFFSET MTP;
	struct RGB_OUTPUT_VOLTARE RGB_OUTPUT;
	struct GRAY_SCALE GRAY;

	/* Because of AID funtion, below members are added*/
	int lux_table_max;
	int *plux_table;
	struct illuminance_table gen_table[LUMINANCE_MAX];
	struct illuminance_table hmt_gen_table[MAX_TABLE_HMT];

	int brightness_level;
	int ldi_revision;
	int vregout_voltage;
} __packed;


#define CANDELA_MAX_TABLE 64
#define RGB_COMPENSATION 24

static int candela_table[][2] = {
	{2, 0},
	{3, 1},
	{4, 2},
	{5, 3},
	{6, 4},
	{7, 5},
	{8, 6},
	{9, 7},
	{10, 8},
	{11, 9},
	{12, 10},
	{13, 11},
	{14, 12},
	{15, 13},
	{16, 14},
	{17, 15},
	{19, 16},
	{20, 17},
	{21, 18},
	{22, 19},
	{24, 20},
	{25, 21},
	{27, 22},
	{29, 23},
	{30, 24},
	{32, 25},
	{34, 26},
	{37, 27},
	{39, 28},
	{41, 29},
	{44, 30},
	{47, 31},
	{50, 32},
	{53, 33},
	{56, 34},
	{60, 35},
	{64, 36},
	{68, 37},
	{72, 38},
	{77, 39},
	{82, 40},
	{87, 41},
	{93, 42},
	{98, 43},
	{105, 44},
	{111, 45},
	{119, 46},
	{126, 47},
	{134, 48},
	{143, 49},
	{152, 50},
	{162, 51},
	{172, 52},
	{183, 53},
	{195, 54},
	{207, 55},
	{220, 56},
	{234, 57},
	{249, 58},
	{265, 59},
	{282, 60},
	{300, 61},
	{316, 62},
	{333, 63},
	{360, 64},
};

static int gradation_offset_revA[][9] = {
/*	V255 V203 V151 V87 V51 V35 V23 V11 V3 */
	{0, 9, 17, 22, 27, 33, 35, 27, 4,},
	{0, 8, 12, 17, 17, 25, 29, 29, 4,},
	{0, 7, 12, 16, 19, 21, 24, 26, 23,},
	{0, 5, 9, 13, 15, 17, 20, 22, 21,},
	{0, 6, 10, 12, 14, 16, 19, 21, 21,},
	{0, 5, 9, 11, 13, 14, 18, 20, 21,},
	{0, 4, 8, 10, 12, 13, 17, 19, 19,},
	{0, 4, 7, 9, 12, 13, 16, 19, 18,},
	{0, 4, 7, 9, 10, 12, 15, 18, 20,},
	{0, 4, 7, 8, 10, 11, 15, 17, 16,},
	{0, 4, 6, 8, 10, 11, 14, 16, 17,},
	{0, 4, 6, 7, 10, 11, 14, 16, 17,},
	{0, 4, 5, 6, 9, 10, 13, 15, 17,},
	{0, 4, 6, 7, 9, 10, 12, 15, 15,},
	{0, 4, 5, 6, 8, 9, 12, 14, 16,},
	{0, 4, 5, 6, 8, 9, 11, 14, 13,},
	{0, 4, 5, 6, 7, 8, 10, 13, 14,},
	{0, 4, 5, 5, 7, 8, 10, 13, 11,},
	{0, 4, 5, 5, 7, 7, 10, 13, 10,},
	{0, 4, 5, 5, 6, 7, 9, 12, 11,},
	{0, 4, 4, 5, 6, 6, 8, 11, 12,},
	{0, 4, 5, 5, 6, 6, 8, 11, 11,},
	{0, 4, 4, 4, 5, 6, 8, 10, 12,},
	{0, 4, 4, 4, 5, 6, 8, 10, 9,},
	{0, 3, 5, 4, 5, 5, 8, 10, 8,},
	{0, 4, 4, 4, 5, 5, 7, 9, 10,},
	{0, 4, 4, 4, 4, 5, 7, 9, 7,},
	{0, 4, 4, 3, 4, 4, 6, 8, 8,},
	{0, 3, 4, 4, 4, 5, 6, 8, 8,},
	{0, 4, 4, 3, 4, 4, 6, 7, 10,},
	{0, 3, 4, 3, 4, 4, 6, 7, 8,},
	{0, 3, 4, 3, 3, 4, 6, 7, 5,},
	{0, 3, 4, 3, 4, 3, 5, 6, 8,},
	{0, 3, 4, 3, 3, 3, 5, 6, 6,},
	{0, 3, 4, 3, 3, 3, 5, 5, 8,},
	{0, 3, 4, 3, 3, 3, 5, 5, 6,},
	{0, 3, 4, 2, 3, 2, 4, 5, 4,},
	{0, 3, 3, 2, 3, 2, 4, 4, 6,},
	{0, 3, 3, 2, 3, 2, 4, 4, 4,},
	{0, 3, 3, 2, 3, 2, 3, 4, 2,},
	{0, 2, 3, 2, 2, 3, 3, 4, 2,},
	{0, 1, 2, 2, 2, 2, 3, 4, 1,},
	{0, 2, 3, 3, 2, 2, 3, 4, 1,},
	{0, 2, 4, 3, 2, 2, 3, 3, 5,},
	{0, 1, 3, 2, 2, 2, 3, 3, 4,},
	{0, 0, 2, 2, 1, 1, 3, 3, 2,},
	{0, 0, 4, 3, 2, 2, 3, 3, 4,},
	{0, -2, 3, 3, 2, 2, 3, 3, 6,},
	{0, -1, 3, 2, 2, 1, 3, 3, 5,},
	{0, 0, 2, 3, 2, 1, 2, 3, 0,},
	{0, 0, 3, 3, 2, 2, 2, 3, 2,},
	{0, 0, 3, 3, 2, 2, 2, 2, 0,},
	{0, 0, 2, 2, 1, 2, 1, 2, 3,},
	{0, 1, 3, 2, 1, 1, 1, 1, 2,},
	{0, 0, 2, 2, 1, 1, 1, 1, 1,},
	{0, -1, 1, 1, 1, 1, 0, 1, 0,},
	{0, -1, 1, 1, 1, 1, 0, 1, 0,},
	{0, -1, 1, 1, 0, 1, 0, 1, 2,},
	{0, 0, 1, 1, 0, 0, 0, 0, 2,},
	{0, 1, 1, 1, 0, 0, 1, 0, 3,},
	{0, 1, 1, 1, 0, 0, 0, 0, 0,},
	{0, 1, 1, 1, 0, 0, 0, 0, 0,},
	{0, 1, 0, 1, 0, -1, 0, 0, 0,},
	{0, 0, 0, 0, 0, -1, -1, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0 ,},
};

static int rgb_offset_revA[][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{-6, 1, -8, -1, 0, -1, -2, 0, -2, -3, 1, -4, -1, 2, -5, 0, 2, -4, -1, 2, -4, 2, 0, -1,},
	{-4, 1, -6, -2, 0, -2, -4, 0, -3, -2, 1, -3, -1, 2, -6, -1, 2, -4, -1, 2, -4, 2, 0, -1,},
	{-3, 1, -5, -2, 0, -1, -3, 0, -3, -2, 1, -3, 0, 2, -5, -2, 1, -4, -1, 2, -5, 0, 0, -2,},
	{-4, 0, -4, -1, 0, -2, -2, 0, -3, -1, 1, -2, -2, 2, -5, -2, 1, -4, -1, 2, -4, 0, 1, -3,},
	{-3, 0, -3, -1, 0, -2, -2, 0, -2, -3, 1, -4, -1, 2, -5, -2, 1, -3, 0, 2, -4, 0, 2, -4,},
	{-3, 0, -3, 0, 0, -1, -3, 0, -2, -2, 1, -3, 0, 2, -4, -2, 2, -4, 0, 2, -5, 0, 2, -4,},
	{-2, 0, -2, -3, 0, -3, 0, 0, 0, -2, 1, -3, -2, 2, -4, -1, 1, -4, 0, 2, -5, 1, 2, -4,},
	{-2, 0, -2, -2, 0, -2, 0, 0, -1, -3, 0, -2, -2, 2, -4, 0, 2, -4, -1, 2, -6, 1, 2, -4,},
	{-2, 0, -2, -2, 0, -2, 0, 0, 0, -2, 1, -3, -1, 2, -4, -1, 1, -4, 0, 2, -6, 1, 2, -4,},
	{-2, 0, -2, -2, 0, -2, 0, 0, 0, -1, 1, -2, -2, 2, -4, 0, 2, -4, 0, 2, -5, 0, 2, -5,},
	{-2, 0, -2, -2, 0, -2, 0, 0, 0, -1, 1, -2, -2, 2, -5, 0, 2, -4, 0, 1, -4, 0, 3, -6,},
	{-2, 0, -2, -1, 0, -1, 0, 0, -1, -1, 1, -2, -2, 2, -4, 0, 1, -4, -1, 2, -6, 0, 2, -6,},
	{-1, 0, -1, -1, 0, -1, -1, 0, -2, -2, 0, -2, -1, 1, -4, 0, 2, -5, 0, 2, -4, 2, 3, -6,},
	{-1, 0, -1, -1, 0, -1, -2, 0, -2, -1, 0, -2, -2, 1, -4, 0, 2, -4, 0, 2, -5, 0, 3, -6,},
	{-1, 0, -1, -1, 0, -1, -2, 0, -2, 0, 0, -1, -3, 1, -4, 0, 2, -5, 0, 2, -5, 1, 2, -6,},
	{-1, 0, -1, -1, 0, -1, -2, 0, -2, 0, 0, -1, -3, 1, -4, 0, 2, -4, 0, 2, -5, 2, 3, -7,},
	{-1, 0, -1, -1, 0, -1, -2, 0, -1, 0, 0, -1, -2, 1, -3, 0, 2, -4, 1, 2, -5, 3, 3, -8,},
	{-1, 0, -1, -1, 0, -1, -2, 0, -1, 0, 0, -1, -2, 1, -3, 0, 1, -4, 0, 2, -6, 3, 3, -8,},
	{-1, 0, -1, -1, 0, -1, -2, 0, -1, 0, 0, -1, -2, 1, -3, 0, 1, -4, 0, 3, -6, 3, 3, -6,},
	{-1, 0, -1, -1, 0, -1, -2, 0, -1, 0, 0, -1, -1, 1, -2, 0, 2, -4, 0, 2, -4, 4, 4, -9,},
	{-1, 0, -1, 0, 0, 0, -2, 0, -1, 0, 0, -1, -1, 1, -2, -1, 1, -4, 0, 2, -6, 3, 3, -8,},
	{-1, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, -1, -2, 1, -3, -1, 1, -4, 0, 2, -6, 3, 3, -7,},
	{-1, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, -1, 1, -3, -1, 1, -4, 0, 2, -6, 3, 4, -9,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -2, 0, -2, -2, 1, -2, 0, 1, -3, 0, 2, -6, 4, 4, -9,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -2, -3, 0, -2, -1, 1, -4, 0, 2, -6, 4, 4, -9,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, -2, 1, -2, 0, 1, -2, 0, 2, -6, 3, 4, -8,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, -2, 0, -2, -1, 1, -3, 0, 2, -5, 5, 5, -11,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, -2, 0, -2, 0, 0, -2, 0, 3, -6, 3, 4, -8,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, -1, 0, -1, 0, 1, -2, 0, 2, -6, 3, 5, -12,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 0, -2, -1, 0, -2, 0, 2, -6, 3, 5, -12,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 0, -2, -1, 1, -2, 0, 2, -6, 3, 4, -10,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, -1, 1, -2, 0, 2, -6, 4, 5, -10,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, -1, -1, 0, -2, 0, 2, -6, 3, 5, -10,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, -1, -1, 0, -2, 0, 3, -7, 5, 4, -10,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, -2, 0, 2, -6, 3, 5, -10,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, -2, 0, 2, -5, 4, 6, -12,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, -5, 4, 5, -12,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 1, 0, 0, -1, 1, 2, -6, 3, 4, -10,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 3, -6, 5, 5, -12,},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 2, -6, 5, 5, -12,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 2, -6, 4, 5, -11,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, -5, 4, 5, -11,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 2, -5, 4, 6, -12,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 0, 2, -5, 4, 5, -11,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, -4, 4, 5, -10,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 1, -4, 3, 5, -11,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 1, -4, 2, 5, -12,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 1, -3, 2, 5, -11,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 0, 0, 1, 0, 1, -3, 3, 5, -10,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -3, 2, 4, -10,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -2, 3, 5, -10,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 0, 1, -2, 2, 4, -10,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -2, 2, 4, -9,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 0, 0, -1, 2, 4, -10,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 0, 0, -1, 2, 4, -9,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 1, 0, 0, -1, 3, 4, -10,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 4, -10,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 3, 4, -9,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	};
#endif
