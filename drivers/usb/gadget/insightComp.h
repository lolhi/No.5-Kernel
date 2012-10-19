/* customer definition for MTP (Media Transfer Protocol)
 *
 * $Id: custMTP.c, v1.00, 2010.04.08:15:28:582$
 *
 * Initial work by:
 *   (c) 2007 Insight-kr Lee Hyun Wook (wondo@insight-kr.com)
 *
                        EDIT HISTORY FOR MODULE
 
when            who          what, where, why
--------      ------     ----------------------------------------------------------
2007/11/27   wondo      Initial work............^^"



-----------------------------------------------------------------------------*/
#ifndef __INSIGHT_ADD_COMPOSITE_H__
#define __INSIGHT_ADD_COMPOSITE_H__   

/*------------------------------------------------------------*/
#define IMTP_ADD				0
#define PRINTROID_ADD 			1

#undef MTP_FIRST_ENUM
#define MTP_UNUSED_CODE	0


#define COMP_UMS 0x0001
#define COMP_ADB 0x0002

#define COMP_IMTP 		0x1000
#define COMP_PRINTROID	0x2000


/*------------------------------------------------------------*/
#define COMP_IMTP_RESET			_IOW('a', 0x11, int)


/*------------------------------------------------------------*/
#if (IMTP_ADD || PRINTROID_ADD)

void comp_set_config(u16 type);
void comp_del_config(u16 type);
u16 comp_get_config(void);

/*-------------------------------------------------------------------------
 * Composite Suspend check
 *------------------------------------------------------------------------*/
void comp_setSuspend(int set);
int comp_getSuspend(void);

#if IMTP_ADD
/*-------------------------------------------------------------------------
 * MTP usb function
 *------------------------------------------------------------------------*/
void mtp_set_func(struct usb_function *function);
struct usb_function *mtp_get_func(void);
#endif

#if PRINTROID_ADD
/*-------------------------------------------------------------------------
 * printRoid usb function
 *------------------------------------------------------------------------*/
void f_printroid_set_func(struct usb_function *function);
struct usb_function *f_printroid_get_func(void);
#endif

/*-------------------------------------------------------------------------
 * ???
 *------------------------------------------------------------------------*/
#endif

#endif  /* __INSIGHT_ADD_COMPOSITE_H__ */
