#include <common.h>

//函数声明
void clears();
void clear();
enum opt menu();
void CREATACCOUNT();
void dep();
void disdep();
void check();
void trans();
void delac();
//全局变量
int s2cmsgid, c2smsgid;
struct C2SMSG W_buf;
struct S2CMSG R_buf;
glob_t file = {0};
int len = 0;

int main(){
	int num = 0;

	printf("客户端启动\n");
	//访问消息队列
	s2cmsgid = msgget(S2CKEY, IPC_CREAT | 0644);
	if(s2cmsgid == -1){
		perror("msgget err");
		return -1;
	}
	c2smsgid = msgget(C2SKEY, IPC_CREAT | 0644);
	if(c2smsgid == -1){
		perror("msgget err");
	}
	clears();
	while(1){
			num = menu();
		switch(num){
			case OPEN: CREATACCOUNT();
				break;
			case DEP:  dep();
				break;
			case DISDEP: disdep();
				break;
			case CHECK: check();
				break;
			case TRANS: trans();
				break;
			case DELAC:	delac();
				break;

			case QUIT: return 0;
				break;

		}	
	}

}

//菜单
enum opt menu(){
	enum opt num;
	glob("/home/yyy/linux/银行项目/src/*.txt", 0, NULL, &file);	//获取所有账号文件
	len = strlen("/home/yyy/linux/银行项目/src/");
//	printf("%d\n", len);

	while(1){
		printf("%d.开户\n", OPEN);
		printf("%d.存款\n", DEP);
		printf("%d.取款\n", DISDEP);
		printf("%d.查询\n", CHECK);
		printf("%d.转账\n", TRANS);
		printf("%d.销户\n", DELAC);
		printf("%d.退出\n", QUIT);
		printf("请选择功能\n");
		scanf("%d",(int *)&num);
		clear();
		if(num > QUIT || num < OPEN){
			clears();
			printf("非法输入，请重新输入\n");
		}
		else{
			clears();
			return num;
		}
	}

}


//开户
void CREATACCOUNT(void){
	char ch = 0;
	char pws1[32] = {0}, pws2[32] = {0};
	printf("请输入姓名\n");
	scanf("%s", W_buf.data.name);
	clear();
	clears();
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
			strcpy(W_buf.data.passwd, pws1);
			printf("密码创建成功\n");
			break;
			}
		else{
			clears();
			printf("密码输入不一致，请重新输入\n");
		}
	}
	printf("请输入金额\n");
	scanf("%f",&W_buf.data.money);
	clear();
	clears();
	W_buf.data.pid = getpid();
	W_buf.type = OPEN;
	if(msgsnd(c2smsgid, &W_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&R_buf, sizeof(struct S2CMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	} 
	if(!strcmp(R_buf.info, "开户失败")){
		printf("%s\n", R_buf.info);
	}
	else{
		printf("%s\n", R_buf.info);
		printf("账号 ： %d\n", R_buf.id);
		printf("户主 ： %s\n", W_buf.data.name);
		printf("余额 ： %f\n", R_buf.money);
		printf("按任意键继续\n");
		while(1){
			if(scanf("%c", &ch)){
				clear();
				clears();
				break;
		}
		}
	}
}

//存款
void dep(){
	int id = 0;
	char passwd[20] = {0};
	char filename[64] = {0};
	char ch = 0;
	//{}int fd = 0;
	int num = 0;
	float money = 0;
	struct C2SMSG  r_buf;
	struct S2CMSG w_buf;
	
	printf("请输入银行卡号\n");
	scanf("%d", &id);
	clear();
	sprintf(filename, "%d.txt", id);
	//查找该账号是否存在
	for(num = 0; num < file.gl_pathc; num ++){
		if(! strcmp(file.gl_pathv[num] + len, filename)){
			break;
		}
	}
	if(num == file.gl_pathc){
		printf("银行卡号不存在，已退出\n");
		printf("按任意键继续\n");
				while(1){
			if(scanf("%c", &ch)){
				clear();
				clears();
				break;
			}
		}
		return ;
	}
//	strcpy(r_buf.data.name, file.gl_pathv[num]);
	r_buf.data.id = id;
	r_buf.data.pid = getpid();
	r_buf.type = DEP;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&r_buf, sizeof(struct C2SMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
//	read(fd,&r_buf, sizeof(struct C2SMSG));
//	if(r_buf.data.id == id){
		num = 0;
		while(1){
			printf("请输入密码\n");
			scanf("%s", passwd);
			clear();
			if(! (id = strcmp(r_buf.data.passwd, passwd))){
				clears();
				printf("密码正确\n");
				break;
			}
			else{
				clears();
				printf("密码或账号错误\n");
				num ++;
			}
			if(num == 3){
				clears();
				printf("密码错误3次，已退出\n");
				printf("按任意键继续\n");
			W_buf.type = DEP;
			W_buf.data.id = -1;
			if(msgsnd(c2smsgid, &W_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}
						while(1){
			if(scanf("%c", &ch)){
				clear();
				clears();
				break;
		}
		}
				return;
			}
		}
		num = 0;
		while(1){
			printf("请输入存款金额\n");
			scanf("%f", &money);
			clear();
			if(money == 0 && num < 4){
				clears();
				printf("存款金额错误，请重新输入\n");
				num ++;
			}
			else{
				clears();
				r_buf.data.money = money + r_buf.data.money;
			//	printf("%f\n", r_buf.data.money);
				break;
			}
			if(num == 3){
				clears();
				printf("金额错误3次，已退出\n");
				printf("按任意键继续\n");
				W_buf.data.id = -1;
				W_buf.type = DEP;
	if(msgsnd(c2smsgid, &W_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
			while(1){
			if(scanf("%c", &ch)){
				clear();
				clears();
				break;
		}
		}
				return;
			}

		}
//	clear();
//	clears();
	r_buf.data.pid = getpid();
	r_buf.type = DEP;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&w_buf, sizeof(struct S2CMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	} 
	if(!strcmp(w_buf.info, "存款失败")){
		printf("%s\n", w_buf.info);
	}
	else{
		printf("%s\n", w_buf.info);
		printf("账号 ： %d\n", w_buf.id);
		printf("户主 ： %s\n", r_buf.data.name);
		printf("余额 ： %f\n", w_buf.money);
	//	while(1){
	//		if(getchar()){
	//			clears();
	//			break;
	//		}
	//	}
	}
		printf("按任意键继续\n");
			while(1){
			if(scanf("%c", &ch)){
				clear();
				clears();
				break;
		}
		}

//	}
		
}

//取款
void disdep(){
	int id = 0;
	char passwd[20] = {0};
	char filename[64] = {0};
	char ch = 0;
	//int fd = 0;
	int num = 0;
	float money = 0;
	struct C2SMSG  r_buf;
	struct S2CMSG w_buf;
	printf("请输入银行卡号\n");
	scanf("%d", &id);
	clear();
	sprintf(filename, "%d.txt", id);
	//查找该账号是否存在
	for(num = 0; num < file.gl_pathc; num ++){
		if(! strcmp(file.gl_pathv[num] + len, filename)){
			break;
		}
	}
	if(num == file.gl_pathc){
		printf("银行卡号不存在，已退出\n");
		printf("按任意键继续\n");
				while(1){
			if(scanf("%c", &ch)){
				clear();
				clears();
				break;
			}
		}
		return ;
	}
//	strcpy(r_buf.data.name, file.gl_pathv[num]);
	r_buf.data.id = id;
	r_buf.data.pid = getpid();
	r_buf.type = DISDEP;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&r_buf, sizeof(struct C2SMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
//	if(r_buf.data.id == id){
		clears();
		num =0;
		while(1){
			printf("请输入密码\n");
			scanf("%s", passwd);
			clear();
			if(! (id = strcmp(r_buf.data.passwd, passwd))){
				clears();
				printf("密码正确\n");
				break;
			}
			else{
				clears();
				printf("密码或账号错误\n");
				num ++;
			}
			if(num == 3){
				clears();
				printf("密码错误3次，已退出\n");
				printf("按任意键继续\n");
			r_buf.type = DISDEP;
			r_buf.data.id = -1;
			if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}
				while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
				return;
			}
		}
		num = 0;
		while(1){
			printf("卡号：%d\n", r_buf.data.id);
			printf("余额：%f\n", r_buf.data.money);
			printf("请输入取款金额\n");
			scanf("%f", &money);
			clear();
			if((money == 0 && num < 4) || (money < 0 && num < 4) || (money > r_buf.data.money && num < 4)){
				clears();
				printf("取款金额错误，请重新输入\n");
				num ++;
			}
			else{
				clears();
				r_buf.data.money = r_buf.data.money - money;
			//	printf("%f\n", r_buf.data.money);
				break;

			
			}
			if(num == 3){
				clears();
				printf("金额错误3次，已退出\n");
			W_buf.type = DISDEP;
			W_buf.data.id = -1;
			if(msgsnd(c2smsgid, &W_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}
			printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
				return;
						}
		}
//	clear();
//	clears();
	r_buf.data.pid = getpid();
	r_buf.type = DISDEP;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&w_buf, sizeof(struct S2CMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
	if(!strcmp(w_buf.info, "取款失败")){
		printf("%s\n", w_buf.info);
	}
	else{
		printf("%s\n", w_buf.info);
		printf("账号 ： %d\n", w_buf.id);
		printf("户主 ： %s\n", r_buf.data.name);
		printf("余额 ： %f\n", w_buf.money);
		printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
					return;
	}


//	}

}


//查询
//存款
void check(){
	int id = 0;
	char passwd[20] = {0};
	char filename[64] = {0};
	char ch = 0;
	//int fd = 0;
	int num = 0;
	//float money = 0;
	struct C2SMSG  r_buf;
	struct S2CMSG w_buf;
	//复制
	//查找该账号是否存在
	printf("请输入银行卡号\n");
	scanf("%d", &id);
	clear();
	sprintf(filename, "%d.txt", id);
	//查找该账号是否存在
	for(num = 0; num < file.gl_pathc; num ++){
		if(! strcmp(file.gl_pathv[num] + len, filename)){
			break;
		}
	}
	if(num == file.gl_pathc){
		printf("银行卡号不存在，已退出\n");
		printf("按任意键退出\n");
		while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}

		return ;
	}
//	strcpy(r_buf.data.name, file.gl_pathv[num]);
	r_buf.data.id = id;
	r_buf.data.pid = getpid();
	r_buf.type = CHECK;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&r_buf, sizeof(struct C2SMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
	//	if(r_buf.data.id == id){
		num = 0;
		clears();
		while(1){
			printf("请输入密码\n");
			scanf("%s", passwd);
			clear();
			if(! (id = strcmp(r_buf.data.passwd, passwd))){
				clears();
				printf("密码正确\n");
				break;
			}
			else{
				clears();
				printf("密码或账号错误\n");
				num ++;
			}
			if(num == 3){
				clears();
			W_buf.type = CHECK;
			W_buf.data.id = -1;
			if(msgsnd(c2smsgid, &W_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}

				printf("密码错误3次，已退出\n");
				printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
				return;
			}
		}

//	clear();
//	clears();
	r_buf.data.pid = getpid();
	r_buf.type = CHECK;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	clears();
	if(msgrcv(s2cmsgid,&w_buf, sizeof(struct S2CMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
	if(!strcmp(w_buf.info, "查询失败")){
		printf("%s\n", w_buf.info);
	}
	else{
		printf("%s\n", w_buf.info);
		printf("账号 ： %d\n", w_buf.id);
		printf("户主 ： %s\n", r_buf.data.name);
		printf("余额 ： %f\n", w_buf.money);
		printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
	}


//	}

}


//转账
//
void trans(){
	int id = 0;
	int i = 0;
	char passwd[20] = {0};
	char filename[64] = {0};
	char ch = 0;
	//int fd = 0;
	int num = 0;
	float money = 0;
	struct C2SMSG  r_buf;
	struct S2CMSG w_buf;
	printf("请输入银行卡号\n");
	scanf("%d", &id);
	clear();
	sprintf(filename, "%d.txt", id);
	//查找该账号是否存在
	for(num = 0; num < file.gl_pathc; num ++){
		if(! strcmp(file.gl_pathv[num] + len, filename)){
			break;
		}
	}
	if(num == file.gl_pathc){
		clears();
		printf("银行卡号不存在，已退出\n");
		return ;
	}
	r_buf.data.id = id;
	r_buf.data.pid = getpid();
	r_buf.type = TRANS;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&r_buf, sizeof(struct C2SMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
	num = 0;
	clears();
		while(1){
			printf("请输入密码\n");
			scanf("%s", passwd);
			clear();
			if(! (id = strcmp(r_buf.data.passwd, passwd))){
				clears();
				printf("密码正确\n");
				break;
			}
			else{
				clears();
				printf("密码或账号错误\n");
				num ++;
			}
			if(num == 3){
				clears();
				printf("密码错误3次，已退出\n");
			r_buf.type = TRANS;
			r_buf.data.id = -1;
			if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}
			printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
				return;
			}
		}
		num = 0;
		i = 0;
		while(1){
			printf("请输入收款银行卡号\n");
			scanf("%d", &id);
			clear();
			sprintf(filename, "%d.txt", id);
			//查找该账号是否存在
			for(num = 0; num < file.gl_pathc; num ++){
				if(! strcmp(file.gl_pathv[num] + len, filename)){
					break;
				}
			}
		if(num == file.gl_pathc){
			clears();
			printf("银行卡号不存在\n");
			i++;
			}
		else{
			clears();
			r_buf.data.o_id = id;
			break;
		}
		if(i == 3){
			clears();
				printf("银行卡号输入错误3次，已退出\n");
				printf("按任意键继续\n");
			r_buf.type = TRANS;
			r_buf.data.id = -1;
			if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
				return;
			}
		}

		num = 0;
		while(1){
			printf("卡号：%d\n", r_buf.data.id);
			printf("余额：%f\n", r_buf.data.money);
			printf("收款方卡号：%d\n", r_buf.data.o_id);
			printf("请输入转账金额\n");
			scanf("%f", &money);
			clear();
			if((money == 0 && num < 4) || (money < 0 && num < 4) || (money > r_buf.data.money && num < 4)){
				clears();
				printf("转账金额错误，请重新输入\n");
				num ++;
			}
			else{
				clears();
				r_buf.data.money = r_buf.data.money - money;
			//	printf("%f\n", r_buf.data.money);
				break;

							}
			if(num == 3){
				clears();
				printf("金额错误3次，已退出\n");
				printf("按任意键继续\n");
			r_buf.type = TRANS;
			r_buf.data.id = -1;
			if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
				return;

						}
		}
//	clear();
//	clears();
	r_buf.data.pid = getpid();
	r_buf.type = TRANS;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&w_buf, sizeof(struct S2CMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
	if(!strcmp(w_buf.info, "取款失败")){
		printf("%s\n", w_buf.info);
	}
	else{
		printf("%s\n", w_buf.info);
		printf("账号 ： %d\n", w_buf.id);
		printf("户主 ： %s\n", r_buf.data.name);
		printf("余额 ： %f\n", w_buf.money);
		printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
	}


//	}

}


//销户
void delac(){
	int id = 0;
	char passwd[20] = {0};
	char filename[64] = {0};
	//int fd = 0;
	int num = 0;
	char ch = 0;
	//float money = 0;
	struct C2SMSG  r_buf;
	struct S2CMSG w_buf;
	//复制
	printf("请输入请输入需要注销账户的银行卡号\n");
	scanf("%d", &id);
	clear();
	sprintf(filename, "%d.txt", id);
	//查找该账号是否存在
	for(num = 0; num < file.gl_pathc; num ++){
		if(! strcmp(file.gl_pathv[num] + len, filename)){
			break;
		}
	}
	if(num == file.gl_pathc){
		printf("银行卡号不存在，已退出\n");
		printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
		return ;
	}
//	strcpy(r_buf.data.name, file.gl_pathv[num]);
	r_buf.data.id = id;
	r_buf.data.pid = getpid();
	r_buf.type = DELAC;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&r_buf, sizeof(struct C2SMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
	num = 0;
	while(1){
			printf("请输入密码\n");
			scanf("%s", passwd);
			clear();
			if(! (id = strcmp(r_buf.data.passwd, passwd))){
				clears();
			//	printf("密码正确\n");
				break;
			}
			else{
				clears();
				printf("密码或账号错误\n");
				num ++;
			}
			if(num == 3){
				printf("密码错误3次，已退出\n");
				printf("按任意键继续\n");
			r_buf.type = DELAC;
			r_buf.data.id = -1;
			if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
				return;
			}
		}
		printf("即将销户的账号信息\n");
		printf("账号 ： %d\n", r_buf.data.id);
		printf("户主 ： %s\n", r_buf.data.name);
		printf("余额 ： %f\n", r_buf.data.money);
		if(r_buf.data.money){
			printf("账号余额不为0， 请先将存款取出\n");
				printf("销户已退出\n");
				printf("按任意键继续\n");
			r_buf.type = DELAC;
			r_buf.data.id = -1;
			if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
					perror("msgsnd err");
					return;
			}
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
			return;
		}
		printf("输入 y 确认注销账户\n");
		scanf("%c", &ch);
		clear();
		if(ch != 'y' && ch != 'Y'){
			r_buf.data.id = -1;
			r_buf.type = DELAC;
			if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
			printf("已取消注销账户\n");
			printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}
			return;
		}
		

//	clear();
//	clears();
	r_buf.data.pid = getpid();
	r_buf.type = DELAC;
	if(msgsnd(c2smsgid, &r_buf, sizeof(struct C2SMSG) - sizeof(long), 0) == -1){
			perror("msgsnd err");
			return;
			}
	if(msgrcv(s2cmsgid,&w_buf, sizeof(struct S2CMSG) - sizeof(long), getpid(), 0) == -1){
		perror("msgrcv err");
		return;
	}
	clears();
	printf("银行卡号 ： %d\n", w_buf.id);
	printf("账户状态 ： %s\n", w_buf.info);
	printf("按任意键继续\n");
			while(1){
					if(scanf("%c", &ch)){
					clear();
					clears();
					break;
						}
		}

}


//清理缓存区
void clear(){
	while(getchar() != '\n');
}

//清屏
void clears() {
    printf("\033[2J\033[1;1H");
}
