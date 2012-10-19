/*
 *  max17040_battery.c
 *  fuel-gauge systems for lithium-ion (Li+) batteries
 *
 *  Copyright (C) 2009 Samsung Electronics
 *  Minkyu Kang <mk7.kang@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/power_supply.h>
#include <linux/max17040_battery.h>
#include <linux/slab.h>

//test code
#include <linux/acct.h>
#include <linux/m_adc.h>
#include <mach/msm_xo.h>
#include <mach/msm_hsusb.h>
#include <linux/android_alarm.h>
#include <linux/earlysuspend.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/rtc.h>
#include <linux/input.h>
#define MAX17040_VCELL_MSB	0x02
#define MAX17040_VCELL_LSB	0x03
#define MAX17040_SOC_MSB	0x04
#define MAX17040_SOC_LSB	0x05
#define MAX17040_MODE_MSB	0x06
#define MAX17040_MODE_LSB	0x07
#define MAX17040_VER_MSB	0x08
#define MAX17040_VER_LSB	0x09
#define MAX17040_RCOMP_MSB	0x0C
#define MAX17040_RCOMP_LSB	0x0D
#ifdef FEATURE_AT1_FUELGAUGE_CUSTOM
#define MAX17040_OCV_MSB	0x0E
#define MAX17040_OCV_LSB	0x0F
#define MAX17040_UNLOCK_MSB	0x3E
#define MAX17040_UNLOCK_LSB	0x3F
#endif
#define MAX17040_CMD_MSB	0xFE
#define MAX17040_CMD_LSB	0xFF

#define MAX17040_DELAY		1000
#define MAX17040_BATTERY_FULL	95

#ifndef FEATURE_AT1_PMIC_BATTERY
#define MAX17040_BATT_ID_MAX_MV  800
#define MAX17040_BATT_ID_MIN_MV  600
#else
#define MAX17040_BATT_ID_MAX_MV  1300
#define MAX17040_BATT_ID_MIN_MV  0
#endif

#ifdef FEATURE_AT1_PMIC_BATTERY
#define MAX17040_CABLE_ID_FACTORY_MAX_MV  1500
#define MAX17040_CABLE_ID_FACTORY_MIN_MV  1000

#define MAX17040_CABLE_ID_USB_MAX_MV  3900
#define MAX17040_CABLE_ID_USB_MIN_MV  3300

typedef enum {
	MAX17040_CABLE_ID_NONE= 0,
	MAX17040_CABLE_ID_USB,
	MAX17040_CABLE_ID_FACTORY,
}MAX17040_TABLE_NUM;
#endif

#define FAST_POLL	(1 * 60) //50 seconds
#define SLOW_POLL	(1 * 130) //120 seconds

#define MAX_READ	10
//#define MAX17040_ALARM_RTC_ENABLE //RTC ENABLE

//#define MAX17040_DEG_ENABLE	//normal debug
#define MAX17040_SLEEP_DEBUG //sleep time debug
//#define MAX17040_DEBUG_QUICK
//#define MAX17040_TIME_DEBUG
#ifdef MAX17040_DEG_ENABLE
#define dbg(fmt, args...)   printk("##################[MAX17040] " fmt, ##args)
#else
#define dbg(fmt, args...)
#endif
#ifdef MAX17040_SLEEP_DEBUG
#define sleep_dbg(fmt, args...)   printk("[MAX17040 SLEEP] " fmt, ##args)
#else
#define sleep_dbg(fmt, args...) 
#endif
#define dbg_func_in()       dbg("[FUNC_IN] %s\n", __func__)
#define dbg_func_out()      dbg("[FUNC_OUT] %s\n", __func__)
#define dbg_line()          dbg("[LINE] %d(%s)\n", __LINE__, __func__)

#define ABS_WAKE                            (ABS_MISC)

//P12911 : Compensation Table
#define COMPENSATION_MAX 8

//p12911 : depend on quick_start
#define SKY_SOC_LSB	256
#define SKY_MULT_100(x)	(x*100)	
#define SKY_MULT_1000(x)	(x*1000)	
#define SKY_MULT_10000(x)	(x*10000)	

#define SKY_MULT_1000000(x)	(x*1000000)	
#define SKY_DIV_1000000(x)	(x/1000000)	
//p12911 : depend on soc
#define SKY_SOC_EMPTY	12
#define SKY_SOC_FULL	984 //original value : 984 

#define FINISHED_CHARGING		5
//p12911 : sleep config
#define SLEEP_ONE_MINUTE 60 // 1 minute
#define SLEEP_THREE_MINUTE 180 // 3 minute
#define SLEEP_FIVE_MINUTE 300 // 5 minute
#define	SLEEP_ONE_HOUR 3600 // 20 minute



static int charge_state=0;

extern int sky_is_batt_status(void);
extern int sky_get_plug_state(void);

typedef enum {
	NoEvents= 0,
	Events,
}MAX17040_EVENT;


/*ps2 team shs 
 Early_resume : 40 seconds
 Early_suspend : 120 seconds
*/

typedef enum {
	Early_resume=0,  
	Early_suspend,

}MAX17040_STATE;
struct max17040_quick_data{
	unsigned int vcell_msb;
	unsigned int vcell_lsb;
	unsigned int soc_msb;
	unsigned int soc_lsb;

	unsigned long refrence_soc;
	/* battery quick voltage*/
	unsigned long quick_vcell;
	/* battery quick soc*/
	unsigned long quick_soc;
	/* battery quick start*/
	int quick_state;

};

struct max17040_chip {

	struct mutex	data_mutex;
	struct mutex	i2c_mutex;
	struct mutex 	quick_mutex;
	struct i2c_client		*client;
	struct delayed_work		work;
	struct power_supply		battery;
	struct max17040_platform_data	*pdata;

	//ps2 team shs : workqueue_struct
	struct workqueue_struct *monitor_wqueue;
	//ps2 team shs : work_struct
	struct work_struct monitor_work;
	//ps2 team shs : alarm
	struct alarm alarm;
	//ps2 team shs : wake up lock
	struct wake_lock work_wake_lock;
	//ps2 team shs : TEST MODE : wake up lock 
	
	unsigned long timestamp;		/* units of time */
	
	//PS2 team shs : 
	ktime_t last_poll;
	

	/* State Of Connect */
	int online;
	/* battery voltage */
	int vcell;
	/* battery capacity */
	int soc;

	/* battery brefore soc*/
	int prev_soc;

	/* i2c error filed*/

	int i2c_state;

	/* i2c voltage_error */
	int i2c_state_vol;

	int prev_voltage;
	/*battery quick data
*/
	struct max17040_quick_data quick_data;

	/* test code states*/
	atomic_t set_test;

	/* State Of update*/
	MAX17040_EVENT event;

	/* State Of update*/
	MAX17040_EVENT suspend_event;

	
	MAX17040_STATE slow_poll;	
	/* test code : input device driver*/
	struct input_dev *max17040_input_data;

	/*ealry suspend */
	struct early_suspend early_suspend;
};
 
typedef struct{
  unsigned long volt;   // read voltage
  unsigned long soc;	// read soc
  unsigned long slop;   // ?????????? ???? ???? ???? ?????? ???? x1000
  unsigned long offset; // mV
}fuelgauge_linearlize_type;

struct max17040_chip max17040_data;
/* ps2 team shs : check state
struct max17040_platform_data {
	int (*battery_online)(void);
	int (*charger_online)(void);
	int (*charger_enable)(void);

	1. battery_online : check battery connection
	2. charger_online : check charger connection
	3. charger_enable : check charger state 
};
*/
//ps1 team shs : msm_pm_set_max_sleep_time function
extern void msm_pm_set_max_sleep_time(int64_t sleep_time_ns);
/*PS2 TEAM SHS [START]: Depend on FUELGAUGE QUICK START */

#ifdef CONFIG_SKY_CHARGING
extern int pm8058_chg_nobattery_factory_cable(void);
#endif
#ifdef FEATURE_AT1_PMIC_BATTERY
static int max17040_is_cable_id(void);
#endif

#ifndef FEATURE_AT1_PMIC_BATTERY
static fuelgauge_linearlize_type sky_fuelgauge_linearlize_table[1][COMPENSATION_MAX]={
	// vol , soc,  slop, offset
  { // Discharging, 1.260W ( 300mA 4.2V )
    { 40936,  985,  232,  18028},
    { 38892,  755,    88,  32183},
    { 37694,  563,    62,  34176},
    { 36904,  316,    32,  35889},
    { 36391,  166,    34,  35824},
    { 35419,   42,     78,  35081},
    { 34862,   18,   226,  34452},
    { 33996,     0,   481,  33995}
  }
  /*{ // Discharging table ( 250mA 4.2V )
    { 40684,  985,  224, 18630},
    { 38791,  769,  88,  31971},
    { 37532,  590,  61,  34044},
    { 36826,  340,  32,  35717},
    { 36357,  166,  26,  35902},
    { 35499,  42,   70,  35167},
    { 34866,  18,   203, 34541},
    { 33975,  0,    561, 33975},
  }*/
};
#else
static fuelgauge_linearlize_type sky_fuelgauge_linearlize_table[2][COMPENSATION_MAX]={
  {
    { 40906,  984,  219,  19334},
    { 38892,  755,    87,  32313},
    { 37694,  563,    61,  34316},
    { 36904,  316,    28,  36027},
    { 36391,  166,    34,  35875},
    { 35419,   42,     68,  35310},
    { 34862,   18,     77,  35233},
    { 33996,     0,   485,  33992},
  },
  {
    { 41943,  1000,  15,  40460},
    { 41754,  873,    69,  35695},
    { 40768,  731,    48,  37288},
    { 39485,  461,    18,  38673},
    { 39077,  229,    48,  37979},
    { 38667,  144,     54,  37888},
    { 38016,   25,    461,  36868},
    { 37223,     8,   967,  36466},
  }
#if 0  
  { // Discharging, 1.260W ( 300mA 4.2V )
    { 40936,  985,  232,  18028},
    { 38892,  755,    88,  32183},
    { 37694,  563,    62,  34176},
    { 36904,  316,    32,  35889},
    { 36391,  166,    34,  35824},
    { 35419,   42,     78,  35081},
    { 34862,   18,   226,  34452},
    { 33996,     0,   481,  33995}
  },
  { // Charging, 900mA
    { 41951, 1000,      12,   40727},
    { 41756,   840,      62,   36474},
    { 41062,   729,      48,   37533},
    { 39988,   507,      21,   38874},
    { 39390,   234,      44,   38350},
    { 38897,   123,      45,   38339},
    { 38435,     21,    200,   38011},
    { 38140,       7,    791,   37568} 
  },
  { // Charging, 400mA
    { 41941, 1000,      83,   33579},
    { 40352,   809,      57,   35703},
    { 38472,   482,      18,   37570},
    { 37990,   224,      61,   36619},
    { 37339,   117,      41,   36846},
    { 36950,     24,    320,   36152},
    { 36510,     11,    644,   35787},
    { 50000,       0,       0,          0}
  } 
#endif  
};
#endif
static int max17040_check_restart( unsigned long avoltage, int soc )
{
	int i=0;
	#if 0//def FEATURE_AT1_PMIC_BATTERY
	int ischarging = 0;
	#endif
	
	//ps2 team shs : dont check charging mode
	unsigned long sky_low_soc=0;
	unsigned long sky_fuelgauge_ref_soc=0;
	unsigned long sky_high_soc=0;
	int high_soc=0;
	int low_soc=0;
	
	#if 0//def FEATURE_AT1_PMIC_BATTERY
	//ischarging = max17040_is_cable_id();
	ischarging = sky_get_plug_state();
	printk("max17040_check_restart :: ischarging = %d", ischarging);
	#endif
	
	for( i = 0; i < COMPENSATION_MAX; i++ )
	{
		#if 0//def FEATURE_AT1_PMIC_BATTERY
		if( avoltage >= sky_fuelgauge_linearlize_table[ischarging][i].volt )
		#else
		if( avoltage >= sky_fuelgauge_linearlize_table[0][i].volt )
		#endif
		{
			mutex_lock(&max17040_data.quick_mutex); 			
			#if 0//def FEATURE_AT1_PMIC_BATTERY
			sky_high_soc = (avoltage - sky_fuelgauge_linearlize_table[ischarging][i].offset)/sky_fuelgauge_linearlize_table[ischarging][i].slop;
			sky_low_soc=(avoltage - sky_fuelgauge_linearlize_table[ischarging][i].offset)%sky_fuelgauge_linearlize_table[ischarging][i].slop;	  
			sky_fuelgauge_ref_soc = SKY_MULT_1000(sky_high_soc)+(SKY_MULT_1000(sky_low_soc)/sky_fuelgauge_linearlize_table[ischarging][i].slop);	  
			#else
			sky_high_soc = (avoltage - sky_fuelgauge_linearlize_table[0][i].offset)/sky_fuelgauge_linearlize_table[0][i].slop;
			sky_low_soc=(avoltage - sky_fuelgauge_linearlize_table[0][i].offset)%sky_fuelgauge_linearlize_table[0][i].slop;	  
			sky_fuelgauge_ref_soc = SKY_MULT_1000(sky_high_soc)+(SKY_MULT_1000(sky_low_soc)/sky_fuelgauge_linearlize_table[0][i].slop);	  
			#endif
			high_soc=sky_fuelgauge_ref_soc + 30000;
			low_soc=sky_fuelgauge_ref_soc  - 30000;
			mutex_unlock(&max17040_data.quick_mutex);		  
			//ps1 team shs :  soc+20 > soc and soc < soc-20 and soc <=1 && ref_soc > 4
			if( (soc > high_soc) || (soc < low_soc)|| ( (soc <= 1000) && (sky_fuelgauge_ref_soc > 4000) ) )
			{
				max17040_data.quick_data.quick_state=i;	      
				sleep_dbg("[QUICK START] voltage : [%u]mv, soc : [%d], index [%d], sky_high_soc [%u] ,sky_low_soc [%u]\n",avoltage,soc,i,sky_high_soc,sky_low_soc);	      
				sleep_dbg("[QUICK START] ref_soc : [%u], high_soc : [%u], low_soc : [%d]\n",sky_fuelgauge_ref_soc,high_soc,low_soc);	      			
				return 1;
			}
			break;
		}
	  }
	max17040_data.quick_data.quick_state=-1;	
	sleep_dbg("[QUICK DISABLE] voltage : [%u]mv, soc : [%d], index [%d], sky_high_soc [%u], sky_low_soc [%u] \n",avoltage,soc,i,sky_high_soc,sky_low_soc);	      
	sleep_dbg("[QUICK DISABLE] ref_soc : [%u], high_soc : [%u], low_soc : [%d]\n",sky_fuelgauge_ref_soc,high_soc,low_soc);	      			
	if( i == 8 && soc > 30000) /* voltage below 3.3v & soc over 30% */
       	return 1;

	return 0;
}

/*PS2 TEAM SHS [END]: Depend on FUELGAUGE QUICK START */

/*PS2 TEAM SHS : usb cable check function */
extern int sky_get_plug_state(void);



/*PS2 TEAM SHS [START]: Depend on batt id */
static int max17040_batt_read_adc(int channel, int *mv_reading)
{
	int ret;
	void *h;
	struct adc_chan_result adc_chan_result;
	struct completion  conv_complete_evt;

	dbg("%s: called for %d\n", __func__, channel);
	ret = adc_channel_open(channel, &h);
	if (ret) {
		dbg("%s: couldnt open channel %d ret=%d\n",
					__func__, channel, ret);
		goto out;
	}
	init_completion(&conv_complete_evt);
	ret = adc_channel_request_conv(h, &conv_complete_evt);
	if (ret) {
		dbg("%s: couldnt request conv channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}

	wait_for_completion(&conv_complete_evt);

/*
	ret = wait_for_completion_interruptible(&conv_complete_evt);
	if (ret) {
		dbg("%s: wait interrupted channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}
*/
	ret = adc_channel_read_result(h, &adc_chan_result);
	if (ret) {
		dbg("%s: couldnt read result channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}
	ret = adc_channel_close(h);
	if (ret) {
		dbg("%s: couldnt close channel %d ret=%d\n",
						__func__, channel, ret);
	}
	if (mv_reading)
		*mv_reading = adc_chan_result.measurement;

	dbg("%s: done for %d\n", __func__, channel);
	return adc_chan_result.physical;
out:
	dbg("%s: done for %d\n", __func__, channel);
	return -EINVAL;

}
static int max17040_is_battery_id_valid(void)
{
	int batt_id_mv;

	batt_id_mv = max17040_batt_read_adc(CHANNEL_ADC_BATT_ID, NULL);

	dbg("%s: BATT ID is %d\n", __func__, batt_id_mv);


	if (batt_id_mv > 0
		&& batt_id_mv > MAX17040_BATT_ID_MIN_MV
		&& batt_id_mv < MAX17040_BATT_ID_MAX_MV)
		return 1;

	return 0;
}
/*PS2 TEAM SHS [END]: Depend on batt id */
#if 0
static inline enum chg_type usb_get_chg_type(struct usb_info *ui)
{
	if ((readl(USB_PORTSC) & PORTSC_LS) == PORTSC_LS)
		return USB_CHG_TYPE__WALLCHARGER;
	else
		return USB_CHG_TYPE__SDP;
}
#endif
#if 0//def FEATURE_AT1_PMIC_BATTERY
static int max17040_is_cable_id(void)
{
	int cable_mv;

	cable_mv = max17040_batt_read_adc(CHANNEL_ADC_BATT_AMON, NULL);
	pr_info("%s: cable_mv is %d\n", __func__, cable_mv);

	#if 0
	if (cable_mv > MAX17040_CABLE_ID_FACTORY_MIN_MV
		&& cable_mv < MAX17040_CABLE_ID_FACTORY_MAX_MV)
		return MAX17040_CABLE_ID_FACTORY;
	#endif	
	if (cable_mv > MAX17040_CABLE_ID_USB_MIN_MV
		&& cable_mv < MAX17040_CABLE_ID_USB_MAX_MV)
		return MAX17040_CABLE_ID_USB;		
	/*
	 * return 0 to tell the upper layers
	 * we couldnt read the battery voltage
	 */
	return MAX17040_CABLE_ID_NONE;
}
#endif

/*PS2 TEAM SHS [START]: depend on wake lock */
static int max17040_wake_state;


//ps2 team shs : depend on normal mode wake lock
void max17040_prevent_suspend(void)
{
	dbg_func_in();
	if(!max17040_wake_state)
		{
		wake_lock(&max17040_data.work_wake_lock);
		max17040_wake_state=1;
		}
	dbg_func_out();
}
void max17040_allow_suspend(void)
{

	dbg_func_in();
	if(max17040_wake_state)
		{
		wake_unlock(&max17040_data.work_wake_lock);
		max17040_wake_state=0;		
		}
	dbg_func_out();
}
/*PS2 TEAM SHS [END]: depend on wake lock */

int max17040_get_voltage(void)
{
	dbg_func_in();
	if( max17040_data.i2c_state_vol )
	return max17040_data.prev_voltage;
	else
	return max17040_data.vcell;
	dbg_func_out();
}

int max17040_get_charge_state(void)
{
	dbg_func_in();
	return charge_state;
	dbg_func_out();	
}
/*PS2 TEAM SHS [END]: depend on test */


static enum power_supply_property max17040_battery_props[] = {
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CAPACITY,
};

static int max17040_get_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    union power_supply_propval *val)
{
	dbg_func_in();
	switch (psp) {
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		if( max17040_data.i2c_state_vol )
		return max17040_data.prev_voltage*1000;
		else
		val->intval = max17040_data.vcell*1000; //2011.05.16 leecy add for battery Info
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		if(max17040_data.i2c_state)
		val->intval = max17040_data.prev_soc;		
		else
		val->intval = max17040_data.soc;
		break;
	default:
		return -EINVAL;
	}
	dbg_func_out();		
	return 0;
}
static void max17040_bat_external_power_changed(struct power_supply *psy)
{
}

/*PS2 TEAM SHS [START]: depend on I2C READ/WRITE */
static int max17040_write_reg(u8 reg, u8 ahigh, u8 alow)
{
	int ret;
	u8 buf[20];
	buf[0]=reg;
	buf[1]=ahigh;
	buf[2]=alow;
	if ( max17040_data.client == NULL ) {
		sleep_dbg("%s : max17040_data.client is NULL\n", __func__);
		return -ENODEV;
	}
	mutex_lock(&max17040_data.i2c_mutex); 
	ret = i2c_master_send(max17040_data.client, buf, 3);		
	mutex_unlock(&max17040_data.i2c_mutex);

	sleep_dbg("max17040 I2C WRITE FILED ret [%d]\n",ret);

	if (ret<0)
	return -1;
	else if (ret != 3)
	return -1;
	else
	return ret;
	
}

#ifdef FEATURE_AT1_FUELGAUGE_CUSTOM
static int max17040_write_reg_16bytes(u8 reg)
{
	int ret;
	u8 buf[20];

	switch(reg)
	{
		case 0x40:
			buf[0]=reg;
			buf[1]=0xa7;
			buf[2]=0x90;
			buf[3]=0xb8;
			buf[4]=0x00;
			buf[5]=0xba;
			buf[6]=0x50;
			buf[7]=0xba;
			buf[8]=0x90;
			buf[9]=0xba;
			buf[10]=0xd0;
			buf[11]=0xbc;
			buf[12]=0x40;
			buf[13]=0xbc;
			buf[14]=0x80;
			buf[15]=0xbc;
			buf[16]=0xc0;
			break;
			
		case 0x50:
			buf[0]=reg;
			buf[1]=0xbc;
			buf[2]=0xe0;
			buf[3]=0xbd;
			buf[4]=0x10;
			buf[5]=0xbd;
			buf[6]=0x40;
			buf[7]=0xc0;
			buf[8]=0x40;
			buf[9]=0xc1;
			buf[10]=0x80;
			buf[11]=0xc4;
			buf[12]=0xd0;
			buf[13]=0xc7;
			buf[14]=0xe0;
			buf[15]=0xd0;
			buf[16]=0x40;		
			break;
			
		case 0x60:
			buf[0]=reg;
			buf[1]=0x02;
			buf[2]=0xf0;
			buf[3]=0x0b;
			buf[4]=0xd0;
			buf[5]=0x6e;
			buf[6]=0xa0;
			buf[7]=0x31;
			buf[8]=0x20;
			buf[9]=0x00;
			buf[10]=0xd0;
			buf[11]=0x24;
			buf[12]=0xf0;
			buf[13]=0x52;
			buf[14]=0x40;
			buf[15]=0x80;
			buf[16]=0x00;		
			break;
			
		case 0x70:
			buf[0]=reg;
			buf[1]=0x7a;
			buf[2]=0xc0;
			buf[3]=0x30;
			buf[4]=0x20;
			buf[5]=0x09;
			buf[6]=0x00;
			buf[7]=0x26;
			buf[8]=0x50;
			buf[9]=0x05;
			buf[10]=0xf0;
			buf[11]=0x0d;
			buf[12]=0xd0;
			buf[13]=0x07;
			buf[14]=0xd0;
			buf[15]=0x07;
			buf[16]=0xd0;		
			break;
			
		default:
			return -ENODEV;
	}
	if ( max17040_data.client == NULL ) {
		sleep_dbg("%s : max17040_data.client is NULL\n", __func__);
		return -ENODEV;
	}
	mutex_lock(&max17040_data.i2c_mutex); 
	ret = i2c_master_send(max17040_data.client, buf, 17);		
	mutex_unlock(&max17040_data.i2c_mutex);

	sleep_dbg("%s : max17040 I2C WRITE FILED ret [%d]\n",__func__,ret);
	
	if (ret<0)
	return -1;
	else if (ret != 17)
	return -1;
	else
	return ret;
}

static void max17040_read_reg_bytes(u8 reg, char* buf, u8 num)
{
	u8 ret_s, ret_r, ret;

	buf[0]=reg;
	mutex_lock(&max17040_data.i2c_mutex); 
	ret_s = i2c_master_send(max17040_data.client, buf, 1);
	ret_r = i2c_master_recv(max17040_data.client, buf, num);
	mutex_unlock(&max17040_data.i2c_mutex);
	if(ret_s<0 || ret_r<0)
	{
		sleep_dbg("max17040 I2C FIELD ret [%d] [%d]\n",ret_s,ret_r);
		return -1;
	}
	else if(ret_s != 1 || ret_r != num)
	{
		sleep_dbg("max17040 I2C FIELD [%d] [%d] bytes transferred (1, num)\n",ret_s,ret_r);
		return -1;
	}
	else
	ret=buf[0];
	return ret;
}
#endif

static u8 max17040_read_reg(u8 reg)
{
	u8 ret_s, ret_r, ret;
	u8 buf[20];

	buf[0]=reg;
	mutex_lock(&max17040_data.i2c_mutex); 
	ret_s = i2c_master_send(max17040_data.client,  buf, 1);
	ret_r = i2c_master_recv(max17040_data.client, buf, 1);
	mutex_unlock(&max17040_data.i2c_mutex);
	if(ret_s<0 || ret_r<0)
	{
		sleep_dbg("max17040 I2C FIELD ret [%d] [%d]\n",ret_s,ret_r);
		return -1;
	}
	else if(ret_s != 1 || ret_r != 1)
	{
		sleep_dbg("max17040 I2C FIELD [%d] [%d] bytes transferred (expected 1)\n",ret_s,ret_r);
		return -1;
	}
	else
	ret=buf[0];
	return ret;
}
/*PS2 TEAM SHS [END]: depend on I2C READ/WRITE */

/*PS2 TEAM SHS [START]: depend on chip command */


static void max17040_restart(void)
{
	int ret=0, i=0;
	ret=max17040_write_reg(MAX17040_MODE_MSB, 0x40,0x00);
	if(ret<0)
	{
		for(i=0;i<MAX_READ;i++)
		{
			ret=max17040_write_reg(MAX17040_MODE_MSB, 0x40,0x00);
			if(ret<0)
			continue;
			else
			break;
		}
	}
}

static void max17040_set_rcomp(void)
{
	#ifdef FEATURE_AT1_PMIC_BATTERY
	max17040_write_reg(MAX17040_RCOMP_MSB, 0x9b,0x00);
	#else
	max17040_write_reg(MAX17040_RCOMP_MSB, 0xc0,0x00);
	#endif
	msleep(200);
}

#ifdef FEATURE_AT1_FUELGAUGE_CUSTOM
static void max17040_restore_backup(u8 reg, char* backup, u8 num)
{
	int ret;
	u8 buf[20];
	int i;

	buf[0]=reg;
	for(i=0;i<num;i++)
	{
		buf[i+1] = backup[i];
	}
	mutex_lock(&max17040_data.i2c_mutex); 
	ret = i2c_master_send(max17040_data.client, buf, num+1);		
	mutex_unlock(&max17040_data.i2c_mutex);
	if (ret!=17)
		sleep_dbg("max17040 I2C WRITE FILED ret [%d]\n",ret);

	return ret;
}

static void max17040_set_max_rcomp(void)
{
	if (max17040_write_reg(MAX17040_RCOMP_MSB, 0xff,0x00) < 0){
		pr_err("%s: failed max17040_set_max_rcomp\n", __func__);
		return 1;
	}
}

static void max17040_set_ocv(void)
{
	if (max17040_write_reg(MAX17040_OCV_MSB, 0xda,0x40) < 0){
		pr_err("%s: failed max17040_set_ocv\n", __func__);
		return 1;
	}
}

static void max17040_unlock(void)
{
	if (max17040_write_reg(MAX17040_UNLOCK_MSB, 0x4a,0x57) < 0){
		pr_err("%s: failed max17040_unlock\n", __func__);
		return 1;
	}
}

static void max17040_lock(void)
{
	if (max17040_write_reg(MAX17040_UNLOCK_MSB, 0x00, 0x00) < 0){
		pr_err("%s: failed max17040_lock\n", __func__);
		return 1;
	}	
}

#endif

static void max17040_quick_get_soc(void)
{
	u8 msb=0;
	u8 lsb=0;
	int avalue=0;
	unsigned long quick_soc;
	int i=0;
	dbg_func_in();
	msb = max17040_read_reg(MAX17040_SOC_MSB);
	lsb = max17040_read_reg(MAX17040_SOC_LSB);
	if(msb < 0 || lsb < 0)
	{
		for(i=0;i<MAX_READ;i++)
		{
		msb = max17040_read_reg(MAX17040_SOC_MSB);
		lsb = max17040_read_reg(MAX17040_SOC_LSB);
		if(msb < 0 || lsb <0)
		{
		continue;
		}
		else
		break;
		}
	}
	/*//description
	read i2c data [msb=20,lsb=10]
	avalue=20*1000+(10*1000)/256
	*/
	avalue=SKY_MULT_1000(msb)+(SKY_MULT_1000(lsb)/SKY_SOC_LSB);	
	//Ajdusted soc%=(SOC%-EMPTY)/(FULL-EMPTY)*100
	//logic code	
	sleep_dbg("MAX17040_QUICK Adjusted SOC MSB [%d] : LSB [%d] : Adjusted SOC [%d] :Try Count [%d]\n",msb,lsb,avalue,i);
	mutex_lock(&max17040_data.data_mutex); 	
	max17040_data.quick_data.soc_msb=msb;	
	max17040_data.quick_data.soc_lsb=lsb;
	if(i==MAX_READ)	
	max17040_data.quick_data.quick_soc=0;			
	else
	max17040_data.quick_data.quick_soc=avalue;	
	mutex_unlock(&max17040_data.data_mutex);
	dbg_func_out();		
}

static void max17040_quick_get_vcell(void)
{
	u8 msb=0;
	u8 lsb=0;
	unsigned long quick_avalue;
	unsigned long temp;	
	unsigned long voltage=0;
	int i=0;	
	msb = max17040_read_reg(MAX17040_VCELL_MSB);
	lsb = max17040_read_reg(MAX17040_VCELL_LSB);
	if(msb < 0 || lsb < 0)
	{
		for(i=0;i<MAX_READ;i++)
		{
		msb = max17040_read_reg(MAX17040_VCELL_MSB);
		lsb = max17040_read_reg(MAX17040_VCELL_LSB);
		if(msb < 0 || lsb <0)
		{
		continue;
		}
		else
		break;
		}
	}
	voltage=(msb<<4)|((lsb&0xf0)>>4);
	quick_avalue=(voltage*1250)/100;
	sleep_dbg("MAX17040_QUICK  LOW MSB [%d] : LSB [%d] : LOW VOLTAGE [%d]\n",msb,lsb,voltage);
	sleep_dbg("MAX17040_QUICK  Adjusted [%d] : I2C Error Count [%d]\n",quick_avalue,i);
	mutex_lock(&max17040_data.data_mutex); 	
	max17040_data.quick_data.vcell_msb = msb;	
	max17040_data.quick_data.vcell_lsb = lsb;		
	if(i==MAX_READ)
	max17040_data.quick_data.quick_vcell = 33996;	
	else
	max17040_data.quick_data.quick_vcell = quick_avalue;	
	mutex_unlock(&max17040_data.data_mutex);	

}
//ps2 team shs : test code
#ifdef MAX17040_DEBUG_QUICK	
static void max17040_quick_get_value(void)
{
	u8 rcomp=0, rcomp_low;
	sleep_dbg("\n=======================================================================\n");	
	sleep_dbg("[INFORMATION] QUICK START STATE [%d]\n",max17040_data.quick_data.quick_state);	      
	sleep_dbg("[INFORMATION] QUICK START SOC_MSB [%d]\n",max17040_data.quick_data.soc_msb);	
	sleep_dbg("[INFORMATION] QUICK START SOC_LSB [%d]\n",max17040_data.quick_data.soc_lsb);		
	sleep_dbg("[INFORMATION] QUICK START SOC [%d]\n",max17040_data.quick_data.quick_soc);	
	sleep_dbg("[INFORMATION] QUICK START REFERENSE SOC [%d]\n",sky_fuelgauge_ref_soc);				
	sleep_dbg("[INFORMATION] QUICK START VCELL_MSB [%d]\n",max17040_data.quick_data.vcell_msb);		
	sleep_dbg("[INFORMATION] QUICK START VCELL_LSB [%d]\n",max17040_data.quick_data.vcell_lsb);	
	sleep_dbg("[INFORMATION] QUICK START VOLTAGE [%d]\n",max17040_data.quick_data.quick_vcell);	
	rcomp=max17040_read_reg(MAX17040_RCOMP_MSB);
	rcomp_low=max17040_read_reg(MAX17040_RCOMP_LSB);	
	sleep_dbg("[INFORMATION] RCOMP 0x[%x][%x]\n",rcomp,rcomp_low);			
	sleep_dbg("\n=======================================================================\n");		
}
#endif
static void max17040_get_vcell(void)
{
	u8 msb;
	u8 lsb;
	int avalue=0;
	int voltage=0;
	dbg_func_in();
	msb = max17040_read_reg(MAX17040_VCELL_MSB);
	lsb = max17040_read_reg(MAX17040_VCELL_LSB);

	//check i2c error
	if(msb < 0 || lsb < 0)
	{
	max17040_data.i2c_state_vol =1;
	}
	else
	{
	max17040_data.i2c_state_vol =0;
	max17040_data.prev_voltage =max17040_data.vcell;
	}
	voltage=(msb<<4)|((lsb&0xf0)>>4);
	avalue=(voltage*125)/100;
//	sleep_dbg(" MSB [%d] : LSB [%d] : LOW VOLTAGE [%d] : VOLTAGE_NOW [%d]\n",msb,lsb,voltage,avalue);
	/* ps2 team shs : voltage changes but the event is not sent.
	//temp code
	if(avalue!=max17040_data.vcell)
	max17040_data.event=Events;
	*/
	mutex_lock(&max17040_data.data_mutex); 	
	max17040_data.vcell = avalue;
	mutex_unlock(&max17040_data.data_mutex);	
	dbg_func_out();	
}
static void max17040_check_power(int msb)
{
	if(max17040_data.slow_poll) //do not call early resume state.
		{
			if(msb<=15) //Battery level is 15% less
			max17040_data.suspend_event=Events;
		}
}
static void max17040_get_soc(void)
{
	u8 msb;
	u8 lsb;
	int avalue=0;
	int soc=0;
	int sky_state=0;
	dbg_func_in();
	msb = max17040_read_reg(MAX17040_SOC_MSB);
	lsb = max17040_read_reg(MAX17040_SOC_LSB);

	//check i2c error
	if(msb<0 ||lsb <0)
	{
	max17040_data.i2c_state =1;
	}
	else
	{
	max17040_data.i2c_state =0;
	max17040_data.prev_soc=max17040_data.soc;
	}

#ifdef MAX17040_DEBUG_QUICK		
	//quick start code
	soc=SKY_MULT_1000(msb)+(SKY_MULT_1000(lsb)/SKY_SOC_LSB);	
	soc=soc/1000;
#else
	/*//description
	read i2c data [msb=20,lsb=10]
	avalue=20*1000+(10*1000)/256
	*/
	avalue=SKY_MULT_1000(msb)+(SKY_MULT_1000(lsb)/SKY_SOC_LSB);	
	//Ajdusted soc%=(SOC%-EMPTY)/(FULL-EMPTY)*100
	#ifdef FEATURE_AT1_FUELGAUGE_CUSTOM
	if(avalue>9000)
		soc=(((avalue-SKY_MULT_100(SKY_SOC_EMPTY))*100)/(SKY_MULT_100(SKY_SOC_FULL)-SKY_MULT_100(SKY_SOC_EMPTY)));
	else if(avalue >= 4100 && avalue<=9000)
		soc=(avalue-3400)/700;
	else
		soc = 0;
	#else
	if(avalue>1200)
		soc=(((avalue-SKY_MULT_100(SKY_SOC_EMPTY))*100)/(SKY_MULT_100(SKY_SOC_FULL)-SKY_MULT_100(SKY_SOC_EMPTY)));
	else 
		soc=0;
	if(avalue >1000 && avalue <1200)
		soc=1;	
	#endif
#endif	
	//logic code
	if(soc>100) //soc>100
		soc=100;
	if(soc==100)
	charge_state=1;
	else
	charge_state=0;		
#ifdef CONFIG_SKY_CHARGING
	if(pm8058_chg_nobattery_factory_cable())
		soc = 10;
#endif
	if(max17040_data.event)
	{
	sleep_dbg("CONFIG CAPACITY [%d] : BATTERY STATS  : [%d]\n",soc,sky_state);
	sleep_dbg("SOC MSB [%d] : LSB [%d] : Lower SOC [%d] : Adjusted SOC [%d] : charge_state [%d] \n",msb,lsb,avalue,soc,charge_state);
	}
	if(soc!=max17040_data.soc)
	max17040_data.event=Events;
	if(soc==0)//ps1 team shs : 0% persent is occured events
	max17040_data.event=Events;
	max17040_check_power(soc);
		
	mutex_lock(&max17040_data.data_mutex); 	
	max17040_data.soc = soc;
	mutex_unlock(&max17040_data.data_mutex);
	dbg_func_out();		
}

static void max17040_get_version(void)
{
	u8 msb;
	u8 lsb;

	msb = max17040_read_reg(MAX17040_VER_MSB);
	lsb = max17040_read_reg(MAX17040_VER_LSB);
	dbg("MAX17040 Fuel-Gauge Ver %d%d\n", msb, lsb);
}
/*PS2 TEAM SHS [END]: depend on chip command*/

/*ps2 team shs [START] : depend on test code*/
static ssize_t max17040_show_flag(struct device *dev, struct device_attribute *attr, char *buf)
{
	int enable;
	dbg_func_in();		
	enable = atomic_read(&max17040_data.set_test);
	return sprintf(buf, "%d\n", enable);
}
static ssize_t max17040_store_flag(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	u8 scale = (u8)simple_strtoul(buf, NULL, 10);
	dbg_func_in();			
	dbg("max17040_store_flag => [%d]\n",scale);
	atomic_set(&max17040_data.set_test, scale);	
	dbg_func_out();					
	return count;
}
static ssize_t max17040_start_quickstart(struct device *dev, struct device_attribute *attr, char *buf)
{
	int enable;
	sleep_dbg("QUICK START\n");
	max17040_restart();
	msleep(300); //quick start update time
	enable=max17040_data.quick_data.quick_state;
	return sprintf(buf, "%d\n", enable);
}
#ifdef MAX17040_DEBUG_QUICK			
static ssize_t max17040_get_low_voltage(struct device *dev, struct device_attribute *attr, char *buf)
{
	int enable;
	sleep_dbg("max17040_get_low_voltage START\n");
	max17040_restart();
	msleep(300); //quick start update time
	enable=max17040_data.vcell;
	return sprintf(buf, "%d\n", enable);
}
static ssize_t max17040_get_low_soc(struct device *dev, struct device_attribute *attr, char *buf)
{
	int enable;
	sleep_dbg("max17040_get_low_soc START\n");
	enable=max17040_data.soc;
	return sprintf(buf, "%d\n", enable);
}

#endif


static DEVICE_ATTR(setflag, S_IWUSR | S_IRUGO, max17040_show_flag, max17040_store_flag);
static DEVICE_ATTR(quickstart, S_IWUSR | S_IRUGO, max17040_start_quickstart, NULL);
#ifdef MAX17040_DEBUG_QUICK			
static DEVICE_ATTR(voltage, S_IWUSR | S_IRUGO, max17040_get_low_voltage, NULL);
static DEVICE_ATTR(soc, S_IWUSR | S_IRUGO, max17040_get_low_soc, NULL);
#endif


static struct attribute *max17040_attrs[] = {
	&dev_attr_setflag.attr, //ps2 team shs : add  test filed
	&dev_attr_quickstart.attr, //ps2 team shs : quickstart test filed
#ifdef MAX17040_DEBUG_QUICK			
	&dev_attr_voltage.attr, //ps2 team shs : quickstart test filed
	&dev_attr_soc.attr, //ps2 team shs : quickstart test filed
#endif
	NULL,
};
static struct attribute_group max17040_attr_group = {
	.attrs = max17040_attrs,
};
/*ps2 team shs [END] : depend on test code*/

static void max17040_program_alarm_set(struct max17040_chip *di, int seconds)
{
	ktime_t low_interval = ktime_set(seconds - 10, 0);
	ktime_t slack = ktime_set(20, 0);
	ktime_t next;
	ktime_t finish;	
	dbg_func_in();
	next = ktime_add(di->last_poll, low_interval);
	finish=ktime_add(next, slack);
	alarm_start_range(&di->alarm, next, finish);
	dbg_func_out();	
}
static void max17040_schedule(void)
{
	switch(max17040_data.online)
		{
		case 0 : //usb not connected
			if(max17040_data.slow_poll) //usb not connected and suspend
			{
			sleep_dbg("CONFIG SLOW_POLL [The alarm is set for [%d] seconds ]\n",SLOW_POLL);
			max17040_program_alarm_set(&max17040_data, SLOW_POLL);		
			}
			else
			{
			sleep_dbg("CONFIG FAST_POLL [The alarm is set for [%d] seconds]\n",FAST_POLL);	
			max17040_program_alarm_set(&max17040_data, FAST_POLL);
			}			
			break;
		case 1 : //usb connection
			sleep_dbg("CONFIG FAST_POLL [The alarm is set for [%d] seconds]\n",FAST_POLL);	
			max17040_program_alarm_set(&max17040_data, FAST_POLL);			
			break;
		default:	
			sleep_dbg("CONFIG [Time setting is not]\n");				
			break;			
		}
}

static void max17040_work(struct work_struct *work)
{
#ifdef MAX17040_ALARM_RTC_ENABLE
	struct max17040_chip *di =
		container_of(work, struct max17040_chip, monitor_work);
#else
	struct max17040_chip *di = container_of(work, struct max17040_chip, work.work);
#endif //MAX17040_ALARM_RTC_ENABLE
	unsigned long flags;	
	int enable;	
	dbg_func_in();
	max17040_data.event=NoEvents;
	max17040_data.suspend_event=NoEvents;
	
#ifndef MAX17040_ALARM_RTC_ENABLE	
//	sleep_dbg("MAX17040_WORK CALL.\n");				
	//prevent suspend
	max17040_prevent_suspend();
#else
//	sleep_dbg("MAX17040_WORK RTC CALL.\n");				
#endif //MAX17040_ALARM_RTC_ENABLE

	//read voltage now
	max17040_get_vcell();

	//read soc 
	max17040_get_soc();
#ifdef MAX17040_DEBUG_QUICK	
	max17040_quick_get_value();	
#endif
	enable = atomic_read(&max17040_data.set_test);
	//save volate now and soc [TEST APPLICATION]
	if(enable)
	{	
		if(max17040_data.event) // ps2 team shs : soc is changed 
			{	
			sleep_dbg("MAX17040 SET TEST.\n");						
			sleep_dbg("MAX17040_WORK SOC [%d] prev[%d] : voltage [%d] : charger_state [%d] : i2c state [%d]\n",max17040_data.soc,max17040_data.prev_soc, max17040_data.vcell,charge_state,max17040_data.i2c_state);							
			input_report_abs(max17040_data.max17040_input_data, ABS_X, max17040_data.vcell);
			input_report_abs(max17040_data.max17040_input_data, ABS_Y, max17040_data.soc);
		    input_report_abs(max17040_data.max17040_input_data, ABS_WAKE, enable);		
			input_sync(max17040_data.max17040_input_data);	
			}

	}
	//ps2 team shs : After you determine the value of voltage and soc If there are changes to the event generates.
	if(max17040_data.slow_poll) //do not call early resume state.
	{
		if(max17040_data.suspend_event)// 15 percent below where they were soc.
			{
				dbg("low battery [%d] percent!!!!!!!\n",max17040_data.soc);				
				power_supply_changed(&di->battery);	
				sleep_dbg("[SLEEP_EVENT] 15 percent below Send Event [%d].\n",max17040_data.soc );				
			}
		else	//15 percent up where were soc.
			{	
				power_supply_changed(&di->battery);					
				sleep_dbg("[SLEEP_EVENT] 15 percent up Send Event [%d].\n",max17040_data.soc );				
			}

	}
	else //call early resume state.
	{
		if(max17040_data.event) //Different values soc.
			{
			sleep_dbg("MAX17040_WORK SOC [%d] prev[%d] : voltage [%d] : charger_state [%d] : i2c state [%d]\n",max17040_data.soc,max17040_data.prev_soc, max17040_data.vcell,charge_state,max17040_data.i2c_state);							
			power_supply_changed(&di->battery);
			}
		else	//same values soc.
			{
			dbg("[EVENT] Stop Event.\n");			
			}
	}
#ifdef MAX17040_ALARM_RTC_ENABLE
	di->last_poll=alarm_get_elapsed_realtime();		
	/* prevent suspend before starting the alarm */
	local_irq_save(flags);	
	max17040_schedule();
	max17040_allow_suspend();			
	local_irq_restore(flags);	
	dbg_func_out();	
#else
	max17040_data.slow_poll = Early_resume;
	max17040_allow_suspend();			
	schedule_delayed_work(&max17040_data.work, MAX17040_DELAY);	
#endif // MAX17040_ALARM_RTC_ENABLE
}
#ifdef MAX17040_ALARM_RTC_ENABLE
static void max_battery_alarm_callback(struct alarm *alarm)
{

	struct max17040_chip *di =
		container_of(alarm, struct max17040_chip, alarm);
	dbg_func_in();
	sleep_dbg("MAX17040_ALARM_CALLBACK CALL.\n");					
	/*enable wake_lock*/
	max17040_prevent_suspend();
	/*schdule workqueue*/
	queue_work(di->monitor_wqueue, &di->monitor_work);
	dbg_func_out();	
}
/*ps2 team shs [START] : depend on early suspend*/

#ifdef CONFIG_HAS_EARLYSUSPEND
void max17040_batt_early_suspend(struct early_suspend *h)
{
	dbg_func_in();
	/* If we are on battery, reduce our update rate until
	 * we next resume.
	 */
	/*ps2 team shs : Runs only when the cable is not connected.*/
	max17040_data.online=sky_get_plug_state();
	max17040_data.slow_poll = Early_suspend;	
	if(!max17040_data.online)
	{
	sleep_dbg("[IMPORT]USB CABLE is not connected\n");				
	max17040_program_alarm_set(&max17040_data, SLOW_POLL);
	sleep_dbg("CONFIG SLOW_POLL [The alarm is set for [%d] seconds ]\n",SLOW_POLL);	
	}
	else
	{
	sleep_dbg("[IMPORT]USB CABLE is connected\n");			
	max17040_program_alarm_set(&max17040_data, FAST_POLL);
	sleep_dbg("CONFIG SLOW_POLL [The alarm is set for [%d] seconds ]\n",FAST_POLL);	
	}
	dbg_func_out();

}
void max17040_batt_late_resume(struct early_suspend *h)
{
	dbg_func_in();
	/* We might be on a slow sample cycle.  If we're
	 * resuming we should resample the battery state
	 * if it's been over a minute since we last did
	 * so, and move back to sampling every minute until
	 * we suspend again.
	 */
	if (max17040_data.slow_poll) {
		max17040_program_alarm_set(&max17040_data, FAST_POLL);
		max17040_data.slow_poll = Early_resume;
		sleep_dbg("CONFIG FAST_POLL [The alarm is set for [%d] seconds]\n",FAST_POLL);	
	}
	dbg_func_out();	
}
#endif// CONFIG_HAS_EARLYSUSPEND
#endif// MAX17040_ALARM_RTC_ENABLE
/*ps2 team shs [END] : depend on early suspend*/


static int __devinit max17040_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
    struct input_dev *input_data = NULL;	
	int ret;
	int aflag=0;
	#ifdef FEATURE_AT1_FUELGAUGE_CUSTOM
	char* rcomp_buf;
	char* soc_buf;
	#endif
	
	sleep_dbg("[MAX17040] max17040_probe [IN]\n");	
	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE))
		return -EIO;

	max17040_data.client = client;
	i2c_set_clientdata(client, &max17040_data);
	//sys file system are registered
	max17040_data.battery.name		= "batterys";
	max17040_data.battery.type		= POWER_SUPPLY_TYPE_BATTERY;
	max17040_data.battery.get_property	= max17040_get_property;
	max17040_data.battery.properties	= max17040_battery_props;
	max17040_data.battery.external_power_changed = max17040_bat_external_power_changed;
	max17040_data.battery.num_properties	= ARRAY_SIZE(max17040_battery_props);
	ret = power_supply_register(&client->dev, &max17040_data.battery);
	if (ret) {
		sleep_dbg("[MAX17040] failed: power supply register [ERROR]\n");			
		i2c_set_clientdata(client, NULL);
		return ret;
	}
	//The code used in the test mode [TEST MODE]	
	ret = sysfs_create_group(&client->dev.kobj, &max17040_attr_group);	
	if (ret) {
		sleep_dbg("[MAX17040] failed: sysfs_create_group  [ERROR]\n");					
	}	
	//mutex is init
	mutex_init(&max17040_data.data_mutex);
	mutex_init(&max17040_data.i2c_mutex);	
	mutex_init(&max17040_data.quick_mutex);		

	#ifdef FEATURE_AT1_FUELGAUGE_CUSTOM
	max17040_unlock();
	max17040_read_reg_bytes(MAX17040_RCOMP_MSB, &rcomp_buf, 4);
	max17040_set_ocv();
	max17040_set_max_rcomp();

	max17040_write_reg_16bytes(0x40);
	max17040_write_reg_16bytes(0x50);
	max17040_write_reg_16bytes(0x60);
	max17040_write_reg_16bytes(0x70);

	msleep(200);
	max17040_set_ocv();
	msleep(200);

	max17040_read_reg_bytes(MAX17040_SOC_MSB, &soc_buf, 2);
	if(soc_buf[0] >= 0x75 || soc_buf[0] <= 0x77)
	{
	    pr_info("succeed to FW update :: fuel_gauge_soc\n");
	}
	else
	{
	    pr_err("failed to FW update :: fuel_gauge_soc\n");
	}
	max17040_restore_backup(MAX17040_RCOMP_MSB, &rcomp_buf, 4);
	max17040_lock();
	#endif
	//rcomp is set
	max17040_set_rcomp();
	//Version of reading
	max17040_get_version();
	//read vell and soc 
	max17040_quick_get_vcell();
	max17040_quick_get_soc();
	//check quick start
	aflag=max17040_check_restart(max17040_data.quick_data.quick_vcell,max17040_data.quick_data.quick_soc);
	if(aflag)
	{
	max17040_restart();	
	msleep(300); //quick start update time
	}
	//The code used in the test mode [TEST MODE]
	atomic_set(&max17040_data.set_test, 0);		
    input_data = input_allocate_device();
  if (!input_data) {
        sleep_dbg("MAX17040: Unable to input_allocate_device \n");  	
		return -1;
    }
	
    set_bit(EV_ABS,input_data->evbit);
    input_set_capability(input_data, EV_ABS, ABS_X);	
    input_set_capability(input_data, EV_ABS, ABS_Y); /* wake */	
    input_set_capability(input_data, EV_ABS, ABS_WAKE); /* wake */
	input_data->name="max17040";
    ret =input_register_device(input_data);
    if (ret) {
        sleep_dbg("MAX17040: Unable to register input_data device\n");
		return -1;
    }
    input_set_drvdata(input_data, &max17040_data);	
	max17040_data.max17040_input_data=input_data;
	//initialize workqueue and alarm setting
	wake_lock_init(&max17040_data.work_wake_lock, WAKE_LOCK_SUSPEND,
			"max17040-battery");


#ifdef MAX17040_ALARM_RTC_ENABLE
	max17040_data.last_poll=alarm_get_elapsed_realtime();	
	INIT_WORK(&max17040_data.monitor_work, max17040_work);
	max17040_data.monitor_wqueue = create_freezeable_workqueue("max17040");
	/* init to something sane */
	if (!max17040_data.monitor_wqueue) {
		sleep_dbg("fail_workqueue Error [PROBE FUNCTION]");
		return -1;
	}
	alarm_init(&max17040_data.alarm, ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP,
			max_battery_alarm_callback);
	//prevent suspend
	max17040_prevent_suspend();
#ifdef CONFIG_HAS_EARLYSUSPEND
	max17040_data.early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN;
	max17040_data.early_suspend.suspend = max17040_batt_early_suspend;
	max17040_data.early_suspend.resume = max17040_batt_late_resume;
	dbg("set max17040 EARLY_SUSPEND\n");	
	register_early_suspend(&max17040_data.early_suspend);
#endif	//CONFIG_HAS_EARLYSUSPEND
	queue_work(max17040_data.monitor_wqueue, &max17040_data.monitor_work);
	
#else
	INIT_DELAYED_WORK_DEFERRABLE(&max17040_data.work, max17040_work);
	schedule_delayed_work(&max17040_data.work, 0);
#endif //MAX17040_ALARM_RTC_ENABLE
	sleep_dbg("[MAX17040] max17040_probe [OUT]\n");	
	return 0;
}

static int __devexit max17040_remove(struct i2c_client *client)
{
	struct max17040_chip *chip = i2c_get_clientdata(client);
	power_supply_unregister(&chip->battery);
	i2c_set_clientdata(client, NULL);
	return 0;
}
static int max17040_sleep_time(int soc)
{
	int time =0;
	if(soc>5)
	time=(soc-5)*SLEEP_ONE_HOUR;		
	else
	time=SLEEP_THREE_MINUTE;
	return time;
}

#ifdef CONFIG_PM
static int max17040_suspend(struct i2c_client *client,
		pm_message_t state)
{
	int time =0;
	int soc=max17040_data.soc;
	dbg_func_in();

	/* If we are on battery, reduce our update rate until
	 * we next resume.
	 */
	/*ps2 team shs : Runs only when the cable is not connected.*/
	max17040_data.online=sky_get_plug_state();
	max17040_data.slow_poll = Early_suspend;	
	//ps1 team shs : cancel schedule_delayer_work
	cancel_delayed_work(&max17040_data.work);	
	//ps1 team shs : set time
	time=max17040_sleep_time(soc);
	msm_pm_set_max_sleep_time((int64_t)((int64_t) time * NSEC_PER_SEC)); 	
	sleep_dbg("[SUSPEND] set time [%d] seconds : soc [%d] ]\n",time,soc);		
	dbg_func_out();
	return 0;
}

static int max17040_resume(struct i2c_client *client)
{
	dbg_func_in();
	if (max17040_data.slow_poll) {
		schedule_delayed_work(&max17040_data.work, 0);		
		sleep_dbg("[RESUME] CONFIG FAST_POLL [The alarm is set for [%d] seconds]\n",FAST_POLL);	
	}
	dbg_func_out();
	return 0;
}

#else

#define max17040_suspend NULL
#define max17040_resume NULL

#endif /* CONFIG_PM */

static const struct i2c_device_id max17040_id[] = {
	{ "max17040", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, max17040_id);

static struct i2c_driver max17040_i2c_driver = {
	.driver	= {
		.name	= "max17040",	
	},
	.probe		= max17040_probe,
	#ifndef MAX17040_ALARM_RTC_ENABLE
	.resume = max17040_resume,
	.suspend = max17040_suspend,
	#endif
	.remove		= __devexit_p(max17040_remove),
	.id_table	= max17040_id,
};

static int __init max17040_init(void)
{
	return i2c_add_driver(&max17040_i2c_driver);
}
module_init(max17040_init);

static void __exit max17040_exit(void)
{
	i2c_del_driver(&max17040_i2c_driver);
}
module_exit(max17040_exit);

MODULE_AUTHOR("Minkyu Kang <mk7.kang@samsung.com>");
MODULE_DESCRIPTION("MAX17040 Fuel Gauge");
MODULE_LICENSE("GPL");
