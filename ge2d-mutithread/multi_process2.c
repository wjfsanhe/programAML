#include "osd.h"
#include <linux/input.h>
#include <sys/poll.h>
#include	<pthread.h>
#include <sys/time.h> 
#include <signal.h>
#define  OSD0_INDEX		0
#define  OSD1_INDEX		1


#define  SCREEN_W		(osd->vinfo->xres)
#define  SCREEN_H		(osd->vinfo->yres)
#define  OSD0_BACKGROUND_COLOR   0x222222ff
#define  OSD1_BACKGROUND_COLOR	 0x44444444ff
#define  TRANSPARENT_COLOR	  0x00000000
#define  OSD0_BUTTON_COLOR     0x0000ffff

#define  OSD1_BUTTON_COLOR	  0xff0000ff

#define  BUTTON_START_X		50
#define  BUTTON_START_Y		50
#define  OSD0_BUTTON_WIDTH		100
#define  OSD0_BUTTON_HEIGHT		50
#define  OSD1_BUTTON_WIDTH		OSD0_BUTTON_HEIGHT
#define  OSD1_BUTTON_HEIGHT		OSD0_BUTTON_WIDTH


#define    MULTI_PROCESS

static int  kill_process=0;



static void  handle_term_int(int sig_num)
{
	if(sig_num==SIGTERM || sig_num==SIGINT)
	{
		kill_process=1;
	}
}
//color format: ARGB
static int  ge2d_config(osd_obj_t*  osd)
{
	alloc_config_t  config ;

	switch (osd->osd_index)
	{
		case OSD0_INDEX:
		config.op_type=OSD0_OSD0;	
		break;
		case OSD1_INDEX:
		config.op_type=OSD1_OSD1;	
		break;	
	}
	config.alu_const_color=0xff0000ff;
#ifdef  MULTI_PROCESS	
	ioctl( osd->ge2d_fd,GE2D_CONFIG, &config) ;
#else
	ioctl( osd->fbfd, GE2D_CONFIG, &config) ;
#endif
	return 0;
}
static int  fill_rect(osd_obj_t*  osd,rectangle_t  *rect,int  color)
{
	
	ge2d_op_para_t  op_para;

	ge2d_config(osd);
	op_para.src1_rect.x=rect->x;
	op_para.src1_rect.y=rect->y;
	op_para.src1_rect.w=rect->w;
	op_para.src1_rect.h=rect->h;
	op_para.color=color;
#ifdef  MULTI_PROCESS	
	ioctl( osd->ge2d_fd , GE2D_FILLRECTANGLE, &op_para) ;
#else
	ioctl( osd->fbfd , GE2D_FILLRECTANGLE, &op_para) ;
#endif
	return 0;
}
static int  create_osd_canvas(osd_obj_t *osd)
{
	rectangle_t  rect;
	switch (osd->osd_index)
	{
		case OSD0_INDEX:
		rect.x=0;
		rect.y=0;
		rect.w=SCREEN_W/2;
		rect.h=SCREEN_H;
		fill_rect(osd,&rect,OSD0_BACKGROUND_COLOR);
		rect.x+=SCREEN_W/2;
		fill_rect(osd,&rect,TRANSPARENT_COLOR);
		break;
		case OSD1_INDEX:
		rect.x=SCREEN_W/2;
		rect.y=0;
		rect.w=SCREEN_W/2;
		rect.h=SCREEN_H;	
		fill_rect(osd,&rect,OSD1_BACKGROUND_COLOR);
		break;	
	}
	return 0;
}
static int  draw_sample_button(osd_obj_t *osd)
{
	rectangle_t  osd0_rect={BUTTON_START_X,BUTTON_START_Y,OSD0_BUTTON_WIDTH,OSD0_BUTTON_HEIGHT};
	rectangle_t  osd1_rect={SCREEN_W/2+BUTTON_START_X,BUTTON_START_Y,OSD1_BUTTON_WIDTH,OSD1_BUTTON_HEIGHT};
	switch (osd->osd_index )
	{
		case OSD0_INDEX:
		fill_rect(osd,&osd0_rect,OSD0_BUTTON_COLOR);	
		break;
		case OSD1_INDEX:
		fill_rect(osd,&osd1_rect,OSD1_BUTTON_COLOR);	
		break;	
	}
	return 0;
}
static int  move_sample_button(osd_obj_t *osd)
{
	rectangle_t  osd0_rect={BUTTON_START_X,BUTTON_START_Y,OSD0_BUTTON_WIDTH,OSD0_BUTTON_HEIGHT};
	rectangle_t  osd1_rect={SCREEN_W/2+BUTTON_START_X,BUTTON_START_Y,OSD1_BUTTON_WIDTH,OSD1_BUTTON_HEIGHT};
	static rectangle_t osd0_last_rect={0,0,0,0};
	static rectangle_t osd1_last_rect={0,0,0,0};
	switch (osd->osd_index )
	{
		case OSD0_INDEX:
		if(osd0_last_rect.x!=0)
		{
			fill_rect(osd,&osd0_last_rect,OSD0_BACKGROUND_COLOR);
		}
		osd0_rect.x=rand()%540;
		osd0_rect.y=rand()%620;
		if(osd0_rect.x < 160 && osd0_rect.y < 110)
		{
			osd0_rect.x +=160;
			osd0_rect.y +=110;
		}
		fill_rect(osd,&osd0_rect,OSD0_BUTTON_COLOR);	
		osd0_last_rect=osd0_rect;
		break;
		case OSD1_INDEX:
		if(osd1_last_rect.x!=0)
		{
			fill_rect(osd,&osd1_last_rect,OSD1_BACKGROUND_COLOR);
		}	
		osd1_rect.x=rand()%640 + 640 ;
		osd1_rect.y=rand()%620;
		if(osd1_rect.x < 760 && osd1_rect.y < 160)
		{
			osd1_rect.x +=60;
			osd1_rect.y +=160;
		}	
		fill_rect(osd,&osd1_rect,OSD1_BUTTON_COLOR);
		osd1_last_rect=osd1_rect;
		break;	
	}
	return 0;
}
static  int kill_all_child_process(int child_pid)
{
	int status;
	pid_t  pid;
	int exit=0;
	printf("begin kill child process:%d\n",child_pid);
	kill(child_pid,SIGKILL);
	while(!exit)
	{
		pid=waitpid(child_pid,&status,WNOHANG);
		if(pid==child_pid)
		{
		//WIFSIGNALED   child process receive signal
		//WIFEXITED 	  child process exit .
			if(WIFSIGNALED(status) ) 
			{
				printf("osd1 process killed\n");
				exit =1;
			}else{
				printf("child process not killed\n");
			}
		}
	}
		
}
static int  osd0_process(int child_pid)
{
	osd_obj_t*  osd0;
	int  input;
	printf("child process pid:%d\n",child_pid);
	osd0=create_osd_obj(OSD_TYPE_32_ARGB,OSD0_INDEX);
	create_osd_canvas(osd0);
	draw_sample_button(osd0);
	while(!kill_process)
	{
		move_sample_button(osd0);
		//usleep(50);
	}
	kill_all_child_process(child_pid);
	return 0 ;
}
static int osd1_process(void)
{
	osd_obj_t*  osd1;
	printf("me pid :%d\n",getpid());
	osd1=create_osd_obj(OSD_TYPE_32_ARGB,OSD1_INDEX);
	create_osd_canvas(osd1);
	draw_sample_button(osd1);
	while(1){
		move_sample_button(osd1);
		//usleep(50);
	}
	return 0;
}
int   multi_process_test(void* para)
{
	int  pid;
	 
	printf("multi process test \n");
	//install SIGTERM,SIGINT
	signal(SIGTERM,handle_term_int);
	signal(SIGINT,handle_term_int);
	pid=fork();
	if(pid>0)  //osd0 process 
	{
		osd0_process(pid);			
	}else{
		osd1_process();	
	}
}