/*************************************************************************
    > File Name: log.c
    > Author: 韩佩言
    > Mail: xiyouhanpeiyan@gmail.com 
    > Created Time: 2014年06月20日 星期五 11时25分19秒
    > Class: 计科1107
 ************************************************************************/

#include "ftp_server.h"
#include "log.h"

// errorlog 日志存储路径
#define ERRORLOG "../runing/errorlog/"

// syslog 日志存储路径
#define SYSLOG "../runing/syslog/"

/*************************************************
     功能：写文件操作，记录 error 日志
     参数：user——登陆用户的用户名
           error_info——错误信息
   返回值：1、表示写入成功
           0、表示写入失败
*************************************************/
int SWriteErrorLog(char *user, char *error_info)
{
	FILE *fp;
	char errorlog[512];
	int len;
	
	strcpy(errorlog, ERRORLOG);
	strcat(errorlog, user);
	strcat(errorlog, ".log");
	if ((fp = fopen(errorlog, "a+")) == NULL)
	{
		return 0;
	}
	len = strlen(error_info);
	if (fwrite(error_info, len, 1, fp) != 1)
	{
		return 0;
	}
	fclose(fp);
	return 1;
}


/*************************************************
     功能：获取当前系统时间
     参数：tmp——作为返回值记录系统时间
   返回值：无
*************************************************/
void SGetTime(char tmp[])
{
	time_t nowtime;
	struct tm *p = NULL;
	int length = 0;
    
	time(&nowtime); 
	p = localtime(&nowtime);
	length =  strftime(tmp, 20, "%Y-%m-%d %H:%M:%S", p);
	tmp[length] = '\0';
}

