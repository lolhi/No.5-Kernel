/* Copyright (c) 2010, PANTECH. All rights reserved.
 */
 
#include "mt9v113.h"

/* register configration */
//static const struct mt9v113_i2c_reg_conf mt9v113_init_parm[] =
static const struct mt9v113_i2c_reg_conf mt9v113_init_parm[] =
{
#if 1
    {0x0018, 0x4028, WORD_LEN, 0},
    {0x0018, 0x4000, POLL_REG, 0},                  
    {0x0000, 0x6464, POLL_REG, 0},
    {0x301a, 0x0004, POLL_REG, 0},
    {0x0004, 0x3214, POLL_REG, 0},
    {0x001a, 0x0013, WORD_LEN, 10},
    {0x001a, 0x0010, WORD_LEN, 10},
    {0x0018, 0x4028, WORD_LEN, 0},
    {0x0018, 0x4000, POLL_REG, 0},
    {0x0000, 0x6464, POLL_REG, 0},
    {0x301a, 0x0004, POLL_REG, 0},
    {0x0004, 0x3214, POLL_REG, 0},
    {0x001A, 0x0000, WORD_LEN, 0},
    {0x3400, 0x0200, WORD_LEN, 0},
    {0x321C, 0x0000, WORD_LEN, 0},
    {0x001E, 0x0777, WORD_LEN, 0},
    {0x0016, 0x42DF, WORD_LEN, 0},
    {0x098c, 0x02f0, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x02f2, WORD_LEN, 0},
    {0x0990, 0x0210, WORD_LEN, 0},
    {0x098c, 0x02f4, WORD_LEN, 0},
    {0x0990, 0x001a, WORD_LEN, 0},
    {0x098c, 0x2145, WORD_LEN, 0},
    {0x0990, 0x02f4, WORD_LEN, 0},
    {0x098c, 0xa134, WORD_LEN, 0},
    {0x0990, 0x0001, WORD_LEN, 0},
    {0x31e0, 0x0001, WORD_LEN, 0},
    {0x001a, 0x0210, WORD_LEN, 0},
    {0x0016, 0x42df, WORD_LEN, 0},
    {0x001E, 0x0404, WORD_LEN, 0},
    {0x0014, 0x2145, WORD_LEN, 0},
    {0x0014, 0x2145, WORD_LEN, 0},
    {0x0010, 0x0110, WORD_LEN, 0},
    {0x0012, 0x0000, WORD_LEN, 0},
    {0x0014, 0x244B, WORD_LEN, 1},
    //{0x0014, 0x304b, WORD_LEN, 0},
    {0x0014, 0x304b, WORD_LEN, 0},
    //{0x0014, 0xb04a, WORD_LEN, 0},
//poll_reg=0x0014, 0x8000, ==0, delay=50, timeout=20   // delay=70
    //{0x0014, 0x8000, POLL_REG, 0},
    //{0x8000, 0x3214, POLL_REG, 0},
    {0x0014, 0xb04a, WORD_LEN, 0}, // PLL_CONTROL
    {0x098C, 0x2703, WORD_LEN, 0},	//Output Width (A)
    {0x0990, 0x0280, WORD_LEN, 0},	//      = 640
    {0x098C, 0x2705, WORD_LEN, 0},	//Output Height (A)
    {0x0990, 0x01E0, WORD_LEN, 0},	//      = 480
    {0x098C, 0x2707, WORD_LEN, 0},	//Output Width (B)
    {0x0990, 0x0280, WORD_LEN, 0},	//      = 640
    {0x098C, 0x2709, WORD_LEN, 0},	//Output Height (B)
    {0x0990, 0x01E0, WORD_LEN, 0}, 	// //      = 480
    {0x098C, 0x270D, WORD_LEN, 0},	//Row Start (A)
    {0x0990, 0x0000, WORD_LEN, 0},	//      = 0
    {0x098C, 0x270F, WORD_LEN, 0},	//Column Start (A)
    {0x0990, 0x0000, WORD_LEN, 0},	//      = 0
    {0x098C, 0x2711, WORD_LEN, 0},	//Row End (A)
    {0x0990, 0x01e7, WORD_LEN, 0},	//	//      = 487
    {0x098C, 0x2713, WORD_LEN, 0},	//Column End (A)
    {0x0990, 0x0287, WORD_LEN, 0},	//      = 647
    {0x098C, 0x2715, WORD_LEN, 0},	//Row Speed (A)
    {0x0990, 0x0001, WORD_LEN, 0},	//      = 1
    {0x098C, 0x2717, WORD_LEN, 0},	//Read Mode (A)
    {0x0990, 0x0027, WORD_LEN, 0},	//      = 38
    {0x098C, 0x2719, WORD_LEN, 0},	//sensor_fine_correction (A)
    {0x0990, 0x001A, WORD_LEN, 0},	//      = 26
    {0x098C, 0x271B, WORD_LEN, 0},	//sensor_fine_IT_min (A)
    {0x0990, 0x006B, WORD_LEN, 0},	//      = 107
    {0x098C, 0x271D, WORD_LEN, 0},	//sensor_fine_IT_max_margin (A)
    {0x0990, 0x006B, WORD_LEN, 0},	//      = 107
    {0x098C, 0x271F, WORD_LEN, 0},	//Frame Lines (A)
    {0x0990, 0x02A9, WORD_LEN, 0},	//2A5	//20b	//7	//      = 519
    {0x098C, 0x2721, WORD_LEN, 0},	//Line Length (A)
    {0x0990, 0x047C, WORD_LEN, 0},	//5DA	//      = 1498
    {0x098C, 0x2723, WORD_LEN, 0},	//Row Start (B)
    {0x0990, 0x0000, WORD_LEN, 0},	//      = 0
    {0x098C, 0x2725, WORD_LEN, 0},	//Column Start (B)
    {0x0990, 0x0000, WORD_LEN, 0},	//      = 0
    {0x098C, 0x2727, WORD_LEN, 0},	//Row End (B)
    {0x0990, 0x01e7, WORD_LEN, 0},	//1E7	//      = 487
    {0x098C, 0x2729, WORD_LEN, 0},	//Column End (B)
    {0x0990, 0x0287, WORD_LEN, 0},	//      = 647
    {0x098C, 0x272B, WORD_LEN, 0},	//Row Speed (B)
    {0x0990, 0x0001, WORD_LEN, 0},	//      = 1
    {0x098C, 0x272D, WORD_LEN, 0},	//Read Mode (B)
    {0x0990, 0x0027, WORD_LEN, 0},	//      = 38
    {0x098C, 0x272F, WORD_LEN, 0},	//sensor_fine_correction (B)
    {0x0990, 0x001A, WORD_LEN, 0},	//      = 26
    {0x098C, 0x2731, WORD_LEN, 0},	//sensor_fine_IT_min (B)
    {0x0990, 0x006B, WORD_LEN, 0},	//      = 107
    {0x098C, 0x2733, WORD_LEN, 0},	//sensor_fine_IT_max_margin (B)
    {0x0990, 0x006B, WORD_LEN, 0},	//      = 107
    {0x098C, 0x2735, WORD_LEN, 0},	//Frame Lines (B)
    {0x0990, 0x02A9, WORD_LEN, 0},	//2A5	//207	//      = 519
    {0x098C, 0x2737, WORD_LEN, 0},	//Line Length (B)
    {0x0990, 0x047C, WORD_LEN, 0},	//5DA	//      = 1498
    {0x098C, 0x2739, WORD_LEN, 0},	//Crop_X0 (A)
    {0x0990, 0x0000, WORD_LEN, 0},	//      = 0
    {0x098C, 0x273B, WORD_LEN, 0},	//Crop_X1 (A)
    {0x0990, 0x027F, WORD_LEN, 0},	//      = 639
    {0x098C, 0x273D, WORD_LEN, 0},	//Crop_Y0 (A)
    {0x0990, 0x0000, WORD_LEN, 0},	//      = 0
    {0x098C, 0x273F, WORD_LEN, 0},	//Crop_Y1 (A)
    {0x0990, 0x01DF, WORD_LEN, 0},	//1DF	//      = 479
    {0x098C, 0x2747, WORD_LEN, 0},	//Crop_X0 (B)
    {0x0990, 0x0000, WORD_LEN, 0},	//      = 0
    {0x098C, 0x2749, WORD_LEN, 0},	//Crop_X1 (B)
    {0x0990, 0x027F, WORD_LEN, 0},	//      = 639
    {0x098C, 0x274B, WORD_LEN, 0},	//Crop_Y0 (B)
    {0x0990, 0x0000, WORD_LEN, 0},	//      = 0
    {0x098C, 0x274D, WORD_LEN, 0},	//Crop_Y1 (B)
    {0x0990, 0x01DF, WORD_LEN, 0},	//1DF	//      = 479
    {0x098C, 0x222D, WORD_LEN, 0},	//R9 Step
    {0x0990, 0x0059, WORD_LEN, 0},	//      = 89
    {0x098C, 0xA408, WORD_LEN, 0},	//search_f1_50
    {0x0990, 0x1C  , WORD_LEN, 0},//      = 28
    {0x098C, 0xA409, WORD_LEN, 0},	//search_f2_50
    {0x0990, 0x1E  , WORD_LEN, 0},	//      = 30
    {0x098C, 0xA40A, WORD_LEN, 0},	//search_f1_60
    {0x0990, 0x22  , WORD_LEN, 0},//      = 34
    {0x098C, 0xA40B, WORD_LEN, 0},	//search_f2_60
    {0x0990, 0x24  , WORD_LEN, 0},//      = 36
    {0x098C, 0x2411, WORD_LEN, 0},	//R9_Step_60 (A)
    {0x0990, 0x0059, WORD_LEN, 0},	//      = 89
    {0x098C, 0x2413, WORD_LEN, 0},	//R9_Step_50 (A)
    {0x0990, 0x006B, WORD_LEN, 0},	//      = 107
    {0x098C, 0x2415, WORD_LEN, 0},	//R9_Step_60 (B)
    {0x0990, 0x0059, WORD_LEN, 0},	//      = 89
    {0x098C, 0x2417, WORD_LEN, 0},	//R9_Step_50 (B)
    {0x0990, 0x006B, WORD_LEN, 0},	//      = 107
    {0x098C, 0xA404, WORD_LEN, 0},	//FD Mode
    {0x0990, 0x0010, WORD_LEN, 0},	//      = 16
    {0x098C, 0xA40D, WORD_LEN, 0},	//Stat_min
    {0x0990, 0x0002, WORD_LEN, 0},	//      = 2
    {0x098C, 0xA40E, WORD_LEN, 0},	//Stat_max
    {0x0990, 0x0003, WORD_LEN, 0},	//      = 3
    {0x098C, 0xA410, WORD_LEN, 0},	//Min_amplitude
    {0x0990, 0x000A, WORD_LEN, 0},	//      = 10
    {0x098C, 0xA124, WORD_LEN, 0},	// MCU_ADDRESS /*SEQ_PREVIEW_2_FD*/
    {0x0990, 0x0002, WORD_LEN, 0},	// MCU_DATA_0
    {0x098C, 0xA12A, WORD_LEN, 0},	// MCU_ADDRESS /*SEQ_PREVIEW_3_FD*/
    {0x0990, 0x0002, WORD_LEN, 0},	// MCU_DATA_0
    {0x098C, 0xA404, WORD_LEN, 0},	// MCU_ADDRESS /*FD_MODE*/
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0
    {0x3658, 0x0110, WORD_LEN, 0},
    {0x365A, 0xBC09, WORD_LEN, 0},
    {0x365C, 0x59F3, WORD_LEN, 0},
    {0x365E, 0x02F0, WORD_LEN, 0},
    {0x3660, 0x9895, WORD_LEN, 0},
    {0x3680, 0x19AC, WORD_LEN, 0},
    {0x3682, 0x83AC, WORD_LEN, 0},
    {0x3684, 0x1DD1, WORD_LEN, 0},
    {0x3686, 0x1B93, WORD_LEN, 0},
    {0x3688, 0xC473, WORD_LEN, 0},
    {0x36A8, 0x20F4, WORD_LEN, 0},
    {0x36AA, 0x97B2, WORD_LEN, 0},
    {0x36AC, 0x9FD6, WORD_LEN, 0},
    {0x36AE, 0xA0D6, WORD_LEN, 0},
    {0x36B0, 0x1379, WORD_LEN, 0},
    {0x36D0, 0x2012, WORD_LEN, 0},
    {0x36D2, 0x1A90, WORD_LEN, 0},
    {0x36D4, 0x25F4, WORD_LEN, 0},
    {0x36D6, 0x8854, WORD_LEN, 0},
    {0x36D8, 0x62D1, WORD_LEN, 0},
    {0x36F8, 0xF555, WORD_LEN, 0},
    {0x36FA, 0x4D75, WORD_LEN, 0},
    {0x36FC, 0x16B8, WORD_LEN, 0},
    {0x36FE, 0x4DB9, WORD_LEN, 0},
    {0x3700, 0xD65C, WORD_LEN, 0},
    {0x364E, 0x01B0, WORD_LEN, 0},
    {0x3650, 0x7D06, WORD_LEN, 0},
    {0x3652, 0x5473, WORD_LEN, 0},
    {0x3654, 0x1D30, WORD_LEN, 0},
    {0x3656, 0xADD5, WORD_LEN, 0},
    {0x3676, 0xA44C, WORD_LEN, 0},
    {0x3678, 0x81AD, WORD_LEN, 0},
    {0x367A, 0xCDC6, WORD_LEN, 0},
    {0x367C, 0x0A73, WORD_LEN, 0},
    {0x367E, 0xD972, WORD_LEN, 0},
    {0x369E, 0x1B74, WORD_LEN, 0},
    {0x36A0, 0x8892, WORD_LEN, 0},
    {0x36A2, 0xEB76, WORD_LEN, 0},
    {0x36A4, 0xB336, WORD_LEN, 0},
    {0x36A6, 0x3E79, WORD_LEN, 0},
    {0x36C6, 0x698F, WORD_LEN, 0},
    {0x36C8, 0x8A71, WORD_LEN, 0},
    {0x36CA, 0x46F5, WORD_LEN, 0},
    {0x36CC, 0xB48F, WORD_LEN, 0},
    {0x36CE, 0x2D35, WORD_LEN, 0},
    {0x36EE, 0x8316, WORD_LEN, 0},
    {0x36F0, 0x45B5, WORD_LEN, 0},
    {0x36F2, 0x1259, WORD_LEN, 0},
    {0x36F4, 0x48D9, WORD_LEN, 0},
    {0x36F6, 0x8D3D, WORD_LEN, 0},
    {0x3662, 0x0170, WORD_LEN, 0},
    {0x3664, 0x4CCD, WORD_LEN, 0},
    {0x3666, 0x4D93, WORD_LEN, 0},
    {0x3668, 0x73AF, WORD_LEN, 0},
    {0x366A, 0xCF35, WORD_LEN, 0},
    {0x368A, 0x3C0D, WORD_LEN, 0},
    {0x368C, 0x02AD, WORD_LEN, 0},
    {0x368E, 0xFB6E, WORD_LEN, 0},
    {0x3690, 0x08D3, WORD_LEN, 0},
    {0x3692, 0x9572, WORD_LEN, 0},
    {0x36B2, 0x0D94, WORD_LEN, 0},
    {0x36B4, 0xE931, WORD_LEN, 0},
    {0x36B6, 0x99B7, WORD_LEN, 0},
    {0x36B8, 0xB8B6, WORD_LEN, 0},
    {0x36BA, 0x027A, WORD_LEN, 0},
    {0x36DA, 0x4272, WORD_LEN, 0},
    {0x36DC, 0xE331, WORD_LEN, 0},
    {0x36DE, 0x1195, WORD_LEN, 0},
    {0x36E0, 0x4635, WORD_LEN, 0},
    {0x36E2, 0x9536, WORD_LEN, 0},
    {0x3702, 0x8456, WORD_LEN, 0},
    {0x3704, 0x4155, WORD_LEN, 0},
    {0x3706, 0x4499, WORD_LEN, 0},
    {0x3708, 0x61F9, WORD_LEN, 0},
    {0x370A, 0x845D, WORD_LEN, 0},
    {0x366C, 0x00B0, WORD_LEN, 0},
    {0x366E, 0x86AD, WORD_LEN, 0},
    {0x3670, 0x5333, WORD_LEN, 0},
    {0x3672, 0x09F0, WORD_LEN, 0},
    {0x3674, 0xB115, WORD_LEN, 0},
    {0x3694, 0xFFEA, WORD_LEN, 0},
    {0x3696, 0xA2EC, WORD_LEN, 0},
    {0x3698, 0x458D, WORD_LEN, 0},
    {0x369A, 0x5212, WORD_LEN, 0},
    {0x369C, 0xC491, WORD_LEN, 0},
    {0x36BC, 0x1AB4, WORD_LEN, 0},
    {0x36BE, 0x9712, WORD_LEN, 0},
    {0x36C0, 0x80F7, WORD_LEN, 0},
    {0x36C2, 0xB3D6, WORD_LEN, 0},
    {0x36C4, 0x69F9, WORD_LEN, 0},
    {0x36E4, 0x74CE, WORD_LEN, 0},
    {0x36E6, 0x8272, WORD_LEN, 0},
    {0x36E8, 0x46F5, WORD_LEN, 0},
    {0x36EA, 0x7F95, WORD_LEN, 0},
    {0x36EC, 0xA176, WORD_LEN, 0},
    {0x370C, 0xF575, WORD_LEN, 0},
    {0x370E, 0x2815, WORD_LEN, 0},
    {0x3710, 0x5759, WORD_LEN, 0},
    {0x3712, 0x167A, WORD_LEN, 0},
    {0x3714, 0xC6DD, WORD_LEN, 0},
    {0x3644, 0x0134, WORD_LEN, 0},
    {0x3642, 0x00E8, WORD_LEN, 0}, 	// POLY_ORIGIN_R
    {0x3210, 0x09B8, WORD_LEN, 0}, 	// COLOR_PIPELINE_CONTROL
    {0x098c, 0x2306, WORD_LEN, 0},  //awb_and_ccm //2306  //2306  //2306    //2306	// MCU_ADDRESS  /*AWB_CCM_L_0*/
    {0x0990, 0x021B, WORD_LEN, 0},  //0264  //0303  //0303    //0303	// MCU_DATA_0
    {0x098c, 0x2308, WORD_LEN, 0},  //2308  //2308  //2308    //2308	// MCU_ADDRESS
    {0x0990, 0xFF0D, WORD_LEN, 0},  //FECF  //fe7d  //FE7D    //fe7d	// MCU_DATA_0
    {0x098c, 0x230A, WORD_LEN, 0},  //230A  //230a  //230A    //230a	// MCU_ADDRESS
    {0x0990, 0xFFF0, WORD_LEN, 0},  //FFE9  //ffe7  //FFE7    //ffe7	// MCU_DATA_0
    {0x098c, 0x230C, WORD_LEN, 0},  //230C  //230c  //230C    //230c	// MCU_ADDRESS
    {0x0990, 0xFFAB, WORD_LEN, 0},  //FF92  //ffd0  //FFC6    //ffd0	// MCU_DATA_0
    {0x098c, 0x230E, WORD_LEN, 0},  //230E  //230e  //230E    //230e	// MCU_ADDRESS
    {0x0990, 0x01D3, WORD_LEN, 0},  //0205  //0203  //01DC    //0203	// MCU_DATA_0
    {0x098c, 0x2310, WORD_LEN, 0},  //2310  //2310  //2310    //2310	// MCU_ADDRESS
    {0x0990, 0xFF99, WORD_LEN, 0},  //FF84  //ff93  //FFC4    //ff93	// MCU_DATA_0
    {0x098c, 0x2312, WORD_LEN, 0},  //2312  //2312  //2312    //2312	// MCU_ADDRESS
    {0x0990, 0xFF61, WORD_LEN, 0},  //FF3C  //ffb9  //FFB9    //ffb9	// MCU_DATA_0
    {0x098c, 0x2314, WORD_LEN, 0},  //2314  //2314  //2314    //2314	// MCU_ADDRESS
    {0x0990, 0xFE71, WORD_LEN, 0},  //FE1A  //fdcf  //FDCF    //fdcf	// MCU_DATA_0
    {0x098c, 0x2316, WORD_LEN, 0},  //2316  //2316  //2316    //2316	// MCU_ADDRESS  /*AWB_CCM_L_8*/
    {0x0990, 0x0324, WORD_LEN, 0},  //039E  //0301  //0301    //0301	// MCU_DATA_0
    {0x098c, 0x2318, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0020, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x231a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x003c, WORD_LEN, 0},	// MCU_DATA_0
#if 0
    {0x098c, 0x231C, WORD_LEN, 0},  // MCU_ADDRESS  /*AWB_CCM_RL_0*/
    {0x0990, 0xFF08, WORD_LEN, 0},  // MCU_DATA_0
    {0x098c, 0x231E, WORD_LEN, 0},  // MCU_ADDRESS
    {0x0990, 0x0015, WORD_LEN, 0},  // MCU_DATA_0
    {0x098c, 0x2320, WORD_LEN, 0},  // MCU_ADDRESS
    {0x0990, 0x0040, WORD_LEN, 0},  // MCU_DATA_0
    {0x098c, 0x2322, WORD_LEN, 0},  // MCU_ADDRESS
    {0x0990, 0x001E, WORD_LEN, 0},  // MCU_DATA_0
    {0x098c, 0x2324, WORD_LEN, 0},  // MCU_ADDRESS
    {0x0990, 0xFFCB, WORD_LEN, 0},  // MCU_DATA_0
    {0x098c, 0x2326, WORD_LEN, 0},  // MCU_ADDRESS
    {0x0990, 0xFFF7, WORD_LEN, 0},  // MCU_DATA_0
    {0x098c, 0x2328, WORD_LEN, 0},  // MCU_ADDRESS
    {0x0990, 0x010A, WORD_LEN, 0},  // MCU_DATA_0
    {0x098c, 0x232A, WORD_LEN, 0},  // MCU_ADDRESS
    {0x0990, 0x0015, WORD_LEN, 0},  // MCU_DATA_0
    {0x098c, 0x232C, WORD_LEN, 0},  // MCU_ADDRESS  /*AWB_CCM_RL_8*/
    {0x0990, 0x001F, WORD_LEN, 0},  // MCU_DATA_0
#else
    {0x098c, 0x231C, WORD_LEN, 0},  //231C  //231c  //231C  //231C    //231c	// MCU_ADDRESS  /*AWB_CCM_RL_0*/
    {0x0990, 0xFF08, WORD_LEN, 0},  //FF43  //fe07  //FE3B  //FE47    //fe07	// MCU_DATA_0
    {0x098c, 0x231E, WORD_LEN, 0},  //231E  //231e  //231E  //231E    //231e	// MCU_ADDRESS
    {0x0990, 0x0015, WORD_LEN, 0},  //FFE0  //00be  //005E  //0048    //00be	// MCU_DATA_0
    {0x098c, 0x2320, WORD_LEN, 0},  //2320  //2320  //2320  //2320    //2320	// MCU_ADDRESS
    {0x0990, 0x0040, WORD_LEN, 0},  //003A  //004d  //0054  //0055    //004d	// MCU_DATA_0
    {0x098c, 0x2322, WORD_LEN, 0},  //2322  //2322  //2322  //2322    //2322	// MCU_ADDRESS
    {0x0990, 0x001E, WORD_LEN, 0},  //001E  //ffe2  //FFB7  //FFAD    //ffe2	// MCU_DATA_0
    {0x098c, 0x2324, WORD_LEN, 0},  //2324  //2324  //2324  //2324    //2324	// MCU_ADDRESS
    {0x0990, 0xFFCB, WORD_LEN, 0},  //FFCB  //ffcb  //0023  //0035    //ffcb	// MCU_DATA_0
    {0x098c, 0x2326, WORD_LEN, 0},  //2326  //2326  //2326  //2326    //2326	// MCU_ADDRESS
    {0x0990, 0xFFF7, WORD_LEN, 0},  //FFF7  //ffe8  //FFB9  //FFB1    //ffe8	// MCU_DATA_0
    {0x098c, 0x2328, WORD_LEN, 0},  //2328  //2328  //2328  //2328    //2328	// MCU_ADDRESS
    {0x0990, 0x010A, WORD_LEN, 0},  //010A  //00a4  //00A9  //00AA    //00a4	// MCU_DATA_0
    {0x098c, 0x232A, WORD_LEN, 0},  //232A  //232a  //232A  //232A    //232a	// MCU_ADDRESS
    {0x0990, 0x0015, WORD_LEN, 0},  //0015  //ff55  //FE61  //FE29    //ff55	// MCU_DATA_0
    {0x098c, 0x232C, WORD_LEN, 0},  //232C  //232c  //232C  //232C    //232c	// MCU_ADDRESS  /*AWB_CCM_RL_8*/
    {0x0990, 0x001F, WORD_LEN, 0},  //001F  //01a2  //02E2  //032C    //01a2	// MCU_DATA_0
#endif
    {0x098c, 0x232e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2330, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffe4, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa348, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0008, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa349, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0002, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0059, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34b, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00e6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa351, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa352, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x007f, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa355, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0001, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35d, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0078, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0086, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x007e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa360, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0082, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2361, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa363, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00d6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa364, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00e2, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa302, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa303, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00ef, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00c7, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa350, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2361, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa366, WORD_LEN, 0},	// 
    {0x0990, 0x007C, WORD_LEN, 0},	// awb_kr_l
    {0x098c, 0xa367, WORD_LEN, 0},	// 
    {0x0990, 0x0082, WORD_LEN, 0},	//
    {0x098c, 0xa368, WORD_LEN, 0},	// 
    {0x0990, 0x0082, WORD_LEN, 0},	// awb_kb_l
    {0x098c, 0xa369, WORD_LEN, 0},	//
    {0x0990, 0x007A, WORD_LEN, 0},	// awb_kr_r
    {0x098c, 0xa36a, WORD_LEN, 0},
    {0x0990, 0x0074, WORD_LEN, 0},
    {0x098c, 0xa36b, WORD_LEN, 0},  //
    {0x0990, 0x007A, WORD_LEN, 0},  //awb_kb_r
    {0x098c, 0xa365, WORD_LEN, 0},  //
    {0x0990, 0x0040, WORD_LEN, 0},  //awb_x0
    {0x098c, 0xa34d, WORD_LEN, 0},
    {0x0990, 0x00a6, WORD_LEN, 0},
    {0x098C, 0xA363, WORD_LEN, 0}, 	///*AWB and CCMs 02/03/10 11:50:40*/// MCU_ADDRESS /*AWB_TG_MIN0*/
    {0x0990, 0x00DA, WORD_LEN, 0}, 	//E0 //D6 // MCU_DATA_0
    {0x098C, 0xA364, WORD_LEN, 0}, 	// MCU_ADDRESS /*AWB_TG_MAX0*/
    {0x0990, 0x00EC, WORD_LEN, 0}, 	//EC // MCU_DATA_0
    {0x098C, 0xAB3C, WORD_LEN, 0}, 	//Gamma_A & B // MCU_ADDRESS /*HG_GAMMA_TABLE_A_0*/
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB3D, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_1*/
    {0x0990, 0x000A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB3E, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_2*/
    {0x0990, 0x001E, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB3F, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_3*/
    {0x0990, 0x0039, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB40, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_4*/
    {0x0990, 0x0059, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB41, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_5*/
    {0x0990, 0x0071, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB42, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_6*/
    {0x0990, 0x0086, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB43, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_7*/
    {0x0990, 0x0097, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB44, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_8*/
    {0x0990, 0x00A6, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB45, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_9*/
    {0x0990, 0x00B3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB46, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_10*/
    {0x0990, 0x00BF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB47, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_11*/
    {0x0990, 0x00C9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB48, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_12*/
    {0x0990, 0x00D3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB49, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_13*/
    {0x0990, 0x00DB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB4A, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_14*/
    {0x0990, 0x00E4, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB4B, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_15*/
    {0x0990, 0x00EB, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB4C, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_16*/
    {0x0990, 0x00F2, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB4D, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_17*/
    {0x0990, 0x00F9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB4E, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_A_18*/
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB4F, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_0*/
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB50, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_1*/
    {0x0990, 0x0019, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB51, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_2*/
    {0x0990, 0x0032, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB52, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_3*/
    {0x0990, 0x004B, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB53, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_4*/
    {0x0990, 0x006A, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB54, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_5*/
    {0x0990, 0x0080, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB55, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_6*/
    {0x0990, 0x0092, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB56, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_7*/
    {0x0990, 0x00A1, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB57, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_8*/
    {0x0990, 0x00AE, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB58, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_9*/
    {0x0990, 0x00B9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB59, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_10*/
    {0x0990, 0x00C3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB5A, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_11*/
    {0x0990, 0x00CD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB5B, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_12*/
    {0x0990, 0x00D5, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB5C, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_13*/
    {0x0990, 0x00DD, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB5D, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_14*/
    {0x0990, 0x00E5, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB5E, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_15*/
    {0x0990, 0x00EC, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB5F, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_16*/
    {0x0990, 0x00F3, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB60, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_17*/
    {0x0990, 0x00F9, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xAB61, WORD_LEN, 0}, 	// MCU_ADDRESS /*HG_GAMMA_TABLE_B_18*/
    {0x0990, 0x00FF, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xa244, WORD_LEN, 0}, 	//drt_off// mcu_address
    {0x0990, 0x0008, WORD_LEN, 0}, 	// mcu_data_0
    {0x098C, 0xA24F, WORD_LEN, 0}, 	//base_target// MCU_ADDRESS
    {0x0990, 0x0040, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xA202, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0011, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098C, 0xA203, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x00dd, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x327A, 0x002B, WORD_LEN, 0}, //offset	//2C 	// BLACK_LEVEL_1ST_RED
    //{0x327c, 0x002a, WORD_LEN, 0},          // BLACK_LEVEL_green1
    //{0x327e, 0x002a, WORD_LEN, 0},          // BLACK_LEVEL_green2
    {0x3280, 0x002B, WORD_LEN, 0}, 	////2E 	// BLACK_LEVEL_1ST_BLUE
    {0x098C, 0xAB04, WORD_LEN, 0},  	// MCU_ADDRESS
    {0x0990, 0x0008, WORD_LEN, 0}, 	//14 	// MCU_DATA_0
    {0x098c, 0xab1f, WORD_LEN, 0}, 	//low_light // MCU_ADDRESS
    {0x0990, 0x00c6, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab20, WORD_LEN, 0}, 	// MCU_ADDRESS //hg_ll_sat1
    {0x0990, 0x0076, WORD_LEN, 0},   //63	// MCU_DATA_0
    {0x098c, 0xab21, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x001f, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab22, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0003, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab23, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0005, WORD_LEN, 0},   //05	// MCU_DATA_0
    {0x098c, 0xab24, WORD_LEN, 0}, 	// MCU_ADDRESS //hg_ll_sat2
    {0x0990, 0x0046, WORD_LEN, 0},   //33  // MCU_DATA_0
    {0x098c, 0xab25, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0040, WORD_LEN, 0},   //70  //c0	// MCU_DATA_0
    {0x098c, 0xab26, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab27, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},   //06	// MCU_DATA_0
    {0x098c, 0x2b28, WORD_LEN, 0}, 	// MCU_ADDRESS //hg_ll_brightness_start
    {0x0990, 0x1000, WORD_LEN, 0},   //600	// MCU_DATA_0
    {0x098c, 0x2b2a, WORD_LEN, 0}, 	// MCU_ADDRESS //hg_ll_brightness_stop
    {0x0990, 0x2000, WORD_LEN, 0},   //1000	// MCU_DATA_0
    {0x098c, 0xab2c, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab2d, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x000a, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab2e, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab2f, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab30, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab31, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x000e, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab32, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab33, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0}, 	// MCU_DATA_0
    {0x098c, 0xab34, WORD_LEN, 0}, 	// MCU_ADDRESS  //nr_gain_start
    {0x0990, 0x0002, WORD_LEN, 0},   //08	// MCU_DATA_0
    {0x098c, 0xab35, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0}, 	// MCU_DATA_0    
    {0x098C, 0xA11D, WORD_LEN, 0},  //ae_settings	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},  	// mcu_data_0
    {0x098C, 0xA208, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x0000, WORD_LEN, 0}, 	//3 	// mcu_data_0
    {0x098C, 0xA209, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},  	// mcu_data_0
    {0x098C, 0xA20A, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x001F, WORD_LEN, 0},  	// mcu_data_0
    {0x098C, 0xA216, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x003c, WORD_LEN, 0},  //42 	// mcu_data_0
    {0x098C, 0xA207, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x0005, WORD_LEN, 0},  //A 	// mcu_data_0
    {0x098C, 0xA20D, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x0020, WORD_LEN, 0},  	// mcu_data_0
    {0x098C, 0xA20E, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x0080, WORD_LEN, 0},  	// mcu_data_0
    {0x098C, 0x2212, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x0200, WORD_LEN, 0}, 	    //0b9 // mcu_data_0
    {0x098C, 0xA129, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},  	// mcu_data_0
    #if 0
    {0x098C, 0x2703, WORD_LEN, 0}, //others_wizard	//Output Width (A)
    {0x0990, 0x0280, WORD_LEN, 0}, 	//      = 640
    {0x098C, 0x2705, WORD_LEN, 0}, 	//Output Height (A)
    {0x0990, 0x01E4, WORD_LEN, 0}, 	//      = 484
    {0x098C, 0x2707, WORD_LEN, 0}, 	//Output Width (B)
    {0x0990, 0x0280, WORD_LEN, 0}, 	//      = 640
    {0x098C, 0x2709, WORD_LEN, 0}, 	//Output Height (B)
    {0x0990, 0x01E4, WORD_LEN, 0}, 	//1E0	//      = 480
    {0x098C, 0x270D, WORD_LEN, 0}, 	//Row Start (A)
    {0x0990, 0x0000, WORD_LEN, 0}, 	//      = 0
    {0x098C, 0x270F, WORD_LEN, 0}, 	//Column Start (A)
    {0x0990, 0x0000, WORD_LEN, 0}, 	//      = 0
    {0x098C, 0x2711, WORD_LEN, 0}, 	//Row End (A)
    {0x0990, 0x01eB, WORD_LEN, 0}, 	//1E7	//      = 487
    {0x098C, 0x2713, WORD_LEN, 0}, 	//Column End (A)
    {0x0990, 0x0287, WORD_LEN, 0}, 	//      = 647
    {0x098C, 0x2715, WORD_LEN, 0}, 	//Row Speed (A)
    {0x0990, 0x0001, WORD_LEN, 0}, 	//      = 1
    {0x098C, 0x2717, WORD_LEN, 0}, 	//Read Mode (A)
    {0x0990, 0x0027, WORD_LEN, 0}, 	//      = 38
    {0x098C, 0x2719, WORD_LEN, 0}, 	//sensor_fine_correction (A)
    {0x0990, 0x001A, WORD_LEN, 0}, 	//      = 26
    {0x098C, 0x271B, WORD_LEN, 0}, 	//sensor_fine_IT_min (A)
    {0x0990, 0x006B, WORD_LEN, 0}, 	//      = 107
    {0x098C, 0x271D, WORD_LEN, 0}, 	//sensor_fine_IT_max_margin (A)
    {0x0990, 0x006B, WORD_LEN, 0}, 	//      = 107
    {0x098C, 0x271F, WORD_LEN, 0}, 	//Frame Lines (A)
    {0x0990, 0x02A9, WORD_LEN, 0}, 	//2A5	//20b	//7	//      = 519
    {0x098C, 0x2721, WORD_LEN, 0}, 	//Line Length (A)
    {0x0990, 0x047C, WORD_LEN, 0}, 	//5DA	//      = 1498
    {0x098C, 0x2723, WORD_LEN, 0}, 	//Row Start (B)
    {0x0990, 0x0000, WORD_LEN, 0}, 	//      = 0
    {0x098C, 0x2725, WORD_LEN, 0}, 	//Column Start (B)
    {0x0990, 0x0000, WORD_LEN, 0}, 	//      = 0
    {0x098C, 0x2727, WORD_LEN, 0}, 	//Row End (B)
    {0x0990, 0x01eb, WORD_LEN, 0}, 	//1E7	//      = 487
    {0x098C, 0x2729, WORD_LEN, 0}, 	//Column End (B)
    {0x0990, 0x0287, WORD_LEN, 0}, 	//      = 647
    {0x098C, 0x272B, WORD_LEN, 0}, 	//Row Speed (B)
    {0x0990, 0x0001, WORD_LEN, 0}, 	//      = 1
    {0x098C, 0x272D, WORD_LEN, 0}, 	//Read Mode (B)
    {0x0990, 0x0027, WORD_LEN, 0}, 	//      = 38
    {0x098C, 0x272F, WORD_LEN, 0}, 	//sensor_fine_correction (B)
    {0x0990, 0x001A, WORD_LEN, 0}, 	//      = 26
    {0x098C, 0x2731, WORD_LEN, 0}, 	//sensor_fine_IT_min (B)
    {0x0990, 0x006B, WORD_LEN, 0}, 	//      = 107
    {0x098C, 0x2733, WORD_LEN, 0}, 	//sensor_fine_IT_max_margin (B)
    {0x0990, 0x006B, WORD_LEN, 0}, 	//      = 107
    {0x098C, 0x2735, WORD_LEN, 0}, 	//Frame Lines (B)
    {0x0990, 0x02A9, WORD_LEN, 0}, 	// A5	//207	//      = 519
    {0x098C, 0x2737, WORD_LEN, 0}, 	//Line Length (B)
    {0x0990, 0x047C, WORD_LEN, 0}, 	//5DA	//      = 1498
    {0x098C, 0x2739, WORD_LEN, 0}, 	//Crop_X0 (A)
    {0x0990, 0x0000, WORD_LEN, 0}, 	//      = 0
    {0x098C, 0x273B, WORD_LEN, 0}, 	//Crop_X1 (A)
    {0x0990, 0x027F, WORD_LEN, 0}, 	//      = 639
    {0x098C, 0x273D, WORD_LEN, 0}, 	//Crop_Y0 (A)
    {0x0990, 0x0000, WORD_LEN, 0}, 	//      = 0
    {0x098C, 0x273F, WORD_LEN, 0}, 	//Crop_Y1 (A)
    {0x0990, 0x01E3, WORD_LEN, 0}, 	// DF	//      = 479
    {0x098C, 0x2747, WORD_LEN, 0}, 	//Crop_X0 (B)
    {0x0990, 0x0000, WORD_LEN, 0}, 	//      = 0
    {0x098C, 0x2749, WORD_LEN, 0}, 	//Crop_X1 (B)
    {0x0990, 0x027F, WORD_LEN, 0}, 	//      = 639
    {0x098C, 0x274B, WORD_LEN, 0}, 	//Crop_Y0 (B)
    {0x0990, 0x0000, WORD_LEN, 0}, 	//      = 0
    {0x098C, 0x274D, WORD_LEN, 0}, 	//Crop_Y1 (B)
    {0x0990, 0x01E3, WORD_LEN, 0},  // DF	//      = 479
    {0x098C, 0x222D, WORD_LEN, 0}, 	//R9 Step
    {0x0990, 0x0059, WORD_LEN, 0}, 	//      = 89
    {0x098C, 0xA408, WORD_LEN, 0}, 	//search_f1_50
    {0x0990, 0x1C  , WORD_LEN, 0}, //      = 28
    {0x098C, 0xA409, WORD_LEN, 0}, 	//search_f2_50
    {0x0990, 0x1E  , WORD_LEN, 0}, 	//      = 30
    {0x098C, 0xA40A, WORD_LEN, 0}, 	//search_f1_60
    {0x0990, 0x22  , WORD_LEN, 0}, //      = 34
    {0x098C, 0xA40B, WORD_LEN, 0}, 	//search_f2_60
    {0x0990, 0x24  , WORD_LEN, 0}, 	//      = 36
    {0x098C, 0x2411, WORD_LEN, 0}, 	//R9_Step_60 (A)
    {0x0990, 0x0059, WORD_LEN, 0}, 	//      = 89
    {0x098C, 0x2413, WORD_LEN, 0}, 	//R9_Step_50 (A)
    {0x0990, 0x006B, WORD_LEN, 0}, 	//      = 107
    {0x098C, 0x2415, WORD_LEN, 0}, 	//R9_Step_60 (B)
    {0x0990, 0x0059, WORD_LEN, 0}, 	//      = 89
    {0x098C, 0x2417, WORD_LEN, 0}, 	//R9_Step_50 (B)
    {0x0990, 0x006B, WORD_LEN, 0}, 	//      = 107
    {0x098C, 0xA404, WORD_LEN, 0}, 	//FD Mode
    {0x0990, 0x0010, WORD_LEN, 0}, 	//      = 16
    {0x098C, 0xA40D, WORD_LEN, 0}, 	//Stat_min
    {0x0990, 0x0002, WORD_LEN, 0}, 	//      = 2
    {0x098C, 0xA40E, WORD_LEN, 0}, 	//Stat_max
    {0x0990, 0x0003, WORD_LEN, 0}, 	//      = 3
    {0x098C, 0xA410, WORD_LEN, 0}, 	//Min_amplitude
    {0x0990, 0x000A, WORD_LEN, 0}, 	//      = 10
    #endif
    #if 0
    {0x098C, 0xA124, WORD_LEN, 0},   // MCU_ADDRESS /*SEQ_PREVIEW_2_FD*/
    {0x0990, 0x0002, WORD_LEN, 0},   // MCU_DATA_0
    {0x098C, 0xA12A, WORD_LEN, 0},   // MCU_ADDRESS /*SEQ_PREVIEW_3_FD*/
    {0x0990, 0x0002, WORD_LEN, 0},   // MCU_DATA_0
    {0x098C, 0xA404, WORD_LEN, 0},  	// MCU_ADDRESS /*FD_MODE*/
    {0x0990, 0x0080, WORD_LEN, 0},  	// MCU_DATA_0
    #endif
//refresh
    {0x098c, 0xa103, WORD_LEN, 0},  	// mcu_address
    {0x0990, 0x0006, WORD_LEN, 0},  	// mcu_data_0
//poll_field=seq_cmd, !=0, delay=10, timeout=100  // wait for command to be processed	//delay=300
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0},
    {0x098C, 0xA103, WORD_LEN, 0}, 	// MCU_ADDRESS
    {0x0990, 0x0005, WORD_LEN, 0}, 	// MCU_DATA_0
//poll_field=seq_cmd, !=0, delay=10, timeout=100  // wait for command to be processed	//delay=300
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0},
    #if 0
	#if 0//AT1_BDVER_G(AT1_WS22)
    {0x0990, 0x0026, WORD_LEN, 0},
    #else
    {0x0990, 0x0025, WORD_LEN, 0},
    #endif
	{0x098c, 0x272d, WORD_LEN, 0},
	#if 0//AT1_BDVER_G(AT1_WS22)
    {0x0990, 0x0026, WORD_LEN, 0},
    #else
    {0x0990, 0x0025, WORD_LEN, 0},
	#endif
    {0x098c, 0xa103, WORD_LEN, 0},
    {0x0990, 0x0006, WORD_LEN, 0},
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    #endif
#if 0
    {0x8000, 0x3214, POLL_REG, 0},
    {0x0014, 0xb04a, WORD_LEN, 0}, // PLL_CONTROL
    {0x098c, 0x2703, WORD_LEN, 0},
    {0x0990, 0x0280, WORD_LEN, 0},
    {0x098c, 0x2705, WORD_LEN, 0},
    {0x0990, 0x01e0, WORD_LEN, 0},
    {0x098c, 0x2707, WORD_LEN, 0},
    {0x0990, 0x0280, WORD_LEN, 0},
    {0x098c, 0x2709, WORD_LEN, 0},
    {0x0990, 0x01e0, WORD_LEN, 0},
    {0x098c, 0x270d, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x270f, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x2711, WORD_LEN, 0},
    {0x0990, 0x01e7, WORD_LEN, 0},
    {0x098c, 0x2713, WORD_LEN, 0},
    {0x0990, 0x0287, WORD_LEN, 0},
    {0x098c, 0x2715, WORD_LEN, 0},
    {0x0990, 0x0001, WORD_LEN, 0},
    {0x098c, 0x2717, WORD_LEN, 0},
	#if 0//AT1_BDVER_G(AT1_WS22)
    {0x0990, 0x0026, WORD_LEN, 0},
    #else
    {0x0990, 0x0025, WORD_LEN, 0},
    #endif
    {0x098c, 0x2719, WORD_LEN, 0},
    {0x0990, 0x001a, WORD_LEN, 0},
    {0x098c, 0x271b, WORD_LEN, 0},
    {0x0990, 0x006b, WORD_LEN, 0},
    {0x098c, 0x271d, WORD_LEN, 0},
    {0x0990, 0x006b, WORD_LEN, 0},
    {0x098c, 0x271f, WORD_LEN, 0}, //Frame Lines (A)
    {0x0990, 0x039B, WORD_LEN, 0},
    {0x098c, 0x2721, WORD_LEN, 0},
    {0x0990, 0x034A, WORD_LEN, 0},
    {0x098c, 0x2723, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x2725, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x2727, WORD_LEN, 0},
    {0x0990, 0x01e7, WORD_LEN, 0},
    {0x098c, 0x2729, WORD_LEN, 0},
    {0x0990, 0x0287, WORD_LEN, 0},
    {0x098c, 0x272b, WORD_LEN, 0},
    {0x0990, 0x0001, WORD_LEN, 0},
    {0x098c, 0x272d, WORD_LEN, 0},
	#if 0//AT1_BDVER_G(AT1_WS22)
    {0x0990, 0x0026, WORD_LEN, 0},
    #else
    {0x0990, 0x0025, WORD_LEN, 0},
	#endif
    {0x098c, 0x272f, WORD_LEN, 0},
    {0x0990, 0x001a, WORD_LEN, 0},
    {0x098c, 0x2731, WORD_LEN, 0},
    {0x0990, 0x006b, WORD_LEN, 0},
    {0x098c, 0x2733, WORD_LEN, 0},
    {0x0990, 0x006b, WORD_LEN, 0},
    {0x098c, 0x2735, WORD_LEN, 0}, //Frame Lines (B)
    {0x0990, 0x039B, WORD_LEN, 0},
    {0x098c, 0x2737, WORD_LEN, 0},
    {0x0990, 0x034A, WORD_LEN, 0},
    {0x098c, 0x2739, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x273b, WORD_LEN, 0},
    {0x0990, 0x027f, WORD_LEN, 0},
    {0x098c, 0x273d, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x273f, WORD_LEN, 0},
    {0x0990, 0x01df, WORD_LEN, 0},
    {0x098c, 0x2747, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x2749, WORD_LEN, 0},
    {0x0990, 0x027f, WORD_LEN, 0},
    {0x098c, 0x274b, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x274d, WORD_LEN, 0},
    {0x0990, 0x01df, WORD_LEN, 0},
    {0x098c, 0x222d, WORD_LEN, 0},
    {0x0990, 0x007A, WORD_LEN, 0},
    {0x098c, 0xa408, WORD_LEN, 0}, //Flicker1_50
    {0x0990, 0x001D, WORD_LEN, 0},
    {0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
    {0x0990, 0x001F, WORD_LEN, 0},
    {0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
    {0x0990, 0x0023, WORD_LEN, 0},
    {0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
    {0x0990, 0x0025, WORD_LEN, 0},
    {0x098c, 0x2411, WORD_LEN, 0},
    {0x0990, 0x007A, WORD_LEN, 0},
    {0x098c, 0x2413, WORD_LEN, 0},
    {0x0990, 0x0092, WORD_LEN, 0},
    {0x098c, 0x2415, WORD_LEN, 0},
    {0x0990, 0x007A, WORD_LEN, 0},
    {0x098c, 0x2417, WORD_LEN, 0},
    {0x0990, 0x0092, WORD_LEN, 0},
    {0x098c, 0xa404, WORD_LEN, 0},
    {0x0990, 0x0010, WORD_LEN, 0},
    {0x098c, 0xa40d, WORD_LEN, 0},
    {0x0990, 0x0002, WORD_LEN, 0},
    {0x098c, 0xa40e, WORD_LEN, 0},
    {0x0990, 0x0003, WORD_LEN, 0},
    {0x098c, 0xa410, WORD_LEN, 0},
    {0x0990, 0x000a, WORD_LEN, 0},

//lens_correction
    {0x3658, 0x0110, WORD_LEN, 0},	// P_RD_P0Q0
    {0x365a, 0xbc09, WORD_LEN, 0},	// P_RD_P0Q1
    {0x365c, 0x59f3, WORD_LEN, 0},	// P_RD_P0Q2
    {0x365e, 0x02f0, WORD_LEN, 0},	// P_RD_P0Q3
    {0x3660, 0x9895, WORD_LEN, 0},	// P_RD_P0Q4
    {0x3680, 0x19ac, WORD_LEN, 0},	// P_RD_P1Q0
    {0x3682, 0x83ac, WORD_LEN, 0},	// P_RD_P1Q1
    {0x3684, 0x1dd1, WORD_LEN, 0},	// P_RD_P1Q2
    {0x3686, 0x1b93, WORD_LEN, 0},	// P_RD_P1Q3
    {0x3688, 0xc473, WORD_LEN, 0},	// P_RD_P1Q4
    {0x36a8, 0x20f4, WORD_LEN, 0},	// P_RD_P2Q0
    {0x36aa, 0x97b2, WORD_LEN, 0},	// P_RD_P2Q1
    {0x36ac, 0x9fd6, WORD_LEN, 0},	// P_RD_P2Q2
    {0x36ae, 0xa0d6, WORD_LEN, 0},	// P_RD_P2Q3
    {0x36b0, 0x1379, WORD_LEN, 0},	// P_RD_P2Q4
    {0x36d0, 0x2012, WORD_LEN, 0},	// P_RD_P3Q0
    {0x36d2, 0x1a90, WORD_LEN, 0},	// P_RD_P3Q1
    {0x36d4, 0x25f4, WORD_LEN, 0},	// P_RD_P3Q2
    {0x36d6, 0x8854, WORD_LEN, 0},	// P_RD_P3Q3
    {0x36d8, 0x62d1, WORD_LEN, 0},	// P_RD_P3Q4
    {0x36f8, 0xf555, WORD_LEN, 0},	// P_RD_P4Q0
    {0x36fa, 0x4d75, WORD_LEN, 0},	// P_RD_P4Q1
    {0x36fc, 0x16b8, WORD_LEN, 0},	// P_RD_P4Q2
    {0x36fe, 0x4db9, WORD_LEN, 0},	// P_RD_P4Q3
    {0x3700, 0xd65c, WORD_LEN, 0},	// P_RD_P4Q4
    {0x364e, 0x01b0, WORD_LEN, 0},	// P_GR_P0Q0
    {0x3650, 0x7d06, WORD_LEN, 0},	// P_GR_P0Q1
    {0x3652, 0x5473, WORD_LEN, 0},	// P_GR_P0Q2
    {0x3654, 0x1d30, WORD_LEN, 0},	// P_GR_P0Q3
    {0x3656, 0xadd5, WORD_LEN, 0},	// P_GR_P0Q4
    {0x3676, 0xa44c, WORD_LEN, 0},	// P_GR_P1Q0
    {0x3678, 0x81ad, WORD_LEN, 0},	// P_GR_P1Q1
    {0x367a, 0xcdc6, WORD_LEN, 0},	// P_GR_P1Q2
    {0x367c, 0x0a73, WORD_LEN, 0},	// P_GR_P1Q3
    {0x367e, 0xd972, WORD_LEN, 0},	// P_GR_P1Q4
    {0x369e, 0x1b74, WORD_LEN, 0},	// P_GR_P2Q0
    {0x36a0, 0x8892, WORD_LEN, 0},	// P_GR_P2Q1
    {0x36a2, 0xeb76, WORD_LEN, 0},	// P_GR_P2Q2
    {0x36a4, 0xb336, WORD_LEN, 0},	// P_GR_P2Q3
    {0x36a6, 0x3e79, WORD_LEN, 0},	// P_GR_P2Q4
    {0x36c6, 0x698f, WORD_LEN, 0},	// P_GR_P3Q0
    {0x36c8, 0x8a71, WORD_LEN, 0},	// P_GR_P3Q1
    {0x36ca, 0x46f5, WORD_LEN, 0},	// P_GR_P3Q2
    {0x36cc, 0xb48f, WORD_LEN, 0},	// P_GR_P3Q3
    {0x36ce, 0x2d35, WORD_LEN, 0},	// P_GR_P3Q4
    {0x36ee, 0x8316, WORD_LEN, 0},	// P_GR_P4Q0
    {0x36f0, 0x45b5, WORD_LEN, 0},	// P_GR_P4Q1
    {0x36f2, 0x1259, WORD_LEN, 0},	// P_GR_P4Q2
    {0x36f4, 0x48d9, WORD_LEN, 0},	// P_GR_P4Q3
    {0x36f6, 0x8d3d, WORD_LEN, 0},	// P_GR_P4Q4
    {0x3662, 0x0170, WORD_LEN, 0},	// P_BL_P0Q0
    {0x3664, 0x4ccd, WORD_LEN, 0},	// P_BL_P0Q1
    {0x3666, 0x4d93, WORD_LEN, 0},	// P_BL_P0Q2
    {0x3668, 0x73af, WORD_LEN, 0},	// P_BL_P0Q3
    {0x366a, 0xcf35, WORD_LEN, 0},	// P_BL_P0Q4
    {0x368a, 0x3c0d, WORD_LEN, 0},	// P_BL_P1Q0
    {0x368c, 0x02ad, WORD_LEN, 0},	// P_BL_P1Q1
    {0x368e, 0xfb6e, WORD_LEN, 0},	// P_BL_P1Q2
    {0x3690, 0x08d3, WORD_LEN, 0},	// P_BL_P1Q3
    {0x3692, 0x9572, WORD_LEN, 0},	// P_BL_P1Q4
    {0x36b2, 0x0d94, WORD_LEN, 0},	// P_BL_P2Q0
    {0x36b4, 0xe931, WORD_LEN, 0},	// P_BL_P2Q1
    {0x36b6, 0x99b7, WORD_LEN, 0},	// P_BL_P2Q2
    {0x36b8, 0xb8b6, WORD_LEN, 0},	// P_BL_P2Q3
    {0x36ba, 0x027a, WORD_LEN, 0},	// P_BL_P2Q4
    {0x36da, 0x4272, WORD_LEN, 0},	// P_BL_P3Q0
    {0x36dc, 0xe331, WORD_LEN, 0},	// P_BL_P3Q1
    {0x36de, 0x1195, WORD_LEN, 0},	// P_BL_P3Q2
    {0x36e0, 0x4635, WORD_LEN, 0},	// P_BL_P3Q3
    {0x36e2, 0x9536, WORD_LEN, 0},	// P_BL_P3Q4
    {0x3702, 0x8456, WORD_LEN, 0},	// P_BL_P4Q0
    {0x3704, 0x4155, WORD_LEN, 0},	// P_BL_P4Q1
    {0x3706, 0x4499, WORD_LEN, 0},	// P_BL_P4Q2
    {0x3708, 0x61f9, WORD_LEN, 0},	// P_BL_P4Q3
    {0x370a, 0x845d, WORD_LEN, 0},	// P_BL_P4Q4
    {0x366c, 0x00b0, WORD_LEN, 0},	// P_GB_P0Q0
    {0x366e, 0x86ad, WORD_LEN, 0},	// P_GB_P0Q1
    {0x3670, 0x5333, WORD_LEN, 0},	// P_GB_P0Q2
    {0x3672, 0x09f0, WORD_LEN, 0},	// P_GB_P0Q3
    {0x3674, 0xb115, WORD_LEN, 0},	// P_GB_P0Q4
    {0x3694, 0xffea, WORD_LEN, 0},	// P_GB_P1Q0
    {0x3696, 0xa2ec, WORD_LEN, 0},	// P_GB_P1Q1
    {0x3698, 0x458d, WORD_LEN, 0},	// P_GB_P1Q2
    {0x369a, 0x5212, WORD_LEN, 0},	// P_GB_P1Q3
    {0x369c, 0xc491, WORD_LEN, 0},	// P_GB_P1Q4
    {0x36bc, 0x1ab4, WORD_LEN, 0},	// P_GB_P2Q0
    {0x36be, 0x9712, WORD_LEN, 0},	// P_GB_P2Q1
    {0x36c0, 0x80f7, WORD_LEN, 0},	// P_GB_P2Q2
    {0x36c2, 0xb3d6, WORD_LEN, 0},	// P_GB_P2Q3
    {0x36c4, 0x69f9, WORD_LEN, 0},	// P_GB_P2Q4
    {0x36e4, 0x74ce, WORD_LEN, 0},	// P_GB_P3Q0
    {0x36e6, 0x8272, WORD_LEN, 0},	// P_GB_P3Q1
    {0x36e8, 0x46f5, WORD_LEN, 0},	// P_GB_P3Q2
    {0x36ea, 0x7f95, WORD_LEN, 0},	// P_GB_P3Q3
    {0x36ec, 0xa176, WORD_LEN, 0},	// P_GB_P3Q4
    {0x370c, 0xf575, WORD_LEN, 0},	// P_GB_P4Q0
    {0x370e, 0x2815, WORD_LEN, 0},	// P_GB_P4Q1
    {0x3710, 0x5759, WORD_LEN, 0},	// P_GB_P4Q2
    {0x3712, 0x167a, WORD_LEN, 0},	// P_GB_P4Q3
    {0x3714, 0xc6dd, WORD_LEN, 0},	// P_GB_P4Q4
    {0x3644, 0x0134, WORD_LEN, 0},	// POLY_ORIGIN_C
    {0x3642, 0x00e8, WORD_LEN, 0},	// POLY_ORIGIN_R
    {0x3210, 0x09b8, WORD_LEN, 0},	// COLOR_PIPELINE_CONTROL
//awb_and_ccm
    {0x098c, 0x2306, WORD_LEN, 0},  
    {0x0990, 0x0264, WORD_LEN, 0}, 
    {0x098c, 0x2308, WORD_LEN, 0}, 
    {0x0990, 0xFECF, WORD_LEN, 0},  
    {0x098c, 0x230A, WORD_LEN, 0},  
    {0x0990, 0xFFE9, WORD_LEN, 0},  
    {0x098c, 0x230C, WORD_LEN, 0},  
    {0x0990, 0xFF92, WORD_LEN, 0},  
    {0x098c, 0x230E, WORD_LEN, 0},  
    {0x0990, 0x0205, WORD_LEN, 0},  
    {0x098c, 0x2310, WORD_LEN, 0},  
    {0x0990, 0xFF84, WORD_LEN, 0},  
    {0x098c, 0x2312, WORD_LEN, 0},  
    {0x0990, 0xFF3C, WORD_LEN, 0},  
    {0x098c, 0x2314, WORD_LEN, 0},  
    {0x0990, 0xFE1A, WORD_LEN, 0},  
    {0x098c, 0x2316, WORD_LEN, 0},  
    {0x0990, 0x039E, WORD_LEN, 0},  
    {0x098c, 0x2318, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0020, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x231a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x003c, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x231C, WORD_LEN, 0},  
    {0x0990, 0xFF08, WORD_LEN, 0},  
    {0x098c, 0x231E, WORD_LEN, 0},  
    {0x0990, 0x0015, WORD_LEN, 0},  
    {0x098c, 0x2320, WORD_LEN, 0},  
    {0x0990, 0x0040, WORD_LEN, 0},  
    {0x098c, 0x2322, WORD_LEN, 0},  
    {0x0990, 0x001E, WORD_LEN, 0},  
    {0x098c, 0x2324, WORD_LEN, 0},  
    {0x0990, 0xFFCB, WORD_LEN, 0},  
    {0x098c, 0x2326, WORD_LEN, 0},  
    {0x0990, 0xFFF7, WORD_LEN, 0},  
    {0x098c, 0x2328, WORD_LEN, 0},  
    {0x0990, 0x010A, WORD_LEN, 0},  
    {0x098c, 0x232A, WORD_LEN, 0},  
    {0x0990, 0x0015, WORD_LEN, 0},  
    {0x098c, 0x232C, WORD_LEN, 0},  
    {0x0990, 0x001F, WORD_LEN, 0},  
    {0x098c, 0x232e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2330, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffe4, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa348, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0008, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa349, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0002, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0059, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34b, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00e6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa351, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa352, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x007f, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa355, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0001, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35d, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0078, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0086, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x007e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa360, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0082, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2361, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa363, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00d6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa364, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00e2, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa302, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa303, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00ef, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00c7, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa350, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2361, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa366, WORD_LEN, 0},	// 
    {0x0990, 0x007C, WORD_LEN, 0},	// awb_kr_l
    {0x098c, 0xa367, WORD_LEN, 0},	// 
    {0x0990, 0x0082, WORD_LEN, 0},	//
    {0x098c, 0xa368, WORD_LEN, 0},	// 
    {0x0990, 0x0082, WORD_LEN, 0},	// awb_kb_l
    {0x098c, 0xa369, WORD_LEN, 0},	//
    {0x0990, 0x007A, WORD_LEN, 0},	// awb_kr_r
    {0x098c, 0xa36a, WORD_LEN, 0},
    {0x0990, 0x0082, WORD_LEN, 0},
    {0x098c, 0xa36b, WORD_LEN, 0},  //
    {0x0990, 0x007A, WORD_LEN, 0},  //awb_kb_r
    {0x098c, 0xa365, WORD_LEN, 0},  //
    {0x0990, 0x0040, WORD_LEN, 0},  //awb_x0
    {0x098c, 0xa34d, WORD_LEN, 0},
    {0x0990, 0x00a6, WORD_LEN, 0},
    {0x098C, 0xA363, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_TG_MIN0]
    {0x0990, 0x00DA, WORD_LEN, 0}, 	//E0 //D6 // MCU_DATA_0
    {0x098C, 0xA364, WORD_LEN, 0}, 	// MCU_ADDRESS [AWB_TG_MAX0]
    {0x0990, 0x00EC, WORD_LEN, 0}, 	//EC // MCU_DATA_0
 
//gamma
    {0x098c, 0xab37, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0003, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2b38, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x1000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2b3a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x4000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab3c, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_0 */
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab3d, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_1 */
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab3e, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_2 */
    {0x0990, 0x0012, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab3f, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_3 */
    {0x0990, 0x002c, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab40, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_4 */
    {0x0990, 0x004d, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab41, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_5 */
    {0x0990, 0x0065, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab42, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_6 */
    {0x0990, 0x0079, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab43, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_7 */
    {0x0990, 0x008c, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab44, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_8 */
    {0x0990, 0x009b, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab45, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_9 */
    {0x0990, 0x00a9, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab46, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_10 */
    {0x0990, 0x00b6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab47, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_11 */
    {0x0990, 0x00c2, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab48, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_12 */
    {0x0990, 0x00cc, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab49, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_13 */
    {0x0990, 0x00d6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4a, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_14 */
    {0x0990, 0x00df, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4b, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_15 */
    {0x0990, 0x00e8, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4c, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_16 */
    {0x0990, 0x00f0, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4d, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_17 */
    {0x0990, 0x00f8, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4e, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_A_18 */
    {0x0990, 0x00ff, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4f, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_0 */
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab50, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_1 */
    {0x0990, 0x0032, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab51, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_2 */
    {0x0990, 0x0044, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab52, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_3 */
    {0x0990, 0x005b, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab53, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_4 */
    {0x0990, 0x0079, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab54, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_5 */
    {0x0990, 0x008f, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab55, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_6 */
    {0x0990, 0x00a0, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab56, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_7 */
    {0x0990, 0x00ad, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab57, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_8 */
    {0x0990, 0x00b9, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab58, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_9 */
    {0x0990, 0x00c3, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab59, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_10 */
    {0x0990, 0x00cc, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5a, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_11 */
    {0x0990, 0x00d4, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5b, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_12 */
    {0x0990, 0x00dc, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5c, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_13 */
    {0x0990, 0x00e3, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5d, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_14 */
    {0x0990, 0x00e9, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5e, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_15 */
    {0x0990, 0x00ef, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5f, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_16 */
    {0x0990, 0x00f5, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab60, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_17 */
    {0x0990, 0x00fa, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab61, WORD_LEN, 0},	// MCU_ADDRESS /* HG_GAMMA_TABLE_B_18 */
    {0x0990, 0x00ff, WORD_LEN, 0},	// MCU_DATA_0   

//drt_off
    {0x098c, 0xa244, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0008, WORD_LEN, 0}, 	// mcu_data_0
//base_target
    {0x098c, 0xa24f, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0048, WORD_LEN, 0}, 	// mcu_data_0
//offset
    { 0x327A, 0x002a, WORD_LEN, 0},	//2C 	// BLACK_LEVEL_1ST_RED
    { 0x3280, 0x002b, WORD_LEN, 0},	//2E 	// BLACK_LEVEL_1ST_BLUE
    {0x098c, 0xab04, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0008, WORD_LEN, 0},	// mcu_data_0
//low_light
    {0x098c, 0xa34e, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x00c7, WORD_LEN, 0}, 	// mcu_data_0
//ll_nr
    {0x098c, 0xab1f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00c6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab20, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0063, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab21, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x001f, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab22, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0003, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab23, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0005, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab24, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0033, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab25, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0040, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab26, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab27, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2b28, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0600, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2b2a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x1000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab2c, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab2d, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x000a, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab2e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab2f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab30, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab31, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x000e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab32, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab33, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab34, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0008, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab35, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0    

//ae_settings
    {0x098c, 0xa11d, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa208, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0000, WORD_LEN, 0},//3 	// mcu_data_0
    {0x098c, 0xa209, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa20a, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x001f, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa216, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x003a, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa207, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x000a, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa20d, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0020, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa20e, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0080, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0x2212, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0200, WORD_LEN, 0},    //0b9 // mcu_data_0
    {0x098c, 0xa129, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},	// mcu_data_0  //recommand_settings
    {0x322a, 0x0004, WORD_LEN, 0},	// decimator_control
    {0x098c, 0xab1f, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x00c7, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xab31, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x001e, WORD_LEN, 0},	// mcu_data_0    

//refresh
    {0x098c, 0xa103, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0006, WORD_LEN, 0}, 	// mcu_data_0
//poll_field=seq_cmd, !=0, delay=10, timeout=100  // wait for command to be processed	//delay=300
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0},
    {0x098c, 0xa103, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0005, WORD_LEN, 0}, 	// mcu_data_0
//poll_field=seq_cmd, !=0, delay=10, timeout=100  // wait for command to be processed	//delay=300
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0}, 

//full_size
#if 0  //hhs test
    {0x098c, 0xa115, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0072, WORD_LEN, 0}, 	// mcu_data_0
    {0x098c, 0xa103, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0}, 	// mcu_data_0
//poll_field=seq_cmd, !=0, delay=10, timeout=100  // wait for command to be processed	//delay=300
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0}, 
#endif
#endif
#else
    //power_up & reset
	{0x0018, 0x4028, WORD_LEN, 0}, 	// standby_control
	//poll_reg=0x0018, 0x4000, !=0, delay=100, timeout=100	//delay=150      
	{0x0018, 0x4000, POLL_REG, 0},
	{0x0000, 0x6464, POLL_REG, 0},
    {0x301a, 0x0004, POLL_REG, 0},
	{0x0004, 0x3214, POLL_REG, 0},
	
	{0x001a, 0x0013, WORD_LEN, 10}, 	// reset_and_misc_control
    {0x001a, 0x0010, WORD_LEN, 10}, 	// reset_and_misc_control
    {0x0018, 0x4028, WORD_LEN, 0}, 	// standby_control
    //poll_reg=0x0018, 0x4000, !=0, delay=100, timeout=100	//delay=150
    {0x0018, 0x4000, POLL_REG, 0},
	{0x0000, 0x6464, POLL_REG, 0},
    //poll_reg=0x301a, 0x0004, !=1, delay=50, timeout=20 // delay=100
    {0x301a, 0x0004, POLL_REG, 0},
	{0x0004, 0x3214, POLL_REG, 0},

	{0x001A, 0x0000, WORD_LEN, 0}, // Mipi enable
    {0x3400, 0x0200, WORD_LEN, 0},
    {0x321C, 0x0000, WORD_LEN, 0},

    {0x001E, 0x0777, WORD_LEN, 0}, // PAD_SLEW
    {0x0016, 0x42DF, WORD_LEN, 0}, // CLOCKS_CONTROL [1]
    
    //reduce_io_current
    {0x098c, 0x02f0, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0000, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0x02f2, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0210, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0x02f4, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x001a, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0x2145, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x02f4, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa134, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0001, WORD_LEN, 0},	// mcu_data_0

    {0x31e0, 0x0001, WORD_LEN, 0},	// pix_def_id
    {0x001a, 0x0210, WORD_LEN, 0},	// reset_and_misc_control
    {0x0016, 0x42df, WORD_LEN, 0},	// clocks_control
    {0x001E, 0x0404, WORD_LEN, 0},	// PAD_SLEW
    //timing
    {0x0014, 0x2145, WORD_LEN, 0}, 	// pll_control
    {0x0014, 0x2145, WORD_LEN, 0}, 	// pll_control
    {0x0010, 0x0110, WORD_LEN, 0}, 	// pll_dividers
    {0x0012, 0x0000, WORD_LEN, 0}, 	// pll_p_dividers
    {0x0014, 0x244B, WORD_LEN, 1}, 	// pll_control
                                    //DELAY=1 
    {0x0014, 0x304b, WORD_LEN, 0}, 	// pll_control
    //poll_reg=0x0014, 0x8000, ==0, delay=50, timeout=20   // delay=70
    {0x0014, 0x8000, POLL_REG, 0},
    {0x8000, 0x3214, POLL_REG, 0},
    {0x0014, 0xb04a, WORD_LEN, 0}, // PLL_CONTROL
    {0x098c, 0x2703, WORD_LEN, 0},
    {0x0990, 0x0280, WORD_LEN, 0},
    {0x098c, 0x2705, WORD_LEN, 0},
    {0x0990, 0x01e0, WORD_LEN, 0},
    {0x098c, 0x2707, WORD_LEN, 0},
    {0x0990, 0x0280, WORD_LEN, 0},
    {0x098c, 0x2709, WORD_LEN, 0},
    {0x0990, 0x01e0, WORD_LEN, 0},
    {0x098c, 0x270d, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x270f, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x2711, WORD_LEN, 0},
    {0x0990, 0x01e7, WORD_LEN, 0},
    {0x098c, 0x2713, WORD_LEN, 0},
    {0x0990, 0x0287, WORD_LEN, 0},
    {0x098c, 0x2715, WORD_LEN, 0},
    {0x0990, 0x0001, WORD_LEN, 0},
    {0x098c, 0x2717, WORD_LEN, 0},
	#if 0//AT1_BDVER_G(AT1_WS22)
    {0x0990, 0x0026, WORD_LEN, 0},
    #else
    {0x0990, 0x0025, WORD_LEN, 0},
    #endif
	{0x098c, 0x2719, WORD_LEN, 0},
    {0x0990, 0x001a, WORD_LEN, 0},
    {0x098c, 0x271b, WORD_LEN, 0},
    {0x0990, 0x006b, WORD_LEN, 0},
    {0x098c, 0x271d, WORD_LEN, 0},
    {0x0990, 0x006b, WORD_LEN, 0},
    {0x098c, 0x271f, WORD_LEN, 0},
    {0x0990, 0x039B/*0x0219*/, WORD_LEN, 0},
    {0x098c, 0x2721, WORD_LEN, 0},
    {0x0990, 0x034A/*0x05a8*/, WORD_LEN, 0},
    {0x098c, 0x2723, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x2725, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x2727, WORD_LEN, 0},
    {0x0990, 0x01e7, WORD_LEN, 0},
    {0x098c, 0x2729, WORD_LEN, 0},
    {0x0990, 0x0287, WORD_LEN, 0},
    {0x098c, 0x272b, WORD_LEN, 0},
    {0x0990, 0x0001, WORD_LEN, 0},
    {0x098c, 0x272d, WORD_LEN, 0},
	#if 0//AT1_BDVER_G(AT1_WS22)
    {0x0990, 0x0026, WORD_LEN, 0},
    #else
    {0x0990, 0x0025, WORD_LEN, 0},
	#endif
	{0x098c, 0x272f, WORD_LEN, 0},
    {0x0990, 0x001a, WORD_LEN, 0},
    {0x098c, 0x2731, WORD_LEN, 0},
    {0x0990, 0x006b, WORD_LEN, 0},
    {0x098c, 0x2733, WORD_LEN, 0},
    {0x0990, 0x006b, WORD_LEN, 0},
    {0x098c, 0x2735, WORD_LEN, 0},
    {0x0990, 0x039B/*0x0219*/, WORD_LEN, 0},
    {0x098c, 0x2737, WORD_LEN, 0},
    {0x0990, 0x034A/*0x05a8*/, WORD_LEN, 0},
    {0x098c, 0x2739, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x273b, WORD_LEN, 0},
    {0x0990, 0x027f, WORD_LEN, 0},
    {0x098c, 0x273d, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x273f, WORD_LEN, 0},
    {0x0990, 0x01df, WORD_LEN, 0},
    {0x098c, 0x2747, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x2749, WORD_LEN, 0},
    {0x0990, 0x027f, WORD_LEN, 0},
    {0x098c, 0x274b, WORD_LEN, 0},
    {0x0990, 0x0000, WORD_LEN, 0},
    {0x098c, 0x274d, WORD_LEN, 0},
    {0x0990, 0x01df, WORD_LEN, 0},
    {0x098c, 0x222d, WORD_LEN, 0},
    {0x0990, 0x007A/*0x0047*/, WORD_LEN, 0},
    {0x098c, 0xa408, WORD_LEN, 0},
    {0x0990, 0x001D /*0x0011*/, WORD_LEN, 0},
    {0x098c, 0xa409, WORD_LEN, 0},
    {0x0990, 0x001F/*0x0014*/, WORD_LEN, 0},
    {0x098c, 0xa40a, WORD_LEN, 0},
    {0x0990, 0x0023/*0x0014*/, WORD_LEN, 0},
    {0x098c, 0xa40b, WORD_LEN, 0},
    {0x0990, 0x0025/*0x0017*/, WORD_LEN, 0},
    {0x098c, 0x2411, WORD_LEN, 0},
    {0x0990, 0x007A/*0x0047*/, WORD_LEN, 0},
    {0x098c, 0x2413, WORD_LEN, 0},
    {0x0990, 0x0092/*0x0055*/, WORD_LEN, 0},
    {0x098c, 0x2415, WORD_LEN, 0},
    {0x0990, 0x007A/*0x0047*/, WORD_LEN, 0},
    {0x098c, 0x2417, WORD_LEN, 0},
    {0x0990, 0x0092/*0x0055*/, WORD_LEN, 0},
    {0x098c, 0xa404, WORD_LEN, 0},
    {0x0990, 0x0010, WORD_LEN, 0},
    {0x098c, 0xa40d, WORD_LEN, 0},
    {0x0990, 0x0002, WORD_LEN, 0},
    {0x098c, 0xa40e, WORD_LEN, 0},
    {0x0990, 0x0003, WORD_LEN, 0},
    {0x098c, 0xa410, WORD_LEN, 0},
    {0x0990, 0x000a, WORD_LEN, 0},

//lens_correction
    {0x3658, 0x0110, WORD_LEN, 0},	// P_RD_P0Q0
    {0x365a, 0xbc09, WORD_LEN, 0},	// P_RD_P0Q1
    {0x365c, 0x59f3, WORD_LEN, 0},	// P_RD_P0Q2
    {0x365e, 0x02f0, WORD_LEN, 0},	// P_RD_P0Q3
    {0x3660, 0x9895, WORD_LEN, 0},	// P_RD_P0Q4
    {0x3680, 0x19ac, WORD_LEN, 0},	// P_RD_P1Q0
    {0x3682, 0x83ac, WORD_LEN, 0},	// P_RD_P1Q1
    {0x3684, 0x1dd1, WORD_LEN, 0},	// P_RD_P1Q2
    {0x3686, 0x1b93, WORD_LEN, 0},	// P_RD_P1Q3
    {0x3688, 0xc473, WORD_LEN, 0},	// P_RD_P1Q4
    {0x36a8, 0x20f4, WORD_LEN, 0},	// P_RD_P2Q0
    {0x36aa, 0x97b2, WORD_LEN, 0},	// P_RD_P2Q1
    {0x36ac, 0x9fd6, WORD_LEN, 0},	// P_RD_P2Q2
    {0x36ae, 0xa0d6, WORD_LEN, 0},	// P_RD_P2Q3
    {0x36b0, 0x1379, WORD_LEN, 0},	// P_RD_P2Q4
    {0x36d0, 0x2012, WORD_LEN, 0},	// P_RD_P3Q0
    {0x36d2, 0x1a90, WORD_LEN, 0},	// P_RD_P3Q1
    {0x36d4, 0x25f4, WORD_LEN, 0},	// P_RD_P3Q2
    {0x36d6, 0x8854, WORD_LEN, 0},	// P_RD_P3Q3
    {0x36d8, 0x62d1, WORD_LEN, 0},	// P_RD_P3Q4
    {0x36f8, 0xf555, WORD_LEN, 0},	// P_RD_P4Q0
    {0x36fa, 0x4d75, WORD_LEN, 0},	// P_RD_P4Q1
    {0x36fc, 0x16b8, WORD_LEN, 0},	// P_RD_P4Q2
    {0x36fe, 0x4db9, WORD_LEN, 0},	// P_RD_P4Q3
    {0x3700, 0xd65c, WORD_LEN, 0},	// P_RD_P4Q4
    {0x364e, 0x01b0, WORD_LEN, 0},	// P_GR_P0Q0
    {0x3650, 0x7d06, WORD_LEN, 0},	// P_GR_P0Q1
    {0x3652, 0x5473, WORD_LEN, 0},	// P_GR_P0Q2
    {0x3654, 0x1d30, WORD_LEN, 0},	// P_GR_P0Q3
    {0x3656, 0xadd5, WORD_LEN, 0},	// P_GR_P0Q4
    {0x3676, 0xa44c, WORD_LEN, 0},	// P_GR_P1Q0
    {0x3678, 0x81ad, WORD_LEN, 0},	// P_GR_P1Q1
    {0x367a, 0xcdc6, WORD_LEN, 0},	// P_GR_P1Q2
    {0x367c, 0x0a73, WORD_LEN, 0},	// P_GR_P1Q3
    {0x367e, 0xd972, WORD_LEN, 0},	// P_GR_P1Q4
    {0x369e, 0x1b74, WORD_LEN, 0},	// P_GR_P2Q0
    {0x36a0, 0x8892, WORD_LEN, 0},	// P_GR_P2Q1
    {0x36a2, 0xeb76, WORD_LEN, 0},	// P_GR_P2Q2
    {0x36a4, 0xb336, WORD_LEN, 0},	// P_GR_P2Q3
    {0x36a6, 0x3e79, WORD_LEN, 0},	// P_GR_P2Q4
    {0x36c6, 0x698f, WORD_LEN, 0},	// P_GR_P3Q0
    {0x36c8, 0x8a71, WORD_LEN, 0},	// P_GR_P3Q1
    {0x36ca, 0x46f5, WORD_LEN, 0},	// P_GR_P3Q2
    {0x36cc, 0xb48f, WORD_LEN, 0},	// P_GR_P3Q3
    {0x36ce, 0x2d35, WORD_LEN, 0},	// P_GR_P3Q4
    {0x36ee, 0x8316, WORD_LEN, 0},	// P_GR_P4Q0
    {0x36f0, 0x45b5, WORD_LEN, 0},	// P_GR_P4Q1
    {0x36f2, 0x1259, WORD_LEN, 0},	// P_GR_P4Q2
    {0x36f4, 0x48d9, WORD_LEN, 0},	// P_GR_P4Q3
    {0x36f6, 0x8d3d, WORD_LEN, 0},	// P_GR_P4Q4
    {0x3662, 0x0170, WORD_LEN, 0},	// P_BL_P0Q0
    {0x3664, 0x4ccd, WORD_LEN, 0},	// P_BL_P0Q1
    {0x3666, 0x4d93, WORD_LEN, 0},	// P_BL_P0Q2
    {0x3668, 0x73af, WORD_LEN, 0},	// P_BL_P0Q3
    {0x366a, 0xcf35, WORD_LEN, 0},	// P_BL_P0Q4
    {0x368a, 0x3c0d, WORD_LEN, 0},	// P_BL_P1Q0
    {0x368c, 0x02ad, WORD_LEN, 0},	// P_BL_P1Q1
    {0x368e, 0xfb6e, WORD_LEN, 0},	// P_BL_P1Q2
    {0x3690, 0x08d3, WORD_LEN, 0},	// P_BL_P1Q3
    {0x3692, 0x9572, WORD_LEN, 0},	// P_BL_P1Q4
    {0x36b2, 0x0d94, WORD_LEN, 0},	// P_BL_P2Q0
    {0x36b4, 0xe931, WORD_LEN, 0},	// P_BL_P2Q1
    {0x36b6, 0x99b7, WORD_LEN, 0},	// P_BL_P2Q2
    {0x36b8, 0xb8b6, WORD_LEN, 0},	// P_BL_P2Q3
    {0x36ba, 0x027a, WORD_LEN, 0},	// P_BL_P2Q4
    {0x36da, 0x4272, WORD_LEN, 0},	// P_BL_P3Q0
    {0x36dc, 0xe331, WORD_LEN, 0},	// P_BL_P3Q1
    {0x36de, 0x1195, WORD_LEN, 0},	// P_BL_P3Q2
    {0x36e0, 0x4635, WORD_LEN, 0},	// P_BL_P3Q3
    {0x36e2, 0x9536, WORD_LEN, 0},	// P_BL_P3Q4
    {0x3702, 0x8456, WORD_LEN, 0},	// P_BL_P4Q0
    {0x3704, 0x4155, WORD_LEN, 0},	// P_BL_P4Q1
    {0x3706, 0x4499, WORD_LEN, 0},	// P_BL_P4Q2
    {0x3708, 0x61f9, WORD_LEN, 0},	// P_BL_P4Q3
    {0x370a, 0x845d, WORD_LEN, 0},	// P_BL_P4Q4
    {0x366c, 0x00b0, WORD_LEN, 0},	// P_GB_P0Q0
    {0x366e, 0x86ad, WORD_LEN, 0},	// P_GB_P0Q1
    {0x3670, 0x5333, WORD_LEN, 0},	// P_GB_P0Q2
    {0x3672, 0x09f0, WORD_LEN, 0},	// P_GB_P0Q3
    {0x3674, 0xb115, WORD_LEN, 0},	// P_GB_P0Q4
    {0x3694, 0xffea, WORD_LEN, 0},	// P_GB_P1Q0
    {0x3696, 0xa2ec, WORD_LEN, 0},	// P_GB_P1Q1
    {0x3698, 0x458d, WORD_LEN, 0},	// P_GB_P1Q2
    {0x369a, 0x5212, WORD_LEN, 0},	// P_GB_P1Q3
    {0x369c, 0xc491, WORD_LEN, 0},	// P_GB_P1Q4
    {0x36bc, 0x1ab4, WORD_LEN, 0},	// P_GB_P2Q0
    {0x36be, 0x9712, WORD_LEN, 0},	// P_GB_P2Q1
    {0x36c0, 0x80f7, WORD_LEN, 0},	// P_GB_P2Q2
    {0x36c2, 0xb3d6, WORD_LEN, 0},	// P_GB_P2Q3
    {0x36c4, 0x69f9, WORD_LEN, 0},	// P_GB_P2Q4
    {0x36e4, 0x74ce, WORD_LEN, 0},	// P_GB_P3Q0
    {0x36e6, 0x8272, WORD_LEN, 0},	// P_GB_P3Q1
    {0x36e8, 0x46f5, WORD_LEN, 0},	// P_GB_P3Q2
    {0x36ea, 0x7f95, WORD_LEN, 0},	// P_GB_P3Q3
    {0x36ec, 0xa176, WORD_LEN, 0},	// P_GB_P3Q4
    {0x370c, 0xf575, WORD_LEN, 0},	// P_GB_P4Q0
    {0x370e, 0x2815, WORD_LEN, 0},	// P_GB_P4Q1
    {0x3710, 0x5759, WORD_LEN, 0},	// P_GB_P4Q2
    {0x3712, 0x167a, WORD_LEN, 0},	// P_GB_P4Q3
    {0x3714, 0xc6dd, WORD_LEN, 0},	// P_GB_P4Q4
    {0x3644, 0x0134, WORD_LEN, 0},	// POLY_ORIGIN_C
    {0x3642, 0x00e8, WORD_LEN, 0},	// POLY_ORIGIN_R
    {0x3210, 0x09b8, WORD_LEN, 0},	// COLOR_PIPELINE_CONTROL
//awb_and_ccm
    {0x098c, 0x2306, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0303, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2308, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xfe7d, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x230a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffe7, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x230c, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffd0, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x230e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0203, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2310, WORD_LEN, 0},  // MCU_ADDRESS
    {0x0990, 0xff93, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2312, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffb9, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2314, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xfdcf, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2316, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0301, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2318, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0020, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x231a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x003c, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x231c, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xfe07, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x231e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00be, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2320, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x004d, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2322, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffe2, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2324, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffcb, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2326, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffe8, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2328, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00a4, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x232a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xff55, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x232c, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x01a2, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x232e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2330, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0xffe4, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa348, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0008, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa349, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0002, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0059, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34b, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00e6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa351, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa352, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x007f, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa355, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0001, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35d, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0078, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0086, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa35f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x007e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa360, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0082, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2361, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa363, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00d6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa364, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00e2, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa302, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa303, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00ef, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00c7, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa34f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa350, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2361, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa366, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x007a, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa367, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x008b, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xa369, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0078, WORD_LEN, 0},	// MCU_DATA_0     
    {0x098c, 0xa36a, WORD_LEN, 0},
    {0x0990, 0x0080, WORD_LEN, 0},          
    {0x098c, 0xa36b, WORD_LEN, 0},         
    {0x0990, 0x0085, WORD_LEN, 0},          
    {0x098c, 0xa365, WORD_LEN, 0},          
    {0x0990, 0x0000, WORD_LEN, 0}, 
     //gamma
    {0x098c, 0xab37, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0003, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2b38, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x1000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2b3a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x4000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab3c, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_0]
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab3d, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_1]
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab3e, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_2]
    {0x0990, 0x0012, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab3f, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_3]
    {0x0990, 0x002c, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab40, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_4]
    {0x0990, 0x004d, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab41, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_5]
    {0x0990, 0x0065, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab42, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_6]
    {0x0990, 0x0079, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab43, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_7]
    {0x0990, 0x008c, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab44, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_8]
    {0x0990, 0x009b, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab45, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_9]
    {0x0990, 0x00a9, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab46, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_10]
    {0x0990, 0x00b6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab47, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_11]
    {0x0990, 0x00c2, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab48, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_12]
    {0x0990, 0x00cc, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab49, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_13]
    {0x0990, 0x00d6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4a, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_14]
    {0x0990, 0x00df, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4b, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_15]
    {0x0990, 0x00e8, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4c, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_16]
    {0x0990, 0x00f0, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4d, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_17]
    {0x0990, 0x00f8, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4e, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_A_18]
    {0x0990, 0x00ff, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab4f, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_0]
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab50, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_1]
    {0x0990, 0x0032, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab51, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_2]
    {0x0990, 0x0044, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab52, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_3]
    {0x0990, 0x005b, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab53, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_4]
    {0x0990, 0x0079, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab54, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_5]
    {0x0990, 0x008f, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab55, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_6]
    {0x0990, 0x00a0, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab56, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_7]
    {0x0990, 0x00ad, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab57, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_8]
    {0x0990, 0x00b9, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab58, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_9]
    {0x0990, 0x00c3, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab59, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_10]
    {0x0990, 0x00cc, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5a, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_11]
    {0x0990, 0x00d4, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5b, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_12]
    {0x0990, 0x00dc, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5c, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_13]
    {0x0990, 0x00e3, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5d, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_14]
    {0x0990, 0x00e9, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5e, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_15]
    {0x0990, 0x00ef, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab5f, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_16]
    {0x0990, 0x00f5, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab60, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_17]
    {0x0990, 0x00fa, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab61, WORD_LEN, 0},	// MCU_ADDRESS [HG_GAMMA_TABLE_B_18]
    {0x0990, 0x00ff, WORD_LEN, 0},	// MCU_DATA_0   
//drt_off
    {0x098c, 0xa244, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0008, WORD_LEN, 0}, 	// mcu_data_0
//base_target
    {0x098c, 0xa24f, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x003d, WORD_LEN, 0}, 	// mcu_data_0
//offset
    {0x327A, 0x002a, WORD_LEN, 0},  // BLACK_LEVEL_1ST_RED
    {0x3280, 0x002b, WORD_LEN, 0},  // BLACK_LEVEL_1ST_BLUE
    {0x098c, 0xab04, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0008, WORD_LEN, 0}, 	// mcu_data_0
//low_light
    {0x098c, 0xa34e, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x00c7, WORD_LEN, 0}, 	// mcu_data_0
//ll_nr
    {0x098c, 0xab1f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00c6, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab20, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0063, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab21, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x001f, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab22, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0003, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab23, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0005, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab24, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab25, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x00c0, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab26, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab27, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2b28, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0600, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0x2b2a, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x1000, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab2c, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab2d, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x000a, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab2e, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab2f, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0006, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab30, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab31, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x000e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab32, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab33, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x001e, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab34, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0008, WORD_LEN, 0},	// MCU_DATA_0
    {0x098c, 0xab35, WORD_LEN, 0},	// MCU_ADDRESS
    {0x0990, 0x0080, WORD_LEN, 0},	// MCU_DATA_0    

//ae_settings
    {0x098c, 0xa11d, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa208, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0000, WORD_LEN, 0},  // mcu_data_0
    {0x098c, 0xa209, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa20a, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x001f, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa216, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x003a, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa207, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x000a, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa20d, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0020, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xa20e, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0080, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0x2212, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0200, WORD_LEN, 0},   //0b9 // mcu_data_0
    {0x098c, 0xa129, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0},	// mcu_data_0  //recommand_settings
    {0x322a, 0x0004, WORD_LEN, 0},	// decimator_control
    {0x098c, 0xab1f, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x00c7, WORD_LEN, 0},	// mcu_data_0
    {0x098c, 0xab31, WORD_LEN, 0},	// mcu_address
    {0x0990, 0x001e, WORD_LEN, 0},	// mcu_data_0    

//refresh
    {0x098c, 0xa103, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0006, WORD_LEN, 0}, 	// mcu_data_0
//poll_field=seq_cmd, !=0, delay=10, timeout=100  // wait for command to be processed	//delay=300
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0},
    {0x098c, 0xa103, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0005, WORD_LEN, 0}, 	// mcu_data_0
//poll_field=seq_cmd, !=0, delay=10, timeout=100  // wait for command to be processed	//delay=300
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0}, 

//full_size
    {0x098c, 0xa115, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0072, WORD_LEN, 0}, 	// mcu_data_0
    {0x098c, 0xa103, WORD_LEN, 0}, 	// mcu_address
    {0x0990, 0x0002, WORD_LEN, 0}, 	// mcu_data_0
//poll_field=seq_cmd, !=0, delay=10, timeout=100  // wait for command to be processed	//delay=300
    {0xa103, 0xffff, POLL_MCU_VAR, 0},
    {0x0000, 0x0a64, POLL_MCU_VAR, 0}
#endif
};

static const struct mt9v113_i2c_reg_conf const mt9v113_preview_cfg[] = {	
	{0x098C,0xA115, WORD_LEN, 0},  // MCU_ADDRESS /* SEQ_CAP_MODE */
	{0x0990,0x0000, WORD_LEN, 0},  // MCU_DATA_0
	{0x098C,0xA103, WORD_LEN, 0},  // MCU_ADDRESS /* SEQ_CMD */
	{0x0990,0x0001, WORD_LEN, 0}   // MCU_DATA_0
};


static const struct mt9v113_i2c_reg_conf const mt9v113_snapshot_cfg[] = {	
	{0x098C, 0xA115, WORD_LEN, 0}, 	// MCU_ADDRESS /* SEQ_CAP_MODE */
	{0x0990, 0x0002, WORD_LEN, 0},	// MCU_DATA_0
	{0x098C, 0xA103, WORD_LEN, 0},  // MCU_ADDRESS /* SEQ_CMD */
	{0x0990, 0x0002, WORD_LEN, 0}   // MCU_DATA_0
};


static const struct mt9v113_i2c_reg_conf mt9v113_preview_fps_settings_cfg[][MT9V113_CFG_PREVIEW_FPS_MAX_PARAMS]={
	#if 1
	{		/* MT9V113_FFPS_5 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x07C0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x07C0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x000f, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_6 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x06C0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x06C0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x000f, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_7 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x05C0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x05C0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x000f, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_8 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x0534, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x0534, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x000f, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_9 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x0480, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x0480, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x000d, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_10 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x042C, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x042C, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x000c, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_11 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x03B0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x03B0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x000A, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_12 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x0370, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x0370, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0009, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_13 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x0320, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x0320, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0008, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_14 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02F0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02F0, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_15 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_16 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_17 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_18 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_19 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_20 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_21 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_22 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_23 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_24 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_25 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_26 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_27 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_28 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_29 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	{		/* MT9V113_FFPS_30 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0x2735, WORD_LEN, 0}, 	 	
		{0x0990, 0x02B8, WORD_LEN, 0}, 		//      = 1824
		{0x098C, 0xA20C, WORD_LEN, 0}, 	 	
		{0x0990, 0x0007, WORD_LEN, 0}, 	 	
		{0x098c, 0xa103, WORD_LEN, 0}, 	
		{0x0990, 0x0006, WORD_LEN, 0}, 	 
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
		{0x0000, 0x0a64, POLL_MCU_VAR, 0},
	},
	#else
	{		/* MT9V113_FFPS_5 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0B27, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0B27, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_6 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0980, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0980, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_7 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0820, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0820, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_8 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0700, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0700, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_9 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0640, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0640, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_10 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0588, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0588, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_11 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0500, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0500, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_12 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x04A0, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x04A0, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_13 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0440, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0440, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_14 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0400, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0400, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_15 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x03B0, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x03B0, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_16 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0380, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0380, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_17 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0340, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0340, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_18 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0318, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0318, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_19 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x02F0, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x02F0, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_20 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x02C8, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x02C8, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_21 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x02A4, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x02A4, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_22 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0288, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0288, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_23 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0269, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0269, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_24 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0253, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0253, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_25 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x023B, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x023B, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_26 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0226, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0226, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_27 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0210, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0210, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_28 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0210, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0210, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_29 */
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0210, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0210, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
        /* MT9V113_FFPS_30 */  //27frame setting
		{0x098C, 0x271F, WORD_LEN, 0}, 	
		{0x0990, 0x0210, WORD_LEN, 0}, 		//0270
		{0x098C, 0x2735, WORD_LEN, 0}, 	
		{0x0990, 0x0210, WORD_LEN, 0}, 	
		{0x098c, 0x222d, WORD_LEN, 0}, 	
		{0x0990, 0x007A, WORD_LEN, 0}, 	
		{0x098c, 0xa408, WORD_LEN, 0},  //Flicker1_50
		{0x0990, 0x001D, WORD_LEN, 0}, //11
		{0x098c, 0xa409, WORD_LEN, 0}, //Flicker2_50
		{0x0990, 0x001F, WORD_LEN, 0}, //14
		{0x098c, 0xa40a, WORD_LEN, 0}, //Flicker1_60
		{0x0990, 0x0023, WORD_LEN, 0}, //14
		{0x098c, 0xa40b, WORD_LEN, 0}, //Flicker2_60
		{0x0990, 0x0025, WORD_LEN, 0}, //17
		{0x098c, 0x2411, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2413, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098c, 0x2415, WORD_LEN, 0},
		{0x0990, 0x007A, WORD_LEN, 0},
		{0x098c, 0x2417, WORD_LEN, 0},
		{0x0990, 0x0092, WORD_LEN, 0},
		{0x098C, 0xA20C, WORD_LEN, 0},	
		{0x0990, 0x0001, WORD_LEN, 0},	 //0F	
		{0x098c, 0xa103, WORD_LEN, 0},	
		{0x0990, 0x0006, WORD_LEN, 0},
        //poll_field=seq_cmd, !=0, delay=10, timeout=100
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	}	
	#endif
};

//MT9V113 EFFECT
//================================================================
static const struct mt9v113_i2c_reg_conf const mt9v113_effect_settings_cfg[][MT9V113_CFG_EFFECT_MAX_PARAMS] = {
	{   // NONE
    	{ 0x098c, 0x2759, WORD_LEN, 0},
    	{ 0x0990, 0x6440, WORD_LEN, 0},
    	{ 0x098c, 0x275b, WORD_LEN, 0},
    	{ 0x0990, 0x6440, WORD_LEN, 0},
     	{ 0x098c, 0xa103, WORD_LEN, 0},
    	{ 0x0990, 0x0005, WORD_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0xa103, 0xffff, POLL_MCU_VAR, 0},
    	{ 0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   // MONO
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    },
    {   // NEGATIVE
    	{ 0x098c, 0x2759, WORD_LEN, 0},
    	{ 0x0990, 0x6443, WORD_LEN, 0},
    	{ 0x098c, 0x275b, WORD_LEN, 0},
    	{ 0x0990, 0x6443, WORD_LEN, 0},
    	{ 0x098c, 0xa103, WORD_LEN, 0},
    	{ 0x0990, 0x0005, WORD_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
    	{ 0, 0, ZERO_LEN, 0},
        { 0xa103, 0xffff, POLL_MCU_VAR, 0},
        { 0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   // SOLARIZE
        { 0x098c, 0x2759, WORD_LEN, 0},
        { 0x0990, 0x4d44, WORD_LEN, 0},
        { 0x098c, 0x275b, WORD_LEN, 0},
        { 0x0990, 0x4d44, WORD_LEN, 0},
        { 0x098c, 0xa103, WORD_LEN, 0},
        { 0x0990, 0x0005, WORD_LEN, 0},
        { 0, 0, ZERO_LEN, 0},
        { 0, 0, ZERO_LEN, 0},
        { 0xa103, 0xffff, POLL_MCU_VAR, 0},
        { 0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   // SEPIA
    	{ 0x098c, 0x2759, WORD_LEN, 0},
    	{ 0x0990, 0x6442, WORD_LEN, 0},
    	{ 0x098c, 0x275b, WORD_LEN, 0},
    	{ 0x0990, 0x6442, WORD_LEN, 0},
    	{ 0x098c, 0x2763, WORD_LEN, 0},
    	{ 0x0990, 0xb023, WORD_LEN, 0},
    	{ 0x098c, 0xa103, WORD_LEN, 0},
    	{ 0x0990, 0x0005, WORD_LEN, 0},
    	{ 0xa103, 0xffff, POLL_MCU_VAR, 0},
    	{ 0x0000, 0x0a64, POLL_MCU_VAR, 0}
    }
};

//MT9V113 WHITE BALANCE
//================================================================
static const struct mt9v113_i2c_reg_conf const mt9v113_wb_settings_cfg[][MT9V113_CFG_WB_MAX_PARAMS] = { 
    {   // AUTO
        {0x098c, 0xa34a, WORD_LEN, 0},
        {0x0990, 0x0059, WORD_LEN, 0},
        {0x098c, 0xa34b, WORD_LEN, 0},
        {0x0990, 0x00e6, WORD_LEN, 0},
        {0x098c, 0xa34c, WORD_LEN, 0},
        {0x0990, 0x0059, WORD_LEN, 0},
        {0x098c, 0xa34d, WORD_LEN, 0},
        {0x0990, 0x00a6, WORD_LEN, 0},
        {0x098c, 0xa351, WORD_LEN, 0},
        {0x0990, 0x0000, WORD_LEN, 0},
        {0x098c, 0xa352, WORD_LEN, 0},
        {0x0990, 0x007f, WORD_LEN, 0}
    },
    {   // AUTO
        {0x098c, 0xa34a, WORD_LEN, 0},
        {0x0990, 0x0059, WORD_LEN, 0},
        {0x098c, 0xa34b, WORD_LEN, 0},
        {0x0990, 0x00e6, WORD_LEN, 0},
        {0x098c, 0xa34c, WORD_LEN, 0},
        {0x0990, 0x0059, WORD_LEN, 0},
        {0x098c, 0xa34d, WORD_LEN, 0},
        {0x0990, 0x00a6, WORD_LEN, 0},
        {0x098c, 0xa351, WORD_LEN, 0},
        {0x0990, 0x0000, WORD_LEN, 0},
        {0x098c, 0xa352, WORD_LEN, 0},
        {0x0990, 0x007f, WORD_LEN, 0}
    },
    {   // INCANDESCENT (A)
        {0x098c, 0xa34a, WORD_LEN, 0},
        {0x0990, 0x007b, WORD_LEN, 0},
        {0x098c, 0xa34b, WORD_LEN, 0},
        {0x0990, 0x0090, WORD_LEN, 0},
        {0x098c, 0xa34c, WORD_LEN, 0},
        {0x0990, 0x007b, WORD_LEN, 0},
        {0x098c, 0xa34d, WORD_LEN, 0},
        {0x0990, 0x008b, WORD_LEN, 0},
        {0x098c, 0xa351, WORD_LEN, 0},
        {0x0990, 0x0000, WORD_LEN, 0},
        {0x098c, 0xa352, WORD_LEN, 0},
        {0x0990, 0x000f, WORD_LEN, 0}
    },
    {   // FLUORESCENT (TL84)
        {0x098c, 0xa34a, WORD_LEN, 0},
        {0x0990, 0x00af, WORD_LEN, 0},
        {0x098c, 0xa34b, WORD_LEN, 0},
        {0x0990, 0x00b8, WORD_LEN, 0},
        {0x098c, 0xa34c, WORD_LEN, 0},
        {0x0990, 0x006a, WORD_LEN, 0},
        {0x098c, 0xa34d, WORD_LEN, 0},
        {0x0990, 0x007a, WORD_LEN, 0},
        {0x098c, 0xa351, WORD_LEN, 0},
        {0x0990, 0x0030, WORD_LEN, 0},
        {0x098c, 0xa352, WORD_LEN, 0},
        {0x0990, 0x0040, WORD_LEN, 0}
    },
    {   // DAYLIGHT (D65)
        {0x098c, 0xa34a, WORD_LEN, 0},
        {0x0990, 0x00c6, WORD_LEN, 0},
        {0x098c, 0xa34b, WORD_LEN, 0},
        {0x0990, 0x00e0, WORD_LEN, 0},
        {0x098c, 0xa34c, WORD_LEN, 0},
        {0x0990, 0x0070, WORD_LEN, 0},
        {0x098c, 0xa34d, WORD_LEN, 0},
        {0x0990, 0x009b, WORD_LEN, 0},
        {0x098c, 0xa351, WORD_LEN, 0},
        {0x0990, 0x0060, WORD_LEN, 0},
        {0x098c, 0xa352, WORD_LEN, 0},
        {0x0990, 0x007f, WORD_LEN, 0}
    },
    {   // CLOUDY (D75)
        {0x098c, 0xa34a, WORD_LEN, 0},
        {0x0990, 0x00f4, WORD_LEN, 0},
        {0x098c, 0xa34b, WORD_LEN, 0},
        {0x0990, 0x00ff, WORD_LEN, 0},
        {0x098c, 0xa34c, WORD_LEN, 0},
        {0x0990, 0x006e, WORD_LEN, 0},
        {0x098c, 0xa34d, WORD_LEN, 0},
        {0x0990, 0x007b, WORD_LEN, 0},
        {0x098c, 0xa351, WORD_LEN, 0},
        {0x0990, 0x007e, WORD_LEN, 0},
        {0x098c, 0xa352, WORD_LEN, 0},
        {0x0990, 0x007f, WORD_LEN, 0}
    }
};


//MT9V113 BRIGHTNESS
//================================================================
static const struct mt9v113_i2c_reg_conf const mt9v113_bright_settings_cfg[][MT9V113_CFG_BRIGHT_MAX_PARAMS] = {
    #if 0  //hhs 20110316 test
    {   // -5
		{0x327a, 0x0050, WORD_LEN, 0},
		{0x327c, 0x0063, WORD_LEN, 0},
		{0x327e, 0x0063, WORD_LEN, 0},
		{0x3280, 0x0064, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0004, WORD_LEN, 0},
		{0x098c, 0xa20a, WORD_LEN, 0},
		{0x0990, 0x0020, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x0013, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    #endif
	#if 1
	//#if 1
	{  //-4
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x0011, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	{  //-3
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x0021, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	{  //-2
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x002C, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	{  //-1
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x0036, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	{  //0
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x0040, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	{  //+1
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0010, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x0054, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	{  //+2
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0020, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x0068, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	{  //+3
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0030, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x007C, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	{  //+4
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0040, WORD_LEN, 0},
		{0x098C, 0xA24F, WORD_LEN, 0}, 	// MCU_ADDRESS /*AE_BASETARGET*/
		{0x0990, 0x009A, WORD_LEN, 0}, 	// MCU_DATA_0
	},
	#if 0
	{   // -4
		{0x327A, 0x004a, WORD_LEN, 0},  //add
        {0x327C, 0x0059, WORD_LEN, 0},  //add 
		{0x327E, 0x0059, WORD_LEN, 0},  //add
        {0x3280, 0x0057, WORD_LEN, 0},  //add 
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0004, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x001c, WORD_LEN, 0},  //add
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		//poll_field=seq_cmd, !=0, delay=10, timeout=50
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a32, POLL_MCU_VAR, 0},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		//poll_field=seq_cmd, !=0, delay=10, timeout=50
		{0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a32, POLL_MCU_VAR, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -3
    	{0x327A, 0x003e, WORD_LEN, 0},  //add
        {0x327C, 0x0049, WORD_LEN, 0},  //add 
		{0x327E, 0x0049, WORD_LEN, 0},  //add
        {0x3280, 0x0049, WORD_LEN, 0},  //add   
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0003, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x0022, WORD_LEN, 0},  //add
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -2
    	{0x327A, 0x003c, WORD_LEN, 0},  //add
        {0x327C, 0x0043, WORD_LEN, 0},  //add 
		{0x327E, 0x0043, WORD_LEN, 0},  //add
        {0x3280, 0x0043, WORD_LEN, 0},  //add  
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0002, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
	    {0x0990, 0x002a, WORD_LEN, 0},  //add
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -1
    	{0x327A, 0x0034, WORD_LEN, 0},  //add
        {0x327C, 0x0033, WORD_LEN, 0},  //add 
		{0x327E, 0x0033, WORD_LEN, 0},  //add
        {0x3280, 0x0033, WORD_LEN, 0},  //add   
        {0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
	    {0x0990, 0x0034, WORD_LEN, 0},  //add
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    #endif
    #if 0
    {   // -4
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0004, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x001C, WORD_LEN, 0},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -3
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0003, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x0022, WORD_LEN, 0},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -2
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0002, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x002A, WORD_LEN, 0},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -1
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x0034, WORD_LEN, 0},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    #endif
	#if 0
    {   // 0
    	{0x327A, 0x002C, WORD_LEN, 0},  //add
        {0x327C, 0x002A, WORD_LEN, 0},  //add 
		{0x327E, 0x002A, WORD_LEN, 0},  //add
        {0x3280, 0x002A, WORD_LEN, 0},  //add      
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
	    {0x0990, 0x003d, WORD_LEN, 0},  //add
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // +1 
    	{0x327A, 0x002C, WORD_LEN, 0},  //add
        {0x327C, 0x002A, WORD_LEN, 0},  //add 
		{0x327E, 0x002A, WORD_LEN, 0},  //add
        {0x3280, 0x002A, WORD_LEN, 0},  //add   
        {0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0010, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
	    {0x0990, 0x003d, WORD_LEN, 0},  //add
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // +2  
        {0x327A, 0x002C, WORD_LEN, 0},  //add
        {0x327C, 0x002A, WORD_LEN, 0},  //add 
		{0x327E, 0x002A, WORD_LEN, 0},  //add
        {0x3280, 0x002A, WORD_LEN, 0},  //add   
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0020, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x003d, WORD_LEN, 0}, //48
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // +3  
        {0x327A, 0x002C, WORD_LEN, 0},  //add
        {0x327C, 0x002A, WORD_LEN, 0},  //add 
		{0x327E, 0x002A, WORD_LEN, 0},  //add
        {0x3280, 0x002A, WORD_LEN, 0},  //add  
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0030, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x003D, WORD_LEN, 0},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // +4
        {0x327A, 0x002C, WORD_LEN, 0},  //add
        {0x327C, 0x002A, WORD_LEN, 0},  //add 
		{0x327E, 0x002A, WORD_LEN, 0},  //add
        {0x3280, 0x002A, WORD_LEN, 0},  //add  
		{0x098C, 0xA209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098C, 0xA75E, WORD_LEN, 0},
		{0x0990, 0x0040, WORD_LEN, 0},
		{0x098c, 0xA24F, WORD_LEN, 0},
		{0x0990, 0x003D, WORD_LEN, 0},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0006, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
		{0x098c, 0xA103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
		{0, 0, DELAY_T, 10},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    #endif
	#else
    {   // -4
		{0x327a, 0x004a, WORD_LEN, 0},
		{0x327c, 0x0059, WORD_LEN, 0},
		{0x327e, 0x0059, WORD_LEN, 0},
		{0x3280, 0x0057, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0004, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x001c, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -3
		{0x327a, 0x003e, WORD_LEN, 0},
		{0x327c, 0x0049, WORD_LEN, 0},
		{0x327e, 0x0049, WORD_LEN, 0},
		{0x3280, 0x0049, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0003, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x0022, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -2
		{0x327a, 0x003c, WORD_LEN, 0},
		{0x327c, 0x0043, WORD_LEN, 0},
		{0x327e, 0x0043, WORD_LEN, 0},
		{0x3280, 0x0043, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0002, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x002a, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // -1
		{0x327a, 0x0034, WORD_LEN, 0},
		{0x327c, 0x0033, WORD_LEN, 0},
		{0x327e, 0x0033, WORD_LEN, 0},
		{0x3280, 0x0033, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x0034, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // 0
		{0x327a, 0x002c, WORD_LEN, 0},
		{0x327c, 0x002a, WORD_LEN, 0},
		{0x327e, 0x002a, WORD_LEN, 0},
		{0x3280, 0x002a, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0000, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x003d, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // +1
		{0x327a, 0x002c, WORD_LEN, 0},
		{0x327c, 0x002a, WORD_LEN, 0},
		{0x327e, 0x002a, WORD_LEN, 0},
		{0x3280, 0x002a, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0010, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x003d, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // +2
		{0x327a, 0x002c, WORD_LEN, 0},
		{0x327c, 0x002a, WORD_LEN, 0},
		{0x327e, 0x002a, WORD_LEN, 0},
		{0x3280, 0x002a, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0020, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x003d, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // +3
		{0x327a, 0x002c, WORD_LEN, 0},
		{0x327c, 0x002a, WORD_LEN, 0},
		{0x327e, 0x002a, WORD_LEN, 0},
		{0x3280, 0x002a, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0030, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x003d, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    {   // +4
		{0x327a, 0x002c, WORD_LEN, 0},
		{0x327c, 0x002a, WORD_LEN, 0},
		{0x327e, 0x002a, WORD_LEN, 0},
		{0x3280, 0x002a, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0040, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x003d, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    },
    #endif
    #if 0  //hhs 20110316 test
    {   // +5
		{0x327a, 0x002c, WORD_LEN, 0},
		{0x327c, 0x002a, WORD_LEN, 0},
		{0x327e, 0x002a, WORD_LEN, 0},
		{0x3280, 0x002a, WORD_LEN, 0},
		{0x098c, 0xa209, WORD_LEN, 0},
		{0x0990, 0x0001, WORD_LEN, 0},
		{0x098c, 0xa75e, WORD_LEN, 0},
		{0x0990, 0x0040, WORD_LEN, 0},
		{0x098c, 0xa24f, WORD_LEN, 0},
		{0x0990, 0x003d, WORD_LEN, 0},
		{0x098c, 0xa103, WORD_LEN, 0},
		{0x0990, 0x0005, WORD_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0},
        {0, 0, ZERO_LEN, 0}
    }
	#endif
};

static const struct mt9v113_i2c_reg_conf const mt9v113_exposure_settings_cfg[][MT9V113_CFG_EXPOSURE_MAX_PARAMS] = {
    {   // NORMAL
        {0x098c, 0xa202, WORD_LEN, 0},  
        {0x0990, 0x0011, WORD_LEN, 0},  
        {0x098c, 0xa203, WORD_LEN, 0},  
        {0x0990, 0x00dd, WORD_LEN, 0},  
        {0x098c, 0xa103, WORD_LEN, 0},  
        {0x0990, 0x0005, WORD_LEN, 0},  
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   // SPOT
        {0x098c, 0xa202, WORD_LEN, 0},  
        {0x0990, 0x0056, WORD_LEN, 0},  
        {0x098c, 0xa203, WORD_LEN, 0},  
        {0x0990, 0x0053, WORD_LEN, 0},  
        {0x098c, 0xa103, WORD_LEN, 0},  
        {0x0990, 0x0005, WORD_LEN, 0},  
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   // AVERAGE
        {0x098c, 0xa202, WORD_LEN, 0},  
        {0x0990, 0x0011, WORD_LEN, 0},  
        {0x098c, 0xa203, WORD_LEN, 0},  
        {0x0990, 0x00dd, WORD_LEN, 0},  
        {0x098c, 0xa103, WORD_LEN, 0},  
        {0x0990, 0x0005, WORD_LEN, 0},  
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   // SPOT
        {0x098c, 0xa202, WORD_LEN, 0},  
        {0x0990, 0x0056, WORD_LEN, 0},  
        {0x098c, 0xa203, WORD_LEN, 0},  
        {0x0990, 0x0053, WORD_LEN, 0},  
        {0x098c, 0xa103, WORD_LEN, 0},  
        {0x0990, 0x0005, WORD_LEN, 0},  
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    }
};

static const struct mt9v113_i2c_reg_conf const mt9v113_flicker_settings_cfg[][MT9V113_CFG_FLICKER_MAX_PARAMS] = {
	/*  */ /* Anti Flicker mode Auto */
	{
        {0x098C, 0xA404, WORD_LEN, 0},  
        {0x0990, 0x0010, WORD_LEN, 0},  
        {0x098C, 0xA103, WORD_LEN, 0},  
        {0x0990, 0x0006, WORD_LEN, 0},  
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0},
        {0x098C, 0xA103, WORD_LEN, 0},
        {0x0990, 0x0005, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	/* Anti Flicker mode Manual 60Hz */
	{
        {0x098C, 0xA404, WORD_LEN, 0},  
        {0x0990, 0x0090, WORD_LEN, 0},  
        {0x098C, 0xA103, WORD_LEN, 0},  
        {0x0990, 0x0006, WORD_LEN, 0},  
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0},
        {0x098C, 0xA103, WORD_LEN, 0},
        {0x0990, 0x0005, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	},
	{
    /* Anti Flicker mode Manual 50Hz */ 
        {0x098C, 0xA404, WORD_LEN, 0},  
        {0x0990, 0x00C0, WORD_LEN, 0},  
        {0x098C, 0xA103, WORD_LEN, 0},  
        {0x0990, 0x0006, WORD_LEN, 0},  
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0},
        {0x098C, 0xA103, WORD_LEN, 0},
        {0x0990, 0x0005, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
	}
};

#ifdef F_SKYCAM_FIX_CFG_REFLECT
static const struct mt9v113_i2c_reg_conf const mt9v113_reflect_settings_cfg[][MT9V113_CFG_REFLECT_MAX_PARAMS] = {
#if 0
    {    /* MT9V113_REFLECT_NONE */
        {0x098c, 0x2717, WORD_LEN, 0},
		#if 0//AT1_BDVER_G(AT1_WS22)
        {0x0990, 0x0026, WORD_LEN, 0},
        #else
        {0x0990, 0x0025, WORD_LEN, 0},
		#endif
		{0x098c, 0x272d, WORD_LEN, 0},
		#if 0//AT1_BDVER_G(AT1_WS22)
        {0x0990, 0x0026, WORD_LEN, 0},
        #else
        {0x0990, 0x0025, WORD_LEN, 0},
		#endif
        {0x098c, 0xa103, WORD_LEN, 0},
        {0x0990, 0x0006, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   /* MT9V113_REFLECT_MIRROR */
        {0x098c, 0x2717, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
        {0x0990, 0x0027, WORD_LEN, 0},
		#else
        {0x0990, 0x0024, WORD_LEN, 0},
        #endif
		{0x098c, 0x272d, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
        {0x0990, 0x0027, WORD_LEN, 0},
		#else
        {0x0990, 0x0024, WORD_LEN, 0},
        #endif
		{0x098c, 0xa103, WORD_LEN, 0},
        {0x0990, 0x0006, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   /* MT9V113_REFLECT_WATER */
        {0x098c, 0x2717, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
		{0x0990, 0x0024, WORD_LEN, 0},
		#else
		{0x0990, 0x0027, WORD_LEN, 0},
        #endif
		{0x098c, 0x272d, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
		{0x0990, 0x0024, WORD_LEN, 0},
		#else
        {0x0990, 0x0027, WORD_LEN, 0},
        #endif
		{0x098c, 0xa103, WORD_LEN, 0},
        {0x0990, 0x0006, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   /* MT9V113_REFLECT_MIRROR_WATER */
        {0x098c, 0x2717, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
		{0x0990, 0x0025, WORD_LEN, 0},
		#else
        {0x0990, 0x0026, WORD_LEN, 0},
        #endif
        {0x098c, 0x272d, WORD_LEN, 0},
        #if AT1_BDVER_G(AT1_WS22)
		{0x0990, 0x0025, WORD_LEN, 0},
		#else
        {0x0990, 0x0026, WORD_LEN, 0},
        #endif
		{0x098c, 0xa103, WORD_LEN, 0},
        {0x0990, 0x0006, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    }
#else
    {    /* MT9V113_REFLECT_NONE */
        {0x098c, 0x2717, WORD_LEN, 0},
		#if 0//AT1_BDVER_G(AT1_WS22)
        {0x0990, 0x0026, WORD_LEN, 0},
        #else
        {0x0990, 0x0025, WORD_LEN, 0},
        #endif
		{0x098c, 0x272d, WORD_LEN, 0},
		#if 0//AT1_BDVER_G(AT1_WS22)
        {0x0990, 0x0026, WORD_LEN, 0},
        #else
        {0x0990, 0x0025, WORD_LEN, 0},
		#endif
        {0x098c, 0xa103, WORD_LEN, 0},
        {0x0990, 0x0006, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   /* MT9V113_REFLECT_MIRROR */
        {0x098c, 0x2717, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
        {0x0990, 0x0027, WORD_LEN, 0},
		#else
        {0x0990, 0x0024, WORD_LEN, 0},
        #endif
		{0x098c, 0x272d, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
        {0x0990, 0x0027, WORD_LEN, 0},
		#else
        {0x0990, 0x0024, WORD_LEN, 0},
        #endif
		{0x098c, 0xa103, WORD_LEN, 0},
        {0x0990, 0x0006, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   /* MT9V113_REFLECT_WATER */
        {0x098c, 0x2717, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
		{0x0990, 0x0024, WORD_LEN, 0},
		#else
		{0x0990, 0x0027, WORD_LEN, 0},
        #endif
		{0x098c, 0x272d, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
		{0x0990, 0x0024, WORD_LEN, 0},
		#else
        {0x0990, 0x0027, WORD_LEN, 0},
        #endif
		{0x098c, 0xa103, WORD_LEN, 0},
        {0x0990, 0x0006, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    },
    {   /* MT9V113_REFLECT_MIRROR_WATER */
        {0x098c, 0x2717, WORD_LEN, 0},
		#if AT1_BDVER_G(AT1_WS22)
		{0x0990, 0x0026, WORD_LEN, 0},
		#else
        {0x0990, 0x0026, WORD_LEN, 0},
        #endif
        {0x098c, 0x272d, WORD_LEN, 0},
        #if AT1_BDVER_G(AT1_WS22)
		{0x0990, 0x0026, WORD_LEN, 0},
		#else
        {0x0990, 0x0026, WORD_LEN, 0},
        #endif
		{0x098c, 0xa103, WORD_LEN, 0},
        {0x0990, 0x0006, WORD_LEN, 0},
        {0xa103, 0xffff, POLL_MCU_VAR, 0},
        {0x0000, 0x0a64, POLL_MCU_VAR, 0}
    }
#endif
};
#endif

struct mt9v113_reg mt9v113_regs = {
	.init_parm = &mt9v113_init_parm[0],
	.init_parm_size = ARRAY_SIZE(mt9v113_init_parm),
	.preview_cfg_settings = mt9v113_preview_cfg,
	.preview_cfg_settings_size = ARRAY_SIZE(mt9v113_preview_cfg),
	.snapshot_cfg_settings = mt9v113_snapshot_cfg,
	.snapshot_cfg_settings_size = ARRAY_SIZE(mt9v113_snapshot_cfg),

//preview frame rate
	.preview_fps_cfg_settings = &mt9v113_preview_fps_settings_cfg,
	.preview_fps_cfg_settings_size = MT9V113_CFG_PREVIEW_FPS_MAX_PARAMS,	
	
//effect
	.effect_cfg_settings = mt9v113_effect_settings_cfg,
	.effect_cfg_settings_size = MT9V113_CFG_EFFECT_MAX_PARAMS,

//wb
	.wb_cfg_settings = mt9v113_wb_settings_cfg,
	.wb_cfg_settings_size = MT9V113_CFG_WB_MAX_PARAMS,

//brightness	
	.bright_cfg_settings = mt9v113_bright_settings_cfg,
	.bright_cfg_settings_size = MT9V113_CFG_BRIGHT_MAX_PARAMS,

//exposure
	.exposure_cfg_settings = mt9v113_exposure_settings_cfg,
	.exposure_cfg_settings_size = MT9V113_CFG_EXPOSURE_MAX_PARAMS,

//flicker
	.flicker_cfg_settings = mt9v113_flicker_settings_cfg,
	.flicker_cfg_settings_size = MT9V113_CFG_FLICKER_MAX_PARAMS,
#ifdef F_SKYCAM_FIX_CFG_REFLECT
//reflect	
       .reflect_cfg_settings = mt9v113_reflect_settings_cfg,
	.reflect_cfg_settings_size = MT9V113_CFG_REFLECT_MAX_PARAMS,
#endif	
};

