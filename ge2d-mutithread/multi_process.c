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
#define  OSD1_BACKGROUND_COLOR	 0x444444ff

#define  TRANSPARENT_COLOR	  0x00000000
#define  OSD0_BUTTON_COLOR     0x0000ffff

#define  OSD1_BUTTON_COLOR	  0xff0000ff
#define  OSD2_BUTTON_COLOR	  0x006600ff
#define  OSD3_BUTTON_COLOR	  0xff00ffff

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
		default:
		config.op_type=OSD1_OSD1;	
		break;	
	}
	config.alu_const_color=0xff0000ff;
#ifdef  MULTI_PROCESS	
	ioctl( osd->ge2d_fd, GE2D_CONFIG, &config) ;
#else
	ioctl( osd->fbfd, GE2D_CONFIG, &config) ;
#endif
	return 0;
}
static int  fill_rect(osd_obj_t*  osd,rectangle_t  *rect,unsigned int  color)
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
		case 0:
		rect.x=0;
		rect.y=0;
		rect.w=SCREEN_W/2;
		rect.h=SCREEN_H;
		fill_rect(osd,&rect,OSD0_BACKGROUND_COLOR);
		rect.x+=SCREEN_W/2;
		fill_rect(osd,&rect,TRANSPARENT_COLOR);
		rect.x=0;
		rect.y=SCREEN_H/2;
		rect.w=SCREEN_W;
		rect.h=SCREEN_H/2;
		fill_rect(osd,&rect,TRANSPARENT_COLOR);
		break;
		case 1:
		rect.x=SCREEN_W/2;
		rect.y=0;
		rect.w=SCREEN_W/2;
		rect.h=SCREEN_H/2;	
		fill_rect(osd,&rect,OSD1_BACKGROUND_COLOR);
		break;
		case 2:
		rect.x=0;
		rect.y=SCREEN_H/2;
		rect.w=SCREEN_W/2;
		rect.h=SCREEN_H/2;	
		fill_rect(osd,&rect,OSD1_BACKGROUND_COLOR);	
		break;	
		case 3:
		rect.x=SCREEN_W/2;
		rect.y=SCREEN_H/2;
		rect.w=SCREEN_W/2;
		rect.h=SCREEN_H/2;	
		fill_rect(osd,&rect,OSD0_BACKGROUND_COLOR);	
		break;		
	}
	return 0;
}
static int  draw_sample_button(osd_obj_t *osd)
{
	rectangle_t  osd0_rect={BUTTON_START_X,BUTTON_START_Y,OSD0_BUTTON_WIDTH,OSD0_BUTTON_HEIGHT};
	rectangle_t  osd1_rect={SCREEN_W/2+BUTTON_START_X,BUTTON_START_Y,OSD1_BUTTON_WIDTH,OSD1_BUTTON_HEIGHT};
	rectangle_t  osd3_rect={BUTTON_START_X+SCREEN_W/2,BUTTON_START_Y+SCREEN_H/2,OSD0_BUTTON_WIDTH,OSD0_BUTTON_HEIGHT};
	rectangle_t  osd2_rect={BUTTON_START_X,BUTTON_START_Y+SCREEN_H/2,OSD1_BUTTON_WIDTH,OSD1_BUTTON_HEIGHT};
	switch (osd->osd_index )
	{
		case OSD0_INDEX:
		fill_rect(osd,&osd0_rect,OSD0_BUTTON_COLOR);	
		break;
		case OSD1_INDEX:
		fill_rect(osd,&osd1_rect,OSD1_BUTTON_COLOR);	
		break;
		case 2:
		fill_rect(osd,&osd2_rect,OSD2_BUTTON_COLOR);
		break;
		case 3:
		fill_rect(osd,&osd3_rect,OSD3_BUTTON_COLOR);
		break;	
		
	}
	return 0;
}
static int  move_sample_button(osd_obj_t *osd)
{
	rectangle_t  osd0_rect={BUTTON_START_X,BUTTON_START_Y,OSD0_BUTTON_WIDTH,OSD0_BUTTON_HEIGHT};
	rectangle_t  osd1_rect={SCREEN_W/2+BUTTON_START_X,BUTTON_START_Y,OSD1_BUTTON_WIDTH,OSD1_BUTTON_HEIGHT};
	rectangle_t  osd3_rect={BUTTON_START_X+SCREEN_W/2,BUTTON_START_Y+SCREEN_H/2,OSD0_BUTTON_WIDTH,OSD0_BUTTON_HEIGHT};
	rectangle_t  osd2_rect={BUTTON_START_X,BUTTON_START_Y+SCREEN_H/2,OSD1_BUTTON_WIDTH,OSD1_BUTTON_HEIGHT};
	static rectangle_t osd0_last_rect={0,0,0,0};
	static rectangle_t osd1_last_rect={0,0,0,0};
	static rectangle_t osd2_last_rect={0,0,0,0};
	static rectangle_t osd3_last_rect={0,0,0,0};
	
	switch (osd->osd_index )
	{
		case 0:
		if(osd0_last_rect.w!=0)
		{
			fill_rect(osd,&osd0_last_rect,OSD0_BACKGROUND_COLOR);
		}
		osd0_rect.x=rand()%540;
		osd0_rect.y=rand()%310;
		if(osd0_rect.x < 150 && osd0_rect.y < 100)
		{
			osd0_rect.x =150;
			osd0_rect.y =150;
		}
		fill_rect(osd,&osd0_rect,OSD0_BUTTON_COLOR);	
		osd0_last_rect=osd0_rect;
		break;
		case 1:
		if(osd1_last_rect.w!=0)
		{
			fill_rect(osd,&osd1_last_rect,OSD1_BACKGROUND_COLOR);
		}	
		osd1_rect.x=rand()%590 + 640 ;
		osd1_rect.y=rand()%260; 
		if(osd1_rect.x < 740 && osd1_rect.y < 150)
		{
			osd1_rect.x =790;
			osd1_rect.y =150;
		}	
		fill_rect(osd,&osd1_rect,OSD1_BUTTON_COLOR);
		osd1_last_rect=osd1_rect;
		break;
		case 2:
		if(osd2_last_rect.w!=0)
		{
			fill_rect(osd,&osd2_last_rect,OSD1_BACKGROUND_COLOR);
		}
		osd2_rect.x=rand()%590;
		osd2_rect.y=rand()%260+360; //360 -100
		if(osd2_rect.x < 100 && osd2_rect.y < 510)
		{
			osd2_rect.x =150;
			osd2_rect.y =510;
		}
		fill_rect(osd,&osd2_rect,OSD2_BUTTON_COLOR);	
		osd2_last_rect=osd2_rect;
		break;
		case 3:
		if(osd3_last_rect.w!=0)
		{
			fill_rect(osd,&osd3_last_rect,OSD0_BACKGROUND_COLOR);
		}
		osd3_rect.x=rand()%540+640;
		osd3_rect.y=rand()%310 + 360;
		if(osd3_rect.x < 790 && osd3_rect.y < 460)
		{
			osd3_rect.x =790;
			osd3_rect.y =510;
		}
		fill_rect(osd,&osd3_rect,OSD3_BUTTON_COLOR);	
		osd3_last_rect=osd3_rect;
		break;
	}
	return 0;
}
static  int kill_all_child_process(int *child_pid,int count)
{
	int status;
	pid_t  pid;
	int exit=0;
	int i;
	
	//printf("begin kill %d child process:\n",count);
	for(i=0;i<count;i++)
	kill(child_pid[i],SIGKILL);
	for(i=0;i<count;i++)
	waitpid(child_pid[i],&status,0);
	//printf("all process quit\n");
/*	
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
	*/	
}
static int  parent_process(int *child_pid,int count)
{
	osd_obj_t*  osd0;
	int  input;
	//printf("child process pid:%d\n",child_pid);
	//osd0=create_osd_obj(OSD_TYPE_32_ARGB,OSD0_INDEX);
	//create_osd_canvas(osd0);
	//draw_sample_button(osd0);
	while(!kill_process)
	{
		//move_sample_button(osd0);
		//usleep(50);
		usleep(100000);
	}
	kill_all_child_process(child_pid,count);
	return 0 ;
}
static int child_process(osd_obj_t*  osd)
{
	create_osd_canvas(osd);
	draw_sample_button(osd);
	while(1){
		move_sample_button(osd);
		//usleep(5);
	}
	return 0;
}
int   create_one_child(osd_obj_t  * osd)
{
	static  int  index=0;
	int  pid;
	pid=fork();
	if(pid==0)
	{
		child_process(osd);
	}
	index ++;
	return pid;
}
int   multi_process_test(void* para)
{
#define  CHILD_PROCESS_NUM  4
	int  pid[CHILD_PROCESS_NUM],i;
	osd_obj_t  *osd[CHILD_PROCESS_NUM];
	 
	//printf("multi process test \n");
	//install SIGTERM,SIGINT
	signal(SIGTERM,handle_term_int);
	signal(SIGINT,handle_term_int);
	for(i=0;i<CHILD_PROCESS_NUM;i++)
	{
		osd[i]=create_osd_obj(OSD_TYPE_32_ARGB,i);
	}
	for(i=0;i<CHILD_PROCESS_NUM;i++)
	{
		pid[i]=create_one_child(osd[i]);
	}
	parent_process(pid,CHILD_PROCESS_NUM);		
	return 0;
}