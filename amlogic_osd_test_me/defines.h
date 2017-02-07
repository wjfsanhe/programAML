/* ============================================================================= *
 * Copyright (C) 2004 Analog Devices Inc. All rights reserved
 *
 * The information and source code contained herein is the exclusive property
 * of Analog Devices and may not be disclosed, examined or reproduced in whole
 * or in part without the explicit written authorization from Analog Devices.
 *
 * ===========================================================================*/
/****************************************************************************
*                           defines.h
*****************************************************************************/
#ifndef _DEFINES_H
#define _DEFINES_H_

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif
#ifndef CHAR
typedef char CHAR;
#endif
#ifndef UINT8
typedef unsigned char UINT8;
#endif
#ifndef INT8
typedef char INT8;
#endif
#ifndef UINT16
typedef unsigned short UINT16;
#endif
#ifndef INT16
typedef short INT16;
#endif
#ifndef UINT32
typedef unsigned int UINT32;
#endif
#ifndef INT32
typedef int INT32;
#endif
#ifndef INT64
typedef long long INT64;
#endif
#ifndef UINT64
typedef unsigned long long UINT64;
#endif
#ifndef BOOL
//typedef int BOOL;
#endif
#ifndef VOID
typedef void VOID;
#endif

#define WAIT    0x00000001
#define NOWAIT  0x00000002
#define ALL     0x00000004
#define ANY     0x00000008
#define NORMAL  0x00000000
#define URGENT	0x00000001

#ifndef NULL
#define	NULL	(void *)0
#endif
#ifndef TRUE
#define	TRUE	(BOOL)1
#endif
#ifndef FALSE
#define	FALSE	(BOOL)0
#endif

#define	SUCCESS	0
#define	FAILURE	-1

#define roundup(x,y) ((((x)+((y)-1))/(y))+(y))
#define bcopy(a, b, c)  memcpy(b, a, c)
#define bzero(a, b)     memset(a, 0, b)

static __inline int imax(int a,int b) { return (a > b ? a : b); }
static __inline int imin(int a,int b) { return (a < b ? a : b); }
static __inline unsigned int max(unsigned int a,unsigned int b) { \
                   return (a > b ? a : b); }
static __inline unsigned int min(unsigned int a,unsigned int b) { \
                   return (a < b ? a : b); }
static __inline long lmax(long a,long b) { return (a > b ? a : b); }
static __inline long lmin(long a,long b) { return (a < b ? a : b); }
static __inline unsigned long ulmax(unsigned long a,unsigned long b) { \
                   return (a > b ? a : b); }
static __inline unsigned long ulmin(unsigned long a,unsigned long b) { \
                   return (a < b ? a : b); }
#endif
