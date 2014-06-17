/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年06月17日 星期二 09时54分44秒
* 
* FileName:     recv.c
* 
* Description:  接收send端发送的信息并存为文件

* Parameter:  -lxml2 
*
*/

#include "ftp_server.h" 

#define	LENGTH		1024
#define	MAXEVENTS	1024

int		client_request(int client_fd);						//处理客户请求
int		msg_analyse(const char * xml_msg, int msg_len);		//xml处理函数
char *	server_time();										//返回服务器的本地时间
int		set_non_blocking(int sockfd);						//将传入的描述符设置为非阻塞
int		tcp_server();										//服务端主程序

/* ——————————————————————————————————————
 *
 * 作用：对客户端请求进行处理
 *
 * 方式：处理xml格式信息
 *
 * ——————————————————————————————————————*/
int client_request(int client_fd)
{
	int recbytes;//	计数buffer收到的字节数[read()]
	char buffer[LENGTH];//存储收到的信息[read()]
	char server_msg[LENGTH];//发送内容长度[write()]
	struct sockaddr_in client_addr;//客户端地址[包括ip与端口]

	/*获取对端ip与端口信息*/
	int len=sizeof(client_addr);
	getpeername( client_fd, (struct sockaddr *)&client_addr,&len );
	/*read() 收取数据*/
	FILE * target_fd;
	target_fd = fopen("timeMusic2.mp3", "ab+");
	if(target_fd == NULL)
	{	puts("****************文件未打开！*******************");}
	
	while( recbytes = read(client_fd, buffer, LENGTH))
	{
	/* 当recbytes > 0，正常
	 * 当recbytes = -1，且errno = 11，正常
	 * 其他情况：关闭*/
		if(recbytes > 0)//当然还有[recbytes <0 && errno == EAGAIN]的情况，但不会被epoll_wait()返回到get_act_fds
		{
			buffer[recbytes]='\0';
			printf("%s    ",buffer);
			printf("from %#x : %#x : ",
					ntohl(client_addr.sin_addr.s_addr),ntohs(client_addr.sin_port));	
			printf("%s\n",server_time());
			
			fwrite(buffer, sizeof(unsigned char), recbytes, target_fd);
			memset(buffer, '\0', LENGTH);
			//msg_analyse(buffer, recbytes);	//解析xml函数
		}
		/*recbytes <= 0，发生读取错误*/
		else 
		{	return -1; }
	}
	/*	while读取结束
	 *	输出断开连接的时间*/
	printf(" server disconnected from %#x : %#x : ",
				ntohl(client_addr.sin_addr.s_addr),ntohs(client_addr.sin_port));	
	printf("%s\n",server_time());
	fclose(target_fd);
	close(client_fd);
	return 0;
}

/*———————————————————————————————————————
 *
 * 作用：获取当前时间
 *
 * ——————————————————————————————————————*/
char * server_time()
{
	time_t rawtime;//服务器时间
	struct tm * server_time;
	time(&rawtime);
	server_time = localtime(&rawtime);
	return asctime(server_time);
}

int	set_non_blocking(int sockfd)
{
	/* 内层调用fcntl()的F_GETFL获取flag，
	 * 外层fcntl()将获取到的flag设置为O_NONBLOCK非阻塞*/
	if( fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) ) == -1)
	{	return -1;}
	return 0;
}

/*———————————————————————————————————————
 *
 * 作用：服务器主函数
 *
 * 方式：启动服务器
 *
 * ——————————————————————————————————————*/
int tcp_server()
{
	int listen_fd,		//描述符：接受所有连接请求	
		client_fd,		//描述符：处理单独的客户请求
		epoll_fd,		//描述符：epoll
		get_act_fds,	//epoll_wait()返回的事件描述符数量
		count_fds = 0;	//epoll监控描述符计数器
	struct sockaddr_in server_addr,//服务端地址
					   client_addr;//客户端地址
	unsigned short portnum = 21567;//服务器使用端口
	int sin_size;//sockaddr_in的地址长度
	struct epoll_event	event_act,//要监听的描述符的动作
						events[MAXEVENTS];//epoll事件队列

	/*设置监听的端口和IP信息*/
	//bzero(&server_addr, sizeof(struct sockaddr_in));
	memset(&server_addr, sizeof(struct sockaddr_in),'\0');
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(portnum);

	/*socket() */
	listen_fd = socket(AF_INET, SOCK_STREAM,0);
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, NULL, 1);//端口复用,最后两个参数常用opt=1和sizeof(opt)
	if(listen_fd == -1)
	{
		printf("SOCKET FAILED    ");
		printf("%s",server_time());
		return 1;
		//exit(1);
	}
	printf("socket ok... ");

	/*bind() */
	if(-1 == bind(listen_fd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr)))
	{
		printf("BIND FAILED    ");
		printf("%s",server_time());
		return 1;
		//exit(1);
	}
	printf("bind ok... \n");

	/*listen() */
	if(-1 == listen(listen_fd,5))
	{
		printf("LISTEN FAILED    ");
		printf("%s",server_time());
		return 1;
		//exit(1);
	}
	printf("listen ok...");
	
	sin_size = sizeof(struct sockaddr_in);

	/*epoll_create() */
	epoll_fd = epoll_create(MAXEVENTS);
	event_act.events = EPOLLIN | EPOLLET;//可读检测 + 边缘触发
	event_act.data.fd = listen_fd;//设置新事件为监听描述符

	/*epoll_ctl() 描述符加入监听队列*/
	if( epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event_act) < 0 )
	{
		printf("EPOLL_CTL FAILED    ");
		printf("%s",server_time());
		return 1;
		//exit(1);
	}
	count_fds++;


	/*开始循环监听 */
	while(1)
	{
		/*epoll_wait() 最后一个参数-1时，当描述符可读则立即返回*/
		get_act_fds = epoll_wait(epoll_fd, events, count_fds, -1);
		//printf("sdklskdlskdl: %d\n",get_act_fds);
		if(get_act_fds == -1)
		{
			printf("EPOLL_WAIT FAILED    ");
			printf("%s",server_time());
			continue;
		}
		for(int i = 0; i < get_act_fds; i++)
		{
			if(events[i].data.fd == listen_fd)
			{
				/*accept() */
				if(-1 == (client_fd = accept(listen_fd,(struct sockaddr *)(&client_addr),&sin_size)))
				{
					printf("ACCEPT FAILED    ");
					printf("%s",server_time());
					continue;
				}
				/*输出连接客户的ip和端口*/
				printf("accept ok... \nserver start get connect from %#x : %#x\n",
							ntohl(client_addr.sin_addr.s_addr),ntohs(client_addr.sin_port));	
				/*如果当前epoll内描述符队列已满*/
				if(count_fds >= MAXEVENTS)
				{
					printf("TOO MANY CONNECTIONS\n");
					continue;
				}
				/*设置非阻塞io*/
				if( set_non_blocking(client_fd) != 0 )
				{
					printf("SET_NON_BLOCKING FAILED    ");
					printf("%s",server_time());
					close(client_fd);
					continue;
				}
				/*设置epoll对该描述符的监听模式*/
				event_act.events = EPOLLIN | EPOLLET;
				event_act.data.fd = client_fd;
				/*epoll_ctl() 描述符加入监听队列*/
				if( epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event_act) <0 )
				{
					printf("EPOLL_CTL ADD CLIENT FAILED    ");
					printf("%s",server_time());
					close(client_fd);
					continue;
				}
				count_fds++;
				continue;
			}//[if(events[i] == listen_fd)]结束
			else if(events[i].events & EPOLLIN)
			{
			  if(client_request(events[i].data.fd)<0)
			  {
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event_act);
				count_fds--;
			  }
			}
			/*处理客户请求，若连接断开则从epoll监听队列中删除该描述符*/
			/*else if(client_request(events[i].data.fd) < 0)
			{
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event_act);
				count_fds--;
			}*/

		}//[for(i = 0; i < get_act_fds; i++)]结束
	}//[while大循环]结束
	close(listen_fd);
}

/*int main(void)
{
	while(1)
	{
		tcp_server();
		sleep(1);
	}

	return 0;
}*/
