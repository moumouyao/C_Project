#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
	glob_t fmp3 = {0};
	int i = -1;
	char ptmp[256] = {0};
	char flag = 0;
	pid_t pid = 0; 
	int n = 0;
void play(char** pathv, int pathc);
void clears();

void clear();
void Clean_Fun(void)
{
    int a = waitpid(getpid(),NULL,WNOHANG);
    if(a == 0)
    {
        kill(pid,9);
    }
    system("stty echo");
    system("echo -e \"\033[?25h\"");
}
void fun(){
	if(flag == 'e' || flag == 'E')
		return;
	int a;
	waitpid(pid, &a, WNOHANG);
	printf("123%ld\n", fmp3.gl_pathc);	

		pid = fork();
	if(WIFEXITED(a) && pid == 0){
	//if(i < (fmp3.gl_pathc - 1)){
	if(i < 8){
		printf("正常结束\n");
						clears();
						strcpy(ptmp,fmp3.gl_pathv[i + 1]);
						i++;
						 printf("i = %d\n", i);

		 execlp("mpg123","mpg123", ptmp, NULL);
						}
					 else{
						 printf("正常结束2   %ld\n", fmp3.gl_pathc);
						 printf("i = %d\n", i);
						 clears();
						strcpy(ptmp,fmp3.gl_pathv[0]);
						
						i = 0;
		 execlp("mpg123","mpg123", ptmp, NULL);
						}

	}
	else{
		if(flag != 'e' && flag != 'E' && flag != 'q' && flag != 'Q' && flag != 'r' && flag != 'R'){
			
		if(pid == 0){
			printf("%ld\n", i);
		printf("非正常结束\n");	
		 execlp("mpg123","mpg123", ptmp, NULL);
		}
		}
	}

}
int main(){
	signal(17, fun);
//	atexit(Clean_Fun);

	glob("/home/yyy/mp3file/*.mp3", 0, NULL, &fmp3);
	if(fmp3.gl_pathc == 0){
		printf("没有可以播放的文件\n");
		return 0;
	
	}
	pid = fork();
	if(pid == 0){
	//	strcpy(ptmp,fmp3.gl_pathv[0]);	
		exit(1);
		// execlp("mpg123","mpg123", ptmp, NULL);
	}
	while(1){
			printf("%s\n", ptmp);
		printf("W 上一曲  S 下一曲  E 歌单  R 暂停  Q 退出\n");
		//clears();
		while(1){
		scanf("%c", &flag);
		clear();
		
		
		if(flag != 'w' && flag != 'W' && flag != 'S' && flag != 's' && flag != 'e' && flag != 'E' && flag != 'Q' && flag != 'q' && flag != 'r' && flag != 'R'){
			clears();
			printf("非法输入，请重新输入\n");
			continue;			
		}
		else{
		break;
		}
	}
	//	else{
			switch (flag){
			
				case 'w': 
				
				case 'W':	 
					 if(i < fmp3.gl_pathc - 1){
						clears();
						i++;
						strcpy(ptmp,fmp3.gl_pathv[i + 1]);
						kill(pid, 9);
						break;
						}
					 else{
						 clears();
						strcpy(ptmp,fmp3.gl_pathv[0]);
						
						i = 0;
						kill(pid, 9);
						}
					 break;
				case 's':
				case 'S':
					  if(i > 0){
						clears();
						strcpy(ptmp,fmp3.gl_pathv[i - 1]);
						i--;
						
						kill(pid, 9);
						break;
					  }
					  else{
						clears();
						strcpy(ptmp,fmp3.gl_pathv[fmp3.gl_pathc - 1]);
						
						i = fmp3.gl_pathc - 1;
						kill(pid, 9);
					  }
						break;
				case 'e':
				case 'E':
					  clears();
						for(int j = 0; j < fmp3.gl_pathc; j++){
	
						printf("%s\n", fmp3.gl_pathv[j]);
							}
					  break;
				case 'Q':
				case 'q':
					  clears();
					  signal(17, SIG_IGN);
						kill(0, 9);
					  printf("已退出播放器\n");
					  return 0;			
				case 'R':
				case 'r':
					  clears();
					  n = !n;
					  if(n){
						kill(pid, 19);
					  printf("已暂停\n");
					  }
					  else{
						kill(pid, 18);
					  }
			}
			

	//	}
	}
//	play(fmp3.gl_pathv, fmp3.gl_pathc);
	return 0;
}

//顺序播放
void play(char** pathv, int pathc){
	int i = -1;


	while(1){
		if(vfork() == 0){
			if(i == pathc - 1){
				i = -1;
			}
				i++;

    execlp("mpg123","mpg123",pathv[i],NULL);
	

		}
	wait(NULL);
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
