#include <stdio.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/fb.h>

#include "amstream.h"
#include "vformat.h"
#include "jpegdec.h"
#include "cmem.h"
#include "ge2d.h"

enum {
	DEC_STAT_DECDEV,
	DEC_STAT_INFOCONFIG,
	DEC_STAT_INFO,
	DEC_STAT_DECCONFIG,
	DEC_STAT_RUN
};

#define READ_UNIT (32*1024)
#define CANVAS_ALIGNED(x)	(((x) + 7) & ~7)
#define CMEM_ALLOC
#define DEC_OUTPUT_WIDTH	720
#define DEC_OUTPUT_ADDR_Y	0x85df0000
#define DEC_OUTPUT_ADDR_U	0x85ff0000
#define DEC_OUTPUT_ADDR_V	0x86070000
int  fd_amport, fd_jpegdec;
void disable_osd(void)
{
	int fd;
	
	fd = open("/dev/fb0", O_RDWR);
	
	if (fd >= 0) {
		ioctl(fd, FBIOBLANK, 0);
		close(fd);
	}
	else {
		printf("Error: Can not open /dev/fb0.");
	}
	
	fd = open("/dev/fb1", O_RDWR);
	
	if (fd >= 0) {
		ioctl(fd, FBIOBLANK, 0);
		close(fd);
	}
	else {
		printf("Error: Can not open /dev/fb0.");
	}
}
int JpegDecode(char *lpName)
{
	int fd_file,err, decLoop = 1;
	unsigned char buf[READ_UNIT];
	int bufSize, padding = 0;
	unsigned decState;
	jpegdec_info_t info;
	jpegdec_config_t config;
	int s = DEC_STAT_INFOCONFIG;
	CMEM_AllocParams cmemParm = {CMEM_HEAP, CMEM_NONCACHED, 8};
	unsigned char *planes[4] = {NULL, NULL, NULL,NULL};

    config_para_t ge2d_config;
    ge2d_op_para_t op_para;
	
	if (lpName == NULL) {
		printf("Usage: jpegdec <filename>.\n");
		return -1;
	}

	fd_file = open(lpName, O_RDONLY);
	if (fd_file < 0) {
		printf("Can not open target file.\n");
		return -2;
	}

	
	bufSize=0;

	while (decLoop) {
		decState = ioctl(fd_jpegdec, JPEGDEC_IOC_STAT);

		if (decState & JPEGDEC_STAT_ERROR) {
			printf("jpegdec error\n");
			break;
		}

		if (decState & JPEGDEC_STAT_UNSUPPORT) {
			printf("jpegdec unsupported format\n");
			break;
		}

		if (decState & JPEGDEC_STAT_DONE && padding==1) {
			printf("jpegdec done\n");
			break;
		}
		else
		{
			if(padding==0 && decState & JPEGDEC_STAT_DONE)
			decState=JPEGDEC_STAT_WAIT_DATA;	
		}
		

		if (decState & JPEGDEC_STAT_WAIT_DATA) {
			/* feed data */
			if (bufSize > 0) {
				if (write(fd_amport, buf, bufSize) == bufSize) {
					bufSize = 0;
				}
			}
			
			/* refill buffer */
			if ((bufSize == 0) && (padding == 0)) {
				bufSize = read(fd_file, buf, READ_UNIT);
						
				if (bufSize == 0) {
					memset(buf, 0, READ_UNIT);
					bufSize = READ_UNIT;
					padding = 1;
printf("feeding data done\n");
				}
			}
		}
		
		switch (s) {
		case DEC_STAT_INFOCONFIG:
			if (decState & JPEGDEC_STAT_WAIT_INFOCONFIG) {
				ioctl(fd_jpegdec, JPEGDEC_IOC_INFOCONFIG, 0);
				s = DEC_STAT_INFO;
			}
			break;

		case DEC_STAT_INFO:
			if (decState & JPEGDEC_STAT_INFO_READY) {
				ioctl(fd_jpegdec, JPEGDEC_IOC_INFO, &info);
				printf("Jpeg info: (%dx%d), comp_num %d\n",
					info.width, info.height, info.comp_num);

#ifdef CMEM_ALLOC
				planes[0] = (unsigned char *)CMEM_alloc(0, 
					CANVAS_ALIGNED(info.width) * info.height, &cmemParm);
				planes[1] = (unsigned char *)CMEM_alloc(0,
					CANVAS_ALIGNED(info.width/2) * info.height/2, &cmemParm);
				planes[2] = (unsigned char *)CMEM_alloc(0,
					CANVAS_ALIGNED(info.width/2) * info.height/2, &cmemParm);


//				planes[3]= (unsigned char *)CMEM_alloc(0, 
//					CANVAS_ALIGNED(info.width) * info.height*4, &cmemParm); //RGB32BIT.


				if ((!planes[0]) || (!planes[1]) || (!planes[2])/* || (!planes[3])*/) {
					printf("Not enough memory\n");
					decLoop = 0;
					break;
				}
#endif

				s = DEC_STAT_DECCONFIG;
			}
			break;

		case DEC_STAT_DECCONFIG:
			if (decState & JPEGDEC_STAT_WAIT_DECCONFIG) {
#ifdef CMEM_ALLOC
				config.addr_y = CMEM_getPhys(planes[0]);
				config.addr_u = CMEM_getPhys(planes[1]);
				config.addr_v = CMEM_getPhys(planes[2]);
				config.canvas_width = CANVAS_ALIGNED(info.width);
#else
				config.addr_y = DEC_OUTPUT_ADDR_Y;
				config.addr_u = DEC_OUTPUT_ADDR_U;
				config.addr_v = DEC_OUTPUT_ADDR_V;
				config.canvas_width = DEC_OUTPUT_WIDTH;
#endif
				config.opt = 0;
				config.dec_x = 0;
				config.dec_y = 0;
				config.dec_w = info.width;
				config.dec_h = info.height;
				config.angle = CLKWISE_0;

				printf("sending jpeg decoding config (%d-%d-%d-%d), planes(0x%x, 0x%x, 0x%x).\n",
					config.dec_x, config.dec_y, config.dec_w, config.dec_h,
					config.addr_y, config.addr_u, config.addr_v);

				err = ioctl(fd_jpegdec, JPEGDEC_IOC_DECCONFIG, &config);
				
				if (err < 0) {
					printf("decoder config failed\n");
					decLoop = 0;
					break;
				}
				
				s = DEC_STAT_RUN;
			}
			break;

		default:
			break;
		}
	}

	if (decState & JPEGDEC_STAT_DONE) {
		int fd_ge2d;
		
		fd_ge2d = open("/dev/ge2d", O_RDWR);
		if (fd_ge2d < 0)
			printf("Can not open frame buffer device\n");
		else {
			/* blit picture to frame buffer */
			ge2d_config.src_dst_type = ALLOC_OSD0;
			ge2d_config.alu_const_color=0xff0000ff;
			ge2d_config.src_format = GE2D_FORMAT_M24_YUV420;
//			ge2d_config.dst_format = GE2D_FORMAT_S32_ARGB;

			ge2d_config.src_planes[0].addr = config.addr_y;
			ge2d_config.src_planes[0].w = config.canvas_width;
			ge2d_config.src_planes[0].h = config.dec_h;

			ge2d_config.src_planes[1].addr = config.addr_u;
			ge2d_config.src_planes[1].w = config.canvas_width/2;
			ge2d_config.src_planes[1].h = config.dec_h / 2;

			ge2d_config.src_planes[2].addr = config.addr_v;
			ge2d_config.src_planes[2].w = config.canvas_width/2;
			ge2d_config.src_planes[2].h = config.dec_h / 2;

//			ge2d_config.dst_planes[0].addr=CMEM_getPhys(planes[3]);
//			ge2d_config.dst_planes[0].w=config.canvas_width;
//			ge2d_config.dst_planes[0].h = config.dec_h;
			
			
			
	    	ioctl(fd_ge2d,  GE2D_CONFIG, &ge2d_config); 

    		op_para.src1_rect.x = config.dec_x;
    		op_para.src1_rect.y = config.dec_y;
    		op_para.src1_rect.w = config.dec_w;
    		op_para.src1_rect.h = config.dec_h;

			op_para.dst_rect.x = 100;
			op_para.dst_rect.y = 100;
			op_para.dst_rect.w = config.dec_w;
			op_para.dst_rect.h = config.dec_h;

    		ioctl(fd_ge2d, GE2D_STRETCHBLIT_NOALPHA, &op_para);
	/*	ge2d_config.src_dst_type = ALLOC_OSD0;
		ge2d_config.alu_const_color=0xff0000ff;
		ge2d_config.src_format = GE2D_FORMAT_S24_RGB;

		ge2d_config.src_planes[0].addr = CMEM_getPhys(planes[3]);
		ge2d_config.src_planes[0].w =config.canvas_width;
		ge2d_config.src_planes[0].h = config.dec_h;
		
		ioctl(fd_ge2d,  GE2D_CONFIG, &ge2d_config);
		
		op_para.src1_rect.x = 0;
    		op_para.src1_rect.y = 0;
    		op_para.src1_rect.w =  config.dec_w;
    		op_para.src1_rect.h = config.dec_h;

			op_para.dst_rect.x = 100;
			op_para.dst_rect.y = 100;
//			op_para.dst_rect.w = config.dec_w;
//			op_para.dst_rect.h = config.dec_h;

    		ioctl(fd_ge2d,  GE2D_BLIT_NOALPHA, &op_para);
*/		
		printf("move picture completed\r\n")		;
			close(fd_ge2d);
    	}
	}

#ifdef CMEM_ALLOC
	if (planes[0])
		CMEM_free(planes[0], &cmemParm);
	if (planes[1])
		CMEM_free(planes[1], &cmemParm);
	if (planes[2])
		CMEM_free(planes[2], &cmemParm);
#endif

	
err:
	close(fd_file);
}
int   init_jpecdev(void)
{
	int retry_count=0;	
	fd_amport = open("/dev/amstream_vbuf", O_RDWR | O_NONBLOCK);
	if (fd_amport < 0) {
		printf("Can not open target file.\n");
		return -3;
	}
		
	ioctl(fd_amport, AMSTREAM_IOC_VB_SIZE, 1024*1024);
	ioctl(fd_amport, AMSTREAM_IOC_VFORMAT, VFORMAT_JPEG);

	/* use AMSTREAM_IOC_PORT_INIT to pop amjpegdec device w/o feeding data */
	ioctl(fd_amport, AMSTREAM_IOC_PORT_INIT);

retry:
	usleep(10000);
	fd_jpegdec = open("/dev/amjpegdec", O_RDWR);
	printf("fp:%d,err:%s\n",strerror(errno));
	if (fd_jpegdec >= 0) {
		printf("/dev/amjpegdec opened\n");
		return 0;
	}
	else 
	{
		if(retry_count++>100) 
		{	
			close(fd_amport);
			return -1;
		}	
		goto retry;
	}	
}
void deinit_jpecdev(void)
{
	close(fd_jpegdec);
	close(fd_amport);
}
int main(int argc, char *argv[])
{
	if (CMEM_init() < 0) 
	{
		printf("Can not allocate physical memory.\n");
		return -1;
	}

	int i = 0;
	char szBuff[512];
	while(1)
	{
		for(i=0; i<10; i++)
		{
			sprintf(szBuff, "resource/%d.jpg", i);
			printf("++++++++++++++++++%s\n",szBuff);
			if(0>init_jpecdev()) continue;
			JpegDecode(szBuff);
			deinit_jpecdev();
		}
	}
	deinit_jpecdev();
}
