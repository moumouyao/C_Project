#pragma once
#ifndef _STU_LINK_H_
#define _STU_LINK_H_

#define id_len 4	//id长度
#define length 10000    //最大存储量
#define f_score 1000    //满分

//定义结构体类型
typedef struct {
    int id;
    char name[20];
    char sex[4];
    float score;
}stu;
//定义链表的结构体
typedef struct  stu_link
{
    stu s;              //数据域
    struct stu_link* pnext;    //下一个节点地址
    struct stu_link* plast;    //上一个节点地址
}STU_LINK;
//定义枚举类型
enum check_opt {
    NAME_C = 1, ID_C, SCORE_C, SEX_C, EXIT_C
};
//函数声明
STU_LINK* add_link(STU_LINK* phead);
void clear();
void add_info(STU_LINK* phead);
STU_LINK* check_id(STU_LINK* phead, int id);
void clearScreen();
char** name_c(STU_LINK* phead, char* pname);
void name_c_c();
void ckeck_info(STU_LINK* phead);
STU_LINK* id_c(STU_LINK* phead, int id);
void id_c_c();
void sort_score(STU_LINK* phead);
void score_c_c();
void sex_c(STU_LINK* phead);
void change_stu_info(STU_LINK* phead);
int delete(STU_LINK* phead, int id);
void delete_c(STU_LINK* phead);
void sort_c(STU_LINK* phead);
void sort_id(STU_LINK* phead);
int isChineseString(char* str);
int check_stu_id(int id);
#endif