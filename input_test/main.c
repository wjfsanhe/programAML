#include <stdlib.h>
#include <stdio.h>
#include <linux/input.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <time.h>
#include <sys/queue.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/wait.h> 
#include <linux/input.h>

void simulate_mouse_wheel(int fd, signed char but,signed char dir )   
{   
         int rel_x,rel_y;   
         static struct input_event event,ev;   
         
	  if(dir!=0)  //x direction
  	  {
		event.type = EV_REL;
		rel_x =dir>0?1:-1;   
              	event.code = REL_WHEEL;   
         	event.value = rel_x; 
		if( write(fd,&event,sizeof(event))!=sizeof(event))   
                   printf("rel_x error~~~:%s\n",strerror(errno)); 	
  	  }

	  if(but)
	  {
	  	
	  	event.type = EV_KEY; 
		event.code = BTN_MIDDLE;  
		event.value = but>0?1:0;
		printf("middle key %s\n",but>0?"press":"release");
		if( write(fd,&event,sizeof(event))!=sizeof(event))   
                   printf("rel_x error~~~:%s\n",strerror(errno)); 
	  }
           
          
          
          //gettimeofday(&event.time,0);   
  
          
          //一定要刷新空的   
         // write(fd,&ev,sizeof(ev));   
}  
void simulate_mouse_xy(int fd, char x_y,char 	)   
{   
         int rel_x,rel_y;   
         static struct input_event event,ev;   
         
	  event.type = EV_REL;
  	  if(x_y==0)  //x direction
  	  {
         	rel_x =(dir>0?1:-1)* 2;   
              	event.code = REL_X;   
         	event.value = rel_x; 
		if( write(fd,&event,sizeof(event))!=sizeof(event))   
                   printf("rel_x error~~~:%s\n",strerror(errno)); 	
  	 }
	 else
	 {
	 	rel_y = (dir>0?-1:1)*2;
		event.code = REL_Y;   
		event.value = rel_y;   
		if( write(fd,&event,sizeof(event))!=sizeof(event))   
                    printf("rel_y error~~~:%s\n",strerror(errno));
	 }
          //gettimeofday(&event.time,0);   
  
           
          
          
          //gettimeofday(&event.time,0);   
  
          
          //一定要刷新空的   
          write(fd,&ev,sizeof(ev));   
}  




static unsigned int 
translate_event( const struct input_event *levt)
{

     switch (levt->type) {
	        case EV_KEY:
		 printf("key  :0x%x",levt->code);
		 if(levt->value==2)
		 printf(" repeat");
		 if(levt->value==1)
		 printf(" press");
		  if(levt->value==0)
		 printf(" release");
		  printf("\n");
               return levt->code;

          case EV_REL:

		  switch (levt->code)
		  {
		  	case REL_X:
			printf("pos x: 0x%x\n",levt->value);
			break;
			case REL_Y:
			printf("posy:0x%x\n",levt->value);	
			break;
			case REL_WHEEL:
			printf("wheel:0x%x",levt->value);
			break;
			default:
			printf("unknown: code:0x%x value:0x%x\n",levt->code,levt->value);
			break;
		  }
		 break;
          case EV_ABS:
	   printf("abs event\n");
	   break;
          default:
	    printf("event code :0x%x\n",levt->type);	  	
	    break ;	  	
               ;
     }

     return -1;
}

static int read_mouse_key(int intput_fd)
{
	fd_set rfds,wfds;
	struct timeval tv;
	int retval ;
	int exit_enable=0;
	int lace=0x1000;
	int i,readlen;
	
	 struct input_event levt[64];
	 
	tv.tv_sec = 10;
	tv.tv_usec = 10;
	FD_ZERO(&rfds);
	FD_SET(intput_fd, &rfds);
	while(!exit_enable)
	{
	retval = select(intput_fd+1, &rfds, NULL, NULL, &tv);  //wait demux data
	if(retval==-1)
	{
		exit_enable=1;
	}
	else if(retval)
	{
		if(FD_ISSET(intput_fd,&rfds)) 
		{
			printf("new key in \r\n");
			readlen =read(intput_fd,levt, sizeof(levt));
			 for (i=0; i<readlen / sizeof(levt[0]); i++) {
			 	
			translate_event(&levt[i]);	
		 	}
		}
	}
	}
	return 0;
}
void  main(int argc ,char** argv)
{
	int fd ,i;
	fd=open("/dev/input/event0",O_RDWR);
	
	/*
	simulate_mouse_wheel(fd,1,0);
	usleep(500);
	simulate_mouse_wheel(fd,-1,0);
	*/
	/*
	for (i=0;i<10;i++)
	{
		simulate_mouse_xy(fd,0,1);
		usleep(500);
	}
	for (i=0;i<10;i++)
	{
		simulate_mouse_xy(fd,1,1);
		usleep(500);
	}*/
	read_mouse_key(fd);
	
	close(fd);
}
