/*************************************************************************
    > File Name: database.c
    > Author: 韩佩言
    > Mail: xiyouhanpeiyan@gmail.com 
    > Created Time: 2014年06月04日 星期三 00时26分37秒
    > Class: 计科1107
 ************************************************************************/

#include "ftp_server.h"
#include "database.h"

// errorlog 日志存储路径
#define ERRORLOG "../runing/errorlog/"

// syslog 日志存储路径
#define SYSLOG "../runing/syslog/"

// 连接描述
MYSQL conn;
MYSQL_RES *res;
MYSQL_ROW row;

// 判断连接情况（1：表示连接，0：表示未连接）
static int conn_status = 0; 

/*************************************************
     功能：连接数据库
     参数：无
   返回值：1、表示连接成功
           0、表示连接失败
*************************************************/
int SMysqlConn()
{
	int leap;

	// 判断连接情况
	if (conn_status == 1)
	{
		printf("已经连接到数据！\n");
		return 1;
	}

	// 初始化数据库结构
	mysql_init(&conn);

	// 连接数据库
	if (!mysql_real_connect(&conn, "localhost", "root", "passwd", "FTPserver", 0, NULL, 0))	// 填写密码
	{
		return 0;
	}
	mysql_set_character_set(&conn, "utf8");
	conn_status = 1;

	return 1;
}

/*************************************************
     功能：数据库注册帐号
     参数：user——注册用户名
           nickname——昵称
           passwd——密码
   返回值：1、表示注册成功
           0、表示注册失败
*************************************************/
int SRegister(char *user, char *nickname, char *passwd)
{
	char add_new[100];
	char error_info[512];
	char nowtime[20];
	
	SGetTime(nowtime);
	memset(add_new, 0, sizeof (add_new));
	sprintf(add_new, "INSERT INTO UserPasswd(user, nickname, passwd) VALUES('%s', '%s', '%s')", user, nickname, passwd);

	if (mysql_query(&conn, add_new))
	{
		sprintf(error_info, "%s   Register %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		SWriteErrorLog(user, error_info);
		return 0;
	}
	return 1;
}

/*************************************************
     功能：数据库登陆帐号
     参数：user——登陆的用户名
           passwd——登陆的密码
   返回值：1、表示登陆成功
           2、表示登陆失败
*************************************************/
int SLogin(char *user, char *passwd)
{
	char FindRecord[100];
	char error_info[512];
	char nowtime[20];
	
	SGetTime(nowtime);
	memset(FindRecord, 0, sizeof (FindRecord));
	sprintf (FindRecord, "SELECT * FROM UserPasswd WHERE user = '%s'", user);
	if (mysql_query(&conn, FindRecord))
	{
		sprintf(error_info, "%s   Login %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		SWriteErrorLog(user, error_info);
	}
	else
	{
		res = mysql_store_result(&conn);
		if (res)
		{
			while (row = mysql_fetch_row(res))
			{
				if (strcmp(row[2], passwd) == 0)
				{
					printf("Login success!\n");
					return 1;
				}
			}
			sprintf(error_info, "%s   No User or Passwd error   %s\n", nowtime, "\r\b");
			SWriteErrorLog(user, error_info);
		}
	}
	return 0;
}

/*************************************************
     功能：数据库修改用户密码
     参数：user——所要修改用户的用户名
   返回值：1、表示修改成功
           0、表示修改失败
*************************************************/
int SEditPasswd(char *user, char *newpasswd)
{
	char edit[100];
	char error_info[512];
	char nowtime[20];
	
	SGetTime(nowtime);
	memset(edit, 0, sizeof (edit));
	sprintf(edit, "UPDATE UserPasswd SET passwd = '%s' WHERE user = '%s'", newpasswd, user);
	if (mysql_query(&conn, edit))
	{
		sprintf(error_info, "%s   EditPasswd %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		return 0;
	}

	return 1;
}

/*************************************************
     功能：断开数据库
     参数：无
   返回值：1、表示连接成功
           0、表示连接失败
*************************************************/
void SMysqlClose()
{
	if (conn_status == 1)
	{
		mysql_close(&conn);
	}
	conn_status = 0;
}


/*************************************************
     功能：注销用户（彻底删除用户）
     参数：user——注销的用户名
   返回值：1、表示注销成功。
           0、表示注销失败
*************************************************/
int SLogout(char *user)
{
	char del[100];
	char error_info[512];
	char nowtime[20];
	
	SGetTime(nowtime);
	memset(del, 0, sizeof (del));
	sprintf(del, "DELETE FROM UserPasswd WHERE user = '%s'", user);
	if (mysql_query(&conn, del))
	{
		sprintf(error_info, "%s   Logout UserPasswd %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		return 0;
	}
	sprintf(del, "DELETE FROM UserInfo WHERE user = '%s'", user);
	if (mysql_query(&conn, del))
	{
		sprintf(error_info, "%s   Logout UserInfo %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		return 0;
	}

	return 1;
}

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
