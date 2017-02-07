#ifndef  _GPIO_H_
#define _GPIO_H_
#include <linux/ioctl.h>

#define   GPIO_CTRL    "/dev/gpio_dev"
#define _IOW_BAD(type,nr,size)			_IOC(_IOC_WRITE,(type),(nr),sizeof(size))
#define   GPIO_CMD_OP	    			_IOW_BAD('G',1,sizeof(short))

typedef 	struct{
	char	 cmd;
	char  bank;
	unsigned 	 bit;
	unsigned	 val;
	unsigned	 enable;
	unsigned	 group_idx;
}cmd_t;

typedef  struct{
	char dev[20];
	char addr[70];
	char size[70];
	char offset[70];
}gpio_uio_t;

typedef enum gpio_bank
{
	PREG_EGPIO=0,
	PREG_FGPIO,
	PREG_GGPIO,
	PREG_HGPIO
}gpio_bank_t;
typedef enum gpio_mode
{
	GPIO_OUTPUT_MODE,
	GPIO_INPUT_MODE,
}gpio_mode_t;

struct gpio_addr
{
	unsigned long mode_addr;
	unsigned long out_addr;
	unsigned long in_addr;
};

// Pre-defined GPIO addresses
// ----------------------------
#define PREG_EGPIO_EN_N                          	0
#define PREG_EGPIO_O                                	1
#define PREG_EGPIO_I                               	2
// ----------------------------
#define PREG_FGPIO_EN_N                            	3
#define PREG_FGPIO_O                            		4
#define PREG_FGPIO_I                        	       5
// ----------------------------
#define PREG_GGPIO_EN_N                            	6
#define PREG_GGPIO_O                               	7
#define PREG_GGPIO_I                               	8
// ----------------------------
#define PREG_HGPIO_EN_N                            	9
#define PREG_HGPIO_O                               	10
#define PREG_HGPIO_I                               	11

#define GPIOA_bank_bit(bit)		(PREG_EGPIO)

#define GPIOA_BANK				(PREG_EGPIO)
#define GPIOA_bit(bit)				(bit)
#define GPIOA_MAX_BIT			(17)

#define GPIOB_BANK				(PREG_FGPIO)
#define GPIOB_bit(bit)				(bit)	
#define GPIOB_MAX_BIT			(7)

#define GPIOC_BANK				(PREG_FGPIO)
#define GPIOC_bit(bit)				(bit+8)	
#define GPIOC_MAX_BIT			(23)

#define GPIOD_BANK				(PREG_GGPIO)
#define GPIOD_bit(bit)				(bit+16)	
#define GPIOD_MAX_BIT			(11)

#define GPIOE_BANK				(PREG_GGPIO)
#define GPIOE_bit(bit)				(bit)	
#define GPIOE_MAX_BIT			(15)

#define GPIOF_BANK				(PREG_EGPIO)
#define GPIOF_bit(bit)				(bit+18)	
#define GPIOF_MAX_BIT			(9)
	

static struct gpio_addr gpio_addrs[]=
{
	[PREG_EGPIO]={PREG_EGPIO_EN_N,PREG_EGPIO_O,PREG_EGPIO_I},//0x200c
	[PREG_FGPIO]={PREG_FGPIO_EN_N,PREG_FGPIO_O,PREG_FGPIO_I},
	[PREG_GGPIO]={PREG_GGPIO_EN_N,PREG_GGPIO_O,PREG_GGPIO_I},
	[PREG_HGPIO]={PREG_HGPIO_EN_N,PREG_HGPIO_O,PREG_HGPIO_I},
};
typedef  struct {
	volatile unsigned int *base_reg;
	int offset;
	int size ;
}gpio_t ;


static  gpio_t  gpio;

static inline int set_gpio_mode(gpio_bank_t bank,int bit,gpio_mode_t mode)
{
	volatile unsigned int *mod_reg=&gpio.base_reg[gpio_addrs[bank].mode_addr];
	*mod_reg=(*mod_reg &~(1<<bit))|((mode&1) << bit) ;
	return 0;
}
static inline gpio_mode_t get_gpio_mode(gpio_bank_t bank,int bit)
{
	volatile unsigned int *mod_reg=&gpio.base_reg[gpio_addrs[bank].mode_addr];
	return ((*mod_reg &(1<<bit))>0)?(GPIO_INPUT_MODE):(GPIO_OUTPUT_MODE);
}


static inline  int set_gpio_val(gpio_bank_t bank,int bit,unsigned long val)
{
	volatile unsigned int *out_reg=&gpio.base_reg[gpio_addrs[bank].out_addr];
	*out_reg=(*out_reg &~(1<<bit))|((val&1) << bit) ;

	return 0;
}

static inline unsigned long  get_gpio_val(gpio_bank_t bank,int bit)
{
	volatile unsigned int *in_reg=&gpio.base_reg[gpio_addrs[bank].in_addr];
	return ((*in_reg &(1<<bit))>0)?1:0;
}


#endif
