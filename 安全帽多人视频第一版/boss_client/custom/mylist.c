#include "custom.h"
//数组结构体的添加
struct client_struct * list_head = NULL; 
void mylist_add(char * name,uint32_t device_id)
{
    struct client_struct * tmpp  = malloc(sizeof(struct client_struct));
    struct client_struct * lastp = list_head; 
    tmpp->pnext = NULL;
    memset(tmpp->name,0,32);
    strcpy(tmpp->name,name);
    tmpp->device_id = device_id;  
    tmpp->list_id = client_count;
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
void mylist_remove(uint32_t device_id)
{
    struct client_struct * oldp  = NULL;
    struct client_struct * lastp = list_head; 
    while(lastp!=NULL)
    {
        if(lastp->device_id== device_id)
        {

            if(lastp == list_head)
            {
                memset(list_head,0,sizeof(struct client_struct));
                free(list_head);
                list_head = NULL;
                break ;
            }
            oldp ->pnext = lastp->pnext;
            memset(list_head,0,sizeof(struct client_struct));
            free(lastp);
            break;
        }
        oldp = lastp;
        lastp = lastp->pnext;
    }

    //客户离线
}


char * bydeviceid_get_name(uint32_t device_id)
{
    struct client_struct * lastp = list_head; 
    while(lastp!=NULL)
    {
        if(lastp->device_id == device_id)
        {
            return lastp->name;
        }
        lastp = lastp->pnext;
    }
}
uint32_t byname_get_device(char *name)
{
    struct client_struct * lastp = list_head; 
    while(lastp!=NULL)
    {
        if(strcmp(lastp->name,name)==0)
        {
            return lastp->device_id;
        }
        lastp = lastp->pnext;
    }
}

uint32_t byname_get_listid(char *name)
{
    struct client_struct * lastp = list_head; 
    while(lastp!=NULL)
    {
        if(strcmp(lastp->name,name)==0)
        {
            return lastp->list_id;
        }
        lastp = lastp->pnext;
    }
}