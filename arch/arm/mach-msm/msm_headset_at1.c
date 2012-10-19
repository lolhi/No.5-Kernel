    /* arch/arm/mach-msm/rpc_server_handset.c
 *
 * Copyright (c) 2008-2010, Code Aurora Forum. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can find it at http://www.fsf.org.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/switch.h>
#include <mach/mpp.h>                                   
#include <asm/mach-types.h>
#include <pmic.h>
#include <mach/board.h>
#include <mach/msm_headset_at1.h>
#include <mach/mpp.h>
#include <linux/mfd/pmic8058.h>
#include <mach/irqs.h>

#include <mach/vreg.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/delay.h>


#include <linux/regulator/consumer.h>                   
#include <linux/regulator/pmic8058-regulator.h>         
#include <linux/pmic8058-xoadc.h>
#include <asm/irq.h>
#include <linux/workqueue.h>
#include <linux/errno.h>

#include <linux/m_adc.h>
#include <mach/msm_xo.h>
#include <mach/msm_hsusb.h>
#include <linux/msm-charger.h>
#include <linux/wakelock.h>

#include <linux/pm_runtime.h>
#include <linux/pm_wakeup.h>

/* Macros assume PMIC GPIOs start at 0 */
#define PM8058_GPIO_BASE			NR_MSM_GPIOS
#define PM8058_GPIO_PM_TO_SYS(pm_gpio)		(pm_gpio + PM8058_GPIO_BASE)
#define PM8058_GPIO_SYS_TO_PM(sys_gpio)		(sys_gpio - PM8058_GPIO_BASE)
#define PM8058_MPP_BASE			(PM8058_GPIO_BASE + PM8058_GPIOS)
#define PM8058_MPP_PM_TO_SYS(pm_gpio)		(pm_gpio + PM8058_MPP_BASE)
#define PM8058_MPP_SYS_TO_PM(sys_gpio)		(sys_gpio - PM8058_MPP_BASE)
#define PM8058_IRQ_BASE				(NR_MSM_IRQS + NR_GPIO_IRQS)


#define HEADSET_DBG
#ifdef HEADSET_DBG
  #define hs_dbg(fmt, args...)	printk("[HEADSET] " fmt, ##args)
#else
  #define hs_dbg(fmt, args...)
#endif

struct wake_lock headset_wakelock;

#define DRIVER_NAME "msm-handset"

enum {
    NO_DEVICE   = 0,
    MSM_HEADSET = 1<<1,
    MSM_HEADPHONE=1<<2,
};

struct msm_headset {
    struct input_dev *ipdev;
    struct switch_dev sdev;
    struct msm_headset_platform_data *hs_pdata;
};

static struct regulator *hs_jack_l8;

static struct delayed_work earjack_work;
static struct delayed_work remotekey_work;
static unsigned int earjack_keycode;
static struct msm_headset *hs;
static struct msm_headset_platform_data *hspd;
static struct mutex headset_adc_lock;
static int headset_init;


static void earjack_det_func(struct work_struct * earjack_work);
static void remotekey_det_func(struct work_struct * remotekey_work);
static irqreturn_t Earjack_Det_handler(int irq, void *dev_id);
static irqreturn_t Remotekey_Det_handler(int irq, void *dev_id);
static int Regulator_L8_Enable(int en);
static int Check_ADC_MPP(int *nValue);
static int Remote_Interrupt_Enable(int en);
static void report_headset_switch(struct input_dev *dev, int key, int value);

static ssize_t show_headset(struct device *dev, struct device_attribute *attr
, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "%d\n", switch_get_state(&hs->sdev));
}

static ssize_t set_headset(struct device *dev, struct device_attribute *attr, 
const char *buf, size_t count)
{
    return 0;
}



static DEVICE_ATTR(headset, S_IRUGO | S_IWUSR, show_headset, set_headset);

static struct attribute *dev_attrs[] = {
    &dev_attr_headset.attr,
    NULL,
};
static struct attribute_group dev_attr_grp = {
    .attrs = dev_attrs,
};


static ssize_t msm_headset_print_name(struct switch_dev *sdev, char *buf)
{
    switch (switch_get_state(&hs->sdev)) {
    case NO_DEVICE:
        return sprintf(buf, "No Device\n");
    case MSM_HEADSET:
    case MSM_HEADPHONE:
        return sprintf(buf, "Headset\n");
        //return sprintf(buf, "HeadPhone\n");
    }
    return -EINVAL;
}

static void
report_headset_switch(struct input_dev *dev, int key, int value)
{
    struct msm_headset *hs = input_get_drvdata(dev);

    input_report_switch(dev, key, value);
    switch_set_state(&hs->sdev, value);
}

static irqreturn_t Earjack_Det_handler(int irq, void *dev_id)
{
	if(!headset_init) return IRQ_HANDLED;
	
	wake_lock_timeout(&headset_wakelock,HZ);
	schedule_delayed_work(&earjack_work,msecs_to_jiffies(200));
	return IRQ_HANDLED;
}

static void earjack_det_func( struct work_struct *earjack_work)
{
	int ret=0;
	int nADCValue=0;

	//wake_lock_timeout(&headset_wakelock,HZ/2);
	ret=gpio_get_value_cansleep(hspd->ear_det);	
	printk("EARJACK_DET=> %d\n", ret);

	if(ret==hspd->ear_det_active) {
		Regulator_L8_Enable(1);
		msleep(30);
		ret=Check_ADC_MPP(&nADCValue);
		hs_dbg("det nADCValue=%d\n",nADCValue);
		if(ret) {
			printk("Check_ADC_MPP() error ret=%d\n",ret);
			goto earjack_det_exit;
		}

		earjack_keycode=0;

		if(nADCValue>=1000) { // 4 pole 2180
			hspd->curr_state= MSM_HEADSET;
			Remote_Interrupt_Enable(1);
		} else {  // 3pole
			hspd->curr_state= MSM_HEADPHONE;
			Regulator_L8_Enable(0);
		}
	} else {
		Regulator_L8_Enable(0);
		if(hspd->curr_state==MSM_HEADSET)
			Remote_Interrupt_Enable(0);
		
		hspd->curr_state=0;
	}

	report_headset_switch(hs->ipdev, SW_HEADPHONE_INSERT, hspd->curr_state);
	input_sync(hs->ipdev);

earjack_det_exit:
	//wake_unlock(&headset_wakelock);
	return;
}

static irqreturn_t Remotekey_Det_handler(int irq, void *dev_id)
{
	int ret=0;

	if(!headset_init) return IRQ_HANDLED;
	if(hspd->curr_state!=MSM_HEADSET) return IRQ_HANDLED;
	
	wake_lock_timeout(&headset_wakelock,HZ*3);
	//printk("Remotekey_Det_handler()\n");

	ret=gpio_get_value_cansleep(hspd->ear_det);	
	if(ret!=hspd->ear_det_active) {
		printk("Remotekey_Det_handler() EARJACK_DET %d\n",ret);
		return IRQ_HANDLED;
	}
	
	schedule_delayed_work(&remotekey_work,msecs_to_jiffies(50));
	return IRQ_HANDLED;
}

static void remotekey_det_func(struct work_struct * remotekey_work) {
	int ret=0;
	int nADCValue=0,nADCValue2=0;
	int keycode=0,check_adc=0;

	//printk("remotekey_det_func()\n");

	ret=gpio_get_value_cansleep(hspd->ear_det);	
	if(ret!=hspd->ear_det_active) {
		printk("EARJACK_DET %d\n",ret);
		return;
	}

	//ret=gpio_get_value(hspd->remote_det);	
	ret=gpio_get_value_cansleep(hspd->remote_det);
	//printk("remote gpio=%d\n",ret);
	if(ret!=hspd->remote_det_active) {
		keycode=earjack_keycode;
		input_report_key(hs->ipdev, keycode,0);
		input_sync(hs->ipdev);
		earjack_keycode=0;
		printk("remote key release %d\n",keycode);
		goto remotekey_det_exit;
	}

	ret=Check_ADC_MPP(&nADCValue);
	if(ret) {
		printk("Check_ADC_MPP() error ret=%d\n",ret);
		goto remotekey_det_exit;
	}	

	msleep(3);

	ret=Check_ADC_MPP(&nADCValue2);
	if(ret) {
		printk("Check_ADC_MPP() error ret=%d\n",ret);
		goto remotekey_det_exit;
	}
	
	hs_dbg("remote nADCValue=%d %d\n",nADCValue,nADCValue2);

	if(nADCValue>=nADCValue2) check_adc=nADCValue-nADCValue2;
	else check_adc=nADCValue2-nADCValue;

	if(check_adc>=100) {
		printk("adc range fail %d\n",check_adc);
		goto remotekey_det_exit;
	}

	keycode=0;

	if(nADCValue<150) {  // 76
		keycode=KEY_MEDIA;
	} else if( nADCValue>=250 && nADCValue<=450) {  // 340
		keycode=KEY_VOLUMEUP;
	} else if( nADCValue>=500 && nADCValue<=700) {  // 600
		keycode=KEY_VOLUMEDOWN;
	}


	if(keycode!=0) {
#if AT1_BDVER_GE(AT1_WS22)
		ret=gpio_get_value_cansleep(hspd->remote_det);	
		if(ret!=hspd->remote_det_active) {
			printk("remote key-2 %d\n",ret);
			return;
		}
#endif
		earjack_keycode=keycode;
		input_report_key(hs->ipdev, earjack_keycode,1);
		input_sync(hs->ipdev);
		printk("earjack key PUSH %d\n",keycode);
	}

remotekey_det_exit:
	//wake_unlock(&headset_wakelock);
	return;
}

static int Remote_Interrupt_Enable(int en) {
	int ret =0;
	
	if(en) {
		ret=gpio_get_value_cansleep(hspd->ear_det);	
		if(ret!=hspd->ear_det_active) {
			printk("EARJACK_DET %d\n",ret);
			return -1;
		}
		
		if(hspd->curr_state == MSM_HEADSET  && hspd->remote_is_int==0) {
#if AT1_BDVER_GE(AT1_WS22) 
			ret = pm8058_mpp_config_digital_in(PM8058_MPP_SYS_TO_PM(hspd->remote_det),
							PM8058_MPP_DIG_LEVEL_S3,
							PM_MPP_DIN_TO_INT);
#else
			gpio_tlmm_config(GPIO_CFG(hspd->remote_det, 0, GPIO_CFG_INPUT,GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
#endif
			enable_irq(gpio_to_irq(hspd->remote_det));
			set_irq_wake(gpio_to_irq(hspd->remote_det),1);
			hspd->remote_is_int = 1;
		}
	} else {
		if(hspd->remote_is_int==1) {
			set_irq_wake(gpio_to_irq(hspd->remote_det),0);
			disable_irq(gpio_to_irq(hspd->remote_det));
			hspd->remote_is_int = 0;
#if AT1_BDVER_E(AT1_WS21) 
			gpio_tlmm_config(GPIO_CFG(hspd->remote_det, 0, GPIO_CFG_INPUT,GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
#endif
		}
	}

	return 0;
}

static int Regulator_L8_Enable(int en) {
	int err=0;
	
	if(en) {
		err=regulator_is_enabled(hs_jack_l8);
		//printk("regulator l8 is enable error=%d\n",err);
		if(!err) {
			err = regulator_set_voltage(hs_jack_l8,2700000,2700000);
			if(err) {
				printk("regulator l8 voltage error=%d\n",err);
				return err;
			}

			err = regulator_enable(hs_jack_l8);
			if(err) {
				printk("regulator l8 enable error=%d\n",err);
				return err;
			}
		}

	} else {
		err=regulator_is_enabled(hs_jack_l8);
		//printk("regulator l8 is enable error=%d\n",err);
		if(err) {
			err = regulator_disable(hs_jack_l8);
			if(err) {
				printk("regulator l8 enable error=%d\n",err);
				return err;
			}
		}
	}

	return 0;
}

static int Check_ADC_MPP(int *nValue) {
	int err=0,ret=0,result=0;
	void *h;
	struct adc_chan_result adc_chan_result;
	long  timeout=0;

	struct completion  conv_complete_evt;

	mutex_lock(&headset_adc_lock);

	ret=gpio_get_value_cansleep(hspd->ear_det);	
	if(ret!=hspd->ear_det_active) {
		printk("EARJACK_DET %d\n",ret);
		mutex_unlock(&headset_adc_lock);
		return -1;
	}

#if AT1_BDVER_GE(AT1_WS22) 
	if(hspd->curr_state == MSM_HEADSET) {
		Remote_Interrupt_Enable(0);
		msleep(1);
	}
#endif
		
	//sys_gpio= PM8901_GPIO_PM_TO_SYS(mpp);
	err=pm8058_mpp_config_analog_input(XOADC_MPP_3,PM_MPP_AIN_AMUX_CH5, PM_MPP_AOUT_CTL_DISABLE);
	if(err) {
		printk("pm8058_mpp_config_analog_input() err=%d\n",err);
		mutex_unlock(&headset_adc_lock);
		return -1;
	}

	ret = adc_channel_open(CHANNEL_ADC_HDSET, &h);
	if(ret) {
		printk("couldn't open channel %d ret=%d\n",CHANNEL_ADC_HDSET,ret);
		mutex_unlock(&headset_adc_lock);
		return -1;
	}

	init_completion(&conv_complete_evt);

	ret = adc_channel_request_conv(h, &conv_complete_evt);
	if(ret) {
		printk("couldn't request convert channel %d ret=%d\n",CHANNEL_ADC_HDSET,ret);
		result=-2;
		goto check_adc_out;
	}

	wait_for_completion(&conv_complete_evt);

/*
	timeout=wait_for_completion_timeout(&conv_complete_evt,msecs_to_jiffies(100));
	if(timeout<=0) {
		printk("headset ADC timeout\n");
		result=-3;
		goto check_adc_out;
	}
*/

	ret = adc_channel_read_result(h, &adc_chan_result);
	if(ret) {
		printk("could't read result channel %d ret=%d\n",CHANNEL_ADC_HDSET,ret);
		result=-4;
		goto check_adc_out;
	}


	*nValue=(int)adc_chan_result.measurement;

check_adc_out:
	
	ret = adc_channel_close(h);
	if(ret) {
		printk("could't close channel %d ret=%d\n",CHANNEL_ADC_HDSET,ret);
		mutex_unlock(&headset_adc_lock);
		return -1;
	}

#if AT1_BDVER_GE(AT1_WS22) 
	if(hspd->curr_state == MSM_HEADSET) {
		Remote_Interrupt_Enable(1);
	}
#endif

	//printk("ADC value=%d, physical=%d\n",(int)adc_chan_result.measurement,adc_chan_result.physical);
	mutex_unlock(&headset_adc_lock);
	return result;		
}


#ifdef CONFIG_PM
static int headset_suspend(struct device *dev)
{
	struct msm_headset  *dd = dev_get_drvdata(dev);
	struct msm_headset_platform_data *pd=dd->hs_pdata;
	int ret=0;

	//hs_dbg("headset_suspend()\n");

#if 0
	if (device_may_wakeup(dev)) {
		ret=gpio_get_value(hspd->ear_det);	
		hs_dbg("suspend EARJACK_DET %d\n",ret);
		ret=gpio_get_value(hspd->remote_det);	
		hs_dbg("suspend REMOTE_DET %d\n",ret);

		set_irq_wake(gpio_to_irq(pd->ear_det),1);

		//if(pd->curr_state==MSM_HEADSET)
			//set_irq_wake(gpio_to_irq(pd->remote_det),1);
	}
#endif

	return 0;
}

static int headset_resume(struct device *dev)
{
	struct msm_headset *dd = dev_get_drvdata(dev);
	struct msm_headset_platform_data *pd=dd->hs_pdata;
	int ret=0;

	//wake_lock_timeout(&headset_wakelock,HZ/3);
	//hs_dbg("headset_resume()\n");

#if 0
	if (device_may_wakeup(dev)) {
		ret=gpio_get_value(hspd->ear_det);	
		hs_dbg("resume EARJACK_DET %d\n",ret);
		ret=gpio_get_value(hspd->remote_det);	
		hs_dbg("resume REMOTE_DET %d\n",ret);
		
		set_irq_wake(gpio_to_irq(pd->ear_det),0);

		//if(pd->curr_state==MSM_HEADSET)
			//set_irq_wake(gpio_to_irq(pd->remote_det),0);
	}
#endif	
	return 0;
}

static struct dev_pm_ops headset_ops = {
	.suspend = headset_suspend,
	.resume = headset_resume,
};
#endif


static int __devinit hs_probe(struct platform_device *pdev)
{
    int rc = 0,err;
    struct input_dev *ipdev;

    hs_dbg("hs_probe start!!!\n");
	headset_init=0;
    //sky_hs_3p5pi_jack_ctrl.state=SKY_HS_JACK_STATE_INIT;
    hs = kzalloc(sizeof(struct msm_headset), GFP_KERNEL);
    if (!hs)
        return -ENOMEM;

    hspd = kzalloc(sizeof(struct msm_headset_platform_data), GFP_KERNEL);
    if (!hspd)
        return -ENOMEM;
	
    hs->sdev.name   = "h2w";
    hs->sdev.print_name = msm_headset_print_name;

    rc = switch_dev_register(&hs->sdev);
    if (rc)
        goto err_switch_dev_register;

    ipdev = input_allocate_device();
    if (!ipdev) {
        rc = -ENOMEM;
        goto err_switch_dev;
    }
    input_set_drvdata(ipdev, hs);

    hs->ipdev = ipdev;

    if (pdev->dev.platform_data) {
        hs->hs_pdata = pdev->dev.platform_data;
	  hspd = pdev->dev.platform_data;
    }

    if (hs->hs_pdata->hs_name)
        ipdev->name = hs->hs_pdata->hs_name;
    else
        ipdev->name = DRIVER_NAME;

	mutex_init(&headset_adc_lock);

    INIT_DELAYED_WORK(&earjack_work,earjack_det_func);
    INIT_DELAYED_WORK(&remotekey_work,remotekey_det_func);

    wake_lock_init(&headset_wakelock, WAKE_LOCK_SUSPEND, "Headset_wakelock");
    
    ipdev->id.vendor    = 0x0001;
    ipdev->id.product   = 1;
    ipdev->id.version   = 1;

    input_set_capability(ipdev, EV_KEY, KEY_MEDIA);
    input_set_capability(ipdev, EV_KEY, KEY_VOLUMEUP);
    input_set_capability(ipdev, EV_KEY, KEY_VOLUMEDOWN);
    //input_set_capability(ipdev, EV_KEY, KEY_END);
    input_set_capability(ipdev, EV_SW, SW_HEADPHONE_INSERT);
    //input_set_capability(ipdev, EV_KEY, KEY_POWER);    
    //input_set_capability(ipdev, EV_KEY, KEY_SEND);
 
    rc = input_register_device(ipdev);
    if (rc) {
        dev_err(&ipdev->dev,
                "hs_probe: input_register_device rc=%d\n", rc);
        goto err_reg_wakelock;
    }

	/* Enable runtime PM ops, start in ACTIVE mode */
	//rc = pm_runtime_set_active(&pdev->dev);
	//if (rc < 0)
	//	dev_dbg(&pdev->dev, "unable to set runtime pm state\n");
	//pm_runtime_enable(&pdev->dev);


    platform_set_drvdata(pdev, hs);

    device_init_wakeup(&pdev->dev,1);

    hs_jack_l8 = regulator_get(NULL, "8058_l8");
    if(IS_ERR(hs_jack_l8)) {
      printk("regulator l8 get error\n");
      goto err_reg_input_dev;
    }

	// Added sysfs. (/sys/devices/platform/msm-handset/headset)
    rc = sysfs_create_group(&pdev->dev.kobj, &dev_attr_grp);
    if (rc) {
        dev_err(&ipdev->dev,
                "hs_probe: sysfs_create_group rc=%d\n", rc);
        goto err_reg_input_dev;
    }


    err = gpio_request(hspd->ear_det, "headset_det");
    if(err) {
		printk("unable to request gpio headset_det err=%d\n",err); 
		goto err_reg_input_dev;
    }

    err=gpio_direction_input(hspd->ear_det);
    if(err) {
		printk("Unable to set direction headset_det err=%d\n",err); 
		goto err_reg_input_dev;
    }
#if AT1_BDVER_GE(AT1_WS22) 
    gpio_tlmm_config(GPIO_CFG(hspd->ear_det, 0, GPIO_CFG_INPUT,GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
#else
	gpio_tlmm_config(GPIO_CFG(hspd->ear_det, 0, GPIO_CFG_INPUT,GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
#endif
    //gpio_set_debounce(hspd->ear_det,5);

    //set_irq_wake(gpio_to_irq(hspd->ear_det),1);

    err=request_irq(gpio_to_irq(hspd->ear_det), Earjack_Det_handler, 
		IRQF_DISABLED|IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "earjack_det-irq", hs);
	if (err  < 0) {
		printk("Couldn't acquire ear_det as request_irq()");
		goto err_reg_input_dev;
	}

	set_irq_wake(gpio_to_irq(hspd->ear_det),1);

    err = gpio_request(hspd->remote_det, "remote_det");
    if(err) {
		printk("unable to request gpio remote_det err=%d\n",err); 
		goto err_reg_input_dev;
    }

#if AT1_BDVER_GE(AT1_WS22) 
	err = pm8058_mpp_config_digital_in(PM8058_MPP_SYS_TO_PM(hspd->remote_det),
					PM8058_MPP_DIG_LEVEL_S3,
					PM_MPP_DIN_TO_INT);
	err |=  pm8058_mpp_config_bi_dir(PM8058_MPP_SYS_TO_PM(hspd->remote_det),
					PM8058_MPP_DIG_LEVEL_S3,
					PM_MPP_BI_PULLUP_OPEN);	

	printk("mpp config %d mpp %d err=%d\n",hspd->remote_det,
		PM8058_MPP_SYS_TO_PM(hspd->remote_det),err);
	printk(" remote %d %d\n",gpio_to_irq(hspd->remote_det),PM8058_MPP_IRQ(PM8058_IRQ_BASE,PM8058_MPP_SYS_TO_PM(hspd->remote_det)));

    err=request_threaded_irq(gpio_to_irq(hspd->remote_det),NULL,
		Remotekey_Det_handler,IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "remote_det-irq", hs);
#else
    err=gpio_direction_input(hspd->remote_det);
    if(err) {
		printk("Unable to set direction remote_det err=%d\n",err); 
		goto err_reg_input_dev;
    }

    //gpio_tlmm_config(GPIO_CFG(hspd->remote_det, 0, GPIO_CFG_INPUT,GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(hspd->remote_det, 0, GPIO_CFG_INPUT,GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
    //gpio_set_debounce(hspd->remote_det,5);


    err=request_irq(gpio_to_irq(hspd->remote_det), Remotekey_Det_handler, 
              IRQF_DISABLED|IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "remote_det-irq", hs);
#endif
	if (err  < 0) {
		printk("Couldn't acquire remote_det as request_irq()");
		goto err_reg_input_dev;
	}

	disable_irq(gpio_to_irq(hspd->remote_det));

	headset_init=1; // headset init

	
    rc=gpio_get_value_cansleep(hspd->ear_det);
    if(rc==hspd->ear_det_active) {
	  schedule_delayed_work(&earjack_work,msecs_to_jiffies(200));
	  //schedule_delayed_work(&earjack_work,200);
    }

        printk("hs_probe success!!!\n");

    return 0;

err_reg_input_dev:
    input_unregister_device(ipdev);
err_reg_wakelock:
    wake_lock_destroy(&headset_wakelock);
    input_free_device(ipdev);
err_switch_dev:
    switch_dev_unregister(&hs->sdev);
err_switch_dev_register:
    kfree(hspd);
    kfree(hs);
    return rc;
}

static int __devexit hs_remove(struct platform_device *pdev)
{
    struct msm_headset *ihs = platform_get_drvdata(pdev);

    //pm_runtime_disable(&pdev->dev);
    device_init_wakeup(&pdev->dev,0);

    input_unregister_device(ihs->ipdev);
    switch_dev_unregister(&ihs->sdev);

    free_irq(gpio_to_irq(hspd->remote_det), NULL);
    free_irq(gpio_to_irq(hspd->ear_det), NULL);

    gpio_free(hspd->remote_det);
    gpio_free(hspd->ear_det);

    kfree(ihs);
    kfree(hspd);
    kfree(hs);

    regulator_put(hs_jack_l8);
    wake_lock_destroy(&headset_wakelock);
    return 0;
}

static struct platform_driver hs_driver = {
    .probe      = hs_probe,
    .remove     = __devexit_p(hs_remove),
    .driver     = {
        .name   = DRIVER_NAME,
        .owner  = THIS_MODULE,
#ifdef CONFIG_PM	
	  .pm = &headset_ops,
#endif
    },
};

static int __init hs_init(void)
{
    return platform_driver_register(&hs_driver);
}
late_initcall(hs_init);

static void __exit hs_exit(void)
{
    platform_driver_unregister(&hs_driver);
}
module_exit(hs_exit);

MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:msm-handset");

