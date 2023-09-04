#include <common.h>

//函数声明
void clear();
void clears();
void syserr(const char *str, int ret);
void create_account(void);
enum log_in menu1();
void contin();
int connect_ser();
void disconnect();
void login();
enum after_log_in menu2();
void chat(DATA tmp);
void *W_FUN(void *arg);
void chat_fun(DATA tmp);
enum account_opt menu3();
int printf_info(DATA data, char *str);
void change_name();
int opt_account();
int change_passwd();
int delete_account();
enum root_opt menu4();
void opt_root();
void kikc();
void chat_record();
void quit();
//全局变量
int sockfd = 0, flog = 1, s_id = 0;
DATA Data;
pthread_mutex_t mutex;
//char str[8] = {0};

int main(){
	int num = 0;
	int ret = 0;
	struct sockaddr_in s_addr;
	clears();
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		//syserr("socker", sockfd);
		perror("socket");
		return -1;
	}
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(NETPORT);
	s_addr.sin_addr.s_addr = inet_addr(S_ADDR);
	ret = connect(sockfd, (struct sockaddr *)&s_addr, sizeof(s_addr));
	if(ret == -1){
		perror("connect");
		//syserr("connect", ret);
		return -1;
	}
	while(1){
		num = menu1();
		switch(num){
			case LOG_IN: login();
						
				break;
			case REGISTER:	create_account();
							
				break;
			case QUITM:	printf("网络聊天室已退出\n");
			 return 0;
				break;
		}
	}
	disconnect();
	return 0;
}
//登陆之后
void chat(DATA tmp){
	int num = 0;
	while(1){
		num = menu2();
		switch(num){
			case GROUP_CHAT:
				flog = 1;			
				chat_fun(tmp);	
				break;
			case PRIVATE_CHAT:
				flog = 2;
				chat_fun(tmp);
				break;
			case ACCOUNT:
				if(opt_account() == 1){
					return;
				}
				break;
			case CHAT_RECORD:
				chat_record();
				break;

			case ROOT:
				opt_root();
				break;
			case QUITL:
				quit();
				return;
		}
	}

}
//退出登陆
void quit(){
	DATA data = Data;
	data.type = 11;
	send(sockfd, &data, sizeof(data), 0);
	printf("已退出登陆\n");
	contin();
}


void chat_record(){
	DATA data;
	strcpy(data.name, Data.name);
	data.id = Data.id;
	int num = 0, fd = 0;
	char priv[256] = {0};
	printf("1.查看群聊记录\n2.查看最近一次私聊记录\n请选择\n");
	scanf("%d", &num);
	clear();
	if(num == 0){
		printf("非法输入，已退出\n");
		contin();
		return;
	}
	if(num == 1){
		if(((fd = open("group.txt", O_RDONLY)) == -1)){
		perror("open");
		return;
		}
		while(num){
		if(((num = read(fd, &data, sizeof(data)))) == -1){
			perror("read");
			return;
		}
		printf("%s\n%s\n", data.w_buf, data.r_buf);
		}
	}
	else{
		sprintf(priv, "%d.txt", data.id);
		if(((fd = open(priv, O_RDONLY)) == -1)){
		perror("open");
		return;
		}
		while(num){
		if(((num = read(fd, &data, sizeof(data)))) == -1){
			perror("read");
			return;
		}
		printf("%s\n%s\n", data.w_buf, data.r_buf);
		}
	}

}

//管理员操作
void opt_root(){
	int num = 0;
	DATA data;
	data.adm = Data.adm;
	data.num = 1;
	data.id = Data.id;
	char *str[] = {"请输入需要解除限制的账号",
				   "请输入需要踢出群聊的账号",
				   "请输入需要禁止登陆的账号",
				   "请输入需要禁言的账号"
				   
				   
				};
	char *rcc[] = {"解除限制成功",	//6  0
				   "踢出群聊成功",	//0  1
				   "禁止登陆成功",	//2  2
				   "禁言成功",	   //4  3	   
				};
	data.type = 10;
	strcpy(data.name, Data.name);
	data.id = Data.id;
	//获取权限
	send(sockfd, &data, sizeof(data), 0);

	recv(sockfd, &data, sizeof(data), 0);

	if(data.adm != 1){
		printf("%s\n", data.feedback);
		contin();
		return;
	}
	printf("1.踢出群聊\n2.禁止登陆\n3.禁止发言\n4.解除限制\n请选择\n");
	scanf("%d", &num);
	clear();
	if(num == 0){
		printf("非法输入，已退出\n");
		contin();
		return;
	}
//	else if(num == 1){
		
		
		if(num == 4){
			data.num = 0;
			printf("%s\n", str[0]);
		}
		else{
			data.num = num + 1;
			printf("%s\n", str[num]);
		}
		
		scanf("%d", &data.s_id);
		clear();
		send(sockfd, &data, sizeof(data), 0);
		recv(sockfd, &data, sizeof(data), 0);
	//	printf("1111112222222111\n");
		if(num == 4){
	//		printf("111111111\n");
			printf_info(data, rcc[0]);
		}
		else{
		//	printf("1234444\n");
			printf_info(data, rcc[num]);
		}
	//	printf("22222222222\n");
		contin();
		
//	}
	
}

//账号管理
int opt_account(){
	int num = 0;
	while(1){
		num = menu3();
		switch(num){
			case NAME:
				change_name();
				break;
			case PASSWD:
				if(change_passwd() == 1){
					return 1;
				}
				break;
			case DELETE:
				if(delete_account() == 1){
					return 1;
				}
				break;
			case QUITA:
				return 2;
				break;
		}
	}
}

//注销账号
int delete_account(){
	DATA data;
	char ch = 0;
	//char passwd1[32] = {0}, passwd2[32] = {0};
	data.id = Data.id;
	data.type = 8;
	printf("请输入密码\n");
	scanf("%s", data.passwd);
	clear();
	data.num = 1;
	strcpy(data.name, Data.name);
	send(sockfd, &data, sizeof(data), 0);
	recv(sockfd, &data, sizeof(data), 0);
	if(printf_info(data, "密码正确") == 0){
		printf("已退出\n");
		contin();
		return 2;
	}else{
		printf("确认注销账户请输入 Y\n");
		scanf("%c", &ch);
		clear();
		if(ch == 'Y' || ch == 'y'){
			data.num = 2;
			strcpy(data.feedback, "确认销户");
			send(sockfd, &data, sizeof(data), 0);
			recv(sockfd, &data, sizeof(data), 0);
			printf_info(data, "注销成功");
			contin();
			return 1;
		}
		else{
			printf("已取消销户操作\n");
			contin();
			return 2;
		}
	}
	return 2;
}

//修改密码
int change_passwd(){
	DATA data;
	char passwd1[32] = {0}, passwd2[32] = {0};
	data.id = Data.id;
	data.type = 7;
	printf("请输入旧密码\n");
	scanf("%s", data.passwd);
	clear();
	data.num = 1;
	strcpy(data.name, Data.name);
	send(sockfd, &data, sizeof(data), 0);
	recv(sockfd, &data, sizeof(data), 0);
	if(printf_info(data, "密码正确") == 0){
		printf("已退出\n");
		contin();
		return -1;
	}else{
		printf("请输入新密码\n");
		scanf("%s", passwd1);
		clear();
		printf("请再次输入新密码\n");
		scanf("%s", passwd2);
		clear();
		if(!strcmp(passwd1, passwd2)){
			data.num = 2;
			clears();
			strcpy(data.passwd, passwd1);
			send(sockfd, &data, sizeof(data), 0);
			recv(sockfd, &data, sizeof(data), 0);
			printf_info(data, "密码修改成功");
			printf("请重新登陆\n");
			contin();
			return 1;
		}
		else{
			clears();
			printf("新密码不一致，已退出\n");
			contin();
			return -1;
		}
	}
}

//修改昵称
void change_name(){
	DATA data;
	data.id = Data.id;
	data.type = 6;
	printf("请输入新的昵称\n");
	scanf("%s", data.name);
	clear();
	send(sockfd, &data, sizeof(data), 0);
	recv(sockfd, &data, sizeof(data), 0);
	clears();
	printf_info(data, "昵称修改成功");
	contin();
}

//子线程调用函数，用于发送消息
void *W_FUN(void *arg){
	DATA data;
	long len = (long)arg;
	DATA* tmp = (DATA *)len;
	//int sockfd = (int)arg;
	data.type = flog;
	s_id = 0;
	//int num = 0;
	if(flog == 2){
		printf("请输入私聊对象的账号\n");
		scanf("%d", &data.s_id);
		clear();
		data.online = 1;
	}
	printf("请写入想要发送的内容\n");
	while(1){
		//pthread_mutex_lock(&mutex);    // 获取互斥锁
		if(s_id != 0){
			
			data.s_id = s_id;
			data.type = 2;
		}
	
	//pthread_mutex_unlock(&mutex);// 释放互斥锁
		scanf("%s", data.w_buf);
		clear();
	
	//printf("s_id = %d\n", s_id);
	//printf("send id = %d   recvid = %d\n",data.id,data.s_id);
	strcpy(data.name, tmp->name);
	data.id = tmp->id;
	data.num = 2;
	send(sockfd, &data, sizeof(data), 0);
	//printf("%d  发送信息为：%s\n",num,data.w_buf);
	//num++;
	if(!strcmp(data.w_buf, "quit")){
	//	strcpy(str, ata.w_buf);
		//printf("已退出，按任意键继续\n");
		//return NULL;
		memset(data.w_buf, 0, sizeof(data.w_buf));
		sleep(1);
	}
	printf("消息已送达\n");
	memset(data.w_buf, 0, sizeof(data.w_buf));
	}
	//contin();

	return NULL;
}
//聊天
void chat_fun(DATA tmp){
	
	DATA data = {0};
	time_t rawtime;
	//int sockfd = 0;
	pthread_t pid;
	data.type = flog;	//标志位
	strcpy(data.name, tmp.name);
	data.id = tmp.id;
	//获取权限
	//printf("第一次发送信息为：%s.....\n",data.w_buf);
	data.num = 1;	
	send(sockfd, &data, sizeof(data), 0);
	recv(sockfd, &data, sizeof(data), 0);
	//printf("第一次接受信息为：%s\n",data.w_buf);
	if(data.adm == 2){
		printf("%s\n", data.feedback);
		contin();
		return;
	}
	else if(flog == 2 && data.adm == 3){
		printf("%s\n", data.feedback);
		contin();
		return;
	}
	if(flog == 1){	
		//进入群聊
		data.online = 1;
		//打招呼
		data.num = 2;
		sprintf(data.w_buf, "\033[31m%s 进入群聊  ID : %d\033[0m\n", tmp.name, tmp.id);
		send(sockfd, &data, sizeof(data),0);
	}
	pthread_create(&pid, NULL, W_FUN, (void *)&tmp);
	
	while(1){
		memset(data.w_buf, 0, sizeof(data.w_buf));
		recv(sockfd, &data, sizeof(data), 0);
		//printf("循环接受信息为：%s\n",data.w_buf);
		if(!strcmp(data.w_buf, "quit")){
			pthread_cancel(pid);
			if(flog == 2){
				printf("\033[34m对方已退出私聊\033[0m\n");
				
			}
			contin();
	
			break;
		}
		if(flog == 2){
			pthread_mutex_lock(&mutex);// 获取互斥锁
			s_id = data.s_id;
			
			if(!strcmp(data.feedback, "创建私聊失败")){
				printf("对方不在群聊，或账号不存在\n\n");
				//s_id = -1;
				pthread_cancel(pid);
				contin();
				break;
			}
			pthread_mutex_unlock(&mutex);// 释放互斥锁

		}	
		time(&rawtime);
		//printf("\033[1A"); // 将光标向上移动一行
		printf("%s", ctime(&rawtime));
		if(data.type == 2){
			printf("\033[34m%s\033[0m\n", data.r_buf);
		}
		else{
			printf("\033[36m%s\033[0m\n", data.r_buf);
		}
		
		memset(&data, 0, sizeof(data));
		printf("请写入想要发送的内容\n");
	}
//	disconnect(sockfd);
}

//登录函数
void login(){
	//DATA Data;
	//int sockfd = 0;
	time_t rawtime;

	while(1){
		printf("请输入账号\n");
		scanf("%d", &Data.id);
		clear();
		printf("请输入密码\n");
		scanf("%s", Data.passwd);
		clear();
		Data.type = 4;	//设置标志位为登录
		//发送账号，检测是否存在
		//连接客户端
		//sockfd = connect_ser();
		if(sockfd == -1){
			printf("连接服务器失败\n");
			return;
		}
		send(sockfd, &Data, sizeof(Data), 0);
		recv(sockfd, &Data, sizeof(Data), 0);

		time(&rawtime);
		printf("%s", ctime(&rawtime));
		if(!strcmp(Data.feedback, "登录成功")){

			printf("%s\n", Data.feedback);
			printf("用户名：%s  账号%d\n", Data.name, Data.id);
			contin();
			chat(Data);
			break;

		}
		else if(!strcmp(Data.feedback, "您的账号被限制登陆，请联系管理员解锁")){
			printf("%s\n", Data.feedback);
			contin();
			break;
		}
		else if(!strcmp(Data.feedback, "您的账号已经登陆")){
			printf("%s\n", Data.feedback);
			printf("非本人操作请立即联系管理员\n");
			contin();
			break;
		}
		else{
			printf("%s\n", Data.feedback);
			contin();
			break;
		}
		//断开连接
	//	disconnect(sockfd);
	}	
}

//注册函数
void create_account(){
	DATA w_data, r_data;
	char pws1[32] = {0}, pws2[32] = {0};
//	int   sockfd = 0;
	//创建账户
	printf("请输入用户名\n");
	scanf("%s", w_data.name);
	clear();
	clears();
	printf("用户名设置成功\n");
	while(1){
		printf("请输入密码\n");
		scanf("%s", pws1);
		clear();
		clears();
		printf("请再次输入密码\n");
		scanf("%s", pws2);
		clear();
		
		if(!strcmp(pws1, pws2)){
			clears();
			strcpy(w_data.passwd, pws1);
			printf("密码创建成功\n");
			break;
			}
		else{
			clears();
			printf("密码输入不一致，请重新输入\n");
		}
	}
	w_data.type = 3;
	//发送给服务器
	//sockfd = connect_ser();
	if(sockfd == -1){
	printf("连接服务器失败\n");
	return;
	}
	send(sockfd, &w_data, sizeof(w_data), 0);
	recv(sockfd, &r_data, sizeof(r_data), 0);
	//断开连接
	//disconnect(sockfd);
	if(!strcmp(r_data.feedback, "账号创建成功")){
		printf("%s\n", r_data.feedback);
		printf("用户名：%s  账号%d\n", r_data.name, r_data.id);
		contin();
	}
	else{
		printf("%s\n", r_data.feedback);
		contin();
	}
}

//断开与服务器连接
void disconnect(){
	 shutdown(sockfd, SHUT_RDWR); // 断开连接
	 close(sockfd); // 关闭套接字,释放资源
}

//连接服务器
int connect_ser(){
	int ret = 0;
	struct sockaddr_in s_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		
		//syserr("socker", sockfd);
		perror("socket");
		return -1;
	}
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(NETPORT);
	s_addr.sin_addr.s_addr = inet_addr(S_ADDR);
	ret = connect(sockfd, (struct sockaddr *)&s_addr, sizeof(s_addr));
	if(ret == -1){
		perror("connect");
		//syserr("connect", ret);
		return -1;
	}
	return sockfd;

}

//菜单	登录界面菜单
enum log_in menu1(){
	enum log_in num;

	while(1){
		printf("%d.登录\n", LOG_IN);
		printf("%d.注册\n", REGISTER);
		printf("%d.退出\n", QUITM);
		printf("请选择功能\n");
		scanf("%d",(int *)&num);
		clear();
		if(num > QUITM || num < LOG_IN){
			clears();
			printf("非法输入，请重新输入\n");
		}
		else{
			clears();
			return num;
		}
	}

}

//菜单	root权限
enum root_opt menu4(){
	enum root_opt num;

	while(1){
		printf("%d.踢出或拉入群聊\n", KICK);
		printf("%d.接触或限制登陆\n", EXCULD);
		printf("%d.接触或禁言\n", DISSEND);
		printf("%d.退出\n", QUITR);
		printf("请选择功能\n");
		scanf("%d",(int *)&num);
		clear();
		if(num > QUITR || num < KICK){
			clears();
			printf("非法输入，请重新输入\n");
		}
		else{
			clears();
			return num;
		}
	}

}


//菜单	账号管理
enum account_opt menu3(){
	enum account_opt num;

	while(1){
		printf("%d.修改昵称\n", NAME);
		printf("%d.修改密码\n", PASSWD);
		printf("%d.注销账号\n", DELETE);
		printf("%d.退出\n", QUITA);
		printf("请选择功能\n");
		scanf("%d",(int *)&num);
		clear();
		if(num > QUITA || num < NAME){
			clears();
			printf("非法输入，请重新输入\n");
		}
		else{
			clears();
			return num;
		}
	}

}

//菜单	登录之后菜单
enum after_log_in menu2(){
	enum after_log_in num;

	while(1){
		printf("%d.群聊\n", GROUP_CHAT);
		printf("%d.私聊\n", PRIVATE_CHAT);
		printf("%d.账号管理\n", ACCOUNT);
		printf("%d.查看聊天记录\n", CHAT_RECORD);
		printf("%d.管理员权限\n", ROOT);
		printf("%d.退出登录\n", QUITL);
		printf("请选择功能\n");
		scanf("%d",(int *)&num);
		clear();
		if(num > QUITL || num < GROUP_CHAT){
			clears();
			printf("非法输入，请重新输入\n");
		}
		else{
			clears();
			return num;
		}
	}

}

//打印信息
int printf_info(DATA data, char *str){
	time_t rawtime;
	time(&rawtime);
	printf( "%s\t", ctime(&rawtime));
	if(!strcmp(data.feedback, str)){
		printf("%s  用户名：%s  账号：%d\n", data.feedback, data.name, data.id);
		return 1;
	}
	else{
		printf("%s  %s\n", data.feedback, data.name);
		return 0;
	}
}
//清理缓存区
void clear(){
	while(getchar() != '\n');
}

//清屏ff
void clears() {
    printf("\033[2J\033[1;1H");
}

//错误函数
void syserr(const char *str, int ret){
	printf("%s error: %s\n", str, strerror(ret));
	exit(1);
}

//卡点
void contin(){
	char ch = 0;
	printf("按任意键继续\n");
	while(1){
		if(scanf("%c", &ch)){
			clear();
			clears();
			break;
		}
		}
}
