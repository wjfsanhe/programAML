#include "Platform.h"
#include "Osd.h"
#include "cmem.h"
#include "DK_Graphics.h"


HDC Osd[2] = {NULL, NULL};
//HDC Osd2;

//static HDC
int OpenGe2d()
{
	/*open ge2d device */
	char ge2d_name[100];
	int fd2d = 0;
	strcpy(ge2d_name, "/dev/ge2d");
	fd2d = open(ge2d_name, O_RDWR);
	if(!fd2d)
	{
		printf("Error: cannot open ge2d device.\n");
		return -1;
	}
	//printf("The ge2d device was opened successfully.\n");
	return fd2d;
}

int CloseGe2d(int fb2d)
{
	/*open ge2d device */
	return close(fb2d);
}

static  int SetupVarInfo(int bpp, struct fb_var_screeninfo *vinfo)
{

	if(NULL == vinfo)
		return -1;

vinfo->bits_per_pixel = 32;
vinfo->red.offset = 16;
vinfo->red.length = 8;
vinfo->red.msb_right = 0;

vinfo->blue.offset = 0;
vinfo->blue.length = 8;
vinfo->blue.msb_right = 0;

vinfo->green.offset = 8;
vinfo->green.length = 8;
vinfo->green.msb_right = 0;

vinfo->transp.offset =24;
vinfo->transp.length = 8;
vinfo->transp.msb_right = 0;
vinfo->activate=FB_ACTIVATE_FORCE ;
#if 0
	if(bpp<33)
		vinfo->bits_per_pixel=default_color_format_array[bpp].bpp;
	else
		vinfo->bits_per_pixel=33;

#ifdef DOUBLE_BUFFER_SUPPORT
       vinfo->yres_virtual = vinfo->yres * 2;
#else
       vinfo->yres_virtual = vinfo->yres;
#endif
	vinfo->red.length=default_color_format_array[bpp].red_length;
	vinfo->red.offset=default_color_format_array[bpp].red_offset;
	vinfo->green.length=default_color_format_array[bpp].green_length;
	vinfo->green.offset=default_color_format_array[bpp].green_offset;
	vinfo->blue.length=default_color_format_array[bpp].blue_length;
	vinfo->blue.offset=default_color_format_array[bpp].blue_offset;
	vinfo->transp.length=default_color_format_array[bpp].transp_length;
	vinfo->transp.offset=default_color_format_array[bpp].transp_offset;
	vinfo->activate=FB_ACTIVATE_FORCE ;
#endif
	return 0;
}

int CloseOsd(int fb)
{
	return close(fb);
}

#if 0
int ioctl(int set, int ntype, void *setting)
{
	return 0;
}
#endif

int OpenOsd(int bpp,int osd_index,struct fb_var_screeninfo *vinfo,
			struct fb_fix_screeninfo *finfo,
			struct fb_var_screeninfo *save_vinfo)
{
	int fbfd ;
	char osd_name[30];
	printf("OpenOsd \n");
	if (vinfo == NULL || finfo == NULL)
		return -1;
	osd_index = osd_index>0?1:0;
	sprintf(osd_name,"/dev/fb%d",osd_index);
       printf("%s\n", osd_name);
	fbfd = open(osd_name, O_RDWR);
       printf("fbfd = %d", fbfd);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		return (-1);
	}

	//printf("The framebuffer device was opened successfully.\n");
	/* Get fixed screen information */
	if(ioctl(fbfd, FBIOGET_FSCREENINFO, finfo))
	{
		close(fbfd);
		printf("Error reading fixed information.\n");
		return (-2);
	}

	/* Get variable screen information */
	if(ioctl(fbfd, FBIOGET_VSCREENINFO, vinfo))
	{
		close(fbfd);
		printf("Error reading variable information.\n");
		return (-3);
	}
	//memcpy(save_vinfo,vinfo,sizeof(struct fb_var_screeninfo));
	*save_vinfo=*vinfo;
	//printf("+++bpp is %d\r\n",bpp);
	if(SetupVarInfo(bpp,vinfo))
	{
		printf("error setup vinfo\n");
		return (-4);
	}

	//printf("before put vscreen information\r\n");
	//vinfo->
	if(ioctl(fbfd, FBIOPUT_VSCREENINFO, vinfo) )
	{
		/* Figure out the size of the screen in bytes */
        perror("FBDEVICE");
		close(fbfd);                                     //screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

		printf("Error setting variable information.\n");
		return -3;
	}
	return  fbfd ;
}

int DefaultDrawFunc(void *para)
{
#if 0
	osd_obj_t *this_osd=para;

	//osdDispRect(this_osd,0x40000000,0,0,720,700);
	osdDispRect(this_osd,0x80ff0000,100,200,500,676);
	osdDispRect(this_osd,0x8000ff00,100,300,500,700);
	osdDispRect(this_osd,0x800000ff,100,400,500,700);
	osdDispRect(this_osd,0x80ffffff,100,50,500,150);
#endif
	return 0;
}

int  OsdShow(void *para)
{
#if 0
	osd_obj_t *this_osd=para;
	printf("bitmap loading...\r\n");
	show_bitmap(this_osd->fbp,this_osd->line_width,this_osd->vinfo->xres, \
		this_osd->vinfo->yres,this_osd->bpp);
	//osdUpdateRegion(this_osd,0 ,0,this_osd->vinfo->xres,this_osd->vinfo->yres);//anti_flick
#endif
	return 0;
}

char* GetOsdBufferPointer(int fbfd,struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo)
{
	char *buf_ptr = NULL;
#if 1 //todo mmap
	long int screensize = 0;//anti_flick
	printf("----xres_vir:%d,yres:%d,bpp:%d,fbfd %d\r\n",vinfo->xres_virtual,vinfo->yres,vinfo->bits_per_pixel,fbfd);
	screensize = vinfo->xres_virtual* vinfo->yres_virtual * vinfo->bits_per_pixel / 8;
	buf_ptr = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	//buf_ptr += vinfo->xres_virtual*vinfo->yres*vinfo->bits_per_pixel/8;
#endif
	return buf_ptr;
}
/*
int CloseOsd(osd_obj_t* osd_obj)
{
	struct fb_var_screeninfo *vinfo = osd_obj->save_vinfo;
	int  fbfd = osd_obj->fbfd;

	printf("reset osd mode in deinit_osd\r\n ");
	vinfo->activate = FB_ACTIVATE_FORCE;
 	ioctl(fbfd, FBIOPUT_VSCREENINFO, vinfo) ;
	printf("close file descriptor\r\n ");
    close(fbfd);
	close(osd_obj->ge2d_fd);
	return  0 ;
}*/

int	ReleaseOsdBufferPointer(osd_obj_t* osd_obj)
{
	//struct fb_var_screeninfo *vinfo = osd_obj->vinfo;

    return munmap(osd_obj->fbp,osd_obj->screen_size);
}

int DestoryOsdObj(osd_obj_t  *osd_obj)
{
	struct fb_var_screeninfo *vinfo = NULL;
	if(NULL != osd_obj)
	{
		ReleaseOsdBufferPointer(osd_obj);
	   	vinfo = osd_obj->save_vinfo;
		printf("reset osd mode in deinit_osd\r\n ");
		vinfo->activate = FB_ACTIVATE_FORCE;
		ioctl(osd_obj->fbfd, FBIOPUT_VSCREENINFO, osd_obj->save_vinfo) ;
		printf("close file descriptor\r\n ");
		CloseOsd(osd_obj->fbfd);
	       CloseGe2d(osd_obj->ge2d_fd);

		if(NULL != osd_obj->finfo)
			free(osd_obj->finfo);
		if(NULL != osd_obj->vinfo)
			free(osd_obj->vinfo);
		if(NULL != osd_obj->save_vinfo)
			free(osd_obj->save_vinfo);
        free(osd_obj);
	}
	return 0;
}

osd_obj_t*  GreateOsdObj(int mode,int osd_index)
{
	osd_obj_t  *osd_obj;
	char*  fbp;
	int fbfd;
//	char ge2d_name[20];
	struct fb_var_screeninfo *vinfo,*save_vinfo ;
	struct fb_fix_screeninfo *finfo  ;

	if(osd_index != FirstDC)
		osd_index = SecondDC;

	vinfo=(struct fb_var_screeninfo*)malloc(sizeof(struct fb_var_screeninfo));
	finfo= (struct fb_fix_screeninfo*)malloc(sizeof(struct fb_fix_screeninfo));
	save_vinfo=(struct fb_var_screeninfo*)malloc(sizeof(struct fb_var_screeninfo));
	//printf("select mode is %d \r\n",mode);
	fbfd = OpenOsd(mode,osd_index,vinfo,finfo,save_vinfo);
	if(fbfd <= 0)
	{
		printf("open osd device error\r\n");
		return NULL ;
	}
	osd_obj = (osd_obj_t *)malloc(sizeof(osd_obj_t));
	if(NULL == osd_obj)
	{
		printf("error alloc osd object\r\n");
		return NULL;
	}

	osd_obj->fbfd = fbfd;

	fbp = GetOsdBufferPointer(fbfd,vinfo,finfo);
	if(NULL >= fbp)
	{
		printf("get framebuffer pointer error\r\n");
		return NULL;
	}
	/*open ge2d device */
	fbfd=0;
//	strcpy(ge2d_name, "/dev/ge2d");
	fbfd = OpenGe2d(); //open(ge2d_name, O_RDWR);
	if(!fbfd)
	{
		printf("Error: cannot open ge2d device.\n");
		//return (-1);
	}
	//printf("The ge2d device was opened successfully.\n");
	osd_obj->ge2d_fd = fbfd;

	sprintf(osd_obj->name,"osd_%dbpp",vinfo->bits_per_pixel);
	printf("osd name is %s\r\n",osd_obj->name);
	osd_obj->bpp = vinfo->bits_per_pixel;
	printf("osd_obj->bpp = %d\n", osd_obj->bpp);
	osd_obj->line_width = vinfo->xres * vinfo->bits_per_pixel/8;
	//printf("+++line width=>%d\r\n",osd_obj->line_width);
	osd_obj->screen_size = osd_obj->line_width * vinfo->yres;
	osd_obj->finfo = finfo;
	osd_obj->vinfo = vinfo;
	osd_obj->save_vinfo = save_vinfo;
#ifdef DOUBLE_BUFFER_SUPPORT
//	osd_obj->fbp = fbp + osd_obj->screen_size;
	osd_obj->fbp = fbp;
#else
       osd_obj->fbp = fbp;
#endif
      //osd_obj->osdfbp = fbp;
	osd_obj->osd_index = osd_index;
	osd_obj->fill_rect = ge2d_op_fill_rect;
	osd_obj->blit = ge2d_op_blit;
	osd_obj->stretch_blit = ge2d_op_stretch_blit;
	osd_obj->blend = ge2d_op_blend;
	osd_obj->para = NULL;
	osd_obj->show = OsdShow;
    //yres_virtual

	osd_obj->nWidth = vinfo->xres;
       osd_obj->nHeight = vinfo->yres;

//	osd_obj->nBeginWidth = 0;
#ifdef DOUBLE_BUFFER_SUPPORT
//       osd_obj->nBeginHeight = vinfo->yres;
#else
//       osd_obj->nBeginHeight = 0;
#endif
	printf("osd_obj->nWidth = %d, osd_obj->nHeight = %d\n", osd_obj->nWidth, osd_obj->nHeight);
    //osd_obj->bpp
	if(osd_index == 0)
            osd_obj->type = CANVAS_OSD0;
	else
	    osd_obj->type = CANVAS_OSD1;

	osd_obj->pixelformat = default_osd_format_to_ge2d_format[mode];

	switch(mode)
	{
	case OSD_TYPE_16_655:
		osd_obj->draw = DefaultDrawFunc;
		break;
	case OSD_TYPE_16_844:
		osd_obj->draw = DefaultDrawFunc;
		break;
	case OSD_TYPE_16_4444_R:
		osd_obj->draw = DefaultDrawFunc;
		break;
	case OSD_TYPE_16_6442:
		osd_obj->draw=DefaultDrawFunc;
		break;
	case OSD_TYPE_24_RGB:
		osd_obj->draw=DefaultDrawFunc;
		break;
	case OSD_TYPE_32_ARGB:
		osd_obj->draw=DefaultDrawFunc;
		break;
	default :
		osd_obj->draw=DefaultDrawFunc;
		break;
	}
	return osd_obj;
}

osd_obj_t*  GreateObj(osd_obj_t  *old_osd_obj)
{
	osd_obj_t  *osd_obj;
	osd_obj = (osd_obj_t *)malloc(sizeof(osd_obj_t));
	if(NULL == osd_obj)
	{
		printf("error alloc mem object\r\n");
		return NULL;
	}
	*osd_obj = *old_osd_obj;

       //osd_obj
	osd_obj->type = CANVAS_ALLOC;
       osd_obj->fbp = NULL;
       //osd_obj->osdfbp = NULL;
	//osd_obj->nBeginHeight = 0;
	//osd_obj->nBeginWidth= 0;
       return osd_obj;
}

int  DestoryObj(osd_obj_t  *osd_obj)
{
//	osd_obj_t  *osd_obj;
//	osd_obj = (osd_obj_t *)malloc(sizeof(osd_obj_t));

	if(NULL != osd_obj)
	{
//	       if(osd_obj->fbp != NULL)
//                  free();
//		printf("free mem object\r\n");
		free(osd_obj);
		return 0;
	}
	return 1;
}

char * GetDCMem(osd_obj_t  *mem_obj)
{
    CMEM_AllocParams cmemParm = {CMEM_HEAP, CMEM_NONCACHED, 8};
    int nBufferLength = 0;
    
    if(NULL == mem_obj)
    {
        printf("GetDCMem mem == NULL\n");
        return NULL;
    }

    if(NULL != mem_obj->fbp)
    {
        printf("!!!GetDCMem mem_obj->fbp != NULL\n");
    }

    nBufferLength = mem_obj->line_width * mem_obj->nHeight;
    nBufferLength = (nBufferLength + 7) & ~7;
    printf("line_width = %d   %d   %d\n", mem_obj->line_width, mem_obj->nHeight, nBufferLength);
    mem_obj->fbp = (char*)CMEM_alloc(0, nBufferLength, &cmemParm);

    if(NULL == mem_obj->fbp)
    {
        printf("GetDCMem mem_obj->fbp == NULL\n");
    }

    mem_obj->planes[0].addr = CMEM_getPhys(mem_obj->fbp);
    mem_obj->planes[0].h = mem_obj->nHeight;
    mem_obj->planes[0].w = mem_obj->nWidth;

    return mem_obj->fbp;

	//old_osd_obj->fbp = NULL;
/*	osd_obj_t  *osd_obj;
	osd_obj = (osd_obj_t *)malloc(sizeof(osd_obj_t));
	if(NULL == osd_obj)
	{
		printf("error alloc mem object\r\n");
		return NULL;
	}
	*osd_obj = *old_osd_obj;

	osd_obj->type = CANVAS_ALLOC;
       osd_obj->fbp = NULL;
	return osd_obj;*/
}

void  ReleaseDCMem(osd_obj_t  *mem_obj)
{
    CMEM_AllocParams cmemParm = {CMEM_HEAP, CMEM_NONCACHED, 8};
    if(NULL != mem_obj)
    {
        if(NULL != mem_obj->fbp)
        {
            CMEM_free(mem_obj->fbp, &cmemParm);
            mem_obj->fbp = NULL;
        }
    }
}
//-----------------------------------------------------------------------------------------
/*//////////////////////////////////////////
//Interface to applications
//ctreate by jzn
//////////////////////////////////////////*/
//#define TEST
void PrintOsdInfo(osd_obj_t *pOsd)
{
	printf("Osd->nDevice = %d\n", pOsd->osd_index);
	printf("Osd->lpFrameBuffer = 0x%x\n", pOsd->fbp);

	printf("pOsd->finfo.id = %s\n", pOsd->finfo->id);
	printf("pOsd->finfo.smem_len = %d\n", pOsd->finfo->smem_len);
	printf("pOsd->finfo.type = %d\n", pOsd->finfo->type);
	printf("pOsd->finfo.type_aux = %d\n", pOsd->finfo->type_aux);
	printf("pOsd->finfo.visual = %d\n", pOsd->finfo->visual);
	printf("pOsd->finfo.xpanstep = %d\n", pOsd->finfo->xpanstep);
	printf("pOsd->finfo.ypanstep = %d\n", pOsd->finfo->ypanstep);
	printf("pOsd->finfo.ywrapstep = %d\n", pOsd->finfo->ywrapstep);
	printf("pOsd->finfo.line_length = %d\n", pOsd->finfo->line_length);
	printf("pOsd->finfo.mmio_start = %d\n", pOsd->finfo->mmio_start);
	printf("pOsd->finfo.mmio_len = %d\n", pOsd->finfo->mmio_len);
	printf("pOsd->finfo.accel = %d\n", pOsd->finfo->accel);

	printf("\n");
	printf("pOsd->vinfo.xres=%d\n", pOsd->vinfo->xres);
	printf("pOsd->vinfo.yres=%d\n", pOsd->vinfo->yres);
	printf("pOsd->vinfo.xres_virtual=%d\n", pOsd->vinfo->xres_virtual);
	printf("pOsd->vinfo.yres_virtual=%d\n", pOsd->vinfo->yres_virtual);
	printf("pOsd->vinfo.xoffset=%d\n", pOsd->vinfo->xoffset);
	printf("pOsd->vinfo.yoffset=%d\n", pOsd->vinfo->yoffset);
	printf("pOsd->vinfo.grayscale=%d\n", pOsd->vinfo->grayscale);
	printf("pOsd->vinfo.nonstd=%d\n", pOsd->vinfo->nonstd);
	printf("pOsd->vinfo.activate=%d\n", pOsd->vinfo->activate);
	printf("pOsd->vinfo.height=%d\n", pOsd->vinfo->height);
	printf("pOsd->vinfo.width=%d\n", pOsd->vinfo->width);
	printf("pOsd->vinfo.pixclock=%d\n", pOsd->vinfo->pixclock);
	printf("pOsd->vinfo.left_margin=%d\n", pOsd->vinfo->left_margin);
	printf("pOsd->vinfo.right_margin=%d\n", pOsd->vinfo->right_margin);
	printf("pOsd->vinfo.upper_margin=%d\n", pOsd->vinfo->upper_margin);
	printf("pOsd->vinfo.lower_margin=%d\n", pOsd->vinfo->lower_margin);
	printf("pOsd->vinfo.hsync_len=%d\n", pOsd->vinfo->hsync_len);
	printf("pOsd->vinfo.vsync_len=%d\n", pOsd->vinfo->vsync_len);
	printf("pOsd->vinfo.sync=%d\n", pOsd->vinfo->sync);
	printf("pOsd->vinfo.vmode=%d\n", pOsd->vinfo->vmode);
	printf("pOsd->vinfo.rotate=%d\n", pOsd->vinfo->rotate);
	printf("pOsd->vinfo.bits_per_pixel=%d\n", pOsd->vinfo->bits_per_pixel);
	printf("pOsd->vinfo.red.length=%d\n", pOsd->vinfo->red.length);
	printf("pOsd->vinfo.red.offset=%d\n", pOsd->vinfo->red.offset);
	printf("pOsd->vinfo.green.length=%d\n", pOsd->vinfo->green.length);
	printf("pOsd->vinfo.green.offset=%d\n", pOsd->vinfo->green.offset);
	printf("pOsd->vinfo.blue.length=%d\n", pOsd->vinfo->blue.length);
	printf("pOsd->vinfo.blue.offset=%d\n", pOsd->vinfo->blue.offset);
	printf("pOsd->vinfo.transp.length=%d\n", pOsd->vinfo->transp.length);
	printf("pOsd->vinfo.transp.offset=%d\n", pOsd->vinfo->transp.offset);
	printf("pOsd->vinfo.activate=%d\n", pOsd->vinfo->activate);
}

int DK_SysInit()
{
#ifdef TEST
       printf("DK_SysInit\n");
       rectangle_t this_rect, this_rect1, this_rect5;
       int i = 0;
       int nn;
       THRGB color;
       HDC A1 = NULL;
       HDC A2 = NULL;
       HDC A3 = NULL;
       HDC A4 = NULL;
       HDC A5 = NULL;

#endif

	CMEM_init();
	Osd[0] = GreateOsdObj(OSD_TYPE_32_RGBA, 0);
    PrintOsdInfo(Osd[0]);
#ifdef TEST
#if 1
    printf("1111111111111\n");
/*    PrintOsdInfo(Osd[0]);
    memset(Osd[0]->fbp, 0xFF, Osd[0]->nHeight * Osd[0]->nWidth * Osd[0]->bpp / 8);
    for(i = 0; i < 100; i++)
    {
       switch(i % 4)
       {
           case 0:
            color = 0xFF0000FF;
            break;
           case 1:
            color = 0xFF00FF;
            break;
           case 2:
            color = 0xFFFF;
            break;
           case 3:
            color = 0x000000FF;
            break;
       }
       printf("%d     %x\n", i , color);
       this_rect.x = 0;
       this_rect.y = 0;
       this_rect.w = Osd[0]->nWidth;
       this_rect.h = Osd[0]->nHeight;

       DK_FillRect(Osd[0], this_rect, color);
//       sleep(1);
    }

    A1 = DK_CreateCompatibleDCEx(Osd[0], 200, 200);
    printf("bbbbbbbbbbbbb\n");
    //A2 = DK_CreateDCFromBitmap("/mnt/disk/2.bmp");
   //A3 = DK_CreateDCFromBitmap("/mnt/disk/3.bmp");
    //A4 = DK_CreateDCFromBitmap("/mnt/disk/4.bmp");
    A5 = DK_CreateCompatibleDCEx(Osd[0], 400, 400);

    //DK_BitBlt(A1, 0, 0, A2->nWidth, A2->nHeight, A2, 0, 0, 0);
    //DK_BitBlt(A1, 200, 100, A3->nWidth, A3->nHeight, A3, 0, 0, 0);
    //DK_BitBlt(A1, 400, 100, A4->nWidth, A4->nHeight, A4, 0, 0, 0);


    //this_rect.x=0;
    //this_rect.y=0;
    //this_rect.w=A1->nWidth;
    //this_rect.h=A1->nHeight;

    this_rect1.x=0;
    this_rect1.y=0;
    this_rect1.w=A1->nWidth;
    this_rect1.h=A1->nHeight;

    this_rect5.x  = 0;
    this_rect5.y  = 0;
    this_rect5.w = A5->nWidth;
    this_rect5.h  = A5->nHeight;

    DK_FillRect(A1, this_rect1, 0xFFFF00FF);
    DK_FillRect(A5, this_rect5, 0xFFFFFF96);
*/
    this_rect.x = 0;
    this_rect.y = 0;
    this_rect.w = Osd[0]->nWidth;
    this_rect.h = Osd[0]->nHeight;
    printf("back buff  fill of 0xFF0000FF\n");
    DK_FillRect(Osd[0], this_rect, 0xFF0000FF);
    sleep(1);

    printf("DK_UpdateRegion\n");
    DK_UpdateRegion(Osd[0], NULL);

    sleep(1);

    printf("back buff  fill of 0xFFFF\n");

    DK_FillRect(Osd[0], this_rect, 0xFFFF);
    sleep(1);

    printf("DK_UpdateRegion\n");
    DK_UpdateRegion(Osd[0], NULL);
    sleep(1);

    double begin = mysecond();
    for(i = 0; i < 10; i++)
    {
       printf("1111111111111111\n");
        DK_UpdateRegion(Osd[0], NULL);
       printf("222222222222222\n");
        //sleep(1);
    }
    A1 = DK_CreateDCFromBitmap("/mnt/disk/1.bmp");
//    DK_BitBlt(Osd[0], 0, 0, Osd[0]->nWidth, Osd[0]->nHeight, A1, 0, 0, 0);
    DK_UpdateRegion(Osd[0], NULL);
    printf("1000 times need time %lf and ps = %f\n", mysecond() - begin, 1000.0 / (mysecond() - begin));
    sleep(2);
#endif
#endif
	Osd[1] = GreateOsdObj(OSD_TYPE_32_RGBA, 1);
    //Osd2;
	//aaa
#ifdef TEST
       PrintOsdInfo(Osd[1]);
       printf("DK_SysInit end\n");
/*       this_rect.x = 0;
       this_rect.y = 0;
       this_rect.w = Osd[1]->nWidth;
       this_rect.h = Osd[1]->nHeight;

       A1 = DK_CreateDCFromBitmap("/mnt/disk/1.bmp");
       A2 = DK_CreateDCFromBitmap("/mnt/disk/2.bmp");

       DK_BitBlt(Osd[1], 0, 0, Osd[1]->nWidth, Osd[1]->nHeight, A1, 0, 0, 0);
       printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
       sleep(5);
       printf("cccccccccccccccccccccccccccccccccccccc\n");
       DK_UpdateRegion(Osd[1], NULL);
       printf("cccccccccccccccccccccccccccccccccccccc\n");
       sleep(5);


       DK_BitBlt(Osd[0], 0, 0, Osd[0]->nWidth, Osd[0]->nHeight, A1, 0, 0, 0);
       printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
       sleep(5);
       printf("cccccccccccccccccccccccccccccccccccccc\n");
       DK_UpdateRegion(Osd[0], NULL);
       printf("cccccccccccccccccccccccccccccccccccccc\n");
       sleep(5);

       nn = 1000;
       while(1)
       {
           DK_FillRect(Osd[0], this_rect, 0x00000000);
           DK_BitBlt(Osd[0], nn, 0, A2->nWidth, A2->nHeight, A2, 0, 0, 0);
           DK_UpdateRegion(Osd[0], NULL);
           nn-=5;
           if(nn < 0)
               nn = 1000;
           usleep(10000);
       }*/

    DK_BitBlt(Osd[1], 0, 0, A1->nWidth, A1->nHeight, A1, 0, 0, 0);
    DK_UpdateRegion(Osd[1], NULL);

    DK_BitBlt(Osd[1], 0, 0, A1->nWidth, A1->nHeight, A1, 0, 0, 0);
    DK_UpdateRegion(Osd[1], NULL);

    A2 = DK_CreateDCFromBitmap("/mnt/disk/2.bmp");

    DK_FillRect(Osd[0], this_rect, 0x00000000);
    DK_UpdateRegion(Osd[0], NULL);
    nn = 1000;
    while(1)
       {
           DK_FillRect(Osd[0], this_rect, 0x00000000);
           DK_BitBlt(Osd[0], nn, 0, A2->nWidth, A2->nHeight, A2, 0, 0, 0);
           DK_UpdateRegion(Osd[0], NULL);
           nn-=5;
           if(nn < 0)
               nn = 1000;
           usleep(10000);
       }
       printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n");
       //sleep(5);
       printf("cccccccccccccccccccccccccccccccccccccc\n");

       printf("cccccccccccccccccccccccccccccccccccccc\n");
       //sleep(5);
#endif
	return 0;
}

//todo
int DK_SysDestory()
{
	DestoryOsdObj(Osd[0]);
	DestoryOsdObj(Osd[1]);
	CMEM_exit();
	return 0;
}

//µÃµ½DC
//
//
HDC DK_GetDC(int nDC)
{
	if(nDC != SecondDC)
		nDC = FirstDC;
	return Osd[nDC];
}

//ÊÍ·ÅDC
int DK_ReleaseDC(HDC hdc)
{
	//if(nDC != FirstDC)
	//	nDC = SecondDC;
	return 0;
}

HDC  DK_CreateCompatibleDCEx(HDC hdc, int nWidth, int nHeight)
{
    HDC newmemdc = GreateObj(hdc);
    if(NULL == newmemdc)
    {
        printf("crreate new mem dc eror\n");
        return NULL;
    }


    newmemdc->nHeight = nHeight;
    newmemdc->nWidth = nWidth;
    newmemdc->line_width = nWidth * newmemdc->bpp / 8;
    newmemdc->line_width = (newmemdc->line_width + 7) & ~7;

    if(NULL == GetDCMem(newmemdc))
	{
		printf("Create memdc error:not mem\n");
		DestoryObj(newmemdc);
		newmemdc = NULL;
	}
	return (HDC)newmemdc;
}

HDC  DK_CreateCompatibleDC(HDC hdc)
{
	return DK_CreateCompatibleDCEx(hdc, hdc->nWidth, hdc->nHeight);
	//return NULL;
}

HDC  DK_CreateDC(HDC hdc, int bpp, UINT32 pixelformat, int nWidth, int nHeight)
{
    HDC newmemdc = GreateObj(hdc);
    if(NULL == newmemdc)
    {
        printf("crreate new mem dc eror\n");
        return NULL;
    }

    newmemdc->bpp = bpp;
    newmemdc->pixelformat = pixelformat;
    newmemdc->nHeight = nHeight;
    newmemdc->nWidth = nWidth;
    newmemdc->line_width = nWidth * newmemdc->bpp / 8;
    newmemdc->line_width = (newmemdc->line_width + 7) & ~7;
    if(NULL == GetDCMem(newmemdc))
    {
        printf("Create memdc error:not mem\n");
        DestoryObj(newmemdc);
        newmemdc = NULL;
    }
    return (HDC)newmemdc;
}

int DK_DeleteDC(HDC hdc)
{
	if(!hdc || (hdc->type != CANVAS_ALLOC))
		return 0;
	ReleaseDCMem(hdc);
	DestoryObj(hdc);
	return 1;
}

int DK_DeleteCompatibleDC(HDC hdc)
{
    return DK_DeleteDC(hdc);
}


int DK_FillDCWithBitmap(HDC hdc, int left, int top, int width, int height, BITMAP *pBitmap, char *pColor)
{
	return DK_FillDCWithBitmapPartDirect(hdc, left, top, width, height,
		pBitmap->bmWidth, pBitmap->bmHeight, pBitmap, 0, 0, NULL);
}


int DK_FillDCWithBitmapPartDirect(HDC hdc, int x, int y, int w, int h,
					  int bw, int bh, const BITMAP* bmp, int xo, int yo, THRGB *pal)
{
      	int x1, y1;
       Uint8 *srcrow, *dstrow;

       if(NULL == hdc)
            return 1;
       if(NULL == bmp)
            return 2;
       if(x > hdc->nWidth)
            return 3;
       if(y > hdc->nHeight)
            return 4;

       if(xo > (int)bmp->bmWidth)
            return 5;
       if(yo > (int)bmp->bmHeight)
            return 6;

       if(x < 0)
           x = 0;
       if(y < 0)
           y = 0;

       if(xo < 0)
           xo = 0;
       if(yo < 0)
           yo = 0;

	if(bw <= 0)
		bw =(int) bmp->bmWidth;
	if(bh <= 0)
		bh =(int) bmp->bmHeight;

	if(bw > (int)bmp->bmWidth)
		bw = (int)bmp->bmWidth;
	if(bh > (int) bmp->bmHeight)
		bh = (int)bmp->bmHeight;

       if(x + w > hdc->nWidth)
            w = hdc->nWidth - x;

       if(y + h > hdc->nHeight)
            h = hdc->nHeight - y;

       if(xo + w > bw)
           w = bw - xo;
       if(yo + h > bh)
           h = bh -yo;

    dstrow = (Uint8 *) hdc->fbp+ y * hdc->line_width + x * hdc->bpp / 8;
    srcrow = (Uint8 *) bmp->bmBits + yo * bmp->bmPitch + xo * bmp->bmBytesPerPixel;

    if (bmp->bmType == BMP_TYPE_COLORKEY)
    {
        for (y1 = 0; y1< h; y1++)
	 {
	     Uint32  *dstpixels = (Uint32 *)dstrow;
	     Uint32  *srcpixels = (Uint32 *)srcrow;
	     for(x1 = 0; x1< w; x1++)
	     {
	         if(((*(srcpixels + x1)) | 0xff000000) == (0xff000000 | bmp->bmColorKey))
	        {
	             continue;
	        }
               else
	        {
	             *(dstpixels+x1) = *(srcpixels+x1);
	         }
	     }
	     srcrow += bmp->bmPitch;
	     dstrow += hdc->line_width;
        }
    }
    else
    {
	 w = w *4;
	 for (y1 = 0; y1< h; y1++)
	 {
	 //myprintf("h = %d, w = %d, y1= %d\n", h, w, y1);
	 //myprintf("bmWidth = %d, bmHeigh = %d, y1= %d\n", h, w, y1);
	 Uint32 *dstpixels = (Uint32 *)dstrow;
	 Uint32  *srcpixels = (Uint32 *)srcrow;
	 memcpy(dstpixels, srcpixels, w);
	 srcrow += bmp->bmPitch;
	 dstrow += hdc->line_width;
	 }
    }
    return 0;
}


void DK_BitBltBlend(HDC dstdc, int nXDest, int nYDest, int nWidth, int nHeight, HDC srcdc, int nXSrc, int nYSrc, DWORD dwRop)
{
    rectangle_t    dst_rect, src_rect, src_rect2;

    dst_rect.x = nXDest;
    dst_rect.y = nYDest;
    dst_rect.w = nWidth;
    dst_rect.h = nHeight;

    src_rect.x = nXSrc;
    src_rect.y = nYSrc;
    src_rect.w = nWidth;
    src_rect.h = nHeight;

    src_rect2.x = nXDest;
    src_rect2.y = nYDest;
    src_rect2.w = nWidth;
    src_rect2.h = nHeight;

    printf("DK_BitBltBlend \n");
    ge2d_op_blend(dstdc, dst_rect, srcdc, src_rect, dstdc, src_rect2);
    printf("DK_BitBltBlend end\n");
}



void DK_BitBlt(HDC dstdc, int nXDest, int nYDest, int nWidth, int nHeight,
		HDC srcdc, int nXSrc, int nYSrc, DWORD dwRop)
{
#if 0
	TDRect rect1,rect2;
	//bsurface_settings settings;

	rect2.x = nXDest;
	rect2.y = nYDest;
	rect2.width = nWidth;
	rect2.height = nHeight;
	rect1.x = nXSrc;
	rect1.y = nYSrc;
	rect1.width = nWidth;
	rect1.height= nHeight;

	TH_surface_copy(nSurfaceDst, &rect2, nSurfaceSrc, &rect1);
#else
    rectangle_t    dst_rect, src_rect;
    dst_rect.x = nXDest;
    dst_rect.y = nYDest;
    dst_rect.w = nWidth;
    dst_rect.h = nHeight;

    src_rect.x = nXSrc;
    src_rect.y = nYSrc;
    src_rect.w = nWidth;
    src_rect.h = nHeight;
    //printf("ge2d_op_blit");
    ge2d_op_blit(dstdc, dst_rect, srcdc, src_rect);
    //printf("ge2d_op_blit end\n");
#endif
}
// 1  invisible   0 visible

void DK_BitBltEx(HDC dsthdc, int dx, int dy, int dw, int dh,
			HDC srcdc1, int sx1, int sy1, int sw1, int sh1,
			HDC srcdc2, int sx2, int sy2, int sw2, int sh2,
			DWORD operation,
			DWORD pixel1,			 //1st constant value specified by some operations
			DWORD pixel2,			 //2nd constant value specified by some operations
			DWORD alpha1,			 //1st constant value specified by some operations
			DWORD alpha2			 //2nd constant value specified by some operations
			)
{
#if 0
	TDRect rect, rect1, rect2;

	rect.x = dx;
    rect.y = dy;
	rect.width = dw;
	rect.height = dh;

	rect1.x = sx1;
	rect1.y = sy1;
	rect1.width = sw1;
	rect1.height = sh1;
	rect2.x = sx2;
	rect2.y = sy2;
	rect2.width = sw2;
	rect2.height= sh2;

	TH_surface_blit(nSurfaceDst, &rect, operation, nSurfaceSrc1, &rect1, nSurfaceSrc2, &rect2, pixel1, pixel2);
#endif
    rectangle_t    dst_rect, src_rect, src_rect2;

    dst_rect.x = dx;
    dst_rect.y = dy;
    dst_rect.w = dw;
    dst_rect.h = dh;

    src_rect.x = sx1;
    src_rect.y = sy1;
    src_rect.w = sw1;
    src_rect.h = sh1;

    src_rect2.x = sx2;
    src_rect2.y = sy2;
    src_rect2.w = sw2;
    src_rect2.h = sh2;

    printf("ge2d_op_blend \n");
    ge2d_op_blend(dsthdc, dst_rect, srcdc1, src_rect, srcdc2, src_rect2);
    printf("ge2d_op_blend end\n");
}

//ge2d_op_stretch_blit
void DK_StretchBitBlt(HDC dstdc, int nXDest, int nYDest, int nWDest, int nHDest,
		HDC srcdc, int nXSrc, int nYSrc, int nWSrc, int nHSrc, DWORD dwRop)
{
#if 0
	TDRect rect1,rect2;
	//bsurface_settings settings;

	rect2.x = nXDest;
	rect2.y = nYDest;
	rect2.width = nWidth;
	rect2.height = nHeight;
	rect1.x = nXSrc;
	rect1.y = nYSrc;
	rect1.width = nWidth;
	rect1.height= nHeight;

	TH_surface_copy(nSurfaceDst, &rect2, nSurfaceSrc, &rect1);
#endif
    rectangle_t    dst_rect, src_rect;
    dst_rect.x = nXDest;
    dst_rect.y = nYDest;
    dst_rect.w = nWDest;
    dst_rect.h = nHDest;

    src_rect.x = nXSrc;
    src_rect.y = nYSrc;
    src_rect.w = nWSrc;
    src_rect.h = nHSrc;
    printf("ge2d_op_stretch_blit");
    ge2d_op_stretch_blit(dstdc, dst_rect, srcdc, src_rect);
    printf("ge2d_op_stretch_blit end\n");
}

void DK_FillRect(HDC hdc, rectangle_t this_rect, THRGB pal)
{
   //if();
   //this_rect.
   //this_rect.y += hdc->nBeginHeight;
   ge2d_op_fill_rect(hdc, this_rect, pal);
}

void DK_InVisable(HDC this_osd,int enable)
{
    if(this_osd->type != CANVAS_OSD1 && this_osd->type != CANVAS_OSD0)
        return;
    osd_set_invisable(this_osd, enable);
}

void DK_UpdateRegion(HDC this_osd, rectangle_t *this_rect)
{
#ifdef DOUBLE_BUFFER_SUPPORT
    if(this_osd->type != CANVAS_OSD0 && this_osd->type != CANVAS_OSD1)
        return;

    osd_update_region(this_osd,this_rect);
#endif
}

//void DK_DCMemset(HDC this_osd, )
//------------------------------------------------------------------------------------------
int convert_bmp_data(char *bmp,char  *bmp_buffer, U32 bpp, int nwidth, int nheight)
{
//	U8 ps=0 ;
//	U8 *pt_src;
//	U16 *pt_dst_16;
//	U32	*pt_dst_32;
//	U8  r,g,b;
	int i;
//todo convert
     int linew = nwidth * bpp / 8;
     int linewbmp = (linew + 3) & ~3;
     int newlinew = (linew + 7) & ~7;
     printf("bmp = %x   %x    %d    %d   %d\n", bmp, bmp_buffer, linew, linewbmp, newlinew);
     for(i = 0; i < nheight; i++)
     {
         //printf("%d\n", i);
         memcpy(bmp + i * newlinew, bmp_buffer + (nheight - i - 1) * linewbmp, linew);
     }

#if 0
	if(24==bpp) //bitmap src bpp24
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
#endif
	return  0;
}

static int  dump_bmp_info(BITMAPFILEHEADER *file_header,BITMAPINFOHEADER *info_header)
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

static void  print_error_string(char *err_str)
{
	printf("error %s of bitmap file,%s\r\n",err_str,strerror(errno));
}

static int load_bmp_data(int bmp_fd,char *bmp,BITMAPFILEHEADER *file_header,BITMAPINFOHEADER *info_header)
{
	char  err_str[20];
	int nlinewidth = 0;
	if(0>lseek(bmp_fd,0,SEEK_SET))
	{
		sprintf(err_str,"%s","seek to start");
		print_error_string(err_str);
		return -errno;
	}
	if(0>lseek(bmp_fd, file_header->bfOffBits, SEEK_SET)) //discard header.
	{
		sprintf(err_str,"%s","read data");
		print_error_string(err_str);
		return -errno;
	}
	//begin to read real bitmap data
	nlinewidth = info_header->biWidth * info_header->biBitCount / 8;
       nlinewidth = (nlinewidth + 3) & ~3;
	if(0> read(bmp_fd,(char*)bmp, nlinewidth  * info_header->biHeight )) //discard header.
	{
		return -errno;
	}
	return 0;
}

HDC DK_CreateDCFromBitmap(char *filename)
{
	int bmp_fd;
	int nlinewidth = 0;
	char  err_str[20];
	BITMAPFILEHEADER bmp_file_header;
	BITMAPINFOHEADER bmp_info_header;
	char  *bmp_buffer = NULL;
	HDC gdc = DK_GetDC(0);
	HDC hdc = NULL;
	int pixelformat;
    printf("-........ filename = %s \n ",filename);
	bmp_fd= open(filename, O_RDONLY);
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

	dump_bmp_info(&bmp_file_header, &bmp_info_header);
	//nWidth=bmp_info_header.biWidth;
	//nHeight=bmp_info_header.biHeight;

       if(bmp_info_header.biBitCount == 16)
           pixelformat = GE2D_FORMAT_S16_RGB_565;
       else if(bmp_info_header.biBitCount == 24)
           pixelformat = GE2D_FORMAT_S24_RGB;
       else if(bmp_info_header.biBitCount == 32)
           pixelformat = GE2D_FORMAT_S32_ARGB;
       else
       {
		sprintf(err_str,"%s","not support bmp format\n ");
		print_error_string(err_str);
		goto err_ret1;

       }

       nlinewidth = bmp_info_header.biWidth * bmp_info_header.biBitCount / 8;
       nlinewidth = (nlinewidth + 3) & ~3;
	bmp_buffer=(char *)malloc(nlinewidth * bmp_info_header.biHeight);
	if(0>load_bmp_data(bmp_fd, bmp_buffer, &bmp_file_header, &bmp_info_header))
	{
		sprintf(err_str,"%s","load bmp data ");
		print_error_string(err_str);
		goto err_ret1;
	}

	printf("loading bmp data completed\r\n");

       hdc = DK_CreateDC(gdc, bmp_info_header.biBitCount, pixelformat,
                                   bmp_info_header.biWidth, bmp_info_header.biHeight);
       if(NULL == hdc)
       {
        	sprintf(err_str,"%s","DK_CreateDC error\n");
		print_error_string(err_str);
		goto err_ret1;
       }
printf("lDK_CreateDC completed\r\n");
	if(0>convert_bmp_data(hdc->fbp, (char*)bmp_buffer, bmp_info_header.biBitCount,
                                   bmp_info_header.biWidth, bmp_info_header.biHeight))
	{
		sprintf(err_str,"%s","convert bmp data ");
		print_error_string(err_str);
		goto err_ret1;
	}
       printf("CMEM_cacheInv(hdc->fbp, hdc->line_width * hdc->nHeight);\n");
    //   CMEM_cacheInv(hdc->fbp, hdc->line_width * hdc->nHeight);
	printf("convert bmp data completed\r\n");
	if(bmp_buffer)
            free((void*)bmp_buffer);
	close(bmp_fd) ;
        return hdc;
err_ret1:
	close(bmp_fd) ;
err_ret2:
	if(bmp_buffer)
            free(bmp_buffer);
       if(NULL != hdc)
            DK_DeleteDC(hdc);
	return NULL ;

}
//-------------------------------------------------------------------
#if 0
HDC DK_FillDCFromBitmap(HDC hdc, char *filename)
{
    	int bmp_fd;
	char  err_str[20];
	BITMAPFILEHEADER bmp_file_header;
	BITMAPINFOHEADER bmp_info_header;
	int  *bmp_buffer;
       int nWidth = 0;
       int nHeight = 0;
       int pixelformat;

	bmp_fd= open(filename, O_RDONLY);
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

	dump_bmp_info(&bmp_file_header, &bmp_info_header);
	//nWidth=bmp_info_header.biWidth;
	//nHeight=bmp_info_header.biHeight;

       if(bmp_info_header.biBitCount == 16)
           pixelformat = GE2D_FORMAT_S16_RGB_565;
       else if(bmp_info_header.biBitCount == 24)
           pixelformat = GE2D_FORMAT_S24_RGB;
       else if(bmp_info_header.biBitCount == 32)
           pixelformat = GE2D_FORMAT_S32_ARGB;
       else
       {
		sprintf(err_str,"%s","not support bmp format\n ");
		print_error_string(err_str);
		goto err_ret1;

       }

	bmp_buffer=(int*)malloc(bmp_info_header.biWidth * bmp_info_header.biHeight * bmp_info_header.biBitCount / 8) ;
	if(0>load_bmp_data(bmp_fd, bmp_buffer, &bmp_file_header, &bmp_info_header))
	{
		sprintf(err_str,"%s","load bmp data ");
		print_error_string(err_str);
		goto err_ret1;
	}

	printf("loading bmp data completed\r\n");
	if(0>convert_bmp_data(hdc->fbp, bmp_buffer, bmp_info_header.biBitCount,
                                   bmp_info_header.biWidth, bmp_info_header.biHeight))
	{
		sprintf(err_str,"%s","convert bmp data ");
		print_error_string(err_str);
		goto err_ret1;
	}
	printf("convert bmp data completed\r\n");
	if(bmp_buffer)
            free((void*)bmp_buffer);
	close(bmp_fd) ;
       printf("hdc = %x\r\n", hdc);
        return hdc;
err_ret1:
	close(bmp_fd) ;
err_ret2:
	if(bmp_buffer)
            free(bmp_buffer);
       if(NULL != hdc)
            DK_DeleteDC(hdc);
	return NULL ;

}
#endif

int  DK_GetOsdWidth(HDC this_osd)
{
	if(this_osd != NULL)
	{
		return this_osd->nWidth;
	}
	return 0;
}
int  DK_GetOsdHeigh(HDC this_osd)
{
	if(this_osd != NULL)
	{
		return this_osd->nHeight;
	}
	return 0;
}
//------------------------------------------------------------------------------------------
