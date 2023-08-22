#include <common.h>

//函数声明
int CREATID();
int save_info(struct C2SMSG r_buf);


//全局变量
int s2cmsgid, c2smsgid;

int main(){
	int fd = 0;
	pid_t pid = 0;
	char filename[64] = {0};
	printf("服务器转账功能启动成功\n");
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
	printf("转账功能准备第一次接收信息\n");
	if(msgrcv(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), TRANS, 0) == -1){
		perror("msgrcv err");
		return 0;
	}
	printf("转账功能第一次接收信息完成\n");
	sprintf(filename, "%d.txt", r_buf.data.id);
	fd = open(filename, O_RDONLY);
	if(fd == -1){
		perror("open err");
		return -1;
	}
	pid = r_buf.data.pid;
	read(fd, &r_buf, sizeof(struct C2SMSG));
	close(fd);
	//printf("%s\n", r_buf.data.passwd);
	r_buf.type = pid;
	printf("转账功能准备第一次发送信息\n");
	if(msgsnd(s2cmsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
		perror("msgsnd err");
		return 0;
	}
	printf("转账功能第一次发送信息完成\n");
	printf("转账功能准备第二次接收信息\n");
	if(msgrcv(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), TRANS, 0) == -1){
		perror("msgrcv err");
		return 0;
	}
	printf("转账功能第二次接收信息完成\n");
		if(r_buf.data.id == -1){
			printf("客户端异常操作，转账功能已重置\n");
		continue;
	}
	w_buf.type = r_buf.data.pid;
	w_buf.id = r_buf.data.id;
	w_buf.money = r_buf.data.money;
	//修改余额
	

	if(save_info(r_buf) == -1){
		strcpy(w_buf.info, "转账失败");
	}
	else{
		strcpy(w_buf.info, "转账成功");
	}
	printf("转账功能准备第二次发送信息\n");
	if(msgsnd(s2cmsgid, &w_buf, sizeof(struct S2CMSG) - sizeof(long), 0) == -1){
		perror("msgsnd err");
		return 0;
	}
	printf("转账功能第二次发送信息完成\n");
	printf("账户金额:%f\n", r_buf.data.money);
	printf("转账成功\n");
	
	}

}

//保存信息

int  save_info(struct C2SMSG r_buf){
	char filename1[64] = {0};
	char filename2[64] = {0};
	struct C2SMSG w_buf1, w_buf2;
	sprintf(filename1, "%d.txt", r_buf.data.id);
	int fd1 = open(filename1, O_RDWR);
	if(fd1 == -1){
		perror("open err");
		return -1;
	}
	sprintf(filename2, "%d.txt", r_buf.data.o_id);
	int fd2 = open(filename2, O_RDWR);
	if(fd2 == -1){
		perror("open err");
		return -1;
	}
	read(fd1, &w_buf1, sizeof(struct C2SMSG));
	read(fd2, &w_buf2, sizeof(struct C2SMSG));
//	printf("%f\n%f\n%f\n", w_buf1.data.money, w_buf2.data.money, r_buf.data.money);
	w_buf2.data.money = w_buf1.data.money - r_buf.data.money + w_buf2.data.money;
	w_buf1.data.money = r_buf.data.money;
//	printf("%f\n", w_buf1.data.money);
//	printf("%f\n", w_buf2.data.money);
	lseek(fd1, 0, 0);
	write(fd1, &w_buf1, sizeof(struct C2SMSG));
	close(fd1);
	lseek(fd2, 0, 0);
	write(fd2, &w_buf2, sizeof(struct C2SMSG));
	close(fd2);
	return 0;
}

