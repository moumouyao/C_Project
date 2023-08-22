#pragma once
#ifndef _STU_LINK_H_
#define _STU_LINK_H_

#define id_len 4	//id����
#define length 10000    //���洢��
#define f_score 1000    //����

//����ṹ������
typedef struct {
    int id;
    char name[20];
    char sex[4];
    float score;
}stu;
//��������Ľṹ��
typedef struct  stu_link
{
    stu s;              //������
    struct stu_link* pnext;    //��һ���ڵ��ַ
    struct stu_link* plast;    //��һ���ڵ��ַ
}STU_LINK;
//����ö������
enum check_opt {
    NAME_C = 1, ID_C, SCORE_C, SEX_C, EXIT_C
};
//��������
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