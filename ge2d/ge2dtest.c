#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "Osd.h"
#include "cmem.h"

#define IW  720
#define IH  576
#define OW  1280
#define OH  720

int main(int argc, char *argv[]){
    int ifd = 0;
    int ofd = 0,
        rc  = -1;
    int ifd_size = 0;
    int ofd_size = 0;
    void *iptr = NULL,
         *optr = NULL;

    if(argc < 3){
        printf("Wrong usage\n"
               "%s ifile ofile\n", argv[0]);
        exit(-1);
    }

    ifd = open(argv[1], O_RDWR);
    if(ifd < 0){
        printf("Error opening the input file, errno = %d,%s\n", errno,strerror(errno));
        return -1;
    }
    ofd = open(argv[2], O_RDWR | O_CREAT, 0755);
    if(ofd < 0){
        printf("Error opening the output file, errno = %d\n", errno,strerror(errno));
        close(ifd);
        return -1;
    }
    ofd_size = (OW * (OH)* 4);
    if(ftruncate(ofd, ofd_size) < 0){
        printf("Error truncating output file, errno = %d\n", errno);
        return -1;
    }
    lseek(ifd, 0, SEEK_SET);
    ifd_size = (IW * IH * 4);
    printf("Input file size = %d\n", ifd_size);
    iptr = mmap(NULL, ifd_size, PROT_READ, MAP_FILE | MAP_PRIVATE, ifd, 0 );//(lseek(ifd, 0, SEEK_END) - ifd_size));
    if(iptr == MAP_FAILED){
        printf("Error mapping the input file, errno = %d\n", errno);
        close(ifd);
        close(ofd);
        unlink(argv[2]);
        return -1;
    }
    optr = mmap(NULL, ofd_size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, ofd, 0);
    if(optr == MAP_FAILED){
        printf("Error mapping the output file, errno = %d\n", errno);
        munmap(iptr, ifd_size);
        close(ifd);
        close(ofd);
        unlink(argv[2]);
        return -1;
    }
    rc = -1;
    do{
        ge2d_op_para_t para;
        config_para_ex_t config;
        CMEM_AllocParams cmemParm = {CMEM_HEAP, CMEM_NONCACHED, 8};
        int ge2d_fd;
        CMEM_BlockAttrs attrs;
        void *ciptr = NULL, *coptr = NULL;

        if(CMEM_init() < 0){
            printf(" -- CMEM_init failed");
            break;
        }
        if(CMEM_getBlockAttrs(0, &attrs) < 0){
            printf("Error getting the block attrs\n");
            break;
        }
        printf("block id 0 has size %d\n", attrs.size);
        printf("src planes addrs [0] = %p, [1] = %p, [2] = %p\n",
               config.src_planes[0], config.src_planes[1], config.src_planes[2]);
        memset(&config, 0, sizeof(config));
        ciptr = CMEM_alloc(0,
                           IW * IH * 4, &cmemParm);
        coptr = CMEM_alloc(0,
                           OW * OH * 4, &cmemParm);
        if(!ciptr || !coptr){
            printf("Error allocating cmem\n");
            break;
        }
#ifdef FILL
        memset(ciptr, 0, ifd_size);
        memset(coptr, 0, ofd_size);
#else
        memcpy(ciptr, iptr, ifd_size);
#endif
        config.src_para.mem_type  = CANVAS_ALLOC;
        config.src_para.format    = GE2D_FORMAT_S32_ARGB;
        config.dst_para.format    = GE2D_FORMAT_S32_ARGB;
#ifdef FILL
        config.src_planes[0].addr = CMEM_getPhys(coptr);
        config.src_planes[0].w    = OW;
        config.src_planes[0].h    = OH;
#else
        config.src_planes[0].addr = CMEM_getPhys(ciptr);
        config.src_planes[0].w    = IW;
        config.src_planes[0].h    = IH;
#endif
        config.src_para.color     = 0;
        config.src2_para.mem_type = CANVAS_TYPE_INVALID;
        config.dst_para.mem_type  = CANVAS_ALLOC;
        config.dst_para.color     = 0;
        config.dst_planes[0].addr = CMEM_getPhys(coptr);
        config.dst_planes[0].w    = OW;
        config.dst_planes[0].h    = OH;
        config.src_para.top       = 0;
        config.src_para.left      = 0;
        config.src_para.width     = IW;
        config.src_para.height    = IH;
        config.dst_para.top       = 0;
        config.dst_para.left      = 0;
        config.dst_para.width     = OW;
        config.dst_para.height    = OH;
        ge2d_fd = open("/dev/ge2d",O_RDWR);
        if(ge2d_fd < 0){
            printf("Error opening the ge2d device, errno = %d\n", errno);
            break;
        }
        if(ioctl(ge2d_fd, GE2D_CONFIG_EX, &config) < 0){
            printf("GE2d_CONFIG_EX failed, errno = %d\n", errno);
            break;
        }
        para.src1_rect.x = 0;
        para.src1_rect.y = 0;
#ifdef FILL
        para.src1_rect.w = OW;
        para.src1_rect.h = OH;
#else
        para.src1_rect.w = IW;
        para.src1_rect.h = IH;
#endif
        para.color       = 0xFF778899;
        para.dst_rect.x  = 0;
        para.dst_rect.y  = 0;
        para.dst_rect.w  = OW;
        para.dst_rect.h  = OH;
#ifdef FILL
        printf("Filling rectangle of size (%d x %d)\n", 
               OW, OH);
        if(ioctl(ge2d_fd, GE2D_FILLRECTANGLE, &para) < 0){
            printf("GE2D_FILLRECTANGLE failed, errno = %d\n", errno);
            break;
        }
#else
        printf("Stretching from (%d x %d) to (%d x %d)\n", 
               IW, IH, OW, OH);
        if(ioctl(ge2d_fd, GE2D_STRETCHBLIT, &para) < 0){
            printf("GE2D_STRETCHBLIT_NOALPHA failed, errno = %d\n", errno);
            break;
        }
#endif
//the second op
#ifdef FILL
	config.dst_para.top       = 0;
        config.dst_para.left      = 0;
        config.dst_para.width     = OW/2;
        config.dst_para.height    = OH/2;
	config.dst_para.mem_type=CANVAS_OSD0;
	 if(ioctl(ge2d_fd, GE2D_CONFIG_EX, &config) < 0){
            printf("GE2d_CONFIG_EX failed, errno = %d\n", errno);
            break;
        }
	    para.dst_rect.x  = 0;
        para.dst_rect.y  = 0;
        para.dst_rect.w  = OW/2;
        para.dst_rect.h  = OH/2;
	 printf("axis:%d-%d-%d-%d\n",para.dst_rect.x,para.dst_rect.y,para.dst_rect.w,para.dst_rect.h)	;
	  if(ioctl(ge2d_fd, GE2D_STRETCHBLIT, &para) < 0){
            printf("GE2D_FILLRECTANGLE failed, errno = %d\n", errno);
            break;
        }	
#else
	config.dst_para.top       = 0;
        config.dst_para.left      = 0;
        config.dst_para.width     = OW/2;
        config.dst_para.height    = OH/2;
	config.dst_para.mem_type=CANVAS_OSD0;
	 if(ioctl(ge2d_fd, GE2D_CONFIG_EX, &config) < 0){
            printf("GE2d_CONFIG_EX failed, errno = %d\n", errno);
            break;
        }
	    para.dst_rect.x  = 0;
        para.dst_rect.y  = 0;
        para.dst_rect.w  = OW/2;
        para.dst_rect.h  = OH/2;
	 printf("axis:%d-%d-%d-%d\n",para.dst_rect.x,para.dst_rect.y,para.dst_rect.w,para.dst_rect.h)	;
	  if(ioctl(ge2d_fd, GE2D_STRETCHBLIT, &para) < 0){
            printf("GE2D_FILLRECTANGLE failed, errno = %d\n", errno);
            break;
        }	
#endif
        printf("Data at 55636 : ip = %#x, op = %#x\n", 
               *((int *)ciptr + 55636), *((int *)coptr + 55636));
        close(ge2d_fd);
        
        memcpy(optr, coptr, ofd_size);
        CMEM_free(ciptr, &cmemParm);
        CMEM_free(coptr, &cmemParm);
        rc = 0;
    }while(0);
    munmap(iptr, ifd_size);
    munmap(optr, ofd_size);
    close(ofd);
    close(ifd);
    if(rc)unlink(argv[2]);
    return 0;
}
