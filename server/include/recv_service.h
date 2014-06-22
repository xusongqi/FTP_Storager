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

#define READY_RECV_FILE_OK	0X000001

//处理客户请求
int	client_request(int client_fd);						

//接收文件函数
void recv_files(int cnt_fd);

//返回服务器的本地时间
char * server_time();										

//将传入的描述符设置为非阻塞
int	set_non_blocking(int sockfd);						

//服务端主程序
int	tcp_server();										

#endif
