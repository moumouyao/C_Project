#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
void play(char** pathv, int pathc);
void clears();

void clear();
int main(){

	glob_t fmp3 = {0};
	int i = 0;
	char ptmp[256] = {0};
	char flag = 0;
	pid_t pid = 0; 
	glob("/home/yyy/mp3file/*.mp3", 0, NULL, &fmp3);
	if(fmp3.gl_pathc == 0){
		printf("没有可以播放的文件\n");
		return 0;
	
	}
	while(1){
		if(flag != 'e' && flag != 'E' && flag != 'q' && flag != 'Q' ){
		while(1){
		pid = fork();
		if(pid == 0){	
			strcpy(ptmp, fmp3.gl_pathv[i]);
			i++;
			if(i >= fmp3.gl_pathc){
				strcpy(ptmp, fmp3.gl_pathv[0]);
				i == 0;
			}
		 execlp("mpg123","mpg123", ptmp, NULL);
		}
		break;
		}
		}
		printf("%s\n", ptmp);
		printf("W 上一曲  S 下一曲  E 歌单  Q 退出\n");
		//clears();
		while(1){
		scanf("%c", &flag);
		clear();
		
		
		if(flag != 'w' && flag != 'W' && flag != 'S' && flag != 's' && flag != 'e' && flag != 'E' && flag != 'Q' && flag != 'q'){
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
						strcpy(ptmp,fmp3.gl_pathv[i + 1]);
						i++;
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
						
						kill(pid, 9);
						i--;
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
						kill(pid, 9);
					  printf("已退出播放器\n");
					  return 0;
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
