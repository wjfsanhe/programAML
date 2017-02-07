#include <stdio.h>
#include <stdlib.h>
int main(char argc ,char** argv)
{
	int delay;
	if( argc < 2)
	{
		printf("please offer delay time\n");
	 	return ; 
	}
	delay=strtoul(argv[1],NULL,0);
	printf("delay time:%dms\n",delay/1000);
	system("echo  c:a:3:1 > /sys/class/gpio/cmd");
	system("echo  c:a:12:1 > /sys/class/gpio/cmd");
		
	//pulses pseduo
	system("echo w:a:12:1 > /sys/class/gpio/cmd");
	system("echo w:a:12:0 > /sys/class/gpio/cmd");
	system("echo w:a:12:1 > /sys/class/gpio/cmd");
        system("echo w:a:12:0 > /sys/class/gpio/cmd");
	system("echo w:a:12:1 > /sys/class/gpio/cmd");
	//falling edge
	system("echo w:a:12:0 > /sys/class/gpio/cmd");
	usleep(delay);
	//raising edge
        system("echo w:a:12:1 > /sys/class/gpio/cmd");
        system("echo w:a:12:0 > /sys/class/gpio/cmd");
        system("echo w:a:12:1 > /sys/class/gpio/cmd");
        system("echo w:a:12:0 > /sys/class/gpio/cmd");
        system("echo w:a:12:1 > /sys/class/gpio/cmd");
	
	
	return  0;
}
