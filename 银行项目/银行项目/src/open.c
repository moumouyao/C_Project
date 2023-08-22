#include <common.h>

//函数声明
int CREATID();
void save_info(struct C2SMSG r_buf);

//全局变量
int s2cmsgid, c2smsgid;

int main(){
	printf("服务器开户功能启动成功\n");
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
	printf("开户服务准备第一次接收信息\n");
	if(msgrcv(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), OPEN, 0) == -1){
		perror("msgrcv err");
		return 0;
	}
	w_buf.type = r_buf.data.pid;
	w_buf.id = CREATID();
	r_buf.data.id = w_buf.id;
	w_buf.money = r_buf.data.money;
	printf("开户服务第一次接收信息完成\n");
	if(w_buf.id == -1){
		strcpy(w_buf.info, "开户失败");
	}
	else{
		strcpy(w_buf.info, "开户成功");
		save_info(r_buf);
	}
	printf("开户服务准备第一次发送信息\n");
	if(msgsnd(s2cmsgid, &w_buf, sizeof(struct S2CMSG) - sizeof(long), 0) == -1){
		perror("msgsnd err");
		return 0;
	}
	printf("开户服务第一次发送信息完成\n");
	printf("账户金额:%f\n", w_buf.money);
	printf("开户成功\n");
	}

}
//创建银行卡号
int CREATID(){
	int init_id = 20230816;
	int fd = 0;
	if(((fd = open("id.txt", O_RDWR | O_CREAT, 0664)) == -1)){
		perror("1open err");
		return -1;
	}
	if((read(fd, &init_id, sizeof(init_id))) == -1){
		perror("read err");
		return -1;
	}
	init_id ++;
	lseek(fd, 0, 0);
	write(fd, &init_id, sizeof(init_id));
	close(fd);
	return init_id;
}

//保存信息

void save_info(struct C2SMSG r_buf){
	char filename[64] = {0};
	sprintf(filename, "%d.txt", r_buf.data.id);
	int fd = open(filename, O_WRONLY | O_CREAT, 0664);
	if(fd == -1){
		perror("open err");
		return;
	}
	write(fd, &r_buf, sizeof(r_buf));
	close(fd);
}
