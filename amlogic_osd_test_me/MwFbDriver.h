/**************************************************************************
* 版权所有 (C)2001, 深圳市中兴通讯股份有限公司。
*
* 文件名称：MwFbDriver.h
* 文件标识：
* 内容摘要：本文件包含了所有MicroWindows帧缓冲驱动的数据和函数声明
* 其它说明：
* 当前版本：
* 作    者：
* 完成日期：
*
* 修改记录1	：
* 	修改日期：
* 	版 本 号：
* 	修 改 人：
* 	修改内容：
***************************************************************************/

#ifndef		__MWFBDRIVER_H__
#define		__MWFBDRIVER_H__

#ifdef		__cplusplus
extern "C" {
#endif

/**************************************************************************
 *                         头文件引用                                     *
 **************************************************************************/
#include	"nano-X.h"

/**************************************************************************
 *                         常      量                                     *
 **************************************************************************/
/**************************************************************************
 *                         宏  定  义                                     *
 **************************************************************************/
/**************************************************************************
 *                         数 据 类 型                                    *
 **************************************************************************/
typedef		struct
{
	/* 以下字段由图层控制模块修改并维护，驱动只可读取 */
	unsigned char	bVideoState;		/* 当前视频状态，0-关闭、1-打开 */
	unsigned char	bSleepState;		/* 当前休眠状态，0-关闭、1-启动 */
	/* 以下字段由驱动修改并维护，图层控制模块只可读取 */
	unsigned char	bAlphaBuffer;		/* 是否有透明度缓冲区，0-否、1-是 */
	unsigned char	abAlpha[2];			/* 两个图层的默认透明度 */
	unsigned long	auColorKey[2];		/* 两个图层的色键 */
}
T_MwDriverState;

/**************************************************************************
 *                        全局变量声明                                    *
 **************************************************************************/
extern		int		g_nFbDevNum;
/**************************************************************************
 *                        全局函数原型                                    *
 **************************************************************************/
int		MwFbInit(int nScrW, int nScrH, unsigned char bColorMode, void **pMwFb);
void	MwFbUninit(void);

void	MwFbGetScrInfo(int *pnScrW, int *pnScrH, int *pnBpp, void **pMwFb);
void	MwFbGetLineLen(int *pnLineLen);
void	MwFbGetPixType(int *pbColorMode);
void	MwFbGetBufSize(int *pnSize);

int		MwFbEarasePixel(unsigned char bDevice, unsigned char bLayer, MWRECT *ptRect, int nNum);
int		MwFbRefreshPixel(unsigned char bDevice, unsigned char bLayer, MWRECT *ptRect, int nNum);
int		MwFbSetTransparent(unsigned char bDevice, unsigned char bLayer, MWRECT *ptRect, MWTRANSPARENCE *ptTrans);
int		MwFbLayerControl(unsigned char bDevice, unsigned char bLayer,
			unsigned char bAction, unsigned char bArgB, unsigned int uArgU);

int		MwFbDriverControl(unsigned char bAction, unsigned char bOption);
T_MwDriverState		*MwFbGetDeviceConfig(unsigned char bDevice);

#ifdef		__cplusplus
}
#endif

#endif  	/* __MWFBDRIVER_H__ */
