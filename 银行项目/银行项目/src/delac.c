#include <common.h>

//函数声明
int CREATID();
int save_info(struct C2SMSG r_buf);

//全局变量
int s2cmsgid, c2smsgid;

int main(){
	int fd = 0;
	pid_t  pid;
	char filename[64] = {0};
	printf("服务器销户功能启动成功\n");
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
	printf("销户功能准备第一次接收信息\n");
	if(msgrcv(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), DELAC, 0) == -1){
		perror("msgrcv err");
		return 0;
	}
	printf("销户功能第一次接收信息完成\n");
	printf("销户账号:%d\n", r_buf.data.id);
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
	printf("销户功能准备第一次发送信息\n");
	if(msgsnd(s2cmsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
		perror("msgsnd err");
		return 0;
	}

	printf("销户功能第一次发送信息完成\n");
	printf("销户功能准备第二次接收信息\n");
	if(msgrcv(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), DELAC, 0) == -1){
		perror("msgrcv err");
		return 0;
	}
	printf("销户功能第二次接收信息完毕\n");
	if(r_buf.data.id == -1){
		printf("客户端异常操作，销户功能已重置\n");
		continue;
	}
	w_buf.type = r_buf.data.pid;
	w_buf.id = r_buf.data.id;
	w_buf.money = r_buf.data.money;
	if(save_info(r_buf) == -1){
		strcpy(w_buf.info, "销户失败");
	}
	else{
		strcpy(w_buf.info, "销户成功");
	}
	printf("销户功能准备第二次发送信息\n");
	if(msgsnd(s2cmsgid, &w_buf, sizeof(struct S2CMSG) - sizeof(long), 0) == -1){
		perror("msgsnd err");
		return 0;
	}
	printf("销户功能第二次发送信息完成\n");
	printf("销户完成\n");
	
	}

}

//保存信息

int  save_info(struct C2SMSG r_buf){
	char filename[64] = {0};
	sprintf(filename, "%d.txt", r_buf.data.id);
	int fd = unlink(filename);
	if(fd == -1){
		perror("unlink err");
		return -1;
	}
	return 0;
}

