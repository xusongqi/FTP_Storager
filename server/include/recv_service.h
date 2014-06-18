/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年06月18日 星期三 13时12分49秒
* 
* FileName:     recv_service.h
* 
* Description:  
*
*/

#ifndef _RECV_SERVICE_
#define _RECV_SERVICE_

int		client_request(int client_fd);						//处理客户请求
int		msg_analyse(const char * xml_msg, int msg_len);		//xml处理函数
char *	server_time();										//返回服务器的本地时间
int		set_non_blocking(int sockfd);						//将传入的描述符设置为非阻塞
int		tcp_server();										//服务端主程序

#endif
