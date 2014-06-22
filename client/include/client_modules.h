/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年06月20日 星期五 14时25分36秒
* 
* FileName:     client_modules.h
* 
* Description:  
*
*/

#ifndef _CLIENT_MUDULES_H_
#define _CLIENT_MUDULES_H_

#define READY_RECV_FILE_OK	0X000001

void login(int cnt_fd);			//登录

void logout(int cnt_fd);		//登出

void signin(int cnt_fd);		//注册

void upload(int cnt_fd);		//上传

void download(int cnt_fd);		//下载

#endif
