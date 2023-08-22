#include <common.h>

//函数声明fff
int CREATID();
int save_info(struct C2SMSG r_buf);
void clears(); 


//全局变量
int s2cmsgid, c2smsgid;

int main(){
	printf("服务器查询功能启动成功\n");
	int fd = 0;
	char filename[64] = {0};
	pid_t pid = 0;
	//访问消息队列
	s2cmsgid = msgget(S2CKEY, IPC_CREAT | 0644);
	if(s2cmsgid == -1){
		perror("msgget err");
		return -1;
	}
	c2smsgid = msgget(C2SKEY, IPC_CREAT | 0644);
	if(c2smsgid == -1){
		perror("msgget err");
		return -1;
	}
	//创建结构体
	struct C2SMSG r_buf;
    struct S2CMSG w_buf;
	while(1){
	//接收消息队列信息
	printf("查询功能准备第一次接收信息\n");
	if(msgrcv(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), CHECK, 0) == -1){
		perror("msgrcv err");
		return 0;
	}
	printf("查询功能第一次接收信息完成\n");
	printf("查询账户为：%d\n", r_buf.data.id);
	sprintf(filename, "%d.txt", r_buf.data.id);
	fd = open(filename, O_RDONLY);
	if(fd == -1){
		perror("open err");
		return -1;
	}
	pid = r_buf.data.pid;
	read(fd, &r_buf, sizeof(struct C2SMSG));
	close(fd);
	r_buf.type = pid;
	printf("查询功能准备第一次发送信息\n");
	if(msgsnd(s2cmsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
		perror("msgsnd err");
		return 0;
	}
	printf("查询功能第一次发送信息完成\n");
	printf("查询功能准备第二次接收信息\n");
	if(msgrcv(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), CHECK, 0) == -1){
		perror("msgrcv err");
		return 0;
	}
	printf("查询功能第二次接收信息完成\n");
	if(r_buf.data.id == -1){
		printf("客户端错误操作，查询功能已重置\n");
		continue;
	}
	w_buf.type = r_buf.data.pid;
	w_buf.id = r_buf.data.id;
	w_buf.money = r_buf.data.money;
	if(save_info(r_buf) == -1){
		strcpy(w_buf.info, "查询失败");
	}
	else{
		strcpy(w_buf.info, "查询成功");
	}
	printf("查询功能准备第二次发送信息\n");
	if(msgsnd(s2cmsgid, &w_buf, sizeof(struct S2CMSG) - sizeof(long), 0) == -1){
		perror("msgsnd err");
		return 0;
	}
	printf("查询功能第二次发送信息完成\n");
	printf("账户金额:%f\n", r_buf.data.money);
	printf("查询完成\n");
	
	}

}

//保存信息

int  save_info(struct C2SMSG r_buf){
	char filename[64] = {0};
	sprintf(filename, "%d.txt", r_buf.data.id);
	int fd = open(filename, O_WRONLY);
	if(fd == -1){
		perror("open err");
		return -1;
	}
	write(fd, &r_buf, sizeof(r_buf));
	close(fd);
	return 0;
}
//清屏
void clears() {
    printf("\033[2J\033[1;1H");
}
