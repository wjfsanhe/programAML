/**********************************************************
* 版权所有 (C)2005, 深圳市中兴通讯股份有限公司
*
* 文件名称： v120.h
* 文件标识：
* 内容摘要：

* 其它说明：
* 当前版本：
* 作    者： 钱静
* 完成日期：
*
* 修改记录1：
**********************************************************/
#ifndef __B120_H__
#define __B120_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nano-X.h"
//#include "serv.h"

//extern	int (*NUL_Read)(MWKEY *kbuf, MWKEYMOD *modifiers, MWSCANCODE *scancode);
//#define EM10_SUCCESS 					0
#define  FBIOPUT_GE2D_BLIT_NOALPHA		 0x4701
#define  FBIOPUT_GE2D_BLIT                       0x46ff
#define  FBIOPUT_GE2D_STRETCHBLIT       0x46fe
#define  FBIOPUT_GE2D_FILLRECTANGLE     0x46fd
#define  FBIOPUT_GE2D_SRCCOLORKEY       0x46fc
#define  FBIOPUT_GE2D_SET_COEF			            0x46fb

#define  FBIOPUT_OSD_SRCCOLORKEY                0x46fb
#define  FBIOPUT_OSD_SRCKEY_ENABLE      0x46fa
#define  FBIOPUT_GE2D_CONFIG			0x46f9
#define  FBIOPUT_OSD_SET_ALPHA          0x46f6


//色彩模式定义：
#define FB_COLORMODE_565				0x01
#define FB_COLORMODE_24				0x02
#define FB_COLORMODE_32				0x03


#define MASKNONE						0xFF0000FF	/* 过滤颜色定义-不过滤任何颜色 */
#define MASKBLACK						0x7f000000	/* 过滤颜色定义-过滤黑色 */
#define MASKMAUVE						0x00F800F8	/* 过滤颜色定义-过滤紫红色 */

#define  aml_swap(x,y,t)     {(t)=(x);(x)=(y);(y)=(t);}

typedef enum
{
                OSD0_OSD0 =0,
                OSD0_OSD1,
                OSD1_OSD1,
                OSD1_OSD0,
                TYPE_INVALID,
}ge2d_src_dst_t;

/*****************需要调整的地方  begin ***********************/
typedef struct {
	unsigned long  addr;
	unsigned int	w;
	unsigned int	h;
}config_planes_t;
typedef  struct{
	int	key_enable;
	int	key_color;
	int 	key_mask;
	int   key_mode;
}src_key_ctrl_t;
typedef    struct {
	int  op_type;
	int  alu_const_color;
	unsigned int src_format;
	unsigned int dst_format ; //add for src&dst all in user space.

	config_planes_t src_planes[4];
	config_planes_t dst_planes[4];
	src_key_ctrl_t  src_key;
       src_key_ctrl_t  src2_key;//src2_key
}config_para_t;


/*****************需要调整的地方  end ***********************/
typedef  struct {
        config_para_t  config;
        void  *pt_osd ;
}ge2d_op_ctl_t;

typedef struct {
     int            x;   /* X coordinate of its top-left point */
     int            y;   /* Y coordinate of its top-left point */
     int            w;   /* width of it */
     int            h;   /* height of it */
}rectangle_t;
typedef  struct {
        unsigned int   color ;
        rectangle_t src1_rect;
        rectangle_t src2_rect;
        rectangle_t     dst_rect;
        int op;
}ge2d_op_para_t ;

/* 函数声明 */
int		MwFbInit(int nScrW, int nScrH, unsigned char bColorMode, void **pMwFb);
void	MwFbUninit(void);
int		MwFbRefreshRegn(MWRECT *ptRect, int nNum);
void	MwFbGetScrInfo(int *pnScrW, int *pnScrH, int *pnBpp, void **pMwFb);
void	MwFbGetLineLen(int *pnLineLen);
void	MwFbGetPixType(int *pbColorMode);
void	MwFbGetBufSize(int *pnSize);
int		MwFbSetTransparent(unsigned char bDevice, unsigned char bLayer, MWRECT *ptRect, MWTRANSPARENCE *ptTrans);
int  MwEnableAntiflicker(void) ;
int  MwDisableAntiflicker(void);

#ifdef __cplusplus
}
#endif

#endif



