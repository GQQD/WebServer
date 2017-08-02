/**************************************
*文件说明:math_cgi.c
*作者:高小调
*创建时间:2017年08月01日 星期二 16时11分04秒
*开发环境:Kali Linux/g++ v6.3.0
****************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void main_handler(){
	char buff[4096] = {0};
	ssize_t s = read(0,buff,sizeof(buff));
	if(s < 0){
		perror("read");
		return ;
	}else if(s == 0){
		printf("no data\n");
		return ;
	}
	char *start = buff;
	int index = 0;
	int count = 0;
	char *argv[3] = {0};
	//只获取前两个参数
	while(*start && count <2){
		if(*start == '='){
			argv[index++] = start+1;
			count++;
		}
		if(*start == '&'){
			*start = '\0';
		}
		++start;
	}
	int l = atoi(argv[0]);
	int r = atoi(argv[1]);
	printf("%d + %d = %d\n",l,r,l+r);
	printf("%d - %d = %d\n",l,r,l-r);
	printf("%d * %d = %d\n",l,r,l*r);
	printf("%d / %d = %d\n",l,r,r!=0?l/r:0);
	printf("%d %% %d = %d\n",l,r,r!=0?l%r:0);
}
int main(){
	main_handler();
	return 0;
}
