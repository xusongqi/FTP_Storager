/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年06月18日 星期三 13时20分41秒
* 
* FileName:     xml_translate.h
* 
* Description:  
*
*/

#ifndef _XML_ANALYSE_H_
#define _XML_ANALYSE_H_

#define ERRX_READ_FILE			0b000001	//读取xml错误
#define ERRX_GET_ELEMENT		0b000010	//获取元素错误
#define ERRX_INFO_NOT_MATCH		0b000100	//文件信息不匹配
#define ERRX_TAG_NOT_MATCH		0b001000	//标签不匹配

#define LOGIN_SUCCEED			0b010000	//登录成功
#define LOGIN_FAILED			0b100000	//登录失败

int msg_analyse(int cnt_fd, const char * xml_msg, int msg_len);

#endif
