/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/hrtimer.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/workqueue.h>
#include <linux/of_gpio.h>
#include <linux/pinctrl/consumer.h>
#include "../staging/android/timed_output.h"

/* default timeout */
#define VIB_DEFAULT_TIMEOUT 10000

#if defined(CONFIG_DC_VIB_PM_LDO)
static struct regulator *motor_ldo;
static int motor_ldo_onoff;

static int motor_onoff(bool onoff)
{
	int ret = 0;
	if (onoff) {
		ret = regulator_enable(motor_ldo);
		if (ret) {
			pr_err("%s regulaor enable failed\n",__func__);
			goto regulator_fail;
		}
		motor_ldo_onoff = 1;
		printk(KERN_CRIT "%s Regulator On!\n",__func__);
	} else if(motor_ldo_onoff == 1) {
		ret = regulator_disable(motor_ldo);
		if (ret) {
			pr_err("%s regulaor disable failed\n",__func__);
			goto regulator_fail;
		}
		motor_ldo_onoff = 0;
		printk(KERN_CRIT "%s Regulator OFF!\n",__func__);
	}

	return ret;
regulator_fail:
	regulator_put(motor_ldo);
	return ret;
}
#endif

struct msm_vib {
	struct hrtimer vib_timer;
	struct timed_output_dev timed_dev;
	struct work_struct work;
	struct workqueue_struct *queue;

	int state;
	int timeout;
	int motor_en;
	struct mutex lock;
};

static void set_vibrator(int motor_en, int on)
{
	pr_info("[VIB] %s, value[%d]\n",__func__, on);
#if defined(CONFIG_DC_VIB_PM_LDO)
	motor_onoff(on);
#else
	gpio_direction_output(motor_en, on);
#endif
}

static void vibrator_enable(struct timed_output_dev *dev, int value)
{
	struct msm_vib *vib = container_of(dev, struct msm_vib,
					 timed_dev);

	mutex_lock(&vib->lock);
	hrtimer_cancel(&vib->vib_timer);

	if (value == 0) {
		pr_info("[VIB] OFF\n");
		vib->state = 0;
	}
	else {
		pr_info("[VIB] ON, Duration : %d msec\n" , value);
		vib->state = 1;

		if (value == 0x7fffffff){
			pr_info("[VIB] No Use Timer %d \n", value);
		}
		else	{
			value = (value > vib->timeout ?	vib->timeout : value);
			hrtimer_start(&vib->vib_timer,ktime_set(value / 1000, (value % 1000) * 1000000),HRTIMER_MODE_REL);
                }
	}
	mutex_unlock(&vib->lock);
	queue_work(vib->queue, &vib->work);
}

static void msm_vibrator_update(struct work_struct *work)
{
	struct msm_vib *vib = container_of(work, struct msm_vib,
					 work);

	set_vibrator(vib->motor_en, vib->state);
}

static int vibrator_get_time(struct timed_output_dev *dev)
{
	struct msm_vib *vib = container_of(dev, struct msm_vib, timed_dev);

	if (hrtimer_active(&vib->vib_timer)) {
		ktime_t r = hrtimer_get_remaining(&vib->vib_timer);
		return (int)ktime_to_us(r);
	}
	else
		return 0;
}

static enum hrtimer_restart vibrator_timer_func(struct hrtimer *timer)
{
	struct msm_vib *vib = container_of(timer, struct msm_vib,
							 vib_timer);

	vib->state = 0;
	queue_work(vib->queue, &vib->work);

	return HRTIMER_NORESTART;
}

#ifdef CONFIG_PM
static int msm_vibrator_suspend(struct device *dev)
{
	struct msm_vib *vib = dev_get_drvdata(dev);

	pr_info("[VIB] %s\n",__func__);

	hrtimer_cancel(&vib->vib_timer);
	cancel_work_sync(&vib->work);
	/* turn-off vibrator */
	set_vibrator(vib->motor_en, 0);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(vibrator_pm_ops, msm_vibrator_suspend, NULL);

static int msm_vibrator_probe(struct platform_device *pdev)
{
	struct msm_vib *vib;
#if !defined(CONFIG_DC_VIB_PM_LDO)
	struct device *dev = &pdev->dev;
	struct pinctrl *vib_pinctrl;
#else
	const char *supply_name;
#endif
	int rc = 0;

	pr_info("[VIB] %s\n",__func__);

	vib = devm_kzalloc(&pdev->dev, sizeof(*vib), GFP_KERNEL);
	if (!vib)	{
		pr_err("%s : Failed to allocate memory\n", __func__);
		return -ENOMEM;
	}

#if defined(CONFIG_DC_VIB_PM_LDO)
	vib->motor_en = 0;
	rc = of_property_read_string(pdev->dev.of_node, "vibrator,motor_en_supply", &supply_name);
	if (rc < 0) {
		pr_err("[%s]: Unable to read ldo source name, exit now\n", __func__);
		goto ldo_absent;
	}
	pr_info("[%s] supply-ldo: pm%s\n", __func__, supply_name);

	motor_ldo = regulator_get(NULL, supply_name);
	if (IS_ERR(motor_ldo)) {
		pr_err("%s: could not get %s, rc = %ld\n", __func__, supply_name, PTR_ERR(motor_ldo));
		goto ldo_absent;
	}

	rc = regulator_set_voltage(motor_ldo,3300000,3300000);
	if (rc) {
		pr_err("%s regulator set volatge failed\n",__func__);
		regulator_put(motor_ldo);
		goto ldo_absent;
	}

#else
	vib->motor_en = of_get_named_gpio(pdev->dev.of_node, "samsung,motor-en", 0);
	pr_info("[VIB] %s motor_en %d\n",__func__, vib->motor_en);

	if (!gpio_is_valid(vib->motor_en)) {
		pr_err("%s:%d, reset gpio not specified\n",
				__func__, __LINE__);
		return -EINVAL;
	}
	if (gpio_request(vib->motor_en, "motor_en")) {
		pr_err("%s:%d, request not specified\n",
				__func__, __LINE__);
		return -EINVAL;
	}

	/* Get pinctrl if target uses pinctrl */
	vib_pinctrl = devm_pinctrl_get_select(dev, "tlmm_motor_en_active");
	if (IS_ERR(vib_pinctrl)) {
		if (PTR_ERR(vib_pinctrl) == -EPROBE_DEFER)
			return -EPROBE_DEFER;

		pr_debug("Target does not use pinctrl\n");
		vib_pinctrl = NULL;
	}
#endif
	vib->timeout = VIB_DEFAULT_TIMEOUT;

	INIT_WORK(&vib->work, msm_vibrator_update);
	mutex_init(&vib->lock);

	vib->queue = create_singlethread_workqueue("msm_vibrator");
	if (!vib->queue) {
		pr_err("%s(): can't create workqueue\n", __func__);
		return -ENOMEM;
	}

	hrtimer_init(&vib->vib_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	vib->vib_timer.function = vibrator_timer_func;

	vib->timed_dev.name = "vibrator";
	vib->timed_dev.get_time = vibrator_get_time;
	vib->timed_dev.enable = vibrator_enable;

	dev_set_drvdata(&pdev->dev, vib);

	rc = timed_output_dev_register(&vib->timed_dev);
	if (rc < 0) {
		pr_err("[VIB] timed_output_dev_register fail (rc=%d)\n", rc);
		goto err_read_vib;
	}
	return 0;
err_read_vib:
	destroy_workqueue(vib->queue);
#if defined(CONFIG_DC_VIB_PM_LDO)
ldo_absent:
#endif
	return rc;
}

static int msm_vibrator_remove(struct platform_device *pdev)
{
	struct msm_vib *vib = dev_get_drvdata(&pdev->dev);

	destroy_workqueue(vib->queue);
	mutex_destroy(&vib->lock);

	return 0;
}

static const struct of_device_id vib_motor_match[] = {
	{	.compatible = "vibrator",
	},
	{}
};

static struct platform_driver msm_vibrator_platdrv =
{
	.driver =
	{
		.name = "msm_vibrator",
		.owner = THIS_MODULE,
		.of_match_table = vib_motor_match,
		.pm	= &vibrator_pm_ops,
	},
	.probe = msm_vibrator_probe,
	.remove = msm_vibrator_remove,
};

static int __init msm_timed_vibrator_init(void)
{
	pr_info("[VIB] %s\n",__func__);

	return platform_driver_register(&msm_vibrator_platdrv);
}

void __exit msm_timed_vibrator_exit(void)
{
	platform_driver_unregister(&msm_vibrator_platdrv);
}
module_init(msm_timed_vibrator_init);
module_exit(msm_timed_vibrator_exit);

MODULE_DESCRIPTION("timed output vibrator device");
MODULE_LICENSE("GPL v2");
