/***********************************************************************
 * Copyright (c) 2006 STMicroelectronics Limited.
 *
 * File: stgfb.h
 * Description :  STGFB API
 *
\***********************************************************************/

#ifndef _STGFB_H
#define _STGFB_H

#include "stgxobj.h"


#define STGFB_GetRevision()  "STGFB-REL_1.0.1"

#define STGFB_DRIVER_ID      417
#define STGFB_DRIVER_BASE    (STGFB_DRIVER_ID << 16)


enum
{
    STGFB_ERROR_I2C = STGFB_DRIVER_BASE,
    STGFB_ERROR_INVALID_PARAMETER,
    STGFB_ERROR_FEATURE_NOT_SUPPORTED
};






/* Type of acceleration available --> the same as STS frame buffer*/
#define STGFB_ACCEL             100

#define STGFB_MAX_DEVICE        8
#define STGFB_MAX_MIXER_NUMBER  8


/*
 * non-standard ioctls to control the FB plane and blitter, although
 * these can be used directly they are really provided for the DirectFB
 * driver
 */
#define STGFB_IO_SET_OVERLAY_COLORKEY       _IOW('B', 0x1, int)
#define STGFB_IO_ENABLE_FLICKER_FILTER      _IOW('B', 0x2, int)
#define STGFB_IO_DISABLE_FLICKER_FILTER     _IOW('B', 0x3, int)
#define STGFB_IO_BLIT_COMMAND               _IOW('B', 0x4, STGFB_BLIT_Command_t)
#define STGFB_IO_SET_OVERLAY_ALPHA          _IOW('B', 0x5, int)
#define STGFB_IO_GET_LAYER_HANDLE           _IOW('B', 0x6, int)
#define STGFB_IO_GET_VIEWPORT_HANDLE        _IOW('B', 0x7, int)
#define STGFB_IO_SET_FB_POSITION            _IOW('B', 0x8, int)
#if 1 //fang.shihuang
#define STGFB_IO_SET_VIEWPORT_HANDLE        _IOW('B', 0x9, int)
#define STGFB_IO_GET_BITMAP  _IOW('B', 0xa, STGXOBJ_Bitmap_t)
#define STGFB_IO_GET_EVTSUBID  _IOW('B', 0xb, STEVT_SubscriberID_t)
#define STGFB_IO_BLT                        _IOW('B', 0xc, STGFB_BltData_t)
#endif//fang.shihuang

typedef enum
{
  STGFB_COLOR_FORMAT_ARGB8888,
  STGFB_COLOR_FORMAT_RGB888,
  STGFB_COLOR_FORMAT_RGB565,
  STGFB_COLOR_FORMAT_ARGB1555
} STGFB_ColorFormat_t;




/*
 *  Structure for compositor alpha
 */
typedef struct STGFB_CompositorAlpha_s
{
    int Alpha0;
    int Alpha1;
} STGFB_CompositorAlpha_t;

/*
 * Structure for Color key
 */
typedef struct STGFB_CompositorColoKey_s
{
    U32 RMin;
    U32 GMin;
    U32 BMin;

    U32 RMax;
    U32 GMax;
    U32 BMax;
} STGFB_CompositorColoKey_t;


/*
 * Structure for Positionning FB
 */
typedef struct STGFB_Position_s
{
    U32 OffsetX;
    U32 OffsetY;
} STGFB_Position_t;

#if 1//fang.shihuang
typedef enum
{
  STGFB_BLIT_FLAG_NULL = 0x0,
  STGFB_BLIT_FLAG_SRC_COLORKEY = 0x2,
  STGFB_BLIT_FLAG_DST_COLORKEY = 0x4,
  STGFB_BLIT_FLAG_BLEND_SRC_ALPHA = 0x8,
  STGFB_BLIT_FLAG_FLICKERFILTER = 0x10
} STGFB_BltFlag_t;

typedef enum
{
  STGFB_BLIT_OP_NULL,
  STGFB_BLIT_OP_FILL_RECTANGLE,
  STGFB_BLIT_OP_COPY,
  STGFB_BLIT_OP_DRAW_RECTANGLE,
  STGFB_BLIT_OP_FLICKER_FILTER,
  STGFB_BLIT_OP_UPDATE_SCREEN,
  STGFB_BLIT_OP_IMAGE_COPY
} STGFB_BltOp_t;

/*
 *  Structure for bitmap
 */
typedef struct STGFB_Pixmap_s
{
    U32                     Width;
    U32                     Height;
    U32                     Size;
    STGFB_ColorFormat_t     ColorType;            /* Color Type of the bitmap */
    void                    *Data_p;
} STGFB_Pixmap_t;


/*
 *  Structure for blitter operation
 */
typedef struct STGFB_BltData_s
{
    STGFB_BltOp_t             Operation;
    STGFB_BltFlag_t           Flags;
    STGXOBJ_Color_t           Color;
    U32                       Alpha;
    STGXOBJ_Rectangle_t       SrcRectangle;
    STGXOBJ_Rectangle_t       DstRectangle;
    STGXOBJ_ColorKey_t        ColorKey;
    STGFB_ColorFormat_t       Format;
    U32                       Pitch;
    STGFB_Pixmap_t            Pixmap;
} STGFB_BltData_t;
#endif //fang.shihuang

#endif /* STGFB_H */
