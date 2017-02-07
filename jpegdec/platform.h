#ifndef PLATFORM_H
#define PLATFORM_H

#define LINUX

#ifdef LINUX
	#include <unistd.h>
	#include <stdio.h>
	#include <fcntl.h>
	#include <sys/mman.h>
	#include <linux/fb.h>
	#include <fcntl.h>
	#include <dlfcn.h>
	#include <time.h>
	#include <string.h>
	#include <stdlib.h>
	#include <sys/queue.h>
	#include <pthread.h>
	#include <sys/syscall.h>
	#include <errno.h>
	#include <sys/wait.h> 
       #include <sys/ioctl.h>
       #include <sys/time.h>
#else
#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h> 
#include "fb.h"


	typedef unsigned int size_t;
#endif
#endif

