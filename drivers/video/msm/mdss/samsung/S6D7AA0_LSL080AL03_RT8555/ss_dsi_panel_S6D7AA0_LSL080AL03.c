/*
 * =================================================================
 *
 *
 *	Description:  samsung display panel file
 *
 *	Author: prabhu.manoj
 *	Company:  Samsung Electronics
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

#include "ss_dsi_panel_S6D7AA0_LSL080AL03.h"
#include "ss_dsi_mdnie_S6d77A1A01_LSL080AL03.h"
#include "../ss_dsi_panel_common.h"
#define S6D7AA0_LSL080AL03_SUPPORT_PANEL_COUNT 1
#define MAX_BRIGHTNESS_LEVEL 255
#define MID_BRIGHTNESS_LEVEL 143
#define LOW_BRIGHTNESS_LEVEL 10
#define DIM_BRIGHTNESS_LEVEL 20

#define BL_MIN_BRIGHTNESS			1
#define BL_MAX_BRIGHTNESS_LEVEL		126
#define BL_MID_BRIGHTNESS_LEVEL		59
#define BL_LOW_BRIGHTNESS_LEVEL		2
#define BL_DIM_BRIGHTNESS_LEVEL		7
#define BL_DEFAULT_BRIGHTNESS		BL_MID_BRIGHTNESS_LEVEL

#define MMSS_GP3_BASE 0x0180A000
#define MMSS_GP3_SIZE  0x28

static void mdss_dsi_panel_bklt_pwm( int level)
{

	if(virt_mmss_gp_base == NULL) {
		pr_err("%s:############ I/O remap failed \n", __func__);
		return ;
	}
	pr_info("%s : %d\n", __func__, level);

	/* Put the MND counter in reset mode for programming */
	HWIO_OUTM(GP0_CFG_RCGR, HWIO_GP_SRC_SEL_VAL_BMSK,     1 << HWIO_GP_SRC_SEL_VAL1_SHFT); //SRC_SEL = 001(GPLL0_MAIN.)
	HWIO_OUTM(GP0_CFG_RCGR, HWIO_GP_SRC_DIV_VAL_BMSK,    31 << HWIO_GP_SRC_DIV_VAL_SHFT); //SRC_DIV = 11111 (Div 16)
	HWIO_OUTM(GP0_CFG_RCGR, HWIO_GP_MODE_VAL_BMSK,        2 << HWIO_GP_MODE_VAL_SHFT);    //Mode Select 10

	//M value
	HWIO_OUTM(GP_M_REG, HWIO_GP_MD_REG_M_VAL_BMSK,	GP_CLK_M_DEFAULT << HWIO_GP_MD_REG_M_VAL_SHFT);

	// D value
	HWIO_OUTM(GP_D_REG, HWIO_GP_MD_REG_D_VAL_BMSK,	 (~((int16_t)level << 1)) << HWIO_GP_MD_REG_D_VAL_SHFT);

	//N value
	HWIO_OUTM(GP_NS_REG, HWIO_GP_NS_REG_GP_N_VAL_BMSK, ~(GP_CLK_N_DEFAULT - GP_CLK_M_DEFAULT) << 0);

	HWIO_OUTM(GP0_CMD_RCGR,HWIO_UPDATE_VAL_BMSK,		1 << HWIO_UPDATE_VAL_SHFT);     //UPDATE ACTIVE
	HWIO_OUTM(GP0_CMD_RCGR,HWIO_ROOT_EN_VAL_BMSK,		1 << HWIO_ROOT_EN_VAL_SHFT);    //ROOT_EN
	HWIO_OUTM(CAMSS_GP0_CBCR, HWIO_CLK_ENABLE_VAL_BMSK,	1 << HWIO_CLK_ENABLE_VAL_SHFT); //CLK_ENABLE
}

static void mdss_dsi_panel_bklt_pwm_clk(int enable) {
	pr_info("%s %d\n", __func__, enable);
	HWIO_OUTM(CAMSS_GP0_CBCR, HWIO_CLK_ENABLE_VAL_BMSK,	enable << HWIO_CLK_ENABLE_VAL_SHFT); //CLK_DISABLE
}

static int mdss_panel_on_pre(struct mdss_dsi_ctrl_pdata *ctrl)
{
	struct samsung_display_driver_data *vdd = check_valid_ctrl(ctrl);

	if (IS_ERR_OR_NULL(vdd)) {
		pr_err("%s: Invalid data ctrl : 0x%zx vdd : 0x%zx", __func__, (size_t)ctrl, (size_t)vdd);
		return false;
	}

	pr_info("%s %d\n", __func__, ctrl->ndx);

	mdss_panel_attach_set(ctrl, true);

	return true;
}
static void backlight_tft_late_on(struct mdss_dsi_ctrl_pdata *ctrl)
{
	pr_info("%s : Backlight is on\n", __func__);
	pwm_backlight_control(1);
}
static int mdss_panel_off_pre(struct mdss_dsi_ctrl_pdata *ctrl)
{
	struct samsung_display_driver_data *vdd = check_valid_ctrl(ctrl);

	if (IS_ERR_OR_NULL(vdd)) {
		pr_err("%s: Invalid data ctrl : 0x%zx vdd : 0x%zx", __func__, (size_t)ctrl, (size_t)vdd);
		return false;
	}

	pr_info("%s %d\n", __func__, ctrl->ndx);

	pwm_backlight_control(0);
	mdss_dsi_panel_bklt_pwm_clk(0);

	return true;
}

static int mdss_panel_revision(struct mdss_dsi_ctrl_pdata *ctrl)
{
	struct samsung_display_driver_data *vdd = check_valid_ctrl(ctrl);

	if (IS_ERR_OR_NULL(vdd)) {
		pr_err("%s: Invalid data ctrl : 0x%zx vdd : 0x%zx", __func__, (size_t)ctrl, (size_t)vdd);
		return false;
	}

	vdd->panel_revision = 0;

	return true;
}
static unsigned char mdss_dsi_panel_pwm_scaling(int level)
{
	unsigned char scaled_level;

	if (level >= MAX_BRIGHTNESS_LEVEL)
		scaled_level  = BL_MAX_BRIGHTNESS_LEVEL;
	else if (level >= MID_BRIGHTNESS_LEVEL) {
		scaled_level  = (level - MID_BRIGHTNESS_LEVEL) *
		(BL_MAX_BRIGHTNESS_LEVEL - BL_MID_BRIGHTNESS_LEVEL) / (MAX_BRIGHTNESS_LEVEL-MID_BRIGHTNESS_LEVEL) + BL_MID_BRIGHTNESS_LEVEL;
	} else if (level >= DIM_BRIGHTNESS_LEVEL) {
		scaled_level  = (level - DIM_BRIGHTNESS_LEVEL) *
		(BL_MID_BRIGHTNESS_LEVEL - BL_DIM_BRIGHTNESS_LEVEL) / (MID_BRIGHTNESS_LEVEL-DIM_BRIGHTNESS_LEVEL) + BL_DIM_BRIGHTNESS_LEVEL;
	} else if (level >= LOW_BRIGHTNESS_LEVEL) {
		scaled_level  = (level - LOW_BRIGHTNESS_LEVEL) *
		(BL_DIM_BRIGHTNESS_LEVEL - BL_LOW_BRIGHTNESS_LEVEL) / (DIM_BRIGHTNESS_LEVEL-LOW_BRIGHTNESS_LEVEL) + BL_LOW_BRIGHTNESS_LEVEL;
	}  else
		scaled_level  = BL_MIN_BRIGHTNESS;

	pr_info("%s  level = [%d]: scaled_level = [%d] \n",__func__,level,scaled_level);

	return scaled_level;
}

static struct dsi_panel_cmds * mdss_brightness_tft_pwm_ldi(struct mdss_dsi_ctrl_pdata *ctrl, int *level_key)
{
	struct samsung_display_driver_data *vdd = check_valid_ctrl(ctrl);
	if (IS_ERR_OR_NULL(vdd)) {
		pr_err("%s: Invalid data ctrl : 0x%zx vdd : 0x%zx", __func__, (size_t)ctrl, (size_t)vdd);
		return NULL;
	}
	vdd->scaled_level = get_scaled_level(vdd, ctrl->ndx);
	pr_info("%s bl_level : %d scaled_level : %d\n", __func__, vdd->bl_level, vdd->scaled_level);
	vdd->dtsi_data[ctrl->ndx].tft_pwm_tx_cmds->cmds->payload[1] = vdd->scaled_level;
	return &vdd->dtsi_data[ctrl->ndx].tft_pwm_tx_cmds[vdd->panel_revision];
}

static void mdss_brightness_tft_pwm_gpio(struct mdss_dsi_ctrl_pdata *ctrl, int bl_lvl)
{
	struct samsung_display_driver_data *vdd = check_valid_ctrl(ctrl);

	if (IS_ERR_OR_NULL(vdd)) {
		pr_err("%s: Invalid data ctrl : 0x%zx vdd : 0x%zx", __func__, (size_t)ctrl, (size_t)vdd);
		return;
	}
	vdd->scaled_level = mdss_dsi_panel_pwm_scaling(bl_lvl);
	pr_info("%s bl_level : %d scaled_level : %d\n", __func__, bl_lvl, vdd->scaled_level);
	vdd->bl_level = bl_lvl;
	if (!get_lcd_attached("GET"))
		return;
	mdss_dsi_panel_bklt_pwm(vdd->scaled_level);
	return;
}
static void mdss_panel_blic_init(struct mdss_dsi_ctrl_pdata *ctrl)
{
	struct samsung_display_driver_data *vdd = check_valid_ctrl(ctrl);
	struct mdss_panel_info *pinfo = NULL;

	if (IS_ERR_OR_NULL(vdd)) {
		pr_err("%s: Invalid data ctrl : 0x%zx vdd : 0x%zx", __func__, (size_t)ctrl, (size_t)vdd);
		return;
	}
	pinfo = &(ctrl->panel_data.panel_info);

	if (ctrl->bklt_ctrl == BL_GPIO_SWING) { /* for Rev 00 */
		pwm_backlight_control(1);
		mdss_brightness_tft_pwm_gpio(ctrl, DEFAULT_BRIGHTNESS);
	} else
		pinfo->blank_state = MDSS_PANEL_BLANK_UNBLANK;


	vdd->manufacture_id_dsi[ctrl->ndx] = get_lcd_attached("GET");
	pr_info("%s: DSI%d manufacture_id=0x%x\n", __func__, ctrl->ndx, vdd->manufacture_id_dsi[ctrl->ndx]);
}
static void dsi_update_mdnie_data(void)
{
	/* Update mdnie command */
	mdnie_data.DSI0_COLOR_BLIND_MDNIE_2 = DSI0_COLOR_BLIND_MDNIE_2;
	mdnie_data.DSI0_RGB_SENSOR_MDNIE_1 = NULL;
	mdnie_data.DSI0_RGB_SENSOR_MDNIE_2 = NULL;
	mdnie_data.DSI0_UI_DYNAMIC_MDNIE_2 = DSI0_UI_MDNIE_2;
	mdnie_data.DSI0_UI_STANDARD_MDNIE_2 = DSI0_UI_MDNIE_2;
	mdnie_data.DSI0_UI_AUTO_MDNIE_2 = DSI0_UI_MDNIE_2;
	mdnie_data.DSI0_VIDEO_DYNAMIC_MDNIE_2 = DSI0_VIDEO_MDNIE_2;
	mdnie_data.DSI0_VIDEO_STANDARD_MDNIE_2 = DSI0_VIDEO_MDNIE_2;
	mdnie_data.DSI0_VIDEO_AUTO_MDNIE_2 = DSI0_VIDEO_MDNIE_2;
	mdnie_data.DSI0_CAMERA_MDNIE_2 = DSI0_CAMERA_MDNIE_2;
	mdnie_data.DSI0_CAMERA_AUTO_MDNIE_2 = DSI0_CAMERA_MDNIE_2;
	mdnie_data.DSI0_GALLERY_DYNAMIC_MDNIE_2 = DSI0_GALLERY_MDNIE_2;
	mdnie_data.DSI0_GALLERY_STANDARD_MDNIE_2 = DSI0_GALLERY_MDNIE_2;
	mdnie_data.DSI0_GALLERY_AUTO_MDNIE_2 = DSI0_GALLERY_MDNIE_2;
	mdnie_data.DSI0_VT_DYNAMIC_MDNIE_2 = NULL;
	mdnie_data.DSI0_VT_STANDARD_MDNIE_2 = NULL;
	mdnie_data.DSI0_VT_AUTO_MDNIE_2 = NULL;
	mdnie_data.DSI0_BROWSER_DYNAMIC_MDNIE_2 = DSI0_BROWSER_MDNIE_2;
	mdnie_data.DSI0_BROWSER_STANDARD_MDNIE_2 = DSI0_BROWSER_MDNIE_2;
	mdnie_data.DSI0_BROWSER_AUTO_MDNIE_2 = DSI0_BROWSER_MDNIE_2;
	mdnie_data.DSI0_EBOOK_DYNAMIC_MDNIE_2 = DSI0_EBOOK_MDNIE_2;
	mdnie_data.DSI0_EBOOK_STANDARD_MDNIE_2 = DSI0_EBOOK_MDNIE_2;
	mdnie_data.DSI0_EBOOK_AUTO_MDNIE_2 = DSI0_EBOOK_MDNIE_2;

	mdnie_data.DSI0_BYPASS_MDNIE = DSI0_BYPASS_MDNIE;
	mdnie_data.DSI0_NEGATIVE_MDNIE = DSI0_NEGATIVE_MDNIE;
	mdnie_data.DSI0_COLOR_BLIND_MDNIE = DSI0_COLOR_BLIND_MDNIE;
	mdnie_data.DSI0_HBM_CE_MDNIE = NULL;
	mdnie_data.DSI0_RGB_SENSOR_MDNIE = NULL;
	mdnie_data.DSI0_CURTAIN = NULL;
	mdnie_data.DSI0_UI_DYNAMIC_MDNIE = DSI0_UI_MDNIE;
	mdnie_data.DSI0_UI_STANDARD_MDNIE = DSI0_UI_MDNIE;
	mdnie_data.DSI0_UI_NATURAL_MDNIE = DSI0_UI_MDNIE;
	mdnie_data.DSI0_UI_MOVIE_MDNIE = DSI0_UI_MDNIE;
	mdnie_data.DSI0_UI_AUTO_MDNIE = DSI0_UI_MDNIE;
	mdnie_data.DSI0_VIDEO_OUTDOOR_MDNIE = NULL;
	mdnie_data.DSI0_VIDEO_DYNAMIC_MDNIE = DSI0_VIDEO_MDNIE;
	mdnie_data.DSI0_VIDEO_STANDARD_MDNIE = DSI0_VIDEO_MDNIE;
	mdnie_data.DSI0_VIDEO_NATURAL_MDNIE = DSI0_VIDEO_MDNIE;
	mdnie_data.DSI0_VIDEO_MOVIE_MDNIE = DSI0_VIDEO_MDNIE;
	mdnie_data.DSI0_VIDEO_AUTO_MDNIE = DSI0_VIDEO_MDNIE;
	mdnie_data.DSI0_VIDEO_WARM_OUTDOOR_MDNIE = NULL;
	mdnie_data.DSI0_VIDEO_WARM_MDNIE = NULL;
	mdnie_data.DSI0_VIDEO_COLD_OUTDOOR_MDNIE = NULL;
	mdnie_data.DSI0_VIDEO_COLD_MDNIE = NULL;
	mdnie_data.DSI0_CAMERA_OUTDOOR_MDNIE = NULL;
	mdnie_data.DSI0_CAMERA_MDNIE = DSI0_CAMERA_MDNIE;
	mdnie_data.DSI0_CAMERA_AUTO_MDNIE = DSI0_CAMERA_MDNIE;
	mdnie_data.DSI0_GALLERY_DYNAMIC_MDNIE = DSI0_GALLERY_MDNIE;
	mdnie_data.DSI0_GALLERY_STANDARD_MDNIE = DSI0_GALLERY_MDNIE;
	mdnie_data.DSI0_GALLERY_NATURAL_MDNIE = DSI0_GALLERY_MDNIE;
	mdnie_data.DSI0_GALLERY_MOVIE_MDNIE = DSI0_GALLERY_MDNIE;
	mdnie_data.DSI0_GALLERY_AUTO_MDNIE = DSI0_GALLERY_MDNIE;
	mdnie_data.DSI0_VT_DYNAMIC_MDNIE = NULL;
	mdnie_data.DSI0_VT_STANDARD_MDNIE = NULL;
	mdnie_data.DSI0_VT_NATURAL_MDNIE = NULL;
	mdnie_data.DSI0_VT_MOVIE_MDNIE = NULL;
	mdnie_data.DSI0_VT_AUTO_MDNIE = NULL;
	mdnie_data.DSI0_BROWSER_DYNAMIC_MDNIE = DSI0_BROWSER_MDNIE;
	mdnie_data.DSI0_BROWSER_STANDARD_MDNIE = DSI0_BROWSER_MDNIE;
	mdnie_data.DSI0_BROWSER_NATURAL_MDNIE = DSI0_BROWSER_MDNIE;
	mdnie_data.DSI0_BROWSER_MOVIE_MDNIE = DSI0_BROWSER_MDNIE;
	mdnie_data.DSI0_BROWSER_AUTO_MDNIE = DSI0_BROWSER_MDNIE;
	mdnie_data.DSI0_EBOOK_DYNAMIC_MDNIE = DSI0_EBOOK_MDNIE;
	mdnie_data.DSI0_EBOOK_STANDARD_MDNIE = DSI0_EBOOK_MDNIE;
	mdnie_data.DSI0_EBOOK_NATURAL_MDNIE = DSI0_EBOOK_MDNIE;
	mdnie_data.DSI0_EBOOK_MOVIE_MDNIE = DSI0_EBOOK_MDNIE;
	mdnie_data.DSI0_EBOOK_AUTO_MDNIE = DSI0_EBOOK_MDNIE;
	mdnie_data.DSI0_EMAIL_AUTO_MDNIE = DSI0_EMAIL_MDNIE;

	mdnie_data.mdnie_tune_value_dsi0 = mdnie_tune_value_dsi0;

	/* Update MDNIE data related with size, offset or index */
	mdnie_data.dsi0_bypass_mdnie_size = ARRAY_SIZE(DSI0_BYPASS_MDNIE);
	mdnie_data.mdnie_color_blinde_cmd_offset = MDNIE_COLOR_BLINDE_CMD_OFFSET;
	mdnie_data.mdnie_step_index[MDNIE_STEP1] = MDNIE_STEP1_INDEX;
	mdnie_data.mdnie_step_index[MDNIE_STEP2] = MDNIE_STEP2_INDEX;
	mdnie_data.address_scr_white[ADDRESS_SCR_WHITE_RED_OFFSET] = 0;
	mdnie_data.address_scr_white[ADDRESS_SCR_WHITE_GREEN_OFFSET] = 0;
	mdnie_data.address_scr_white[ADDRESS_SCR_WHITE_BLUE_OFFSET] = 0;
	mdnie_data.dsi0_rgb_sensor_mdnie_1_size = 0;
	mdnie_data.dsi0_rgb_sensor_mdnie_2_size = 0;
}

static void mdss_panel_init(struct samsung_display_driver_data *vdd)
{
	pr_info("%s : %s", __func__, vdd->panel_name);

	virt_mmss_gp_base = ioremap(MMSS_GP3_BASE, MMSS_GP3_SIZE);

	vdd->support_panel_max = S6D7AA0_LSL080AL03_SUPPORT_PANEL_COUNT;
	vdd->support_mdnie_lite = true;
	vdd->support_cabc = true;

	/* ON/OFF */
	vdd->panel_func.samsung_panel_on_pre = mdss_panel_on_pre;
	vdd->panel_func.samsung_panel_on_post = NULL;
	vdd->panel_func.samsung_panel_off_pre = mdss_panel_off_pre;
	vdd->panel_func.samsung_panel_off_post = NULL;
	vdd->panel_func.samsung_backlight_late_on = backlight_tft_late_on;

	/* DDI RX */
	vdd->panel_func.samsung_panel_revision = mdss_panel_revision;
	vdd->panel_func.samsung_manufacture_date_read = NULL;
	vdd->panel_func.samsung_ddi_id_read = NULL;
	vdd->panel_func.samsung_hbm_read = NULL;
	vdd->panel_func.samsung_mdnie_read = NULL;
	vdd->panel_func.samsung_smart_dimming_init = NULL;

	/* Brightness */
	vdd->panel_func.samsung_brightness_hbm_off = NULL;
	vdd->panel_func.samsung_brightness_aid = NULL;
	vdd->panel_func.samsung_brightness_acl_on = NULL;
	vdd->panel_func.samsung_brightness_acl_percent = NULL;
	vdd->panel_func.samsung_brightness_acl_off = NULL;
	vdd->panel_func.samsung_brightness_elvss = NULL;
	vdd->panel_func.samsung_brightness_elvss_temperature1 = NULL;
	vdd->panel_func.samsung_brightness_elvss_temperature2 = NULL;
	vdd->panel_func.samsung_brightness_vint = NULL;
	vdd->panel_func.samsung_brightness_gamma = NULL;
	vdd->brightness[0].brightness_packet_tx_cmds_dsi.link_state = DSI_HS_MODE;

	vdd->panel_func.samsung_tft_blic_init = mdss_panel_blic_init;
	vdd->panel_func.samsung_brightness_tft_pwm_gpio = mdss_brightness_tft_pwm_gpio;
	vdd->panel_func.samsung_brightness_tft_pwm_ldi = mdss_brightness_tft_pwm_ldi;

	dsi_update_mdnie_data();
	mdss_panel_attach_set(vdd->ctrl_dsi[DISPLAY_1], true);
}

static int __init samsung_panel_init(void)
{
	struct samsung_display_driver_data *vdd = samsung_get_vdd();
	char panel_string[] = "ss_dsi_panel_S6D7AA0_LSL080AL03_WXGA";

	vdd->panel_name = mdss_mdp_panel + 8;
	pr_info("%s : %s\n", __func__, vdd->panel_name);

	if (!strncmp(vdd->panel_name, panel_string, strlen(panel_string)))
		vdd->panel_func.samsung_panel_init = mdss_panel_init;
	else
		vdd->panel_func.samsung_panel_init = NULL;
	return 0;
}
early_initcall(samsung_panel_init);
