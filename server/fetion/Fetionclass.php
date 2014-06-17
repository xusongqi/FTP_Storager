<?php
 // PHP飞信发送类
class Fetionclass
{
    protected $mobile;				// 发送者手机号码
    protected $password;			// 发送者飞信密码
    protected $cookie = '';
    protected $uids = array();
    protected $csrfToten = null;
    
    public function __construct($getmobile, $getpassword)
    {
        $this->mobile = $getmobile;
        $this->password = $getpassword;
	$this->login();
    }

    public function __destruct()
    {
        $this->logout();
    }

    /*
     * 登录
     */
    protected function login()
    {
        $uri = '/huc/user/space/login.do?m=submit&fr=space';
        $data = 'mobilenum='.$this->mobile.'&password='.urlencode($this->password);
        $result = $this->postWithCookie($uri, $data);
        preg_match_all('/.*?\r\nSet-Cookie: (.*?);.*?/si', $result, $matches);  // 解析 Cookie
        if (isset($matches[1]))
        {
            $this->cookie = implode('; ', $matches[1]);
        }
        $result = $this->postWithCookie('/im/login/cklogin.action', '');
        return $result;
    }

    /*
     * 向指定的手机号发送飞信
     */
    public function send($tomobile, $message, $repeat)
    {
	if ($repeat == '')
	{
		$repeat=1;
	}
        if ($tomobile == $this->mobile)  // 判断是给自己发的还是给好友发，if 成立则给自己发
        {
		while($repeat > 0)
		{
			$result = $this->toMyself($message);
			$repeat--;
		}
            	$result = "to_myself_success";
		return $result;
        }
        else				// 给别人发
        {
        	$uid = $this->getUid($tomobile);
		while($repeat > 0)
		{
			$result = ($uid == '' ? '' : $this->toUid($uid, $message));
			$repeat--;
		}
            	$result = "to_uid_success";
            	return $result;
        }
	return $result;
    }

    /*
     * 获取飞信ID
     */
    protected function getUid($tomobile)
    {
        if (empty($this->uids[$tomobile]))
        {
            $uri = '/im/index/searchOtherInfoList.action';
            $data = 'searchText='.$tomobile;
            $result = $this->postWithCookie($uri, $data);
            preg_match('/toinputMsg\.action\?touserid=(\d+)/si', $result, $matches);	// 匹配
            $this->uids[$tomobile] = isset($matches[1]) ? $matches[1] : '';
        }   
        return $this->uids[$tomobile];
    }

    /*
     * 获取csrfToken，给好友发飞信时需要这个字段
     */
    protected function getCsrfToken($uid)
    {
        if ($this->csrfToten == null)
        {
            $uri = '/im/chat/toinputMsg.action?touserid='.$uid;
            $result = $this->postWithCookie($uri, '');
            preg_match('/name="csrfToken".*?value="(.*?)"/', $result, $matches);
            $this->csrfToten = isset($matches[1]) ? $matches[1] : '';
        }
        return $this->csrfToten;
    }

    /*
     * 向好友发送飞信
     */
    protected function toUid($uid, $message)
    {
        $uri = '/im/chat/sendMsg.action?touserid='.$uid;
        $csrfToken = $this->getCsrfToken($uid);
        $data = 'msg='.urlencode($message).'&csrfToken='.$csrfToken;
        $result = $this->postWithCookie($uri, $data);
        return $result;
    }

    /*
     * 给自己发飞信
     */
    protected function toMyself($message)
    {
        $uri = '/im/user/sendMsgToMyselfs.action';
        $result = $this->postWithCookie($uri, 'msg='.urlencode($message));
        return $result;
    }

    /*
     * 退出飞信
     */
    protected function logout()
    {
        $uri = '/im/index/logoutsubmit.action';
        $result = $this->postWithCookie($uri, '');
        return $result;
    }

    /*
     * 携带Cookie向f.10086.cn发送POST请求
     */
    protected function postWithCookie($uri, $data)
    {
        $fp = fsockopen('f.10086.cn', 80, $errno , $errstr,10);
	if(!$fp)
	{
		$result = 'socket error'.$errstr;
		echo $result;
	}
	else
	{
		fputs($fp, "POST $uri HTTP/1.1\r\n");
		fputs($fp, "Host: f.10086.cn\r\n");
		fputs($fp, "Cookie: {$this->cookie}\r\n");
		fputs($fp, "Content-Type: application/x-www-form-urlencoded\r\n");  // 这是标准的编码格式
		fputs($fp, "User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:14.0) Gecko/20100101 Firefox/14.0.1\r\n");
		fputs($fp, "Content-Length: ".strlen($data)."\r\n");
		fputs($fp, "Connection: close\r\n\r\n");
		fputs($fp, $data);
		$result = '';
		while (!feof($fp))
		{
			$result .= fgets($fp);
		}
	}
        fclose($fp);	
        return $result;
    }
}
