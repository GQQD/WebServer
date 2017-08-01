/**************************************
*文件说明:httpd.
*作者:高小调
*创建时间:2017年07月31日 星期一 19时16分40秒
*开发环境:Kali Linux/g++ v6.3.0
****************************************/
#include"httpd.h"
static const char* respond_line = "HTTP/1.0 200 OK\r\n";
static const char* blank_line = "\r\n";


//处理CGI
void exec_cgi(int sock,char *method,char *path){
	
}
//处理简单的get请求
void handle_simple_get(int sock,const char *path){
	//读取多余请求
	int ret = 1;
	char buff[1024];
	do{
		ret = get_line(sock,buff,sizeof(buff));
		printf("ret = %d,I get some data:%s",ret,buff);
	}while(ret!=1);
	printf("ret:%d,buff:%s",ret,buff);	
	//将简单请求应答回去
	struct stat file_info;
	if(stat(path,&file_info) < 0){
		//文件不存在,返回404状态码
		send_error(sock,404);
		return ;
	}
	int fd = open(path,O_RDONLY);
	if(fd < 0){
		perror("handle_simple_get:open");
		return ;
	}
	send(sock,respond_line,strlen(respond_line),0);
	send(sock,blank_line,strlen(blank_line),0);
	if(sendfile(sock,fd,NULL,file_info.st_size) < 0){
		perror("handle_simple_get:sendfile");
	}
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
	printf("method:%s\n",method);
	//提取path
	path = tmp;
	while(*tmp!=' '){
		++tmp;
	}
	*tmp = '\0';
	--tmp;
	//如果请求的是 / ,则为其添加默认首页
	if(*tmp == '/'){
		sprintf(path,"%sindex.html",path);
	}
	printf("path:%s\n",path);
	//处理异常请求
	if(strcasecmp(method,"GET")!=0 && strcasecmp(method,"POST")!=0){
		//既不是GET请求,也不是POST请求
		error_code = 400;	//请求无效
		goto end;
	}
	char full_path[128];
	sprintf(full_path,"wwwroot%s",path);
	printf("full_path:%s\n",full_path);
	//GET POST GET带参数
	if(strcasecmp(method,"GET")==0 && strchr(path,'?')==NULL){
		//GET请求 且 不带参数
		handle_simple_get(sock,full_path);
	}
	//GET请求带参数 或者 POST请求,需要用到CGI
	exec_cgi(sock,method,full_path);
end:
	//printf("enddddddddddddddddddd\n");
	//send_error(sock,error_code);
	return ;
}
//发送错误码
void send_error(int sock,int error_code){
	char buff[128];
	sprintf(buff,"HTTP/1.0 %d %s\r\n",error_code,"not found");
	send(sock,buff,strlen(buff),0);
}


