#ifndef   _OSD_H_
#define   _OSD_H_
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/wait.h> 


#define  BMP_FILE_NAME		"./picture.bmp"

#define  BITMAP_USE_FILE
#define   GE2D_BLEND			 0x4700
/**blend op relative macro*/
#define OPERATION_ADD           0    //Cd = Cs*Fs+Cd*Fd
#define OPERATION_SUB           1    //Cd = Cs*Fs-Cd*Fd
#define OPERATION_REVERSE_SUB   2    //Cd = Cd*Fd-Cs*Fs
#define OPERATION_MIN           3    //Cd = Min(Cd*Fd,Cs*Fs)
#define OPERATION_MAX           4    //Cd = Max(Cd*Fd,Cs*Fs)
#define OPERATION_LOGIC         5

#define COLOR_FACTOR_ZERO                     0
#define COLOR_FACTOR_ONE                      1
#define COLOR_FACTOR_SRC_COLOR                2
#define COLOR_FACTOR_ONE_MINUS_SRC_COLOR      3
#define COLOR_FACTOR_DST_COLOR                4
#define COLOR_FACTOR_ONE_MINUS_DST_COLOR      5
#define COLOR_FACTOR_SRC_ALPHA                6
#define COLOR_FACTOR_ONE_MINUS_SRC_ALPHA      7
#define COLOR_FACTOR_DST_ALPHA                8
#define COLOR_FACTOR_ONE_MINUS_DST_ALPHA      9
#define COLOR_FACTOR_CONST_COLOR              10
#define COLOR_FACTOR_ONE_MINUS_CONST_COLOR    11
#define COLOR_FACTOR_CONST_ALPHA              12
#define COLOR_FACTOR_ONE_MINUS_CONST_ALPHA    13
#define COLOR_FACTOR_SRC_ALPHA_SATURATE       14

#define ALPHA_FACTOR_ZERO                     0
#define ALPHA_FACTOR_ONE                      1
#define ALPHA_FACTOR_SRC_ALPHA                2
#define ALPHA_FACTOR_ONE_MINUS_SRC_ALPHA      3
#define ALPHA_FACTOR_DST_ALPHA                4
#define ALPHA_FACTOR_ONE_MINUS_DST_ALPHA      5
#define ALPHA_FACTOR_CONST_ALPHA              6
#define ALPHA_FACTOR_ONE_MINUS_CONST_ALPHA    7

#define LOGIC_OPERATION_CLEAR       0
#define LOGIC_OPERATION_COPY        1
#define LOGIC_OPERATION_NOOP        2
#define LOGIC_OPERATION_SET         3
#define LOGIC_OPERATION_COPY_INVERT 4
#define LOGIC_OPERATION_INVERT      5
#define LOGIC_OPERATION_AND_REVERSE 6
#define LOGIC_OPERATION_OR_REVERSE  7
#define LOGIC_OPERATION_AND         8
#define LOGIC_OPERATION_OR          9
#define LOGIC_OPERATION_NAND        10
#define LOGIC_OPERATION_NOR         11
#define LOGIC_OPERATION_XOR         12
#define LOGIC_OPERATION_EQUIV       13
#define LOGIC_OPERATION_AND_INVERT  14
#define LOGIC_OPERATION_OR_INVERT   15 

#define BLENDOP_ADD           0    //Cd = Cs*Fs+Cd*Fd
#define BLENDOP_SUB           1    //Cd = Cs*Fs-Cd*Fd
#define BLENDOP_REVERSE_SUB   2    //Cd = Cd*Fd-Cs*Fs
#define BLENDOP_MIN           3    //Cd = Min(Cd*Fd,Cs*Fs)
#define BLENDOP_MAX           4    //Cd = Max(Cd*Fd,Cs*Fs)
#define BLENDOP_LOGIC         5    //Cd = Cs op Cd
#define BLENDOP_LOGIC_CLEAR       (BLENDOP_LOGIC+0 )
#define BLENDOP_LOGIC_COPY        (BLENDOP_LOGIC+1 )
#define BLENDOP_LOGIC_NOOP        (BLENDOP_LOGIC+2 )
#define BLENDOP_LOGIC_SET         (BLENDOP_LOGIC+3 )
#define BLENDOP_LOGIC_COPY_INVERT (BLENDOP_LOGIC+4 )
#define BLENDOP_LOGIC_INVERT      (BLENDOP_LOGIC+5 )
#define BLENDOP_LOGIC_AND_REVERSE (BLENDOP_LOGIC+6 )
#define BLENDOP_LOGIC_OR_REVERSE  (BLENDOP_LOGIC+7 )
#define BLENDOP_LOGIC_AND         (BLENDOP_LOGIC+8 )
#define BLENDOP_LOGIC_OR          (BLENDOP_LOGIC+9 )
#define BLENDOP_LOGIC_NAND        (BLENDOP_LOGIC+10)
#define BLENDOP_LOGIC_NOR         (BLENDOP_LOGIC+11)
#define BLENDOP_LOGIC_XOR         (BLENDOP_LOGIC+12)
#define BLENDOP_LOGIC_EQUIV       (BLENDOP_LOGIC+13)
#define BLENDOP_LOGIC_AND_INVERT  (BLENDOP_LOGIC+14)
#define BLENDOP_LOGIC_OR_INVERT   (BLENDOP_LOGIC+15) 



#define GE2D_ENDIAN_SHIFT       	24
#define GE2D_ENDIAN_MASK            (0x1 << GE2D_ENDIAN_SHIFT)
#define GE2D_BIG_ENDIAN             (0 << GE2D_ENDIAN_SHIFT)
#define GE2D_LITTLE_ENDIAN          (1 << GE2D_ENDIAN_SHIFT)

#define GE2D_COLOR_MAP_SHIFT        20
#define GE2D_COLOR_MAP_MASK         (0xf << GE2D_COLOR_MAP_SHIFT)
/* 16 bit */
#define GE2D_COLOR_MAP_YUV422		(0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGB655		(1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUV655		(1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGB844		(2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUV844		(2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA6442     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA6442     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA4444     (4 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA4444     (4 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGB565       (5 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUV565       (5 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB4444		(6 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV4444		(6 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB1555     (7 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV1555     (7 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA4642     (8 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA4642     (8 << GE2D_COLOR_MAP_SHIFT)
/* 24 bit */
#define GE2D_COLOR_MAP_RGB888       (0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUV444       (0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA5658     (1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA5658     (1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB8565     (2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV8565     (2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA6666     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA6666     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB6666     (4 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV6666     (4 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_BGR888		(5 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_VUY888		(5 << GE2D_COLOR_MAP_SHIFT)
/* 32 bit */
#define GE2D_COLOR_MAP_RGBA8888		(0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA8888		(0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB8888     (1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV8888     (1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ABGR8888     (2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AVUY8888     (2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_BGRA8888     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_VUYA8888     (3 << GE2D_COLOR_MAP_SHIFT)

/* format code is defined as:
[11] : 1-YUV color space, 0-RGB color space
[10] : compress_range
[9:8]: format
[7:6]: 8bit_mode_sel
[5]  : LUT_EN
[4:3]: PIC_STRUCT
[2]  : SEP_EN
[1:0]: X_YC_RATIO, SRC1_Y_YC_RATIO
*/
#define GE2D_FORMAT_MASK            0x0ffff
#define GE2D_BPP_MASK								0x00300
#define GE2D_BPP_8BIT								0x00000
#define GE2D_BPP_16BIT							0x00100
#define GE2D_BPP_24BIT							0x00200
#define GE2D_BPP_32BIT							0x00300
#define GE2D_FORMAT_YUV             0x20000
#define GE2D_FORMAT_COMP_RANGE      0x10000
/*bit8(2)  format   bi6(2) mode_8b_sel  bit5(1)lut_en   bit2 sep_en*/
/*M  seperate block S one block.*/ 

#define GE2D_FMT_S8_Y            	0x00000 /* 00_00_0_00_0_00 */
#define GE2D_FMT_S8_CB           	0x00040 /* 00_01_0_00_0_00 */
#define GE2D_FMT_S8_CR           	0x00080 /* 00_10_0_00_0_00 */
#define GE2D_FMT_S8_R            	0x00000 /* 00_00_0_00_0_00 */
#define GE2D_FMT_S8_G            	0x00040 /* 00_01_0_00_0_00 */
#define GE2D_FMT_S8_B            	0x00080 /* 00_10_0_00_0_00 */
#define GE2D_FMT_S8_A            	0x000c0 /* 00_11_0_00_0_00 */
#define GE2D_FMT_S8_LUT          	0x00020 /* 00_00_1_00_0_00 */
#define GE2D_FMT_S16_YUV422      	0x20100 /* 01_00_0_00_0_00 */
#define GE2D_FMT_S16_RGB         	(GE2D_LITTLE_ENDIAN|0x00100) /* 01_00_0_00_0_00 */
#define GE2D_FMT_S24_YUV444      	0x20200 /* 10_00_0_00_0_00 */
#define GE2D_FMT_S24_RGB         	(GE2D_LITTLE_ENDIAN|0x00200) /* 10_00_0_00_0_00 */
#define GE2D_FMT_S32_YUVA444     	0x20300 /* 11_00_0_00_0_00 */
#define GE2D_FMT_S32_RGBA        	(GE2D_LITTLE_ENDIAN|0x00300) /* 11_00_0_00_0_00 */
#define GE2D_FMT_M24_YUV420      	0x20007 /* 00_00_0_00_1_11 */
#define GE2D_FMT_M24_YUV422      	0x20006 /* 00_00_0_00_1_10 */
#define GE2D_FMT_M24_YUV444      	0x20004 /* 00_00_0_00_1_00 */
#define GE2D_FMT_M24_RGB         	0x00004 /* 00_00_0_00_1_00 */
#define GE2D_FMT_M24_YUV420T     	0x20017 /* 00_00_0_10_1_11 */
#define GE2D_FMT_M24_YUV420B     	0x2001f /* 00_00_0_11_1_11 */
#define GE2D_FMT_S16_YUV422T     	0x20110 /* 01_00_0_10_0_00 */
#define GE2D_FMT_S16_YUV422B     	0x20138 /* 01_00_0_11_0_00 */
#define GE2D_FMT_S24_YUV444T     	0x20210 /* 10_00_0_10_0_00 */
#define GE2D_FMT_S24_YUV444B     	0x20218 /* 10_00_0_11_0_00 */

/* back compatible defines */
#define GE2D_FORMAT_S8_Y            (GE2D_FORMAT_YUV|GE2D_FMT_S8_Y)            
#define GE2D_FORMAT_S8_CB          (GE2D_FORMAT_YUV|GE2D_FMT_S8_CB)          
#define GE2D_FORMAT_S8_CR          (GE2D_FORMAT_YUV|GE2D_FMT_S8_CR)           
#define GE2D_FORMAT_S8_R            GE2D_FMT_S8_R            
#define GE2D_FORMAT_S8_G            GE2D_FMT_S8_G            
#define GE2D_FORMAT_S8_B            GE2D_FMT_S8_B            
#define GE2D_FORMAT_S8_A            GE2D_FMT_S8_A            
#define GE2D_FORMAT_S8_LUT          GE2D_FMT_S8_LUT          
#define GE2D_FORMAT_S16_YUV422      (GE2D_FMT_S16_YUV422  | GE2D_COLOR_MAP_YUV422)
#define GE2D_FORMAT_S16_RGB_655         (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_RGB655)  
#define GE2D_FORMAT_S24_YUV444      (GE2D_FMT_S24_YUV444  | GE2D_COLOR_MAP_YUV444) 
#define GE2D_FORMAT_S24_RGB         (GE2D_FMT_S24_RGB     | GE2D_COLOR_MAP_RGB888)   
#define GE2D_FORMAT_S32_YUVA444     (GE2D_FMT_S32_YUVA444 | GE2D_COLOR_MAP_YUVA4444)   
#define GE2D_FORMAT_S32_RGBA        (GE2D_FMT_S32_RGBA    | GE2D_COLOR_MAP_RGBA8888) 
#define GE2D_FORMAT_M24_YUV420      GE2D_FMT_M24_YUV420    
#define GE2D_FORMAT_M24_YUV422      GE2D_FMT_M24_YUV422
#define GE2D_FORMAT_M24_YUV444      GE2D_FMT_M24_YUV444
#define GE2D_FORMAT_M24_RGB         GE2D_FMT_M24_RGB
#define GE2D_FORMAT_M24_YUV420T     GE2D_FMT_M24_YUV420T
#define GE2D_FORMAT_M24_YUV420B     GE2D_FMT_M24_YUV420B
#define GE2D_FORMAT_S16_YUV422T     (GE2D_FMT_S16_YUV422T | GE2D_COLOR_MAP_YUV422)
#define GE2D_FORMAT_S16_YUV422B     (GE2D_FMT_S16_YUV422B | GE2D_COLOR_MAP_YUV422)   
#define GE2D_FORMAT_S24_YUV444T     (GE2D_FMT_S24_YUV444T | GE2D_COLOR_MAP_YUV444)   
#define GE2D_FORMAT_S24_YUV444B     (GE2D_FMT_S24_YUV444B | GE2D_COLOR_MAP_YUV444)
//format added in A1H
/*16 bit*/
#define GE2D_FORMAT_S16_RGB_565         (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_RGB565) 
#define GE2D_FORMAT_S16_RGB_844         (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_RGB844) 
#define GE2D_FORMAT_S16_RGBA_6442        (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_RGBA6442)
#define GE2D_FORMAT_S16_RGBA_4444        (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_RGBA4444)
#define GE2D_FORMAT_S16_ARGB_4444        (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_ARGB4444)
#define GE2D_FORMAT_S16_ARGB_1555        (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_ARGB1555)
#define GE2D_FORMAT_S16_RGBA_4642        (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_RGBA4642)
/*24 bit*/
#define GE2D_FORMAT_S24_RGBA_5658         (GE2D_FMT_S24_RGB | GE2D_COLOR_MAP_RGBA5658)  
#define GE2D_FORMAT_S24_ARGB_8565         (GE2D_FMT_S24_RGB | GE2D_COLOR_MAP_ARGB8565) 
#define GE2D_FORMAT_S24_RGBA_6666         (GE2D_FMT_S24_RGB | GE2D_COLOR_MAP_RGBA6666)
#define GE2D_FORMAT_S24_ARGB_6666         (GE2D_FMT_S24_RGB | GE2D_COLOR_MAP_ARGB6666)
#define GE2D_FORMAT_S24_BGR        	     	   (GE2D_FMT_S24_RGB | GE2D_COLOR_MAP_BGR888)
/*32 bit*/
#define GE2D_FORMAT_S32_ARGB        (GE2D_FMT_S32_RGBA    | GE2D_COLOR_MAP_ARGB8888) 
#define GE2D_FORMAT_S32_ABGR        (GE2D_FMT_S32_RGBA    | GE2D_COLOR_MAP_ABGR8888) 
#define GE2D_FORMAT_S32_BGRA        (GE2D_FMT_S32_RGBA    | GE2D_COLOR_MAP_BGRA8888) 



//macro used by this lib 

#define    	GE2D_CLASS_NAME   				"ge2d"

#define  	 	GE2D_STRETCHBLIT_NOALPHA_NOBLOCK   	0x4708
#define  		GE2D_BLIT_NOALPHA_NOBLOCK 	0x4707
#define  		GE2D_BLEND_NOBLOCK 	 		0x4706
#define  		GE2D_BLIT_NOBLOCK 	 		0x4705
#define  		GE2D_STRETCHBLIT_NOBLOCK 	0x4704
#define  		GE2D_FILLRECTANGLE_NOBLOCK 	0x4703

#define  	 	GE2D_STRETCHBLIT_NOALPHA   	0x4702
#define  		GE2D_BLIT_NOALPHA	 			0x4701
#define  		GE2D_BLEND			 			0x4700
#define  		GE2D_BLIT    			 		0x46ff
#define  		GE2D_STRETCHBLIT   				0x46fe
#define  		GE2D_FILLRECTANGLE   			0x46fd
#define  		GE2D_SRCCOLORKEY   			0x46fc
#define		GE2D_SET_COEF					0x46fb
#define  		GE2D_CONFIG_EX  			       0x46fa
#define  		GE2D_CONFIG					0x46f9
#define		GE2D_ANTIFLICKER_ENABLE		0x46f8


#define  FBIOPUT_OSD_SRCCOLORKEY		0x46fb
#define  FBIOPUT_OSD_SRCKEY_ENABLE	0x46fa
#define  FBIOPUT_OSD_SET_GBL_ALPHA	0x4500
#define  FBIOGET_OSD_GET_GBL_ALPHA	0x4501
#define  FBIOPUT_OSD_2X_SCALE		0x4502	

#define VB_SIZE         0x100000

#define  U32   		unsigned int 
#define  U16   		unsigned short 
#define  U8			unsigned char
#define  INT32		int
#define	INT16		short
#define  INT8S		char	
#define  UINT8 		U8
#define  UINT16		U16
#define  UINT32		U32
	
#define  swap(x,y,t)     {(t)=(x);(x)=(y);(y)=(t);}
#ifndef min
#define	min(a,b)	(((a) < (b)) ? (a) : (b))
#endif
#define 	ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))



typedef struct {
     int            x;   /* X coordinate of its top-left point */
     int            y;   /* Y coordinate of its top-left point */
     int            w;   /* width of it */
     int            h;   /* height of it */
}osd_rectangle_t;

typedef  struct {
	U32   color ;
	osd_rectangle_t src1_rect;
	osd_rectangle_t src2_rect;
	osd_rectangle_t dst_rect;
}osd_para_t ;

typedef   struct {
	U32	 disp_start_x;
	U32	 disp_start_y;
}disp_offset_t;


typedef  enum {
	OSD_TYPE_16_565 =16,
	OSD_TYPE_16_844 =10,
	OSD_TYPE_16_6442 =11 ,
	OSD_TYPE_16_4444_R = 12,
	OSD_TYPE_16_4642_R = 13,
	OSD_TYPE_16_1555_A=14,
	OSD_TYPE_16_4444_A = 15,
	OSD_TYPE_16_655 =9,
	
	OSD_TYPE_24_6666_A=19,
	OSD_TYPE_24_6666_R=20,
	OSD_TYPE_24_8565 =21,
	OSD_TYPE_24_5658 = 22,
	OSD_TYPE_24_888_B = 23,
	OSD_TYPE_24_RGB = 24,

	OSD_TYPE_32_BGRA=29,
	OSD_TYPE_32_ABGR = 30,
	OSD_TYPE_32_ARGB=32,
	OSD_TYPE_32_RGBA=31,
}osd_disp_type_t;
typedef enum  	
{
		OSD0_OSD0 =0,
		OSD0_OSD1,	 
		OSD1_OSD1,
		OSD1_OSD0,
		TYPE_INVALID,
}ge2d_src_dst_t;

enum   GE2D_OP_TYPE{
	GE2D_OP_FILL_RECT=1,
	GE2D_OP_BLIT,
	GE2D_OP_STRETCH_BLIT,
	GE2D_OP_BLEND,
	GE2D_MAX_OP_NUMBER
};

typedef enum  	
{
    CANVAS_OSD0 =0,
    CANVAS_OSD1,	 
    CANVAS_ALLOC,
    CANVAS_TYPE_INVALID,
}ge2d_src_canvas_type;

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
}config_para_t;

typedef  struct  {
    int  canvas_index;
    int  top;
    int  left;
    int  width;
    int  height;
    int  format;
    int  mem_type;
    int  color;
    unsigned char x_rev;
    unsigned char y_rev;
    unsigned char fill_color_en;
    unsigned char fill_mode;    
}src_dst_para_ex_t ;

typedef    struct {
    src_dst_para_ex_t src_para;
    src_dst_para_ex_t src2_para;
    src_dst_para_ex_t dst_para;

//key mask
    src_key_ctrl_t  src_key;
    src_key_ctrl_t  src2_key;

    int alu_const_color;
    unsigned src1_gb_alpha;
    unsigned op_mode;
    unsigned char bitmask_en;
    unsigned char bytemask_only;
    unsigned int  bitmask;
    unsigned char dst_xy_swap;

// scaler and phase releated
    unsigned hf_init_phase;
    int hf_rpt_num;
    unsigned hsc_start_phase_step;
    int hsc_phase_slope;
    unsigned vf_init_phase;
    int vf_rpt_num;
    unsigned vsc_start_phase_step;
    int vsc_phase_slope;
    unsigned char src1_vsc_phase0_always_en;
    unsigned char src1_hsc_phase0_always_en;
    unsigned char src1_hsc_rpt_ctrl;  //1bit, 0: using minus, 1: using repeat data
    unsigned char src1_vsc_rpt_ctrl;  //1bit, 0: using minus  1: using repeat data

//canvas info
    config_planes_t src_planes[4];
    config_planes_t src2_planes[4];
    config_planes_t dst_planes[4];
}config_para_ex_t;
typedef config_para_t alloc_config_t  ;
/*
typedef    struct {
	ge2d_src_dst_t  op_type;
	int  alu_const_color;	
}config_para_t;
*/
typedef  struct {
	alloc_config_t  config;
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
	rectangle_t	dst_rect;
	int op;
}ge2d_op_para_t ;





typedef  int	(*func_test_t)(void *);

typedef  struct {
	int	(*fill_rect)(void* para);
	int	(*blit)(void* para);
	int	(*stretch_blit)(void* para);
	int	(*blend)(void *para);
}ge2d_op_func_t;

typedef  struct {
	char 	name[40];
	int		osd_index;
	int		bpp;
	int		line_width;
	int 		screen_size;
	int 		fbfd ;
	int 		ge2d_fd;
	char		*fbp;
	struct fb_var_screeninfo *vinfo ;
	struct fb_var_screeninfo *save_vinfo;
	struct fb_fix_screeninfo *finfo ;
	int	(*draw)(void* para);
	int 	(*show)(void *para);
	ge2d_op_func_t  ge2d_op_func;
	void  *para ;
}osd_obj_t ;

typedef  struct {
	INT32	handle;
	U8*		ptr;
	U32		fix_line_length;
	U32		xres;
	U32		yres;
	U32		bpp;
}osd_info_t ;

typedef struct T_DEC_PARAMETER
{
	INT32    			i32DecoderID;	 
	UINT8 			u8StreamType;	 
 	UINT32 			u32ACodeType;	 
 	UINT32 			u32VCodeType;	 
	UINT16 			u16VPID;		 
	UINT16 			u16APID;		 
}T_DEC_PARA;
enum   STREAM_TYPE{
	STREAM_ES,
	STREAM_TS,
	STREAM_PS
	};
enum  AUDIO_TYPE{
	A_MPG=1,
	};
enum  VIDEO_TYPE{
	V_MPG1=1,
	V_MPG2,
	V_MPG4,	
	};
#pragma pack(1)
typedef struct tagBITMAPFILEHEADER
{
	U16	bfType;  //BM
	U32 bfSize;   //file size 
	U16	bfReserved1;  //0
	U16	bfReserved2;  //0
	U32 bfOffBits;  	//bitmap data offset to  file header	
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
	U32 biSize;  //this structure occupy size 
	U32	biWidth; //bitmap width (pixel unit)
	U32 biHeight; //bitmap height (pixel unit)
	U16 biPlanes; // 1
	U16 biBitCount;//bits of erery pixel . must be one of follow values 1(double color)
			// 4(16color) 8(256 color) 24(true color)
	U32 biCompression; // bitmap compresstion type must be one of 0 (uncompress)
					  // 1(BI_RLE8) 2(BI_RLE4)
	U32 biSizeImage; 	// bitmap size 
	U32	biXPelsPerMeter; //bitmap horizontal resolution.
	U32	biYPelsPerMeter; //bitmap vertical resolution.
	U32 biClrUsed;	//bitmap used color number.
	U32 biClrImportant;//bitmap most important color number during display process.
} BITMAPINFOHEADER;
#pragma pack()
typedef  enum {
	COLOR_INDEX_02_PAL4    = 2,  // 0
    	COLOR_INDEX_04_PAL16   = 4, // 0
	COLOR_INDEX_08_PAL256=8,
	COLOR_INDEX_16_655 =9,
	COLOR_INDEX_16_844 =10,
	COLOR_INDEX_16_6442 =11 ,
	COLOR_INDEX_16_4444_R = 12,
	COLOR_INDEX_16_4642_R = 13,
	COLOR_INDEX_16_1555_A=14,
	COLOR_INDEX_16_4444_A = 15,
	COLOR_INDEX_16_565 =16,
	
	COLOR_INDEX_24_6666_A=19,
	COLOR_INDEX_24_6666_R=20,
	COLOR_INDEX_24_8565 =21,
	COLOR_INDEX_24_5658 = 22,
	COLOR_INDEX_24_888_B = 23,
	COLOR_INDEX_24_RGB = 24,

	COLOR_INDEX_32_BGRA=29,
	COLOR_INDEX_32_ABGR = 30,
	COLOR_INDEX_32_RGBA=31,
	COLOR_INDEX_32_ARGB=32,

	COLOR_INDEX_YUV_422=33,
	
}color_index_t;
typedef  unsigned char u8  ;
typedef  struct {
	u8	red_offset ;
	u8	red_length;
	u8	red_msb_right;
	
	u8	green_offset;
	u8	green_length;
	u8	green_msb_right;

	u8	blue_offset;
	u8	blue_length;
	u8	blue_msb_right;

	u8	transp_offset;
	u8	transp_length;
	u8	transp_msb_right;

	u8	color_type;
	u8	bpp;

		
	
}color_bit_define_t;
#define  INVALID_BPP_ITEM    {0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define FB_VISUAL_PSEUDOCOLOR		3	/* Pseudo color (like atari) */
#define FB_VISUAL_TRUECOLOR		2	/* True color	*/
static const  color_bit_define_t   default_color_format_array[]={
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{/*red*/ 0,2,0,/*green*/0,2,0,/*blue*/0,2,0,/*trans*/0,0,0,FB_VISUAL_PSEUDOCOLOR,2},
	INVALID_BPP_ITEM,	
	{/*red*/ 0,4,0,/*green*/0,4,0,/*blue*/0,4,0,/*trans*/0,0,0,FB_VISUAL_PSEUDOCOLOR,4},
	INVALID_BPP_ITEM,	
	INVALID_BPP_ITEM,	
	INVALID_BPP_ITEM,	
	{/*red*/ 0,8,0,/*green*/0,8,0,/*blue*/0,8,0,/*trans*/0,0,0,FB_VISUAL_PSEUDOCOLOR,8},
/*16 bit color*/
	{/*red*/ 10,6,0,/*green*/5,5,0,/*blue*/0,5,0,/*trans*/0,0,0,FB_VISUAL_TRUECOLOR,16},
	{/*red*/ 8,8,0,/*green*/4,4,0,/*blue*/0,4,0,/*trans*/0,0,0,FB_VISUAL_TRUECOLOR,16},
	{/*red*/ 10,6,0,/*green*/6,4,0,/*blue*/2,4,0,/*trans*/0,2,0,FB_VISUAL_TRUECOLOR,16},
	{/*red*/ 12,4,0,/*green*/8,4,0,/*blue*/4,4,0,/*trans*/0,4,0,FB_VISUAL_TRUECOLOR,16,},
	{/*red*/ 12,4,0,/*green*/6,6,0,/*blue*/2,4,0,/*trans*/0,2,0,FB_VISUAL_TRUECOLOR,16},
	{/*red*/ 10,5,0,/*green*/5,5,0,/*blue*/0,5,0,/*trans*/15,1,0,FB_VISUAL_TRUECOLOR,16},
	{/*red*/ 8,4,0,/*green*/4,4,0,/*blue*/0,4,0,/*trans*/12,4,0,FB_VISUAL_TRUECOLOR,16},
	{/*red*/ 11,5,0,/*green*/5,6,0,/*blue*/0,5,0,/*trans*/12,4,0,FB_VISUAL_TRUECOLOR,16},
/*24 bit color*/
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{/*red*/ 12,6,0,/*green*/6,6,0,/*blue*/0,6,0,/*trans*/18,6,0,FB_VISUAL_TRUECOLOR,24},
	{/*red*/ 18,6,0,/*green*/12,6,0,/*blue*/6,6,0,/*trans*/0,6,0,FB_VISUAL_TRUECOLOR,24},
	{/*red*/ 11,5,0,/*green*/5,6,0,/*blue*/0,5,0,/*trans*/16,8,0,FB_VISUAL_TRUECOLOR,24},
	{/*red*/ 19,5,0,/*green*/13,6,0,/*blue*/8,5,0,/*trans*/0,8,0,FB_VISUAL_TRUECOLOR,24},
	{/*red*/ 0,8,0,/*green*/8,8,0,/*blue*/16,8,0,/*trans*/0,0,0,FB_VISUAL_TRUECOLOR,24},
	{/*red*/ 16,8,0,/*green*/8,8,0,/*blue*/0,8,0,/*trans*/0,0,0,FB_VISUAL_TRUECOLOR,24},
/*32 bit color*/
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{/*red*/ 8,8,0,/*green*/16,8,0,/*blue*/24,8,0,/*trans*/0,8,0,FB_VISUAL_TRUECOLOR,32},
	{/*red*/ 0,8,0,/*green*/8,8,0,/*blue*/16,8,0,/*trans*/24,8,0,FB_VISUAL_TRUECOLOR,32},
	{/*red*/ 24,8,0,/*green*/16,8,0,/*blue*/8,8,0,/*trans*/0,8,0,FB_VISUAL_TRUECOLOR,32},
	{/*red*/ 16,8,0,/*green*/8,8,0,/*blue*/0,8,0,/*trans*/24,8,0,FB_VISUAL_TRUECOLOR,32},
/*YUV color*/
	{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,16},
};

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
 INT32 DecData(INT32 i32DecoderID, UINT8* pData, INT32 nLen) ;
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
INT32 DecInit(INT32 i32PlayerID, void *pDecoderPara);

/****************************************************
*
*     func name 	:  osdOpen   
*     	 desp  	:  get osd layer handle  
*     	 para  	:  index  0 : osd1  1:osd2 
*	return value	:  >0  osd handle  
*			   	   <0  fail 
*
*****************************************************/
INT32 osdOpen(INT32   index );
/****************************************************
*
*     func name 	:  osdClose   
*     	 desp  	:  close one osd layer by it's handle  
*     	 para  	:  osdHandle   
*	return value	:    0   success  
*			   	   -1   fail 
*
*****************************************************/
INT32 osdClose(INT32   osdHandle );
/****************************************************
*
*     func name 	:  osdOn   
*     	 desp  	:  switch on one osd layer   
*     	 para  	:  osdHandle   
*	return value	:    0   success  
*			   	   -1   fail 
*
*****************************************************/
INT32 osdOn(INT32  osdHandle) ;
/****************************************************
*
*     func name 	:  osdOff   
*     	 desp  	:  switch off one osd layer . 
*     	 para  	:  osdHandle   
*	return value	:    0   success  
*			   	   -1   fail 
*
*****************************************************/
INT32 osdOff(INT32  osdHandle) ;
INT32 osdSetColorKey(osd_obj_t* osd_this,INT32 iColorKey);

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
INT32 osdDispRect(osd_obj_t *  osd_this,U32  argb,U16 x0,U16 y0,U16 x1,U16 y1) ;
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
INT32 osdEraseRect(osd_obj_t* osd_this,U16 x0,U16 y0,U16 x1,U16 y1);

/****************************************************
*
*     func name 	:  osdInit   
*     	 desp  	:  osd layer init   
*     	 para  	:  osdHandle
*	return value	:  0  SUCCESS 
*			   	   <0  fail 
*
*****************************************************/
INT32 osdInit(INT32 osdHandle,U8 bpp);
/****************************************************
*
*     func name 	:  UninitOsd   
*     	 desp  	:  osd layer UNinit   
*     	 para  	:  osdHandle
*	return value	:  0  SUCCESS 
*			   	   <0  fail 
*
*****************************************************/
INT32 UninitOsd(INT32 osdHandle);
extern  int  test_osd_display(void* para);
extern osd_obj_t*  create_osd_obj(int mode,int osd_index);
extern  int  test_ge2d_op(void* para);
extern int 	test_osd_gbl_alpha(void* para);	
extern int 	test_osd_colorkey(void* para);
extern int destroy_osd_obj(osd_obj_t* osd_obj) ;
//ge2d_op
extern int ge2d_op_fill_rect(void *para) ;
extern int ge2d_op_blit(void *para) ;
extern int ge2d_op_stretch_blit(void *para) ;
 extern int ge2d_op_blend(void *para) ;
 extern  int   osd_order_change(void* para);
extern  int   multi_process_test(void* para);
 //osd
 extern  int  show_bitmap(char *p, unsigned line_width, unsigned xres, unsigned yres, unsigned bpp);
#endif

