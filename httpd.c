/**************************************
*文件说明:httpd.
*作者:高小调
*创建时间:2017年07月31日 星期一 19时16分40秒
*开发环境:Kali Linux/g++ v6.3.0
****************************************/
#include"httpd.h"
static const char* respond_line = "HTTP/1.0 200 OK\r\n";
static const char* blank_line = "\r\n";
void handler(int sig){
	//printf("get a sig %d\n",sig);
}

//检测客户端是否在线(处于连接状态)
int check_sock_connected(int sock){
	/*
	struct tcp_info info;
	socklen_t len = sizeof(info);
	getsockopt(sock,IPPROTO_TCP,TCP_INFO,&info,&len);
	if(info.tcpi_state == TCP_ESTABLISHED){
		return 1;
	}
	return 0;
	*/
	return 1;
}
//CGI处理
void exec_cgi(int sock,char *method,char *path){
	char buff[1024] = {0};
	char * query_string = NULL;
	int len = 0;
	int ret = 0;
	//获取参数
	if(strcasecmp(method,"GET") == 0){
		//当前为get请求,从path提取请求参数
		char *tmp = strchr(path,'?');
		if(tmp!=NULL){
			query_string = tmp+1;
		}
		do{
			ret = get_line(sock,buff,sizeof(buff));
			//printf("[GET]%s",buff);
		}while(ret!=1);
		*tmp = '\0';
	}else{
		//当前为post请求,从请求正文中提取请求参数
		do{
			ret = get_line(sock,buff,sizeof(buff));
			//printf("[POST]%s",buff);
			if(strncasecmp(buff,"Content-Length: ",16) == 0){
				char *space = strchr(buff,' ');
				if(space == NULL){
					//printf("space is NULL\n");
					return ;
				}
				len = atoi(space);
			}
		}while(ret!=1&&ret!=0);
		//printf("i get the len:%d\n",len);
		//读post的请求参数
		ret = recv(sock,buff,len,0);
		buff[len] = '\0';
		if(ret == 0){
			//printf("[POST]the data is over\n");
			return ;
		}else if(ret < 0){
			perror("[POST]get data fail");
			return ;
		}
		query_string = buff;
	}
	//printf("query_string:%s\n",query_string);
	int input[2];
	int output[2];
	if(pipe(input) < 0){
		perror("pipe");
		return ;
	}
	if(pipe(output) < 0){
		perror("pipe");
		return ;
	}
	pid_t pid = fork();
	//printf("child:running path is %s\n",path);
	if(pid == 0){		//child
		close(input[1]);
		close(output[0]);
		dup2(input[0],0);
		dup2(output[1],1);
		execl(path,path,NULL);
		//printf("child:error");
	}else if(pid > 0){	//father
		close(input[0]);
		close(output[1]);
		char buff[4096] = {0};
		//将请求信息写入管道
		write(input[1],query_string,strlen(query_string));
		//printf("father:%s is written in pipe\n",query_string);
		//从管道中读出信息发送给client	
		send(sock,respond_line,strlen(respond_line),0);
		send(sock,blank_line,strlen(blank_line),0);
		ssize_t s = 0;
		do{
			s = read(output[0],buff,sizeof(buff));
			buff[s] = 0;
			send(sock,buff,s,0);
			//printf("buff is %s",buff);
		}while(s != 0);
		wait(NULL);
		//printf("wait over\n");
		close(input[1]);
		close(output[0]);
		//printf("father is over\n");
	}
}
//处理简单的get请求
void handle_simple_get(int sock,char *path,int size){
	//读取多余请求
	int ret = 1;
	char buff[1024];
	do{
		ret = get_line(sock,buff,sizeof(buff));
		//printf("ret = %d,I get some data:%s",ret,buff);
	}while(ret!=1&&ret!=0);
	int state= check_sock_connected(sock);		
	if(state && send(sock,respond_line,strlen(respond_line),0) < 0){
		perror("send respond_line fail:");
		return ;	
	}
	state = check_sock_connected(sock);
	if(state && send(sock,blank_line,strlen(blank_line),0) < 0){
		perror("blank_line fail:");
		return ;
	}
	//打开请求文件
	int fd = open(path,O_RDONLY);
	if(fd < 0){
		perror("handle_simple_get:open");
		goto end;
	}
	state = check_sock_connected(sock);
	//发送数据
	if(state && sendfile(sock,fd,NULL,size) < 0){
		perror("handle_simple_get:sendfile");
	}
	//printf("数据发送完毕,并关闭fd\n");
end:
	close(fd);
}

//获取一个监听套接字
int get_listen_sock(int port){
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		perror("get_listen_sock:socket");
		exit(-1);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = 0;
	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
		perror("get_listen_sock:bind");
		exit(-2);
	}
	if(listen(sock,10) < 0){
		perror("get_listen_sock:listen");
		exit(-3);
	}
	return sock;
}

//从http请求中获取一行数据
//成功大于0,连接终止返回-1
int get_line(int sock,char *buff,int size){
	char ch = '\0';
	int index = 0;
	while(index<size &&ch!='\n'){
		if(recv(sock,&ch,1,0) == 0){
			//客户端断开连接
			return -1;
		}
		if(ch == '\r'){
			if(recv(sock,&ch,1,MSG_PEEK) == 0){
				//客户端断开连接
				return -1;
			}
			if(ch == '\n'){
				if(recv(sock,&ch,1,0) == 0){
					//客户端断开链接
					return -1;
				}
			}else{
				ch = '\n';
			}
		}
		buff[index++] = ch;
	}
	buff[index] = 0;
	return index;
}
//处理http请求
void handle_http_request(int sock){
	char buff[1024];
	int error_code = 500;
	//拿到http请求头信息 GET / HTTP/1.0
	if(get_line(sock,buff,sizeof(buff)) == -1){
		//printf("没有请求任何信息\n");
		goto end;
	}
	char *method = buff;
	char *tmp = buff;
	char *path = NULL;
	//提取method GET 或 POST
	while(*tmp!=' '){
		++tmp;
	}
	*tmp = '\0';
	++tmp;
	while(*tmp == ' '){
		++tmp;
	}
	//printf("method:%s\n",method);
	//path:不带web根目录的路径
	path = tmp;
	while(*tmp!=' '){
		++tmp;
	}
	*tmp = '\0';
	--tmp;
	if(*tmp == '/'){
		//如果请求的是 / ,则为其添加默认首页
		sprintf(path,"%sindex.html",path);
	}
	//printf("path:%s\n",path);
	
	if(strcasecmp(method,"GET")!=0 && strcasecmp(method,"POST")!=0){
		//既不是GET请求,也不是POST请求
		error_code = 400;	//请求无效
		goto end;
	}

	//full_path:完整带参数的文件路径
	char full_path[128] = {0};
	sprintf(full_path,"wwwroot%s",path);
	//printf("full_path:%s\n",full_path);
	
	//really_path:完整无参的文件路径
	char really_path[128] = {0};
	sprintf(really_path,"wwwroot%s",path);
	tmp = strchr(really_path,'?');	
	if(tmp!=NULL)
		*tmp = '\0';
	//printf("really_path:%s\n",really_path);
	
	//对请求文件进行检测
	struct stat file_info;
	if(stat(really_path,&file_info) < 0){
		//文件不存在,返回404状态码
		//printf("文件不存在,返回404\n");
		send_error(sock,404);
		return ;
	}
	if(S_ISDIR(file_info.st_mode)){
		//当前请求的为目录,则为其加上首页
		sprintf(full_path,"%s/index.html",full_path);
		//printf("当前请求的为目录,新的full_path为%s\n",full_path);
	}

	/*
	if(file_info.st_mode & S_IXOTH){
		//当前请求存在可执行权限
		//printf("path=%s\n",path);
		return ;
	}
	*/	
	
	//GET POST GET带参数
	if(strcasecmp(method,"GET")==0 && strchr(path,'?')==NULL){
		//GET请求 且 不带参数
		//printf("GET请求且不带参数\n");
		handle_simple_get(sock,full_path,file_info.st_size);
		return ;
	}
	if(strchr(path,'?')!=NULL && strchr(path,'=')==NULL){	
		//虽然GET请求中存在?但不存在=,仍然当作简单请求处理.
		//printf("带?但不带=\n");
		handle_simple_get(sock,full_path,file_info.st_size);
		return ;
	}
	//printf("need cgi!\n");
	//GET请求带参数(一定存在=) 或者 POST请求,需要用到CGI
	exec_cgi(sock,method,full_path);
end:
	//printf("enddddddddddddddddddd\n");
	//send_error(sock,error_code);
	return ;
}
//发送错误码
void send_error(int sock,int error_code){
	char buff[128];
	sprintf(buff,"HTTP/1.0 %d %s\r\n",error_code,"some info");
	send(sock,buff,strlen(buff),0);
}




void* thread_handle_http_request(void* arg){
	struct thread_arg *info = (struct thread_arg*)arg;
	int sock = info->sock;

	char buff[1024];
	int error_code = 500;
	//拿到http请求头信息 GET / HTTP/1.0
	get_line(sock,buff,sizeof(buff));
	char *method = buff;
	char *tmp = buff;
	char *path = NULL;
	//提取method GET 或 POST
	while(*tmp!=' '){
		++tmp;
	}
	*tmp = '\0';
	++tmp;
	while(*tmp == ' '){
		++tmp;
	}
	//printf("method:%s\n",method);
	//path:不带web根目录的路径
	path = tmp;
	while(*tmp!=' '){
		++tmp;
	}
	*tmp = '\0';
	--tmp;
	if(*tmp == '/'){
		//如果请求的是 / ,则为其添加默认首页
		sprintf(path,"%sindex.html",path);
	}
	//printf("path:%s\n",path);
	
	if(strcasecmp(method,"GET")!=0 && strcasecmp(method,"POST")!=0){
		//既不是GET请求,也不是POST请求
		error_code = 400;	//请求无效
		goto end;
	}

	//full_path:完整带参数的文件路径
	char full_path[128] = {0};
	sprintf(full_path,"wwwroot%s",path);
	//printf("full_path:%s\n",full_path);
	
	//really_path:完整无参的文件路径
	char really_path[128] = {0};
	sprintf(really_path,"wwwroot%s",path);
	tmp = strchr(really_path,'?');	
	if(tmp!=NULL)
		*tmp = '\0';
	//printf("really_path:%s\n",really_path);
	
	//对请求文件进行检测
	struct stat file_info;
	if(stat(really_path,&file_info) < 0){
		//文件不存在,返回404状态码
		//printf("文件不存在,返回404\n");
		send_error(sock,404);
		goto end;
	}
	if(S_ISDIR(file_info.st_mode)){
		//当前请求的为目录,则为其加上首页
		sprintf(full_path,"%s/index.html",full_path);
		//printf("当前请求的为目录,新的full_path为%s\n",full_path);
	}

	/*
	if(file_info.st_mode & S_IXOTH){
		//当前请求存在可执行权限
		//printf("path=%s\n",path);
		return ;
	}
	*/	
	
	//GET POST GET带参数
	if(strcasecmp(method,"GET")==0 && strchr(path,'?')==NULL){
		//GET请求 且 不带参数
		//printf("GET请求且不带参数\n");
		handle_simple_get(sock,full_path,file_info.st_size);
		goto end;
	}
	if(strchr(path,'?')!=NULL && strchr(path,'=')==NULL){	
		//虽然GET请求中存在?但不存在=,仍然当作简单请求处理.
		//printf("带?但不带=\n");
		handle_simple_get(sock,full_path,file_info.st_size);
		goto end;
	}
	//printf("need cgi!\n");
	//GET请求带参数(一定存在=) 或者 POST请求,需要用到CGI
	exec_cgi(sock,method,full_path);
end:
	epoll_ctl(info->epfd,EPOLL_CTL_DEL,sock,info->event);
	close(sock);
	return 0;
}
