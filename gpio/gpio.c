#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include "gpio.h"

static unsigned long  get_num_infile(char *file)
{
    int fd;
    char buf[24]="";
    unsigned long num=0;
    if ((fd = open(file, O_RDONLY)) < 0) {
       printf("unable to open file %s,err: %s",file, strerror(errno));
        return 0;
    }
    read(fd, buf, sizeof(buf));
    num = strtoul(buf, NULL, 0);
    close(fd);
   return num;
}

static void gpio_uio_test(void)
{
	int i;
	//GPIOA_2  sata LED 
	set_gpio_mode(GPIOA_BANK,GPIOA_bit(3),GPIO_OUTPUT_MODE);
	while(1)
	{
		set_gpio_val(GPIOA_BANK, GPIOA_bit(3),1);
		set_gpio_val(GPIOA_BANK, GPIOA_bit(3),0);
	}
}
int  setup_gpio_uio(gpio_uio_t  *gpio_uio)
{
	const char* GPIO_UIO_DEVICE_DIR="/sys/devices/virtual/gpio/gpio_dev";
	int found=0;
	struct dirent   *ent  ;
	DIR *pdir ;
	char *cp;
	int	idx;
	if(gpio_uio==NULL) return -1;

	if((pdir=opendir(GPIO_UIO_DEVICE_DIR))==NULL)
	{
		printf("cant open dir :%s,%s\n",GPIO_UIO_DEVICE_DIR,strerror(errno));
		return -1;
	}
	
	while((ent=readdir(pdir))!=NULL)
	{
		if(strncmp(ent->d_name,"uio",3)==0 )
		{
			found=1;
			break;
		}
	}
	if(!found) return -1;
	cp=&ent->d_name[3];
	idx=strtoul(cp,NULL,0);
	sprintf(gpio_uio->dev,"/dev/uio%d",idx);
	sprintf(gpio_uio->addr,"/sys/class/gpio/gpio_dev/uio%d/maps/map0/addr",idx);
	sprintf(gpio_uio->size,"/sys/class/gpio/gpio_dev/uio%d/maps/map0/size",idx);
	sprintf(gpio_uio->offset,"/sys/class/gpio/gpio_dev/uio%d/maps/map0/offset",idx);	
	return 0;
	
}
int  main(char argc,char **argv)
{
	int  page_size=getpagesize();
	int  phys_start,fd,ctrl_fd;
	int  phys_size;
	int  phys_offset ; //all addr mapped will be page allign.
	cmd_t  ctrl;
       gpio_uio_t  uio;
	   
	 
	 if ((ctrl_fd = open(GPIO_CTRL, O_RDWR)) < 0) 
	 {
        	printf("unable to open gpio ctrl device , %s", strerror(errno));
        	return -4;
    	}
	 ctrl.bank='a';
	 ctrl.bit=3;
	 ctrl.cmd='c';
	 ctrl.enable=1;
	 if(0>ioctl(ctrl_fd,GPIO_CMD_OP,&ctrl))
	 {
	 	printf("can't change gpio mode\n");
		return -3;
	 }
	if(0> setup_gpio_uio(&uio))
	return -1;
	 if ((fd = open(uio.dev, O_RDWR)) < 0) 
	 {
        	printf("unable to open UIO device, %s", strerror(errno));
        	return -4;
    	}
	phys_start=get_num_infile(uio.addr);
	phys_size=get_num_infile(uio.size);
	phys_offset=get_num_infile(uio.offset);
	printf("gpio[start-size-offset]: 0x%x - 0x%x - 0x%x\n ",phys_start,phys_size,phys_offset);

	phys_size=(phys_size+page_size - 1 )&~(page_size -1);
	gpio.base_reg=(unsigned int *) mmap(NULL, phys_size,
                       PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	gpio.base_reg+=(phys_offset>>2);
	gpio.size=phys_size;
	gpio.offset=phys_offset;
	
	gpio_uio_test();
	ctrl.enable=0;
	ioctl(ctrl_fd,GPIO_CMD_OP,&ctrl);
	return 0;
	
}

