#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "menu.h"
#include "stu_link.h"
#include <stdlib.h>
#include <string.h>
#include "File_stu.h"

extern STU_LINK* Ph;
int count = 0;

//��������
void clear() {
    while (getchar() != '\n');
}
//����
void clearScreen() {
    printf("\033[2J\033[1;1H");
}

/*
*���ܣ���������ڵ�
*������STU_LINK* phead ����ͷ
*����ֵ���ɹ� ��ַ      ʧ�� null
*/
STU_LINK* add_link(STU_LINK* phead) {
    STU_LINK* ptmp = NULL;
    ptmp = phead;
    while (ptmp->pnext) {        //Ѱ��β�ڵ�
        ptmp = ptmp->pnext;
    }
    if (ptmp->pnext = (STU_LINK*)malloc(sizeof(STU_LINK))) {    //���ٿռ�
        
        count++;
        printf("�����ɹ�\n%p\n", ptmp);
    }
    else {
        printf("����ʧ��\n");
        return NULL;
    }
    memset(ptmp->pnext, 0, sizeof(STU_LINK));//��ʼ��
    ptmp->pnext->plast = ptmp;
    return ptmp->pnext;
}

/*
*���ܣ����ѧ���Ƿ��ظ�
*������STU_LINK* phead, int id
*����ֵ��0 �ظ� 1���ظ�
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
 * ���ܣ�   ���ѧ����Ϣ
 * ������
 * ����ֵ��
 **/
void add_info(STU_LINK* phead) {
    int id = 0;
    int ret = 1;
    STU_LINK* pnew = NULL;
    STU_LINK* ptmp = phead;
    while (1) {   //����ѧ�Ų�����
        printf("������ѧ��\n");
        scanf("%d", &id);
        clear();
         if (id <= 0 || check_stu_id(id) != id_len) {
            printf("������ 1---%d ��Χ�ڵ�ѧ��\n", length - 1);
            continue;
        }
        if (check_id(phead, id)) {
            printf("����ѧ�ųɹ�\n");
            break;
        }
        else {
            clearScreen();
            printf("ѧ��Ϊ%d��ѧ���Ѵ���\n", id);

            sprintf(err_name, "�����Ϣ");
            sprintf(err_buf, "id�Ѿ�����");
            history_info(err_name, err_buf);
       

        }
    }
    pnew = add_link(phead);
    if (pnew == NULL)
    {
        printf("�ռ俪��ʧ��\n");
        sprintf(err_name, "�����Ϣ");
        sprintf(err_buf, "�ռ俪��ʧ��");
        history_info(err_name, err_buf);
    }
    pnew->s.id = id;    //����id
    while (1) {
        printf("����������\n");
        scanf("%s", pnew->s.name);
        clear();
        if (isChineseString(pnew->s.name) == 0) {
            printf("����������Ǵ����ģ�����������\n");
        }
        else if (isChineseString(pnew->s.name) == 1) {
            break;
        }
    }
    

    //�����Ա�
    while (1) {
        ret = 0;
        printf("��ѡ��ѧ���Ա�\n");
        printf("1.����\n");
        printf("2.Ů��\n");
     // printf("3.�˳�\n");
        scanf("%d", &ret);
        while ((getchar()) != '\n') {}
        if (ret != 1 && ret != 2) {
            printf("�Ƿ����룬������ѡ��\n");
        }
        else
            break;
    }
    switch (ret) {
    case 1: {
        strncpy(pnew->s.sex, "��", 15);
        break;
    }
    case 2: {
        strcpy(pnew->s.sex, "Ů");
        break;
    }
    //case 3: {	//ɾ��
    //    stu_info[count] = stu_info[count + 1];//����
    //    return -3;
    //}
    }

   
    
    do {

        printf("������ѧ������:\n");
        scanf("%f", &pnew->s.score);
        clear();
        if (pnew->s.score <= 0 || pnew->s.score > 1000) {
            printf("����������0-%d֮��!\n",f_score);
        }
        else {
            break;
        }
    } while (1);
    printf("��ӳɹ�\n");

    sprintf(err_name, "�����Ϣ");
    sprintf(err_buf, "��ӳɹ�");
    history_info(err_name, err_buf);

}
/**
*���ܣ��鿴ѧ����Ϣ
* ������
* ����ֵ��
**/
void ckeck_info(STU_LINK* phead) {
    int num = 0;
    while (1) {
        printf("��ѡ��鿴��ʽ\n");
        printf("%d.��������ѯ\n", NAME_C);
        printf("%d.��ѧ�Ų�ѯ\n", ID_C);
        printf("%d.�������β�ѯ\n", SCORE_C);
        printf("%d.���Ա��ѯ\n", SEX_C);
        printf("%d.�˳�\n", EXIT_C);
        scanf("%d", &num);
        clear();
        if (num > EXIT || num < NAME_C) {
            clearScreen();
            printf("�Ƿ����룬����������\n");
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
*���ܣ�����������ѧ����Ϣ
* ������STU_LINK* phead, char* pname
* ����ֵ��return p  ָ������
**/
char** name_c(STU_LINK* phead, char* pname) {
    STU_LINK* ptmp = phead;
    static char* p[100] = { 0 }; //�����ͬ����ѧ���ĵ�ַ��p[0]�������
    int i = 1;     // ����
    memset(p, 0, sizeof(STU_LINK * [100]));//��ʼ��
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
*���ܣ�������ѯʵ��
* ������
* ����ֵ��
**/
void name_c_c() {
    while (1) {
        char name[20] = { 0 };
        char** p = NULL;
        int i = 1;
        printf("��������Ҫ��ѯ������\n");
        scanf("%s", name);
        clear();
        p = name_c(Ph, name);
        if (*(int*)p == 0) {
            clearScreen();
            printf("���޴���\n");
            sprintf(err_name, "��������ѯѧ����Ϣ");
            sprintf(err_buf, "���޴���");
            history_info(err_name, err_buf);
            while (1) {
                printf("1.������ѯ\n");
                printf("2.�˳�\n");
                scanf("%d", &i);
                clear();
                if (i != 1 && i != 2) {
                    clearScreen();
                    printf("�����������������\n");
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
            printf("һ����%d����ͬ������ѧ��\n", *(long*)p);
            printf("ѧ��\t����\t�Ա�\t����\n");
            for (i = 1; i <= *(long*)p; i++) {
                printf("%d\t%s\t%s\t%.2f\n",
                    (*((STU_LINK**)p + i))->s.id,
                    (*((STU_LINK * *)p + i))->s.name,
                    (*((STU_LINK * *)p + i))->s.sex,
                    (*((STU_LINK * *)p + i))->s.score
                );
            };

            sprintf(err_name, "��������ѯѧ����Ϣ");
            sprintf(err_buf, "��ѯ�ɹ�");
            history_info(err_name, err_buf);

            while (1) {
                printf("1.������ѯ\n");
                printf("2.�˳�\n");
                scanf("%d", &i);
                clear();
                if (i != 1 && i != 2) {
                    clearScreen();
                    printf("�����������������\n");
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
*���ܣ���ѧ�Ų���ѧ����Ϣ
* ������STU_LINK* phead, int id
* ����ֵ��   return ptmp->pnext;      return NULL;
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
*���ܣ�ѧ�Ų�ѯʵ��
* ������
* ����ֵ��
**/
void id_c_c() {
    int id = 0;
    STU_LINK* ptmp = NULL;
    while (1) {
        printf("��������Ҫ���ҵ�ѧ��\n");
        scanf("%d", &id);
        clear();
        ptmp = id_c(Ph, id);
        if (ptmp == NULL) {
            printf("���޴˺�\n");

            sprintf(err_name, "��ѧ�Ų�ѯѧ����Ϣ");
            sprintf(err_buf, "���޴˺�");
            history_info(err_name, err_buf);
        }
        else {
            printf("ѧ��\t����\t�Ա�\t����\n");
            printf("%d\t%s\t%s\t%.2f\n",
                ptmp->s.id,
                ptmp->s.name,
                ptmp->s.sex,
                ptmp->s.score
            );

            sprintf(err_name, "��ѧ�Ų�ѯ��Ϣ");
            sprintf(err_buf, "��ѯ�ɹ�");
            history_info(err_name, err_buf);
        }
        while (1) {
            printf("1.������ѯ\n");
            printf("2.�˳�\n");
            scanf("%d", &id);
            clear();
            if (id != 1 && id != 2) {
                clearScreen();
                printf("�����������������\n");
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
*���ܣ��������β���ѧ����Ϣ
* ������STU_LINK* phead, float beg_sco, float end_sco
* ����ֵ��return p_s  ָ������
**/
char** score_c(STU_LINK* phead, float beg_sco, float end_sco) {
    STU_LINK* ptmp = phead;
    static char* p_s[100] = { 0 }; //�����ͬ����ѧ���ĵ�ַ��p[0]�������
    int i = 1;     // ����
    memset(p_s, 0, sizeof(STU_LINK * [100]));//��ʼ��
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
*���ܣ��������β���ѧ����Ϣʵ��
* ������
* ����ֵ��
**/
void score_c_c() {
    
    float beg_sco = 0;	//������ʼ������
    float end_sco = 0;	//�������������
    float sbeg_sco = 0;	//ѧ����ʼ������
    float send_sco = 0;	//ѧ������������
    char ch = 0;
    STU_LINK* ptmp = Ph;
    sort_score(Ph);
    sbeg_sco = Ph->pnext->s.score;  //ϵͳ����ͷ�
    //ptmp = ptmp->pnext;
    while (ptmp->pnext) {
        ptmp = ptmp->pnext;
    }
    send_sco = ptmp->s.score;
    while (1) {
        while (1) {//�������䲢�ж��Ƿ�Ϸ�
            printf("ѧ���ĳɼ�����Ϊ��%.2f ---- %.2f\n", sbeg_sco, send_sco);
            printf("��������ʼ������\n");
            scanf("%f", &beg_sco);
            clear();
            printf("���������������\n");
            scanf("%f", &end_sco);
            clear();
            if (beg_sco < sbeg_sco || beg_sco > send_sco || end_sco < sbeg_sco || end_sco > send_sco || beg_sco > end_sco) {
                printf("�ɼ�����Ƿ�������������\n");

                sprintf(err_name, "�����������ѯѧ����Ϣ");
                sprintf(err_buf, "��������Ƿ�");
                history_info(err_name, err_buf);
            }
            else
                break;
        }
        ptmp = Ph;
        printf("ѧ��\t����\t�Ա�\t����\n");
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
        sprintf(err_name, "�������β�ѯѧ����Ϣ");
        sprintf(err_buf, "��ѯ�ɹ�");
        history_info(err_name, err_buf);
        while (1) {
            printf("���� Y ������ѯ\n");
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
*����:���ɼ���������
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
*����:��ѧ����������
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
*����:���Ա����
**/

void sex_c(STU_LINK* phead) {

}

/**
*���ܣ��޸�ѧ����Ϣ
* 
**/
void change_stu_info(STU_LINK* phead) {
    int id = 0;		//ѧ��
    char ch = 0;	//�ж��ַ�
    int input = 0;	//����޸���Ϣ��־
    stu n_stu = { 0 };	//���Ҫ�޸ĵ���Ϣ
    STU_LINK* ptmp = Ph;
    while (1) {
        printf("��������Ҫ���ҵ�ѧ��\n");
        scanf("%d", &id);
        clear();
        ptmp = id_c(Ph, id);
        if (ptmp == NULL) {
            printf("���޴˺�\n");

            sprintf(err_name, "�޸�ѧ����Ϣ");
            sprintf(err_buf, "���޴˺�");
            history_info(err_name, err_buf);

            printf("�� Y ������ѯ\n");
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
            printf("ѧ��\t����\t�Ա�\t����\n");
            printf("%d\t%s\t%s\t%.2f\n",
                ptmp->s.id,
                ptmp->s.name,
                ptmp->s.sex,
                ptmp->s.score
            );
        }
       
        do {	//�޸���Ϣ
            printf("��������Ҫ�޸ĵ���Ϣ��\n");
            printf("1.�޸�ѧ��\n");
            printf("2.�޸�����\n");
            printf("3.�޸��Ա�\n");
            printf("4.�޸ķ���\n");
            printf("0.�˳�\n");
            input = 5;  //�����ж�
            scanf("%d", &input);
            clear();
            switch (input) {
            case 1: {	//�޸�ѧ��
                printf("�������µ�ѧ�ţ�\n");
                scanf("%d", &n_stu.id);
                clear();
                if (n_stu.id <= 0) {
                    printf("�µ�ѧ�ŷǷ�\n");

                    sprintf(err_name, "�޸�ѧ����Ϣ");
                    sprintf(err_buf, "�µ�ѧ�ŷǷ�");
                    history_info(err_name, err_buf);
                    
                    break;
                }
                if (check_id(phead, n_stu.id)) {
                    ptmp->s.id = n_stu.id;
                    clearScreen();
                    printf("�޸ĳɹ�\n");
                    printf("ѧ��\t����\t�Ա�\t����\n");
                    printf("%d\t%s\t%s\t%.2f\n",
                        ptmp->s.id,
                        ptmp->s.name,
                        ptmp->s.sex,
                        ptmp->s.score
                    );
                    sprintf(err_name, "�޸�ѧ����Ϣ");
                    sprintf(err_buf, "ѧ���޸ĳɹ�");
                    history_info(err_name, err_buf);
                    break;
                }
                else {
                    printf("��ѧ���Ѿ���ʹ��\n");

                    sprintf(err_name, "�޸�ѧ����Ϣ");
                    sprintf(err_buf, "�µ�ѧ���ѱ�ʹ��");
                    history_info(err_name, err_buf);
                    break;
                }

            }
            case 2: {	//�޸�����
                printf("�������µ�������\n");
                scanf("%s", ptmp->s.name);
                clear();
                clearScreen();
                printf("�����޸ĳɹ�\n");
                printf("ѧ��\t����\t�Ա�\t����\n");
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->s.id,
                    ptmp->s.name,
                    ptmp->s.sex,
                    ptmp->s.score
                );

                sprintf(err_name, "�޸�ѧ����Ϣ");
                sprintf(err_buf, "�����޸ĳɹ�");
                history_info(err_name, err_buf);
                break;
            }
            case 3: {	//�޸��Ա�
                printf("�������µ��Ա�\n");
                scanf("%s", ptmp->s.sex);
                clear();
                clearScreen();
                printf("�Ա��޸ĳɹ�\n");
                printf("ѧ��\t����\t�Ա�\t����\n");
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->s.id,
                    ptmp->s.name,
                    ptmp->s.sex,
                    ptmp->s.score
                );

                sprintf(err_name, "�޸�ѧ����Ϣ");
                sprintf(err_buf, "�Ա��޸ĳɹ�");
                history_info(err_name, err_buf);
                break;
            }
            case 4: {	//�޸ķ���
                printf("�������µķ���:\n");
                scanf("%f", &ptmp->s.score);
                clear();
                clearScreen();
                printf("�����޸ĳɹ�\n");
                printf("ѧ��\t����\t�Ա�\t����\n");
                printf("%d\t%s\t%s\t%.2f\n",
                    ptmp->s.id,
                    ptmp->s.name,
                    ptmp->s.sex,
                    ptmp->s.score
                );
                sprintf(err_name, "�޸�ѧ����Ϣ");
                sprintf(err_buf, "�����޸ĳɹ�");
                history_info(err_name, err_buf);
                break;
            }
            case 0: {
                return;
            }
            default: {
                printf("�Ƿ����룬�� Y ��������\n");
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
        printf("���� Y �����޸�ѧ����Ϣ\n");
        scanf("%c", &ch);
        clear();
        if (ch == 'y' || ch == 'Y')
            continue;
        else
            break;

    }





}

/**
*���ܣ�����ѧ��ɾ��ѧ����Ϣ
* ������STU_LINK* phead, int id
* ����ֵ�� return 0;   //�Ҳ���   return 1;   //ɾ���ɹ�
**/
int delete(STU_LINK* phead, int id) {
    STU_LINK* ptmp = phead;
    STU_LINK* pdel = NULL;
    pdel = id_c(Ph, id);
    if (pdel == NULL) {
        return 0;   //�Ҳ���
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
    return 1;   //ɾ���ɹ�
}

/***
*���ܣ�ɾ��ѧ����Ϣʵ��
* ������
* ����ֵ��
**/
void delete_c (STU_LINK* phead) {
    int num = 0;
    int id = 0;
    char ch = 0;
    STU_LINK* ptmp = NULL;
    do {
        while (1) {
            printf("��ѡ��ɾ����ʽ\n");
            printf("1.��ѧ�Ų�ѯ\n");
            printf("2.��������ѯ\n");
            printf("3.�˳�\n");
            scanf("%d", &num);
            clear();
            if (num < 1 && num > 3) {
                clearScreen();
                printf("�����������������\n");
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
                printf("��������Ҫ���ҵ�ѧ��\n");
                scanf("%d", &id);
                clear();
                ptmp = id_c(Ph, id);
                if (ptmp == NULL) {
                    clearScreen();
                    printf("���޴˺�\n");

                    sprintf(err_name, "ɾ��ѧ����Ϣ");
                    sprintf(err_buf, "���޴˺�");
                    history_info(err_name, err_buf);

                    printf("�Ƿ�������ң�y/n��\n");
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
                    printf("ѧ��\t����\t�Ա�\t����\n");
                    printf("%d\t%s\t%s\t%.2f\n",
                        ptmp->s.id,
                        ptmp->s.name,
                        ptmp->s.sex,
                        ptmp->s.score
                    );
                    
                }
                printf("���� Y ȷ��ɾ��\n");
                scanf("%c", &ch);
                clear();
                if (ch == 'y' || ch == 'Y') {
                    clearScreen();
                    delete(phead, id);
                    count--;
                    printf("ɾ���ɹ�\n");
                    sprintf(err_name, "ɾ��ѧ����Ϣ");
                    sprintf(err_buf, "��ѧ��ɾ���ɹ�");
                    history_info(err_name, err_buf);
                }
                else {
                    clearScreen();
                    
                }
                printf("���� Y ������ѧ��ɾ��\n");
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
                    printf("��������Ҫ��ѯ������\n");
                    scanf("%s", name);
                    clear();
                    p = name_c(Ph, name);
                    if (*(int*)p == 0) {
                        clearScreen();
                        printf("���޴���\n");

                        sprintf(err_name, "ɾ��ѧ����Ϣ");
                        sprintf(err_buf, "���޴���");
                        history_info(err_name, err_buf);

                        while (1) {
                            printf("1.������ѯ\n");
                            printf("2.�˳�\n");
                            scanf("%d", &i);
                            clear();
                            if (i != 1 && i != 2) {
                                clearScreen();
                                printf("�����������������\n");
                            }
                            else if (i == 1) {
                                break;
                            }
                            else
                                return;
                        }
                    }
                    else {
                        printf("һ����%d����ͬ������ѧ��\n", *(long*)p);
                        printf("ѧ��\t����\t�Ա�\t����\n");
                        for (i = 1; i <= *(long*)p; i++) {
                            printf("%d\t%s\t%s\t%.2f\n",
                                (*((STU_LINK**)p + i))->s.id,
                                (*((STU_LINK**)p + i))->s.name,
                                (*((STU_LINK**)p + i))->s.sex,
                                (*((STU_LINK**)p + i))->s.score
                            );
                        };
                        while (1) {
                            printf("������ѧ�Ž���ɾ��\n");
                            
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
                                    printf("�Ƿ�ȷ��ɾ��(y/n)\n");
                                    scanf("%c", &ch);
                                    clear();
                                    if (ch == 'y' || ch == 'Y') {
                                        clearScreen();
                                        delete(phead, id);
                                        count--;
                                        printf("ɾ���ɹ�\n");
                                        sprintf(err_name, "ɾ��ѧ����Ϣ");
                                        sprintf(err_buf, "������ɾ���ɹ�");
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
                                printf("ѧ�������������������\n");
                             
                            }
                            printf("���� Y ��������������ѯ��ɾ��\n");
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
*���ܣ�   ����ʵ��
* ������
* ����ֵ:
**/
void sort_c(STU_LINK* phead) {
    int num = 0;
    char ch = 0;
    STU_LINK* ptmp = phead;
    while (1) { //�����ж�
        printf("��ѡ������ʽ\n");
        printf("1.���ɼ�����\n");
        printf("2.��ѧ������\n");
        printf("3.�˳�\n");
        scanf("%d", &num);
        clear();
        if (num != 1 && num != 2 && num != 3) {
            printf("�Ƿ����룬�� Y ��������\n");
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
    //����ʵ��
    switch (num) {
        //���ɼ�����
    case 1: {
        while (1) {
            num = 0;
            ch = 0;
            printf("��ѡ������˳��\n");
            printf("1.���ɼ���������\n");
            printf("2.������������\n");
            printf("3.�˳�\n");
            scanf("%d", &num);
            clear();
            if (num != 1 && num != 2 && num != 3) {
                printf("�Ƿ����룬�� Y ����ѡ��\n");
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
        case 1: {   //�ɼ�����
            ptmp = phead;
            
            printf("ѧ��\t����\t�Ա�\t����\n");
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
        case 2: {   //�ɼ�����
            ptmp = phead;
            while (ptmp->pnext) {//Ѱ��β�ڵ�
                ptmp = ptmp->pnext;
            }
            printf("ѧ��\t����\t�Ա�\t����\n");
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
    //��ѧ������
    case 2: {
        while (1) {
            num = 0;
            ch = 0;
            printf("��ѡ������˳��\n");
            printf("1.��ѧ����������\n");
            printf("2.��ѧ�Ž�������\n");
            printf("3.�˳�\n");
            scanf("%d", &num);
            clear();
            if (num != 1 && num != 2 && num != 3) {
                printf("�Ƿ����룬�� Y ����ѡ��\n");
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
        case 1: {   //ѧ������
            ptmp = phead;
          
            printf("ѧ��\t����\t�Ա�\t����\n");
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
        case 2: {   //�ɼ�����
            ptmp = phead;
            while (ptmp->pnext) {//Ѱ��β�ڵ�
                ptmp = ptmp->pnext;
            }
            printf("ѧ��\t����\t�Ա�\t����\n");
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
    //�˳�
    case 3: {

        break;
    }
    default: {

        break;
    }
    }
 
}

// �ж��ַ����Ƿ�ȫΪ����
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
    ���ܣ�����ѧ��λ��
    ������int id
    ����ֵ
        dig		���ȷ�������ѧ��λ��
        id_len	��ȷ��ع涨λ��
*/
int check_stu_id(int id) {

    int dig = 0;

    // ����ѧ����λ��
    while (id > 0) {
        dig++;
        id /= 10;
    }

    // ��Ҫ�󳤶�У��
    if (dig < id_len) {
        return id_len;
    }
    else {
        return dig;
    }

}