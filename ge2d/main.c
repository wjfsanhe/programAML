#include "Platform.h"
#include "DK_Graphics.h"
#include "Osd.h"

int main (int argc ,char **argv)
{
	#if 0
    rectangle_t this_rect, this_rect1, this_rect5;
    int i = 0;
    
    int nn;
    int ss = 0;
    THRGB color;
    HDC A1 = NULL;
    HDC A2 = NULL;
    HDC A3 = NULL;
    HDC A4 = NULL;
    HDC A5 = NULL;

    DK_SysInit();
    HDC ghdc0 = DK_GetDC(0);
    HDC ghdc1 = DK_GetDC(1);
	
	  this_rect.x = 0;
    this_rect.y = 0;
    this_rect.w = ghdc0->nWidth;
    this_rect.h = ghdc0->nHeight;
    
    printf("back buff  fill of 0xFF0000FF\n");   
    DK_FillRect(ghdc0, this_rect, 0xFF0000FF);
    sleep(1);
    
    printf("DK_UpdateRegion\n");       
    DK_UpdateRegion(ghdc0, NULL);

    sleep(1);
    
    printf("back buff  fill of 0xFFFF\n");   

    DK_FillRect(ghdc0, this_rect, 0xFFFF);
    sleep(1);
    
    printf("DK_UpdateRegion\n");       
    DK_UpdateRegion(ghdc0, NULL);
    sleep(1);
    
    double begin = mysecond();
    for(i = 0; i < 10; i++)
    {
       printf("1111111111111111\n");
       DK_UpdateRegion(ghdc0, NULL);
       printf("222222222222222\n");
       sleep(1);
    }
    A1 = DK_CreateDCFromBitmap("/mnt/disk/1.bmp");
//    DK_BitBlt(Osd[0], 0, 0, Osd[0]->nWidth, Osd[0]->nHeight, A1, 0, 0, 0);
    DK_UpdateRegion(ghdc0, NULL);
    
    DK_BitBlt(ghdc1, 0, 0, A1->nWidth, A1->nHeight, A1, 0, 0, 0);
    DK_UpdateRegion(ghdc1, NULL);

    DK_BitBlt(ghdc1, 0, 0, A1->nWidth, A1->nHeight, A1, 0, 0, 0);
    DK_UpdateRegion(ghdc1, NULL);

    A2 = DK_CreateDCFromBitmap("/mnt/disk/2.bmp");

    DK_FillRect(ghdc0, this_rect, 0x00000000);
    DK_UpdateRegion(ghdc0, NULL);
    nn = 1000;
#else
    DK_SysInit();
	HDC  gdc = NULL;
	HDC  dc = NULL;
	HDC  dc_bk = NULL;
	HDC  dc9 = DK_CreateDCFromBitmap("/mnt/mmc/button_setting.bmp");
	HDC  dc1 = DK_CreateDCFromBitmap("/mnt/mmc/main.bmp");
	HDC  dc2 = DK_CreateDCFromBitmap("/mnt/mmc/icon_wifi.bmp");
	HDC  dc3 = DK_CreateDCFromBitmap("/mnt/mmc/icon_wifi1.bmp");
	//HDC  dc3 = DK_CreateDCFromBitmap("/mnt/mmc/icon_wifi.bmp");
	HDC  dc4 = DK_CreateDCFromBitmap("/mnt/mmc/icon_wifi1.bmp");
	HDC  dc5 = DK_CreateDCFromBitmap("/mnt/mmc/icon_wifi.bmp");
	HDC  dc6 = DK_CreateDCFromBitmap("/mnt/mmc/button_setting.bmp");
	HDC  dc7 = DK_CreateDCFromBitmap("/mnt/mmc/button_movie.bmp");
	HDC  dc8 = DK_CreateDCFromBitmap("/mnt/mmc/button_book.bmp");
HDC  dc10 = DK_CreateDCFromBitmap("/mnt/mmc/button_movie.bmp");
	dc = DK_GetDC(0);
	dc_bk =DK_GetDC(1);
	
	rectangle_t rect;
	rect.x = 0;
       rect.y = 0;
       rect.w = DK_GetOsdWidth(dc_bk);
       rect.h = DK_GetOsdHeigh(dc_bk);
	DK_FillRect(dc_bk, rect, 0xFF0000FF);   
	DK_UpdateRegion(dc_bk, &rect);
       rect.x = 0;
       rect.y = 0;
       rect.w = DK_GetOsdWidth(dc);
       rect.h = DK_GetOsdHeigh(dc);
       DK_FillRect(dc, rect, 0xFFFF);
	//DK_BitBlt(dc, 400, 400, DK_GetOsdWidth(dc9), DK_GetOsdHeigh(dc9), dc9, 0, 0, 1);
	DK_UpdateRegion(dc, &rect);
	
	  

	DK_BitBlt(dc, 0, 0, DK_GetOsdWidth(dc1), DK_GetOsdHeigh(dc1), dc1, 0, 0, 1);
       DK_BitBlt(dc, 800, 400, DK_GetOsdWidth(dc10), DK_GetOsdHeigh(dc10), dc10, 0, 0, 1);
       DK_BitBlt(dc, 400, 400, DK_GetOsdWidth(dc9), DK_GetOsdHeigh(dc9), dc9, 0, 0, 1);
	DK_BitBlt(dc, 100, 100, DK_GetOsdWidth(dc8), DK_GetOsdHeigh(dc8), dc8, 0, 0, 1);
	DK_BitBlt(dc, 100, 200, DK_GetOsdWidth(dc7), DK_GetOsdHeigh(dc7), dc7, 0, 0, 1);
	DK_BitBlt(dc, 100, 300, DK_GetOsdWidth(dc6), DK_GetOsdHeigh(dc6), dc6, 0, 0, 1);
	DK_BitBlt(dc, 100, 400, DK_GetOsdWidth(dc5), DK_GetOsdHeigh(dc5), dc5, 0, 0, 1);
	DK_BitBlt(dc, 100, 500, DK_GetOsdWidth(dc4), DK_GetOsdHeigh(dc4), dc4, 0, 0, 1);
	DK_BitBlt(dc, 300, 100, DK_GetOsdWidth(dc3), DK_GetOsdHeigh(dc3), dc3, 0, 0, 1);
	DK_BitBlt(dc, 300, 200, DK_GetOsdWidth(dc2), DK_GetOsdHeigh(dc2), dc2, 0, 0, 1);
	DK_BitBlt(dc, 400, 100, DK_GetOsdWidth(dc8), DK_GetOsdHeigh(dc8), dc8, 0, 0, 1);
	DK_BitBlt(dc, 600, 400, DK_GetOsdWidth(dc9), DK_GetOsdHeigh(dc9), dc9, 0, 0, 1);
	//DK_BitBlt(dc, 300, 300, DK_GetOsdWidth(dc3), DK_GetOsdHeigh(dc3), dc3, 0, 0, 1);
	
	DK_UpdateRegion(dc, &rect);
         printf("aaaaaaaaa\n");
       sleep(2);
      printf("bbbbbbbbbb\n");
       DK_FillRect(dc, rect, 0xFFFF);
	DK_UpdateRegion(dc, &rect);
       printf("aaaaaaaaa\n");
       sleep(3);
       printf("bbbbbbbbbb\n");
       DK_BitBltEx(dc, 100, 100, DK_GetOsdWidth(dc8), DK_GetOsdHeigh(dc8), 
                           dc8, 0, 0, DK_GetOsdWidth(dc8), DK_GetOsdHeigh(dc8),
                           dc9, 0, 0, DK_GetOsdWidth(dc8), DK_GetOsdHeigh(dc8),
                           0, 0, 0, 0, 0);
	DK_UpdateRegion(dc, &rect);
       printf("aaaaaaaaa\n");
       sleep(3);
       printf("bbbbbbbbbb\n");
     
       DK_BitBltBlend(dc1, 100, 100, DK_GetOsdWidth(dc2), DK_GetOsdHeigh(dc2), dc2, 0, 0, 0);
	DK_BitBlt(dc, 0, 0, DK_GetOsdWidth(dc1), DK_GetOsdHeigh(dc1), dc1, 0, 0, 1);
	DK_BitBlt(dc, 400, 400, DK_GetOsdWidth(dc2), DK_GetOsdHeigh(dc2), dc2, 0, 0, 1);
	DK_UpdateRegion(dc, &rect);
	sleep(3);
	printf("update next window\n");;
	DK_UpdateRegion(dc, NULL);
/*      printf("cccccccccccccc\n");
       sleep(3);
      printf("dddddddddddd\n");


       DK_FillRect(dc, rect, 0xFFFF);
	DK_UpdateRegion(dc, NULL);
       printf("aaaaaaaaa\n");
       sleep(3);
       printf("bbbbbbbbbb\n");

       DK_BitBltBlend(dc1, 0, 0, DK_GetOsdWidth(dc1), DK_GetOsdHeigh(dc1), dc1, 0, 0, 0);
*/       
#endif
dir_exit :    

	DK_DeleteDC(dc1);
  	DK_DeleteDC(dc2);
       DK_DeleteDC(dc3);
    	DK_DeleteDC(dc4);	  
      DK_DeleteDC(dc5);
      DK_DeleteDC(dc6);
        DK_DeleteDC(dc7);
      DK_DeleteDC(dc8);
      DK_DeleteDC(dc9);
      DK_DeleteDC(dc10);

	DK_SysDestory();
	return 0;
}
