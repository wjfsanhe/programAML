/***************************************************************************
 *  Copyright C 2009 by Amlogic, Inc. All Rights Reserved.
 */
/**\file
 * \brief 视频输出模块测试程序
 ***************************************************************************/

#define AM_DEBUG_LEVEL 5

#include <am_vout.h>
#include <am_debug.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
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
	fatal("Usage: am_vout_test [options]\n"
		"options:\n"
		"\t -e, --device		specific device\n"
		"\t -l, --list_fmts		list all supported formats\n"
		"\t -f, --fmt=FMT		set vout format\n"
		"\t -s, --status		get vout status\n"
		"\t -1, --on			enable the vout\n"
		"\t -0, --off			disable the vout\n"
		"\t -h, --help			print this help\n"
		);
}

static int long_index;
static char* const short_options = "lf:s01he:";  
static struct option long_options[] = {  
     { "dev",     required_argument,   NULL,    'e'     },  		
     { "list_fmts",   no_argument,          NULL,   'l'     },  
     { "status",      no_argument,           NULL,    's'     },  
     { "fmt",            required_argument,   NULL,    'f'     },  
     { "on",     no_argument,             NULL,   '1'     },  	
     { "off",     no_argument,             NULL,   '0'     },  	
     { "help",     no_argument,             NULL,   'h'     },  	
     {      0,     0,     0,     0},  
};

static struct {
	AM_VOUT_Format_t fmt;
	char *name;
#define FMTSET(fmt) {AM_VOUT_FORMAT_##fmt, #fmt}	
}VOUT_FMT[] = {
	FMTSET(UNKNOWN),
	FMTSET(576CVBS),
	FMTSET(480CVBS), 
	FMTSET(576I),
	FMTSET(576P), 
	FMTSET(480I),
	FMTSET(480P),
	FMTSET(720P), 
	FMTSET(1080I),
	FMTSET(1080P), 
	};
static int fmts_cnt = sizeof(VOUT_FMT) / sizeof(VOUT_FMT[0]);
static int dev_no;
static int s_fmt;
static AM_VOUT_Format_t fmt;
static int s_enable;
static int enable;
static int show_status;

static void showFormat(AM_VOUT_Format_t f)
{
	int i;
	
	for(i=0; i<fmts_cnt; i++)
		if(VOUT_FMT[i].fmt == f) {
			printf("FORMAT: %s\n", VOUT_FMT[i].name);
			return ;
		}
	printf("Unkonwn format.\n");
}

static AM_VOUT_Format_t getFormat(char * str)
{
	int i;
	for(i=0; i<fmts_cnt; i++)
		if(strcmp(VOUT_FMT[i].name, str)==0) {
			return VOUT_FMT[i].fmt;
		}
	printf("Unkonwn format.\n");
	return AM_VOUT_FORMAT_UNKNOWN;
}

static void dumpFormats(void)
{
	int i;
	printf("Formats supported:\n");
	for(i=0; i<fmts_cnt; i++)
	{
		printf("\t%s\n", VOUT_FMT[i].name);
	}
	printf("\n");
}

int main(int argc,char **argv)
{
	AM_ErrorCode_t err;
	AM_VOUT_OpenPara_t o;
	AM_VOUT_Format_t f;

	int ch;
	
	if(argc == 1)
		usage();
	
	opterr = 0;
	while((ch = getopt_long(argc,argv,short_options, long_options, &long_index))!= -1)
	switch(ch)
	{
		case 'e':
			if(sscanf(optarg, "%i", &dev_no)!=1)
				fatal("wrong devno.\n");
			break;
		case 'l':
			dumpFormats();
			break;

		case 'f':
			fmt = getFormat(optarg);
			s_fmt = 1;
			break;
		case 's':
			show_status = 1;
			break;
		case '1':
			s_enable = 1;
			enable = 1;
			break;
		case '0':
			s_enable = 1;
			enable = 0;
			break;

		case 0:
			break;
			
		default:
			usage();
	}

	#define FATAL(msg)	fatal(msg"dev[%d]. err[0x%x]\n", dev_no, err)
	
	if((err = AM_VOUT_Open(dev_no, &o)) != AM_SUCCESS)
		FATAL("can not open vout device.");

	if(show_status) {
		if((err = AM_VOUT_GetFormat(dev_no, &f)) != AM_SUCCESS)
			FATAL("get format fail.");
		showFormat(f);
	}

	if(s_enable){
		if(enable) {
			if((err = AM_VOUT_Enable(dev_no)) != AM_SUCCESS)
				FATAL("set enable fail.");
		} else {
			if((err = AM_VOUT_Disable(dev_no)) != AM_SUCCESS)
				FATAL("set disable fail.");
		}
	}
		
	if(s_fmt)
		if((err = AM_VOUT_SetFormat(dev_no, fmt)) != AM_SUCCESS)
			FATAL("set fmt fail.");

	AM_VOUT_Close(dev_no);

	return 0;
}

