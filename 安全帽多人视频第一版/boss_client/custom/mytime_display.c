#include "custom.h"


void * pthread_show_time(void * arg)
{
    arg = arg;
    time_t tim = 0;
    struct tm * ltim= NULL ;
    char buf[64]={0};
    while(1)
    {
        tim = time(NULL);
        ltim = localtime(&tim);
        memset(buf,0,sizeof(buf));
        sprintf(buf,"%02d:%02d:%02d",ltim->tm_hour,ltim->tm_min,ltim->tm_sec);
        lv_label_set_text(guider_ui.screen_time_label,buf);
        memset(buf,0,sizeof(buf));
        sprintf(buf,"%d-%02d-%02d",ltim->tm_year+1900,ltim->tm_mon+1,ltim->tm_mday);
        lv_label_set_text(guider_ui.screen_date_label,buf);
        sleep(1);
    }
   
}
