#include "my_ls.h"


//创建链表节点
Dirlink* add_link(Dirlink* phead) {
    Dirlink* ptmp = NULL;
    ptmp = phead;
    while (ptmp->pnext) {        //寻找尾节点
        ptmp = ptmp->pnext;
    }
    if ((ptmp->pnext = (Dirlink*)malloc(sizeof(Dirlink)))){    //开辟空间
        
    }
    else {
        return NULL;
    }
    memset(ptmp->pnext, 0, sizeof(Dirlink));//初始化
	ptmp->pnext->plast = ptmp;
    return ptmp->pnext;
}

//获取目录项,i节点
int my_ls(Dirlink* phead, const char* dname){
	Dirlink* pnew = NULL;
//	Dirlink* ptmp = phead;
	int i = 0;
	struct dirent * rp = NULL;
	struct stat stap ={0}; 
	DIR* dp = opendir(dname);
	struct tm *timep = 0;
	char a_path[128] = {0};

	mode_t perms[] = {
						  S_IRUSR, S_IWUSR, S_IXUSR,
						  S_IRGRP, S_IWGRP, S_IXGRP, 
						  S_IROTH, S_IWOTH, S_IXOTH
					};
	if(dp == NULL){
		perror("opendir");
		return -1;
	}
	getcwd(a_path, sizeof(a_path));//获得当前的绝对路径
	chdir(dname);//切换路径
	while((rp = readdir(dp)) != NULL){	//获取目录项名，i节点
		
		pnew = add_link(phead);
		pnew->s.ino = rp->d_ino;
		strcpy(pnew->s.name, rp->d_name);
		if(lstat(rp->d_name, &stap) == -1){
			perror("stat");
			return -1;
		}
			
			switch(stap.st_mode & S_IFMT){	//文件类型
				case S_IFBLK: pnew->s.type = 'b'; break;
				case S_IFCHR: pnew->s.type = 'c'; break;
				case S_IFDIR: pnew->s.type = 'd'; break;
				case S_IFIFO: pnew->s.type = 'p'; break;
				case S_IFLNK: pnew->s.type = 'l'; break;
				case S_IFREG: pnew->s.type = '-'; break;
				case S_IFSOCK: pnew->s.type = 's'; break;
		
		}
		strcpy(pnew->s.per, "rwxrwxrwx");
		i = 0;
		while(i < 9){//文件权限
			if ((stap.st_mode & perms[i]) == 0){
				pnew->s.per[i] = '-';
			}
			i++;
		}
		//strcpy(pnew->s.ctime, ctime(&stap.st_ctime));
		timep = localtime(&stap.st_ctime);
		strftime(pnew->s.ctime, sizeof(pnew->s.ctime), "%m月\t%d %H:%m ", timep);
	
	}

	

	
	if(closedir(dp) == -1){
		perror("closedir");
		return -1;
	}
	chdir(a_path);//切换为原路径
	
	return 0;
}

//判断目录项是否为MP3文件 
void find_mp3(Dirlink* phead, Dirlink* mphead){
	char* fname = NULL;
	Dirlink * ptmp = phead;
	Dirlink * pnew = NULL;

	while(ptmp->pnext){
		fname = ptmp->pnext->s.name + strlen(ptmp->pnext->s.name) - 4;
		if(strcmp(fname, ".mp3") == 0){
			pnew = add_link(mphead);
			strcpy(pnew->s.name, ptmp->pnext->s.name);
			pnew = ptmp->pnext;	

		}
		ptmp = ptmp->pnext;
	}
}

//选择播放

void cut_songs(Dirlink* mphead){
	char flag = 0;
	Dirlink* ptmp = mphead;
	Dirlink* plast = NULL;
	Dirlink* pm = mphead->pnext;
	while(ptmp->pnext){
		ptmp = ptmp->pnext;
	}
	plast = ptmp;
	ptmp = mphead;
	while(1){
		printf("%s\n", ptmp->s.name);
		printf("W 上一曲  S 下一曲  E 歌单  Q 退出\n");
		scanf("%c", &flag);
		clear();
		if(flag != 'w' && flag != 'W' && flag != 'S' && flag != 's' && flag != 'e' && flag != 'E' && flag != 'Q' && flag != 'q'){
			clears();
			printf("非法输入，请重新输入\n");
			continue;			
		}
		else{
			switch (flag){
			
				case 'w': 
				
				case 'W':	 
					 if(ptmp->pnext){
						clears();
						ptmp = ptmp->pnext;
						break;
						}
					 else{
						 clears();
						ptmp = mphead->pnext;
						}
					 break;
				case 's':
				case 'S':
					  if(ptmp->plast != mphead){
						clears();
						ptmp = ptmp->plast;
						break;
					  }
					  else{
						clears();
						ptmp = plast;
					  }
						break;
				case 'e':
				case 'E':
					  clears();
					  pm = mphead->pnext;
					  while(pm){
						printf("%s\n", pm->s.name);
						pm = pm->pnext;
					  }
					  break;
				case 'Q':
				case 'q':
					  clears();
					  printf("已退出播放器\n");
					  return;
			}
			

		}
	}
}

//清理缓冲区
void clear(){
	while(getchar() != '\n');
}

//清屏
void clears() {
    printf("\033[2J\033[1;1H");
}
