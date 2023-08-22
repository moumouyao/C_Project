#define  _CRT_SECURE_NO_WARNINGS 1
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "stu_link.h"

//ȫ�ֱ���
//��Ų�����Ϣ
char err_buf[128];
//��Ų����ľ���ѧ��
stu stu_info;
//��Ų�������
char err_name[64];

/**
*���ܣ�������Ϣ�˳�����	stu_info.txt
* ������STU_LINK* phead ����ͷ
* ����ֵ��return EOF;		return 0;
**/

int save_stu_info(STU_LINK* phead) {
	//���ļ�
	FILE* fp = fopen("stu_info.txt", "w");
	STU_LINK* ptmp = phead;
	if (fp == NULL) {
		perror("fopen");
		return EOF;
	}
	//д������
	//fprintf(fp, "%s\t%s\t%s\t%s\n", "ѧ��", "����", "�Ա�", "����");
	while (ptmp->pnext) {
		fprintf(fp, "%d\t%s\t%s\t%f\n",
			ptmp->pnext->s.id,
			ptmp->pnext->s.name,
			ptmp->pnext->s.sex,
			ptmp->pnext->s.score
		);
		ptmp = ptmp->pnext;
	}
	//�ر��ļ�
	fclose(fp);
	fp = NULL;
	return 0;

}


/**
*���ܣ���ȡ	stu_info.txt�е�ѧ����Ϣ����������������
* ������STU_LINK* phead ����ͷ
* ����ֵ��return EOF;		return 0;
**/

int read_stu_info(STU_LINK** phead) {
	//���ļ�
	int i = 0;
	//char n[64] = { 0 };
	FILE* fp = fopen("stu_info.txt", "r");
	STU_LINK* pnew = NULL;
	if (fp == NULL) {
		perror("fopen");
		return EOF;
	}
	//��������
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
	
	//�ر��ļ�
	fclose(fp);
	fp = NULL;
	return 0;

}


/**
*���ܣ���ʷ��¼
* ������STU_LINK* phead ����ͷ
* ����ֵ��return EOF;		return 0;
**/

int history_info(char* name, char* str) {
	time_t rawtime;
	//���ļ�
	FILE* fp = fopen("history_info.txt", "a");
	if (fp == NULL) {
		perror("fopen");
		return EOF;
	}
	//��������
	time(&rawtime);
	fprintf(fp, "%s\t", ctime(&rawtime));
	fprintf(fp, "%s\t", name);
	fprintf(fp, "%s\n", str);

	//�ر��ļ�
	fclose(fp);
	fp = NULL;
	return 0;

}