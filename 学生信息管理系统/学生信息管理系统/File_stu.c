#define  _CRT_SECURE_NO_WARNINGS 1
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "stu_link.h"

//全局变量
//存放操作信息
char err_buf[128];
//存放操作的具体学生
stu stu_info;
//存放操作函数
char err_name[64];

/**
*功能：链表信息退出保存	stu_info.txt
* 参数：STU_LINK* phead 链表头
* 返回值：return EOF;		return 0;
**/

int save_stu_info(STU_LINK* phead) {
	//打开文件
	FILE* fp = fopen("stu_info.txt", "w");
	STU_LINK* ptmp = phead;
	if (fp == NULL) {
		perror("fopen");
		return EOF;
	}
	//写入数据
	//fprintf(fp, "%s\t%s\t%s\t%s\n", "学号", "姓名", "性别", "分数");
	while (ptmp->pnext) {
		fprintf(fp, "%d\t%s\t%s\t%f\n",
			ptmp->pnext->s.id,
			ptmp->pnext->s.name,
			ptmp->pnext->s.sex,
			ptmp->pnext->s.score
		);
		ptmp = ptmp->pnext;
	}
	//关闭文件
	fclose(fp);
	fp = NULL;
	return 0;

}


/**
*功能：读取	stu_info.txt中的学生信息，并保存在链表中
* 参数：STU_LINK* phead 链表头
* 返回值：return EOF;		return 0;
**/

int read_stu_info(STU_LINK** phead) {
	//打开文件
	int i = 0;
	//char n[64] = { 0 };
	FILE* fp = fopen("stu_info.txt", "r");
	STU_LINK* pnew = NULL;
	if (fp == NULL) {
		perror("fopen");
		return EOF;
	}
	//读入数据
	//fscanf(fp, "%s\t%s\t%s\t%s\n",n, n, n, n);
	//do {
	//	
	//	pnew = add_link(phead);
	//	i = fscanf(fp, "%d\t%s\t%s\t%f\n",
	//		&pnew->s.id,
	//		pnew->s.name,
	//		pnew->s.sex,
	//		&pnew->s.score
	//	);
	//} while (i != EOF);

	*phead = (STU_LINK*)malloc(sizeof(STU_LINK));
	memset(*phead, 0, sizeof(STU_LINK));
	pnew = *phead;
	while (fscanf(fp, "%d\t%s\t%s\t%f\n",
		&pnew->s.id,
		pnew->s.name,
		pnew->s.sex,
		&pnew->s.score
	) != EOF){

		
		
		pnew = add_link(*phead);
	} 
	
	//关闭文件
	fclose(fp);
	fp = NULL;
	return 0;

}


/**
*功能：历史记录
* 参数：STU_LINK* phead 链表头
* 返回值：return EOF;		return 0;
**/

int history_info(char* name, char* str) {
	time_t rawtime;
	//打开文件
	FILE* fp = fopen("history_info.txt", "a");
	if (fp == NULL) {
		perror("fopen");
		return EOF;
	}
	//读入数据
	time(&rawtime);
	fprintf(fp, "%s\t", ctime(&rawtime));
	fprintf(fp, "%s\t", name);
	fprintf(fp, "%s\n", str);

	//关闭文件
	fclose(fp);
	fp = NULL;
	return 0;

}