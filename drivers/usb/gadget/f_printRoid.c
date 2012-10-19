/* Gadget Driver for Android PrintRoid (direct printering)
 *
 * $Id: f_printRoid.c, v1.00, 2010.04.08:15:27:22$
 *
 * Initial work by:
 *   (c) 2007 Insight-kr Lee Hyun Wook (wondo@insight-kr.com)
 *
                        EDIT HISTORY FOR MODULE
 
when            who          what, where, why
--------      ------     ----------------------------------------------------------
2010/11/27   wondo      Initial work............^^"
2010/12/08   wondo      msm으로 변경작업...
2010/12/28   wondo      안정화 작업


-----------------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/err.h>
#include <linux/interrupt.h>

#include <linux/types.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#include <linux/usb/android_composite.h>

#include "f_printRoid.h"
#include "insightComp.h"

#include "gadget_chips.h"

/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/

#define DRIVER_DESC		"printRoid"
#define DRIVER_VERSION	"printRoid v1.0"

static const char shortname[] = "printRoid";
static const char driver_desc [] = DRIVER_DESC;
/*-------------------------------------------------------------------------*/
#if CUSTOM_DEVICE
#define PRINTROID_VENDOR_NUM		0x10A9
#define PRINTROID_PRODUCT_NUM		0x3901
#else
#define PRINTROID_VENDOR_NUM		0x0525
#define PRINTROID_PRODUCT_NUM		0x0005
#endif

#define STRING_MANUFACTURER		1
#define STRING_PRODUCT			2
#define STRING_SERIALNUM		3

#define DEV_CONFIG_VALUE		1

#define USB_DESC_BUFSIZE		256
#define INTERRUPT_BUFFER_SIZE	4096
#define BULK_BUFFER_SIZE		8192  //..wondo 2010/07/28  4096  -->  8192

/* number of rx and tx requests to allocate */
#define RX_REQ_MAX 4
#define RX_REQ_MAX_2 2  //..wondo 2010/07/27  2  -->  1
#define TX_REQ_MAX 2  //..wondo 2010/07/28  4  -->  2


struct f_printroid_dev {
	struct usb_function function;
	struct usb_composite_dev *cdev;
	spinlock_t lock;


	struct usb_ep *ep_int;
	struct usb_ep *ep_in;
	struct usb_ep *ep_out;

	int online;
	int error;
	int suspend;

	atomic_t read_excl;
	atomic_t write_excl;
	atomic_t write_interrupt_excl;
	atomic_t open_excl;

	struct list_head tx_idle;
	struct list_head tx_interrupt_idle;
	struct list_head rx_idle;
	struct list_head rx_done;

	wait_queue_head_t read_wq;
	wait_queue_head_t write_wq;
	wait_queue_head_t write_interrupt_wq;

	/* the request we're currently reading from */
	struct usb_request *read_req;
	unsigned char *read_buf;
	unsigned read_count;

	//......................................//
	USB_PRINTROID_STATE state;
	u8 config;
	u16 interface;
	const struct usb_endpoint_descriptor *interrupt, *in, *out;

	u8 sendObj_Cancel;
	
};

/*-------------------------------------------------------------------------*/
static struct usb_device_descriptor device_desc = {
	.bLength =		sizeof device_desc,
	.bDescriptorType =	USB_DT_DEVICE,
	.bcdUSB =		__constant_cpu_to_le16(0x0200),
	.bDeviceClass =		USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass =	0,
	.bDeviceProtocol =	0,
	.bMaxPacketSize0 = __constant_cpu_to_le16(64),
	.idVendor =		__constant_cpu_to_le16(PRINTROID_VENDOR_NUM),
	.idProduct =		__constant_cpu_to_le16(PRINTROID_PRODUCT_NUM),
	.iManufacturer =	STRING_MANUFACTURER,
	.iProduct =		STRING_PRODUCT,
	.iSerialNumber =	STRING_SERIALNUM,
	.bNumConfigurations =	1
};

static struct usb_config_descriptor printroid_configuration_desc = {
	.bLength =		sizeof printroid_configuration_desc,
	.bDescriptorType =	USB_DT_CONFIG,

	/* compute wTotalLength on the fly */
	.bNumInterfaces =	1,
	.bConfigurationValue =	DEV_CONFIG_VALUE,
	.iConfiguration =	0,
	.bmAttributes =		USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower	= 0xFA, /* 500ma */
};
/*-------------------------------------------------------------------------*/
static struct usb_otg_descriptor otg_desc = {
	.bLength =		sizeof otg_desc,
	.bDescriptorType =	USB_DT_OTG,
	.bmAttributes =		USB_OTG_SRP
};

static struct usb_interface_descriptor printroid_interface_desc = {
	.bLength                = USB_DT_INTERFACE_SIZE,
	.bDescriptorType        = USB_DT_INTERFACE,
	.bInterfaceNumber       = 0,
	.bNumEndpoints          = 3,
	.bInterfaceClass        = USB_CLASS_STILL_IMAGE,
	.bInterfaceSubClass     = 1,
	.bInterfaceProtocol     = 1,
};

static struct usb_endpoint_descriptor hs_printroid_interrupt_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_IN,
	.bmAttributes           = USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize         = __constant_cpu_to_le16(16),
	.bInterval = 0x0C,
};

static struct usb_endpoint_descriptor hs_printroid_in_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_IN,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize         = __constant_cpu_to_le16(512),
};

static struct usb_endpoint_descriptor hs_printroid_out_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_OUT,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize         = __constant_cpu_to_le16(512),
};

//=============================================//

static struct usb_endpoint_descriptor fs_printroid_interrupt_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_IN,
	.bmAttributes           = USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize         = __constant_cpu_to_le16(16),
	.bInterval = 0x0C,	
};

static struct usb_endpoint_descriptor fs_printroid_in_desc = {  //..wMaxPacketSize other 0x40(64)
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_IN,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor fs_printroid_out_desc = {  //..wMaxPacketSize other 0x40(64)
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_OUT,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
};

static struct usb_qualifier_descriptor dev_qualifier = {
	.bLength =		sizeof dev_qualifier,
	.bDescriptorType =	USB_DT_DEVICE_QUALIFIER,
	.bcdUSB =		__constant_cpu_to_le16(0x0200),
	.bDeviceClass =		USB_CLASS_STILL_IMAGE,
	.bNumConfigurations =	1
};

static struct usb_descriptor_header *fs_printroid_descs[] = {
//..>> wondo..test	(struct usb_descriptor_header *) &otg_desc,
	(struct usb_descriptor_header *) &printroid_interface_desc,
	(struct usb_descriptor_header *) &fs_printroid_interrupt_desc,
	(struct usb_descriptor_header *) &fs_printroid_in_desc,
	(struct usb_descriptor_header *) &fs_printroid_out_desc,
	NULL,
};

static struct usb_descriptor_header *hs_printroid_descs[] = {
//..>> wondo..test	(struct usb_descriptor_header *) &otg_desc,
	(struct usb_descriptor_header *) &printroid_interface_desc,
	(struct usb_descriptor_header *) &hs_printroid_interrupt_desc,
	(struct usb_descriptor_header *) &hs_printroid_in_desc,
	(struct usb_descriptor_header *) &hs_printroid_out_desc,
	NULL,
};

/* used when adb function is disabled */
static struct usb_descriptor_header *null_printroid_descs[] = {
	NULL,
};

/* maxpacket and other transfer characteristics vary by speed. */
#define ep_desc(g, hs, fs) (((g)->speed == USB_SPEED_HIGH)?(hs):(fs))


/*-------------------------------------------------------------------------*/
#if CUSTOM_DEVICE
/* manufacturer */
static char manufacturer[] = "INSIGHT";
/* product */
static const char product_desc[] = "INSIGHT PRINTROID";
/* serial */
static char serial_num[] = "2010-12-07.0123456789";
#else
/* manufacturer */
static char manufacturer[] = "INSIGHT";
/* product */
static const char product_desc[] = "INSIGHT PRINTROID";
/* serial */
static char serial_num[] = "2010-12-07.0123456789";
#endif

/*.x
static char				pnp_string [1024] =
	"XXMFG:linux;MDL:PrintRoid;CLS:PRINTER;SN:1;";
*/

/* static strings, in UTF-8 */
static struct usb_string		strings [] = {
	{ STRING_MANUFACTURER,	manufacturer, },
	{ STRING_PRODUCT,	product_desc, },
	{ STRING_SERIALNUM,	serial_num, },
	{  }		/* end of list */
};

static struct usb_gadget_strings	stringtab = {
	.language	= 0x0409,	/* en-us */
	.strings	= strings,
};

/*-------------------------------------------------------------------------*/


//===============================================
//===============================================
//===============================================
//===============================================
//===============================================
//===============================================
//===============================================
//===============================================


/* temporary variable used between adb_open() and adb_gadget_bind() */
struct usb_function *f_printroid = NULL;
static atomic_t printroid_enable_excl;
static struct f_printroid_dev *__f_printroid_dev;

//===============================================
// - printRoid Command Define
#define USB_PRINTROID_NONE			0x0000
#define USB_PRINTROID_ENABLE		0x0001  

#define PRINTROI_USB_1_1		1
#define PRINTROI_USB_2_0		2

//===============================================
static int g_printRoid_usb = 0;
static int g_printRoid_Enable = 0;
static int g_USB_PRINTROID_CMD = USB_PRINTROID_NONE;

//===============================================
#define ADD_USB_PRINTROID_CMD(x) g_USB_PRINTROID_CMD |= x;
#define DEL_USB_PRINTROID_CMD(x) g_USB_PRINTROID_CMD &= ~x;

//===============================================
//===============================================
//===============================================
//===============================================

/*-------------------------------------------------------------------------*/
#define __DEBUG_DUMP

#ifdef __DEBUG_DUMP
void _dbgdump(int type, char *data, int len)
{
	int i, j;
	char *tmp = data;

	if(type != 0)
	{
		printk("| ");
		for(i = j = 0; i < len; i++, j++)
		{
			if(j >= 16)
			{
				j = 0;
				printk(" |\n| ");
			}
			
			printk("0x%x ", (unsigned char)*tmp);
			tmp++;
		}
		printk(" |\n");
	}
}

#else
void _dbgdump(int type, char *data, int len)
{

}
#endif

/*-------------------------------------------------------------------------*/
void f_printroid_set_func(struct usb_function *function)
{
	f_printroid = function;

}

struct usb_function *f_printroid_get_func(void)
{
	return f_printroid;
	
}


/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/

static inline struct f_printroid_dev *f_printroid_to_dev(struct usb_function *f)
{
	return container_of(f, struct f_printroid_dev, function);
}



static inline int f_printroid_lock(atomic_t *excl)
{
	if (atomic_inc_return(excl) == 1) {
		return 0;
	} else {
		atomic_dec(excl);
		return -1;
	}
}

static inline void f_printroid_unlock(atomic_t *excl)
{
	atomic_dec(excl);
}

static struct usb_request *req_printroid_new(struct usb_ep *ep, int buffer_size)
{
	struct usb_request *req = usb_ep_alloc_request(ep, GFP_KERNEL);
	if (!req)
		return NULL;

	/* now allocate buffers for the requests */
	req->buf = kmalloc(buffer_size, GFP_KERNEL);
	if (!req->buf) {
		usb_ep_free_request(ep, req);
		return NULL;
	}

	return req;
}

static void req_printroid_free(struct usb_request *req, struct usb_ep *ep)
{
	if (req) {
		kfree(req->buf);
		usb_ep_free_request(ep, req);
	}
}

/* add a request to the tail of a list */
void req_printroid_putList(struct f_printroid_dev *dev, struct list_head *head,
		struct usb_request *req)
{
	unsigned long flags;

	spin_lock_irqsave(&dev->lock, flags);
	list_add_tail(&req->list, head);
	spin_unlock_irqrestore(&dev->lock, flags);
}

/* remove a request from the head of a list */
struct usb_request *req_printroid_getList(struct f_printroid_dev *dev, struct list_head *head)
{
	unsigned long flags;
	struct usb_request *req;

	spin_lock_irqsave(&dev->lock, flags);
	if (list_empty(head)) {
		req = 0;
	} else {
		req = list_first_entry(head, struct usb_request, list);
		list_del(&req->list);
	}
	spin_unlock_irqrestore(&dev->lock, flags);
	return req;
}

static void f_printroid_complete_interrupt(struct usb_ep *ep, struct usb_request *req)
{
	struct f_printroid_dev *dev = __f_printroid_dev;

	if (req->status != 0)
		dev->error = 1;

	req_printroid_putList(dev, &dev->tx_interrupt_idle, req);

	wake_up(&dev->write_interrupt_wq);

}

static void f_printroid_complete_in(struct usb_ep *ep, struct usb_request *req)
{
	struct f_printroid_dev *dev = __f_printroid_dev;

	if (req->status != 0)
		dev->error = 1;

	req_printroid_putList(dev, &dev->tx_idle, req);

	wake_up(&dev->write_wq);

}

static void f_printroid_complete_out(struct usb_ep *ep, struct usb_request *req)
{
	struct f_printroid_dev *dev = __f_printroid_dev;

	if (req->status != 0) {
		dev->error = 1;
		req_printroid_putList(dev, &dev->rx_idle, req);
	} else {
		req_printroid_putList(dev, &dev->rx_done, req);
	}

	wake_up(&dev->read_wq);

}

static int f_printroid_create_endpoints(struct f_printroid_dev *dev,
				struct usb_endpoint_descriptor *int_desc,
				struct usb_endpoint_descriptor *in_desc,
				struct usb_endpoint_descriptor *out_desc)
{
	struct usb_composite_dev *cdev = dev->cdev;
	struct usb_request *req;
	struct usb_ep *ep;
	int i;

	dbglog(DBG_INFO, "f_printroid_create_endpoints dev: %p\n", dev);

	ep = usb_ep_autoconfig(cdev->gadget, int_desc);
	if (!ep) {
		dbglog(DBG_INFO, "usb_ep_autoconfig for ep_interrupt failed\n");
		return -ENODEV;
	}
	dbglog(DBG_INFO, "usb_ep_autoconfig for ep_interrupt got %s\n", ep->name);
	ep->driver_data = dev;
	dev->ep_int = ep;

	ep = usb_ep_autoconfig(cdev->gadget, in_desc);
	if (!ep) {
		dbglog(DBG_INFO, "usb_ep_autoconfig for ep_in failed\n");
		return -ENODEV;
	}
	dbglog(DBG_INFO, "usb_ep_autoconfig for ep_in got %s\n", ep->name);
	ep->driver_data = dev;
	dev->ep_in = ep;

	ep = usb_ep_autoconfig(cdev->gadget, out_desc);
	if (!ep) {
		dbglog(DBG_INFO, "usb_ep_autoconfig for ep_out failed\n");
		return -ENODEV;
	}
	dbglog(DBG_INFO, "usb_ep_autoconfig for ep_out got %s\n", ep->name);
	ep->driver_data = dev;
	dev->ep_out = ep;

	/* now allocate requests for our endpoints */
	for (i = 0; i < RX_REQ_MAX_2; i++) {
		req = req_printroid_new(dev->ep_out, BULK_BUFFER_SIZE);
		if (!req)
			goto fail;
		req->complete = f_printroid_complete_out;
		req_printroid_putList(dev, &dev->rx_idle, req);
	}

	for (i = 0; i < TX_REQ_MAX; i++) {
		req = req_printroid_new(dev->ep_in, BULK_BUFFER_SIZE);
		if (!req)
			goto fail;
		req->complete = f_printroid_complete_in;
		req_printroid_putList(dev, &dev->tx_idle, req);
	}
	
	for (i = 0; i < TX_REQ_MAX; i++) {
		req = req_printroid_new(dev->ep_int, INTERRUPT_BUFFER_SIZE);
		if (!req)
			goto fail;
		req->complete = f_printroid_complete_interrupt;
		req_printroid_putList(dev, &dev->tx_interrupt_idle, req);
	}

	return 0;

fail:
	return -1;
}


//===========================================================//
//===========================================================//
static ssize_t f_printroid_read(struct file *fp, char __user *buf,
				size_t count, loff_t *pos)
{
	struct f_printroid_dev *dev = fp->private_data;
//.x	struct usb_composite_dev *cdev = dev->cdev;
	struct usb_request *req;
	int r = count, xfer = 0;
	int ret;
//.x	int getLength = 0;

	dbglog(DBG_INFO, "f_printroid_read(%d)\n", count);

	if (f_printroid_lock(&dev->read_excl))
		return -EBUSY;

	/* we will block until we're online */
	while (!(dev->online || dev->error)) 
	{
		dbglog(DBG_INFO, "f_printroid_read: waiting for online state\n");
		ret = wait_event_interruptible(dev->read_wq,
				(dev->online || dev->error));
		if (ret < 0)
		{
			f_printroid_unlock(&dev->read_excl);
			return ret;
		}
	}

	while (count > 0) 
	{
		if(dev->error) 
		{
			dbglog(DBG_INFO, "f_printroid_read dev->error\n");
			r = -EIO;
			break;
		}

		/* if we have idle read requests, get them queued */
		while ((req = req_printroid_getList(dev, &dev->rx_idle))) 
		{
requeue_req:
			req->length = BULK_BUFFER_SIZE;
			ret = usb_ep_queue(dev->ep_out, req, GFP_ATOMIC);

			if (ret < 0) {
				r = -EIO;
				dev->error = 1;
				req_printroid_putList(dev, &dev->rx_idle, req);
				goto fail;
			} else {
				dbglog(DBG_INFO, "rx %p queue\n", req);
				printk("rx %p queue\n", req);
			}
		}

		/* if we have data pending, give it to userspace */
		if (dev->read_count > 0) 
		{
			if (dev->read_count < count)
				xfer = dev->read_count;
			else
				xfer = count;

			if (copy_to_user(buf, dev->read_buf, xfer)) 
			{	
				r = -EFAULT;
				break;
			}
			dev->read_buf += xfer;
			dev->read_count -= xfer;
			buf += xfer;
			count -= xfer;

			/* if we've emptied the buffer, release the request */
			if (dev->read_count == 0) 
			{
				req_printroid_putList(dev, &dev->rx_idle, dev->read_req);
				dev->read_req = 0;
			}
			continue;
		}	
		else if(xfer == BULK_BUFFER_SIZE)
		{

		}
		else if(xfer)
		{
			r -= count;
			break;
		}
			

		/* wait for a request to complete */
		req = 0;

		ret = wait_event_interruptible(dev->read_wq,
			((req = req_printroid_getList(dev, &dev->rx_done)) || dev->error || dev->sendObj_Cancel));
		
		if (req != 0) {
			/* if we got a 0-len one we need to put it back into
			** service.  if we made it the current read req we'd
			** be stuck forever
			*/
			if (req->actual == 0)
				goto requeue_req;

			dev->read_req = req;
			dev->read_count = req->actual;
			dev->read_buf = req->buf;
			dbglog(DBG_INFO, "rx %p %d\n", req, req->actual);
		}

		if (ret < 0) {
			r = ret;
			break;
		}

		
	}

fail:
	f_printroid_unlock(&dev->read_excl);
	dbglog(DBG_INFO, "f_printroid_read returning %d\n", r);
	
		if(dev->sendObj_Cancel){
			r = -500;
		}
	
	return r;
}

static ssize_t f_printroid_write(struct file *fp, const char __user *buf,
				 size_t count, loff_t *pos)
{
	struct f_printroid_dev *dev = fp->private_data;
//.x	struct usb_composite_dev *cdev = dev->cdev;
	struct usb_request *req = 0;
	int r = count, xfer;
	int ret;

	dbglog(DBG_INFO, "f_printroid_write(%d)\n", count);

	if (f_printroid_lock(&dev->write_excl))
		return -EBUSY;

	if(count == 0)  //..ZeroLength Packet
	{
		req = 0;
		ret = wait_event_interruptible(dev->write_wq,
			((req = req_printroid_getList(dev, &dev->tx_idle)) || dev->error || dev->sendObj_Cancel));

		if (ret < 0)
		{
			r = ret;
		}
		else if (req != 0) 
		{
			req->length = 0;
			ret = usb_ep_queue(dev->ep_in, req, GFP_ATOMIC);
			if (ret < 0) {
				dbglog(DBG_ERR, "f_printroid_write: ZERO Length Packet Error %d\n", ret);
				dev->error = 1;
				r = -EIO;
			}
			else
			{
				/* zero this so we don't try to free it on error exit */
				req = 0;
			}
		}
	}

	while (count > 0) {
		if (dev->error) {
			dbglog(DBG_ERR, "f_printroid_write dev->error\n");
			r = -EIO;
			break;
		}

		/* get an idle tx request to use */
		req = 0;
		ret = wait_event_interruptible(dev->write_wq,
			((req = req_printroid_getList(dev, &dev->tx_idle)) || dev->error || dev->sendObj_Cancel));

		if (ret < 0) {
			r = ret;
			break;
		}
		
		if (req != 0) {
			if (count > BULK_BUFFER_SIZE)
				xfer = BULK_BUFFER_SIZE;
			else
				xfer = count;
			if (copy_from_user(req->buf, buf, xfer)) {
				r = -EFAULT;
				break;
			}

			req->length = xfer;
			ret = usb_ep_queue(dev->ep_in, req, GFP_ATOMIC);
			if (ret < 0) {
				dbglog(DBG_ERR, "f_printroid_write: xfer error %d\n", ret);
				dev->error = 1;
				r = -EIO;
				break;
			}

			buf += xfer;
			count -= xfer;

			/* zero this so we don't try to free it on error exit */
			req = 0;
		}
	}

	if (req)
		req_printroid_putList(dev, &dev->tx_idle, req);

	f_printroid_unlock(&dev->write_excl);
	dbglog(DBG_INFO, "f_printroid_write returning %d\n", r);

	if(dev->sendObj_Cancel){
			r = -500;
	}
		
	return r;
}

static int f_printroid_open(struct inode *ip, struct file *fp)
{
	dbglog(DBG_INFO, "f_printroid_open\n");
	
	if(__f_printroid_dev->state != UP_ACTIVE) 
		return -EAGAIN;
	
	if (f_printroid_lock(&__f_printroid_dev->open_excl))
		return -EBUSY;

	fp->private_data = __f_printroid_dev;
	
	/* clear the error latch */
	__f_printroid_dev->error = 0;

	return 0;
}

static int f_printroid_release(struct inode *ip, struct file *fp)
{
	dbglog(DBG_INFO, "f_printroid_release\n");
	f_printroid_unlock(&__f_printroid_dev->open_excl);

//_______________________________________Nvidia kill이 걸리는 경우 disable를 태운다...
	{
		struct usb_function *f;

		f = f_printroid_get_func();

		if (f && f->disable)
			f->disable(f);
	}
//_______________________________________

	return 0;
}

static int f_printroid_ioctl(struct inode *ip, struct file *fd, unsigned int code, unsigned long arg)
{
	struct f_printroid_dev	*dev = fd->private_data;
//.x	unsigned long		flags;
	int			status = 0;

	switch(code){
		case PRINTROID_SET_STATUS :
//..>>wondo.2010.12.28
			dev->online = 0;
			dev->error = 1;
			
			/* readers may be blocked waiting for us to go online */
			wake_up(&dev->read_wq);

			usb_ep_set_halt(dev->ep_out);
			msleep(10);
			usb_ep_clear_halt(dev->ep_out);

			dbglog(DBG_INFO, "%s PRINTROID_SET_STATUS\n", dev->function.name);
//..<<
			break;

		case PRINTROID_GET_STATUS :
			if(g_printRoid_Enable)
			{
				ADD_USB_PRINTROID_CMD(USB_PRINTROID_ENABLE);
			}
			else
			{
				DEL_USB_PRINTROID_CMD(USB_PRINTROID_ENABLE);
			}

			if(copy_to_user((void *) arg, &g_USB_PRINTROID_CMD, sizeof(g_USB_PRINTROID_CMD)))
				return -EFAULT;
			break;
			
		case PRINTROID_GET_SPEED :
			if(copy_to_user((void *) arg, &g_printRoid_usb, sizeof(g_printRoid_usb)))
				return -EFAULT;
			break;

#if 1				
		case PRINTROID_INTERRUPT :
			{
				struct usb_request *req = 0;
				int r, xfer, zeroPacket;
				int ret;
				
				INTERRUPT_DATA_TYPE int_data;

				if(copy_from_user(&int_data, (const INTERRUPT_DATA_TYPE *)arg, sizeof(INTERRUPT_DATA_TYPE)))
					return -EFAULT;

				r = int_data.size;
				
				zeroPacket = int_data.size % dev->ep_int->maxpacket;
				//..zeroPacket == 0이면 zeroPacket을 보내야 된다....^^"
				
				dbglog(DBG_INFO, "f_printroid_interrupt_write(%d = %d %% %d)\n", zeroPacket, int_data.size, dev->ep_int->maxpacket);

				if (f_printroid_lock(&dev->write_interrupt_excl))
					return -EBUSY;

				while(int_data.size > 0) {
					if (dev->error) {
						dbglog(DBG_ERR, "f_printroid_interrupt_write dev->error\n");
						r = -EIO;
						break;
					}

					/* get an idle tx request to use */
					req = 0;
					ret = wait_event_interruptible(dev->write_interrupt_wq,
						((req = req_printroid_getList(dev, &dev->tx_interrupt_idle)) || dev->error || dev->sendObj_Cancel));

					if (ret < 0) {
						r = ret;
						break;
					}
					
					if (req != 0) {
						xfer = int_data.size;
						memcpy(req->buf, int_data.data, xfer);

						req->length = xfer;
						ret = usb_ep_queue(dev->ep_int, req, GFP_ATOMIC);
						if (ret < 0) {
							dbglog(DBG_ERR, "f_printroid_interrupt_write: xfer error %d\n", ret);
							dev->error = 1;
							r = -EIO;
							break;
						}

						int_data.size -= xfer;

						/* zero this so we don't try to free it on error exit */
						req = 0;
					}
				}

				if(zeroPacket == 0)
				{
					req = 0;
					ret = wait_event_interruptible(dev->write_interrupt_wq,
						((req = req_printroid_getList(dev, &dev->tx_interrupt_idle)) || dev->error || dev->sendObj_Cancel));

					if (ret < 0)
					{
						r = ret;
					}
					else if (req != 0) 
					{
						req->length = 0;
						ret = usb_ep_queue(dev->ep_int, req, GFP_ATOMIC);
						if (ret < 0) {
							dbglog(DBG_ERR, "f_printroid_interrupt_write: ZERO Length Packet Error %d\n", ret);
							dev->error = 1;
							r = -EIO;
						}
						else
						{
							/* zero this so we don't try to free it on error exit */
							req = 0;
						}
					}
				}

				if (req)
					req_printroid_putList(dev, &dev->tx_interrupt_idle, req);

				f_printroid_unlock(&dev->write_interrupt_excl);
				dbglog(DBG_INFO, "f_printroid_interrupt_write returning %d\n", r);

				int_data.result = r;
				if(copy_to_user((void *) arg, &int_data, sizeof(INTERRUPT_DATA_TYPE)))
					return -EFAULT;
			}
			break;
#endif				
		default:
			/* could not handle ioctl */
			dbglog(DBG_ERR, "f_printroid_ioctl: ERROR cmd=0x%4.4xis not supported\n", code);
			status = -ENOTTY;
	}

//..??	spin_unlock_irqrestore(&dev->lock, flags);

	return status;
}

/* file operations for ADB device /dev/android_adb */
static struct file_operations f_printroid_fops = {
	.owner = THIS_MODULE,
	.read = f_printroid_read,
	.write = f_printroid_write,
	.open = f_printroid_open,
	.release = f_printroid_release,
	.ioctl = f_printroid_ioctl,

};

static struct miscdevice f_printroid_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = shortname,
	.fops = &f_printroid_fops,
};

//===========================================================//
//===========================================================//

static int f_printroid_set_interface(struct usb_function *f)
{
	struct f_printroid_dev	*dev = f_printroid_to_dev(f);
	struct usb_composite_dev *cdev = f->config->cdev;
//.x	struct usb_ep *ep;
	int			result = 0;


	dev->interrupt = ep_desc(cdev->gadget, &hs_printroid_interrupt_desc, &fs_printroid_interrupt_desc);
	dev->ep_int->driver_data = dev;

	dev->in = ep_desc(cdev->gadget, &hs_printroid_in_desc, &fs_printroid_in_desc);
	dev->ep_in->driver_data = dev;

	dev->out = ep_desc(cdev->gadget, &hs_printroid_out_desc, &fs_printroid_out_desc);
	dev->ep_out->driver_data = dev;

	result = usb_ep_enable(dev->ep_int, dev->interrupt);
	if (result != 0) {
		dbglog(DBG_ERR, "enable %s --> %d\n", dev->ep_int->name, result);
		goto done;
	}

	result = usb_ep_enable(dev->ep_in, dev->in);
	if (result != 0) {
		dbglog(DBG_ERR, "enable %s --> %d\n", dev->ep_in->name, result);
		goto done;
	}

	result = usb_ep_enable(dev->ep_out, dev->out);
	if (result != 0) {
		dbglog(DBG_ERR, "enable %s --> %d\n", dev->ep_out->name, result);
		goto done;
	}

done:
	/* on error, disable any endpoints  */
	if (result != 0) {
		(void) usb_ep_disable(dev->ep_int);
		(void) usb_ep_disable(dev->ep_in);
		(void) usb_ep_disable(dev->ep_out);
		dev->interrupt = NULL;
		dev->in = NULL;
		dev->out = NULL;
	}

	/* caller is responsible for cleanup on error */
	return result;
}

static void f_printroid_reset_interface(struct f_printroid_dev *dev)
{
	dbglog(DBG_INFO, "%s\n", __func__);

	if (dev->interrupt)
		usb_ep_disable(dev->ep_int);
	
	if (dev->in)
		usb_ep_disable(dev->ep_in);

	if (dev->out)
		usb_ep_disable(dev->ep_out);

}

static int f_printroid_set_config(struct usb_function *f, unsigned number)
{
	struct f_printroid_dev	*dev = f_printroid_to_dev(f);
	struct usb_composite_dev *cdev = dev->cdev;
	int			result = 0;
	struct usb_gadget	*gadget = cdev->gadget;


	if(dev->config)
	{
		f_printroid_reset_interface(dev);	
	}

	result = f_printroid_set_interface(f);
	
	switch (number) {
		case DEV_CONFIG_VALUE:
			result = 0;
			break;
		default:
			result = -EINVAL;
			/* FALL THROUGH */
		case 0:
			break;
	}

	if(result)
	{
		usb_gadget_vbus_draw(gadget, gadget->is_otg ? 8 : 100);
	}
	else
	{
		char *speed;
		unsigned power;

		power = 2 * printroid_configuration_desc.bMaxPower;
		usb_gadget_vbus_draw(gadget, power);

		switch (gadget->speed) {
			case USB_SPEED_FULL:	
				speed = "full"; 
				g_printRoid_usb = PRINTROI_USB_1_1;
				break;
#ifdef CONFIG_USB_GADGET_DUALSPEED
			case USB_SPEED_HIGH:	
				speed = "high"; 
				g_printRoid_usb = PRINTROI_USB_2_0;
				break;
#endif
			default:		
				speed = "?"; 
				g_printRoid_usb = PRINTROI_USB_1_1;
				break;
		}

		dev->config = number;
		dbglog(DBG_INFO, "%s speed config #%d: %d mA, %s\n",
				speed, number, power, driver_desc);
	}
	
	return result;
}

static int f_printroid_config_buf(enum usb_device_speed speed, u8 *buf, u8 type, unsigned index,
		int is_otg)
{
	int					len;
	//.x const struct usb_descriptor_header	**function;
	struct usb_descriptor_header	**function;
	
#ifdef CONFIG_USB_GADGET_DUALSPEED
	int hs = (speed == USB_SPEED_HIGH);

	if (type == USB_DT_OTHER_SPEED_CONFIG)
		hs = !hs;

	if (hs) {
		function = hs_printroid_descs;
	} else {
		function = fs_printroid_descs;
	}
#else
	function = fs_printroid_descs;
#endif

	if (index >= device_desc.bNumConfigurations)
		return -EINVAL;

	/* for now, don't advertise srp-only devices */
/*..wondo this device not otg	
	if (!is_otg)
		function++;
*/
	len = usb_gadget_config_buf(&printroid_configuration_desc, buf, USB_DESC_BUFSIZE,
			(const struct usb_descriptor_header **)function);
	
	if (len < 0)
		return len;
	
	((struct usb_config_descriptor *) buf)->bDescriptorType = type;
		return len;
}


//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//

static int f_printroid_bind(struct usb_configuration *c, struct usb_function *f)
{
	struct usb_composite_dev *cdev = c->cdev;  //..wondo.2010/09/11
	struct f_printroid_dev	*dev = f_printroid_to_dev(f);
//.x	int			id;
	int			ret;
	int			status;

	dev->cdev = cdev;
	dbglog(DBG_INFO, "f_printroid_bind dev: %x, usb_f : %x \n", (int)dev, (int)f);

	/* allocate instance-specific interface IDs, and patch descriptors */
	status = usb_interface_id(c, f);
	if (status < 0)
		return -ENODEV;

	/* allocate endpoints */
	ret = f_printroid_create_endpoints(dev, 
							&fs_printroid_interrupt_desc,
							&fs_printroid_in_desc,
							&fs_printroid_out_desc);

	if (ret)
		return ret;

	/* support high speed hardware */
	if (gadget_is_dualspeed(c->cdev->gadget)) 
	{
		hs_printroid_interrupt_desc.bEndpointAddress = fs_printroid_interrupt_desc.bEndpointAddress;
		hs_printroid_in_desc.bEndpointAddress = fs_printroid_in_desc.bEndpointAddress;
		hs_printroid_out_desc.bEndpointAddress = fs_printroid_out_desc.bEndpointAddress;
	}

	dbglog(DBG_INFO, "%s speed %s: INT/%s, IN/%s, OUT/%s\n",
			gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full", f->name, 
			dev->ep_int->name, dev->ep_in->name, dev->ep_out->name);

	return 0;
}

static void f_printroid_unbind(struct usb_configuration *c, struct usb_function *f)
{
	struct f_printroid_dev	*dev = f_printroid_to_dev(f);
	struct usb_request *req;

	dbglog(DBG_INFO, "f_printroid_unbind dev: %x, usb_f : %x \n", (int)dev, (int)f);
	spin_lock_irq(&dev->lock);

	while ((req = req_printroid_getList(dev, &dev->rx_idle)))
		req_printroid_free(req, dev->ep_out);
	while ((req = req_printroid_getList(dev, &dev->tx_idle)))
		req_printroid_free(req, dev->ep_in);
	while ((req = req_printroid_getList(dev, &dev->tx_interrupt_idle)))
		req_printroid_free(req, dev->ep_int);

	dev->online = 0;
	dev->error = 1;
	spin_unlock_irq(&dev->lock);

	misc_deregister(&f_printroid_device);
	kfree(__f_printroid_dev);
	__f_printroid_dev = NULL;
}

static int f_printroid_set_alt(struct usb_function *f, unsigned intf, unsigned alt)
{
	struct f_printroid_dev	*dev = f_printroid_to_dev(f);
	struct usb_composite_dev *cdev = f->config->cdev;
	struct usb_ep *ep;
	int ret = -EOPNOTSUPP;


	dbglog(DBG_INFO, "f_printroid_set_alt intf: %d alt: %d\n", intf, alt);

#if 1  //..MTP_UNUSED_CODE
	ret = usb_ep_enable(dev->ep_int,
			ep_choose(cdev->gadget,
				&hs_printroid_interrupt_desc,
				&fs_printroid_interrupt_desc));
	if (ret)
	{
		ep = dev->ep_int;
		dbglog(DBG_ERR, "can't enable %s, result %d\n", ep->name, ret);
		return ret;
	}
	
	ret = usb_ep_enable(dev->ep_in,
			ep_choose(cdev->gadget,
				&hs_printroid_in_desc,
				&fs_printroid_in_desc));
	if (ret)
	{
		ep = dev->ep_in;
		dbglog(DBG_ERR, "can't enable %s, result %d\n", ep->name, ret);
		usb_ep_disable(dev->ep_int);
		return ret;
	}
	
	ret = usb_ep_enable(dev->ep_out,
			ep_choose(cdev->gadget,
				&hs_printroid_out_desc,
				&fs_printroid_out_desc));
	if (ret) {
		ep = dev->ep_out;
		dbglog(DBG_ERR, "can't enable %s, result %d\n", ep->name, ret);
		
		usb_ep_disable(dev->ep_int);
		usb_ep_disable(dev->ep_in);
		return ret;
	}
	dev->state = UP_ACTIVE;
	dev->online = 1;
	dev->error = 0;
	dev->suspend = 0;

	g_printRoid_Enable = 1;

	/* readers may be blocked waiting for us to go online */
	wake_up(&dev->read_wq);
#else
/* 이놈이 호출 되었다는 것 은 "bConfigurationValue == number"의 조건이 ok~~~ */
	ret = f_printroid_set_config(f, DEV_CONFIG_VALUE);
	if(ret >= 0)
	{
		dev->state = UP_ACTIVE;
		dev->online = 1;
		dev->error = 0;
		dev->suspend = 0;

		g_printRoid_Enable = 1;
	}
	
#endif
	
	return ret;
}

static int f_printroid_setup(struct usb_function *f, const struct usb_ctrlrequest *ctrl)
{

	struct f_printroid_dev	*dev = f_printroid_to_dev(f);
	struct usb_composite_dev *cdev = dev->cdev;
	struct usb_request	*req = cdev->req;

	int			value = -EOPNOTSUPP;
#ifdef __DEBUG
	u16			wIndex = le16_to_cpu(ctrl->wIndex);
#endif
	u16			wValue = le16_to_cpu(ctrl->wValue);
	u16			wLength = le16_to_cpu(ctrl->wLength);

	dbglog(DBG_SETUP, "ctrl req%02x.%02x v%04x i%04x l%d ",
		ctrl->bRequestType, ctrl->bRequest, wValue, wIndex, wLength);

	switch (ctrl->bRequestType&USB_TYPE_MASK){

	case USB_TYPE_STANDARD:
		dbglog(DBG_SETUP, "USB_TYPE_STANDARD----->\n");
		switch (ctrl->bRequest) {

		case USB_REQ_GET_DESCRIPTOR:
			if (ctrl->bRequestType != USB_DIR_IN)
				break;
			switch (wValue >> 8) {

			case USB_DT_DEVICE:
				value = min(wLength, (u16) sizeof device_desc);
				memcpy(req->buf, &device_desc, value);
				_dbgdump(DBG_SETUP, req->buf, value);
				break;
#ifdef CONFIG_USB_GADGET_DUALSPEED
			case USB_DT_DEVICE_QUALIFIER:
				if (!cdev->gadget->is_dualspeed)
					break;
				value = min(wLength,
						(u16) sizeof dev_qualifier);
				memcpy(req->buf, &dev_qualifier, value);
				break;

			case USB_DT_OTHER_SPEED_CONFIG:
				if (!cdev->gadget->is_dualspeed)
					break;
				/* FALLTHROUGH */
#endif /* CONFIG_USB_GADGET_DUALSPEED */
			case USB_DT_CONFIG:
				value = f_printroid_config_buf(cdev->gadget->speed, req->buf,
						wValue >> 8,
						wValue & 0xff,
						cdev->gadget->is_otg);
				if (value >= 0)
					value = min(wLength, (u16) value);

				_dbgdump(DBG_SETUP, req->buf, value);
				break;

			case USB_DT_STRING:
				{
					int id;

					id = wValue & 0xff;
					dbglog(DBG_SETUP,"String index = 0x%x", id);

					value = usb_gadget_get_string(&stringtab, id, req->buf);
					if (value >= 0)
						value = min(wLength, (u16) value);
					
					_dbgdump(DBG_SETUP, req->buf, value);
				}
				break;
			}
			break;

		case USB_REQ_SET_CONFIGURATION:
			if (ctrl->bRequestType != 0)
			{
				break;
			}
			
			if (cdev->gadget->a_hnp_support)
			{
				dbglog(DBG_SETUP, "HNP available\n");
			}
			else if (cdev->gadget->a_alt_hnp_support)
			{
				dbglog(DBG_SETUP, "HNP needs a different root port\n");
			}
			
			value = f_printroid_set_config(f, wValue);
			if(value >= 0)
			{
				dev->state = UP_ACTIVE;
				dev->online = 1;
				dev->error = 0;
				dev->suspend = 0;

				g_printRoid_Enable = 1;
			}
			break;
		case USB_REQ_GET_CONFIGURATION:
			if (ctrl->bRequestType != USB_DIR_IN)
				break;
			*(u8 *)req->buf = dev->config;
//..>>wondo.2010.12.28			
			if(dev->config > 0)
			{
				f_printroid_set_config(f, dev->config);

				dev->state = UP_ACTIVE;
				dev->online = 1;
				dev->error = 0;
				dev->suspend = 0;

				g_printRoid_Enable = 1;
			}
//..<<			
			value = min(wLength, (u16) 1);
			break;

		case USB_REQ_SET_INTERFACE:
			if (ctrl->bRequestType != (USB_DIR_OUT| USB_TYPE_STANDARD | USB_RECIP_INTERFACE))
				break;
			
			dev->interface = wValue;
			
			*(u8 *)req->buf = 0;
			value = min(wLength, (u16) 1);
			break;

		case USB_REQ_GET_INTERFACE:
			if (ctrl->bRequestType !=
					(USB_DIR_IN|USB_RECIP_INTERFACE)
					|| !dev->config)
				break;

			*(u8 *)req->buf = dev->interface;
			value = min(wLength, (u16) 1);
			break;

		default:
			goto unknown;
		}
		break;

	case USB_TYPE_CLASS:
		dbglog(DBG_SETUP, "USB_TYPE_CLASS[ctrl->bRequest = 0x%x]----->\n", ctrl->bRequest);
		break;

	case USB_TYPE_VENDOR:
		dbglog(DBG_SETUP, "USB_TYPE_VENDOR----->\n");
		break;

	case USB_TYPE_RESERVED:
		dbglog(DBG_SETUP, "USB_TYPE_RESERVED----->\n");
		break;
		
	default:
unknown:
		dbglog(DBG_SETUP, "unknown ctrl req%02x.%02x v%04x i%04x l%d\n",
			ctrl->bRequestType, ctrl->bRequest, wValue, wIndex, wLength);
		break;
	}

	return value;
}

static void f_printroid_disable(struct usb_function *f)
{
	struct f_printroid_dev	*dev = f_printroid_to_dev(f);
//.x	struct usb_composite_dev	*cdev = dev->cdev;

	dbglog(DBG_INFO, "f_printroid_disable\n");
	dev->online = 0;
	dev->error = 1;
	dev->config = 0;
//..>>wondo.2010.12.28
	dev->interface = 0;
	dev->state = UP_DEACTIVE;
	g_printRoid_usb = 0;
	g_printRoid_Enable = 0;
//..<<
	usb_ep_disable(dev->ep_int);
	usb_ep_disable(dev->ep_in);
	usb_ep_disable(dev->ep_out);

	/* readers may be blocked waiting for us to go online */
	wake_up(&dev->read_wq);

	dbglog(DBG_INFO, "%s disabled\n", dev->function.name);
}


static void f_printroid_suspend(struct usb_function *f)
{
 
	struct f_printroid_dev	*dev = f_printroid_to_dev(f);
//.x	struct usb_request *req;

	dbglog(DBG_INFO, "f_printroid_suspend\n");
	dev->online = 0;
	dev->error = 1;
//..	dev->config = 0;
	dev->interface = 0;  //..wondo.2010.12.28
	dev->suspend = 1;
	dev->state = UP_DEACTIVE;

//..Qualcomm에서 사용 	f_printroid_set_func(NULL);
	g_printRoid_usb = 0;
	g_printRoid_Enable = 0;

	/* readers may be blocked waiting for us to go online */
	wake_up(&dev->read_wq);


	dbglog(DBG_INFO, "%s suspended\n", dev->function.name);
}

/*.x
static void f_printroid_resum(struct usb_function *f)
{
	dbglog(DBG_INFO, "f_printroid_suspend\n");


}
*/

//===========================================================//
//===========================================================//
int f_printroid_function_add(struct usb_composite_dev *cdev, struct usb_configuration *c)
{
  printk(KERN_INFO "f_printroid_function_add\n");
	
	if(__f_printroid_dev)
	{
//..		__f_printroid_dev->cdev = cdev;
		
//..		comp_set_config(COMP_MTP);  // ++
		f_printroid_set_func(&__f_printroid_dev->function);  // ++

		return usb_add_function(c, &__f_printroid_dev->function);
	}
	else
		return 0;
}

void f_printroid_function_enable(int enable)
{
	struct f_printroid_dev *dev = __f_printroid_dev;

	if (dev) {
		dbglog(DBG_INFO, "f_printroid_enable(%s)\n",
			enable ? "true" : "false");

		if (enable) {
			dev->function.descriptors = fs_printroid_descs;
			dev->function.hs_descriptors = hs_printroid_descs;
		} else {
			dev->function.descriptors = null_printroid_descs;
			dev->function.hs_descriptors = null_printroid_descs;
		}
	}
}

//===========================================================//
//===========================================================//
static int printroid_enable_open(struct inode *ip, struct file *fp)
{
	if (atomic_inc_return(&printroid_enable_excl) != 1) {
		atomic_dec(&printroid_enable_excl);
		return -EBUSY;
	}

	printk("enabling printroid\n");
	
	comp_set_config(COMP_PRINTROID);
	android_enable_function(&__f_printroid_dev->function, 1);

	return 0;
}

static int printroid_enable_release(struct inode *ip, struct file *fp)
{
	printk("disabling printroid\n");

	comp_del_config(COMP_PRINTROID);
	android_enable_function(&__f_printroid_dev->function, 0);
	atomic_dec(&printroid_enable_excl);
	return 0;
}

static const struct file_operations printroid_enable_fops = {
	.owner =   THIS_MODULE,
	.open =    printroid_enable_open,
	.release = printroid_enable_release,
};

static struct miscdevice printroid_enable_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "printroid_enable",
	.fops = &printroid_enable_fops,
};
//===========================================================//
//===========================================================//

//int f_printroid_function_init(void)
static int printroid_bind_config(struct usb_configuration *c)
{
	struct f_printroid_dev *dev;
	int ret;
	
	dbglog(DBG_INFO, "printroid_bind_config\n");

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	spin_lock_init(&dev->lock);

	init_waitqueue_head(&dev->read_wq);
	init_waitqueue_head(&dev->write_wq);
	init_waitqueue_head(&dev->write_interrupt_wq);

	atomic_set(&dev->open_excl, 0);
	atomic_set(&dev->read_excl, 0);
	atomic_set(&dev->write_excl, 0);
	atomic_set(&dev->write_interrupt_excl, 0);

	INIT_LIST_HEAD(&dev->rx_idle);
	INIT_LIST_HEAD(&dev->rx_done);
	INIT_LIST_HEAD(&dev->tx_idle);
	INIT_LIST_HEAD(&dev->tx_interrupt_idle);

	dev->cdev = c->cdev;
	dev->function.name = "printroid";
	dev->suspend = 0;
	dev->state = UP_NONE;
	dev->function.descriptors = fs_printroid_descs;
	dev->function.hs_descriptors = hs_printroid_descs;

	dev->function.bind = f_printroid_bind;
	dev->function.unbind = f_printroid_unbind;
	dev->function.set_alt = f_printroid_set_alt;
	dev->function.disable = f_printroid_disable;
	dev->function.setup = f_printroid_setup;
	dev->function.suspend = f_printroid_suspend;

//_____________________________________________________
	dev->sendObj_Cancel = 0;

//_____________________________________________________

	/* __f_printroid_dev must be set before calling usb_gadget_register_driver */
	__f_printroid_dev = dev;

	ret = misc_register(&f_printroid_device);
	if (ret)
		goto err1;
	ret = misc_register(&printroid_enable_device);
	if (ret)
		goto err2;
	ret = usb_add_function(c, &dev->function);
	if (ret)
		goto err3;

	f_printroid_set_func(&__f_printroid_dev->function);  // ++
	
	return 0;

err3:
	misc_deregister(&printroid_enable_device);
err2:
	misc_deregister(&f_printroid_device);
err1:
	kfree(dev);
	printk("f_printroid gadget driver failed to initialize\n");
	return ret;

}

static struct android_usb_function printroid_function = {
	.name = "printroid",
	.bind_config = printroid_bind_config,
};

static int __init init(void)
{
	printk(KERN_INFO "f_printRoid init\n");
	android_register_function(&printroid_function);
	return 0;
}
module_init(init);

