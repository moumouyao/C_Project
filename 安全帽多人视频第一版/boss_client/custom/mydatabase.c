#include "custom.h"
#include <mysql/mysql.h>
#include "asm-generic/socket.h"

char * bydeviceid_get_database_name(uint32_t device_id)
{
    char * pname = NULL;
    static char namebuf[32]={0};
    char tmpbuf[128]={0};
    MYSQL * retmysql =NULL;
//1：初始化数据库的核心结构体
    MYSQL mysql ;
    mysql_init(&mysql);
//2:连接我们的数据库 
    retmysql = mysql_real_connect(&mysql,\
            "localhost",\
            "root",\
            "1",\
            "xyd_database",\
            0,NULL,0
    );
    if(retmysql == NULL)
    {
        printf("数据库连接失败！\r\n");
        return NULL;
    }
    //3：执行要执行的指令
    //我就插入一条数据 
    sprintf(tmpbuf,"select * from device_id_table where device_id='%u'",device_id);
 
    int ret =mysql_real_query(retmysql,\
        tmpbuf,\
        strlen(tmpbuf)
    );
    if(ret !=0)
    {
        printf("指令执行失败！\r\n");
        goto ERROR;
    }
    MYSQL_RES * retmsg = mysql_store_result(retmysql);
    //我们需要获取指令执行成功后结果！ 
    if(retmsg == NULL)
    {
        printf("没有结果集合可以获取！\r\n");
        goto ERROR;
    }
    int hang= mysql_num_rows(retmsg);
    if(hang==0)
    {
        goto ERROR;
    }else
    {
        char ** msg =NULL;
        msg = mysql_fetch_row(retmsg);
        pname = msg[1];
        strcpy(namebuf,pname);
        mysql_free_result(retmsg);//释放结果空间
    //4:关闭数据库的连接 
        mysql_close(retmysql);
        printf("返回真实姓名！\r\n");
        return namebuf;
    }
ERROR:
    if(retmysql!=NULL)
    {
        mysql_free_result(retmsg);//释放结果空间
        printf("释放结果空间!\r\n");
        return NULL;
    }
//4:关闭数据库的连接 
    mysql_close(retmysql);
    return NULL;
}

