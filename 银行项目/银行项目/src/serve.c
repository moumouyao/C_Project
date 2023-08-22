#include <common.h>

void stop(int a);

int c2smsgid, s2cmsgid;	//信息队列id
struct INFOR{
	char pathname[128];	//可执行文件路径
	char xname[32];	//可执行文件文件名
	pid_t pid; //子进程pid
};
//	功能
struct INFOR infor[] = {{"/home/yyy/linux/银行项目/src/open", "open"},	//开户功能		
						{"/home/yyy/linux/银行项目/src/dep", "dep"},
						{"/home/yyy/linux/银行项目/src/disdep", "disdep"},
						{"/home/yyy/linux/银行项目/src/check", "check"},
						{"/home/yyy/linux/银行项目/src/trans", "trans"},
						{"/home/yyy/linux/银行项目/src/delac", "delac"}
						};
int main(){
	signal(2, stop);//信号改造
	//创建消息队列
s2cmsgid = msgget(S2CKEY, IPC_CREAT | 0644);
	if(s2cmsgid == -1){
		perror("msgget err");
		return -1;
	}
	c2smsgid = msgget(C2SKEY, IPC_CREAT | 0644);
	if(c2smsgid == -1){
		perror("msgget err");
		return -1;//创建子进程
	}

	//创建子进程{}
	for(int i = 0; i < sizeof(infor) / sizeof(infor[0]); i++){

		infor[i].pid = fork();
		if(infor[i].pid == 0){
			execl(infor[i].pathname, infor[i].xname, NULL);
		}
	}


	printf("ctrl + C退出服务器\n");
	while(1){};
}

//退出函数
void stop(int a){
	for(int i = 0; i < sizeof(infor) / sizeof(infor[0]); i++){

		kill(infor[i].pid, 9);

	}
		msgctl(c2smsgid, IPC_RMID, NULL);
		msgctl(s2cmsgid, IPC_RMID, NULL);
		exit(0);

}
