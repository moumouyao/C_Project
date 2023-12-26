#include "main.h"
//数组结构体的添加
void mylist_add(int ckd,uint32_t device_id)
{
    struct client_struct * tmpp  = malloc(sizeof(struct client_struct));
    struct client_struct * lastp = list_head; 
    tmpp->pnext = NULL;
    tmpp->ckd = ckd;
    tmpp->device_id = device_id;  
    if(list_head == NULL)
    {
        list_head = tmpp;
        return ;
    }
    while(lastp->pnext!=NULL)
    {
        lastp = lastp->pnext;
    }
    lastp->pnext = tmpp;
    
}
//数组结构体的删除
void mylist_remove(int ckd)
{
    struct client_struct * oldp  = NULL;
    struct client_struct * lastp = list_head; 
    while(lastp!=NULL)
    {
        if(lastp->ckd == ckd)
        {
            if(lastp == list_head)
            {
                free(list_head);
                list_head = NULL;
                break ;
            }
            oldp ->pnext = lastp->pnext;
            free(lastp);
            break;
        }
        oldp = lastp;
        lastp = lastp->pnext;
    }

    //客户离线
}

int byckd_get_device_id(uint32_t device_id)
{
    struct client_struct * lastp = list_head; 
    while(lastp!=NULL)
    {
        if(lastp->device_id == device_id)
        {
            return lastp->ckd;
        }
        lastp = lastp->pnext;
    }
}

uint32_t bydeviceid_get_ckd(int ckd)
{
    struct client_struct * lastp = list_head; 
    while(lastp!=NULL)
    {
        if(lastp->ckd == ckd)
        {
            return lastp->device_id;
        }
        lastp = lastp->pnext;
    }
}