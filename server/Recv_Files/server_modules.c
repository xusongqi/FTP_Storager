/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年06月24日 星期二 11时26分02秒
* 
* FileName:     Recv_Files/server_modules.c
* 
* Description:  
*
*/

#include "../include/database.h"
#include "../include/ftp_server.h"
#include "../include/log.h"
#include "../include/recv_service.h"
#include "../include/xml_translate.h"
#include "../include/server_modules.h"

/*———————————————————————————————————————
 * 
 * 函数：recv_files
 *
 * 作用：接收文件
 *
 * ——————————————————————————————————————*/
void recv_files(int cnt_fd)
{
	char buffer[LENGTH] = {'\0'};
	int recbytes = 0;
	FILE * target_fd = NULL;

	target_fd = fopen("NEW_timeInABottle.mp3", "ab+");
	if(target_fd == NULL)
	{	perror("../runing/errorlog/syserr.log");}

	//请求client上传文件
	write(cnt_fd, "READY_RECV_FILE_OK", sizeof("READY_RECV_FILE_OK"));

	while( recbytes = read(cnt_fd, buffer, LENGTH) )
	{
		fwrite(buffer, sizeof(unsigned char), recbytes, target_fd);
		memset(buffer, '\0', LENGTH);
	}

	fclose(target_fd);
}


