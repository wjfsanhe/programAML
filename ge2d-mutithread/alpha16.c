#include "osd.h"


#if  !defined(BITMAP_USE_FILE)
#include "bitmap16.h"
#include "bitmap24.h"
#else
U16 *bitmap16=NULL;
U32	  *bitmap24=NULL;
#endif





int 	BMP_WIDTH ;
int	BMP_HEIGHT	;
char *error_msg_table[]={
	"invalid input menu item number \r\n" , \
	"draw function for this mode not impleted\r\n", \
	"invalid input menu item number \r\n", \
	"invalid color format for osd device\r\n", \
};


#if  defined(BITMAP_USE_FILE)
int  dump_bmp_info(BITMAPFILEHEADER *file_header,BITMAPINFOHEADER *info_header)
{
	printf("BITMAP file informaion : \n"   \
			"file size 		: %d (Kbyte) \n"  \
			"bitmap data offset	: 0x%x \n"	\
			"bitmap width		: %d \n" \
			"bitmap height		: %d \n" \
			"bitmap bpp		: %d \n" ,\
			file_header->bfSize>>10,file_header->bfOffBits, \
			info_header->biWidth,info_header->biHeight,info_header->biBitCount);
	return 0;
}
void  print_error_string(char *err_str)
{
	printf("error %s of bitmap file,%s\r\n",err_str,strerror(errno));
}
int load_bmp_data(int bmp_fd,int *bmp,BITMAPFILEHEADER *file_header,BITMAPINFOHEADER *info_header)
{
	char  err_str[20];
	
	if(0>lseek(bmp_fd,0,SEEK_SET))
	{
		sprintf(err_str,"%s","seek to start");
		print_error_string(err_str);
		return -errno;
	}
	if(0>lseek(bmp_fd,file_header->bfOffBits,SEEK_SET)) //discard header.
	{
		sprintf(err_str,"%s","read data");
		print_error_string(err_str);
		return -errno;
	}
	//begin to read real bitmap data 
	if(0> read(bmp_fd,(char*)bmp,info_header->biWidth*info_header->biHeight* \
				info_header->biBitCount/8)) //discard header.
	{
		return -errno;
	}
	return 0;
}
/************************************************************
*
*	func name : convert_bmp_data
*		para	  :  
*				bmp			framebuffer pointer
*				bmp_buffer	temp buffer which store bmp data
*				info_header	bitmap information header
*				 
*				bpp			osd bits per pixel
*
*************************************************************/
int convert_bmp_data(U32 *bmp,U32 *bmp_buffer, U32 bpp,BITMAPINFOHEADER *info_header)
{
	U8 ps=0 ;
	U8 *pt_src;
	U16 *pt_dst_16;
	U32	*pt_dst_32;
	U8  r,g,b;
	U32 i,j;
	
	if(24==info_header->biBitCount) //bitmap src bpp24
	{
		switch (bpp)
		{
			case OSD_TYPE_16_655:
			case OSD_TYPE_16_565:
			case OSD_TYPE_16_6442:
			case OSD_TYPE_16_844:
			case OSD_TYPE_16_4444_R:
			case OSD_TYPE_16_4642_R:
			case OSD_TYPE_16_1555_A:
			case OSD_TYPE_16_4444_A:	
			ps=1;
			break;	
		}
	
	pt_src=(char*)bmp_buffer ;
	if(ps) //16bpp
	{
		pt_dst_16=(U16*)bmp_buffer;
		for(i=0;i<BMP_HEIGHT;i++)
		for(j=0;j<BMP_WIDTH;j++)
		{
			b=*pt_src,g=*(pt_src+1),r=*(pt_src+2);
			*pt_dst_16=((r>>2)<<10|(g>>3)<<5|b>>3); //RGB 655 format
			pt_src+=3;
			pt_dst_16++;
		}
		bitmap16=(U16*)bmp_buffer;
		load_bitmap_local(bmp,bpp);
		bitmap16=NULL;
	}
	else //24bpp
	{
		pt_dst_32=bmp;
		for(i=0;i<BMP_HEIGHT-1;i++)
		for(j=0;j<BMP_WIDTH;j++)
		{
			*pt_dst_32++=0xff<<24|*(pt_src+2)<<16|*(pt_src+1)<<8|*pt_src;
			pt_src+=3;
			
		}
		memcpy((char*)bmp_buffer,(char*)bmp,(size_t)(BMP_HEIGHT*BMP_WIDTH*sizeof(int)));
		bitmap24=bmp_buffer;
		load_bitmap_local(bmp,bpp);
		bitmap24=NULL;
	}
	}
	return  0;
}
int load_bitmap_file(int* bmp,int bpp)
{
	int bmp_fd;
	char  err_str[20];
	BITMAPFILEHEADER bmp_file_header;
	BITMAPINFOHEADER bmp_info_header;
	int  *bmp_buffer;
	
	bmp_fd= open(BMP_FILE_NAME, O_RDONLY);
	if(bmp_fd<0)
	{
		sprintf(err_str,"%s","open");
		print_error_string(err_str);
		goto err_ret2;		
	}
	if(0> read(bmp_fd,&bmp_file_header,sizeof(BITMAPFILEHEADER)))
	{
		sprintf(err_str,"%s","read file header  ");
		print_error_string(err_str);
		goto err_ret1;
	}
	if(0> read(bmp_fd,&bmp_info_header,sizeof(BITMAPINFOHEADER)))
	{
		sprintf(err_str,"%s","read information  header  ");
		print_error_string(err_str);
		goto err_ret1;
	}
	dump_bmp_info(&bmp_file_header,&bmp_info_header);
	BMP_WIDTH=bmp_info_header.biWidth;
	BMP_HEIGHT=bmp_info_header.biHeight;
	bmp_buffer=(int*)malloc((size_t)(BMP_WIDTH*BMP_HEIGHT* sizeof(int))) ;
	if(0>load_bmp_data(bmp_fd,bmp_buffer,&bmp_file_header,&bmp_info_header))
	{
		sprintf(err_str,"%s","load bmp data ");
		print_error_string(err_str);
		goto err_ret1;
	}
	printf("loading bmp data completed\r\n");
	if(0>convert_bmp_data(bmp,bmp_buffer,bpp,&bmp_info_header))
	{
		sprintf(err_str,"%s","convert bmp data ");
		print_error_string(err_str);
		goto err_ret1;
	}
	printf("convert bmp data completed\r\n");
	if(bmp_buffer) free((void*)bmp_buffer);
	close(bmp_fd) ;
	return 0;
err_ret1:
	close(bmp_fd) ;	
err_ret2:
	if(bmp_buffer) free(bmp_buffer);
	return -errno ;	
	
}
#endif 
/********************************************************
*we will load bitmap data into buffer which is pointed by bmp and the data 
*will be convert to match color format first .
*********************************************************/
int  load_bitmap_local(int*bmp , int bpp)
{
	
	U32	*bmp_data_32,*buf_bmp_32,tmp_data;
	U16 *bmp_data_16,*buf_bmp_16;
	U8	ps;
	U8	r,g,b,a;
	U32 i,j;

	switch(bpp)
	{
		case OSD_TYPE_16_655:
		case OSD_TYPE_16_565:
		case OSD_TYPE_16_6442:
		case OSD_TYPE_16_844:
		case OSD_TYPE_16_4444_R:
		case OSD_TYPE_16_4642_R:
		case OSD_TYPE_16_1555_A:
		case OSD_TYPE_16_4444_A:
		bmp_data_16=bitmap16 ;
		buf_bmp_16=(U16*)bmp;
		ps=1;
		break;
		default :
		bmp_data_32=bitmap24 ;
		buf_bmp_32=bmp;
		ps=0;
		break;
		
	}
		
	for(i=0;i<BMP_HEIGHT;i++)
	for(j=0;j<BMP_WIDTH;j++)
	{
		if(ps)//16bit 
		{
			r=*bmp_data_16>>10; //bitmap data RGB  is 655 format
			g=(*bmp_data_16&0x03e0)>>5;
			b=*bmp_data_16&0x1f;
			switch(bpp)
			{
				case OSD_TYPE_16_655:
				//*buf_bmp_16=(b>>1|(g)<<5|(r<<1)<<10);	
				*buf_bmp_16=b|(g)<<5|(r<<10);
				break;	
				case OSD_TYPE_16_565:
				*buf_bmp_16=(b|(g<<1)<<5|(r>>1)<<11);		
		 		break;
				case OSD_TYPE_16_6442:
				*buf_bmp_16=(3|(b>>1)<<2|(g>>1)<<6|r<<10);//alpha =3	
				break;
				case OSD_TYPE_16_844:
				*buf_bmp_16=((b>>1)<<0|(g>>1)<<4|(r<<2)<<8);	
				break;
				case OSD_TYPE_16_4444_R:
				*buf_bmp_16=(0xf|(b>>1&0xf)<<4|(g>>1&0xf)<<8|(r>>2)<<12);	
				break;	
				case OSD_TYPE_16_4642_R:
				*buf_bmp_16=(0x3|(b>>1&0xf)<<2|(g<<1&0x3f)<<6|(r>>2&0xf)<<12);	
				break;
				case OSD_TYPE_16_1555_A:
				*buf_bmp_16=(1<<15|(b&0x1f)|(g&0x1f)<<5|(r>>1&0x1f)<<10);	
				break;
				case OSD_TYPE_16_4444_A:
				*buf_bmp_16=(0xf<<12|(b>>1&0xf)|(g>>1&0xf)<<4|(r>>2&0xf)<<8);	
				break;
			}
			bmp_data_16++;
			buf_bmp_16++;
		}
		else //32bit
		{
			r=(*bmp_data_32&0x00ff0000)>>16; //bitmap data is 655 format
			g=(*bmp_data_32&0x0000ff00)>>8;
			b=(*bmp_data_32&0x000000ff);
			switch(bpp)
			{
				case  OSD_TYPE_32_RGBA:
    				*buf_bmp_32=0xff|b<<8|g<<16|r<<24;//special order.
    				break;
				case OSD_TYPE_32_BGRA:
				*buf_bmp_32=(0xff|(b)<<24|(g)<<16|(r)<<8);	
				break;
				case OSD_TYPE_32_ABGR:
				*buf_bmp_32=(0xff<<24|(b)<<16|(g)<<8|(r));	
				break;	
				case OSD_TYPE_32_ARGB:
				*buf_bmp_32=(0xff<<24|(b)|(g)<<8|(r)<<16);	
				break;	
				case OSD_TYPE_24_888_B:
				*buf_bmp_32=((b)<<16|(g)<<8|(r));	
				break;
				case OSD_TYPE_24_5658:
				*buf_bmp_32=(0xff|(b>>3&0x1f)<<8|(g>>2&0x3f)<<13|(r>>3&0x1f)<<19);	
				break;
				case OSD_TYPE_24_6666_A:
				*buf_bmp_32=((0xff>>2&0x3f)<<18|(b>>2&0x3f)|(g>>2&0x3f)<<6|(r>>2&0x3f)<<12);	
				break;
				case OSD_TYPE_24_6666_R:
				*buf_bmp_32=((0xff>>2&0x3f)|(b>>2&0x3f)<<6|(g>>2&0x3f)<<12|(r>>2&0x3f)<<18);	
				break;
				case OSD_TYPE_24_8565:
				*buf_bmp_32=((0xff)<<16|(b>>3&0x1f)|(g>>2&0x3f)<<5|(r>>3&0x1f)<<11);	
				break;
				case OSD_TYPE_24_RGB:
				*buf_bmp_32=(r<<16|g<<8|b);
				break;
				
			}
			bmp_data_32++;
			buf_bmp_32++;
		}
		
	}
	return 0;
}

/************************************************************
*
*	func name : show_bitmap
*		para	  :  
*				p			framebuffer pointer
*				line_width	osd fix line width
*				xres			osd xres
*				yres			osd yres
*				bpp			osd bits per pixel
*
*************************************************************/

 int  show_bitmap(char *p, unsigned line_width, unsigned xres, unsigned yres, unsigned bpp)
{
	int x, y;
    	char *wp = p;
 	int  *bmp;
	int count=0 ;
	int * rec_pos;
	
	bmp=(int*)malloc(1920*1080*sizeof(int)+10);
	if(NULL==bmp) 
	{
		printf("Error can't alloc memory for bmp buffer\r\n");
		return -1 ;
	}
#if defined(BITMAP_USE_FILE)	
	load_bitmap_file(bmp,bpp);
#else
	BMP_WIDTH=1920 ;
	BMP_HEIGHT=1080;
	load_bitmap_local(bmp,bpp);
#endif
	yres =min(yres,BMP_HEIGHT);
	xres =min(xres,BMP_WIDTH);
    	if (bpp <= OSD_TYPE_32_ARGB && bpp >= OSD_TYPE_32_BGRA ){
	 int * bmp32=bmp;
	 rec_pos=bmp32+BMP_WIDTH*(BMP_HEIGHT-1) ;
	 
        for (y = 0; y <yres; y++) {
            wp = &p[y * line_width];
		bmp32=rec_pos;
	      for (x = 0; x < xres; x++) {
		   	
		   *wp++ = *bmp32&0xff;   
                *wp++ = *bmp32>>8&0xff;   
                *wp++ = *bmp32>>16&0xff;   
                *wp++ = *bmp32>>24&0xff;   
		   bmp32++;	
		}
		rec_pos-=BMP_WIDTH;  
	      		
        }
    }
     else if (bpp >=OSD_TYPE_16_655 && bpp <=OSD_TYPE_16_565) {
	 short* bmp16=(short *)bmp;
	  
	  rec_pos=(int *)(bmp16+BMP_WIDTH*(BMP_HEIGHT-1));
        for (y = 0; y < yres; y++) {
            wp = &p[y * line_width];
		bmp16=(short *)rec_pos ;	
            for (x = 0; x < xres; x++) {
		*wp++ = *bmp16 &0xff;	
		*wp++ =*bmp16>>8&0xff;	
		
	       bmp16++;
                
            }
		rec_pos-=BMP_WIDTH/(sizeof(int)/sizeof(short));	
			
        }
    }
    else if (bpp <= OSD_TYPE_24_RGB && bpp >=OSD_TYPE_24_6666_A ) {
	 int * bmp24=bmp;	
	rec_pos=bmp24+(BMP_WIDTH)*(BMP_HEIGHT-1)  ;
        for (y = 0; y < yres; y++) {
            wp = &p[y * line_width];
		bmp24=rec_pos ;		
            for (x = 0; x < xres  ; x++) {
      		      *wp++ = *bmp24&0xff;  
	      		*wp++ = *bmp24>>8&0xff;   
                	*wp++ = *bmp24>>16&0xff;  
		   	bmp24++;	
			 
            }
		rec_pos-=BMP_WIDTH;  		
        }
    }
	free(bmp);
	return 0;
}


void  display_color_mode_list(void)
{
	printf("color list : \n"
	"	OSD_TYPE_16_655 =9, \n" 		\
	"	OSD_TYPE_16_844 =10, \n"
	"	OSD_TYPE_16_6442 =11 , \n"	\
	"	OSD_TYPE_16_4444_R = 12, \n"	\
	"	OSD_TYPE_16_4642_R = 13, \n"	\
	"	OSD_TYPE_16_1555_A=14, \n"	\
	"	OSD_TYPE_16_4444_A = 15, \n"	\
	"	OSD_TYPE_16_565 =16, \n"		\
	"	OSD_TYPE_24_6666_A=19, \n "	\
	"	OSD_TYPE_24_6666_R=20, \n"	\
	"	OSD_TYPE_24_8565 =21, \n"	\
	"	OSD_TYPE_24_5658 = 22, \n"	\
	"	OSD_TYPE_24_888_B = 23, \n"	\
	"	OSD_TYPE_24_RGB = 24, \n"	\
	"	OSD_TYPE_32_BGRA=29, \n"	\
	"	OSD_TYPE_32_ABGR = 30, \n"	\
	"	OSD_TYPE_32_RGBA=31, \n"	\
	"	OSD_TYPE_32_ARGB=32, \n"\
	"	OSD_TYPE_YUV422=33\r\n");


}
int test_osd_gbl_alpha(void *para)
{
	int gbl_alpha;
	ge2d_op_ctl_t  ge2d_control;
    	osd_obj_t  *osd[2] ;	
	int i=0;
	/*begin  draw a figure*/
	while(1)
	{
		for (i=0;i<2;i++)
		{
			osd[i]=create_osd_obj(16,i);
			if(NULL==osd[i]) 
			{
				printf("can't create osd object for  osd%d\r\n",i);
				return -1;
			}
		}
		osd[1]->show(osd[1]);
		osd[0]->show(osd[0]);
		ge2d_control.config.op_type=OSD0_OSD0 ;
		ge2d_control.config.alu_const_color=0xff0000ff;
		ge2d_control.pt_osd=osd[0] ;
		osd[0]->ge2d_op_func.fill_rect(&ge2d_control);
		printf("please input global alpha value\n");
		scanf("%x",&gbl_alpha);
		if(gbl_alpha>0xff)
		{
			printf("invalid alpha value \n");
			return -1;
		}
		ioctl(osd[0]->fbfd,FBIOPUT_OSD_SET_GBL_ALPHA,&gbl_alpha);
		ioctl(osd[0]->fbfd,FBIOGET_OSD_GET_GBL_ALPHA,&gbl_alpha);
		printf("global alpha read back :0x%x\n",gbl_alpha);
	}	
}

int 	test_osd_colorkey(void* para)
{
	int mode ;
      	int osd_index=0;	  
    	osd_obj_t  *osd[2] ;	
	int i=0,exit=0;
	/*begin  draw a figure*/
	while (!exit)
	{
		display_color_mode_list();
		printf("please select one color mode for osd:");
		scanf("%d",&mode) ;
		for (i=0;i<2;i++)
		{
			if(mode<OSD_TYPE_16_655 || mode>OSD_TYPE_32_ARGB)
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
		osd[1]->show(osd[1]);
		osd[0]->show(osd[0]);
		char  r=0xff,g=0,b=0,a=0x20;
		int  colorkey;
		switch(mode)
		{
			case OSD_TYPE_16_655:
			colorkey=((b>>3&0x1f)|(g>>3&0x1f)<<5|(r>>2&0x3f)<<10);
			break;
			case OSD_TYPE_16_844:
			colorkey=((b>>4&0xf)<<0|(g>>4&0xf)<<4|(r)<<8);	
			break;
			case OSD_TYPE_16_565:
			colorkey=((b>>3&0x1f)|(g>>2&0x3f)<<5|(r>>3&0x1f)<<11);
			break;
			case OSD_TYPE_24_888_B:
			colorkey=((b)<<16|(g)<<8|(r));
			break;
			case OSD_TYPE_24_RGB:
			colorkey=(r<<16|g<<8|b);	
			break;
		}
		colorkey |=a<<24;
		osdSetColorKey(osd[0],colorkey);
		osdDispRect(osd[0],r<<16|g<<8|b,100,100,380,380);
		/*
		int x=215,y=344,w=29,h=50;
		
		for(i=0;i<10;i++)
		{
			osdDispRect(osd[0],(r-1)<<16|g<<8|b,x,y,x+w,y+h);
			x+=29 ;
			sleep(1);
		}*/
		printf("press Enter to exit\r\n");
		scanf("%c%c",&mode,&mode);
		destroy_osd_obj(osd[0]);
		destroy_osd_obj(osd[1]);	
	}
		
}
int   osd_order_change(void* para)
{
#define  FBIOPUT_OSD_ORDER_SET  		0x46f8
#define  FBIOPUT_OSD_ORDER_GET  		0x46f7
	osd_obj_t  *osd[2] ;
	int mode ;
	int i=0,exit=0;
	int osd_order=0x02;// 01  osd0-osd1  10  osd1-osd0

	
	display_color_mode_list();
	printf("please select one color mode for osd:");
	//scanf("%d",&mode) ;
	mode=32;
	for (i=0;i<2;i++)
	{
		if(mode<OSD_TYPE_16_655 || mode>OSD_TYPE_32_ARGB)
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
	osdDispRect(osd[0],0xffff0000,111,100,111,400);
	osdDispRect(osd[0],0xffff0000,120,100,121,400);
	return 0;
	osd[1]->show(osd[1]);
	osd[0]->show(osd[0]);
	init_key_array(osd[0]);
	printf("pay attention osd0 has rectangle array\n");
	//each time 
	while (!exit)
	{
		printf("press 'q' to exit\r\n");
		scanf("%c",&mode);
		if(mode=='q'||mode=='Q')
		{
			break;
		}
		osd_order^=0x3; 
		ioctl(osd[1]->fbfd,FBIOPUT_OSD_ORDER_SET,&osd_order);
		ioctl(osd[1]->fbfd,FBIOPUT_OSD_ORDER_GET,&i);
		printf("current osd_order:%s \r\n",i==0x01?"osd0-osd1":"osd1-osd0");
	}
	return 0;
}
 int  test_osd_display(void* para)
{
	int mode ;
      	int osd_index=0;	  
    	osd_obj_t  *osd[2] ;	
	int i,exit=0;
	/*begin  draw a figure*/
	while (!exit)
	{
		display_color_mode_list();
		printf("please input osd1 color mode:");
		scanf("%d",&mode) ;
		if(mode==99) 
		{
			exit =1;
			continue ;
		}
		
		for (i=0;i<2;i++)
		{
			osd[i]=create_osd_obj(i>0?mode:32,i);
			if(NULL==osd[i]) return -1;
		}
	
		for (i=0;i<2;i++)
		{
			if(!osd[i]->draw)
			{
				printf("draw function for this mode not impleted\r\n");
			}else
			{
				osd[i]->draw(osd[i]);
			}
			//	destroy_osd_obj(this_osd);
			sleep(3);
		}
	
		destroy_osd_obj(osd[0]);
		destroy_osd_obj(osd[1]);
	}
	return 0;
}



