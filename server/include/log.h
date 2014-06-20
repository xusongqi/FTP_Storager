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

// 获取当前系统时间
void SGetTime(char tmp[]);

#endif
