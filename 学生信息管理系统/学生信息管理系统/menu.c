#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "menu.h"


enum opt menu() {
    enum opt num;
    while (1) {

        printf("学生信息管理系统\n");
        printf("%d.增加学生信息\n", ADD);
        printf("%d.删除学生信息\n", DELETE);
        printf("%d.修改学生信息\n", MOD);
        printf("%d.学生信息排序\n", SORT);
        printf("%d.学生信息查询\n", REFER);
        printf("%d.退出系统\n", EXIT);
        printf("请输入相应标号\n");
        scanf("%d", (int*)&num);
        while (getchar() != '\n');
        if (num > EXIT || num < ADD) {
            printf("范围错误，请重新输入\n");
        }
        else
            return num;

    }

    //return num; 
}