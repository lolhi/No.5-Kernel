/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
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

#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <media/msm_camera.h>
#include <mach/gpio.h>
#include "mt9v113.h"
#include <linux/regulator/consumer.h>
#include <linux/regulator/machine.h>

//#include <mach/vreg.h>

#define F_MT9V113_POWER
#define F_ICP_HD_STANDBY

/* Micron S5K6AAFX13 Registers and their values */
#define SENSOR_DEBUG 0

#define ON  1
#define OFF 0

#define SENSOR_RESET 137
#define SENSOR_STANDBY 139
#ifdef F_ICP_HD_STANDBY
#define SENSOR_RESET_8M 106
#define SENSOR_STANDBY_8M 57
#define SENSOR_8M_RESET 106
#endif

#define mt9v113_delay_msecs_stream 100//200 //500
#define MT9V113_I2C_RETRY	10
#define MT9V113_I2C_MPERIOD	200

struct mt9v113_work {
	struct work_struct work;
};

static struct  mt9v113_work *mt9v113_sensorw;
static struct  i2c_client *mt9v113_client;

struct mt9v113_ctrl_t {
	const struct msm_camera_sensor_info *sensordata;
};


static struct mt9v113_ctrl_t *mt9v113_ctrl;

static int32_t config_csi;
static bool b_snapshot_flag;

static DECLARE_WAIT_QUEUE_HEAD(mt9v113_wait_queue);
DECLARE_MUTEX(mt9v113_sem);
static int16_t mt9v113_effect = CAMERA_EFFECT_OFF;


struct mt9v113_vreg_t {
	const char *name;
	unsigned short mvolt;
};

#ifdef F_ICP_HD_STANDBY
static struct regulator *l2b_2p8v_8m;
static struct regulator *mvs0b_1p8v_8m;
static struct regulator *s2b_1p2v_8m;
static struct regulator *l3b_2p8v_8m;
#endif

#ifdef F_MT9V113_POWER
static struct regulator *lvs1b_1p8v;
static struct regulator *lvs3b_1p8v;
static struct regulator *l15a_2p8v;
#endif

static int32_t mt9v113_i2c_read(unsigned short   saddr,unsigned short raddr, unsigned short *rdata, enum mt9v113_width width);

/*=============================================================*/
#ifdef F_ICP_HD_STANDBY
static int icp_hd_vreg_init()
{
	int rc = 0;
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);

	s2b_1p2v_8m = regulator_get(NULL, "8901_s2");
	if (IS_ERR(s2b_1p2v_8m)) {
		CAM_ERR("regulator_get s2b_1p2v fail : 0x%x\n", s2b_1p2v_8m);
		return -ENODEV;
	}
	rc = regulator_set_voltage(s2b_1p2v_8m, 1300000, 1300000);
	if (rc) {
		CAM_ERR("%s: unable to set s2b_1p2v voltage to 1.2V\n", __func__);
		goto fail;
	}

	lvs3b_1p8v = regulator_get(NULL, "8901_lvs3");
	if (IS_ERR(lvs3b_1p8v)) {
		CAM_ERR("regulator_get lvs3b_1p8v : 0x%x fail\n", lvs3b_1p8v);
		return -ENODEV;
	}

	mvs0b_1p8v_8m = regulator_get(NULL, "8901_mvs0");
	if (IS_ERR(mvs0b_1p8v_8m)) {
		CAM_ERR("regulator_get mvs0b_1p8v : 0x%x fail\n", mvs0b_1p8v_8m);
		return -ENODEV;
	}
	
	l2b_2p8v_8m = regulator_get(NULL, "8901_l2");
	if (IS_ERR(l2b_2p8v_8m)) {
		CAM_ERR("regulator_get l2b_2p8v : 0x%x fail\n", l2b_2p8v_8m);
		return -ENODEV;
	}

	rc = regulator_set_voltage(l2b_2p8v_8m, 2800000, 2800000);
	if (rc) {
		CAM_ERR("%s: unable to set l2b_2p8v voltage to 2.8V\n", __func__);
		goto fail;
	}

	l3b_2p8v_8m = regulator_get(NULL, "8901_l3");
	if (IS_ERR(l3b_2p8v_8m)) {
		CAM_ERR("regulator_get l3b_2p8v : 0x%x fail\n", l3b_2p8v_8m);
		return -ENODEV;
	}
	rc = regulator_set_voltage(l3b_2p8v_8m, 2800000, 2800000);
	if (rc) {
		CAM_ERR("%s: unable to set l3b_2p8v voltage to 2.8V\n", __func__);
		goto fail;
	}
	
	//CAM_INFO("%s %s Success!:%d\n", __FILE__, __func__, __LINE__);
	return rc;
fail:
	//CAM_INFO("%s %s Failed!:%d\n", __FILE__, __func__, __LINE__);
	if(l2b_2p8v_8m) {
		regulator_put(l2b_2p8v_8m);
	}
	if(s2b_1p2v_8m) {
		regulator_put(s2b_1p2v_8m);
	}
	if(l3b_2p8v_8m) {
		regulator_put(l3b_2p8v_8m);
	}
	return rc;	
}

static int icp_hd_power(int on)
{
	int rc = 0;
	int status=0;
		//CAM_INFO("%s %s:%d power = %d\n", __FILE__, __func__, __LINE__,on);
	if(on) {
		//standby control
		rc = gpio_tlmm_config(GPIO_CFG(SENSOR_STANDBY_8M, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
		if (!rc) {
			//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
			gpio_set_value(SENSOR_STANDBY_8M,0);					
		}
		
		rc = regulator_enable(s2b_1p2v_8m);
		if (rc) {
			CAM_ERR("%s: Enable regulator s2b_1p2v failed\n", __func__);
			goto fail;
		}
		msleep(1);
		
		rc = regulator_enable(mvs0b_1p8v_8m);
		if (rc) {
			CAM_ERR("%s: Enable regulator mvs0b_1p8v failed\n", __func__);
			goto fail;
		}
		msleep(1);
		
		rc = regulator_enable(lvs3b_1p8v);
		if (rc) {
			CAM_ERR("%s: Enable regulator lvs3b_1p8v failed\n", __func__);
			goto fail;
		}		
		msleep(1);
		
		rc = regulator_enable(l2b_2p8v_8m);
		if (rc) {
			CAM_ERR("%s: Enable regulator l2b_2p8v failed\n", __func__);
			goto fail;
		}
		msleep(1);
		
		rc = regulator_enable(l3b_2p8v_8m);
		if (rc) {
			CAM_ERR("%s: Enable regulator l3b_2p8v failed\n", __func__);
			goto fail;
		}

		//CAM_INFO("%s %s ON Success:%d\n", __FILE__, __func__, __LINE__);
	}
	else {
		//CAM_INFO("%s %s:%d power \n", __FILE__, __func__, __LINE__);
		//CAM_INFO("%s %s:%d power \n", __FILE__, __func__, __LINE__);
		if(1)//mvs0b_1p8v) 
		{
			rc = regulator_disable(mvs0b_1p8v_8m);
			if (rc){
				CAM_ERR("%s: Disable regulator mvs0b_1p8v failed\n", __func__);
				goto fail;
			}
		}
		//CAM_INFO("%s %s:%d power \n", __FILE__, __func__, __LINE__);
		if(1)//l2b_2p8v) 
		{
			rc = regulator_disable(l2b_2p8v_8m);
			if (rc){
				CAM_ERR("%s: Disable regulator l2b_2p8v failed\n", __func__);
				goto fail;
			}
			regulator_put(l2b_2p8v_8m);
		}
		//CAM_INFO("%s %s:%d power \n", __FILE__, __func__, __LINE__);
		if(1)//l3b_2p8v) 
		{
		rc = regulator_disable(l3b_2p8v_8m);
			if (rc){
				CAM_ERR("%s: Disable regulator l3b_2p8v failed\n", __func__);
				goto fail;
			}
			regulator_put(l3b_2p8v_8m);		
		}
		//CAM_INFO("%s %s OFF Success:%d\n", __FILE__, __func__, __LINE__);
	}
	
	return rc;
fail:
	CAM_ERR("%s %s Failed!:%d\n", __FILE__, __func__, __LINE__);
	if(l2b_2p8v_8m){
		regulator_put(l2b_2p8v_8m);
	}
	if(s2b_1p2v_8m){
		regulator_put(s2b_1p2v_8m);
	}
	if(l3b_2p8v_8m){
		regulator_put(l3b_2p8v_8m);
	}
	return rc;			
}

static int icp_hd_reset(int set)
{
	int rc = 0;

	rc = gpio_tlmm_config(GPIO_CFG(SENSOR_RESET_8M, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);

	if (!rc) {
		//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
		gpio_set_value(SENSOR_RESET_8M,0);		
		if(set){
			gpio_set_value(SENSOR_RESET_8M,1);		
		}
	}
	else{
		CAM_ERR("icp_hd_reset gpio_tlmm_config Failed!\n");
		goto reset_fail;
	}

	//CAM_INFO("%s %s Success:%d\n", __FILE__, __func__, __LINE__);
	return rc;

reset_fail:
	CAM_ERR("%s %s Failed!:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}
#endif

#ifdef F_MT9V113_POWER
static int mt9v113_vreg_init()
{
	int rc;
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);

	lvs3b_1p8v = regulator_get(NULL, "8901_mvs0");
	if (IS_ERR(lvs3b_1p8v)) {
		CAM_ERR("regulator_get lvs3b_1p8v fail\n" );;
		return -ENODEV;
	}
	
	lvs1b_1p8v = regulator_get(NULL, "8901_lvs1");
	if (IS_ERR(lvs1b_1p8v)) {
		CAM_ERR("regulator_get lvs1b_1p8v fail\n" );;
		return -ENODEV;
	}

	l15a_2p8v = regulator_get(NULL, "8058_l15");
	if (IS_ERR(l15a_2p8v)) {
		CAM_ERR("regulator_get l15a_2p8v fail\n" );;
		return -ENODEV;
	}	
	rc = regulator_set_voltage(l15a_2p8v, 2800000, 2800000);
	if (rc) {
		CAM_ERR("%s: unable to set l15a_2p8v voltage to 2.8V\n", __func__);
		goto fail;
	}
	
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
fail:
	CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	if(l15a_2p8v) {
	regulator_put(l15a_2p8v);
	}
	return rc;	
}
static int mt9v113_power(int on)
{
	int rc = 0;
	int status = 0;
	//CAM_INFO("%s %s:%d power = %d\n", __FILE__, __func__, __LINE__,on);
	if(on) {
		rc = regulator_enable(lvs3b_1p8v);
		if (rc) {
			CAM_ERR("%s: Enable regulator lvs3b_1p8v failed\n", __func__);
			goto fail;
		}
		msleep(1);
		rc = regulator_enable(lvs1b_1p8v);
		if (rc) {
			CAM_ERR("%s: Enable regulator lvs1b_1p8v failed\n", __func__);
			goto fail;
		}
		msleep(1);
		rc = regulator_enable(l15a_2p8v);
		if (rc) {
			CAM_ERR("%s: Enable regulator l15a_2p8v failed\n", __func__);
			goto fail;
		}
		msleep(1);
	}
	else {
		rc = regulator_disable(l15a_2p8v);
		if (rc)
			CAM_INFO("%s: Disable regulator l15a_2p8v failed\n", __func__);
		regulator_put(l15a_2p8v);
		
		rc = regulator_disable(lvs3b_1p8v);
		if (rc)
			CAM_ERR("%s: Disable regulator lvs3b_1p8v failed\n", __func__);
		rc = regulator_disable(lvs1b_1p8v);
		if (rc)
			CAM_ERR("%s: Disable regulator lvs1b_1p8v failed\n", __func__);
	}
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);

	status = gpio_tlmm_config(GPIO_CFG(SENSOR_RESET, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);

	if (!status) 
    {
		//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
		gpio_set_value(SENSOR_RESET,0);
	}
	
	return rc;
fail:
	CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	if(l15a_2p8v)
		regulator_put(l15a_2p8v);
	return rc;			
}
#endif

static int mt9v113_reset(int set)//const struct msm_camera_sensor_info *dev)
{
	int rc = 0;

	//standby
	rc = gpio_tlmm_config(GPIO_CFG(SENSOR_RESET, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);

	if (!rc) {
		//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
		gpio_set_value(SENSOR_RESET,0);
		//rc = gpio_direction_output(137, 0);
		mdelay(10);  //hhs 20);
		gpio_set_value(SENSOR_RESET,1);
		//rc = gpio_direction_output(137, 1);
	}
	//standby
	rc = gpio_tlmm_config(GPIO_CFG(SENSOR_STANDBY, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);

	if (!rc) {
		//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
		if(set)
			set=0;
		else
			set=1;
		gpio_set_value(SENSOR_STANDBY,set);
		//rc = gpio_direction_output(137, 0);
		mdelay(10);  //hhs 20);		
	}
    CAM_ERR("%s: mt9v113_reset set=%d=\n", __func__, set);
	return rc;
}

static int32_t mt9v113_i2c_txdata(unsigned short saddr,
	unsigned char *txdata, int length)
{
	uint32_t i = 0;
	int32_t rc = 0;
	
	struct i2c_msg msg[] = {
		{
			.addr = saddr,
			.flags = 0,
			.len = length,
			.buf = txdata,
		},
	};

#if SENSOR_DEBUG
	if (length == 2)
		CAM_INFO("msm_io_i2c_w: 0x%04x 0x%04x\n",
			*(u16 *) txdata, *(u16 *) (txdata + 2));
	else if (length == 4)
		CAM_INFO("msm_io_i2c_w: 0x%04x\n", *(u16 *) txdata);
	else
		CAM_INFO("msm_io_i2c_w: length = %d\n", length);
#endif
	for (i = 0; i < MT9V113_I2C_RETRY; i++) {
		rc = i2c_transfer(mt9v113_client->adapter, msg, 1); 
		if (rc >= 0) {			
			return 0;
		}
		CAM_INFO("%s: tx retry. [%02x.%02x.%02x] len=%d rc=%d\n", __func__,saddr, *txdata, *(txdata + 1), length, rc);
		msleep(MT9V113_I2C_MPERIOD);
	}
	return -EIO;	
}

#ifdef F_ICP_HD_STANDBY
static int32_t icp_hd_i2c_write_dw(unsigned short saddr,
	unsigned short waddr, unsigned short wdata1, unsigned short wdata2)
{
	int32_t rc = -EIO;
	unsigned char buf[6];

	memset(buf, 0, sizeof(buf));
	
	buf[0] = (waddr & 0xFF00)>>8;
	buf[1] = (waddr & 0x00FF);
	buf[2] = (wdata1 & 0xFF00)>>8;
	buf[3] = (wdata1 & 0x00FF);
	buf[4] = (wdata2 & 0xFF00)>>8;
	buf[5] = (wdata2 & 0x00FF);

	rc = mt9v113_i2c_txdata(saddr, buf, 6);

	if (rc < 0)
		CAM_ERR(
		"i2c_write failed, addr = 0x%x, val1 = 0x%x, val2 = 0x%x!\n",
		waddr, wdata1, wdata1);

	return rc;
}
#endif

static int32_t mt9v113_i2c_write(unsigned short saddr,
	unsigned short waddr, unsigned short wdata, enum mt9v113_width width)
{
	int32_t rc = -EIO;
	unsigned char buf[4];

	memset(buf, 0, sizeof(buf));
	switch (width) {
	case WORD_LEN: {
		buf[0] = (waddr & 0xFF00)>>8;
		buf[1] = (waddr & 0x00FF);
		buf[2] = (wdata & 0xFF00)>>8;
		buf[3] = (wdata & 0x00FF);

		rc = mt9v113_i2c_txdata(saddr, buf, 4);
	}
		break;

	case TRIPLE_LEN: {
		buf[0] = (waddr & 0xFF00)>>8;
		buf[1] = (waddr & 0x00FF);
		buf[2] = wdata;
		rc = mt9v113_i2c_txdata(saddr, buf, 3);
	}
		break;

	case BYTE_LEN: {
		buf[0] = waddr;
		buf[1] = wdata;
		rc = mt9v113_i2c_txdata(saddr, buf, 2);
	}
		break;

	default:
		break;
	}

	if (rc < 0)
		CAM_ERR(
		"i2c_write failed, addr = 0x%x, val = 0x%x!\n",
		waddr, wdata);

	return rc;
}

static int32_t mt9v113_i2c_write_a2d1(unsigned short waddr, unsigned char wdata)
{
	int32_t rc = -EIO;
	unsigned char buf[3];

	memset(buf, 0, sizeof(buf));

	buf[0] = (waddr & 0xFF00)>>8;
	buf[1] = (waddr & 0x00FF);
	buf[2] = wdata;
	
	rc = mt9v113_i2c_txdata(mt9v113_client->addr, buf, 3);

	if (rc < 0)
		CAM_ERR(
		"i2c_write failed, saddr= 0x%x, addr = 0x%x, val = 0x%x!\n",
		mt9v113_client->addr, waddr, wdata);

	return rc;
}

#define MT9V113_REG_MCU_VAR_ADDR      0x098c
#define MT9V113_REG_MCU_VAR_DATA      0x0990
#define MT9V113_REG_MODEL_ID          0x3000
#define MT9V113_REG_FRAME_CNT         0x303a
#define MT9V113_REG_MCU_DATA_SEQ_CMD  0xa103

#define MT9V113_I2C_RETRY_CNT   3
#define MT9V113_MAX_WAIT_CNT    20
#define MT9V113_POLL_PERIOD_MS  10

static int32_t mt9v113_i2c_write_table(
	struct mt9v113_i2c_reg_conf const *reg_conf_tbl,
	int num_of_items_in_table)
{
	int i;
	int32_t rc = -EIO;
    uint32_t poll_reg = 0;
	uint32_t poll_delay = 0;
	uint32_t poll_retry = 0;
	uint32_t poll_mcu_var = 0;
	uint32_t poll_data = 0;
	uint32_t poll_mask = 0;
	uint32_t retry_cnt = 0;
	uint32_t read_data = 0;
	//OTP 방어코드 추가
	uint32_t otp_retry_cnt = 0;
	uint32_t otp_poll_retry = 20;

	for (i = 0; i < num_of_items_in_table; i++) 
	{	
	//CAM_INFO("hhs addr=%4x, data=%4x\n",reg_conf_tbl->waddr, reg_conf_tbl->wdata);
	switch(reg_conf_tbl->width )
	{
	    case DELAY_T:
		{
			if (reg_conf_tbl->mdelay_time != 0)
				msleep(reg_conf_tbl->mdelay_time);

			reg_conf_tbl++;

			break;
	    }
		case ZERO_LEN:
		{
			//CAM_INFO("ZERO_LEN continue ADDR = 0x%x, VALUE = 0x%x\n",reg_conf_tbl->waddr, reg_conf_tbl->wdata);
			reg_conf_tbl++;			
			break;
		}		
		case POLL_REG:
        {
			poll_reg = reg_conf_tbl->waddr;
			poll_mask = reg_conf_tbl->wdata;	              
			poll_data = (reg_conf_tbl+1)->waddr;
			poll_delay = ((reg_conf_tbl+1)->wdata & 0xff00) >> 8;
			poll_retry = ((reg_conf_tbl+1)->wdata & 0x00ff);              

			//CAM_INFO("POLLING!! poll_delay=%x, poll_retry=%x, poll_mcu_var=%x, poll_data=%x, poll_mask=%x\n",poll_delay, poll_retry, poll_mcu_var, poll_data, poll_mask);

            //poll_retry=poll_retry*4;
			//poll_delay=poll_delay*4;
			for (retry_cnt = 0; retry_cnt < poll_retry; retry_cnt++)
			{
				if (mt9v113_i2c_read(mt9v113_client->addr,poll_reg, &read_data,WORD_LEN) < 0)
					//mt9v113_i2c_read_word(poll_reg, &read_data) == FALSE)
				{
					CAM_INFO("<<POLL_MCU_VAR mt9v113_i2c_read (FALSE)\n");
					return -1;
				}
				if ((read_data & poll_mask) != poll_data)
				{
					//hhs test CAM_INFO("retry polling MCU variable... after sleeping %d ms, read_data=%2x\n", poll_delay, read_data);
					//CAM_INFO("(read_data & poll_mask)=%4x , poll_data=%4x\n", (read_data & poll_mask), poll_data);
					msleep(poll_delay);
				}
				else
				{
					//CAM_INFO("stop polling MCU variable... retried %d/%d time(s) (delay = %d ms), read_data=%2x\n", retry_cnt, poll_retry, poll_delay, read_data);
				    break;
				}
			}

			if (retry_cnt == poll_retry)
			{
				CAM_INFO("<<RETRY FAIL!! POLL_REG read_data = %x (FALSE)\n", read_data);
			    return -1;
			}

			//  2개의 값을 이용하므로 +2를 해준다
			reg_conf_tbl++;
			reg_conf_tbl++;
			break;
		}
		case POLL_MCU_VAR: //polling을 빠져나오는 조건이 정해준 값과 같을 경우
        {
			poll_mcu_var = reg_conf_tbl->waddr;
			poll_mask = reg_conf_tbl->wdata;	              
			poll_data = (reg_conf_tbl+1)->waddr;
			poll_delay = ((reg_conf_tbl+1)->wdata & 0xff00) >> 8;
			poll_retry = ((reg_conf_tbl+1)->wdata & 0x00ff);              

			CAM_INFO("POLLING!! poll_delay=%x, poll_retry=%x, poll_mcu_var=%x, poll_data=%x, poll_mask=%x\n",poll_delay, poll_retry, poll_mcu_var, poll_data, poll_mask);

			for (retry_cnt = 0; retry_cnt < poll_retry; retry_cnt++)
			{
				if (mt9v113_i2c_write(mt9v113_client->addr,MT9V113_REG_MCU_VAR_ADDR, poll_mcu_var,WORD_LEN) < 0)
					//mt9p111_i2c_write_word(MT9P111_REG_MCU_ADDR, poll_mcu_var) < 0)
				{
					CAM_INFO("<<POLL_MCU_VAR mt9p111_i2c_write_word (FALSE)\n");
					return -1;
				}
				if (mt9v113_i2c_read(mt9v113_client->addr,MT9V113_REG_MCU_VAR_DATA, &read_data,WORD_LEN) < 0)
					//mt9p111_i2c_read_word(MT9P111_REG_MCU_DATA, &read_data) < 0)
				{
					CAM_INFO("<<POLL_MCU_VAR mt9p111_i2c_read_word (FALSE)\n");
					return -1;
				}

				if ((read_data & poll_mask) != poll_data)
				{
					//hhs test CAM_INFO("retry polling MCU variable... after sleeping %d ms, read_data=%2x\n", poll_delay, read_data);
					msleep(poll_delay);
				}
				else
				{
					//CAM_INFO("stop polling MCU variable... retried %d/%d time(s) (delay = %d ms), read_data=%2x\n", retry_cnt, poll_retry, poll_delay, read_data);
				break;
				}
			}

			if (retry_cnt == poll_retry)
			{
				CAM_INFO("<<RETRY FAIL!! POLL_MCU_VAR read_data = %x (FALSE)\n", read_data);
			    return -1;
			}

			//  2개의 값을 이용하므로 +2를 해준다
			reg_conf_tbl++;
			reg_conf_tbl++;
			break;
		}
		default:
		{
			rc = mt9v113_i2c_write(mt9v113_client->addr,
								reg_conf_tbl->waddr, reg_conf_tbl->wdata,
								reg_conf_tbl->width);
			//CAM_INFO("I2C WRITE!!! ADDR = 0x%x, VALUE = 0x%x, width = %d, num_of_items_in_table=%d, i=%d\n",
			//	reg_conf_tbl->waddr, reg_conf_tbl->wdata, reg_conf_tbl->width, num_of_items_in_table, i);

			if (rc < 0)
			{
				CAM_ERR("mt9v113_i2c_read failed!\n");
				return rc;
			}
			
			if (reg_conf_tbl->mdelay_time != 0)
				msleep(reg_conf_tbl->mdelay_time);

			reg_conf_tbl++;

			break;
		}			
	}	
	}

	return rc;
}


static int mt9v113_i2c_rxdata(unsigned short saddr,
	unsigned char *rxdata, int length)
{
	uint32_t i = 0;
	int32_t rc = 0;
	
	struct i2c_msg msgs[] = {
	{
		.addr   = saddr,
		.flags = 0,
		.len   = 2,
		.buf   = rxdata,
	},
	{
		.addr   = saddr,
		.flags = I2C_M_RD,
		.len   = length,
		.buf   = rxdata,
	},
	};

#if SENSOR_DEBUG
	if (length == 2)
		CAM_INFO("msm_io_i2c_r: 0x%04x 0x%04x\n",
			*(u16 *) rxdata, *(u16 *) (rxdata + 2));
	else if (length == 4)
		CAM_INFO("msm_io_i2c_r: 0x%04x\n", *(u16 *) rxdata);
	else
		CAM_INFO("msm_io_i2c_r: length = %d\n", length);
#endif

	for (i = 0; i < MT9V113_I2C_RETRY; i++) {
		rc = i2c_transfer(mt9v113_client->adapter, msgs, 2); 
		if (rc >= 0) {			
			return 0;
		}
		CAM_INFO("%s: tx retry. [%02x.%02x.%02x] len=%d rc=%d\n", __func__,saddr, *rxdata, *(rxdata + 1), length, rc);
		msleep(MT9V113_I2C_MPERIOD);
	}
	return -EIO;
}

static int32_t mt9v113_i2c_read(unsigned short   saddr,
	unsigned short raddr, unsigned short *rdata, enum mt9v113_width width)
{
	int32_t rc = 0;
	unsigned char buf[4];

	if (!rdata)
		return -EIO;

	memset(buf, 0, sizeof(buf));

	switch (width) {
	case WORD_LEN: {
		buf[0] = (raddr & 0xFF00)>>8;
		buf[1] = (raddr & 0x00FF);

		rc = mt9v113_i2c_rxdata(saddr, buf, 2);
		if (rc < 0)
			return rc;

		*rdata = buf[0] << 8 | buf[1];
	}
		break;

	default:
		break;
	}

	if (rc < 0)
		CAM_ERR("mt9v113_i2c_read failed!\n");

	return rc;
}

static int32_t mt9v113_set_lens_roll_off(void)
{
	int32_t rc = 0;
#if 0
	rc = s5k6aafx13_i2c_write_table(&s5k6aafx13_regs.rftbl[0],
								 s5k6aafx13_regs.rftbl_size);
#endif
	return rc;
}

static long mt9v113_reg_init(void)
{
	int32_t array_length;
	int32_t i;
	long rc;

    #if 1    
	/* PLL Setup Start */
	rc = mt9v113_i2c_write_table(&mt9v113_regs.init_parm[0],
					mt9v113_regs.init_parm_size);
    #else
	//{0x0018,0x4028, WORD_LEN, 0}, 	// standby_control
	rc = mt9v113_i2c_write(mt9v113_client->addr,0x0018, 0x4028,WORD_LEN);
	//CAM_INFO("I2C WRITE!!! ADDR = 0x%x, VALUE = 0x%x, width = %d, num_of_items_in_table=%d, i=%d\n",
	//	reg_conf_tbl->waddr, reg_conf_tbl->wdata, reg_conf_tbl->width, num_of_items_in_table, i);
	#endif
	if (rc < 0)
	{
		CAM_ERR("mt9v113_i2c_write failed!\n");
		return rc;
	}
	else
	{
	    CAM_INFO("--hhs mt9v113_i2c_write sucessed \n");
	}
	if (rc < 0)
		return rc;
	/* PLL Setup End   */

	return 0;
}

#ifdef F_SKYCAM_FIX_CFG_EFFECT
static long mt9v113_set_effect(int mode, int effect)
{
	uint16_t reg_addr;
	uint16_t reg_val;
	long rc = 0;

	//CAM_INFO("%s start\n",__func__);

	if(effect < CAMERA_EFFECT_OFF || effect >= CAMERA_EFFECT_MAX){
		CAM_ERR("%s error. effect=%d\n", __func__, effect);
		return 0;//-EINVAL;
	}

	rc = mt9v113_i2c_write_table(mt9v113_regs.effect_cfg_settings[effect],
					mt9v113_regs.effect_cfg_settings_size);
	if (rc < 0)
	{
		CAM_INFO("CAMERA_WB I2C FAIL!!! return~~\n");
		return rc;
	}	
	//CAM_INFO("%s end\n",__func__);
	return rc;
}
#endif

#ifdef F_SKYCAM_FIX_CFG_BRIGHTNESS
static int32_t mt9v113_set_brightness(int8_t brightness)
{
	int32_t rc = 0;
	int i = 0;
	//CAM_INFO("%s start~ receive brightness = %d\n",__func__, brightness);

	if ((brightness < C_SKYCAM_MIN_BRIGHTNESS) || (brightness > C_SKYCAM_MAX_BRIGHTNESS)) {
		CAM_ERR("%s error. brightness=%d\n", __func__, brightness);
		return 0;//-EINVAL;
	}

	rc = mt9v113_i2c_write_table(mt9v113_regs.bright_cfg_settings[brightness],	
					mt9v113_regs.bright_cfg_settings_size);
	if (rc < 0)
	{
		CAM_INFO("CAMERA_BRIGHTNESS I2C FAIL!!! return~~\n");
		return rc;
	}
	//CAM_INFO("%s end\n",__func__);
	return rc;
}
#endif

#ifdef F_SKYCAM_FIX_CFG_WB
static int32_t mt9v113_set_whitebalance (int32_t whitebalance)
{
	
	int32_t rc = 0;
	int8_t m_wb = 0;
		
	//CAM_INFO("%s start  whitebalance=%d\n",__func__, whitebalance);

	rc = mt9v113_i2c_write_table(mt9v113_regs.wb_cfg_settings[whitebalance-1],
					mt9v113_regs.wb_cfg_settings_size);
	if (rc < 0)
	{
		CAM_INFO("CAMERA_WB I2C FAIL!!! return~~\n");
		return rc;
	}		

	//CAM_INFO("%s end\n",__func__);
	return rc;
}
#endif

#ifdef F_SKYCAM_FIX_CFG_EXPOSURE
static int32_t mt9v113_set_exposure_mode(int32_t exposure)
{
	int32_t rc = 0;

	//CAM_INFO("%s  exposure = %d\n",__func__, exposure);

	if ((exposure < 0) || (exposure >= 4))
	{
		CAM_ERR("%s FAIL!!! return~~  exposure = %d\n",__func__,exposure);
		return 0;//-EINVAL;
	}

	rc = mt9v113_i2c_write_table(mt9v113_regs.exposure_cfg_settings[exposure],
					mt9v113_regs.exposure_cfg_settings_size);
	if (rc < 0)
	{
		CAM_INFO("CAMERA_EFFECT_SEPIA I2C FAIL!!! return~~\n");
		return rc;
	}		
	
	//CAM_INFO("%s end\n",__func__);

	return rc;
}
#endif

#ifdef F_SKYCAM_FIX_CFG_PREVIEW_FPS
#define MT9V113_MAX_PREVIEW_FPS 5
static int32_t mt9v113_set_preview_fps(int32_t preview_fps)
{
	/* 0 : variable 30fps, 1 ~ 30 : fixed fps */
	/* default: variable 7 ~ 30fps */
	int32_t rc = 0;	

    #if 1 //hhs 20110502 test
	if ((preview_fps < C_SKYCAM_MIN_PREVIEW_FPS) || (preview_fps > C_SKYCAM_MAX_PREVIEW_FPS)) {
		CAM_INFO("%s: -EINVAL, preview_fps=%d\n", 
			__func__, preview_fps);
		return -EINVAL;
	}

	//limit actually max frame rate
	if((preview_fps > MT9V113_MAX_PREVIEW_FPS) && (preview_fps < C_SKYCAM_MAX_PREVIEW_FPS))
		preview_fps = MT9V113_MAX_PREVIEW_FPS;

	CAM_INFO("%s: preview_fps=%d\n", __func__, preview_fps);

	rc = mt9v113_i2c_write_table(&mt9v113_regs.preview_fps_cfg_settings[preview_fps - C_SKYCAM_MIN_PREVIEW_FPS],
					mt9v113_regs.preview_fps_cfg_settings_size);

	CAM_INFO("%s end rc = %d\n",__func__, rc);
    #endif
	return rc;
}
#endif

#ifdef F_SKYCAM_FIX_CFG_REFLECT
static int32_t mt9v113_set_reflect(int32_t reflect)
{
	int32_t rc = 0;
	int32_t i = 0;
	int8_t npolling = -1;

	//CAM_INFO("%s  reflect = %d\n",__func__, reflect);

	if ((reflect < 0) || (reflect >= 4))
	{
		CAM_INFO("%s FAIL!!! return~~  reflect = %d\n",__func__,reflect);
		return 0;//-EINVAL;
	}
	rc = mt9v113_i2c_write_table(mt9v113_regs.reflect_cfg_settings[reflect],
				mt9v113_regs.reflect_cfg_settings_size);
	if (rc < 0)
	{
		CAM_ERR("CAMERA_SET_REFLECT I2C FAIL!!! return~~\n");
		return rc;
	}		
	
	//CAM_INFO("%s end\n",__func__);

	return rc;
}
#endif

static int32_t mt9v113_video_config(void)
{
	int32_t rc = 0;
	
	//CAM_INFO("%s start\n",__func__);

	rc = mt9v113_i2c_write_table(&mt9v113_regs.preview_cfg_settings[0],
						mt9v113_regs.preview_cfg_settings_size);

	if (rc < 0)
	{
		CAM_ERR("mt9v113_i2c_write_table FAIL!!! return~~\n");
		return rc;
	}
	//CAM_INFO("%s end rc = %d\n",__func__, rc);

	return rc;
}

static int32_t mt9v113_snapshot_config(void)
{
	int32_t rc = 0;

    return;
	/* set snapshot resolution to 1280x960 */
	//CAM_INFO("%s start\n",__func__);
	rc = mt9v113_i2c_write_table(&mt9v113_regs.snapshot_cfg_settings[0],
					mt9v113_regs.snapshot_cfg_settings_size);
	if (rc < 0)
	{
		CAM_ERR("mt9v113_i2c_write_table FAIL!!! return~~\n");
		return rc;
	}
	//CAM_INFO("%s end rc = %d\n",__func__, rc);
	
	return rc;
}


static uint8_t once=0;
static long mt9v113_set_sensor_mode(int mode)
{
	uint16_t clock;
	long rc = 0;
       struct msm_camera_csi_params mt9v113_csi_params;
	   
	//CAM_INFO("mt9v113_set_sensor_mode mode = %d!\n",mode);

	switch (mode) {
	case SENSOR_PREVIEW_MODE:
		b_snapshot_flag = false;
		if(config_csi == 0)
		{
			CAM_INFO("mt9v113_set_sensor_mode config_csi E\n");
			mt9v113_csi_params.lane_cnt = 1; // 4;
			mt9v113_csi_params.data_format = CSI_8BIT; //CSI_10BIT;
			mt9v113_csi_params.lane_assign = 0xe4;
			mt9v113_csi_params.dpcm_scheme = 0;
			mt9v113_csi_params.settle_cnt = 0x14;
			rc = msm_camio_csi_config(&mt9v113_csi_params);
			msleep(mt9v113_delay_msecs_stream);
			config_csi = 1;
			CAM_INFO("mt9v113_set_sensor_mode config_csi X\n");
			mt9v113_set_reflect(0);
			mt9v113_set_preview_fps(5);
		}
		#if 1
		else
		{
		if(once==0)
		{
			rc = mt9v113_video_config();
			CAM_INFO("mt9v113_video_config2, rc = %d \n", rc);
			if (rc < 0)
		    {
			    CAM_ERR("mt9v113_video_config FAIL!!! return~~\n");
			    return rc;
			}
			once++;
			b_snapshot_flag = false;
		}
		}
		#endif
		break;
		
	case SENSOR_SNAPSHOT_MODE:
		if(!b_snapshot_flag) {
		rc = mt9v113_snapshot_config();
		b_snapshot_flag = true;
		once=0;
		CAM_INFO("mt9v113_snapshot_config, rc = %d \n", rc);
		if (rc < 0)
              {
			CAM_ERR("mt9v113_snapshot_config FAIL!!! return~~\n");
			return rc;
			}			
		}			
		break;

	case SENSOR_RAW_SNAPSHOT_MODE:
		
		break;

	default:
		return -EINVAL;
	}
	CAM_INFO("mt9v113_set_sensor_mode X!\n",mode);
	
	return rc;
}

static int mt9v113_sensor_init_probe(const struct msm_camera_sensor_info *data)
{
	uint16_t model_id = 0;
	int rc = 0;

	CAM_INFO("init entry \n");
	rc = mt9v113_reset(ON);
	if (rc < 0) {
		CAM_ERR("reset failed!\n");
		goto init_probe_fail;
	}

	msleep(5);//mdelay(5);
#if 1
	CAM_INFO("mt9v113_reg_init E \n");
	rc = mt9v113_reg_init();
	CAM_INFO("mt9v113_reg_init X \n");
	if (rc < 0)
		goto init_probe_fail;
#endif
	return rc;

init_probe_fail:
	return rc;
}

int mt9v113_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	config_csi = 0;
	b_snapshot_flag = false;
	once=0;
	CAM_INFO(" mt9v113_sensor_init E\n");
#ifdef F_ICP_HD_STANDBY
	icp_hd_vreg_init();
	rc = icp_hd_power(ON);
	if (rc) {		
		CAM_ERR(" icp_hd_power failed rc=%d\n",rc);
		goto init_fail; 
	}
	//msm_camio_clk_rate_set(12000000);
	msm_camio_clk_rate_set(24000000);
	icp_hd_reset(ON);
	msleep(5);//mdelay(5);

	icp_hd_i2c_write_dw(0x7A>>1, 0xf03c,  0x0009, 0x5000);
	icp_hd_i2c_write_dw(0x7A>>1, 0xe000,  0x0000, 0x0000);
	icp_hd_i2c_write_dw(0x7A>>1, 0xe004,  0x0000, 0x8fff);
	
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	gpio_set_value(SENSOR_STANDBY_8M,1);					
	
	if(s2b_1p2v_8m) {
		rc = regulator_disable(s2b_1p2v_8m);
		if (rc){
			CAM_ERR("%s: Disable regulator s2b_1p2v failed\n", __func__);
			goto init_fail;
		}
		regulator_put(s2b_1p2v_8m);
	}
#endif	

#ifdef F_MT9V113_POWER	
	mt9v113_vreg_init();
	rc = mt9v113_power(ON);
	if (rc) {		
		CAM_ERR(" mt9v113_power failed rc=%d\n",rc);
		goto init_fail; 
	}
#endif
	mt9v113_ctrl = kzalloc(sizeof(struct mt9v113_ctrl_t), GFP_KERNEL);
	if (!mt9v113_ctrl) {
		CAM_ERR("mt9v113_init failed!\n");
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		mt9v113_ctrl->sensordata = data;

	/* Input MCLK = 24MHz */
	CAM_INFO(" msm_camio_clk_rate_set E\n");
#ifndef F_ICP_HD_STANDBY	
	msm_camio_clk_rate_set(24000000);
#endif
	CAM_INFO(" msm_camio_clk_rate_set X\n");
	msleep(5);//mdelay(5);

	rc = mt9v113_sensor_init_probe(data);
	if (rc < 0) {
		CAM_ERR("mt9v113_sensor_init failed!\n");
		goto init_fail;
	}

init_done:
	return rc;

init_fail:
	kfree(mt9v113_ctrl);
	return rc;
}

static int mt9v113_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	init_waitqueue_head(&mt9v113_wait_queue);
	return 0;
}

int mt9v113_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long   rc = 0;

	if (copy_from_user(&cfg_data,
			(void *)argp,
			sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	/* down(&s5k6aafx13_sem); */

	CAM_INFO("mt9v113_sensor_config, cfgtype = %d, mode = %d\n",
		cfg_data.cfgtype, cfg_data.mode);

		switch (cfg_data.cfgtype) {
		case CFG_SET_MODE:
			rc = mt9v113_set_sensor_mode(
						cfg_data.mode);
			break;

		case CFG_SET_EFFECT:
			rc = mt9v113_set_effect(cfg_data.mode,
						cfg_data.cfg.effect);
			CAM_INFO("mt9v113_set_effect OK! rc = [%d], cfg_data.mode = [%d], cfg_data.cfg.effect =[%d]\n", rc, cfg_data.mode, cfg_data.cfg.effect);			
			break;
#ifdef F_SKYCAM_FIX_CFG_BRIGHTNESS
		case CFG_SET_BRIGHTNESS:
			rc = mt9v113_set_brightness(cfg_data.cfg.brightness);
			CAM_INFO("mt9v113_set_brightness OK! rc = [%d], cfg_data.cfg.brightness =[%d]\n", rc, cfg_data.cfg.brightness);
			break;
#endif			
#ifdef F_SKYCAM_FIX_CFG_WB
		case CFG_SET_WB:			
			rc = mt9v113_set_whitebalance(cfg_data.cfg.whitebalance);
			CAM_INFO("mt9v113_set_whitebalance OK! rc = [%d], cfg_data.mode = [%d], cfg_data.cfg.whitebalance =[%d]\n", rc, cfg_data.mode, cfg_data.cfg.whitebalance);
			break;
#endif	
#ifdef F_SKYCAM_FIX_CFG_EXPOSURE
		case CFG_SET_EXPOSURE_MODE:			
			rc = mt9v113_set_exposure_mode(cfg_data.cfg.exposure);
			CAM_INFO("mt9v113_set_exposure_mode OK! rc = [%d], cfg_data.mode = [%d], cfg_data.cfg.exposure =[%d]\n", rc, cfg_data.mode, cfg_data.cfg.exposure);
			break;
#endif
#ifdef F_SKYCAM_FIX_CFG_PREVIEW_FPS
		case CFG_SET_PREVIEW_FPS:			
			rc = mt9v113_set_preview_fps(cfg_data.cfg.preview_fps);
			CAM_INFO("mt9v113_set_frame_rate OK! rc = [%d], cfg_data.mode = [%d], cfg_data.cfg.preview_fps =[%d]\n", rc, cfg_data.mode, cfg_data.cfg.preview_fps);
			break;
#endif
#ifdef F_SKYCAM_FIX_CFG_REFLECT
		case CFG_SET_REFLECT:			
			rc = mt9v113_set_reflect(cfg_data.cfg.reflect);
			CAM_INFO("mt9v113_set_reflect OK! rc = [%d], cfg_data.mode = [%d], cfg_data.cfg.reflect =[%d]\n", rc, cfg_data.mode, cfg_data.cfg.reflect);
			break;
#endif
		case CFG_GET_AF_MAX_STEPS:
		default:
			rc = -EINVAL;
			break;
		}

	/* up(&s5k6aafx13_sem); */

	return rc;
}

int mt9v113_sensor_release(void)
{
	int rc = 0;

#ifdef F_MT9V113_POWER	
	CAM_ERR(" mt9v113_sensor_release E\n");	
    rc = mt9v113_reset(OFF);
	if (rc < 0) {
		CAM_ERR("%s reset failed!\n",__func__);		
	}
	rc = mt9v113_power(OFF);
	if (rc) {
		CAM_ERR(" mt9v113_power failed rc=%d\n",rc);		
	}
#endif	
#ifdef F_ICP_HD_STANDBY
	gpio_set_value_cansleep(SENSOR_STANDBY_8M,0);	
	icp_hd_reset(OFF);
	rc  = icp_hd_power(OFF);
	if (rc) {
		CAM_ERR(" icp_hd_power failed rc=%d\n",rc);		
	}
#endif

	kfree(mt9v113_ctrl);

	return rc;
}

static int mt9v113_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int rc = 0;
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	mt9v113_sensorw =
		kzalloc(sizeof(struct mt9v113_work), GFP_KERNEL);
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	if (!mt9v113_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	i2c_set_clientdata(client, mt9v113_sensorw);
	mt9v113_init_client(client);
	mt9v113_client = client;

	CAM_INFO("mt9v113_probe succeeded!\n");

	return 0;

probe_failure:
	kfree(mt9v113_sensorw);
	mt9v113_sensorw = NULL;
	CAM_ERR("mt9v113_probe failed!\n");
	return rc;
}

static const struct i2c_device_id mt9v113_i2c_id[] = {
	{ "mt9v113", 0},
	{ },
};

static struct i2c_driver mt9v113_i2c_driver = {
	.id_table = mt9v113_i2c_id,
	.probe  = mt9v113_i2c_probe,
	.remove = __exit_p(mt9v113_i2c_remove),
	.driver = {
		.name = "mt9v113",
	},
};

static int32_t mt9v113_init_i2c(void)
{
	int32_t rc = 0;

	//CAM_INFO("%s start\n",__func__);

	rc = i2c_add_driver(&mt9v113_i2c_driver);
	//CAM_INFO("%s mt9v113_i2c_driver rc = %d\n",__func__, rc);
	if (IS_ERR_VALUE(rc))
		goto init_i2c_fail;

	CAM_INFO("%s end\n",__func__);

	//hhs i2c test 
	return 0;

init_i2c_fail:
	CAM_ERR("%s failed! (%d)\n", __func__, rc);
	return rc;
}

static int mt9v113_sensor_probe(const struct msm_camera_sensor_info *info,
				struct msm_sensor_ctrl *s)
{
	int rc = 0;
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);

	rc = mt9v113_init_i2c();	
	if (rc < 0 || mt9v113_client == NULL)
	{
		CAM_ERR("%s rc = %d, mt9v113_client = %x\n",__func__, rc, mt9v113_client);
		goto probe_done;
	}

	s->s_init = mt9v113_sensor_init;
	s->s_release = mt9v113_sensor_release;
	s->s_config  = mt9v113_sensor_config;
	s->s_camera_type = FRONT_CAMERA_2D;
	//s->s_mount_angle = 270;
	#if BOARD_VER_G(AT1_WS22)			
	s->s_mount_angle = 270;
    #else
	s->s_mount_angle = 0;//180;//90;
    #endif
probe_done:
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}

static int __mt9v113_probe(struct platform_device *pdev)
{
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return msm_camera_drv_start(pdev, mt9v113_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __mt9v113_probe,
	.driver = {
		.name = "msm_camera_mt9v113",
		.owner = THIS_MODULE,
	},
};

static int __init mt9v113_init(void)
{
	//CAM_INFO("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return platform_driver_register(&msm_camera_driver);
}

module_init(mt9v113_init);
