#pragma once
#ifndef _FILE_STU_H_
#define _FILE_STU_H_

extern char err_buf[128];
extern stu stu_info;
extern char err_name[64];
//º¯ÊıÉùÃ÷
int save_stu_info(STU_LINK* phead);
int read_stu_info(STU_LINK** phead);
int history_info(char* name, char* str);


#endif
