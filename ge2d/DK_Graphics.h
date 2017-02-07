#ifndef DK_GRAPHICS_H
#define DK_GRAPHICS_H
#include "DK_TypeDefine.h"
#include "Osd.h"

#define FirstDC 0
#define SecondDC 1
typedef osd_obj_t * HDC;
struct _BITMAP;
typedef struct _BITMAP BITMAP;
typedef BITMAP* PBITMAP;

#define BMP_TYPE_NORMAL         0x00
#define BMP_TYPE_COLORKEY       0x10

struct _BITMAP
{
    unsigned char   bmType;
    unsigned char   bmBitsPerPixel;
    unsigned char   bmBytesPerPixel;
    unsigned char   bmAlpha;
    unsigned long  bmColorKey;

    unsigned long  bmWidth;
    unsigned long  bmHeight;
    unsigned long  bmPitch;
    unsigned char*  bmBits;

    void*   bmAlphaPixelFormat;
};

typedef unsigned long   THRGB;
//struct hdc{
//	struct osd_obj_t *psd;	/* screen or memory device*/
//	HWND		hwnd;		/* associated window*/
//	ge2d_src_canvas_type flags;		/*screen or memory flags*/
//	int		    ge2d_format;		/* background mode*/
//	long		textalign;	/* text alignment flags*/
//	COLORVAL	bkcolor;	/* text background color*/
//	COLORVAL	textcolor;	/* text color*/
//	BRUSHOBJ *	brush;		/* current brush*/
//	PENOBJ *	pen;		/* current pen*/
//	BITMAPOBJ *	bitmap;		/* current bitmap (mem dc's only)*/
//	int		    drawmode;	/* rop2 drawing mode */
//    POINT 		CurPenPos;
//};
//
int DK_SysInit();
int DK_SysDestory();

//µÃµ½ÆÁÄ»DC
HDC DK_GetDC(int nDC);
HDC  DK_CreateCompatibleDCEx(HDC hdc, int nWidth, int nHeight);
HDC  DK_CreateCompatibleDC(HDC hdc);
HDC  DK_CreateDC(HDC hdc, int bpp, UINT32 pixelformat, int nWidth, int nHeight);
int DK_DeleteDC(HDC hdc);
int DK_DeleteCompatibleDC(HDC hdc);
int DK_FillDCWithBitmap(HDC hdc, int left, int top, int width, int height, BITMAP *pBitmap, char *pColor);
int DK_FillDCWithBitmapPartDirect(HDC hdc, int x, int y, int w, int h, int bw, int bh, const BITMAP* bmp, int xo, int yo, THRGB *pal);
void DK_BitBlt(HDC dstdc, int nXDest, int nYDest, int nWidth, int nHeight, HDC srcdc, int nXSrc, int nYSrc, DWORD dwRop);
void DK_BitBltEx(HDC dsthdc, int dx, int dy, int dw, int dh,
			HDC srcdc1, int sx1, int sy1, int sw1, int sh1,
			HDC srcdc2, int sx2, int sy2, int sw2, int sh2,
			DWORD operation,
			DWORD pixel1,			 //1st constant value specified by some operations
			DWORD pixel2,			 //2nd constant value specified by some operations
			DWORD alpha1,			 //1st constant value specified by some operations
			DWORD alpha2			 //2nd constant value specified by some operations
			);
void DK_BitBltBlend(HDC dstdc, int nXDest, int nYDest, int nWidth, int nHeight, HDC srcdc, int nXSrc, int nYSrc, DWORD dwRop);
void DK_StretchBitBlt(HDC dstdc, int nXDest, int nYDest, int nWDest, int nHDest,
		HDC srcdc, int nXSrc, int nYSrc, int nWSrc, int nHSrc, DWORD dwRop);
void DK_FillRect(HDC hdc, rectangle_t this_rect, THRGB pal);
HDC DK_CreateDCFromBitmap(char *filename);
void DK_InVisable(HDC this_osd,int enable);
void DK_UpdateRegion(HDC this_osd, rectangle_t *this_rect);
int  DK_GetOsdWidth(HDC this_osd);
int  DK_GetOsdHeigh(HDC this_osd);
#endif //end DK_GRAPHICS_H

