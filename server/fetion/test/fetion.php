<?php
class Fetion{
	private $url = "http://localhost/fetion/feixinclass.php?user=";	// 服务器的 url
	private $SendTel = "*********";					// 发送方飞信帐号
	private $SendTelPwd = "******";					// 发送方飞信密码
	private $SendTelTo;
	private $Message;

	// 构造函数
	function __construct($tel, $msg)
	{
		$this->SendTelTo = $tel;
		$this->Message = $msg;
	}
	
	// 创建目录
	function Mkdirs($dir)  
	{  
		if(!is_dir($dir))  
		{  
			if(!mkdir($dir, 0777))
			{  
				return false;  
			}  
		}  
		return true;  
	}

	// 日志记录
	function RecordLog()
	{
		date_default_timezone_set('Etc/GMT-8');				//这里设置了时区
		$nowtime = date("Y-m-d H:i:s");
		
		if (!$this->Mkdirs("./log"))
		{
			return false;
		}
		$filename = "log/" . $this->SendTelTo . ".txt";
		$tmp = "发送至:   " . $this->SendTelTo . " \n时间:     " . $nowtime . "\n内容:     " . $this->Message . "\n\n";
		$fp = fopen($filename, "a");
		fwrite($fp, $tmp);
		fclose($fp);
	}
	
	// 飞信发送
	function SendFetion()
	{
		$address = $this->url . $this->SendTel . "&passwd=" . $this->SendTelPwd . "&to=" . $this->SendTelTo . "&msg=" . $this->Message;
		file_get_contents($address);
		$this->RecordLog();
	}
}
?>
