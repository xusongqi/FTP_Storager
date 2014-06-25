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
#define ERRORLOG "./runing/errorlog/"

// syslog 日志存储路径
#define SYSLOG "./runing/syslog/"

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
	char errorlog[100];
	int len;
	
	strcpy(errorlog, ERRORLOG);
	strcat(errorlog, user);
	strcat(errorlog, ".log");
	if ((fp = fopen(errorlog, "a+"))==NULL)
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
     功能：写文件操作，记录 sys 日志（用户操作日志）
     参数：user——登陆用户的用户名
           sys_info——错误信息
   返回值：1、表示写入成功
           0、表示写入失败
*************************************************/
int SWriteSysLog(char *user, char *sys_info)
{
	FILE *fp;
	char nowtime[20];
	char syslog[100];
	int len;

	SGetTime_11(nowtime);
	strcpy(syslog, SYSLOG);
	strcat(syslog, nowtime);
	strcat(syslog, ".log");
	if ((fp = fopen(syslog, "a+")) == NULL)
	{
		return 0;
	}
	len = strlen(sys_info);
	if (fwrite(sys_info, len, 1, fp) != 1)
	{
		return 0;
	}
	fclose(fp);
	return 1;
}

/*************************************************
     功能：获取当前系统时间（年月日时分秒）
     参数：tmp——作为返回值记录系统时间
   返回值：无
*************************************************/
void SGetTime_20(char tmp[])
{
	time_t nowtime;
	struct tm *p = NULL;
	int length = 0;
    
	time(&nowtime); 
	p = localtime(&nowtime);
	length =  strftime(tmp, 20, "%Y-%m-%d %H:%M:%S", p);
	tmp[length] = '\0';
}

/*************************************************
     功能：获取当前系统日期（年月日）
     参数：tmp——作为返回值记录系统时间
   返回值：无
*************************************************/
void SGetTime_11(char tmp[])
{
	time_t nowtime;
	struct tm *p = NULL;
	int length = 0;
    
	time(&nowtime); 
	p = localtime(&nowtime);
	length =  strftime(tmp, 11, "%Y-%m-%d", p);
	tmp[length] = '\0';
}
