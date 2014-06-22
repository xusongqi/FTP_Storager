/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年05月31日 星期六 11时17分15秒
* 
* FileName:     xml_analyse.h
* 
* Description:  -lxml2
*
* tag：	xml_root->name类型为xmlChar，本质上是unsigned char
*
*/


#include "../include/ftp_server.h"
#include "../include/xml_translate.h"
#include "../include/recv_service.h"

/*———————————————————————————————————————
 * 
 * 函数：xml_translate
 *
 * 作用：解析xml包
 *
 *—————————————————————————————————————— */
//void xml_translate(cnt_fd)

int msg_analyse(int cnt_fd, const char * xml_msg, int msg_len)
{
	//char		n = 0;				//子节点计数器
	xmlDocPtr	xml_fd = NULL;		//xml描述符
	xmlNodePtr	xml_root = NULL,	//xml的root节点
				curNode = NULL;		//xml的子节点

	xmlKeepBlanksDefault(0);//忽视xml中的空格，避免读出node = text的空节点

	/*读取msg内容*/
	xml_fd = xmlParseMemory(xml_msg, msg_len);
	if(xml_fd == NULL)
	{
		printf("read file error\n");
		return ERRX_READ_FILE;
	}

	/*获取msg中的root元素*/
	xml_root = xmlDocGetRootElement(xml_fd);
	if(xml_root == NULL)
	{
		printf("get element error\n");
		return ERRX_GET_ELEMENT;
	}


	/* 匹配xml的根结点
	 *		login
	 *		signin
	 *		upload
	 *		download
	 * */
	if( !xmlStrcmp(xml_root->name, BAD_CAST "login") )
	{	//root == "login"
		printf("login user: %s\nlogin pwd: %s\n", 
					xmlNodeGetContent(xml_root->xmlChildrenNode), 
					xmlNodeGetContent(xml_root->xmlChildrenNode->next));
	}
	else if( !xmlStrcmp(xml_root->name, BAD_CAST "signin") )
	{	//root == "signin"
		//获取用户名和密码
		printf("signin user: %s\nsignin pwd: %s\n", 
					xmlNodeGetContent(xml_root->xmlChildrenNode), 
					xmlNodeGetContent(xml_root->xmlChildrenNode->next));
	}
	else if( !xmlStrcmp(xml_root->name, BAD_CAST "upload") )
	{	//root == "upload"
		recv_files(cnt_fd);
	}
	else if( !xmlStrcmp(xml_root->name, BAD_CAST "download") )
	{	//root == "download"
		//send_files();
	}
	else
	{	return ERRX_TAG_NOT_MATCH;}
	
	xmlFreeDoc(xml_fd);
	xmlCleanupParser();

	return 0;
}
