#include "osd.h"

void  display_run_menu(void)
{
	printf("\ntest menu : \n"
	"	1	osd display test, \n" 		\
	"	2	ge2d operation test \n"		\
	"	3 	osd color key test \n"		\
	"	4	osd global alpha test\n"		\
	"	5	osd order change \n"		\
	"	6	multi process test"
	"please input menu item number: ");
}
func_test_t  func_array[]={ NULL,test_osd_display, test_ge2d_op,test_osd_colorkey,test_osd_gbl_alpha,osd_order_change,multi_process_test} ;
int main(void)
{
	int  menu_item ;
		
	display_run_menu();
	printf("please select menu item\n");
	scanf("%d",&menu_item);
	//menu_item=2;
	if(menu_item >= ARRAY_SIZE(func_array) || func_array[menu_item]==NULL)
	{
		printf("invalid input menu item number \r\n");
		return -1;
	}
	return func_array[menu_item](NULL);
}

