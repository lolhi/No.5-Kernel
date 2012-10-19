/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Code Aurora Forum nor
 *       the names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 *
 * Alternatively, provided that this notice is retained in full, this software
 * may be relicensed by the recipient under the terms of the GNU General Public
 * License version 2 ("GPL") and only version 2, in which case the provisions of
 * the GPL apply INSTEAD OF those given above.  If the recipient relicenses the
 * software under the GPL, then the identification text in the MODULE_LICENSE
 * macro must be changed to reflect "GPLv2" instead of "Dual BSD/GPL".  Once a
 * recipient changes the license terms to the GPL, subsequent recipients shall
 * not relicense under alternate licensing terms, including the BSD or dual
 * BSD/GPL terms.  In addition, the following license statement immediately
 * below and between the words START and END shall also then apply when this
 * software is relicensed under the GPL:
 *
 * START
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 and only version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * END
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

//#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <mach/gpio.h>
#include <mach/pmic.h>
#include "msm_fb.h"
#if 0
#include <linux/regulator/consumer.h>
#include <linux/regulator/machine.h>
#endif

#ifndef __LCD_DEBUG__
#define __LCD_DEBUG__
#if 0
#define ENTER_FUNC2() printk(KERN_ERR "[SKY_LCD] +%s\n", __FUNCTION__);
#define EXIT_FUNC2() printk(KERN_ERR "[SKY_LCD] -%s\n", __FUNCTION__);
#define PRINT(X) printk(KERN_ERR "[SKY_LCD] -%s:%s\n", __FUNCTION__,X);
#define PRINT2(X) printk(KERN_ERR "[SKY_LCD] -%s:%d\n", __FUNCTION__,X);
#else
#define ENTER_FUNC2()
#define EXIT_FUNC2() printk(KERN_ERR "[SKY_LCD] -%s\n", __FUNCTION__);
#define PRINT(X)
#define PRINT2(X)
#endif
#endif /* __LCD_DEBUG__ */

// 20100702, kkcho, 사용 모델별 정의
//=============================
#define LCDC_SKY_LCD_LMS501KF02 //20101119 at1 5inch SMD lcd

//== 모델 추가.... ==
// Backlight
#define FEATURE_SKY_BACKLIGHT_MAX8831  //hhs 20101007 ci2 backlight
//#define I2C_SW_CONTROL  // kkcho, HW_i2c Error시 SW_GPIO_CTRL로 확인필요할때 사용.
//=============================
#define GPIO_I2C_LED_CONTROL
#define FEATURE_VEE_I2C_CONTROL //hhs vee chip control 20100906

/* define spi function */
static void send_spi_command(u8 reg, u8 count, u8 *param);

#define SPI_SCLK	   36
#define SPI_CS		   35
#define SPI_SDI		   33
//#define SPI_SDO		   48
#define LCD_RESET	   94
//#define LCD_BL_EN        130

#if 1//AT1_BDVER_GE(AT1_WS10)
#define VEE_RESET      143
#define VEE_ACTIVE     144
#define VEE_RGB_OE     145
#define VEE_VLP        146
#endif

#ifdef I2C_SW_CONTROL
#define LCD_BL_SDA      171
#define LCD_BL_SCL      170
#endif

#ifdef FEATURE_SKY_BACKLIGHT_MAX8831
const uint8 MAX8831_I2C_ID = 0x9A;
const uint8 MAX8831_I2C_DELAY = 2;
#endif

#define GPIO_HIGH_VALUE          1
#define GPIO_LOW_VALUE           0

#define DEBUG_EN 0

#ifdef FEATURE_SKY_BACKLIGHT_MAX8831
static boolean flag_lcd_bl_off = FALSE;
static boolean flag_lcd_bl_ctrl = TRUE;
#endif

 //20110725 bandit block charging current 700ma at lcd on 
#if 0//def FEATURE_AT1_PMIC_CHARGING
extern void msm_charger_set_lcd_onoff(unsigned int onff);
#endif

#define NOP()   	usleep(1)//do {asm volatile ("NOP");} while (0)//usleep(1)
//do {asm volatile ("NOP");} while (0)

u8 parameter_list[128];

static unsigned char bit_shift[8] = { (1 << 7),	/* MSB */
	(1 << 6),
	(1 << 5),
	(1 << 4),
	(1 << 3),
	(1 << 2),
	(1 << 1),
	(1 << 0)		               /* LSB */
};

#if 0
struct lcdc_sky_pdata {
	struct msm_panel_common_pdata *pdata;
	struct platform_device *fbpdev;
};

static struct lcdc_sky_pdata *dd;
#else


static struct msm_panel_common_pdata *lcdc_sky_pdata;

#endif

#ifdef FEATURE_VEE_I2C_CONTROL
static struct i2c_client *vee_i2c_client = NULL;

static int vee_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int rc = 0;

	ENTER_FUNC2(); 

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
	{
		vee_i2c_client = NULL;
		rc = -1;
		if(DEBUG_EN) 
		printk(KERN_ERR "[vee_i2c_probe] failed!!!\n");
	}
	else
	{
		vee_i2c_client = client;
	}
	EXIT_FUNC2();

	if(DEBUG_EN) printk(KERN_ERR "[vee_i2c_probe] successed!!!\n");

	return rc;
}


static int vee_i2c_remove(struct i2c_client *client)
{
#if 0
	int rc;

	rc = i2c_detach_client(client);

	return rc;
#endif

	vee_i2c_client = NULL;

	if(DEBUG_EN) printk(KERN_ERR "[vee_i2c_probe] removed!!!\n");
	return 0;
}

static const struct i2c_device_id vee_id[] = {
	{ "VEE20", 2 },  { }
};

static struct i2c_driver vee_i2c_driver = {
	.driver = {
		.name = "VEE20",
		.owner = THIS_MODULE,
	},
	.probe = vee_i2c_probe,
	.remove = __devexit_p(vee_i2c_remove),
	.id_table = vee_id,
};

//static uint32_t vee_gpio_init_table[] = {
//	GPIO_CFG(36, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),  //vee_rgb_oe
//	GPIO_CFG(34, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA),  //vee_active
//};


static void vee_i2c_api_Init(void)
{
	int result = 0;

	ENTER_FUNC2();
	
	result = i2c_add_driver(&vee_i2c_driver);
	if(result){
		if(DEBUG_EN) printk(KERN_ERR "[vee_i2c_api_Init] error!!!\n");
	}
	EXIT_FUNC2();  
}
#define FEATURE_VEE_GPIO_CONTROL
#if defined(FEATURE_VEE_GPIO_CONTROL)
#define VEE_SDA        116
#define VEE_SCL        115

static uint32_t vee_gpio_init_table[] = {
	GPIO_CFG(VEE_SDA, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(VEE_SCL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};

static void vee_gpio_init(uint32_t *table, int len, unsigned enable)
{
	int n, rc;
	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n],
			enable ? GPIO_CFG_ENABLE : GPIO_CFG_DISABLE);
		if (rc) {
			printk(KERN_ERR "%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
	}
}


static void vee_sda_write (unsigned char onoff)
{
	if(onoff)
	{
		gpio_set_value(VEE_SDA, GPIO_HIGH_VALUE);
	}
	else
	{
		gpio_set_value(VEE_SDA, GPIO_LOW_VALUE);
	}
} 

static void vee_scl_write (unsigned char onoff)
{
	if(onoff)
	{
		gpio_set_value(VEE_SCL, GPIO_HIGH_VALUE);
	}
	else
	{
		gpio_set_value(VEE_SCL, GPIO_LOW_VALUE);
	}
}

static void vee_scl_delay (void)
{
	clk_busy_wait(2);
}

static void vee_i2c_wait (void)
{
	clk_busy_wait(2);
}


static void vee_i2c_start_condition (void)
{
	vee_sda_write (1);
	vee_scl_write (1);  
	vee_sda_write(0);
	vee_scl_delay();
	vee_scl_write (0);
	vee_i2c_wait();
}

static void vee_i2c_stop_condition (void)
{
	vee_sda_write (0);
	vee_scl_write(1);
	vee_scl_delay();
	vee_sda_write (1);
	vee_i2c_wait();  
}

static void vee_i2c_ack (void)
{
	vee_sda_write (0);
	vee_scl_write (1);
	vee_scl_delay();
	vee_scl_write(0);
	vee_i2c_wait();
}

static void vee_i2c_write_byte(unsigned char data)
{
	signed char  bit_idx;

	for (bit_idx = 7; bit_idx >= 0; bit_idx--)
	{
		vee_sda_write (data>>bit_idx & 0x01);
		vee_scl_delay();
		vee_scl_write (1);
		vee_scl_delay();
		vee_scl_write (0);
		//BOOT_SCL_DELAY();
	}
	//boot_i2c_ack();
}

static void vee_i2c_gpio_master_send(void)
{
  __u8 i=0;
  
  vee_gpio_init(vee_gpio_init_table, ARRAY_SIZE(vee_gpio_init_table), 1);
  vee_i2c_start_condition();
  vee_i2c_write_byte(0x54);
  
  for(i=0; i<8; i++)
  {
    vee_i2c_ack();
    vee_i2c_write_byte(0x00);
  }
  vee_i2c_ack();
  vee_i2c_stop_condition();
}
#endif


#if 0
static uint8 vee_i2c_master_byte_send(uint8 addr, uint8 val)
{
	static int ret = 0;
	unsigned char buf[8];
	struct i2c_msg msg[4];
#ifdef FEATURE_I2C_WRITE_CHECK
	uint16 rData;
#endif /* FEATURE_I2C_WRITE_CHECK */

	if(!vee_i2c_client)
	{
		printk(KERN_ERR "[vee_i2c_master_send] led_i2c_client is null!!!\n");
			return -1;
	}

	buf[0] = addr;
	buf[1] = val;

	msg[0].addr = vee_i2c_client->addr;  
	msg[0].flags = 0;
	msg[0].len = 2;
	msg[0].buf = buf;

#if  1//def FEATURE_I2C_DBG_MSG
	printk(KERN_ERR "[vee_i2c_master_send] ID[0x%06x] reg[0x%06x] data[0x%06x]\n", vee_i2c_client->addr, addr, val);
#endif /* FEATURE_I2C_DBG_MSG */

	ret = i2c_transfer(vee_i2c_client->adapter, msg, 1);
	if (ret < 0)
	{
		printk(KERN_ERR "[vee_i2c_master_send] write error!!!\n");
			return FALSE;
	}
	else
	{
#if  1//def FEATURE_I2C_DBG_MSG
		printk(KERN_ERR "[vee_i2c_master_send] write OK!!!\n");
#endif /* FEATURE_I2C_DBG_MSG */

#ifdef FEATURE_I2C_WRITE_CHECK
		rData = led_i2c_master_recv(reg);
		if (rData != data)
		{
			printk(KERN_ERR "[vee_i2c_master_send:error] reg[0x%06x], data[0x%06x]\n", reg, rData);
		}
#endif /* FEATURE_I2C_WRITE_CHECK */
		return TRUE;
	}
}
#endif

#if 1
static uint8 vee_i2c_master_send(uint32 addr, uint32 val)
{
	static int ret = 0;
	static unsigned char buf[8];
	struct i2c_msg msg[2];
	
#ifdef FEATURE_I2C_WRITE_CHECK
	uint16 rData;
#endif /* FEATURE_I2C_WRITE_CHECK */

	if(!vee_i2c_client)
	{
		printk(KERN_ERR "[vee_i2c_master_send] led_i2c_client is null!!!\n");
			return -1;
	}
		

	buf[0] = addr & 0xff;
	buf[1] = (addr >> 8) & 0xff;
	buf[2] = (addr >> 16) & 0xff;
	buf[3] = (addr >> 24) & 0xff;
	
	buf[4] = val & 0xff;
	buf[5] = (val >> 8) & 0xff;
	buf[6] = (val >> 16) & 0xff;
	buf[7] = (val >> 24) & 0xff;

	msg[0].addr = vee_i2c_client->addr;  
	msg[0].flags = 0;
	msg[0].len = 8;//sizeof(buf);
	msg[0].buf = buf;

#if  1//def FEATURE_I2C_DBG_MSG
	printk(KERN_ERR "[vee_i2c_master_send] ID[0x%08x] reg[0x%08x] data[0x%08x]\n", vee_i2c_client->addr, addr, val);
#endif /* FEATURE_I2C_DBG_MSG */

	//ret = i2c_master_send(vee_i2c_client, buf, 8);
    //printk(KERN_ERR "[vee_i2c 111] msgs[0]->flags[%d] vee_i2c_client->name[%s]==%d\n", msg[0].flags, vee_i2c_client->name,vee_i2c_client->adapter->id);

    //if( (msg[0].flags == 0) && (strncmp(vee_i2c_client->adapter,"VEE20",5)==0) )
	//	printk(KERN_ERR "[vee_i2c_strncmp\n");
	//return TRUE;
	
    ret = i2c_transfer(vee_i2c_client->adapter, msg, 1);
	if (ret < 0)
	{
		printk(KERN_ERR "[vee_i2c_master_send] write error!!!\n=ret=%d",ret);
			return FALSE;
	}
	else
	{
#if  1//def FEATURE_I2C_DBG_MSG
		printk(KERN_ERR "[vee_i2c_master_send] write OK!!!\n");
#endif /* FEATURE_I2C_DBG_MSG */

#ifdef FEATURE_I2C_WRITE_CHECK
		rData = led_i2c_master_recv(reg);
		if (rData != data)
		{
			printk(KERN_ERR "[vee_i2c_master_send:error] reg[0x%08x], data[0x%08x]\n", reg, rData);
		}
#endif /* FEATURE_I2C_WRITE_CHECK */
		return TRUE;
	}
}
#endif

#if 0
static uint32 vee_i2c_master_recv(uint32 addr)
{
	static int ret = 0;
	unsigned char wbuf[4];
	unsigned char rbuf[4] = {0,0,0,0};
	struct i2c_msg msgs[2];
	uint32 uiData;

	if(!vee_i2c_client)
	{
		printk(KERN_ERR "[vee_i2c_write_word] led_i2c_client is null!!!\n");
		return -1;
	}

	wbuf[0] = addr & 0xff;
	wbuf[1] = (addr >> 8) & 0xff;
	wbuf[2] = (addr >> 16) & 0xff;
	wbuf[3] = (addr >> 24) & 0xff;

	msgs[0].addr = vee_i2c_client->addr;
	msgs[0].flags = 0;
	msgs[0].len = 4;
	msgs[0].buf = wbuf;

	msgs[1].addr = vee_i2c_client->addr;
	msgs[1].flags = 1; //I2C_M_RD;
	msgs[1].len = 4;
	msgs[1].buf = rbuf;

#if  1//def FEATURE_I2C_DBG_MSG
	printk(KERN_ERR "[vee_i2c_read_word] ID[0x%08x] reg[0x%08x]\n", vee_i2c_client->addr, addr);
#endif

#if 1 // android 3145 버전에서 msgs 2개 한번에 보내면 에러리턴됨
	ret = i2c_transfer(vee_i2c_client->adapter, msgs, 1);
	if (ret < 0)
	{
		printk(KERN_ERR "[vee_i2c_read_word] write error!!!\n");
		return FALSE;
	}
	ret = i2c_transfer(vee_i2c_client->adapter, &msgs[1], 1);
#else
	ret = i2c_transfer(led_i2c_client->adapter, msgs, 2);
#endif
	if (ret < 0)
	{
		printk(KERN_ERR "[vee_i2c_read_word] read error!!!\n");
		return FALSE;
	}
	else
	{
		uiData = rbuf[0];
#if  1//def FEATURE_I2C_DBG_MSG
		printk(KERN_ERR "[vee_i2c_read_word] read OK!!!\n");
		printk(KERN_ERR "[vee_i2c_read_word] reg[0x%08x], data[0x%08x]\n", addr, uiData);
#endif
		return uiData;
	}
}
#endif
#endif

#ifndef I2C_SW_CONTROL
static struct i2c_client *led_i2c_client = NULL;

static int led_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int rc = 0;

	ENTER_FUNC2(); 
#if 0
	led_i2c_client = client;
#else
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
	{
		led_i2c_client = NULL;
		rc = -1;
		if(DEBUG_EN) 
		printk(KERN_ERR "[led_i2c_probe] failed!!!\n");
	}
	else
	{
		led_i2c_client = client;
	}
#endif
	EXIT_FUNC2();

	if(DEBUG_EN) printk(KERN_ERR "[led_i2c_probe] successed!!!\n");

	return rc;
}


static int led_i2c_remove(struct i2c_client *client)
{
#if 0
	int rc;

	rc = i2c_detach_client(client);

	return rc;
#endif

	led_i2c_client = NULL;

	if(DEBUG_EN) printk(KERN_ERR "[led_i2c_probe] removed!!!\n");
	return 0;
}

static const struct i2c_device_id led_id[] = {
	{ "MAX8831", 0 },  { }
};

static struct i2c_driver led_i2c_driver = {
	.driver = {
		.name = "MAX8831",
		.owner = THIS_MODULE,
	},
	.probe = led_i2c_probe,
	.remove = __devexit_p(led_i2c_remove),
	.id_table = led_id,
};
#endif

#ifdef I2C_SW_CONTROL
static uint32_t backlight_gpio_init_table[] = {
	GPIO_CFG(LCD_BL_SDA, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(LCD_BL_SCL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};
#endif

static void lcd_gpio_init(uint32_t *table, int len, unsigned enable)
{
	int n, rc;
	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n],
			enable ? GPIO_CFG_ENABLE : GPIO_CFG_DISABLE);
		if (rc) {
			printk(KERN_ERR "%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
	}
}
static void led_i2c_api_Init(void)
{
#ifdef I2C_SW_CONTROL
    ENTER_FUNC2();
	lcd_gpio_init(backlight_gpio_init_table, ARRAY_SIZE(backlight_gpio_init_table), 1);
	gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);
	gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);   
	EXIT_FUNC2();
#else
	int result = 0;

	ENTER_FUNC2();
	result = i2c_add_driver(&led_i2c_driver);
	if(result){
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_api_Init] error!!!\n");
	}
	EXIT_FUNC2();
#endif  
}


#ifdef FEATURE_I2C_WRITE_CHECK
static uint8 led_i2c_master_recv(uint8 reg)
{
	static int ret = 0;
	unsigned char wbuf[2];
	unsigned char rbuf[2] = {0,0};
	struct i2c_msg msgs[2];
	uint8 uiData;

	if(!led_i2c_client)
	{
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_write_word] led_i2c_client is null!!!\n");
		return -1;
	}

	wbuf[0] = reg;

	msgs[0].addr = led_i2c_client->addr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = wbuf;

	msgs[1].addr = led_i2c_client->addr;
	msgs[1].flags = 1; //I2C_M_RD;
	msgs[1].len = 1;
	msgs[1].buf = rbuf;

#ifdef FEATURE_I2C_DBG_MSG
	if(DEBUG_EN) printk(KERN_ERR "[led_i2c_read_word] ID[0x%02x] reg[0x%02x]\n", led_i2c_client->addr, reg);
#endif

#if 1 // android 3145 버전에서 msgs 2개 한번에 보내면 에러리턴됨
	ret = i2c_transfer(led_i2c_client->adapter, msgs, 1);
	if (ret < 0)
	{
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_read_word] write error!!!\n");
		return FALSE;
	}
	ret = i2c_transfer(led_i2c_client->adapter, &msgs[1], 1);
#else
	ret = i2c_transfer(led_i2c_client->adapter, msgs, 2);
#endif
	if (ret < 0)
	{
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_read_word] read error!!!\n");
			return FALSE;
	}
	else
	{
		uiData = rbuf[0];
#ifdef FEATURE_I2C_DBG_MSG
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_read_word] read OK!!!\n");
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_read_word] reg[0x%02x], data[0x%02x]\n", reg, uiData);
#endif
		return uiData;
	}
}
#endif

#ifdef I2C_SW_CONTROL
static void sda_write (unsigned char onoff)
{
	if(onoff)
	{
		gpio_set_value(LCD_BL_SDA, GPIO_HIGH_VALUE);
	}
	else
	{
		gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);
	}
} 

static void scl_write (unsigned char onoff)
{
	if(onoff)
	{
		gpio_set_value(LCD_BL_SCL, GPIO_HIGH_VALUE);
	}
	else
	{
		gpio_set_value(LCD_BL_SCL, GPIO_LOW_VALUE);
	}
}

static void SCL_DELAY (void)
{
	clk_busy_wait(MAX8831_I2C_DELAY);
}

static void i2c_wait (void)
{
	clk_busy_wait(MAX8831_I2C_DELAY);
}


static void i2c_start_condition (void)
{
	sda_write (1);
	scl_write (1);  
	sda_write(0);
	SCL_DELAY();
	scl_write (0);
	i2c_wait();
}

static void i2c_stop_condition (void)
{
	sda_write (0);
	scl_write(1);
	SCL_DELAY();
	sda_write (1);
	i2c_wait();  
}

static void i2c_ack (void)
{
	sda_write (0);
	scl_write (1);
	SCL_DELAY();
	scl_write(0);
	i2c_wait();
}

static void i2c_write_byte(unsigned char data)
{
	signed char  bit_idx;

	for (bit_idx = 7; bit_idx >= 0; bit_idx--)
	{
		sda_write (data>>bit_idx & 0x01);
		SCL_DELAY();
		scl_write (1);
		SCL_DELAY();
		scl_write (0);
		//BOOT_SCL_DELAY();
	}
	//boot_i2c_ack();
}
#endif

static uint8 led_i2c_master_send(uint8 reg, uint8 data)
{
#ifdef I2C_SW_CONTROL
	i2c_start_condition();
	i2c_write_byte(MAX8831_I2C_ID);
	i2c_ack();
	i2c_write_byte(reg);
	i2c_ack();
	i2c_write_byte(data);
	i2c_ack();
	i2c_stop_condition();
	return TRUE;
#else
	static int ret = 0;
	unsigned char buf[4];
	struct i2c_msg msg[2];
#ifdef FEATURE_I2C_WRITE_CHECK
	uint16 rData;
#endif /* FEATURE_I2C_WRITE_CHECK */

	if(!led_i2c_client)
	{
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_master_send] led_i2c_client is null!!!\n");
			return -1;
	}

	buf[0] = reg;
	buf[1] = data;

	msg[0].addr = led_i2c_client->addr;  
	msg[0].flags = 0;
	msg[0].len = 2;
	msg[0].buf = buf;

#ifdef FEATURE_I2C_DBG_MSG
	if(DEBUG_EN) printk(KERN_ERR "[led_i2c_master_send] ID[0x%02x] reg[0x%02x] data[0x%02x]\n", led_i2c_client->addr, reg, data);
#endif /* FEATURE_I2C_DBG_MSG */

	ret = i2c_transfer(led_i2c_client->adapter, msg, 1);
	if (ret < 0)
	{
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_master_send] write error!!!\n");
			return FALSE;
	}
	else
	{
#ifdef FEATURE_I2C_DBG_MSG
		if(DEBUG_EN) printk(KERN_ERR "[led_i2c_master_send] write OK!!!\n");
#endif /* FEATURE_I2C_DBG_MSG */

#ifdef FEATURE_I2C_WRITE_CHECK
		rData = led_i2c_master_recv(reg);
		if (rData != data)
		{
			if(DEBUG_EN) printk(KERN_ERR "[led_i2c_master_send:error] reg[0x%02x], data[0x%02x]\n", reg, rData);
		}
#endif /* FEATURE_I2C_WRITE_CHECK */
		return TRUE;
	}
#endif
}

#if defined(GPIO_I2C_LED_CONTROL)
#define LCD_BL_SDA      171
#define LCD_BL_SCL      170

static uint32_t backlight_gpio_init_table[] = {
	GPIO_CFG(LCD_BL_SDA, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(LCD_BL_SCL, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};
static void sda_write (unsigned char onoff)
{
	if(onoff)
	{
		gpio_set_value(LCD_BL_SDA, GPIO_HIGH_VALUE);
	}
	else
	{
		gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);
	}
} 

static void scl_write (unsigned char onoff)
{
	if(onoff)
	{
		gpio_set_value(LCD_BL_SCL, GPIO_HIGH_VALUE);
	}
	else
	{
		gpio_set_value(LCD_BL_SCL, GPIO_LOW_VALUE);
	}
}

static void SCL_DELAY (void)
{
	clk_busy_wait(MAX8831_I2C_DELAY);
}

static void i2c_wait (void)
{
	clk_busy_wait(MAX8831_I2C_DELAY);
}


static void i2c_start_condition (void)
{
	sda_write (1);
	scl_write (1);  
	sda_write(0);
	SCL_DELAY();
	scl_write (0);
	i2c_wait();
}

static void i2c_stop_condition (void)
{
	sda_write (0);
	scl_write(1);
	SCL_DELAY();
	sda_write (1);
	i2c_wait();  
}

static void i2c_ack (void)
{
	sda_write (0);
	scl_write (1);
	SCL_DELAY();
	scl_write(0);
	i2c_wait();
}

static void i2c_write_byte(unsigned char data)
{
	signed char  bit_idx;

	for (bit_idx = 7; bit_idx >= 0; bit_idx--)
	{
		sda_write (data>>bit_idx & 0x01);
		SCL_DELAY();
		scl_write (1);
		SCL_DELAY();
		scl_write (0);
		//BOOT_SCL_DELAY();
	}
	//boot_i2c_ack();
}

static void led_gpio_i2c_set(uint8 reg, uint8 data)
{
	i2c_start_condition();
	i2c_write_byte(MAX8831_I2C_ID);
	i2c_ack();
	i2c_write_byte(reg);
	i2c_ack();
	i2c_write_byte(data);
	i2c_ack();
	i2c_stop_condition();
}
static int once_backlight_on=0;
#endif

#if defined(FEATURE_SKY_BACKLIGHT_MAX8831)
static void backlight_on(void)
{
    ENTER_FUNC2();  
	#if defined(GPIO_I2C_LED_CONTROL)
	if (once_backlight_on==0)
	{
	    #if 0
        lcd_gpio_init(backlight_gpio_init_table, ARRAY_SIZE(backlight_gpio_init_table), 1);
        gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);
        gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);   
		led_gpio_i2c_set(0x00, 0x03);
		led_gpio_i2c_set(0x0B, 0x6A);
		led_gpio_i2c_set(0x0C, 0x6A);
        #endif
		once_backlight_on=1;
    }
	else
	{
	#endif
		led_i2c_master_send(0x00, 0x03);
		led_i2c_master_send(0x0B, 0x6A);
		led_i2c_master_send(0x0C, 0x6A);
	#if defined(GPIO_I2C_LED_CONTROL)
	}
	#endif
	EXIT_FUNC2();  
}

static int once_backlight_off=0;
static void backlight_off(void)
{
    ENTER_FUNC2();
    #if 0
	if(once_backlight_off==0)
		once_backlight_off=1;
	else
	{
		led_i2c_master_send(0x0B, 0x1B);
	    led_i2c_master_send(0x0C, 0x1B);
		led_i2c_master_send(0x00, 0x03);
        //led_i2c_master_send(0x00, 0x00);
	    flag_lcd_bl_off = TRUE;
	}
    #else
	#if 0
	if(once_backlight_off==0)
		once_backlight_off=1;
	else
	#endif
        led_i2c_master_send(0x00, 0x00);
	flag_lcd_bl_off = TRUE;
    #endif
	EXIT_FUNC2();  
}
#endif


static void send_spi_command(u8 command, u8 count, u8 *parameter_list)
{
	u8 i,j = 0;

	/* Enable the Chip Select */
	gpio_set_value(SPI_CS, GPIO_LOW_VALUE);
	NOP();
	NOP();

	gpio_set_value(SPI_SCLK, GPIO_LOW_VALUE);
	NOP();
	//CMD
	/* DNC : COMMAND = 0, PARAMETER = 1  */
	gpio_set_value(SPI_SDI, GPIO_LOW_VALUE);

	/* #2: Drive the Clk High*/
	NOP(); 
	gpio_set_value(SPI_SCLK, GPIO_HIGH_VALUE);   
	NOP();
	NOP();
   
	/* Before we enter here the Clock should be Low ! */
	for(i=0;i<8;i++)
	{
		gpio_set_value(SPI_SCLK, GPIO_LOW_VALUE);
		NOP();
		/* #1: Drive the Data (High or Low) */
		if(command & bit_shift[i])
		gpio_set_value(SPI_SDI, GPIO_HIGH_VALUE);        
		else
		gpio_set_value(SPI_SDI, GPIO_LOW_VALUE);        
		/* #2: Drive the Clk High*/
		NOP(); 
		gpio_set_value(SPI_SCLK, GPIO_HIGH_VALUE); 
		NOP();
		NOP();
	}

	//PARAMETER
	for(j=0; j<count; j++)
	{
		gpio_set_value(SPI_SCLK, GPIO_LOW_VALUE);
		NOP();
		/* DNC : COMMAND = 0, PARAMETER = 1  */
		gpio_set_value(SPI_SDI, GPIO_HIGH_VALUE); 
		/* #2: Drive the Clk High*/
		NOP(); 
		gpio_set_value(SPI_SCLK, GPIO_HIGH_VALUE);    
		NOP();
		NOP();

		for(i=0;i<8;i++)
		{
			gpio_set_value(SPI_SCLK, GPIO_LOW_VALUE);
			NOP();
			/* #1: Drive the Data (High or Low) */
			if(parameter_list[j] & bit_shift[i])
			    gpio_set_value(SPI_SDI, GPIO_HIGH_VALUE); 
			else
			    gpio_set_value(SPI_SDI, GPIO_LOW_VALUE); 

			/* #2: Drive the Clk High*/
			NOP(); 
			gpio_set_value(SPI_SCLK, GPIO_HIGH_VALUE);    
			NOP(); 
			NOP();     
		}  
	}
	/* Idle state of SDO (MOSI) is Low */
	gpio_set_value(SPI_SDI, GPIO_LOW_VALUE); 
	gpio_set_value(SPI_SCLK, GPIO_LOW_VALUE);

	/* Now Disable the Chip Select */
	NOP();
	NOP();
	gpio_set_value(SPI_CS, GPIO_HIGH_VALUE);

	NOP();
}

#if 0
static void boot_sda_write (unsigned char onoff)
{
  if(onoff)
  {
    gpio_set_value(LCD_BL_SDA, GPIO_HIGH_VALUE);
  }
  else
  {
    gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);
  }
} 

static void boot_scl_write (unsigned char onoff)
{
  if(onoff)
  {
    gpio_set_value(LCD_BL_SCL, GPIO_HIGH_VALUE);
  }
  else
  {
    gpio_set_value(LCD_BL_SCL, GPIO_LOW_VALUE);
  }
}

static void BOOT_SCL_DELAY (void)
{ 
  clk_busy_wait(MAX8831_I2C_DELAY);
}

static void boot_i2c_wait (void)
{ 
  clk_busy_wait(MAX8831_I2C_DELAY);
}

static void boot_i2c_start_condition (void)
  {
  boot_sda_write (1);
  boot_scl_write (1);  
  boot_sda_write(0);
  BOOT_SCL_DELAY();
  boot_scl_write (0);
  boot_i2c_wait();
}

static void boot_i2c_stop_condition (void)
{
  boot_sda_write (0);
  boot_scl_write(1);
  BOOT_SCL_DELAY();
  boot_sda_write (1);
  boot_i2c_wait();  
}

static void boot_i2c_ack (void)
  {
  boot_sda_write (0);
  boot_scl_write (1);
  BOOT_SCL_DELAY();
  boot_scl_write(0);
  boot_i2c_wait();
}

static void boot_i2c_write_byte(unsigned char data)
{
  signed char  bit_idx;

  for (bit_idx = 7; bit_idx >= 0; bit_idx--)
{
  boot_sda_write (data>>bit_idx & 0x01);
  BOOT_SCL_DELAY();
  boot_scl_write (1);
  BOOT_SCL_DELAY();
  boot_scl_write (0);
  //BOOT_SCL_DELAY();
  }

  //boot_i2c_ack();
}
static void boot_max8831_write (uint8 reg, uint8 data)
{
  boot_i2c_start_condition();
  boot_i2c_write_byte(MAX8831_I2C_ID);
  boot_i2c_ack();
  boot_i2c_write_byte(reg);
  boot_i2c_ack();
  boot_i2c_write_byte(data);
  boot_i2c_ack();
  boot_i2c_stop_condition();
}

void backlight_boot_on(void)
{
  boot_max8831_write(0x0B, 0x6A);
  boot_max8831_write(0x0C, 0x6A);
  boot_max8831_write(0x00, 0x03);
}
static uint8_t once=0;
#endif
static void lcd_setup(void)
{
	ENTER_FUNC2();
	#if defined(LCDC_SKY_LCD_LMS501KF02)  
    gpio_set_value(VEE_VLP, GPIO_HIGH_VALUE);  //VEE_VLP
    #endif

    #if 1//AT1_BDVER_GE(AT1_WS10)
	//gpio_set_value(VEE_RESET, GPIO_LOW_VALUE);
    #endif
	gpio_set_value(LCD_RESET, GPIO_LOW_VALUE);
	usleep(10); //mdelay(1);
	#if 1//AT1_BDVER_GE(AT1_WS10)
	//gpio_set_value(VEE_RESET, GPIO_HIGH_VALUE);
    #endif
	gpio_set_value(LCD_RESET, GPIO_HIGH_VALUE);
	usleep(10);//mdelay(10);   

    #if defined(LCDC_SKY_LCD_LMS501KF02)
	#if 1
	//#define GAMMA_2_5
	#define GAMMA_2_7
	//#define GAMMA_3_0

    parameter_list[0] = 0xFF;
    parameter_list[1] = 0x83;
    parameter_list[2] = 0x69;
    send_spi_command(0xB9, 3, parameter_list);

    parameter_list[0] = 0x01;
    parameter_list[1] = 0x00;
    parameter_list[2] = 0x34;
    parameter_list[3] = 0x06;
    parameter_list[4] = 0x00;
    parameter_list[5] = 0x14;
    parameter_list[6] = 0x14;
    parameter_list[7] = 0x20;
    parameter_list[8] = 0x28;
    parameter_list[9] = 0x12;
    parameter_list[10] = 0x12;
    parameter_list[11] = 0x17;
	parameter_list[12] = 0x0A;
    parameter_list[13] = 0x01;
    parameter_list[14] = 0xE6;
	parameter_list[15] = 0xE6;
    parameter_list[16] = 0xE6;
    parameter_list[17] = 0xE6;
    parameter_list[18] = 0xE6;
    send_spi_command(0xB1, 19, parameter_list);
	
    parameter_list[0] = 0x00;
    parameter_list[1] = 0x2B;
    parameter_list[2] = 0x03;
    parameter_list[3] = 0x03;
    parameter_list[4] = 0x70;
    parameter_list[5] = 0x00;
    parameter_list[6] = 0xFF;
    parameter_list[7] = 0x00;
    parameter_list[8] = 0x00;
    parameter_list[9] = 0x00;
    parameter_list[10] = 0x00;
    parameter_list[11] = 0x03;
	parameter_list[12] = 0x03;
    parameter_list[13] = 0x00;
    parameter_list[14] = 0x01;
    send_spi_command(0xB2, 15, parameter_list);

	//hhs 20101220 test DPL(b3)=0
    parameter_list[0] = 0x07;//0x09;
	send_spi_command(0xB3, 1, parameter_list);

	parameter_list[0] = 0x01;
	parameter_list[1] = 0x08;
    parameter_list[2] = 0x77;
    parameter_list[3] = 0x0E;
    parameter_list[4] = 0x06;
    send_spi_command(0xB4, 5, parameter_list);

	parameter_list[0] = 0x4C;
    parameter_list[1] = 0x2E;
	send_spi_command(0xB6, 2, parameter_list);
	
    parameter_list[0] = 0x00;
    parameter_list[1] = 0x05;
    parameter_list[2] = 0x03;
    parameter_list[3] = 0x29;
    parameter_list[4] = 0x01;
    parameter_list[5] = 0x07;
    parameter_list[6] = 0x17;
    parameter_list[7] = 0x68;
    parameter_list[8] = 0x13;
    parameter_list[9] = 0x37;
    parameter_list[10] = 0x20;
    parameter_list[11] = 0x31;
	parameter_list[12] = 0x8A;
    parameter_list[13] = 0x46;
    parameter_list[14] = 0x9B;
	parameter_list[15] = 0x57;
    parameter_list[16] = 0x13;
    parameter_list[17] = 0x02;
    parameter_list[18] = 0x75;
    parameter_list[19] = 0xB9;
    parameter_list[20] = 0x64;
    parameter_list[21] = 0xA8;
    parameter_list[22] = 0x07;
    parameter_list[23] = 0x0F;
    parameter_list[24] = 0x04;
	parameter_list[25] = 0x07;
    send_spi_command(0xD5, 26, parameter_list);

    parameter_list[0] = 0x0A;
	send_spi_command(0xCC, 1, parameter_list);

    parameter_list[0] = 0x3A;
	send_spi_command(0x77, 1, parameter_list);
	
    parameter_list[0] = 0x00;
    parameter_list[1] = 0x04;
    parameter_list[2] = 0x09;
    parameter_list[3] = 0x0F;
    parameter_list[4] = 0x1F;
    parameter_list[5] = 0x3F;
	parameter_list[6] = 0x1F;
    parameter_list[7] = 0x2F;
	parameter_list[8] = 0x0A;
    parameter_list[9] = 0x0F;
    parameter_list[10] = 0x10;
    parameter_list[11] = 0x16;
	parameter_list[12] = 0x18;
    parameter_list[13] = 0x16;
    parameter_list[14] = 0x17;
	parameter_list[15] = 0x0D;
    parameter_list[16] = 0x15;
    parameter_list[17] = 0x00;
    parameter_list[18] = 0x04;
    parameter_list[19] = 0x09;
    parameter_list[20] = 0x0F;
    parameter_list[21] = 0x38;
    parameter_list[22] = 0x3F;
    parameter_list[23] = 0x20;
    parameter_list[24] = 0x39;
    parameter_list[25] = 0x0A;
    parameter_list[26] = 0x0F;
    parameter_list[27] = 0x10;
	parameter_list[28] = 0x16;
    parameter_list[29] = 0x18;
    parameter_list[30] = 0x16;
    parameter_list[31] = 0x17;
    parameter_list[32] = 0x0D;
    parameter_list[33] = 0x15;
    send_spi_command(0xE0, 34, parameter_list);

    parameter_list[0] = 0x01;
    parameter_list[1] = 0x03;
	parameter_list[2] = 0x07; 
	parameter_list[3] = 0x0F; 
	parameter_list[4] = 0x1A; 
	parameter_list[5] = 0x22; 
	parameter_list[6] = 0x2C; 
	parameter_list[7] = 0x33; 
	parameter_list[8] = 0x3C; 
	parameter_list[9] = 0x46; 
	parameter_list[10] = 0x4F; 
	parameter_list[11] = 0x58; 
	parameter_list[12] = 0x60; 
	parameter_list[13] = 0x69; 
	parameter_list[14] = 0x71; 
	parameter_list[15] = 0x79; 
	parameter_list[16] = 0x82; 
	parameter_list[17] = 0x89; 
	parameter_list[18] = 0x92; 
	parameter_list[19] = 0x9A; 
	parameter_list[20] = 0xA1; 
	parameter_list[21] = 0xA9; 
	parameter_list[22] = 0xB1; 
	parameter_list[23] = 0xB9; 
	parameter_list[24] = 0xC1; 
	parameter_list[25] = 0xC9; 
	parameter_list[26] = 0xCF; 
	parameter_list[27] = 0xD6; 
	parameter_list[28] = 0xDE; 
	parameter_list[29] = 0xE5; 
	parameter_list[30] = 0xEC; 
	parameter_list[31] = 0xF3; 
	parameter_list[32] = 0xF9; 
	parameter_list[33] = 0xFF; 
	parameter_list[34] = 0xDD;
	parameter_list[35] = 0x39; 
	parameter_list[36] = 0x07; 
	parameter_list[37] = 0x1C; 
	parameter_list[38] = 0xCB; 
	parameter_list[39] = 0xAB; 
	parameter_list[40] = 0x5F; 
	parameter_list[41] = 0x49; 
	parameter_list[42] = 0x80;   
	            
	parameter_list[43] = 0x03; //UPDATE THIS BLOCK
	parameter_list[44] = 0x07; 
	parameter_list[45] = 0x0F; 
	parameter_list[46] = 0x19; 
	parameter_list[47] = 0x20; 
	parameter_list[48] = 0x2A; 
	parameter_list[49] = 0x31; 
	parameter_list[50] = 0x39; 
	parameter_list[51] = 0x42; 
	parameter_list[52] = 0x4B; 
	parameter_list[53] = 0x53; 
	parameter_list[54] = 0x5B; 
	parameter_list[55] = 0x63; 
	parameter_list[56] = 0x6B; 
	parameter_list[57] = 0x73; 
	parameter_list[58] = 0x7B; 
	parameter_list[59] = 0x83; 
	parameter_list[60] = 0x8A; 
	parameter_list[61] = 0x92; 
	parameter_list[62] = 0x9B; 
	parameter_list[63] = 0xA2; 
	parameter_list[64] = 0xAA; 
	parameter_list[65] = 0xB2; 
	parameter_list[66] = 0xBA; 
	parameter_list[67] = 0xC2; 
	parameter_list[68] = 0xCA; 
	parameter_list[69] = 0xD0; 
	parameter_list[70] = 0xD8; 
	parameter_list[71] = 0xE1; 
	parameter_list[72] = 0xE8; 
	parameter_list[73] = 0xF0; 
	parameter_list[74] = 0xF8; 
	parameter_list[75] = 0xFF; 
	parameter_list[76] = 0xF7;
	parameter_list[77] = 0xD8; 
	parameter_list[78] = 0xBE; 
	parameter_list[79] = 0xA7; 
	parameter_list[80] = 0x39; 
	parameter_list[81] = 0x40; 
	parameter_list[82] = 0x85; 
	parameter_list[83] = 0x8C; 
	parameter_list[84] = 0xC0; 
	            
	parameter_list[85] = 0x04; //UPDATE THIS BLOCK
	parameter_list[86] = 0x07; 
	parameter_list[87] = 0x0C; 
	parameter_list[88] = 0x17; 
	parameter_list[89] = 0x1C; 
	parameter_list[90] = 0x23; 
	parameter_list[91] = 0x2B; 
	parameter_list[92] = 0x34; 
	parameter_list[93] = 0x3B; 
	parameter_list[94] = 0x43; 
	parameter_list[95] = 0x4C; 
	parameter_list[96] = 0x54; 
	parameter_list[97] = 0x5B; 
	parameter_list[98] = 0x63; 
	parameter_list[99] = 0x6A;
	parameter_list[100] = 0x73; 
	parameter_list[101] = 0x7A; 
	parameter_list[102] = 0x82; 
	parameter_list[103] = 0x8A; 
	parameter_list[104] = 0x91; 
	parameter_list[105] = 0x98; 
	parameter_list[106] = 0xA1; 
	parameter_list[107] = 0xA8; 
	parameter_list[108] = 0xB0; 
	parameter_list[109] = 0xB7; 
	parameter_list[110] = 0xC1; 
	parameter_list[111] = 0xC9; 
	parameter_list[112] = 0xCF; 
	parameter_list[113] = 0xD9; 
	parameter_list[114] = 0xE3; 
	parameter_list[115] = 0xEA; 
	parameter_list[116] = 0xF4; 
	parameter_list[117] = 0xFF; 
	parameter_list[118] = 0x00;
	parameter_list[119] = 0x00; 
	parameter_list[120] = 0x00; 
	parameter_list[121] = 0x00; 
	parameter_list[122] = 0x00; 
	parameter_list[123] = 0x00; 
	parameter_list[124] = 0x00; 
	parameter_list[125] = 0x00; 
	parameter_list[126] = 0x00; 

    send_spi_command(0xC1, 127, parameter_list);

	parameter_list[0] = 0x10;
	send_spi_command(0x36, 1, parameter_list);
	
    send_spi_command(0x11, 0, parameter_list);

    msleep(120);
                                       
    send_spi_command(0x29, 0, parameter_list);
	#else
	//#define GAMMA_2_5
	#define GAMMA_2_7
	//#define GAMMA_3_0

    parameter_list[0] = 0xFF;
    parameter_list[1] = 0x83;
    parameter_list[2] = 0x69;
    send_spi_command(0xB9, 3, parameter_list);

    parameter_list[0] = 0x00;
    parameter_list[1] = 0x2B;
    parameter_list[2] = 0x03;
    parameter_list[3] = 0x03;
    parameter_list[4] = 0x70;
    parameter_list[5] = 0x00;
    parameter_list[6] = 0xFF;
    parameter_list[7] = 0x00;
    parameter_list[8] = 0x00;
    parameter_list[9] = 0x00;
    parameter_list[10] = 0x00;
    parameter_list[11] = 0x03;
	parameter_list[12] = 0x03;
    parameter_list[13] = 0x00;
    parameter_list[14] = 0x01;
    send_spi_command(0xB2, 15, parameter_list);

    parameter_list[0] = 0x00;
    parameter_list[1] = 0x05;
    parameter_list[2] = 0x03;
    parameter_list[3] = 0x29;
    parameter_list[4] = 0x01;
    parameter_list[5] = 0x07;
    parameter_list[6] = 0x07;
    parameter_list[7] = 0x68;
    parameter_list[8] = 0x13;
    parameter_list[9] = 0x37;
    parameter_list[10] = 0x20;
    parameter_list[11] = 0x31;
	parameter_list[12] = 0x8A;
    parameter_list[13] = 0x46;
    parameter_list[14] = 0x9B;
	parameter_list[15] = 0x57;
    parameter_list[16] = 0x13;
    parameter_list[17] = 0x02;
    parameter_list[18] = 0x75;
    parameter_list[19] = 0xB9;
    parameter_list[20] = 0x64;
    parameter_list[21] = 0xA8;
    parameter_list[22] = 0x07;
    parameter_list[23] = 0x0F;
	#if defined(GAMMA_2_5) || defined(GAMMA_2_7) || defined(GAMMA_3_0)
    parameter_list[24] = 0x01;
	#else
    parameter_list[24] = 0x04;
    #endif
	parameter_list[25] = 0x07;
    send_spi_command(0xD5, 26, parameter_list);

    parameter_list[0] = 0x01;
    parameter_list[1] = 0x00;
    parameter_list[2] = 0x37;
    parameter_list[3] = 0x03;
    parameter_list[4] = 0x00;
    parameter_list[5] = 0x0E;
    parameter_list[6] = 0x0E;
    parameter_list[7] = 0x20;
    parameter_list[8] = 0x28;
    parameter_list[9] = 0x12;
    parameter_list[10] = 0x12;
    parameter_list[11] = 0x17;
	parameter_list[12] = 0x22;
    parameter_list[13] = 0x01;
    parameter_list[14] = 0xE6;
	parameter_list[15] = 0xE6;
    parameter_list[16] = 0xE6;
    parameter_list[17] = 0xE6;
    parameter_list[18] = 0xE6;
    send_spi_command(0xB1, 19, parameter_list);

    parameter_list[0] = 0x01;
	#if defined(GAMMA_2_5) || defined(GAMMA_2_7) || defined(GAMMA_3_0)
	parameter_list[1] = 0x02;
    parameter_list[2] = 0x7F;
    parameter_list[3] = 0x01;
    parameter_list[4] = 0x00;
	#else
    parameter_list[1] = 0x0E;
    parameter_list[2] = 0x77;
    parameter_list[3] = 0x0E;
    parameter_list[4] = 0x06;
	#endif
    send_spi_command(0xB4, 5, parameter_list);

    parameter_list[0] = 0x49;
	#if defined(GAMMA_2_5)
    parameter_list[1] = 0x32;
	#elif defined(GAMMA_2_7)
    parameter_list[1] = 0x2F;
	#elif defined(GAMMA_3_0)
    parameter_list[1] = 0x2C;
	#else
    parameter_list[1] = 0x36;
    #endif
	send_spi_command(0xB6, 2, parameter_list);

    parameter_list[0] = 0x0A;
	send_spi_command(0xCC, 1, parameter_list);

	//hhs 20101220 test DPL(b3)=0
    parameter_list[0] = 0x01;//0x09;
	send_spi_command(0xB3, 1, parameter_list);
	
    parameter_list[0] = 0x00;
    parameter_list[1] = 0x04;
    parameter_list[2] = 0x09;
    parameter_list[3] = 0x0F;
    parameter_list[4] = 0x1A;
    parameter_list[5] = 0x3F;
	#if defined(GAMMA_2_5)
	parameter_list[6] = 0x20;
    parameter_list[7] = 0x2F;
	#elif defined(GAMMA_2_7)
	parameter_list[6] = 0x21;
    parameter_list[7] = 0x35;
	#elif defined(GAMMA_3_0)
	parameter_list[6] = 0x25;
    parameter_list[7] = 0x3A;	
	#else
	parameter_list[6] = 0x18;
    parameter_list[7] = 0x2A;
    #endif
	parameter_list[8] = 0x07;
    parameter_list[9] = 0x0D;
    parameter_list[10] = 0x10;
    parameter_list[11] = 0x15;
	parameter_list[12] = 0x18;
    parameter_list[13] = 0x16;
    parameter_list[14] = 0x17;
	parameter_list[15] = 0x0D;
    parameter_list[16] = 0x15;
    parameter_list[17] = 0x00;
    parameter_list[18] = 0x04;
    parameter_list[19] = 0x09;
    parameter_list[20] = 0x0F;
    parameter_list[21] = 0x1A;
    parameter_list[22] = 0x3F;
	#if defined(GAMMA_2_5)
    parameter_list[23] = 0x20;
    parameter_list[24] = 0x39;
    #elif defined(GAMMA_2_7)
    parameter_list[23] = 0x21;
    parameter_list[24] = 0x3A;
    #elif defined(GAMMA_3_0)
    parameter_list[23] = 0x25;
    parameter_list[24] = 0x3E;
    #else
	parameter_list[23] = 0x18;
    parameter_list[24] = 0x38;
	#endif
    parameter_list[25] = 0x07;
    parameter_list[26] = 0x0D;
    parameter_list[27] = 0x10;
	parameter_list[28] = 0x15;
    parameter_list[29] = 0x18;
    parameter_list[30] = 0x16;
    parameter_list[31] = 0x17;
    parameter_list[32] = 0x0D;
    parameter_list[33] = 0x15;
    send_spi_command(0xE0, 34, parameter_list);

    parameter_list[0] = 0x01;
    parameter_list[1] = 0x05;
	parameter_list[2] = 0x07; 
	parameter_list[3] = 0x11; 
	parameter_list[4] = 0x1B; 
	parameter_list[5] = 0x23; 
	parameter_list[6] = 0x2C; 
	parameter_list[7] = 0x34; 
	parameter_list[8] = 0x3D; 
	parameter_list[9] = 0x47; 
	parameter_list[10] = 0x50; 
	parameter_list[11] = 0x58; 
	parameter_list[12] = 0x60; 
	parameter_list[13] = 0x69; 
	parameter_list[14] = 0x71; 
	parameter_list[15] = 0x79; 
	parameter_list[16] = 0x82; 
	parameter_list[17] = 0x89; 
	parameter_list[18] = 0x92; 
	parameter_list[19] = 0x9A; 
	parameter_list[20] = 0xA1; 
	parameter_list[21] = 0xA9; 
	parameter_list[22] = 0xB1; 
	parameter_list[23] = 0xB9; 
	parameter_list[24] = 0xC1; 
	parameter_list[25] = 0xC9; 
	parameter_list[26] = 0xCF; 
	parameter_list[27] = 0xD6; 
	parameter_list[28] = 0xDE; 
	parameter_list[29] = 0xE5; 
	parameter_list[30] = 0xEC; 
	parameter_list[31] = 0xF3; 
	parameter_list[32] = 0xF9; 
	parameter_list[33] = 0xFF; 
	parameter_list[34] = 0xDD;
	parameter_list[35] = 0x39; 
	parameter_list[36] = 0x07; 
	parameter_list[37] = 0x1C; 
	parameter_list[38] = 0xCB; 
	parameter_list[39] = 0xAB; 
	parameter_list[40] = 0x5F; 
	parameter_list[41] = 0x49; 
	parameter_list[42] = 0x80;   
	            
	parameter_list[43] = 0x02; //UPDATE THIS BLOCK
	parameter_list[44] = 0x06; 
	parameter_list[45] = 0x10; 
	parameter_list[46] = 0x19; 
	parameter_list[47] = 0x20; 
	parameter_list[48] = 0x2A; 
	parameter_list[49] = 0x32; 
	parameter_list[50] = 0x39; 
	parameter_list[51] = 0x42; 
	parameter_list[52] = 0x4B; 
	parameter_list[53] = 0x53; 
	parameter_list[54] = 0x5B; 
	parameter_list[55] = 0x63; 
	parameter_list[56] = 0x6B; 
	parameter_list[57] = 0x73; 
	parameter_list[58] = 0x7B; 
	parameter_list[59] = 0x83; 
	parameter_list[60] = 0x8A; 
	parameter_list[61] = 0x92; 
	parameter_list[62] = 0x9B; 
	parameter_list[63] = 0xA2; 
	parameter_list[64] = 0xAA; 
	parameter_list[65] = 0xB2; 
	parameter_list[66] = 0xBA; 
	parameter_list[67] = 0xC2; 
	parameter_list[68] = 0xCA; 
	parameter_list[69] = 0xD0; 
	parameter_list[70] = 0xD8; 
	parameter_list[71] = 0xE1; 
	parameter_list[72] = 0xE8; 
	parameter_list[73] = 0xF0; 
	parameter_list[74] = 0xF8; 
	parameter_list[75] = 0xFF; 
	parameter_list[76] = 0xF7;
	parameter_list[77] = 0xD8; 
	parameter_list[78] = 0xBE; 
	parameter_list[79] = 0xA7; 
	parameter_list[80] = 0x39; 
	parameter_list[81] = 0x40; 
	parameter_list[82] = 0x85; 
	parameter_list[83] = 0x8C; 
	parameter_list[84] = 0xC0; 
	            
	parameter_list[85] = 0x05; //UPDATE THIS BLOCK
	parameter_list[86] = 0x07; 
	parameter_list[87] = 0x11; 
	parameter_list[88] = 0x19; 
	parameter_list[89] = 0x1F; 
	parameter_list[90] = 0x28; 
	parameter_list[91] = 0x30; 
	parameter_list[92] = 0x37; 
	parameter_list[93] = 0x40; 
	parameter_list[94] = 0x49; 
	parameter_list[95] = 0x51; 
	parameter_list[96] = 0x59; 
	parameter_list[97] = 0x61; 
	parameter_list[98] = 0x69; 
	parameter_list[99] = 0x70; 
	parameter_list[100] = 0x78; 
	parameter_list[101] = 0x80; 
	parameter_list[102] = 0x87; 
	parameter_list[103] = 0x8F; 
	parameter_list[104] = 0x97; 
	parameter_list[105] = 0x9E; 
	parameter_list[106] = 0xA6; 
	parameter_list[107] = 0xAE; 
	parameter_list[108] = 0xB6; 
	parameter_list[109] = 0xBE; 
	parameter_list[110] = 0xC7; 
	parameter_list[111] = 0xCD; 
	parameter_list[112] = 0xD5; 
	parameter_list[113] = 0xDF; 
	parameter_list[114] = 0xE6; 
	parameter_list[115] = 0xEF; 
	parameter_list[116] = 0xF7; 
	parameter_list[117] = 0xFF; 
	parameter_list[118] = 0x51;
	parameter_list[119] = 0xFE; 
	parameter_list[120] = 0x19; 
	parameter_list[121] = 0x08; 
	parameter_list[122] = 0x11; 
	parameter_list[123] = 0xD5; 
	parameter_list[124] = 0x9A; 
	parameter_list[125] = 0x17; 
	parameter_list[126] = 0xC0; 

    send_spi_command(0xC1, 127, parameter_list);

	parameter_list[0] = 0x10;
	send_spi_command(0x36, 1, parameter_list);
	
    send_spi_command(0x11, 0, parameter_list);

    mdelay(120);
	
    parameter_list[0] = 0x77;
    send_spi_command(0x3A, 1, parameter_list);
                                       
    send_spi_command(0x29, 0, parameter_list);
	#endif
	#endif
}

static void spi_init(void)
{
	ENTER_FUNC2();
	/* Set the output so that we dont disturb the slave device */    
	gpio_set_value(SPI_SCLK, GPIO_LOW_VALUE);
    gpio_set_value(SPI_SDI, GPIO_LOW_VALUE);
        
	/* Set the Chip Select De-asserted */
	gpio_set_value(SPI_CS, GPIO_HIGH_VALUE);
	EXIT_FUNC2();
}

static uint32_t lcdc_gpio_init_enable[] = {
	GPIO_CFG(0,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_pclk */
	GPIO_CFG(1,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_hsync*/
	GPIO_CFG(2,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_vsync*/
	GPIO_CFG(3,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_den */
	GPIO_CFG(4,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red7 */
	GPIO_CFG(5,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red6 */
	GPIO_CFG(6,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red5 */
	GPIO_CFG(7,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red4 */
	GPIO_CFG(8,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red3 */
	GPIO_CFG(9,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red2 */
	GPIO_CFG(10,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red1 */
	GPIO_CFG(11,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red0 */
	GPIO_CFG(12,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn7 */
	GPIO_CFG(13,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn6 */
	GPIO_CFG(14,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn5 */
	GPIO_CFG(15,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn4 */
	GPIO_CFG(16,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn3 */
	GPIO_CFG(17,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn2 */
	GPIO_CFG(18,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn1 */
	GPIO_CFG(19,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn0 */
	GPIO_CFG(20,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu7 */
	GPIO_CFG(21,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu6 */
	GPIO_CFG(22,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu5 */
	GPIO_CFG(23,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu4 */
	GPIO_CFG(24,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu3 */
	GPIO_CFG(25,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu2 */
	GPIO_CFG(26,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu1 */
	GPIO_CFG(27,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu0 */
};

static uint32_t lcdc_gpio_init_disable[] = {
	GPIO_CFG(0,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_pclk */
	GPIO_CFG(1,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_hsync*/
	GPIO_CFG(2,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_vsync*/
	GPIO_CFG(3,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_den */
	GPIO_CFG(4,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_red7 */
	GPIO_CFG(5,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_red6 */
	GPIO_CFG(6,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_red5 */
	GPIO_CFG(7,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_red4 */
	GPIO_CFG(8,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_red3 */
	GPIO_CFG(9,          0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_red2 */
	GPIO_CFG(10,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_red1 */
	GPIO_CFG(11,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_red0 */
	GPIO_CFG(12,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_grn7 */
	GPIO_CFG(13,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_grn6 */
	GPIO_CFG(14,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_grn5 */
	GPIO_CFG(15,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_grn4 */
	GPIO_CFG(16,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_grn3 */
	GPIO_CFG(17,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_grn2 */
	GPIO_CFG(18,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_grn1 */
	GPIO_CFG(19,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_grn0 */
	GPIO_CFG(20,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_blu7 */
	GPIO_CFG(21,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_blu6 */
	GPIO_CFG(22,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_blu5 */
	GPIO_CFG(23,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_blu4 */
	GPIO_CFG(24,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_blu3 */
	GPIO_CFG(25,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_blu2 */
	GPIO_CFG(26,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_blu1 */
	GPIO_CFG(27,         0, GPIO_CFG_OUTPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* lcdc_blu0 */
};

static bool flag_sky_panel_off = FALSE;
static int once=0;
static int panel_on_status=0;
/* AT1 : 20110625 add for wifi dirver loading issue */
int get_lcd_status(void) { return flag_sky_panel_off;} 
EXPORT_SYMBOL(get_lcd_status);

static int lcdc_sky_panel_on(struct platform_device *pdev)
{
	ENTER_FUNC2();   
	if (flag_sky_panel_off || once==0)
	{
	   if(once==0)
	   {
	   	  lcd_gpio_init(backlight_gpio_init_table, ARRAY_SIZE(backlight_gpio_init_table), 1);
		  gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);
		  gpio_set_value(LCD_BL_SCL, GPIO_LOW_VALUE);   
		  led_gpio_i2c_set(0x00, 0x00);
		  once=1;
	   }
	   //flag_sky_panel_off = FALSE;
	   gpio_set_value(SPI_CS, GPIO_HIGH_VALUE);
	   gpio_set_value(LCD_RESET, GPIO_HIGH_VALUE);
	   //gpio_set_value(VEE_ACTIVE, GPIO_HIGH_VALUE);
	   lcd_gpio_init(lcdc_gpio_init_enable, ARRAY_SIZE(lcdc_gpio_init_enable), 1);
	   lcd_setup();
	   //msleep(50);
	   gpio_set_value(VEE_ACTIVE, GPIO_HIGH_VALUE);
	   panel_on_status=1;
	}
	flag_sky_panel_off = FALSE;

	 //20110725 bandit block charging current 700ma at lcd on 
	 #if 0//def CONFIG_SKY_CHARGING
       msm_charger_set_lcd_onoff(true);
	#endif
	EXIT_FUNC2();
	return 0;
}

static int lcdc_sky_panel_off(struct platform_device *pdev)
{
    ENTER_FUNC2(); 

     //20110725 bandit block charging current 700ma at lcd on 
     #if 0//def CONFIG_SKY_CHARGING
    msm_charger_set_lcd_onoff(false);
    #endif
    
	flag_sky_panel_off = TRUE;  
#if defined(FEATURE_SKY_BACKLIGHT_MAX8831)   
    backlight_off();  
#endif
#if defined(LCDC_SKY_LCD_LMS501KF02) 
	//gpio_set_value(VEE_VLP, GPIO_LOW_VALUE);  //VEE_VLP
	send_spi_command(0x10, 0, parameter_list);
    msleep(120);
	lcd_gpio_init(lcdc_gpio_init_disable, ARRAY_SIZE(lcdc_gpio_init_disable), 1);
    gpio_set_value(SPI_CS, GPIO_LOW_VALUE);
	gpio_set_value(LCD_RESET, GPIO_LOW_VALUE);
    gpio_tlmm_config(GPIO_CFG(0, 0,GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),GPIO_CFG_ENABLE);
	gpio_set_value(0, GPIO_LOW_VALUE);
	//msleep(5);
	gpio_set_value(VEE_ACTIVE, GPIO_LOW_VALUE);
#endif
    panel_on_status=0;
    EXIT_FUNC2();
    return 0;
}
#if defined(GPIO_I2C_LED_CONTROL)
static int lcdc_sky_set_once=0;
static int lcdc_sky_set_once1=0;
#endif
#include <mach/msm_iomap.h>
#define MSM_TLMM_BASE1		IOMEM(0xFA004000)
#define GPIO_CONFIG1(gpio)         (MSM_TLMM_BASE1 + 0x1000 + (0x10 * (gpio)))

static void lcdc_sky_set_backlight(struct msm_fb_data_type *mfd)
{
#if defined(FEATURE_SKY_BACKLIGHT_MAX8831)
	boolean ret = TRUE;
    //   int dimming_enable_counter;
    //   int dimming_control_bit;

	//ENTER_FUNC2();   

	if (mfd->bl_level ==0) 
	{
		backlight_off();
		flag_lcd_bl_ctrl = TRUE;
	}
    else
    {
		if (flag_lcd_bl_off)
		{
			flag_lcd_bl_off = FALSE;
		}       

		if (flag_lcd_bl_ctrl)
		{
		    #if defined(FEATURE_SKY_BACKLIGHT_MAX8831)
                #if defined(GPIO_I2C_LED_CONTROL)
				if (lcdc_sky_set_once==0)
				{
				    #if 0
					lcd_gpio_init(backlight_gpio_init_table, ARRAY_SIZE(backlight_gpio_init_table), 1);
					gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);
					gpio_set_value(LCD_BL_SCL, GPIO_LOW_VALUE);   
					led_gpio_i2c_set(0x00, 0x03);
					led_gpio_i2c_set(0x0B, 0x6A);
					led_gpio_i2c_set(0x0C, 0x6A);
					//gpio_free(LCD_BL_SDA);
					//gpio_free(LCD_BL_SCL);
                    #endif
					lcdc_sky_set_once=1;
				}
				else
				{
                #endif
		            ret = led_i2c_master_send(0x00, 0x03);
				    ret = led_i2c_master_send(0x0B, 0x6A);
		    	    ret = led_i2c_master_send(0x0C, 0x6A);
				#if defined(GPIO_I2C_LED_CONTROL)
				}
				#endif
			#endif
			flag_lcd_bl_ctrl = FALSE;
		}
		#if defined(FEATURE_SKY_BACKLIGHT_MAX8831)
		{
	      static uint8 max_bl_level=0;
		  printk(KERN_ERR "[backlight control level=%d=]\n",mfd->bl_level);
          #if defined(GPIO_I2C_LED_CONTROL)
		  if (lcdc_sky_set_once1==0)
		  {
			  //lcd_gpio_init(backlight_gpio_init_table, ARRAY_SIZE(backlight_gpio_init_table), 1);
			  //gpio_set_value(LCD_BL_SDA, GPIO_LOW_VALUE);
			  //gpio_set_value(LCD_BL_SCL, GPIO_LOW_VALUE);   
			  //led_gpio_i2c_set(0x00, 0x03);
			  //hhs 20110617 at1 lcd tearing issue
              msleep(30);
              panel_on_status=0;
			  ret = led_i2c_master_send(0x0B, 0x47);
	          ret = led_i2c_master_send(0x0C, 0x47);
			  ret = led_i2c_master_send(0x00, 0x03);
			  vee_i2c_gpio_master_send();  //vee register init
			  //gpio_free(LCD_BL_SDA);
			  //gpio_free(LCD_BL_SCL);
			  lcdc_sky_set_once1=1;
		  }
		  else
		  {
		  //hhs 20110617 at1 lcd tearing issue
		  if(panel_on_status==1)
		  {
            msleep(20);
            panel_on_status=0;
		  }
          #endif
		  #if 0
		  if( (mfd->bl_level == 16) || (mfd->bl_level == 15) )
	        max_bl_level = 0x6A;
		  else if(mfd->bl_level == 14)
	        max_bl_level = 0x64;
		  else if(mfd->bl_level == 13)
	        max_bl_level = 0x5E;
		  else if(mfd->bl_level == 12)
	        max_bl_level = 0x58;
		  else if(mfd->bl_level == 11)
	        max_bl_level = 0x4D;
		  else if(mfd->bl_level == 10)
	        max_bl_level = 0x47;
		  else if(mfd->bl_level == 9)
	        max_bl_level = 0x41;
		  else if(mfd->bl_level == 8)
	        max_bl_level = 0x3B;
		  else if(mfd->bl_level == 7)
	        max_bl_level = 0x35;
		  else if(mfd->bl_level == 6)
	        max_bl_level = 0x32;
		  else if(mfd->bl_level == 5)
	        max_bl_level = 0x2D;
		  else if(mfd->bl_level == 4)
	        max_bl_level = 0x25;
		  else if(mfd->bl_level == 3)
	        max_bl_level = 0x20;
		  else
	        max_bl_level = 0x1B;
		  #else
		  if(mfd->bl_level == 16) 
	        max_bl_level = 0x5E;
		  else if (mfd->bl_level == 15)
		  	max_bl_level = 0x58;
		  else if(mfd->bl_level == 14)
	        max_bl_level = 0x53;
		  else if(mfd->bl_level == 13)
	        max_bl_level = 0x4D;
		  else if(mfd->bl_level == 12)
	        max_bl_level = 0x47;
		  else if(mfd->bl_level == 11)
	        max_bl_level = 0x41;
		  else if(mfd->bl_level == 10)
	        max_bl_level = 0x3B;
		  else if(mfd->bl_level == 9)
	        max_bl_level = 0x35;
		  else if(mfd->bl_level == 8)
	        max_bl_level = 0x31;
		  else if(mfd->bl_level == 7)
	        max_bl_level = 0x2D;
		  else if(mfd->bl_level == 6)
	        max_bl_level = 0x29;
		  else if(mfd->bl_level == 5)
	        max_bl_level = 0x25;
		  else if(mfd->bl_level == 4)
	        max_bl_level = 0x21;
		  else if(mfd->bl_level == 3)
	        max_bl_level = 0x1C;
		  else if(mfd->bl_level == 2)
	        max_bl_level = 0x17;
		  else
	        max_bl_level = 0x12;
		  #endif

		  ret = led_i2c_master_send(0x0B, max_bl_level);
	      ret = led_i2c_master_send(0x0C, max_bl_level);
		  ret = led_i2c_master_send(0x00, 0x03);
		  #if 0
          {
            uint32_t flags;
            flags=readl(GPIO_CONFIG1(7));
			if(flags==512)
				writel(452, GPIO_CONFIG1(7));
		    printk(KERN_ERR "hhs gpio_config(7)=%d=\n", flags);
    	  }
		  #endif
		  #if defined(GPIO_I2C_LED_CONTROL)
		  }
		  #endif
		  #if 0
		  if(max_bl_level == 0x6A)
		  {
		      //vee on
			  vee_i2c_master_send(0x00000000, 0x00000007);

			  vee_i2c_master_send(0x00030020, 0x00001FC0);
	          //width size 0x1EC => 492
			  vee_i2c_master_send(0x00000034, 0x00000001);
			  vee_i2c_master_send(0x00000038, 0x000000EC);
			  //height size 0x320 => 800
			  vee_i2c_master_send(0x0000003C, 0x00000003);
			  vee_i2c_master_send(0x00000040, 0x00000020);
	          //control register1 
			  vee_i2c_master_send(0x00000048, 0x00000025);
			  //horizontal position register 80
			  vee_i2c_master_send(0x00000028, 0x00000050);
	          #if 0
			  //indoor mode
	          //gamma   0x7: off  0xF: on
			  boot_vee_write(0x0000004C, 0x00000007);
			  //vee strength  8/31= 25%
			  //boot_vee_write(0x0000000C, 0x00000040);
			  //default
	          //boot_vee_write(0x0000000C, 0x00000060);
	          //66%
			  //boot_vee_write(0x0000000C, 0x000000A8);
	          //55% 
			  boot_vee_write(0x0000000C, 0x00000088);
			  #else
	          //outdoor mode
	          //gamma   0x7: off  0xF: on
			  vee_i2c_master_send(0x0000004C, 0x0000000F);
			  //vee strength
	          vee_i2c_master_send(0x0000000C, 0x00000060);
			  #endif
		  }
		  #endif
	      #endif
          }
   	}	   
	//EXIT_FUNC2();
#endif  
}
#if 1
static int lcdc_sky_probe(struct platform_device *pdev)
{
	ENTER_FUNC2();
	if (pdev->id == 0) {
		lcdc_sky_pdata = pdev->dev.platform_data;
		return 0;
	}
	msm_fb_add_device(pdev);
	EXIT_FUNC2();
	return 0;
}
#else
static int lcdc_sky_probe(struct platform_device *pdev)
{
	int rc = 0;

    ENTER_FUNC2();
	if (pdev->id == 0) {
		dd = kzalloc(sizeof *dd, GFP_KERNEL);
		if (!dd)
			return -ENOMEM;
		dd->pdata = pdev->dev.platform_data;
		return 0;
	} else if (!dd)
		return -ENODEV;


	dd->fbpdev = msm_fb_add_device(pdev);
	if (!dd->fbpdev) {
		dev_err(&pdev->dev, "failed to add msm_fb device\n");
		rc = -ENODEV;
		goto probe_exit;
	}
	EXIT_FUNC2();
probe_exit:
	return rc;
}
#endif

static struct platform_driver this_driver = {
	.probe  = lcdc_sky_probe,
	.driver = {
		.name   = "lcdc_sky_lcd",
	},
};

static struct msm_fb_panel_data lcdc_sky_panel_data = {
	.on = lcdc_sky_panel_on,
	.off = lcdc_sky_panel_off,
	.set_backlight = lcdc_sky_set_backlight,
};

static struct platform_device this_device = {
	.name   = "lcdc_sky_lcd",
	.id	= 1,
	.dev	= {
		.platform_data = &lcdc_sky_panel_data,
	}
};

static uint32_t lcd_gpio_init_table[] = {
	GPIO_CFG(SPI_SCLK,   0, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(SPI_CS,     0, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(SPI_SDI,    0, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(LCD_RESET,  0, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(VEE_RESET,  0, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(VEE_ACTIVE, 0, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(VEE_RGB_OE, 0, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(VEE_VLP,    0, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(0,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_pclk */
	GPIO_CFG(1,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_hsync*/
	GPIO_CFG(2,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_vsync*/
	GPIO_CFG(3,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_den */
	GPIO_CFG(4,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red7 */
	GPIO_CFG(5,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red6 */
	GPIO_CFG(6,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red5 */
	GPIO_CFG(7,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red4 */
	GPIO_CFG(8,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red3 */
	GPIO_CFG(9,          1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red2 */
	GPIO_CFG(10,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red1 */
	GPIO_CFG(11,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_red0 */
	GPIO_CFG(12,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn7 */
	GPIO_CFG(13,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn6 */
	GPIO_CFG(14,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn5 */
	GPIO_CFG(15,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn4 */
	GPIO_CFG(16,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn3 */
	GPIO_CFG(17,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn2 */
	GPIO_CFG(18,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn1 */
	GPIO_CFG(19,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_grn0 */
	GPIO_CFG(20,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu7 */
	GPIO_CFG(21,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu6 */
	GPIO_CFG(22,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu5 */
	GPIO_CFG(23,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu4 */
	GPIO_CFG(24,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu3 */
	GPIO_CFG(25,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu2 */
	GPIO_CFG(26,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu1 */
	GPIO_CFG(27,         1, GPIO_CFG_OUTPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* lcdc_blu0 */
};

static int __init lcdc_sky_init(void)
{
	int ret;
	struct msm_panel_info *pinfo;

	ENTER_FUNC2();

	lcd_gpio_init(lcd_gpio_init_table, ARRAY_SIZE(lcd_gpio_init_table), 1);
	
    // SPI Output Init.
    spi_init();
    #if defined(LCDC_SKY_LCD_LMS501KF02) 
    gpio_set_value(VEE_VLP, GPIO_HIGH_VALUE);  //VEE_VLP
    gpio_set_value(VEE_ACTIVE, GPIO_LOW_VALUE);
    gpio_set_value(VEE_RGB_OE, GPIO_LOW_VALUE);
    #endif  
    #if 1//AT1_BDVER_GE(AT1_WS10)
    gpio_set_value(VEE_RESET, GPIO_HIGH_VALUE);
    #endif
    gpio_set_value(LCD_RESET, GPIO_HIGH_VALUE);
	udelay(2);
	gpio_set_value(VEE_RESET, GPIO_LOW_VALUE);
	udelay(2);
	gpio_set_value(VEE_RESET, GPIO_HIGH_VALUE);
#if defined(FEATURE_SKY_BACKLIGHT_MAX8831)   
    led_i2c_api_Init();
#endif
#ifdef FEATURE_VEE_I2C_CONTROL
    vee_i2c_api_Init();
#endif
	ret = platform_driver_register(&this_driver);
	if (ret)
		return ret;

	pinfo = &lcdc_sky_panel_data.panel_info;
	pinfo->xres = 480;
	pinfo->yres = 800;
	pinfo->type = LCDC_PANEL;
	pinfo->pdest = DISPLAY_1;
	pinfo->bl_max = 16;
    pinfo->bl_min = 1;
	pinfo->wait_cycle = 0;
	pinfo->bpp = 24;
	pinfo->fb_num = 2;
	pinfo->clk_rate = 24000000;//24500000;
	#if defined(LCDC_SKY_LCD_LMS501KF02)
	pinfo->lcdc.h_back_porch = 8;
	pinfo->lcdc.h_front_porch = 8;	
	pinfo->lcdc.v_back_porch = 6;	
	pinfo->lcdc.v_front_porch = 6;
	pinfo->lcdc.h_pulse_width = 6;	
	pinfo->lcdc.v_pulse_width = 4;
	#endif
	pinfo->lcdc.border_clr = 0;	/* blk */
	pinfo->lcdc.underflow_clr = 0x00; /* black */
	pinfo->lcdc.hsync_skew = 0;

	ret = platform_device_register(&this_device);
	if (ret)
		platform_driver_unregister(&this_driver);

	// Initial Seq.
	//lcd_setup();
	#if 0
	{
		static struct regulator *l2_3p0v;
		uint8_t rc=0;
		
		l2_3p0v = regulator_get(NULL, "8058_l2");
		if (!l2_3p0v)
			goto out;
		
		rc = regulator_set_voltage(l2_3p0v, 3000000, 3000000);
		if (rc)
		  goto out;

		rc = regulator_enable(l2_3p0v);
		if (rc)
		  goto out;

		printk(KERN_ERR "%s: 8058_l12 3v enable\n", __func__);
		
	out:
		regulator_put(l2_3p0v);

		l2_3p0v=NULL;
	}
    #endif
	EXIT_FUNC2();
	return ret;
}

int sky_lcd_screen_off(void)
{
    ENTER_FUNC2();
	#if defined(LCDC_SKY_LCD_LMS501KF02)
    send_spi_command(0x10, 0, parameter_list);
	gpio_set_value(VEE_VLP, GPIO_LOW_VALUE);  //VEE_VLP
	#if 1//AT1_BDVER_GE(AT1_WS10)
	gpio_set_value(VEE_RESET, GPIO_LOW_VALUE);
    #endif
	gpio_set_value(LCD_RESET, GPIO_LOW_VALUE);
	#endif
	msleep(40);//mdelay(20); //wait for several frame :: guide
    flag_sky_panel_off = TRUE; 
    EXIT_FUNC2();
    return 0;
}
void force_lcd_screen_on(void)
{
    ENTER_FUNC2(); 
    //lcd_gpio_init(lcd_gpio_init_table, ARRAY_SIZE(lcd_gpio_init_table), 1);
    //spi_init();
    #if 1//AT1_BDVER_GE(AT1_WS10)
	gpio_set_value(VEE_RESET, GPIO_HIGH_VALUE);
    #endif
    gpio_set_value(LCD_RESET, GPIO_HIGH_VALUE);
    //led_i2c_api_Init();
    lcd_setup();
    backlight_on();
    EXIT_FUNC2();
}
EXPORT_SYMBOL(force_lcd_screen_on);
module_init(lcdc_sky_init);
