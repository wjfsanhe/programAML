#include <stdio.h>
#include <stdlib.h>
#include "Amlogic.h"
#include <sys/poll.h>
#include <linux/input.h>
#include <fcntl.h> 
//#define COLOR_TYPE FB_COLORMODE_565
//#define COLOR_TYPE FB_COLORMODE_24
#define COLOR_TYPE FB_COLORMODE_32

int main( int argc, char** argv)
{
	int nScrW, nScrH;
	unsigned char bColorMode = COLOR_TYPE;
	void      *pMwFb;
	int times = 0;
	int	iResult;	
	nScrW = 644;//640;
	nScrH = 534;//526;
	printf("WM Width: %d Height: %d\n", nScrW, nScrH);   	
        if (MwFbInit(nScrW, nScrH, bColorMode, &pMwFb) != 0)                     /* 3?¨º??¡¥¦Ì¡Á2??y?¡¥3¨¦1|¡ê? */
        {
             printf("fail to execute MwFbInit\n");
             return 1;
        }
	
	MWCOORD    screenWidth;
        MWCOORD    screenHeight;
        int        bpp;
        MwFbGetScrInfo(&screenWidth, &screenHeight, &bpp, &pMwFb);
        printf("MwFbGetScrInfo:%d, %d, %d\n", screenWidth, screenHeight, bpp);
        
        int pnLineLen = 0;
        MwFbGetLineLen(&pnLineLen);
        printf("MwFbGetLineLen:%d\n", pnLineLen);
        
        int pbColorMode = 0;
        MwFbGetPixType(&pbColorMode);
        printf("MwFbGetPixType:%d\n", pbColorMode);
        
        int pnSize = 0;
        MwFbGetBufSize(&pnSize);
        printf("MwFbGetBufSize:%d\n", pnSize);        
        
        MwFbDriverControl(1, 1);
        
        MwFbDriverControl(1, 0);
 	
        unsigned int color = 0xFFFF0000;  
        //unsigned int color = 0x1f1f1f;  
        if (COLOR_TYPE == FB_COLORMODE_565){		
        	printf("FB_COLORMODE_565\n");
		int y, x; 
		unsigned char* p = (unsigned char*)pMwFb;		
		/*unsigned int color1;// = rand() % 0xFFFFFF;
		        
	        unsigned int red = (color >> 16) & 0xff;
		unsigned int green = (color >> 8) & 0xff;
		unsigned int blue = color & 0xff;
	        
	        color1 = ((((unsigned int)(red) >> 3) << 11)    \
			| (((unsigned int)(green) >> 2) << 5)	 \
			| (((unsigned int)(blue) >> 3) << 0));		
				
		printf("##########0x%04x\n", color1);				
				
		for(y = 0; y <= pnSize / 2; y++){
		        
							
			memcpy(p, (char*)&color1, 2);
			p++;
			p++;
			//*((unsigned short*)p) = (unsigned short)color1;
			//*p++ = (unsigned char)(color1  & 0xFF);				
			//*p++ = (unsigned char)((color1 >> 8) & 0xFF);					
	        }*/
	        FILE* fp = fopen("./pic", "rb");		
		int i, j;
		unsigned short	tmpR,tmpG,tmpB;
		unsigned int color;
		char buffer[626 * 2 + 1];
		fseek(fp, 0, SEEK_SET);	
		
		for(i = 0; i < 446; i++){	
			fread(p, sizeof(char), 626 * 2, fp);
			p += pnLineLen;			
			
                }    
               
		fclose(fp);
        }
        else if (COLOR_TYPE == FB_COLORMODE_24){
        	unsigned char* p = (unsigned char*)pMwFb;
        	/*int y, x; 		
		color = 0x0000FF00;		
		for(y = 0; y <= pnSize / 3; y++){	
			//color = rand() % 0xFFFFFF;					
			*p++ = (unsigned char)(color  & 0xFF);		
			*p++ = (unsigned char)((color >> 8) & 0xFF);				
			*p++ = (unsigned char)((color >> 16) & 0xFF);				
	        }*/
	        printf("start to draw!\n");
		FILE* fp = fopen("./pic", "rb");	
	     	int i, count = 0, col = 0, j;
	     	/*for(i = 0; i < 576; i++){	
			fread(p, sizeof(char), 720 * 3, fp);
			p += pnLineLen;			
			
                } */ 
                
	     		
		/*
		fseek(fp, 0, SEEK_SET);
		char buffer[720 * 3 + 1];
		unsigned char* head = p;
		for(i = 0; i < 576; i++){	
			p = head;
			fread(buffer, sizeof(char), 720 * 3, fp);
			//memcpy(p, buffer, 720 * 3);
			for(j = 0; j < 720 * 3;j += 3){
				*p++ = (unsigned char)buffer[j + 2];
				*p++ = (unsigned char)buffer[j + 1];
				*p++ = (unsigned char)buffer[j + 0];				
			}	
			head += pnLineLen;												
			
                } */
		
		/*char buffer[626 * 2 + 1];
		unsigned char* head = p;
		for(i = 0; i < 446; i++){
			p = head;
			fread(buffer, sizeof(char), 626 * 2, fp);
			j = 0;
			while (j < 626 * 2){			
				memcpy(&color, buffer + j, 2);
							
				*p++ = color & 0x1f;		
				*p++ = (color >> 5) & 0x3f;				
				*p++ = (color >> 11) & 0x1f;	  				
										
				j += 2;
			}				
			head += pnLineLen;									
                }   */         
		
                	
		//p += 100 * pnLineLen;					
		unsigned char* orgin = p;
		int high = 576, weight = 720;		                
		//while (!feof(fp)){
			/*if (col > 2){
				break;
			}						
			p = orgin  + pnLineLen * high * col + count * weight * 3;	
			*/
			for(i = 0; i < high; i++){			
	                	//fread(p,sizeof(char),weight * 3,fp);
	                	for (j = 0; j < weight; j++){       
	                		memcpy(p + j * 3, &color, 3);
	                	}	          	          	
	                	p += pnLineLen;	                	
	                } 		                        
	                /*count++;	  
	                if (count % 4 == 0){
	                	col++;
	                	count = 0;
	                	p = orgin + pnLineLen * high * col;
	                }   */       	                  
                //}
                printf("draw end\n");
		fclose(fp);
			
        }
        else if (COLOR_TYPE == FB_COLORMODE_32){
        	printf("#############%04x\n", color);
        	color = 0x440000ff;  //BGRA
        	unsigned char* p = (unsigned char*)pMwFb;
        	int i, count = 0, col = 0, j;
        	unsigned char* orgin = p;
		int high = nScrH, weight = nScrW;	
		           
		for(i = 0; i < high; i++){		
                	for (j = 0; j < weight; j++){       
                		memcpy(p + j * 4, &color, 4);
                	}	          	          	
                	p += pnLineLen;	
                	
                	if (p == NULL)
                		printf("p is null\n");                	
                }
		color=MASKNONE;;
		 p=pMwFb+pnLineLen*40+50*4;
		 for(i = 40; i < 200; i++){		
                	for (j =50; j < 200; j++){       
                		memcpy(p + j * 4, &color, 4);
                	}	          	          	
                	p += pnLineLen;	
                	
                	if (p == NULL)
                		printf("p is null\n");                	
                }    		
        }
	
	MWRECT ptRect;
	ptRect.top = 20;
	ptRect.left = 30;
	ptRect.right = 300;//352;
	ptRect.bottom = 300; //288;	
	printf("@@@@@@@@pic begin\n");
 	MwFbRefreshPixel(0x10, 0, &ptRect, 1);
 	printf("@@@@@@@@pic end\n");
	sleep(2);
		exit(0);
        printf("start to draw!\n");
	//FILE* fp = fopen("./pic", "rb");	
     	int i, count = 0, col = 0, j;
        ptRect.top = 0;
	ptRect.left = 0;
	ptRect.right = 100;//352;
	ptRect.bottom = 100; //288;	
	int high = ptRect.bottom - ptRect.top;
	int weight = ptRect.right - ptRect.left;
	char buffer[high * weight * 4 + 1];
	if (COLOR_TYPE == FB_COLORMODE_32){		
 		color = 0xFF000000;
 		
        	unsigned char* p = (unsigned char*)buffer;
        	int i, count = 0, col = 0, j;
        	unsigned char* orgin = p;			                
		for(i = 0; i < high * weight; i++){		
                	//for (j = 0; j < weight; j++){       
                		memcpy(p + i * 4, &color, 4);
                	//}	          	          	
                	//p += pnLineLen;	
                	
                	if (p == NULL)
                		printf("p is null\n");                	
                }    	
        }
	printf("@@@@@@@@pic begin,fsfsdafas\n");
 	MwFbRefreshBuf(&ptRect, buffer);
 	 	printf("@@@@@@@@pic end\n");  
       	sleep(2);
	exit(0);
	//MwFbEarasePixel(0x10, 0, &ptRect, 1);	
       	/*unsigned int  iIrDevice = 0;
       	iIrDevice	= open("/dev/input/event0", O_RDWR);    
       	if (iIrDevice > 0)
	{
		printf("open irdevice O_RDWR ok!\r\n");
	}  
	else{
		printf("fail to open irdevice\n");
	} 
        while (times < 1000000){
        	struct pollfd	tPfd;
        	struct input_event read_event;
        	tPfd.fd			= iIrDevice;
		tPfd.events		= POLLIN;
		tPfd.revents		= 0;
		iResult	=	poll(&tPfd,1, 0);
		//printf("##################result:%d\n", iResult);
		if (iResult	!= 0)
		{
			//printf("#$$$$$$$$$$$$$$$enter\n");
			if (tPfd.revents & POLLIN)
			{
				iResult = read (iIrDevice,&read_event, sizeof(read_event));
				printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$key:0x%06x, %d\n", read_event.code, read_event.value);
					
			}
		}
        	//char tmp;
        	//scanf("%c", &tmp); 
        	ptRect.top = 0;
		ptRect.left = 0;
		ptRect.right = 720;//352;
		ptRect.bottom = 576; //288;	
		printf("@@@@@@@@pic begin, times:%d\n",times);
		//osdUpdateRegion1(ptRect.top, ptRect.left, ptRect.right, ptRect.bottom);	       	
	       	
        	times++;
        	//osdUpdateRegion(ptRect.top, ptRect.left, ptRect.right, ptRect.bottom);	       	
        	MwFbRefreshPixel(0x10, 0, &ptRect, 1);
        	//printf("@@@@@@@@pic end\n"); 
        	  
        	
        }        
        close(iIrDevice);      
        sleep(10);       */
__end0:
	printf("444\n");
	MwFbUninit();
	printf("5555\n");
	printf("success to exit main!\n");
	return 0;
}


/*memset(buffer, 0, sizeof(buffer));		
                	fread(buffer,sizeof(char),563 * 3,fp);
                	for(x = 0; x < 563; x++){
                	    	tmpR = buffer[x * 3 + 0];
				tmpG = buffer[x * 3 + 1];
				tmpB = buffer[x * 3 + 2];
				
				color=((tmpR>>3&0x1f)<<11|(tmpG>>2&0x1f)<<5|(tmpB>>3&0x3f));//*1.0/0xff*Alpha;//OK
				
				*(p++) = (color & 0x0000FF00) >> 8;
				*(p++) = color & 0x000000FF;								
                	}	
                	p += (pnLineLen - 563 * 2);*/
                	


       	
