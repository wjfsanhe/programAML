#define		MWFB_MSGBUF_SIZE	1400
#define		MWFB_MAX_REC_NUM	((MWFB_MSGBUF_SIZE - sizeof(T_MAIN_CTRL) - sizeof(int) * 2) / sizeof(int))

#define		MWFB_RESPONSE		0x80

#define		MWFB_SETCONFIG		1
#define		MWFB_RECTREFRESH	2
#define		MWFB_GUITRANSMODE	3
#define		MWFB_RECTRANSMODE	4

#define		FBMW_GETCONFIG		8001
#define		FBMW_REFRESHOVER	8002
#define		FBMW_GUITRANSOVER	8003
#define		FBMW_RECTRANSOVER	8004

#define		NANO_LISTEN_PORT	7130
#define		VOD_LISTEN_PORT		7121