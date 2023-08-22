#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "menu.h"
#include "stu_link.h"
#include <stdlib.h>
#include <string.h>
#include "File_stu.h"

extern STU_LINK* Ph;
int count = 0;

//清理缓存区
void clear() {
    while (getchar() != '\n');
}
//清屏
void clearScreen() {
    printf("\033[2J\033[1;1H");
}

/*
*功能：创建链表节点
*参数：STU_LINK* phead 链表头
*返回值：成功 地址      失败 null
*/
STU_LINK* add_link(STU_LINK* phead) {
    STU_LINK* ptmp = NULL;
    ptmp = phead;
    while (ptmp->pnext) {        //寻找尾节点
        ptmp = ptmp->pnext;
    }
    if (ptmp->pnext = (STU_LINK*)malloc(sizeof(STU_LINK))) {    //开辟空间
        
        count++;
        printf("创建成功\n%p\n", ptmp);
    }
    else {
        printf("创建失败\n");
        return NULL;
    }
    memset(ptmp->pnext, 0, sizeof(STU_LINK));//初始化
    ptmp->pnext->plast = ptmp;
    return ptmp->pnext;
}

/*
*功能：检查学号是否重复
*参数：STU_LINK* phead, int id
*返回值：0 重复 1不重复
*/
STU_LINK* check_id(STU_LINK* phead, int id) {
    STU_LINK* ptmp = phead;
    while (ptmp->pnext) {
        if (ptmp->pnext->s.id == id) {
            return NULL;
        }
        ptmp = ptmp->pnext;
    }
    return ptmp;
}

/**
 * 功能：   添加学生信息
 * 参数：
 * 返回值：
 **/
void add_info(STU_LINK* phead) {
    int id = 0;
    int ret = 1;
    STU_LINK* pnew = NULL;
    STU_LINK* ptmp = phead;
    while (1) {   //读入学号并查重
        printf("请输入学号\n");
        scanf("%d", &id);
        clear();
         if (id <= 0 || check_stu_id(id) != id_len) {
            printf("请输入 1---%d 范围内的学号\n", length - 1);
            continue;
        }
        if (check_id(phead, id)) {
            printf("创建学号成功\n");
            break;
        }
        else {
            clearScreen();
            printf("学号为%d的学生已存在\n", id);

            sprintf(err_name, "添加信息");
            sprintf(err_buf, "id已经存在");
            history_info(err_name, err_buf);
       

        }
    }
    pnew = add_link(phead);
    if (pnew == NULL)
    {
        printf("空间开辟失败\n");
        sprintf(err_name, "添加信息");
        sprintf(err_buf, "空间开辟失败");
        history_info(err_name, err_buf);
    }
    pnew->s.id = id;    //读入id
    while (1) {
        printf("请输入姓名\n");
        scanf("%s", pnew->s.name);
        clear();
        if (isChineseString(pnew->s.name) == 0) {
            printf("输入的姓名非纯中文，请重新输入\n");
        }
        else if (isChineseString(pnew->s.name) == 1) {
            break;
        }
    }
    

    //读入性别
    while (1) {
        ret = 0;
        printf("请选择学生性别：\n");
        printf("1.男性\n");
        printf("2.女性\n");
     // printf("3.退出\n");
        scanf("%d", &ret);
        while ((getchar()) != '\n') {}
        if (ret != 1 && ret != 2) {
            printf("非法输入，请重新选择\n");
        }
        else
            break;
    }
    switch (ret) {
    case 1: {
        strncpy(pnew->s.sex, "男", 15);
        break;
    }
    case 2: {
        strcpy(pnew->s.sex, "女");
        break;
    }
    //case 3: {	//删除
    //    stu_info[count] = stu_info[count + 1];//置零
    //    return -3;
    //}
    }

   
    
    do {

        printf("请输入学生分数:\n");
        scanf("%f", &pnew->s.score);
        clear();
        if (pnew->s.score <= 0 || pnew->s.score > 1000) {
            printf("分数必须在0-%d之间!\n",f_score);
        }
        else {
            break;
        }
    } while (1);
    printf("添加成功\n");

    sprintf(err_name, "添加信息");
    sprintf(err_buf, "添加成功");
    history_info(err_name, err_buf);

}
/**
*功能：查看学生信息
* 参数：
* 返回值：
**/
void ckeck_info(STU_LINK* phead) {
    int num = 0;
    while (1) {
        printf("请选择查看方式\n");
        printf("%d.按姓名查询\n", NAME_C);
        printf("%d.按学号查询\n", ID_C);
        printf("%d.按分数段查询\n", SCORE_C);
        printf("%d.按性别查询\n", SEX_C);
        printf("%d.退出\n", EXIT_C);
        scanf("%d", &num);
        clear();
        if (num > EXIT || num < NAME_C) {
            clearScreen();
            printf("非法输入，请重新输入\n");
            continue;
        }
        else {
            clearScreen();
        }

        switch (num)
        {
        case NAME_C: {
          name_c_c();
            break;
        }
        case ID_C: {
            id_c_c();
            break;
        }
        case SCORE_C: {
            score_c_c();
            break;
        }
        case SEX_C: {

            break;
        }
        case EXIT_C: {
            return;
            break;
        }
        }
    }
}
//   
/**
*功能：按姓名查找学生信息
* 参数：STU_LINK* phead, char* pname
* 返回值：return p  指针数组
**/
char** name_c(STU_LINK* phead, char* pname) {
    STU_LINK* ptmp = phead;
    static char* p[100] = { 0 }; //存放相同姓名学生的地址，p[0]存放数量
    int i = 1;     // 计数
    memset(p, 0, sizeof(STU_LINK * [100]));//初始化
    while (ptmp->pnext) {
        if (strcmp(ptmp->pnext->s.name, pname) == 0) {
            p[i] = ptmp->pnext;
            (*p)++;
            i++;
        }
        ptmp = ptmp->pnext;
    }
    return p;
}
/**
*功能：姓名查询实现
* 参数：
* 返回值：
**/
void name_c_c() {
    while (1) {
        char name[20] = { 0 };
        char** p = NULL;
        int i = 1;
        printf("请输入想要查询的姓名\n");
        scanf("%s", name);
        clear();
        p = name_c(Ph, name);
        if (*(int*)p == 0) {
            clearScreen();
            printf("查无此人\n");
            sprintf(err_name, "按姓名查询学生信息");
            sprintf(err_buf, "查无此人");
            history_info(err_name, err_buf);
            while (1) {
                printf("1.继续查询\n");
                printf("2.退出\n");
                scanf("%d", &i);
                clear();
                if (i != 1 && i != 2) {
                    clearScreen();
                    printf("输入错误，请重新输入\n");
                }
                else if (i == 1) {
                    break;
                }
                else
                    clearScreen();
                    return;
            }
        }
        else {
            printf("一共由%d个相同姓名的学生\n", *(long*)p);
            printf("学号\t姓名\t性别\t分数\n");
            for (i = 1; i <= *(long*)p; i++) {
                printf("%d\t%s\t%s\t%.2f\n",
                    (*((STU_LINK**)p + i))->s.id,
                    (*((STU_LINK * *)p + i))->s.name,
                    (*((STU_LINK * *)p + i))->s.sex,
                    (*((STU_LINK * *)p + i))->s.score
                );
            };

            sprintf(err_name, "按姓名查询学生信息");
            sprintf(err_buf, "查询成功");
            history_info(err_name, err_buf);

            while (1) {
                printf("1.继续查询\n");
                printf("2.退出\n");
                scanf("%d", &i);
                clear();
                if (i != 1 && i != 2) {
                    clearScreen();
                    printf("输入错误，请重新输入\n");
                }
                else if (i == 1) {
                    break;
                }
                else
                    return;
            }
        }
    }
    
}

/**
*功能：按学号查找学生信息
* 参数：STU_LINK* phead, int id
* 返回值：   return ptmp->pnext;      return NULL;
**/
STU_LINK* id_c(STU_LINK* phead, int id) {
    STU_LINK* ptmp = phead;
    while (ptmp->pnext) {
        if (ptmp->pnext->s.id == id) {
            return ptmp->pnext;
        }
        ptmp = ptmp->pnext;
    }
    return NULL;
}
/**
*功能：学号查询实现
* 参数：
* 返回值：
**/
void id_c_c() {
    int id = 0;
    STU_LINK* ptmp = NULL;
    while (1) {
        printf("请输入需要查找的学号\n");
        scanf("%d", &id);
        clear();
        ptmp = id_c(Ph, id);
        if (ptmp == NULL) {
            printf("查无此号\n");

            sprintf(err_name, "按学号查询学生信息");
            sprintf(err_buf, "查无此号");
            history_info(err_name, err_buf);
        }
        else {
            printf("学号\t姓名\t性别\t分数\n");
            printf("%d\t%s\t%s\t%.2f\n",
                ptmp->s.id,
                ptmp->s.name,
                ptmp->s.sex,
                ptmp->s.score
            );

            sprintf(err_name, "按学号查询信息");
            sprintf(err_buf, "查询成功");
            history_info(err_name, err_buf);
        }
        while (1) {
            printf("1.继续查询\n");
            printf("2.退出\n");
            scanf("%d", &id);
            clear();
            if (id != 1 && id != 2) {
                clearScreen();
                printf("输入错误，请重新输入\n");
            }
            else if (id == 1) {
                break;
            }
            else
                clearScreen();
                return;
        }
        
    }
}

/**
*功能：按分数段查找学生信息
* 参数：STU_LINK* phead, float beg_sco, float end_sco
* 返回值：return p_s  指针数组
**/
char** score_c(STU_LINK* phead, float beg_sco, float end_sco) {
    STU_LINK* ptmp = phead;
    static char* p_s[100] = { 0 }; //存放相同姓名学生的地址，p[0]存放数量
    int i = 1;     // 计数
    memset(p_s, 0, sizeof(STU_LINK * [100]));//初始化
    while (ptmp->pnext) {
        if (ptmp->pnext->s.score > beg_sco && ptmp->pnext->s.score < end_sco) {
            p_s[i] = ptmp->pnext;
            (*p_s)++;
            i++;
        }
        ptmp = ptmp->pnext;
    }
    return p_s;
}
/**
*功能：按分数段查找学生信息实现
* 参数：
* 返回值：
**/
void score_c_c() {
    
    float beg_sco = 0;	//读入起始分数线
    float end_sco = 0;	//读入截至分数线
    float sbeg_sco = 0;	//学生起始分数线
    float send_sco = 0;	//学生截至分数线
    char ch = 0;
    STU_LINK* ptmp = Ph;
    sort_score(Ph);
    sbeg_sco = Ph->pnext->s.score;  //系统中最低分
    //ptmp = ptmp->pnext;
    while (ptmp->pnext) {
        ptmp = ptmp->pnext;
    }
    send_sco = ptmp->s.score;
    while (1) {
        while (1) {//读入区间并判断是否合法
            printf("学生的成绩区间为：%.2f ---- %.2f\n", sbeg_sco, send_sco);
            printf("请输入起始分数线\n");
            scanf("%f", &beg_sco);
            clear();
            printf("请输入截至分数线\n");
            scanf("%f", &end_sco);
            clear();
            if (beg_sco < sbeg_sco || beg_sco > send_sco || end_sco < sbeg_sco || end_sco > send_sco || beg_sco > end_sco) {
                printf("成绩区间非法，请重新输入\n");

                sprintf(err_name, "按分数区间查询学生信息");
                sprintf(err_buf, "分数区间非法");
                history_info(err_name, err_buf);
            }
            else
                break;
        }
        ptmp = Ph;
        printf("学号\t姓名\t性别\t分数\n");
        while (ptmp->pnext) {
            if (ptmp->pnext->s.score <= end_sco && ptmp->pnext->s.score >= beg_sco) {
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->pnext->s.id,
                    ptmp->pnext->s.name,
                    ptmp->pnext->s.sex,
                    ptmp->pnext->s.score
                );
            }

            ptmp = ptmp->pnext;

        }
        sprintf(err_name, "按分数段查询学生信息");
        sprintf(err_buf, "查询成功");
        history_info(err_name, err_buf);
        while (1) {
            printf("输入 Y 继续查询\n");
            scanf("%c", &ch);
            clear();
            if (ch != 'y' && ch != 'Y') {
                clearScreen();
                break;
            }
            else {
                clearScreen();
                return;
            }
        }

    }
    

}

/**
*功能:按成绩升序排列
**/

void sort_score(STU_LINK* phead) {
    STU_LINK* ptmp = phead->pnext;
    stu date = { 0 };
    for (int i = 1; i <= count - 1; i++) {
        ptmp = phead;
        for (int j = 1; j <= count; j++ ) {
            if (ptmp->pnext == NULL) {
                break;
            }
            if (ptmp->s.score > ptmp->pnext->s.score ) {
                date = ptmp->s;
                ptmp->s = ptmp->pnext->s;
                ptmp->pnext->s = date;
            }
           
            if (ptmp->pnext->pnext == NULL) {
                break;
            }
            ptmp = ptmp->pnext;
            
        }
    }

}

/**
*功能:按学号升序排列
**/

void sort_id(STU_LINK* phead) {
    STU_LINK* ptmp = phead->pnext;
    stu date = { 0 };
    for (int i = 1; i <= count - 1; i++) {
        ptmp = phead;
        for (int j = 1; j <= count; j++) {
            if (ptmp->pnext == NULL) {
                break;
            }
            if (ptmp->s.id > ptmp->pnext->s.id) {
                date = ptmp->s;
                ptmp->s = ptmp->pnext->s;
                ptmp->pnext->s = date;
            }

            if (ptmp->pnext->pnext == NULL) {
                break;
            }
            ptmp = ptmp->pnext;

        }
    }

}

/**
*功能:按性别查找
**/

void sex_c(STU_LINK* phead) {

}

/**
*功能：修改学生信息
* 
**/
void change_stu_info(STU_LINK* phead) {
    int id = 0;		//学号
    char ch = 0;	//判断字符
    int input = 0;	//存放修改信息标志
    stu n_stu = { 0 };	//存放要修改的信息
    STU_LINK* ptmp = Ph;
    while (1) {
        printf("请输入需要查找的学号\n");
        scanf("%d", &id);
        clear();
        ptmp = id_c(Ph, id);
        if (ptmp == NULL) {
            printf("查无此号\n");

            sprintf(err_name, "修改学生信息");
            sprintf(err_buf, "查无此号");
            history_info(err_name, err_buf);

            printf("按 Y 继续查询\n");
            ch = 0;
            scanf("%c", &ch);
            clear();
            if (ch == 'y' || ch == 'Y') {
                continue;
            }
            else {
                clearScreen();
                break;
            }
          
        }
        else {
            printf("学号\t姓名\t性别\t分数\n");
            printf("%d\t%s\t%s\t%.2f\n",
                ptmp->s.id,
                ptmp->s.name,
                ptmp->s.sex,
                ptmp->s.score
            );
        }
       
        do {	//修改信息
            printf("请输入需要修改的信息：\n");
            printf("1.修改学号\n");
            printf("2.修改姓名\n");
            printf("3.修改性别\n");
            printf("4.修改分数\n");
            printf("0.退出\n");
            input = 5;  //读入判定
            scanf("%d", &input);
            clear();
            switch (input) {
            case 1: {	//修改学号
                printf("请输入新的学号：\n");
                scanf("%d", &n_stu.id);
                clear();
                if (n_stu.id <= 0) {
                    printf("新的学号非法\n");

                    sprintf(err_name, "修改学生信息");
                    sprintf(err_buf, "新的学号非法");
                    history_info(err_name, err_buf);
                    
                    break;
                }
                if (check_id(phead, n_stu.id)) {
                    ptmp->s.id = n_stu.id;
                    clearScreen();
                    printf("修改成功\n");
                    printf("学号\t姓名\t性别\t分数\n");
                    printf("%d\t%s\t%s\t%.2f\n",
                        ptmp->s.id,
                        ptmp->s.name,
                        ptmp->s.sex,
                        ptmp->s.score
                    );
                    sprintf(err_name, "修改学生信息");
                    sprintf(err_buf, "学号修改成功");
                    history_info(err_name, err_buf);
                    break;
                }
                else {
                    printf("该学号已经被使用\n");

                    sprintf(err_name, "修改学生信息");
                    sprintf(err_buf, "新的学号已被使用");
                    history_info(err_name, err_buf);
                    break;
                }

            }
            case 2: {	//修改姓名
                printf("请输入新的姓名：\n");
                scanf("%s", ptmp->s.name);
                clear();
                clearScreen();
                printf("姓名修改成功\n");
                printf("学号\t姓名\t性别\t分数\n");
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->s.id,
                    ptmp->s.name,
                    ptmp->s.sex,
                    ptmp->s.score
                );

                sprintf(err_name, "修改学生信息");
                sprintf(err_buf, "姓名修改成功");
                history_info(err_name, err_buf);
                break;
            }
            case 3: {	//修改性别
                printf("请输入新的性别：\n");
                scanf("%s", ptmp->s.sex);
                clear();
                clearScreen();
                printf("性别修改成功\n");
                printf("学号\t姓名\t性别\t分数\n");
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->s.id,
                    ptmp->s.name,
                    ptmp->s.sex,
                    ptmp->s.score
                );

                sprintf(err_name, "修改学生信息");
                sprintf(err_buf, "性别修改成功");
                history_info(err_name, err_buf);
                break;
            }
            case 4: {	//修改分数
                printf("请输入新的分数:\n");
                scanf("%f", &ptmp->s.score);
                clear();
                clearScreen();
                printf("分数修改成功\n");
                printf("学号\t姓名\t性别\t分数\n");
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->s.id,
                    ptmp->s.name,
                    ptmp->s.sex,
                    ptmp->s.score
                );
                sprintf(err_name, "修改学生信息");
                sprintf(err_buf, "分数修改成功");
                history_info(err_name, err_buf);
                break;
            }
            case 0: {
                return;
            }
            default: {
                printf("非法输入，按 Y 继续输入\n");
                scanf("%c", &ch);
                clear();
                if (ch == 'y' || ch == 'Y') {
                    clearScreen();
                    break;
                }
                else {
                    clearScreen();
                    return;
                }
            }
            }

        } while (input);
        printf("输入 Y 继续修改学生信息\n");
        scanf("%c", &ch);
        clear();
        if (ch == 'y' || ch == 'Y')
            continue;
        else
            break;

    }





}

/**
*功能：根据学号删除学生信息
* 参数：STU_LINK* phead, int id
* 返回值： return 0;   //找不到   return 1;   //删除成功
**/
int delete(STU_LINK* phead, int id) {
    STU_LINK* ptmp = phead;
    STU_LINK* pdel = NULL;
    pdel = id_c(Ph, id);
    if (pdel == NULL) {
        return 0;   //找不到
    }
    else {
        while (ptmp->pnext - pdel) {
            ptmp = ptmp->pnext;
            if (ptmp == NULL) {
                break;
            }
        }
        ptmp->pnext = pdel->pnext;
        free(pdel);
    }
    return 1;   //删除成功
}

/***
*功能：删除学生信息实现
* 参数；
* 返回值：
**/
void delete_c (STU_LINK* phead) {
    int num = 0;
    int id = 0;
    char ch = 0;
    STU_LINK* ptmp = NULL;
    do {
        while (1) {
            printf("请选择删除方式\n");
            printf("1.按学号查询\n");
            printf("2.按姓名查询\n");
            printf("3.退出\n");
            scanf("%d", &num);
            clear();
            if (num < 1 && num > 3) {
                clearScreen();
                printf("输入错误，请重新输入\n");
            }
            else {
                clearScreen();
                break;
            }

        }
        switch (num) {
        case 1: {   
            ptmp = NULL;
            while (1) {
                printf("请输入需要查找的学号\n");
                scanf("%d", &id);
                clear();
                ptmp = id_c(Ph, id);
                if (ptmp == NULL) {
                    clearScreen();
                    printf("查无此号\n");

                    sprintf(err_name, "删除学生信息");
                    sprintf(err_buf, "查无此号");
                    history_info(err_name, err_buf);

                    printf("是否继续查找（y/n）\n");
                    scanf("%c", &ch);
                    clear();
                    if (ch == 'y' || ch == 'Y') {
                        clearScreen();
                        continue;
                    }
                    else {
                        clearScreen();
                        break;
                    }

                }
                else {
                    printf("学号\t姓名\t性别\t分数\n");
                    printf("%d\t%s\t%s\t%.2f\n",
                        ptmp->s.id,
                        ptmp->s.name,
                        ptmp->s.sex,
                        ptmp->s.score
                    );
                    
                }
                printf("输入 Y 确定删除\n");
                scanf("%c", &ch);
                clear();
                if (ch == 'y' || ch == 'Y') {
                    clearScreen();
                    delete(phead, id);
                    count--;
                    printf("删除成功\n");
                    sprintf(err_name, "删除学生信息");
                    sprintf(err_buf, "按学号删除成功");
                    history_info(err_name, err_buf);
                }
                else {
                    clearScreen();
                    
                }
                printf("输入 Y 继续按学号删除\n");
                scanf("%c", &ch);
                clear();
                if (ch == 'y' || ch == 'Y') {
                    continue;
                }
                else {
                    break;
                }
            }
          
           
            break;
        }
        case 2: {
                while (1) {
                    char name[20] = { 0 };
                    char** p = NULL;
                    int i = 1;
                    printf("请输入想要查询的姓名\n");
                    scanf("%s", name);
                    clear();
                    p = name_c(Ph, name);
                    if (*(int*)p == 0) {
                        clearScreen();
                        printf("查无此人\n");

                        sprintf(err_name, "删除学生信息");
                        sprintf(err_buf, "查无此人");
                        history_info(err_name, err_buf);

                        while (1) {
                            printf("1.继续查询\n");
                            printf("2.退出\n");
                            scanf("%d", &i);
                            clear();
                            if (i != 1 && i != 2) {
                                clearScreen();
                                printf("输入错误，请重新输入\n");
                            }
                            else if (i == 1) {
                                break;
                            }
                            else
                                return;
                        }
                    }
                    else {
                        printf("一共由%d个相同姓名的学生\n", *(long*)p);
                        printf("学号\t姓名\t性别\t分数\n");
                        for (i = 1; i <= *(long*)p; i++) {
                            printf("%d\t%s\t%s\t%.2f\n",
                                (*((STU_LINK**)p + i))->s.id,
                                (*((STU_LINK**)p + i))->s.name,
                                (*((STU_LINK**)p + i))->s.sex,
                                (*((STU_LINK**)p + i))->s.score
                            );
                        };
                        while (1) {
                            printf("请输入学号进行删除\n");
                            
                            scanf("%d", &id);
                            clear();
                            for (i = 1; i <= *(long*)p; i++) {
                                if ((*((STU_LINK**)p + i))->s.id == id) {
                                    printf("%d\t%s\t%s\t%.2f\n",
                                        (*((STU_LINK**)p + i))->s.id,
                                        (*((STU_LINK**)p + i))->s.name,
                                        (*((STU_LINK**)p + i))->s.sex,
                                        (*((STU_LINK**)p + i))->s.score
                                    );
                                    printf("是否确定删除(y/n)\n");
                                    scanf("%c", &ch);
                                    clear();
                                    if (ch == 'y' || ch == 'Y') {
                                        clearScreen();
                                        delete(phead, id);
                                        count--;
                                        printf("删除成功\n");
                                        sprintf(err_name, "删除学生信息");
                                        sprintf(err_buf, "按姓名删除成功");
                                        history_info(err_name, err_buf);
                                        break;
                                    }
                                    else {
                                        clearScreen();
                                        break;
                                    }
                               }
                            };
                            if (i == *(long*)p + 1) {
                                printf("学号输入错误，请重新输入\n");
                             
                            }
                            printf("输入 Y 继续按照姓名查询并删除\n");
                            scanf("%c", &ch);
                            clear();
                            if (ch == 'y' || ch == 'Y') {
                                clearScreen();
                                break;
                            }
                            else {
                                clearScreen();
                                return;
                            }
                        }
                    }
                }

            
            break;
        }
        case 3: {

            break;
        }
        }
    } while (num - 3);
    
    

}

/**
*功能：   拍序实现
* 参数：
* 返回值:
**/
void sort_c(STU_LINK* phead) {
    int num = 0;
    char ch = 0;
    STU_LINK* ptmp = phead;
    while (1) { //输入判定
        printf("请选择排序方式\n");
        printf("1.按成绩排序\n");
        printf("2.按学号排序\n");
        printf("3.退出\n");
        scanf("%d", &num);
        clear();
        if (num != 1 && num != 2 && num != 3) {
            printf("非法输入，按 Y 继续输入\n");
            scanf("%c", &ch);
            if (ch == 'Y' || ch == 'y') {
                clearScreen();
                continue;
            }
            else {
                clearScreen();
                return;
            }
        }
        else {
            clearScreen();
            break;
        }
    }
    //功能实现
    switch (num) {
        //按成绩排列
    case 1: {
        while (1) {
            num = 0;
            ch = 0;
            printf("请选择排序顺序\n");
            printf("1.按成绩升序排序\n");
            printf("2.按降序降序排序\n");
            printf("3.退出\n");
            scanf("%d", &num);
            clear();
            if (num != 1 && num != 2 && num != 3) {
                printf("非法输入，按 Y 重新选择\n");
                scanf("%c", &ch);
                if (ch == 'y' || ch == 'Y') {
                    clearScreen();
                    continue;
                }
                else {
                    clearScreen();
                    break;
                }
            }
            else {
                clearScreen();
                break;
            }
        }
        sort_score(phead);
        switch (num) {
        case 1: {   //成绩升序
            ptmp = phead;
            
            printf("学号\t姓名\t性别\t分数\n");
            while (ptmp->pnext) {
              
                    printf("%d\t%s\t%s\t%.2f\n",
                        ptmp->pnext->s.id,
                        ptmp->pnext->s.name,
                        ptmp->pnext->s.sex,
                        ptmp->pnext->s.score
                    );
                

                ptmp = ptmp->pnext;

            }

            break;
        }
        case 2: {   //成绩降序
            ptmp = phead;
            while (ptmp->pnext) {//寻找尾节点
                ptmp = ptmp->pnext;
            }
            printf("学号\t姓名\t性别\t分数\n");
            while (ptmp->plast) {
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->s.id,
                    ptmp->s.name,
                    ptmp->s.sex,
                    ptmp->s.score
                );
                ptmp = ptmp->plast;
            }

            break;
        }
        case 3: {

            break;
        }
        } 
        break;
    }
    //按学号排列
    case 2: {
        while (1) {
            num = 0;
            ch = 0;
            printf("请选择排序顺序\n");
            printf("1.按学号升序排序\n");
            printf("2.按学号降序排序\n");
            printf("3.退出\n");
            scanf("%d", &num);
            clear();
            if (num != 1 && num != 2 && num != 3) {
                printf("非法输入，按 Y 重新选择\n");
                scanf("%c", &ch);
                if (ch == 'y' || ch == 'Y') {
                    clearScreen();
                    continue;
                }
                else {
                    clearScreen();
                    break;
                }
            }
            else {
                clearScreen();
                break;
            }
        }
        sort_id(phead);
        switch (num) {
        case 1: {   //学号升序
            ptmp = phead;
          
            printf("学号\t姓名\t性别\t分数\n");
            while (ptmp->pnext) {

                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->pnext->s.id,
                    ptmp->pnext->s.name,
                    ptmp->pnext->s.sex,
                    ptmp->pnext->s.score
                );


                ptmp = ptmp->pnext;

            }

            break;
        }
        case 2: {   //成绩降序
            ptmp = phead;
            while (ptmp->pnext) {//寻找尾节点
                ptmp = ptmp->pnext;
            }
            printf("学号\t姓名\t性别\t分数\n");
            while (ptmp->plast) {
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->s.id,
                    ptmp->s.name,
                    ptmp->s.sex,
                    ptmp->s.score
                );
                ptmp = ptmp->plast;
            }

            break;
        }
        case 3: {

            break;
        }
        }
        break;
    }
    //退出
    case 3: {

        break;
    }
    default: {

        break;
    }
    }
 
}

// 判断字符串是否全为中文
int isChineseString(char* str) {

    while (*str != '\0') {


        if (!(*str > 127 || *str < 0)) {
            return 0;
        }
        str = str + 1;
    }
    return 1;
}

/*
    功能：计算学号位数
    参数：int id
    返回值
        dig		不等返回输入学号位数
        id_len	相等返回规定位数
*/
int check_stu_id(int id) {

    int dig = 0;

    // 计算学号总位数
    while (id > 0) {
        dig++;
        id /= 10;
    }

    // 和要求长度校验
    if (dig < id_len) {
        return id_len;
    }
    else {
        return dig;
    }

}