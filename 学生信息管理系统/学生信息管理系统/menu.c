#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "menu.h"


enum opt menu() {
    enum opt num;
    while (1) {

        printf("ѧ����Ϣ����ϵͳ\n");
        printf("%d.����ѧ����Ϣ\n", ADD);
        printf("%d.ɾ��ѧ����Ϣ\n", DELETE);
        printf("%d.�޸�ѧ����Ϣ\n", MOD);
        printf("%d.ѧ����Ϣ����\n", SORT);
        printf("%d.ѧ����Ϣ��ѯ\n", REFER);
        printf("%d.�˳�ϵͳ\n", EXIT);
        printf("��������Ӧ���\n");
        scanf("%d", (int*)&num);
        while (getchar() != '\n');
        if (num > EXIT || num < ADD) {
            printf("��Χ��������������\n");
        }
        else
            return num;

    }

    //return num; 
}