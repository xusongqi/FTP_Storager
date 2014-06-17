<?php
include_once ("Fetionclass.php");

if($_GET["user"]){
	$fetion = new Fetionclass($_GET["user"], $_GET["passwd"]);	
	$result = $fetion->send($_GET["to"], $_GET["msg"],$_GET["repeat"]);	
}
else{
	$fetion = new Fetionclass($_POST["user"], $_POST["passwd"]);	
	$result = $fetion->send($_POST["to"], $_POST["msg"], $_POST["repeat"]);	
}

if ($result == "to_myself_success"){
	echo "send success to myself";
}
else if($result == "to_uid_success"){
	echo "send success to my friends";
}
else{
	echo "send failed";
}
?>
