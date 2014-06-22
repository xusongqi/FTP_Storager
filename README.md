FTP_Storager
============

实现一个类FTP服务器，对文件进行存储和版本管理<br/>
使用B/S架构，分为client端和server端<br/>

Server
============

server端使用epoll对连接进行监听，并使用边缘触发方式处理请求

### 登录模块(尚未对接数据库)

用户信息由[client]封装在xml中发送到[server]<br/>
通过数据库进行登录验证<br/>
使用开源库libxml<br/>

### 文件上传模块

[client]将待上传文件信息封装在xml中发送到server<br/>
收到server的接收确认后，使用sendfile函数发送文件<br/>
[server]下个版本加入splice函数<br/>
	
### 文件下载模块(正在编写中..)

### 文件删除模块(正在编写中..)

### 文件管理模块(下个版本加入)

### 短信通知模块
[client]验证码<br/>
[server]系统报错通知<br/>
使用php调用飞信API<br/>
				
### 邮箱验证模块
向注册邮箱发送验证码<br/>
使用SMTP协议<br/>
			
### 文件快照模块(下个版本加入)
