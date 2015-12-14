/*
 * MELFAS MMS400 Touchscreen
 *
 * Copyright (C) 2014 MELFAS Inc.
 *
 *
 * Model dependent functions
 *
 */

#include "melfas_mms400.h"

#ifdef USE_TSP_TA_CALLBACKS
static bool ta_connected = 0;
#endif

/**
 * Control power supply
 */
int mms_power_control(struct mms_ts_info *info, int enable)
{
	int ret;
	struct i2c_client *client = info->client;

	gpio_direction_output(info->dtdata->gpio_vdd, enable);

	if (!info->dtdata->vreg_vio) {
		info->dtdata->vreg_vio = regulator_get(&client->dev, "vdd-io");
		if (IS_ERR(info->dtdata->vreg_vio)) {
			dev_err(&client->dev, "%s [ERROR] vdd-io get\n", __func__);
			info->dtdata->vreg_vio = NULL;
			goto exit;
		}
	}

	if (enable) {
		ret = regulator_enable(info->dtdata->vreg_vio);
		if (ret)
			dev_err(&client->dev,
				"%s [ERROR] regulator_enable [%d]\n", __func__, ret);
	} else{
		ret = regulator_disable(info->dtdata->vreg_vio);
		if (ret)
			dev_err(&client->dev,
				"%s [ERROR] regulator_disable [%d]\n", __func__, ret);
	}

exit:
	if (!enable)
		usleep_range(10 * 1000, 11 * 1000);
	else
		msleep(50);

	dev_info(&info->client->dev, "%s: %s vdd:%d\n",
			__func__, enable? "on":"off",
			gpio_get_value(info->dtdata->gpio_vdd));
	return 0;
}

/**
 * Clear touch input events
 */
void mms_clear_input(struct mms_ts_info *info)
{
	int i;

	input_report_key(info->input_dev, BTN_TOUCH, 0);
	input_report_key(info->input_dev, BTN_TOOL_FINGER, 0);

	for (i = 0; i< MAX_FINGER_NUM; i++) {
		info->finger_state[i] = 0;
		input_mt_slot(info->input_dev, i);
		input_mt_report_slot_state(info->input_dev, MT_TOOL_FINGER, false);
	}

	info->touch_count = 0;

	input_sync(info->input_dev);

#ifdef TSP_BOOSTER
	if (info->booster && info->booster->dvfs_set)
		info->booster->dvfs_off(info->booster);
#endif
}

/**
 * Input event handler - Report touch input event
 */
void mms_input_event_handler(struct mms_ts_info *info, u8 sz, u8 *buf)
{
	struct i2c_client *client = info->client;
	int i;

	dev_dbg(&client->dev, "%s [START]\n", __func__);
	dev_dbg(&client->dev, "%s - sz[%d] buf[0x%02X]\n", __func__, sz, buf[0]);

	for (i = 1; i < sz; i += info->event_size) {
		u8 *tmp = &buf[i];

		int id = (tmp[0] & MIP_EVENT_INPUT_ID) - 1;
		int x = tmp[2] | ((tmp[1] & 0xf) << 8);
		int y = tmp[3] | (((tmp[1] >> 4) & 0xf) << 8);
		int pressure = tmp[4];
		int size = tmp[5];
		int touch_major = tmp[6];
		int touch_minor = tmp[7];
		int palm = (tmp[0] & MIP_EVENT_INPUT_PALM) >> 4;
		// Report input data
#if MMS_USE_TOUCHKEY
		if ((tmp[0] & MIP_EVENT_INPUT_SCREEN) == 0) {
			//Touchkey Event
			int key = tmp[0] & 0xf;
			int key_state = (tmp[0] & MIP_EVENT_INPUT_PRESS) ? 1 : 0;
			int key_code = 0;

			//Report touchkey event
			switch (key) {
			case 1:
				key_code = KEY_RECENT;
				//dev_dbg(&client->dev, "Key : KEY_MENU\n");
				break;
			case 2:
				key_code = KEY_BACK;
				//dev_dbg(&client->dev, "Key : KEY_BACK\n");
				break;
			default:
				dev_err(&client->dev,
					"%s [ERROR] Unknown key code [%d]\n",
					__func__, key);
				continue;
				break;
			}

			input_report_key(info->input_dev, key_code, key_state);

			dev_err(&client->dev, "%s - Key : ID[%d] Code[%d] State[%d]\n",
				__func__, key, key_code, key_state);
		} else
#endif
		{
			//Report touchscreen event
			if ((tmp[0] & MIP_EVENT_INPUT_PRESS) == 0) {
				//Release
				input_mt_slot(info->input_dev, id);
				input_mt_report_slot_state(info->input_dev,
								MT_TOOL_FINGER, false);
				if (info->finger_state[id] != 0){
					info->touch_count--;
					if (!info->touch_count) {
						input_report_key(info->input_dev, BTN_TOUCH, 0);
						input_report_key(info->input_dev,
									BTN_TOOL_FINGER, 0);
					}
					info->finger_state[id] = 0;
					dev_err(&client->dev,
						"R[%d] V[%02x%02x%02x] tc:%d\n",
						id, info->boot_ver_ic, info->core_ver_ic,
						info->config_ver_ic, info->touch_count);
				}

				continue;
			}

			//Press or Move
			input_mt_slot(info->input_dev, id);
			input_mt_report_slot_state(info->input_dev, MT_TOOL_FINGER, true);
			input_report_key(info->input_dev, BTN_TOUCH, 1);
			input_report_key(info->input_dev, BTN_TOOL_FINGER, 1);
			input_report_abs(info->input_dev, ABS_MT_POSITION_X, x);
			input_report_abs(info->input_dev, ABS_MT_POSITION_Y, y);
			//input_report_abs(info->input_dev, ABS_MT_PRESSURE, pressure);
			input_report_abs(info->input_dev, ABS_MT_TOUCH_MAJOR, touch_major);
			input_report_abs(info->input_dev, ABS_MT_TOUCH_MINOR, touch_minor);
			input_report_abs(info->input_dev,ABS_MT_SUMSIZE, size);
			input_report_abs(info->input_dev,ABS_MT_PALM, palm);

			if (info->finger_state[id] == 0){
				info->finger_state[id] = 1;
				info->touch_count++;
				dev_err(&client->dev,
					"P[%d] (%d, %d) p:%d m:%d tc:%d\n",
					id, x, y, pressure, touch_major, info->touch_count);
			}
		}
	}

	input_sync(info->input_dev);

#ifdef TSP_BOOSTER
	if (info->booster && info->booster->dvfs_set)
		info->booster->dvfs_set(info->booster, info->touch_count);
#endif


	dev_dbg(&client->dev, "%s [DONE]\n", __func__);
	return;
}

int mms_pinctrl_configure(struct mms_ts_info *info, int active)
{
	struct pinctrl_state *set_state_i2c;
	int retval;

	dev_dbg(&info->client->dev, "%s: %d\n", __func__, active);

	if(active){
		set_state_i2c =	pinctrl_lookup_state(info->pinctrl, "tsp_gpio_active");
		if (IS_ERR(set_state_i2c)) {
			dev_err(&info->client->dev,
				"%s: cannot get pinctrl(i2c) active state\n", __func__);
			return PTR_ERR(set_state_i2c);
		}
	} else {
		set_state_i2c =	pinctrl_lookup_state(info->pinctrl, "tsp_gpio_suspend");
		if (IS_ERR(set_state_i2c)) {
			dev_err(&info->client->dev,
				"%s: cannot get pinctrl(i2c) suspend state\n", __func__);
			return PTR_ERR(set_state_i2c);
		}
	}
	retval = pinctrl_select_state(info->pinctrl, set_state_i2c);
	if (retval) {
		dev_err(&info->client->dev,
			"%s: cannot set pinctrl(i2c) %d state\n", __func__, active);
		return retval;
	}

	return 0;
}


#if MMS_USE_DEVICETREE
/**
 * Parse device tree
 */
int mms_parse_devicetree(struct device *dev, struct mms_ts_info *info)
{
	struct device_node *np = dev->of_node;
	int ret;

	dev_dbg(dev, "%s [START]\n", __func__);

	ret = of_property_read_u32(np, "melfas,max_x", &info->dtdata->max_x);
	if (ret) {
		dev_err(dev, "%s [ERROR] max_x\n", __func__);
		info->dtdata->max_x = 1080;
	}

	ret = of_property_read_u32(np, "melfas,max_y", &info->dtdata->max_y);
	if (ret) {
		dev_err(dev, "%s [ERROR] max_y\n", __func__);
		info->dtdata->max_y = 1920;
	}

	info->dtdata->gpio_intr = of_get_named_gpio(np, "melfas,irq-gpio", 0);
	gpio_request(info->dtdata->gpio_intr, "irq-gpio");
	gpio_direction_input(info->dtdata->gpio_intr);
	info->client->irq = gpio_to_irq(info->dtdata->gpio_intr);

	info->dtdata->gpio_scl = of_get_named_gpio(np, "melfas,scl-gpio", 0);
	gpio_request(info->dtdata->gpio_scl, "melfas_scl_gpio");
	info->dtdata->gpio_sda = of_get_named_gpio(np, "melfas,sda-gpio", 0);
	gpio_request(info->dtdata->gpio_sda, "melfas_sda_gpio");

	info->dtdata->gpio_vdd = of_get_named_gpio(np, "melfas,vdd_en", 0);
	gpio_request(info->dtdata->gpio_vdd, "melfas_vdd_gpio");

	/* gpio_vio is not used actually. It's for Test. Real 1.8 IO EN is vreg */
	//info->dtdata->gpio_vio = of_get_named_gpio(np, "melfas,io_en", 0);
	//gpio_request(info->dtdata->gpio_vio, "melfas_io_gpio");
	//gpio_direction_output(info->dtdata->gpio_vio, 0);

	info->dtdata->panel = 0;

	ret = of_property_read_string(np, "melfas,fw_name", &info->dtdata->fw_name);
	if (ret < 0) {
		dev_err(dev, "%s: failed to get built-in firmware path!\n", __func__);
		info->dtdata->fw_name = INTERNAL_FW_PATH;
	}
	dev_info(dev, "%s: max_x:%d max_y:%d int:%d irq:%d sda:%d scl:%d"
		" vdd:%d gpio_vio:%d panel:%d fw_name:%s\n",
		__func__, info->dtdata->max_x, info->dtdata->max_y,
		info->dtdata->gpio_intr, info->client->irq, info->dtdata->gpio_sda,
		info->dtdata->gpio_scl, info->dtdata->gpio_vdd,
		info->dtdata->gpio_vio, info->dtdata->panel, info->dtdata->fw_name);

	return 0;
}
#endif

/**
 * Config input interface
 */
void mms_config_input(struct mms_ts_info *info)
{
	struct input_dev *input_dev = info->input_dev;

	dev_dbg(&info->client->dev, "%s [START]\n", __func__);

	//Screen
	set_bit(EV_SYN, input_dev->evbit);
	set_bit(EV_ABS, input_dev->evbit);

	set_bit(BTN_TOUCH, input_dev->keybit);
	set_bit(BTN_TOOL_FINGER, input_dev->keybit);
	set_bit(INPUT_PROP_DIRECT, input_dev->propbit);

	input_mt_init_slots(input_dev, MAX_FINGER_NUM, INPUT_MT_DIRECT);

	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, info->max_x, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, info->max_y, 0, 0);
	//input_set_abs_params(input_dev, ABS_MT_PRESSURE, 0, INPUT_PRESSURE_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, INPUT_TOUCH_MAJOR_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MINOR, 0, INPUT_TOUCH_MINOR_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_PALM,0, INPUT_PALM_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_SUMSIZE,0, INPUT_SUMSIZE_MAX, 0, 0);


	//Key
	set_bit(EV_KEY, input_dev->evbit);
#if MMS_USE_TOUCHKEY
	set_bit(KEY_BACK, input_dev->keybit);
	set_bit(KEY_RECENT, input_dev->keybit);
#endif
#if MMS_USE_NAP_MODE
	set_bit(EV_KEY, input_dev->evbit);
	set_bit(KEY_POWER, input_dev->keybit);
#endif
	dev_dbg(&info->client->dev, "%s [DONE]\n", __func__);
	return;
}

/**
 * Callback - get charger status
 */
#ifdef USE_TSP_TA_CALLBACKS
void mms_charger_status_cb(struct tsp_callbacks *cb, int status)
{
	pr_err("%s: TA %s\n",
		__func__, status ? "connected" : "disconnected");

	if (status)
		ta_connected = true;
	else
		ta_connected = false;

	/* not yet defined functions */
}

void mms_register_callback(struct tsp_callbacks *cb)
{
	charger_callbacks = cb;
	pr_info("%s\n", __func__);
}
#endif
