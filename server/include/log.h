/*************************************************************************
    > File Name: log.h
    > Author: 韩佩言
    > Mail: xiyouhanpeiyan@gmail.com 
    > Created Time: 2014年06月20日 星期五 11时22分16秒
    > Class: 计科1107
 ************************************************************************/

#ifndef LOG_H_
#define LOG_H_

// 写文件操作，记录 error 日志
int SWriteErrorLog(char *user, char *error_info);

// 写文件操作，记录 sys 日志
int SWriteSysLog(char *user, char *sys_info);

// 获取当前系统时间（年月日时分秒）(20 指 tmp 长度)
void SGetTime_20(char tmp[]);

// 获取当前系统日期（年月日）（11 指 tmp 长度）
void SGetTime_11(char tmp[]);

#endif
