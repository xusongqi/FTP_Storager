/*************************************************************************
    > File Name: database.h
    > Author: 韩佩言
    > Mail: xiyouhanpeiyan@gmail.com 
    > Created Time: 2014年06月04日 星期三 00时26分37秒
    > Class: 计科1107
 ************************************************************************/

#ifndef DATABASE_H_
#define DATABASE_H_

// 连接数据库
int SMysqlConn();

// 数据库注册帐号
int SRegister(char *user, char *nickname, char *passwd);

// 数据库登陆帐号
int SLogin(char *user, char *passwd);

// 数据库修改用户密码
int SEditPasswd(char *user, char *newpasswd);

// 断开数据库
void SMysqlClose();

// 注销用户（彻底删除用户）
int SLogout(char *user);

#endif
