#include "Osd.h"
#include "Platform.h"

/****************************************************
*
*     func name 	:  DecInit
*     	 desp  	:  send data to decoder
*     	 para  	:  i32DecoderID    decoder handle
*				   pData  data pointer
*				   nLen    data length.
*
*	return value	:    >0   write data success  ,return write byte.
*			   	     <0   fail
*
*****************************************************/
INT32 DecData(INT32 i32DecoderID, UINT8* pData, INT32 nLen)
{
	return 0;
}
/****************************************************
*
*     func name 	:  DecInit
*     	 desp  	:  init one media path.
*     	 para  	:  i32PlayerID   player id
*				   pDecoderPara    type is T_DEC_PARA
*
*
*	return value	:    0   success
*			   	   -1   fail
*
*****************************************************/
INT32 DecInit(INT32 i32PlayerID, void *pDecoderPara)
{
	return 0;
}

/****************************************************
*
*     func name 	:  osdOpen
*     	 desp  	:  get osd layer handle
*     	 para  	:  index  0 : osd1  1:osd2
*	return value	:  >0  osd handle
*			   	   <0  fail
*
*****************************************************/
INT32 osdOpen(INT32   index)
{
	return 0;
}
/****************************************************
*
*     func name 	:  osdClose
*     	 desp  	:  close one osd layer by it's handle
*     	 para  	:  osdHandle
*	return value	:    0   success
*			   	   -1   fail
*
*****************************************************/
INT32 osdClose(INT32   osdHandle )
{
	return 0;
}
/****************************************************
*
*     func name 	:  osdOn
*     	 desp  	:  switch on one osd layer
*     	 para  	:  osdHandle
*	return value	:    0   success
*			   	   -1   fail
*
*****************************************************/
INT32 osdOn(INT32  osdHandle)
{
	return 0;
}
/****************************************************
*
*     func name 	:  osdOff
*     	 desp  	:  switch off one osd layer .
*     	 para  	:  osdHandle
*	return value	:    0   success
*			   	   -1   fail
*
*****************************************************/
INT32 osdOff(INT32  osdHandle)
{
	return 0;
}

INT32 osdSetColorKey(osd_obj_t* osd_this,INT32 iColorKey)

{
	return 0;
}

/****************************************************
*
*     func name 	:  osdDispRect
*     	 desp  	:  display rectangle at a giving postion.
*     	 para  	:  osdHandle
*				    argb		:  32bit mode alpha (the highest byte) blue (the lowest byte)
*							   24bit mode represent  rgb  (red the highest byte)
*							   16bit mode  represent rgb  (red the highest byte)
*				    x0,y0		: top left point.
*				    x1,y1		: bottom right point.
*
*	return value	:    0   success
*			   	   -1   fail
*
*****************************************************/
INT32 osdDispRect(osd_obj_t *  osd_this,U32  argb,U16 x0,U16 y0,U16 x1,U16 y1)
{
	return 0;
}
/****************************************************
*
*     func name 	:  osdEraseRect
*     	 desp  	:  erase rectangle at a giving postion.
*     	 para  	:  osdHandle
*				    x0,y0		: top left point.
*				    x1,y1		: bottom right point.
*
*	return value	:    0   success
*			   	   -1   fail
*
*****************************************************/
INT32 osdEraseRect(osd_obj_t* osd_this,U16 x0,U16 y0,U16 x1,U16 y1)
{
	return 0;
}

/****************************************************
*
*     func name 	:  osdInit
*     	 desp  	:  osd layer init
*     	 para  	:  osdHandle
*	return value	:  0  SUCCESS
*			   	   <0  fail
*
*****************************************************/
INT32 osdInit(INT32 osdHandle,U8 bpp)
{
	return 0;
}
/****************************************************
*
*     func name 	:  UninitOsd
*     	 desp  	:  osd layer UNinit
*     	 para  	:  osdHandle
*	return value	:  0  SUCCESS
*			   	   <0  fail
*
*****************************************************/
INT32 UninitOsd(INT32 osdHandle)
{
	return 0;
}

int  test_osd_display(void* para)
{
	return 0;
}

osd_obj_t*  create_osd_obj(int mode,int osd_index)
{
	return 0;
}
int  test_ge2d_op(void* para)
{
	return 0;
}

int 	test_osd_colorkey(void* para)
{
	return 0;
}

int destroy_osd_obj(osd_obj_t* osd_obj)
{
	return 0;
}

//ge2d_op
static inline int __setup_region_axis(osd_obj_t *this_osd ,rectangle_t *rect_to,rectangle_t *rect_from)
{
	*rect_to=*rect_from;
#ifdef DOUBLE_BUFFER_SUPPORT
	if(this_osd->type == CANVAS_OSD0 || this_osd->type == CANVAS_OSD1)
	{
		rect_to->y += this_osd->vinfo->yoffset >0?0:this_osd->vinfo->yres;
	}
#endif
	return 0;
}

static inline int __setup_para_axis(osd_obj_t *this_osd ,src_dst_para_ex_t *para_to)
{
	//*rect_to=*rect_from;
    para_to->top = 0;
    para_to->left = 0;
    para_to->width = this_osd->nWidth;
#ifdef DOUBLE_BUFFER_SUPPORT
    printf("__setup_para_axis %d\n", this_osd->type);
    if(this_osd->type == CANVAS_OSD0 ||this_osd->type == CANVAS_OSD1)
    {
       printf("this_osd->vinfo->yres_virtual = %d\n", this_osd->vinfo->yres_virtual);
    	para_to->height = this_osd->vinfo->yres_virtual;
    }
    else
    {
#endif
       printf("tthis_osd->nHeight= %d\n", this_osd->nHeight);
      para_to->height = this_osd->nHeight;
#ifdef DOUBLE_BUFFER_SUPPORT
    }
#endif
    return 0;
}

int ge2d_op_fill_rect(osd_obj_t *this_osd, rectangle_t this_rect, unsigned long color)
{
    config_para_ex_t  ge2d_config_ex;
    ge2d_op_para_t  op_para;

    memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));
    memset((char*)&op_para,0,sizeof(ge2d_op_para_t));


    ge2d_config_ex.src_para.mem_type = this_osd->type;
    ge2d_config_ex.src_para.format  = this_osd->pixelformat;
    ge2d_config_ex.src_planes[0].addr = this_osd->planes[0].addr;
    ge2d_config_ex.src_planes[0].w = this_osd->planes[0].w;
    ge2d_config_ex.src_planes[0].h = this_osd->planes[0].h;

    ge2d_config_ex.src_para.color = 0;
/*
    ge2d_config_ex.src_para.top = 0;
    ge2d_config_ex.src_para.left = 0;
    ge2d_config_ex.src_para.width = this_osd->nWidth;
    if(this_osd->type == CANVAS_OSD0 ||this_osd->type == CANVAS_OSD1)
    ge2d_config_ex.src_para.height = this_osd->vinfo->yres_virtual;
    else
    ge2d_config_ex.src_para.height = this_osd->nHeight;*/
    __setup_para_axis(this_osd, &ge2d_config_ex.src_para);

    ge2d_config_ex.src2_para.mem_type = CANVAS_TYPE_INVALID;

    ge2d_config_ex.dst_para.mem_type = this_osd->type;
    ge2d_config_ex.dst_para.format  = this_osd->pixelformat;
    ge2d_config_ex.dst_planes[0].addr = this_osd->planes[0].addr;
    ge2d_config_ex.dst_planes[0].w = this_osd->planes[0].w;
    ge2d_config_ex.dst_planes[0].h = this_osd->planes[0].h;

    ge2d_config_ex.dst_para.color = 0;
/*
    ge2d_config_ex.dst_para.top = 0;
    ge2d_config_ex.dst_para.left = 0;
    ge2d_config_ex.dst_para.width = this_osd->nWidth;
    if(this_osd->type == CANVAS_OSD0 ||this_osd->type == CANVAS_OSD1)
    ge2d_config_ex.dst_para.height = this_osd->nHeight * 2;
    else
    ge2d_config_ex.dst_para.height = this_osd->nHeight;
*/
    __setup_para_axis(this_osd, &ge2d_config_ex.dst_para);

    ioctl(this_osd->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);


    /*op_para.src1_rect.x = this_rect.x;
    op_para.src1_rect.y = this_rect.y;
    op_para.src1_rect.w = this_rect.w;
    op_para.src1_rect.h = this_rect.h;

    op_para.dst_rect.x = this_rect.x;
    op_para.dst_rect.y = this_rect.y;
    op_para.dst_rect.w = this_rect.w;
    op_para.dst_rect.h = this_rect.h;*/
    __setup_region_axis(this_osd,&op_para.src1_rect,&this_rect);
    __setup_region_axis(this_osd,&op_para.dst_rect,&this_rect);
    op_para.color = color;

    printf("color = %x    %x %x %x %x\n", (unsigned int)color, (unsigned int)this_osd->fbp[0],(unsigned int) this_osd->fbp[1],
    		(unsigned int)this_osd->fbp[2], (unsigned int)this_osd->fbp[3]);

    ioctl(this_osd->ge2d_fd, GE2D_FILLRECTANGLE, &op_para);

//    printf("color = %x    %x %x %x %x\n", color, this_osd->fbp[0], this_osd->fbp[1],
//                                                            this_osd->fbp[2], this_osd->fbp[3]);

    return 0;
}

int ge2d_op_blit(osd_obj_t *dst, rectangle_t dst_rect, osd_obj_t *src,  rectangle_t src_rect)
{
        printf("dst -> %d %d         %d  %d\n", dst->type, CANVAS_OSD0,  src->type, CANVAS_ALLOC);
        printf("dst->nHeight  %d, dst->nWidth  %d\n", dst->nHeight, dst->nWidth);

        if(dst_rect.w > src_rect.w)
            dst_rect.w = src_rect.w;
        else
            src_rect.w = dst_rect.w;

        printf("rect %d %d %d %d %d %d %d %d\n", dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h,
            src_rect.x, src_rect.y, src_rect.w, src_rect.h);

        printf("rect %d %d %d %d %d %d %x %d\n", dst->fbp, dst->fbfd, dst->ge2d_fd, src->fbfd, src->ge2d_fd, src->fbp,
            src->planes[0].addr, src->planes[0].w);
#if 0
	config_para_t ge2d_config;
	ge2d_op_para_t  op_para;
       memset(&ge2d_config, 0, sizeof(config_para_t));
       memset(&op_para, 0, sizeof(ge2d_op_para_t));
//    CANVAS_OSD0 =0,
//    CANVAS_OSD1,
//    CANVAS_ALLOC,
//    CANVAS_TYPE_INVALID,
    switch(dst->type)
    {
        case CANVAS_OSD0:
            switch(src->type)
            {
                case CANVAS_OSD0:
                    ge2d_config.op_type=OSD0_OSD0;
                    break;
                case CANVAS_OSD1:
                    ge2d_config.op_type=OSD1_OSD0;
                    break;
                case CANVAS_ALLOC:
                    ge2d_config.op_type=ALLOC_OSD0;
                    break;
                 default:
                    return 1;
                    break;
            };
            break;
        case CANVAS_OSD1:
            switch(src->type)
            {
                case CANVAS_OSD0:
                    ge2d_config.op_type=OSD0_OSD1;
                    break;
                case CANVAS_OSD1:
                    ge2d_config.op_type=OSD1_OSD1;
                    break;
                case CANVAS_ALLOC:
                    ge2d_config.op_type=ALLOC_OSD1;
                    break;
                 default:
                    return 1;
                    break;
            };
            break;
        case CANVAS_ALLOC:
            switch(src->type)
            {
                case CANVAS_OSD0:
                    //ge2d_config.op_type=OSD0_ALLOC;
                    //not support;
                    return 1;
                    break;
                case CANVAS_OSD1:
                    //ge2d_config.op_type=OSD1_ALLOC;
                    //not support
                    return 1;
                    break;
                case CANVAS_ALLOC:
                    ge2d_config.op_type=ALLOC_ALLOC;
                    break;
                 default:
                    return 1;
                    break;
            };
            break;
        default:
            return 1;
            break;
        };

	//ge2d_config.op_type=OSD1_OSD0;
	ge2d_config.dst_format = dst->pixelformat;
	//ge2d_config. // = dst->pixelformat;
	ge2d_config.dst_planes.addr = dst->planes[0].addr;
	ge2d_config.dst_planes.h = dst->planes[0].h;
	ge2d_config.dst_planes.w = dst->planes[0].w;

       ge2d_config.src_format = src->pixelformat;
	ge2d_config.src_planes.addr = src->planes[0].addr;
	ge2d_config.src_planes.h = src->planes[0].h;
	ge2d_config.src_planes.w = src->planes[0].w;
       //ge2d_config.src_key = key_
	ge2d_config.alu_const_color=0x00000000;
	ioctl(dst->ge2d_fd, GE2D_CONFIG, &ge2d_config) ;

	op_para.src1_rect.x=dst_rect.x;
	op_para.src1_rect.y=dst_rect.y;
	op_para.src1_rect.w=dst_rect.w;
	op_para.src1_rect.h=dst_rect.h;

	op_para.dst_rect.x=src_rect.x;
	op_para.dst_rect.y=src_rect.y;
	op_para.dst_rect.w=src_rect.w;
	op_para.dst_rect.h=src_rect.h;

	ioctl(dst->ge2d_fd, GE2D_BLIT, &op_para);
	return 0;
#else
    ge2d_op_para_t  op_para;
    config_para_ex_t ge2d_config_ex;
    memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));
    memset(&op_para, 0, sizeof(ge2d_op_para_t));

    ge2d_config_ex.src_para.mem_type = src->type;
    ge2d_config_ex.src_para.format=src->pixelformat;

    /*ge2d_config_ex.src_para.width=src->nWidth;
    ge2d_config_ex.src_para.height=src->nHeight;
    ge2d_config_ex.src_para.left=0;
    ge2d_config_ex.src_para.top=0;*/
     __setup_para_axis(src, &ge2d_config_ex.src_para);
		//plane
    ge2d_config_ex.src_planes[0].addr=src->planes[0].addr;
    ge2d_config_ex.src_planes[0].w=src->planes[0].w;
    ge2d_config_ex.src_planes[0].h=src->planes[0].h;
/*
    ge2d_config_ex.src_planes[1].addr=src->planes[1].addr;
    ge2d_config_ex.src_planes[1].w=src->planes[1].w;
    ge2d_config_ex.src_planes[1].h=src->planes[1].h;

    ge2d_config_ex.src_planes[2].addr=src->planes[2].addr;
    ge2d_config_ex.src_planes[2].w=src->planes[2].w;
    ge2d_config_ex.src_planes[2].h=src->planes[2].h;
*/
    ge2d_config_ex.src2_para.mem_type=CANVAS_TYPE_INVALID;
/*
    ge2d_config_ex.src2_para.format=src2->pixelformat;
    ge2d_config_ex.src2_para.width=src2->nWidth;
    ge2d_config_ex.src2_para.height=src2->nHeight;
    ge2d_config_ex.src2_para.left=0;
    ge2d_config_ex.src2_para.top=0;

    ge2d_config_ex.src2_planes[0].addr=src2->planes[0].addr;
    ge2d_config_ex.src2_planes[0].w=src2->planes[0].w;
    ge2d_config_ex.src2_planes[0].h=src2->planes[0].h;

    ge2d_config_ex.src2_planes[1].addr=src2->planes[1].addr;
    ge2d_config_ex.src2_planes[1].w=src2->planes[1].w;
    ge2d_config_ex.src2_planes[1].h=src2->planes[1].h;

    ge2d_config_ex.src2_planes[2].addr=src2->planes[2].addr;
    ge2d_config_ex.src2_planes[2].w=src2->planes[2].w;
    ge2d_config_ex.src2_planes[2].h=src2->planes[2].h;

*/

//    ge2d_config_ex.dst_para.mem_type = dst->type;
//    ge2d_config_ex.dst_para.format = dst->pixelformat;
//    ge2d_config_ex.dst_para.left = 0;
//    ge2d_config_ex.dst_para.top = 0;
//    ge2d_config_ex.dst_para.width = osd[0]->vinfo->xres;
//    ge2d_config_ex.dst_para.height = osd[0]->vinfo->yres;

    ge2d_config_ex.dst_para.mem_type=dst->type;

    ge2d_config_ex.dst_para.format=dst->pixelformat;
    /*ge2d_config_ex.dst_para.width=dst->nWidth;
    ge2d_config_ex.dst_para.height=dst->nHeight;
    ge2d_config_ex.dst_para.left=0;
    ge2d_config_ex.dst_para.top=0;*/
     __setup_para_axis(dst, &ge2d_config_ex.dst_para);

    ge2d_config_ex.dst_planes[0].addr=dst->planes[0].addr;
    ge2d_config_ex.dst_planes[0].w=dst->planes[0].w;
    ge2d_config_ex.dst_planes[0].h=dst->planes[0].h;
/*
    ge2d_config_ex.dst_planes[1].addr=dst->planes[1].addr;
    ge2d_config_ex.dst_planes[1].w=dst->planes[1].w;
    ge2d_config_ex.dst_planes[1].h=dst->planes[1].h;

    ge2d_config_ex.dst_planes[2].addr=dst->planes[2].addr;
    ge2d_config_ex.dst_planes[2].w=dst->planes[2].w;
    ge2d_config_ex.dst_planes[2].h=dst->planes[2].h;*/

    //printf("    ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);\n");
 //printf("    ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);  end\n");
//    memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
     ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
/*
    op_para.src1_rect.x = src_rect.x;
    op_para.src1_rect.y = src_rect.y;
    op_para.src1_rect.w =  src_rect.w;
    op_para.src1_rect.h = src_rect.h;

    op_para.dst_rect.x = dst_rect.x;
    op_para.dst_rect.y = dst_rect.y;
    op_para.dst_rect.w = dst_rect.w;
    op_para.dst_rect.h = dst_rect.h;
*/
   __setup_region_axis(src,&op_para.src1_rect,&src_rect);
	__setup_region_axis(dst,&op_para.dst_rect,&dst_rect);
    //printf("   ioctl(dst->ge2d_fd,GE2D_STRETCHBLIT_NOALPHA, &op_para);;\n");
   //int i = 0;
   //double begin = mysecond();
   //        ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
   //for(i = 0; i < 1000; i++)
   //{
        ioctl(dst->ge2d_fd,GE2D_BLIT_NOALPHA, &op_para);
   //}
   //  printf("111 1000 times need time %lf and ps = %f\n", mysecond() - begin, 1000.0 / (mysecond() - begin));

    //printf("   ioctl(dst->ge2d_fd,GE2D_STRETCHBLIT_NOALPHA, &op_para); end;\n");
    // printf("moving data from yuv to osd back end\n");

    return 0;
#endif
}

int ge2d_op_stretch_blit(osd_obj_t *dst, rectangle_t dst_rect, osd_obj_t *src,  rectangle_t src_rect)
{
#if 0
/*ALPHA_FACTOR_SRC_ALPHA                                                 As

ALPHA_FACTOR_ONE_MINUS_SRC_ALPHA                                      1 - As

ALPHA_FACTOR_DST_ALPHA                                                Ad

ALPHA_FACTOR_ONE_MINUS_DST_ALPHA                                      1 – Ad

ALPHA_FACTOR_CONST_ALPHA                                              Ac

ALPHA_FACTOR_ONE_MINUS_CONST_ALPHA                                     1 – Ac *

下面的例子演示如下的效果

  Cdst= Csrc1 *1 + Csrc2*0
  Adst= Asrc1*0 + Asrc2*1

Sample codes:

memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));

ge2d_config_ex.src_para.mem_type=CANVAS_ALLOC;

ge2d_config_ex.src_para.format=ge2d_yuv_format;

//clip region

ge2d_config_ex.src_para.width=PLANE_WIDTH;

ge2d_config_ex.src_para.height=PLANE_HEIGHT;

ge2d_config_ex.src_para.left=0;

ge2d_config_ex.src_para.top=0;

//plane

ge2d_config_ex.src_planes[0].addr=addr_y;

ge2d_config_ex.src_planes[0].w=PLANE_WIDTH;

ge2d_config_ex.src_planes[0].h=PLANE_HEIGHT;

ge2d_config_ex.src_planes[1].addr=addr_u;

ge2d_config_ex.src_planes[1].w=PLANE_WIDTH*x_ratio;

ge2d_config_ex.src_planes[1].h=PLANE_HEIGHT*y_ratio;

ge2d_config_ex.src_planes[2].addr=addr_v;

ge2d_config_ex.src_planes[2].w=PLANE_WIDTH*x_ratio;

ge2d_config_ex.src_planes[2].h=PLANE_HEIGHT*y_ratio;

ge2d_config_ex.src2_para.mem_type=CANVAS_ALLOC;

ge2d_config_ex.src2_para.format=GE2D_FORMAT_S32_ARGB;

ge2d_config_ex.src2_planes[0].addr=addr_rgb;

ge2d_config_ex.src2_planes[0].w=PLANE_WIDTH;

ge2d_config_ex.src2_planes[0].h=PLANE_HEIGHT;

ge2d_config_ex.src2_para.width=PLANE_WIDTH;

ge2d_config_ex.src2_para.height=PLANE_HEIGHT;

ge2d_config_ex.src2_para.left=0;

ge2d_config_ex.src2_para.top=0;

ge2d_config_ex.dst_para.mem_type=CANVAS_OSD0;

ge2d_config_ex.dst_para.left=0;

ge2d_config_ex.dst_para.top=0;

ge2d_config_ex.dst_para.width=osd[0]->vinfo->xres;

ge2d_config_ex.dst_para.height=osd[0]->vinfo->yres;

ge2d_config_ex.dst_xy_swap=0;

ge2d_config_ex.src_para.x_rev = 0;

ge2d_config_ex.src_para.y_rev = 0;

ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);

memset((char*)&op_para,0,sizeof(ge2d_op_para_t));

op_para.src1_rect.x = 0;

op_para.src1_rect.y = 0;

op_para.src1_rect.w =           PLANE_WIDTH;

op_para.src1_rect.h = PLANE_HEIGHT;

op_para.src2_rect.x = 0;

op_para.src2_rect.y = 0;

op_para.src2_rect.w =           PLANE_WIDTH;

op_para.src2_rect.h = PLANE_HEIGHT;

op_para.dst_rect.x = 0;

op_para.dst_rect.y = 0;

op_para.dst_rect.w =PLANE_WIDTH;

op_para.dst_rect.h = PLANE_HEIGHT;

op_para.op=blendop(

       OPERATION_ADD,

        color blending src factor */

       COLOR_FACTOR_ONE,

       /* color blending dst factor */

       COLOR_FACTOR_ZERO,

         /* alpha blending_mode */

         OPERATION_ADD,

         /* alpha blending src factor */

         ALPHA_FACTOR_ZERO,

       /* color blending dst factor */

         ALPHA_FACTOR_ONE);

ioctl(osd[0]->ge2d_fd,GE2D_BLEND, &op_para);
#else
    ge2d_op_para_t  op_para;
    config_para_ex_t ge2d_config_ex;
    memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));
    memset(&op_para, 0, sizeof(ge2d_op_para_t));

    ge2d_config_ex.src_para.mem_type = src->type;
    ge2d_config_ex.src_para.format=src->pixelformat;
    /*ge2d_config_ex.src_para.width=src->nWidth;
    ge2d_config_ex.src_para.height=src->nHeight;
    ge2d_config_ex.src_para.left=0;
    ge2d_config_ex.src_para.top=0;*/
     __setup_para_axis(src, &ge2d_config_ex.dst_para);
		//plane
    ge2d_config_ex.src_planes[0].addr=src->planes[0].addr;
    ge2d_config_ex.src_planes[0].w=src->planes[0].w;
    ge2d_config_ex.src_planes[0].h=src->planes[0].h;
/*
    ge2d_config_ex.src_planes[1].addr=src->planes[1].addr;
    ge2d_config_ex.src_planes[1].w=src->planes[1].w;
    ge2d_config_ex.src_planes[1].h=src->planes[1].h;

    ge2d_config_ex.src_planes[2].addr=src->planes[2].addr;
    ge2d_config_ex.src_planes[2].w=src->planes[2].w;
    ge2d_config_ex.src_planes[2].h=src->planes[2].h;
*/
    ge2d_config_ex.src2_para.mem_type=CANVAS_TYPE_INVALID;
/*
    ge2d_config_ex.src2_para.format=src2->pixelformat;
    ge2d_config_ex.src2_para.width=src2->nWidth;
    ge2d_config_ex.src2_para.height=src2->nHeight;
    ge2d_config_ex.src2_para.left=0;
    ge2d_config_ex.src2_para.top=0;

    ge2d_config_ex.src2_planes[0].addr=src2->planes[0].addr;
    ge2d_config_ex.src2_planes[0].w=src2->planes[0].w;
    ge2d_config_ex.src2_planes[0].h=src2->planes[0].h;

    ge2d_config_ex.src2_planes[1].addr=src2->planes[1].addr;
    ge2d_config_ex.src2_planes[1].w=src2->planes[1].w;
    ge2d_config_ex.src2_planes[1].h=src2->planes[1].h;

    ge2d_config_ex.src2_planes[2].addr=src2->planes[2].addr;
    ge2d_config_ex.src2_planes[2].w=src2->planes[2].w;
    ge2d_config_ex.src2_planes[2].h=src2->planes[2].h;

*/

//    ge2d_config_ex.dst_para.mem_type = dst->type;
//    ge2d_config_ex.dst_para.format = dst->pixelformat;
//    ge2d_config_ex.dst_para.left = 0;
//    ge2d_config_ex.dst_para.top = 0;
//    ge2d_config_ex.dst_para.width = osd[0]->vinfo->xres;
//    ge2d_config_ex.dst_para.height = osd[0]->vinfo->yres;

    ge2d_config_ex.dst_para.mem_type=dst->type;

    ge2d_config_ex.dst_para.format=dst->pixelformat;
    /*ge2d_config_ex.dst_para.width=dst->nWidth;
    ge2d_config_ex.dst_para.height=dst->nHeight;
    ge2d_config_ex.dst_para.left=0;
    ge2d_config_ex.dst_para.top=0;*/
    __setup_para_axis(dst, &ge2d_config_ex.dst_para);

    ge2d_config_ex.dst_planes[0].addr=dst->planes[0].addr;
    ge2d_config_ex.dst_planes[0].w=dst->planes[0].w;
    ge2d_config_ex.dst_planes[0].h=dst->planes[0].h;
/*
    ge2d_config_ex.dst_planes[1].addr=dst->planes[1].addr;
    ge2d_config_ex.dst_planes[1].w=dst->planes[1].w;
    ge2d_config_ex.dst_planes[1].h=dst->planes[1].h;

    ge2d_config_ex.dst_planes[2].addr=dst->planes[2].addr;
    ge2d_config_ex.dst_planes[2].w=dst->planes[2].w;
    ge2d_config_ex.dst_planes[2].h=dst->planes[2].h;*/

    //printf("    ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);\n");
 //printf("    ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);  end\n");
//    memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
     ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
/*
    op_para.src1_rect.x = src_rect.x;
    op_para.src1_rect.y = src_rect.y;
    op_para.src1_rect.w =  src_rect.w;
    op_para.src1_rect.h = src_rect.h;

    op_para.dst_rect.x = dst_rect.x;
    op_para.dst_rect.y = dst_rect.y;
    op_para.dst_rect.w = dst_rect.w;
    op_para.dst_rect.h = dst_rect.h;	*/
    	__setup_region_axis(src,&op_para.src1_rect,&src_rect);
	__setup_region_axis(dst,&op_para.dst_rect,&dst_rect);
    //printf("   ioctl(dst->ge2d_fd,GE2D_STRETCHBLIT_NOALPHA, &op_para);;\n");
   //int i = 0;
   //double begin = mysecond();
   //        ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
   //for(i = 0; i < 1000; i++)
   //{
        ioctl(dst->ge2d_fd, GE2D_STRETCHBLIT, &op_para);
   //}
   //  printf("111 1000 times need time %lf and ps = %f\n", mysecond() - begin, 1000.0 / (mysecond() - begin));

    //printf("   ioctl(dst->ge2d_fd,GE2D_STRETCHBLIT_NOALPHA, &op_para); end;\n");
    // printf("moving data from yuv to osd back end\n");

    return 0;

#endif
	return 0;
}

static inline unsigned blendop(unsigned color_blending_mode,
                               unsigned color_blending_src_factor,
                               unsigned color_blending_dst_factor,
                               unsigned alpha_blending_mode,
                               unsigned alpha_blending_src_factor,
                               unsigned alpha_blending_dst_factor)
{
    return (color_blending_mode << 24) |
           (color_blending_src_factor << 20) |
           (color_blending_dst_factor << 16) |
           (alpha_blending_mode << 8) |
           (alpha_blending_src_factor << 4) |
           (alpha_blending_dst_factor << 0);
}

int ge2d_op_blend(osd_obj_t *dst, rectangle_t dst_rect, osd_obj_t *src,  rectangle_t src_rect, osd_obj_t *src2,  rectangle_t src2_rect)
{
#if 1
/*ALPHA_FACTOR_SRC_ALPHA                                                 As
ALPHA_FACTOR_ONE_MINUS_SRC_ALPHA                                      1 - As
ALPHA_FACTOR_DST_ALPHA                                                Ad
ALPHA_FACTOR_ONE_MINUS_DST_ALPHA                                      1 – Ad
ALPHA_FACTOR_CONST_ALPHA                                              Ac
ALPHA_FACTOR_ONE_MINUS_CONST_ALPHA                                     1 – Ac


  Cdst= Csrc1 *1 + Csrc2*0
  Adst= Asrc1*0 + Asrc2*1 */
    config_para_ex_t ge2d_config_ex;
    ge2d_op_para_t  op_para;
    memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));
    memset(&op_para, 0, sizeof(ge2d_op_para_t));

    ge2d_config_ex.src_para.mem_type = src->type;
    ge2d_config_ex.src_para.format=src->pixelformat;
    /*ge2d_config_ex.src_para.width=src->nWidth;
    ge2d_config_ex.src_para.height=src->nHeight;
    ge2d_config_ex.src_para.left=0;
    ge2d_config_ex.src_para.top=0;*/
     __setup_para_axis(src, &ge2d_config_ex.src_para);
		//plane
    ge2d_config_ex.src_planes[0].addr=src->planes[0].addr;
    ge2d_config_ex.src_planes[0].w=src->planes[0].w;
    ge2d_config_ex.src_planes[0].h=src->planes[0].h;
/*
    ge2d_config_ex.src_planes[1].addr=src->planes[1].addr;
    ge2d_config_ex.src_planes[1].w=src->planes[1].w;
    ge2d_config_ex.src_planes[1].h=src->planes[1].h;

    ge2d_config_ex.src_planes[2].addr=src->planes[2].addr;
    ge2d_config_ex.src_planes[2].w=src->planes[2].w;
    ge2d_config_ex.src_planes[2].h=src->planes[2].h;
*/
    ge2d_config_ex.src2_para.mem_type=src2->type;

    ge2d_config_ex.src2_para.format=src2->pixelformat;
    /*ge2d_config_ex.src2_para.width=src2->nWidth;
    ge2d_config_ex.src2_para.height=src2->nHeight;
    ge2d_config_ex.src2_para.left=0;
    ge2d_config_ex.src2_para.top=0;*/
     __setup_para_axis(src2, &ge2d_config_ex.src2_para);

    ge2d_config_ex.src2_planes[0].addr=src2->planes[0].addr;
    ge2d_config_ex.src2_planes[0].w=src2->planes[0].w;
    ge2d_config_ex.src2_planes[0].h=src2->planes[0].h;
/*
    ge2d_config_ex.src2_planes[1].addr=src2->planes[1].addr;
    ge2d_config_ex.src2_planes[1].w=src2->planes[1].w;
    ge2d_config_ex.src2_planes[1].h=src2->planes[1].h;

    ge2d_config_ex.src2_planes[2].addr=src2->planes[2].addr;
    ge2d_config_ex.src2_planes[2].w=src2->planes[2].w;
    ge2d_config_ex.src2_planes[2].h=src2->planes[2].h;

*/

//    ge2d_config_ex.dst_para.mem_type = dst->type;
//    ge2d_config_ex.dst_para.format = dst->pixelformat;
//    ge2d_config_ex.dst_para.left = 0;
//    ge2d_config_ex.dst_para.top = 0;
//    ge2d_config_ex.dst_para.width = osd[0]->vinfo->xres;
//    ge2d_config_ex.dst_para.height = osd[0]->vinfo->yres;

    ge2d_config_ex.dst_para.mem_type=dst->type;

    ge2d_config_ex.dst_para.format=dst->pixelformat;
    /*ge2d_config_ex.dst_para.width=dst->nWidth;
    ge2d_config_ex.dst_para.height=dst->nHeight;
    ge2d_config_ex.dst_para.left=0;
    ge2d_config_ex.dst_para.top=0;*/
     __setup_para_axis(dst, &ge2d_config_ex.dst_para);

    ge2d_config_ex.dst_planes[0].addr=dst->planes[0].addr;
    ge2d_config_ex.dst_planes[0].w=dst->planes[0].w;
    ge2d_config_ex.dst_planes[0].h=dst->planes[0].h;
/*
    ge2d_config_ex.dst_planes[1].addr=dst->planes[1].addr;
    ge2d_config_ex.dst_planes[1].w=dst->planes[1].w;
    ge2d_config_ex.dst_planes[1].h=dst->planes[1].h;

    ge2d_config_ex.dst_planes[2].addr=dst->planes[2].addr;
    ge2d_config_ex.dst_planes[2].w=dst->planes[2].w;
    ge2d_config_ex.dst_planes[2].h=dst->planes[2].h;*/

printf("aaaaaaaaaaaaaaaaaaaaa\n");
    ioctl(dst->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
printf("bbbbbbbbbbbbbbbbbbbbb\n");
/*
    op_para.src1_rect.x = src_rect.x;
    op_para.src1_rect.y = src_rect.y;
    op_para.src1_rect.w =  src_rect.w;
    op_para.src1_rect.h =  src_rect.h;

    op_para.src2_rect.x = src2_rect.x;
    op_para.src2_rect.y = src2_rect.y;
    op_para.src2_rect.w = src2_rect.w;
    op_para.src2_rect.h = src2_rect.h;

    op_para.dst_rect.x = dst_rect.x;
    op_para.dst_rect.y = dst_rect.y;
    op_para.dst_rect.w =  dst_rect.w;
    op_para.dst_rect.h = dst_rect.h;*/
   	__setup_region_axis(src,&op_para.src1_rect,&src_rect);
	__setup_region_axis(src2,&op_para.src2_rect,&src2_rect);
	__setup_region_axis(dst,&op_para.dst_rect,&dst_rect);

    op_para.op=blendop(
       OPERATION_ADD,
       /* color blending src factor */
       COLOR_FACTOR_SRC_ALPHA,
       /* color blending dst factor */
       COLOR_FACTOR_ONE_MINUS_SRC_ALPHA,
         /* alpha blending_mode */
         OPERATION_ADD,
         /* alpha blending src factor */
         ALPHA_FACTOR_ZERO,
       /* color blending dst factor */
         ALPHA_FACTOR_ONE);
    printf("ge2d_config_ex.src_para.mem_type = %d\n", src->type);
    printf("ge2d_config_ex.src_para.format = %d\n", src->pixelformat);
    printf("ge2d_config_ex.src_para.width= %d\n", src->nWidth);
    printf("ge2d_config_ex.src_para.height= %d\n", src->nHeight);
    printf("ge2d_config_ex.src_para.left= %d\n", 0);
    printf("ge2d_config_ex.src_para.top= %d\n", 0);
		//plane
    printf("ge2d_config_ex.src_planes[0].addr= %x\n", (unsigned int)src->planes[0].addr);
    printf("ge2d_config_ex.src_planes[0].w= %d\n", src->planes[0].w);
    printf("ge2d_config_ex.src_planes[0].h= %d\n", src->planes[0].h);

    printf("ge2d_config_ex.src2_para.mem_type = %d\n", src2->type);
    printf("ge2d_config_ex.src2_para.format = %d\n", src2->pixelformat);
    printf("ge2d_config_ex.src2_para.width= %d\n", src2->nWidth);
    printf("ge2d_config_ex.src2_para.height= %d\n", src2->nHeight);
    printf("ge2d_config_ex.src2_para.left= %d\n", 0);
    printf("ge2d_config_ex.src2_para.top= %d\n", 0);
		//plane
    printf("ge2d_config_ex.src2_planes[0].addr= %x\n", (unsigned int)src2->planes[0].addr);
    printf("ge2d_config_ex.src2_planes[0].w= %d\n", src2->planes[0].w);
    printf("ge2d_config_ex.src2_planes[0].h= %d\n", src2->planes[0].h);


    printf("ge2d_config_ex.dst_para.mem_type = %d\n", dst->type);
    printf("ge2d_config_ex.dst_para.format = %d\n", dst->pixelformat);
    printf("ge2d_config_ex.dst_para.width= %d\n", dst->nWidth);
    printf("ge2d_config_ex.dst_para.height= %d\n", dst->nHeight);
    printf("ge2d_config_ex.dst_para.left= %d\n", 0);
    printf("ge2d_config_ex.dst_para.top= %d\n", 0);
		//plane
    printf("ge2d_config_ex.dst_planes[0].addr= %x\n", (unsigned int)dst->planes[0].addr);
    printf("ge2d_config_ex.dst_planes[0].w= %d\n", dst->planes[0].w);
    printf("ge2d_config_ex.dst_planes[0].h= %d\n", dst->planes[0].h);

    printf("op_para.src1_rect.x = %d\n", src_rect.x);
    printf("op_para.src1_rect.y = %d\n", src_rect.y);
    printf("op_para.src1_rect.w =  %d\n", src_rect.w);
    printf("op_para.src1_rect.h =  %d\n", src_rect.h);

    printf("op_para.src2_rect.x = %d\n", src2_rect.x);
    printf("op_para.src2_rect.y = %d\n", src2_rect.y);
    printf("op_para.src2_rect.w =  %d\n", src2_rect.w);
    printf("op_para.src2_rect.h =  %d\n", src2_rect.h);

    printf("op_para.dst_rect.x = %d\n", dst_rect.x);
    printf("op_para.dst_rect.y = %d\n", dst_rect.y);
    printf("op_para.dst_rect.w =  %d\n", dst_rect.w);
    printf("op_para.dst_rect.h =  %d\n", dst_rect.h);
printf("cccccccccccccccccccccccccc\n");
    ioctl(dst->ge2d_fd,GE2D_BLEND, &op_para);
printf("ddddddddddddddddddddddd\n");

#endif

	return 0;
}

void osd_set_invisable(osd_obj_t * this_osd,int enable)
{
    ioctl(this_osd->fbfd,FBIOBLANK,enable);
}

int osd_update_region(osd_obj_t *this_osd,rectangle_t *update_rect)
{
	config_para_t ge2d_config;
	ge2d_op_para_t  op_para;
//cmem region do not support update,only hardware osd support it
	if(this_osd->type == CANVAS_ALLOC) return -1;
    	this_osd->vinfo->yoffset=this_osd->vinfo->yoffset >0?0:this_osd->vinfo->yres;
    	ioctl(this_osd->fbfd, FBIOPAN_DISPLAY, this_osd->vinfo);
//if update region is null ,then only do pan then return
	if(update_rect==NULL||update_rect->w==0 || update_rect->h==0) return 0;
     	switch (this_osd->type)
     	{
			case CANVAS_OSD0:
					ge2d_config.op_type=OSD0_OSD0;
					break;
			case CANVAS_OSD1:
					ge2d_config.op_type=OSD1_OSD1;
					break;
			default:
				break;
     	}
	ioctl( this_osd->ge2d_fd, GE2D_CONFIG, &ge2d_config) ;
	op_para.dst_rect =op_para.src1_rect=*update_rect;
	op_para.src1_rect.y+=this_osd->vinfo->yres;
	if(this_osd->vinfo->yoffset<this_osd->vinfo->yres)
	{
		swap(op_para.src1_rect.y,op_para.dst_rect.y);
	}
	ioctl( this_osd->ge2d_fd, GE2D_BLIT, &op_para) ;
	return 0;
}

int   osd_order_change(void* para)
{
	return 0;
}

int   multi_process_test(void* para)
{
	return 0;
}

 //osd
int show_bitmap(char *p, unsigned int line_width, unsigned int xres, unsigned int yres, unsigned int bpp)
{
	return 0;
}


