/* definition for PrintRoid (Media Transfer Protocol)
 *
 * $Id: f_printRoid.h, v1.01, 2010.04.08:15:28:582$
 *
 * Initial work by:
 *   (c) 2007 Insight-kr Lee Hyun Wook (wondo@insight-kr.com)
 *
                        EDIT HISTORY FOR MODULE
 
when            who          what, where, why
--------      ------     ----------------------------------------------------------
2010/11/27   wondo      Initial work............^^"



-----------------------------------------------------------------------------*/
#ifndef __USB_FUNCTION_PRINTROID_H__
#define __USB_FUNCTION_PRINTROID_H__   


/* customer Define----------------------------------------------*/
#define CUSTOM_DEVICE	1  //..for pantech



/* wondo Define------------------------------------------------*/
#define DBG_NONE	0
#define DBG_INFO	1
#define DBG_SETUP	2
#define DBG_RX		3
#define DBG_TX		4

#define DBG_ERR	200
#define DBG_CHK	255
/*---------------------------*/

//..#define __DEBUG
#undef __DEBUG
#ifdef __DEBUG
#define dbglog(xx, format, arg...) if(xx == DBG_NONE) do {} while (0); \
								else if(xx == DBG_INFO) printk("pR: info: " format "\n" , ## arg); \
								else  if(xx == DBG_SETUP) printk("pR: setup: " format "\n" , ## arg); \
								else  if(xx == DBG_RX) printk("pR: Rx: " format "\n" , ## arg); \
								else  if(xx == DBG_TX) printk("pR: Tx: " format "\n" , ## arg); \
								else  if(xx == DBG_ERR) printk("pR: err: " format "\n" , ## arg); \
								else  if(xx == DBG_CHK) printk("pR: chk> " format "\n" , ## arg)
#else
#define dbglog(xx, format, arg...) do {} while (0)
#endif

#define true 1
#define false 0

/*------------------------------------------------------------*/
typedef struct{
	int size;
	int result;
	char data[17];
}__attribute__ ((packed)) INTERRUPT_DATA_TYPE;

/*------------------------------------------------------------*/
#define PRINTROID_SET_STATUS			_IOW('p', 0x21, int)
#define PRINTROID_GET_STATUS			_IOR('p', 0x22, int *)
#define PRINTROID_INTERRUPT			_IOWR('p', 0x23, INTERRUPT_DATA_TYPE)
#define PRINTROID_GET_SPEED			_IOR('p', 0x24, int *)

typedef enum{
	UP_NONE,
	UP_START,
	UP_ACTIVE,
	UP_DEACTIVE
}USB_PRINTROID_STATE;
/*------------------------------------------------------------*/

//..int f_printroid_function_init(void);

int f_printroid_function_add(struct usb_composite_dev *cdev,	struct usb_configuration *c);
void f_printroid_function_enable(int enable);

#endif  /* __USB_FUNCTION_PRINTROID_H__ */

