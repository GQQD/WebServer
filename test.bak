/**************************************
*文件说明:test.c
*作者:高小调
*创建时间:2017年07月31日 星期一 19时16分48秒
*开发环境:Kali Linux/g++ v6.3.0
****************************************/
#include"httpd.h"
int main(int argc,const char * argv[]){
	if(argc!=2){
		printf("Usage:%s [port]\n",argv[0]);
		return 1;
	}
	int listen_sock = get_listen_sock(atoi(argv[1]));
	int epfd = epoll_create(256);
	if(epfd < 0){
		perror("epoll_create");
		return 2;
	}
	//将监听套接字加入epoll模型中,关注其读事件.x
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = listen_sock;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&event);
	while(1){
		struct epoll_event event_list[128];
		int ready_nums = epoll_wait(epfd,event_list,128,-1);
		switch(ready_nums){
			case -1:
				perror("epoll_wait");
				break;
			case 0:
				//time out
				break;
			default:
				for(int i=0; i<ready_nums; ++i){
					int fd = event_list[i].data.fd;
					int ev = event_list[i].events;
					if(fd == listen_sock && (ev&EPOLLIN)){
						//处理新客户连接
						struct sockaddr_in client;
						socklen_t len = sizeof(client);
						int new_client = accept(fd,\
								(struct sockaddr*)&client,\
								&len);
						if(new_client < 0){
							perror("accept");
							break;
						}
						printf("get a new client:%s:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
						//将新客户的socket加入epoll模型
						event.events = EPOLLIN;
						event.data.fd = new_client;
						if(epoll_ctl(epfd,EPOLL_CTL_ADD,new_client,&event) < 0){
							perror("add new client to epoll fail");
							close(new_client);
						}
					}else if( ev&&EPOLLIN){
						//处理客户端发来的http请求
						handle_http_request(fd);	
						//处理完后关闭连接
						epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event_list[i]);
						close(fd);	
					}else{
						//忽略其他事件,关闭链接
						printf("received other request\n");
						epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event_list[i]);
						close(fd);	
					}
				}//for
				break;
		}//swtich
	}//while
	close(epfd);
	close(listen_sock);
	return 0;
}
