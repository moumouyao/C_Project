#include "sw_app.h"
#include "includes.h"
#include "sw_conf.h"
#include "sw_global.h"
#include "DisPlay_Data_Student.h"
#include "malloc.h"

DisPlay_Data_Student *create()
{
	DisPlay_Data_Student* p1, * p2, * head;
	head = NULL;
	p1=p2=mymalloc(SRAMIN,sizeof(DisPlay_Data_Student));
	head=p1;
	p2=p2;
	head->next=NULL;
	return head;
}
