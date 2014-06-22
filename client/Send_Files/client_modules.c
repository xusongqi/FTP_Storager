/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年06月20日 星期五 14时18分37秒
* 
* FileName:     client_modules.c
* 
* Description:  
*
*/
 
#include "../include/ftp_client_include.h"
#include "../include/client_modules.h"


/*——————————————————————————
 *
 * 函数：login(int cnt_fd)
 *
 * 作用：发送登录请求
 *
 * 实现：使用xml发送登录请求
 *
 *—————————————————————————— */
void login(int cnt_fd)
{	
	char user[20],
		 pwd[20];
	xmlChar *xbuffer;
	int length = LENGTH;
	xmlDocPtr	xdoc	= xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr	xroot	= xmlNewNode(NULL, BAD_CAST "login");
			//	xuser	= xmlNewNode(NULL, BAD_CAST "user"),
			//	xpwd	= xmlNewNode(NULL, BAD_CAST "password");

	xmlDocSetRootElement(xdoc,xroot);
	xmlNewChild(xroot, NULL, BAD_CAST "user", BAD_CAST "xsq_user");
	xmlNewChild(xroot, NULL, BAD_CAST "pwd", BAD_CAST "xsq_pwd");

	xmlDocDumpFormatMemoryEnc(xdoc, &xbuffer, &length, "utf8",1);
	write(cnt_fd, xbuffer, strlen(xbuffer)+1);
}

void logout(int cnt_fd)
{
	
}

/*
void signin(int cnt_fd)
{
	char create_user[20],
		 create_pwd[20];
	xmlDocPtr	xcreate_doc	= xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr	xcreate_root	= xmlNewNode(NULL, BAD_CAST "login");
			//	xcreate_user	= xmlNewNode(NULL, BAD_CAST "user"),
			//	xcreate_pwd		= xmlNewNode(NULL, BAD_CAST "password");

	puts("create user:");
	gets(create_user);
	fflush(stdin);//清空输入缓冲区
	puts("create password:");
	gets(create_pwd);
	fflush(stdin);//清空输入缓冲区

	xmlDocSetRootElement(xcreate_doc,xcreate_root);
	xmlNewChild(xcreate_root, NULL, BAD_CAST "create_user", BAD_CAST create_user);
	xmlNewChild(xcreate_root, NULL, BAD_CAST "create_pwd", BAD_CAST create_pwd);
}
*/
void upload(int cnt_fd)
{
	/*文件读取模块*/
	//FILE * target_fd;
	char readline[LENGTH],
		 buffer[LENGTH];
/*	target_fd = fopen("timeInABottle.mp3", "rb+");
	if( target_fd == NULL)
	{
		printf("OPEN FILE ERROR\n");
		return;
	}
*/	//获取文件大小
	int target_fd = open("timeInABottle.mp3",O_RDONLY);
	struct stat stat_buf;
	fstat(target_fd, &stat_buf);
	unsigned long file_left = stat_buf.st_size,
				  recbytes = 0;

	/*xml包生成模块*/
	xmlDocPtr xdoc = xmlNewDoc(BAD_CAST "1.0");
	xmlNodePtr xroot = xmlNewNode(NULL, BAD_CAST "upload"),
			   xfilename = xmlNewNode(NULL, BAD_CAST "test_filename");
	
	xmlDocSetRootElement(xdoc,xroot);
	xmlAddChild(xroot, xfilename );
	xmlChar *xbuffer;		//将xml写入该内存
	int length = LENGTH;	//写入内存的xml内容长度
	xmlDocDumpFormatMemoryEnc(xdoc, &xbuffer, &length, "utf8",1);
	
	/*发送xml包*/
	write(cnt_fd, xbuffer, strlen(xbuffer)+1 );
	
	/*读取ack_upload*/
	read(cnt_fd, buffer, LENGTH);
	if( ! strcmp(buffer, "READY_RECV_FILE_OK") )
	{	
		/* sendfile函数，零拷贝，直接通过内核发送，效率高*/
		while( file_left > 0 )
		{
			recbytes = sendfile(cnt_fd, target_fd, NULL, LENGTH);
			if(recbytes < 0 && errno == EAGAIN)
			{	continue; }
			else if( recbytes == 0)
			{	break; }
			else
			{	file_left -= recbytes; }
		}
	
		/* 普通方式发送文件内容，经过用户态，效率低*/
/*		while( fread(readline, sizeof(unsigned char), LENGTH, target_fd) )
		{	write(cnt_fd, readline, sizeof(readline) ); }
*/	}

	xmlFree(xbuffer);
	xmlFreeDoc(xdoc);
	xmlCleanupParser();
	close(target_fd);
	return;

}
/*
void download(int cnt_fd)
{
	char	filename[],	
			buffer[LENGTH];		//接受文件缓冲区
	int		recbytes;			//接收到的长度
	FILE *	target_fd;			//文件描述符

	target_fd = fopen(filename, )

	while( recbytes = read(cnt_fd, buffer, LENGTH) )
	{
	/* 当recbytes > 0，正常
	 * 当recbytes = -1，且errno = 11，正常
	 * 其他情况：关闭*/
/*		if(recbytes > 0)//当然还有[recbytes <0 && errno == EAGAIN]的情况，但不会被epoll_wait()返回到get_act_fds
		{
			buffer[recbytes]='\0';
			fwrite(buffer, sizeof(unsigned char), recbytes, target_fd);
			memset(buffer, '\0', LENGTH);
			//msg_analyse(buffer, recbytes);	//解析xml函数
		}
		/*recbytes <= 0，发生读取错误*/
/*		else 
		{	return -1; }
	}
}*/
