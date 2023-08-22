#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "menu.h"
#include "stu_link.h"
#include <stdlib.h>     
#include <string.h>
#include "File_stu.h"

STU_LINK* Ph;

int main()
{
    
   // Ph = (STU_LINK*)malloc(sizeof(STU_LINK));
    int num = 0;
  //  memset(Ph, 0, sizeof(STU_LINK));
    read_stu_info(&Ph);
 
    do {
        num = menu();
        clearScreen();
        switch (num) {
        case ADD: {

            add_info(Ph);
            clearScreen();
            break;
        }
        case DELETE: {
            delete_c(Ph);
            clearScreen();
            break;
        }
        case MOD: {
            change_stu_info(Ph);
            clearScreen();
            break;
        }
        case SORT: {
            sort_c(Ph);
          //  clearScreen();
            break;
        }
        case REFER: {
            ckeck_info(Ph);
            clearScreen();
            break;
        }
        case EXIT: {
            save_stu_info(Ph);
            break;
        }
        }
       
    } while (num - EXIT);
    return 0;
}
