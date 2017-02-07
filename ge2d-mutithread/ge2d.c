#include "osd.h"
#include <linux/input.h>
#include <sys/poll.h>
#include	<pthread.h>
#include <sys/time.h> 
#include <signal.h>
#include "cmemlib.h"



#define    KEYBOARD_DEV     "/dev/input/event0"
#define		GE2D_KEY_UP   		0x67
#define		GE2D_KEY_DOWN		0x6c
#define		GE2D_KEY_LEFT   		0x69
#define		GE2D_KEY_RIGHT		0x6a
#define 		GE2D_KEY_PULL		0x42
#define  		GE2D_KEY_PUSH		0x43
#define 		GE2D_KEY_EXIT		0x74
#define 		INVALID_THREAD_ID  ((pthread_t)(-1))
#define 		MAX_THREAD_NUM	10

#define		THREAD_DRAW	0
#define		THREAD_INPUT	1

	


typedef   struct {
	pthread_mutex_t  mutex;
	int				int_exit;
	pthread_cond_t    cond_exit;
	pthread_cond_t 	cond_draw;
	osd_obj_t  * 		osd;
	int				key_code;
	pthread_t  pthread_id[MAX_THREAD_NUM] ;
}ge2d_context_t ;
typedef void (*sighandler_t)(int); 



ge2d_context_t  thread_context;
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

int ge2d_op_fill_rect(void *para)
{
	ge2d_op_para_t  op_para;
	int r=0xff;
	int a=0xff ;
	ge2d_op_ctl_t *control=(ge2d_op_ctl_t*)para;
	osd_obj_t *this_osd=(osd_obj_t *)control->pt_osd;

	
	ioctl( this_osd->ge2d_fd, GE2D_CONFIG, &control->config) ;
	op_para.src1_rect.x=0;
	op_para.src1_rect.y=0;
	op_para.src1_rect.w=1200;
	op_para.src1_rect.h=720;
	//RGBA
	 op_para.color=0xff0000ff;	
	 
	
	ioctl( this_osd->ge2d_fd, GE2D_FILLRECTANGLE, &op_para) ;
	return 0;
}
int ge2d_op_blend(void *para)
{
	
	config_para_t ge2d_config;
	osd_obj_t **osd=(osd_obj_t **)para;
	ge2d_op_para_t  op_para;
	osd_obj_t  *osd0=*osd++,*osd1=*osd;

	ge2d_config.op_type=OSD0_OSD0;
	ge2d_config.alu_const_color=0x0000ffff;
	ioctl( osd0->ge2d_fd, GE2D_CONFIG, &ge2d_config) ;
	op_para.src1_rect.x=100;
	op_para.src1_rect.y=100;
	op_para.src1_rect.w=100;
	op_para.src1_rect.h=100;

	op_para.src2_rect.x=200;
	op_para.src2_rect.y=200;
	op_para.src2_rect.w=100;
	op_para.src2_rect.h=100;

	op_para.dst_rect.x=600;
	op_para.dst_rect.y=300;
	op_para.dst_rect.w=400;
	op_para.dst_rect.h=400;


	op_para.op=blendop( 
			OPERATION_ADD,
                    /* color blending src factor */
                    COLOR_FACTOR_ONE,
                    /* color blending dst factor */
                    COLOR_FACTOR_ONE,
                    /* alpha blending_mode */
                    OPERATION_MAX,
                    /* alpha blending src factor */
                    ALPHA_FACTOR_ONE,
                    /* color blending dst factor */
                    ALPHA_FACTOR_ONE);
	ioctl( osd0->ge2d_fd, GE2D_BLEND, &op_para) ;
	return 0;
	
}
int ge2d_op_blit(void *para)
{
	config_para_t ge2d_config;
	osd_obj_t **osd=(osd_obj_t **)para;
	ge2d_op_para_t  op_para;
	int move_mode;
	osd_obj_t  *osd0=*osd++,*osd1=*osd;

	printf("blit type \n"  \
		  "1	move block from osd1 to osd0 \n" \
		  "2	move block from osd0 to osd0 \n" \
		  "please input blit type number: ");
	scanf("%d",&move_mode);
	if(move_mode==1)
	{
		ge2d_config.op_type=OSD1_OSD0;
	}
	else
	{
		ge2d_config.op_type=OSD0_OSD0;
	}
	ge2d_config.alu_const_color=0xff0000ff;
	ioctl( osd0->ge2d_fd, GE2D_CONFIG, &ge2d_config) ;

	op_para.src1_rect.x=100;
	op_para.src1_rect.y=100;
	op_para.src1_rect.w=380;
	op_para.src1_rect.h=380;

	op_para.dst_rect.x=200;
	op_para.dst_rect.y=300;
	op_para.dst_rect.w=380;
	op_para.dst_rect.h=380;

	
	ioctl( osd0->ge2d_fd, GE2D_BLIT, &op_para) ;
	ioctl(osd0->fbfd, FBIOPUT_OSD_SET_GBL_ALPHA, 0x80);
	return 0;
}
int ge2d_op_stretch_blit(void *para)
{
	config_para_t ge2d_config;
	osd_obj_t **osd=(osd_obj_t **)para;
	ge2d_op_para_t  op_para;
	int stretch_mode;
	osd_obj_t  *osd0=*osd++,*osd1=*osd;
	
	printf("blit type \n"  \
		  "1	stretch  block from osd1 to osd0 \n" \
		  "2	stretch  block from osd0 to osd0 \n" \
		  "please input blit type number: ");
	scanf("%d",&stretch_mode);
	if(stretch_mode==1)
	{
		ge2d_config.op_type=OSD1_OSD0;
	}
	else
	{
		ge2d_config.op_type=OSD0_OSD0;
	}
	ge2d_config.alu_const_color=0xff0000ff;
	ioctl( osd0->ge2d_fd, GE2D_CONFIG, &ge2d_config) ;
	
	op_para.src1_rect.x=50;
	op_para.src1_rect.y=50;
	op_para.src1_rect.w=100;
	op_para.src1_rect.h=100;

	op_para.dst_rect.x=150;
	op_para.dst_rect.y=150;
	op_para.dst_rect.w=200;
	op_para.dst_rect.h=200;
	ioctl( osd0->ge2d_fd, GE2D_STRETCHBLIT, &op_para) ;
	/*
	op_para.src1_rect.x=0;
	op_para.src1_rect.y=0;
	op_para.src1_rect.w=osd1->vinfo->xres;
	op_para.src1_rect.h=osd1->vinfo->yres;

	op_para.dst_rect.x=0;
	op_para.dst_rect.y=0;
	op_para.dst_rect.w=osd1->vinfo->xres;
	op_para.dst_rect.h=osd1->vinfo->yres;
	
	while(1)
	{
	printf("press enter :\r\n");
	scanf("%c",&stretch_mode,&stretch_mode);
	ge2d_config.op_type=ge2d_config.op_type < 2 ? 3 : 1; 
	ioctl( osd0->ge2d_fd, GE2D_CONFIG, &ge2d_config) ;
	
	}*/
	return 0;
}
void  display_ge2d_op_menu(void)
{
	printf("\nge2d_op menu : \n"
	"	1	fill rect, \n" 		\
	"	2	blit\n" 			\
	"	3	stretch blit \n"	\
	"	4	blend	\n"		\
	"	5	key test \n"
	"	6	scale test \n"
	"	7	3d test \n" \
	"	8	cmem test\n"
	"please input menu item number: ");
}

static  int   key_is_pressed(int key_fd)
{

	int rc;
      	struct pollfd	tPfd;
      	struct input_event read_event;
   	tPfd.fd			= key_fd;
	tPfd.events		= POLLIN;
	tPfd.revents		= 0;
	rc	=	poll(&tPfd,1, 0);
	if (rc != 0)
	{
		if (tPfd.revents & POLLIN)
		{
			rc = read (key_fd,&read_event, sizeof(read_event));
			//printf("key:0x%06x, %d\n", read_event.code, read_event.value);
			if(read_event.value==1)
			{
				printf("code:0x%x\r\n", read_event.code);
				return  read_event.code;
			}
			return 0;
		}
	}
	return  0;
		 
}
#define		KEY_NUM_H 		11
#define		KEY_NUM_V		11	
typedef  struct{
	char   index;
	int     x ;
	int 	  y;
	int 	  w;
	int     h;
}key_prop_t;
static const  int   key_width=50,key_height=30 ;
static const  int   focus_border=4,key_array_gap_h=10 ,key_array_gap_v=10;
static const  int   key_array_start_x=50, key_array_start_y=50;
static key_prop_t  key_pos[KEY_NUM_V][KEY_NUM_H];
static int 	focus_key_index=0;


int  init_key_array(osd_obj_t  * osd_this)
{
	int i,j;
	int  index=0;
	
	memset((char*)key_pos,0,sizeof(int)*KEY_NUM_H*KEY_NUM_V);
	for(i=0;i<KEY_NUM_V;i++)//initialize key array position.
	{
		for(j=0;j<KEY_NUM_H;j++)
		{
			key_pos[i][j].x=key_array_start_x + (key_width+key_array_gap_h)*j;
			key_pos[i][j].y=key_array_start_y + (key_height+key_array_gap_v)*i;
			key_pos[i][j].w=key_width;
			key_pos[i][j].h=key_height;
			key_pos[i][j].index=index;
			index ++;
		}
	}
	
	for(i=0;i<KEY_NUM_V;i++)//initialize key array position.
	{
		for(j=0;j<KEY_NUM_H;j++)
		{
			osdDispRect(osd_this,0xff00ffff,key_pos[i][j].x,key_pos[i][j].y,key_pos[i][j].x+key_pos[i][j].w,key_pos[i][j].y+key_pos[i][j].h);
		}
	}	
	focus_key_index=0;
	
	return 0;
}
int  slide_window(osd_obj_t  * osd_this,int status)
{
	
	switch (status)
	{
		case GE2D_KEY_PUSH:
		if(osd_this->vinfo->yoffset < 500)
		{
			osd_this->vinfo->yoffset+=10;
		}
		break;
		case GE2D_KEY_PULL:
		if(osd_this->vinfo->yoffset >10)
		{
			osd_this->vinfo->yoffset-=10;
		}	
		break;	
	}
	ioctl(osd_this->fbfd,FBIOPAN_DISPLAY,osd_this->vinfo);
}
void * random_thread(void *para)
{
	ge2d_context_t* ge2d_context=para;
	
	printf("enter random thread\r\n");
	while(!ge2d_context->int_exit)
	{
		ge2d_context->key_code=	rand()%7 +GE2D_KEY_UP;
		pthread_cond_signal(&ge2d_context->cond_draw);
		//usleep(100000);
	}
}
int  test_keyboard(ge2d_context_t* ge2d_context)
{
	static  int  id_num=THREAD_INPUT;
	int ret ;
	id_num++;
	printf("+++++create thread 0x%x\r\n",id_num);
	ret=pthread_create(&ge2d_context->pthread_id[id_num],NULL,random_thread,ge2d_context);
	if(ret)
	{
		printf("error create thread%d:%s\r\n",id_num,strerror(errno));
		return -1;
	}
	printf("create thread%d success \r\n",id_num);
	return 0;
}

void  *ge2d_draw(void *para)
{
	 struct   timeval   now;   
  	 struct   timespec   tm;  
	 ge2d_context_t* ge2d_context=para;
	 int 		i,j;
	 int 		ret;
	 printf("enter draw thread\r\n");;
	 init_key_array(ge2d_context->osd);
	 printf("enter draw thread2\r\n");;
	 while (1)
	 {
		pthread_mutex_lock(&ge2d_context->mutex);
		gettimeofday(&now,NULL);
		tm.tv_sec=now.tv_sec;
		tm.tv_nsec=now.tv_usec*1000 + 30000000;//30ms
		ret=pthread_cond_timedwait(&ge2d_context->cond_draw,&ge2d_context->mutex,&tm);
		if(!ret) //condition signaled.
		{
			if(ge2d_context->osd)
			{
				i=focus_key_index/KEY_NUM_H,j=focus_key_index%KEY_NUM_H;
				switch(ge2d_context->key_code)
				{
					case GE2D_KEY_UP:
					//printf("^\r\n");
					if (focus_key_index > KEY_NUM_H - 1)
					{
						focus_key_index -=KEY_NUM_H ;
					}
					break;
					case GE2D_KEY_DOWN:
					//printf("V\r\n");
					if (focus_key_index <= (KEY_NUM_V-1)*KEY_NUM_H-1)
					{
						focus_key_index += KEY_NUM_H;
					}
					break;
					case GE2D_KEY_LEFT:
					//printf("<\r\n");
					if (focus_key_index %KEY_NUM_H != 0)
					{
						focus_key_index --;
					}
					break;
					case GE2D_KEY_RIGHT:
					//printf(">\r\n");
					if (focus_key_index %KEY_NUM_H != KEY_NUM_H-1)
					{
						focus_key_index ++;
					}
					break;
				}
	
				osdDispRect(ge2d_context->osd,0xff00ffff,key_pos[i][j].x - focus_border ,key_pos[i][j].y- focus_border,key_pos[i][j].x+key_pos[i][j].w+focus_border,key_pos[i][j].y+key_pos[i][j].h+focus_border) ;
			 	i=focus_key_index/KEY_NUM_H,j=focus_key_index%KEY_NUM_H;
	 			osdDispRect(ge2d_context->osd,0xffff0000,key_pos[i][j].x - focus_border ,key_pos[i][j].y- focus_border,key_pos[i][j].x+key_pos[i][j].w+focus_border,key_pos[i][j].y+key_pos[i][j].h+focus_border) ;
	 			osdDispRect(ge2d_context->osd,0xff00ffff,key_pos[i][j].x,key_pos[i][j].y,key_pos[i][j].x+key_pos[i][j].w,key_pos[i][j].y+key_pos[i][j].h);
			}
		}
		pthread_mutex_unlock(&ge2d_context->mutex);
	}
	 
}
void  stop_all_thread(ge2d_context_t* ge2d_context)
{
	int  i;
	for (i=0;i<MAX_THREAD_NUM;i++)
	{
		if(ge2d_context->pthread_id[i]==INVALID_THREAD_ID)
		{
			break;
		}
		else{
			pthread_cancel(ge2d_context->pthread_id[i]) ;
			ge2d_context->pthread_id[i]=INVALID_THREAD_ID;
		}	
	}
	ge2d_context->int_exit=1;
	printf("all thread has been stopped\r\n");
}
void *ge2d_reap_thread(void *para)
{
	ge2d_context_t* ge2d_context=para;
	printf("enter reap  thread\r\n");
	pthread_mutex_lock(&ge2d_context->mutex);
	pthread_cond_wait(&ge2d_context->cond_exit,&ge2d_context->mutex);
	pthread_mutex_unlock(&ge2d_context->mutex);
	printf("reap all thread\r\n");
	stop_all_thread(ge2d_context);
	
}
void  *ge2d_input(void *para)
{
	int key_fd;
	int exit =0;
	ge2d_context_t* ge2d_context=para;
	union sigval value ;
	printf("enter input thread\r\n");;
	key_fd=open(KEYBOARD_DEV,O_RDONLY);
	if(key_fd<0) 
	{
		printf("error open keyboard dev\r\n",key_fd);
		return NULL;
	}
 	printf("enter input thread2\r\n");;
	
	while(!exit)
	{
		int status=key_is_pressed(key_fd);
		switch(status)
		{
			case  0:
			//printf("error key down\r\n");
			break;
			case GE2D_KEY_UP:
			case GE2D_KEY_DOWN:
			case GE2D_KEY_LEFT:
			case GE2D_KEY_RIGHT:
			printf("++++++++++++++++++one key in\r\n");	
			ge2d_context->key_code=	status;
			pthread_cond_signal(&ge2d_context->cond_draw);
			break;
			case GE2D_KEY_EXIT:
			pthread_cond_signal(&ge2d_context->cond_exit);
			exit=1;
			break;
			case GE2D_KEY_PUSH:
			case GE2D_KEY_PULL:
			if(ge2d_context->osd)
			slide_window(ge2d_context->osd,status);
			break;
		}
		usleep(10000);
	}
	ge2d_context->pthread_id[THREAD_INPUT]=INVALID_THREAD_ID;
	return NULL;
}
void  ge2d_handle_quit(int sig_num)
{
	printf("signal number %d\r\n",sig_num) ;
	if(sig_num==SIGINT)
	{
		printf("right, got it \r\n");
		thread_context.int_exit=1;
	}
	return;
}
static int  scale_test(osd_obj_t **  osd)
{
	unsigned int  osd1_scale,osd2_scale;
	printf("please input scale  para for osd1\n");
	printf("\texamble	0x10001 ==>bit[16..31] h_scale,bit[0..15]v_scale\n");
	scanf("%x",&osd1_scale);
	printf("please input scale  para for osd2\n");
	scanf("%x",&osd2_scale);

	ioctl( osd[0]->fbfd, FBIOPUT_OSD_2X_SCALE, osd1_scale) ;
	ioctl( osd[1]->fbfd, FBIOPUT_OSD_2X_SCALE, osd2_scale) ;	

}
static int test_3d(osd_obj_t **  osd)
{
	int enable;
	printf("please input 3d enable\n");
	scanf("%d",&enable);
	ioctl( osd[0]->fbfd, 0x4503, enable) ; // 3d test
	return 0;
}
static int cmem_fillrect(unsigned int ge2d_fd,
						unsigned int  mem_type,
						unsigned int width,		//mem width
						unsigned int height,	//mem_height
						unsigned int format ,
						unsigned int addr, 
						osd_rectangle_t *rect,
						unsigned int color)
{
	config_para_ex_t  ge2d_config_ex;
	ge2d_op_para_t  op_para;
	
		memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));  

			
		ge2d_config_ex.src_para.mem_type=mem_type;
		ge2d_config_ex.src_para.format  = format;
		ge2d_config_ex.src_planes[0].addr=addr;
		ge2d_config_ex.src_planes[0].w=width;
		ge2d_config_ex.src_planes[0].h=height;

		ge2d_config_ex.src_para.color = 0;
    		ge2d_config_ex.src_para.top = 0;
    		ge2d_config_ex.src_para.left = 0;
    		ge2d_config_ex.src_para.width = width;
    		ge2d_config_ex.src_para.height = height;

	       ge2d_config_ex.src2_para.mem_type = CANVAS_TYPE_INVALID;

 		ge2d_config_ex.dst_planes[0].addr=addr;
		ge2d_config_ex.dst_planes[0].w=width;
		ge2d_config_ex.dst_planes[0].h=height;
    		ge2d_config_ex.dst_para.mem_type = mem_type;
    		ge2d_config_ex.dst_para.format  = format;
 	    	ge2d_config_ex.dst_para.width = width;
 	    	ge2d_config_ex.dst_para.height = height;
		ioctl(ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);


    		memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
    		op_para.src1_rect.x = rect->x;
    		op_para.src1_rect.y = rect->y;
    		op_para.src1_rect.w = rect->w;
    		op_para.src1_rect.h = rect->h;

    		op_para.color = color;
    		ioctl(ge2d_fd, GE2D_FILLRECTANGLE, &op_para); 
}
static int test_cmem(osd_obj_t **  osd,int  format)
{

#define  PLANE_WIDTH	800 
#define  PLANE_HEIGHT 	600
#define  COLOR_DECADE_STEP   10
		unsigned char *planes[6];
		alloc_config_t  config;
		int  addr_y,addr_u,addr_v,addr_rgb;
		CMEM_AllocParams cmemParm = {CMEM_HEAP, CMEM_NONCACHED, 8};
		config_para_ex_t ge2d_config_ex;
		ge2d_op_para_t  op_para;
		float  x_ratio,y_ratio;
		unsigned int 	ge2d_yuv_format,color,color_step;
		osd_rectangle_t   rect;
		int i;
		ge2d_yuv_format=format;
		switch(format)
		{
			case 8:
			ge2d_yuv_format=GE2D_FORMAT_M24_YUV420;
			x_ratio=0.5;
			y_ratio=0.5;
			break;
			case 9:
			ge2d_yuv_format=GE2D_FORMAT_M24_YUV422;
			x_ratio=0.5;
			y_ratio=1;
			break;	
			case 10:
			ge2d_yuv_format=GE2D_FORMAT_M24_YUV444;	
			x_ratio=1;
			y_ratio=1;
			break;
			default:
			ge2d_yuv_format=GE2D_FORMAT_M24_YUV444;	
			x_ratio=1;
			y_ratio=1;
			break;
				
		}
		CMEM_init();
		//request yuv plane .
		planes[0]=(unsigned char *)CMEM_alloc(0, 
					 	PLANE_WIDTH * PLANE_HEIGHT, &cmemParm);
		planes[1]=(unsigned char *)CMEM_alloc(0, 
					 	PLANE_WIDTH * PLANE_HEIGHT, &cmemParm);	
		planes[2]=(unsigned char *)CMEM_alloc(0, 
					 	PLANE_WIDTH * PLANE_HEIGHT, &cmemParm);	
		addr_y = CMEM_getPhys(planes[0]);
		addr_u = CMEM_getPhys(planes[1]);
		addr_v = CMEM_getPhys(planes[2]); 
		
		//request another rgb32 space 
		planes[3]=(unsigned char *)CMEM_alloc(0, 
					 	PLANE_WIDTH * PLANE_HEIGHT*4, &cmemParm);
		addr_rgb=CMEM_getPhys(planes[3]);
		printf("virtual: 0x%x 0x%x 0x%x 0x%x\n",planes[0],planes[1],planes[2],planes[3]);
		printf("request y=0x%x u=0x%x v=0x%x\n\t rgb=0x%x\n",addr_y,addr_u,addr_v,addr_rgb);	
/**************************************************************
**
**		step  1 :fill rect in rgb space 
**
***************************************************************/
		rect.x=0,rect.y=0,rect.w=COLOR_DECADE_STEP,rect.h=PLANE_HEIGHT;
		color=0xff;
		color_step=0xff*COLOR_DECADE_STEP/PLANE_WIDTH;
		for (i=0;i<PLANE_WIDTH;i+=COLOR_DECADE_STEP,rect.x+=COLOR_DECADE_STEP)
 		{
			cmem_fillrect(osd[0]->ge2d_fd,CANVAS_ALLOC,PLANE_WIDTH,PLANE_HEIGHT,GE2D_FORMAT_S32_ARGB,addr_rgb,&rect,color);
			color=(color-color_step)&0xff;
			printf("color =0x%x\n",color);
		}
		color=0xff0000ff;
		rect.x=PLANE_WIDTH/4,rect.y=PLANE_HEIGHT/4,rect.w=PLANE_WIDTH/2,rect.h=PLANE_HEIGHT/2;
		cmem_fillrect(osd[0]->ge2d_fd,CANVAS_ALLOC,PLANE_WIDTH,PLANE_HEIGHT,GE2D_FORMAT_S32_ARGB,addr_rgb,&rect,color);
/**************************************************************
**
**		step  2 :move osd data to rgb space 
**
***************************************************************/
/*
		// 1.1  config 
		memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));    


		// src	
		ge2d_config_ex.src_para.mem_type=CANVAS_OSD0;
    		ge2d_config_ex.src_para.format  = GE2D_FORMAT_S32_ARGB;

    		ge2d_config_ex.src_para.top = 0;
    		ge2d_config_ex.src_para.left = 0;
		ge2d_config_ex.src_para.width=osd[0]->vinfo->xres;  
		ge2d_config_ex.src_para.height=osd[0]->vinfo->yres;
		//src2
		ge2d_config_ex.src2_para.mem_type=CANVAS_TYPE_INVALID;
		 //dst  
		ge2d_config_ex.dst_planes[0].addr=addr_rgb;
		ge2d_config_ex.dst_planes[0].w=PLANE_WIDTH;
		ge2d_config_ex.dst_planes[0].h=PLANE_HEIGHT;
		ge2d_config_ex.dst_para.mem_type = CANVAS_ALLOC;	
		ge2d_config_ex.dst_para.format=GE2D_FORMAT_S32_ARGB;
    		ge2d_config_ex.dst_para.x_rev = 0;
    		ge2d_config_ex.dst_para.y_rev = 0;
    		ge2d_config_ex.dst_para.color = 0;
    		ge2d_config_ex.dst_para.top = 0;
    		ge2d_config_ex.dst_para.left = 0;  
	  	ge2d_config_ex.dst_para.width =PLANE_WIDTH ;
    		ge2d_config_ex.dst_para.height =PLANE_HEIGHT;
			    
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);

		memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
		op_para.src1_rect.x = 0;
		op_para.src1_rect.y = 0;
		op_para.src1_rect.w = osd[0]->vinfo->xres;
		op_para.src1_rect.h = osd[0]->vinfo->yres;

		op_para.dst_rect.x = 0;
		op_para.dst_rect.y = 0;
		op_para.dst_rect.w = PLANE_WIDTH;
		op_para.dst_rect.h = PLANE_HEIGHT;
		ioctl(osd[0]->ge2d_fd,GE2D_STRETCHBLIT, &op_para);
		    	
		ge2d_config_ex.src_planes[0].addr=addr_rgb;
		ge2d_config_ex.src_planes[0].w = PLANE_WIDTH;
		ge2d_config_ex.src_planes[0].h = PLANE_HEIGHT;
		ge2d_config_ex.src_para.width =PLANE_WIDTH ;
		ge2d_config_ex.src_para.height =PLANE_HEIGHT;	
		ge2d_config_ex.src_para.mem_type = CANVAS_ALLOC;
		ge2d_config_ex.src_para.format=GE2D_FORMAT_S32_ARGB;

		ge2d_config_ex.dst_para.mem_type=CANVAS_OSD0;
		ge2d_config_ex.dst_para.width=osd[0]->vinfo->xres;
		ge2d_config_ex.dst_xy_swap=0;	
		ge2d_config_ex.src_para.x_rev = 0;
    		ge2d_config_ex.src_para.y_rev = 0;	
		ge2d_config_ex.dst_para.height=osd[0]->vinfo->yres; 	
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
		op_para.src1_rect.x = 0;
		op_para.src1_rect.y = 0;
		op_para.src1_rect.w =  PLANE_WIDTH;
		op_para.src1_rect.h = PLANE_HEIGHT;

		 op_para.dst_rect.x = 0;
		 op_para.dst_rect.y = 0;
		 op_para.dst_rect.w =osd[0]->vinfo->xres/2;
		 op_para.dst_rect.h = osd[0]->vinfo->yres/2;	
		 ioctl(osd[0]->ge2d_fd,GE2D_STRETCHBLIT, &op_para);
	*/	
/**************************************************************
**
**		step  3 :fill rect in yuv space 
**
***************************************************************/

		memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));   
		ge2d_config_ex.src_para.mem_type=CANVAS_ALLOC;
		ge2d_config_ex.src_para.format=ge2d_yuv_format;
		//ge2d_config_ex.src_para.color = 0x0080804f;
		//clip region
		ge2d_config_ex.src_para.width=PLANE_WIDTH;;
		ge2d_config_ex.src_para.height=PLANE_HEIGHT; 
		ge2d_config_ex.src_para.left=0;
		ge2d_config_ex.src_para.top=0;
		//plane
		
		ge2d_config_ex.src_planes[0].addr=addr_y;
		ge2d_config_ex.src_planes[0].w=PLANE_WIDTH;
		ge2d_config_ex.src_planes[0].h=PLANE_HEIGHT;

		ge2d_config_ex.src_planes[1].addr=addr_u;
		ge2d_config_ex.src_planes[1].w=PLANE_WIDTH;
		ge2d_config_ex.src_planes[1].h=PLANE_HEIGHT;

		ge2d_config_ex.src_planes[2].addr=addr_v;
		ge2d_config_ex.src_planes[2].w=PLANE_WIDTH;
		ge2d_config_ex.src_planes[2].h=PLANE_HEIGHT;

		//src2
		ge2d_config_ex.src2_para.mem_type=CANVAS_TYPE_INVALID;  
		
		ge2d_config_ex.dst_para.mem_type=CANVAS_ALLOC;
		ge2d_config_ex.dst_para.format=ge2d_yuv_format;
		//ge2d_config_ex.src_para.color = 0x0080804f;
		//clip region
		ge2d_config_ex.dst_para.width=PLANE_WIDTH;
		ge2d_config_ex.dst_para.height=PLANE_HEIGHT; 
		ge2d_config_ex.dst_para.left=0;
		ge2d_config_ex.dst_para.top=0;
		//plane
		ge2d_config_ex.dst_planes[0].addr=addr_y;
		ge2d_config_ex.dst_planes[0].w=PLANE_WIDTH;
		ge2d_config_ex.dst_planes[0].h=PLANE_HEIGHT;

		ge2d_config_ex.dst_planes[1].addr=addr_u;
		ge2d_config_ex.dst_planes[1].w=PLANE_WIDTH;
		ge2d_config_ex.dst_planes[1].h=PLANE_HEIGHT;

		ge2d_config_ex.dst_planes[2].addr=addr_v;
		ge2d_config_ex.dst_planes[2].w=PLANE_WIDTH;
		ge2d_config_ex.dst_planes[2].h=PLANE_HEIGHT;
		
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
		memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
		op_para.src1_rect.x = 0;
		op_para.src1_rect.y = 0;
		op_para.src1_rect.w =  PLANE_WIDTH ;
		op_para.src1_rect.h = PLANE_HEIGHT ;

		 op_para.dst_rect.x = 0;
		 op_para.dst_rect.y = 0;
		 op_para.dst_rect.w =PLANE_WIDTH;
		 op_para.dst_rect.h = PLANE_HEIGHT;	
		 op_para.color=0x83808182;
		 ioctl(osd[0]->ge2d_fd,GE2D_FILLRECTANGLE, &op_para);//this fillrect is equal to fill rect single y plane.
		 
/**************************************************************
**
**		step  3 :move alloc rgb space to alloc yuv space
**
***************************************************************/
		//type
/*		
		config.op_type=OSD0_OSD0;
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG,&config) ;
		op_para.src1_rect.x=0;
		op_para.src1_rect.y=0;
		op_para.src1_rect.w=osd[0]->vinfo->xres;
		op_para.src1_rect.h=osd[0]->vinfo->yres;
		//RGBA
		 op_para.color=0xaabbccdd;	
	 
	
		ioctl( osd[0]->ge2d_fd, GE2D_FILLRECTANGLE, &op_para) ;
*/
		memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));    
		ge2d_config_ex.src_para.mem_type=CANVAS_OSD0;
    		ge2d_config_ex.src_para.format  = GE2D_FORMAT_S32_ARGB;

    		ge2d_config_ex.src_para.top = 0;
    		ge2d_config_ex.src_para.left = 0;
		ge2d_config_ex.src_para.width=osd[0]->vinfo->xres;  
		ge2d_config_ex.src_para.height=osd[0]->vinfo->yres;
		//src2
		ge2d_config_ex.src2_para.mem_type=CANVAS_TYPE_INVALID;  
		//ge2d_config_ex.src_para.color = 0x0080804f;
		ge2d_config_ex.dst_para.mem_type=CANVAS_ALLOC;
		ge2d_config_ex.dst_para.format=GE2D_FORMAT_YUV|GE2D_FORMAT_S8_Y;
		//clip region
		ge2d_config_ex.dst_para.width=PLANE_WIDTH;
		ge2d_config_ex.dst_para.height=PLANE_HEIGHT; 
		ge2d_config_ex.dst_xy_swap=0;	
		ge2d_config_ex.dst_para.left=0;
		ge2d_config_ex.dst_para.top=0;
		//plane
		ge2d_config_ex.dst_planes[0].addr=addr_y;
		ge2d_config_ex.dst_planes[0].w=PLANE_WIDTH;
		ge2d_config_ex.dst_planes[0].h=PLANE_HEIGHT;
/*
		ge2d_config_ex.dst_planes[1].addr=addr_u;
		ge2d_config_ex.dst_planes[1].w=PLANE_WIDTH;
		ge2d_config_ex.dst_planes[1].h=PLANE_HEIGHT;

		ge2d_config_ex.dst_planes[2].addr=addr_v;
		ge2d_config_ex.dst_planes[2].w=PLANE_WIDTH;
		ge2d_config_ex.dst_planes[2].h=PLANE_HEIGHT;
*/
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
		memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
		op_para.src1_rect.x = 0;
		op_para.src1_rect.y = 0;
		op_para.src1_rect.w =  PLANE_WIDTH ;
		op_para.src1_rect.h = PLANE_HEIGHT ;

		 op_para.dst_rect.x = 0;
		 op_para.dst_rect.y = 0;
		 op_para.dst_rect.w =PLANE_WIDTH;
		 op_para.dst_rect.h = PLANE_HEIGHT;	
		 ioctl(osd[0]->ge2d_fd,GE2D_STRETCHBLIT_NOALPHA, &op_para);
		
		 
		ge2d_config_ex.dst_para.format=GE2D_FORMAT_YUV|GE2D_FORMAT_S8_CB;
		//plane
		ge2d_config_ex.dst_planes[0].addr=addr_u;
		ge2d_config_ex.dst_planes[0].w=PLANE_WIDTH*x_ratio;
		ge2d_config_ex.dst_planes[0].h=PLANE_HEIGHT*y_ratio;
		ge2d_config_ex.dst_para.width=PLANE_WIDTH*x_ratio;
		ge2d_config_ex.dst_para.height=PLANE_HEIGHT*y_ratio;
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
		memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
		op_para.src1_rect.x = 0;
		op_para.src1_rect.y = 0;
		op_para.src1_rect.w =  PLANE_WIDTH ;
		op_para.src1_rect.h = PLANE_HEIGHT ;

		 op_para.dst_rect.x = 0;
		 op_para.dst_rect.y = 0;
		 op_para.dst_rect.w =PLANE_WIDTH*x_ratio;
		 op_para.dst_rect.h = PLANE_HEIGHT*y_ratio;	
		ioctl(osd[0]->ge2d_fd,GE2D_STRETCHBLIT_NOALPHA, &op_para);

		ge2d_config_ex.dst_para.format=GE2D_FORMAT_YUV|GE2D_FORMAT_S8_CR;
		//plane
		ge2d_config_ex.dst_planes[0].addr=addr_v;
		ge2d_config_ex.dst_planes[0].w=PLANE_WIDTH*x_ratio;
		ge2d_config_ex.dst_planes[0].h=PLANE_HEIGHT*y_ratio;
		ge2d_config_ex.dst_para.width=PLANE_WIDTH*x_ratio;
		ge2d_config_ex.dst_para.height=PLANE_HEIGHT*y_ratio;
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
		memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
		op_para.src1_rect.x = 0;
		op_para.src1_rect.y = 0;
		op_para.src1_rect.w =  PLANE_WIDTH ;
		op_para.src1_rect.h = PLANE_HEIGHT ;

		 op_para.dst_rect.x = 0;
		 op_para.dst_rect.y = 0;
		 op_para.dst_rect.w =PLANE_WIDTH*x_ratio;
		 op_para.dst_rect.h = PLANE_HEIGHT*y_ratio;	
		ioctl(osd[0]->ge2d_fd,GE2D_STRETCHBLIT_NOALPHA, &op_para);

					
/**************************************************************
**
**		step  4 :move alloc yuv space to osd0
**
***************************************************************/
		memset((char*)&ge2d_config_ex,0,sizeof(config_para_ex_t));    
		ge2d_config_ex.src_para.mem_type=CANVAS_ALLOC;
		ge2d_config_ex.src_para.format=ge2d_yuv_format;
		//ge2d_config_ex.src_para.color = 0x0080804f;
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

		ge2d_config_ex.src2_para.mem_type=CANVAS_TYPE_INVALID;  
		
		ge2d_config_ex.dst_para.mem_type=CANVAS_OSD0;
		ge2d_config_ex.dst_para.left=0;
		ge2d_config_ex.dst_para.top=0;
		ge2d_config_ex.dst_para.width=osd[0]->vinfo->xres;
	
		ge2d_config_ex.dst_para.height=osd[0]->vinfo->yres; 	
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);
		memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
		op_para.src1_rect.x = 0;
		op_para.src1_rect.y = 0;
		op_para.src1_rect.w =  PLANE_WIDTH;
		op_para.src1_rect.h = PLANE_HEIGHT;

		 op_para.dst_rect.x = 0;
		 op_para.dst_rect.y = 0;
		 op_para.dst_rect.w =osd[0]->vinfo->xres;
		 op_para.dst_rect.h = osd[0]->vinfo->yres;	
		 ioctl(osd[0]->ge2d_fd,GE2D_STRETCHBLIT_NOALPHA, &op_para);
		 printf("moving data from yuv to osd back end\n");
		 sleep(2);
/**************************************************************
**
**		step  4 :blend yuv and argb to osd0
**
***************************************************************/
	
		ge2d_config_ex.src_para.mem_type=CANVAS_OSD0;
		ge2d_config_ex.src_para.left=0;
		ge2d_config_ex.src_para.top=0;
		ge2d_config_ex.src_para.width=osd[0]->vinfo->xres;
		ge2d_config_ex.src_para.height=osd[0]->vinfo->yres;
		//ge2d_config_ex.dst_xy_swap=1;	
		ge2d_config_ex.src_para.x_rev = 1;
    		//ge2d_config_ex.src_para.y_rev = 0;
		ge2d_config_ex.src2_para.mem_type=CANVAS_ALLOC;
		ge2d_config_ex.src2_para.format=GE2D_FORMAT_S32_ARGB;
		ge2d_config_ex.src2_planes[0].addr=addr_rgb;
		ge2d_config_ex.src2_planes[0].w=PLANE_WIDTH;
		ge2d_config_ex.src2_planes[0].h=PLANE_HEIGHT;
		ge2d_config_ex.src2_para.width=PLANE_WIDTH;
		ge2d_config_ex.src2_para.height=PLANE_HEIGHT;
		ge2d_config_ex.src2_para.left=0;
		ge2d_config_ex.src2_para.top=0;
		ioctl(osd[0]->ge2d_fd, GE2D_CONFIG_EX, &ge2d_config_ex);

		memset((char*)&op_para,0,sizeof(ge2d_op_para_t));
		op_para.src1_rect.x = 0;
		op_para.src1_rect.y = 0;
		op_para.src1_rect.w =  PLANE_WIDTH;
		op_para.src1_rect.h = PLANE_HEIGHT;

		op_para.src2_rect.x = 0;
		op_para.src2_rect.y = 0;
		op_para.src2_rect.w =  PLANE_WIDTH;
		op_para.src2_rect.h = PLANE_HEIGHT;
		
		op_para.dst_rect.x = 0;
		op_para.dst_rect.y = 0;
		op_para.dst_rect.w =PLANE_WIDTH;
		op_para.dst_rect.h = PLANE_HEIGHT;	
		op_para.op=blendop( 
			OPERATION_ADD,
                    /* color blending src factor */
                    COLOR_FACTOR_SRC_ALPHA,
                    /* color blending dst factor */
                    COLOR_FACTOR_ONE_MINUS_SRC_ALPHA,
                    /* alpha blending_mode */
                    OPERATION_ADD,
                    /* alpha blending src factor */
                    COLOR_FACTOR_SRC_ALPHA,
                    /* color blending dst factor */
                    COLOR_FACTOR_ONE_MINUS_SRC_ALPHA);
		ioctl(osd[0]->ge2d_fd,GE2D_BLEND, &op_para);
		
	  
	/*		
		    if(planes[0])
				CMEM_free( planes[0], &cmemParm);
   	           if(planes[1])
				CMEM_free( planes[1], &cmemParm);	
		    if(planes[2])
				CMEM_free( planes[2], &cmemParm);
	           if(planes[3])
				CMEM_free( planes[3], &cmemParm);
	*/			  
		    return 0;		
			
}
int  test_ge2d_op(void* para)
{
	int i,mode;
	osd_obj_t  *osd[2] ;
	int menu_item;
	ge2d_op_ctl_t  ge2d_control;
	int ret;
	
	pthread_t  reap_pthread_id;
	struct sigaction act ;
	sigset_t  mask;
	sighandler_t  old_sigint;

	
	old_sigint=signal(SIGINT,ge2d_handle_quit);
	//create two osd object 
	display_color_mode_list();
	thread_context.int_exit=0;
	for (i=0;i<MAX_THREAD_NUM;i++)
	{
		thread_context.pthread_id[i]=INVALID_THREAD_ID;
	}
	for (i=0;i<2;i++)
	{
		printf("please select one color mode for osd%d:",i);
		//mode=16;
		scanf("%d",&mode);
		if(mode<OSD_TYPE_16_655 || mode>33)
		{
			printf("invalid color format for osd device\r\n");
			return -1;
		}
		osd[i]=create_osd_obj(mode,i);
		if(NULL==osd[i]) 
		{
			printf("can't create osd object for  osd%d\r\n",i);
			return -1;
		}
	}
	thread_context.osd=osd[0] ;
	osd[1]->show(osd[1]);
	osd[0]->show(osd[0]);
	//init_key_array(osd[0]);
	//we first create two daemon.to process input & ouput.
	pthread_mutex_init(&thread_context.mutex,NULL);   
      	pthread_cond_init(&thread_context.cond_draw,NULL); 
	pthread_cond_init(&thread_context.cond_exit,NULL);

	ret=pthread_create(&thread_context.pthread_id[THREAD_DRAW],NULL,ge2d_draw,&thread_context);
	if(ret)
	{
		printf("error create thread draw:%s\r\n",strerror(errno));
		return -1;
	}
	printf("create input thread\r\n");
	ret=pthread_create(&thread_context.pthread_id[THREAD_INPUT],NULL,ge2d_input,&thread_context);
	if(ret)
	{
		printf("error create thread input:%s\r\n",strerror(errno));
		return -1;
	}
	printf("create reap  thread\r\n");
	
	ret=pthread_create(&reap_pthread_id,NULL,ge2d_reap_thread,(void *)&thread_context);
	if(ret)
	{
		printf("error create thread input:%s\r\n",strerror(errno));
		return -1;
	}
	//first we must select one color format for each osd device .
	printf("three daemon has been created\r\n");;
      
	
	while(!thread_context.int_exit)
	{
	
	//create osd object success ,begin ge2d operation.
	
	display_ge2d_op_menu();
	scanf("%d",&menu_item);
	//menu_item=5;
	if(menu_item >= GE2D_MAX_OP_NUMBER+7 || menu_item <=0 )
	{
		printf("invalid input menu item number \r\n");
		return -1;
	}
	
	switch(menu_item)
	{
		case GE2D_OP_FILL_RECT:
		ge2d_control.config.op_type=OSD0_OSD0 ;
		ge2d_control.config.alu_const_color=0xff0000ff;
		ge2d_control.pt_osd=osd[0] ;
		osd[0]->ge2d_op_func.fill_rect(&ge2d_control);
		sleep(1);
		osd[0]->show(osd[0]);
		sleep(3);
		break ;
		case GE2D_OP_BLEND:
		osd[1]->show(osd[1]);
		osd[0]->show(osd[0]);
		sleep(1);
		osd[0]->ge2d_op_func.blend(osd);	
		sleep(3);
		break;
		break;	
		case GE2D_OP_BLIT:
		osd[1]->show(osd[1]);
		osd[0]->show(osd[0]);
		sleep(1);	
		osd[0]->ge2d_op_func.blit(osd);	
		sleep(3);
		break;
		case GE2D_OP_STRETCH_BLIT:
		osd[1]->show(osd[1]);
		osd[0]->show(osd[0]);
		sleep(1);		
		osd[0]->ge2d_op_func.stretch_blit(osd);				
		sleep(3);
		case  5:
		printf("begin test keyboard \r\n");
		test_keyboard(&thread_context);
		break;
		case 6:
		printf("scale test \n");
		scale_test(osd);
		break;
		case 7:
		test_3d(osd);
		break;
		case 8:
		case 9:
		case 10:	
		/*	
		switch(mode)
		{
			case OSD_TYPE_16_565:
			mode=GE2D_FORMAT_S16_RGB_565;
			break;
			case OSD_TYPE_24_RGB:
			mode=GE2D_FORMAT_S24_RGB;
			break;
			case OSD_TYPE_32_ARGB:
			mode=OSD_TYPE_32_ARGB;
			break;
			default:
			continue ;	
		}
		*/
		test_cmem(osd,menu_item);
		break;
	}
	printf("press Enter to exit\r\n");
	scanf("%c%c",&mode,&mode);
	
	}
	
	
	pthread_cond_signal(&thread_context.cond_exit);
	pthread_join(reap_pthread_id,NULL);
	signal(SIGINT,old_sigint);
	
	destroy_osd_obj(osd[0]);
	destroy_osd_obj(osd[1]);
	return 0;
}
