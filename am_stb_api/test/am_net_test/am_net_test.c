/***************************************************************************
 *  Copyright C 2009 by Amlogic, Inc. All Rights Reserved.
 */
/**\file
 * \brief 网络模块测试程序
 ***************************************************************************/

#define AM_DEBUG_LEVEL 5

#include <am_net.h>
#include <am_debug.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdlib.h>

void fatal(char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintf(fmt, args);
	va_end(args);
	exit(1);
}

void usage(void) 
{
	fatal("Usage: am_net_test [options]\n"
		"options:\n"
		"\t -d, --dynamic		set dhcp mode\n"
		"\t -s, --status		get the network status\n"
		"\t -i, --ip			set ip address\n"
		"\t -m, --netmask		set netmask\n"
		"\t -g, --gw			set gateway\n"
		"\t -n, --dns1			set dns1\n"
		"\t --dns2			set dns2\n"
		"\t --dns3			set dns3\n"
		"\t --dns4			set dns4\n"
		"\t -e, --device		specific device\n"
		"\t -c, --mac			set MAC address\n"
		"\t -h, --help			print this help\n");
}

static int dev_no, dynamic_mode, show_status;
static int long_index;
static char* const short_options = "e:dsi:m:g:n:c:h";  
static struct option long_options[] = {  
     { "dynamic",   no_argument,          NULL,   'd'     },  
     { "status",      no_argument,           NULL,    's'     },  
     { "ip",            required_argument,   NULL,    'i'     },  
     { "netmask",  required_argument,   NULL,    'm'     },  
     { "gw",          required_argument,   NULL,    'g'     },  
     { "dns1",     required_argument,   NULL,    'n'     },  
     { "dns2",     required_argument,   NULL,    1     },  
     { "dns3",     required_argument,   NULL,    2     },
     { "dns4",     required_argument,   NULL,    3     },
     { "dev",     required_argument,   NULL,    'e'     },  	
     { "mac",     required_argument,   NULL,   'c'     },  	
     { "help",     no_argument,             NULL,   'h'     },  	
     {      0,     0,     0,     0},  
};

static int s_mask;
static AM_NET_IPAddr_t ip;
static AM_NET_IPAddr_t mask;
static AM_NET_IPAddr_t gw;
static AM_NET_IPAddr_t dns1;
static AM_NET_IPAddr_t dns2;
static AM_NET_IPAddr_t dns3;
static AM_NET_IPAddr_t dns4;
static int s_mac;
static AM_NET_HWAddr_t hw;

static void dumpPara(AM_NET_Para_t *para)
{
	if(para) {
		printf("DHCP: %s\n", para->dynamic?"on":"off");
		printf("IP: %s\n", inet_ntoa(para->ip));
		printf("MASK: %s\n", inet_ntoa(para->mask));
		printf("GATEWAY: %s\n", inet_ntoa(para->gw));
		printf("DNS1: %s\n", inet_ntoa(para->dns1));
		printf("DNS2: %s\n", inet_ntoa(para->dns2));
		printf("DNS3: %s\n", inet_ntoa(para->dns3));
		printf("DNS4: %s\n", inet_ntoa(para->dns4));
	}
}
static void dumpMAC(AM_NET_HWAddr_t *hw)
{
	if(hw) 
		printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", 
				hw->mac[0], hw->mac[1], hw->mac[2]
				, hw->mac[3], hw->mac[4], hw->mac[5]);
}

int main(int argc,char **argv)
{
	AM_ErrorCode_t err;
	AM_NET_Para_t para;
	AM_NET_Status_t status;
	AM_NET_HWAddr_t mac;

	int ch;
	
	if(argc == 1)
		usage();
	
	opterr = 0;
	while((ch = getopt_long(argc,argv,short_options, long_options, &long_index))!= -1)
	switch(ch)
	{
		case 's':
			show_status = 1;
			break;
		case 'd':
			dynamic_mode = 1;
			break;
		case 'e':
			if(sscanf(optarg, "%i", &dev_no)!=1)
				fatal("wrong devno.\n");
			break;
		case 'i':
			s_mask |= AM_NET_SP_IP;
			if(!inet_aton(optarg, &ip))
				fatal("wrong ip address.\n");
			break;
		case 'm':
			s_mask |= AM_NET_SP_MASK;
			if(!inet_aton(optarg, &mask))
				fatal("wrong netmask.\n");
			break;
		case 'g':
			s_mask |= AM_NET_SP_GW;
			if(!inet_aton(optarg, &gw))
				fatal("wrong gateway ip address.\n");
			break;
		case 'n':
			s_mask |= AM_NET_SP_DNS1;
			if(!inet_aton(optarg, &dns1))
				fatal("wrong dns1 address.\n");
			break;
		case 1:     /* long option without a short arg */
			s_mask |= AM_NET_SP_DNS2;
			if(!inet_aton(optarg, &dns2))
				fatal("wrong dns2 address.\n");
			break;
		case 2:
			s_mask |= AM_NET_SP_DNS3;
			if(!inet_aton(optarg, &dns3))
				fatal("wrong dns3 address.\n");
			break;
		case 3:
			s_mask |= AM_NET_SP_DNS4;
			if(!inet_aton(optarg, &dns4))
				fatal("wrong dns4 address.\n");
			break;
		case 'c':
			{
			unsigned int mac[6];
			int i;
			if(sscanf(optarg, "%x:%x:%x:%x:%x:%x", 
				&mac[0], &mac[1], &mac[2]
				, &mac[3], &mac[4], &mac[5])!=6)
				fatal("wrong mac address.\n");
			for(i=0;i<6;i++)
				hw.mac[i] = mac[i];
			s_mac = 1;
			}
			break;
		case 0:
			break;
			
		case 'h':
		default:
			usage();
	}

	s_mask |= AM_NET_SP_DHCP;
	

	#define FATAL(msg)	fatal(msg"dev[%d]. err[0x%x]\n", dev_no, err)
	
	if((err = AM_NET_Open(dev_no)) != AM_SUCCESS)
		FATAL("can not open net device.");

	if(show_status) {
		if((err = AM_NET_GetStatus(dev_no, &status)) != AM_SUCCESS)
			FATAL("get status fail.");
		printf("STATUS : %s\n", (status==AM_NET_STATUS_LINKED)?"LINKED":"UNLINKED");
	}

	if(s_mask || show_status) {
		if((err = AM_NET_GetPara(dev_no, &para)) != AM_SUCCESS)
			FATAL("get param fail.");
		
		if((err = AM_NET_GetHWAddr(dev_no, &mac)) != AM_SUCCESS)
			FATAL("get hw address fail.");
		
		if(show_status) {
			dumpPara(&para);
			dumpMAC(&mac);
		}
			
		if(s_mask) {
			if(s_mask&AM_NET_SP_DHCP)
				para.dynamic = dynamic_mode? AM_TRUE : AM_FALSE;
			if(s_mask&AM_NET_SP_IP)
				memcpy(&para.ip, &ip, sizeof(para.ip));
			if(s_mask&AM_NET_SP_MASK)
				memcpy(&para.mask, &mask, sizeof(para.mask));
			if(s_mask&AM_NET_SP_GW)
				memcpy(&para.gw, &gw, sizeof(para.gw));
			if(s_mask&AM_NET_SP_DNS1)
				memcpy(&para.dns1, &dns1, sizeof(para.dns1));
			if(s_mask&AM_NET_SP_DNS2)
				memcpy(&para.dns2, &dns2, sizeof(para.dns2));
			if(s_mask&AM_NET_SP_DNS3)
				memcpy(&para.dns3, &dns3, sizeof(para.dns3));
			if(s_mask&AM_NET_SP_DNS4)
				memcpy(&para.dns4, &dns4, sizeof(para.dns4));
			
			if((err = AM_NET_SetPara(dev_no, s_mask, &para)) != AM_SUCCESS)
				FATAL("set Para fail.");
		}
	}

	if(s_mac)
		if((err = AM_NET_SetHWAddr(dev_no, &hw)) != AM_SUCCESS)
			FATAL("set MAC fail.");

	AM_NET_Close(dev_no);

	return 0;
}

