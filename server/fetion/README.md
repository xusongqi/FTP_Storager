最近在网上找了一些飞信发送的实现，这个就是参考网上代码写的

使用方法：
	环境：
		首先你得安装 PHP 环境和 Apache 服务器.我的电脑是 fedora 18系统，64 位操作系统。
		使用 yum 安装的环境

		网页默认执行路径是 /var/www/html。
		我的飞信类放的路径是 /var/www/html/fetion/
		下面是我的路径结构

		[hpy@localhost fetion]$ pwd
		/var/www/html/fetion
		[hpy@localhost fetion]$ tree
		.
		|-- feixinclass.php
		|-- Fetionclass.php
		|-- README.md
		`-- test
		    |-- fetion.php
			`-- test.php

				1 directory, 5 files


eg: http://localhost/feixin.php?user=你的手机号&passwd=你的飞信密码&to=发送给某人的手机&msg=发送的内容
所以你可以将这个飞信代码( feixinclass.php 和 Fetionclass.php )放到该目录下，然后使用 url就可以直接调用进行发送了。

注意：
	当然我自己也实现了不用 url 发送的部分，代码在 test 文件中。如果你使用test文件里的接口，那么必须设置下面的内容
	fetion.php 中可以设置你的发送者手机号、密码
	test.php 中是你调用的接口，里面设置接受者手机号码、发送内容


