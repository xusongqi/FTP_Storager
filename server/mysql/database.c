/*************************************************************************
    > File Name: database.c
    > Author: 韩佩言
    > Mail: xiyouhanpeiyan@gmail.com 
    > Created Time: 2014年06月04日 星期三 00时26分37秒
    > Class: 计科1107
 ************************************************************************/

#include "ftp_server.h"
#include "database.h"
#include "log.h"

// 错误日志缓冲区大小
#define N 100

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
	// 判断连接情况
	if (conn_status == 1)
	{
		printf("已经连接到数据！\n");
		return 1;
	}

	// 初始化数据库结构
	mysql_init(&conn);

	// 连接数据库
	if (!mysql_real_connect(&conn, "localhost", "root", "19921029", "FTPserver", 0, NULL, 0))	// 填写密码
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
	char error_info[N];
	char sys_info[N];
	char nowtime[20];
	
	SGetTime_20(nowtime);
	memset(add_new, 0, sizeof (add_new));
	sprintf(add_new, "INSERT INTO UserPasswd(user, nickname, passwd) VALUES('%s', '%s', '%s')", user, nickname, passwd);

	if (mysql_query(&conn, add_new))
	{
		sprintf(error_info, "%s   Register %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		SWriteErrorLog(user, error_info);
		return 0;
	}
	else
	{
		sprintf(sys_info, "%s   %s   Register   %s\n", nowtime, user, "\r\b");
		SWriteSysLog(user, sys_info);
		return 1;
	}
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
	char error_info[N];
	char sys_info[N];
	char nowtime[20];
	
	SGetTime_20(nowtime);
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
			while ((row = mysql_fetch_row(res)))
			{
				if (strcmp(row[2], passwd) == 0)
				{
					sprintf(sys_info, "%s   %s   Login   %s\n", nowtime, user, "\r\b");
					SWriteSysLog(user, sys_info);
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
         ：newpasswd——修改后的密码
   返回值：1、表示修改成功
           0、表示修改失败
*************************************************/
int SEditPasswd(char *user, char *newpasswd)
{
	char edit[100];
	char error_info[N];
	char sys_info[N];
	char nowtime[20];
	
	SGetTime_20(nowtime);
	memset(edit, 0, sizeof (edit));
	sprintf(edit, "UPDATE UserPasswd SET passwd = '%s' WHERE user = '%s'", newpasswd, user);
	if (mysql_query(&conn, edit))
	{
		sprintf(error_info, "%s   EditPasswd %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		SWriteErrorLog(user, error_info);
		return 0;
	}
	else
	{
		sprintf(sys_info, "%s   %s   EditPasswd   %s\n", nowtime, user, "\r\b");
		SWriteSysLog(user, sys_info);
	}

	return 1;
}

/*************************************************
     功能：数据库修改用户昵称
     参数：user——所要修改用户的用户名
         ：newnick——修改后的昵称
   返回值：1、表示修改成功
           0、表示修改失败
*************************************************/
int SEditNickname(char *user, char *newnick)
{
	char edit[100];
	char error_info[N];
	char sys_info[N];
	char nowtime[20];
	
	SGetTime_20(nowtime);
	memset(edit, 0, sizeof (edit));
	sprintf(edit, "UPDATE UserPasswd SET nickname = '%s' WHERE user = '%s'", newnick, user);
	if (mysql_query(&conn, edit))
	{
		sprintf(error_info, "%s   EditNickname %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		SWriteErrorLog(user, error_info);
		return 0;
	}
	else
	{
		sprintf(sys_info, "%s   %s   EditNickname   %s\n", nowtime, user, "\r\b");
		SWriteSysLog(user, sys_info);
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
	char error_info[N];
	char sys_info[N];
	char nowtime[20];
	
	SGetTime_20(nowtime);
	memset(del, 0, sizeof (del));
	sprintf(del, "DELETE FROM UserPasswd WHERE user = '%s'", user);
	if (mysql_query(&conn, del))
	{
		sprintf(error_info, "%s   Logout UserPasswd %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		SWriteErrorLog(user, error_info);
		return 0;
	}
	sprintf(del, "DELETE FROM UserInfo WHERE user = '%s'", user);
	if (mysql_query(&conn, del))
	{
		sprintf(error_info, "%s   Logout UserInfo %d: %s   %s\n", nowtime, mysql_errno(&conn), mysql_error(&conn), "\r\b");
		SWriteErrorLog(user, error_info);
		return 0;
	}
	else
	{
		sprintf(sys_info, "%s   %s   EditPasswd   %s\n", nowtime, user, "\r\b");
		SWriteSysLog(user, sys_info);
		return 1;
	}
}
