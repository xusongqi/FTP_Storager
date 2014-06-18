/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年06月17日 星期二 11时25分44秒
* 
* FileName:     send.c
* 
* Description:  
*
*/

#include "../include/ftp_client_include.h"
#include "../include/send.h"

#define LENGTH 1024

int tcp_client()
{
	int i,								//循环变量
		choice,							//用户功能选择
		cnt_fd,							//连接的描述符
		recbytes;						//计数器
	char recv_buffer[LENGTH]={'\0'},	//发送数据缓冲区
		 send_buffer[LENGTH]={'\0'},	//接收数据缓冲区
		 user_name[30] = {'\0'},		//用户名
		 user_password[30] = {'\0'},	//密码
		 encrypt_password[32] = {'\0'};	//加密后的密码
	struct sockaddr_in server_addr,		//服务端地址
					   client_addr;		//客户端地址
	unsigned short portnum = 21567;		//端口信息

	cnt_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(cnt_fd <= 0)
	{
		printf("SOCKET FAILED\n");
		return -1;
	}
	printf("socket...ok\t");


	memset(&server_addr, sizeof(server_addr), '\0');		//将目标地址变量置零
	server_addr.sin_family = AF_INET;						//设置目标地址的地址描述，一般都是AF_INET
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	//设置目标IP
	server_addr.sin_port = htons(portnum);					//设置目标端口

	if(connect(cnt_fd, (struct sockaddr *)(&server_addr), sizeof(server_addr) ) == -1)
	{
		printf("CONNECT FAILED\n");
		return -1;
	}printf("connect ...ok\n");

	/*
	 *文件读取模块
	 * */
	FILE * target_fd;
	char readline[LENGTH];
	target_fd = fopen("timeMusic.mp3", "rb+");

	/* sendfile函数，零拷贝，直接通过内核发送，效率高*/
	sendfile();

	/* 普通方式发送文件内容，经过用户态，效率低
	while( fread(readline, sizeof(unsigned char), LENGTH, target_fd) )
	{
		//printf("%s\n", readline);
		write(cnt_fd, readline, sizeof(readline) );
	}
	*/
}

/*
int main(void)
{
	tcp_client();
}
*/
