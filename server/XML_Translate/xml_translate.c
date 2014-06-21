/* 
* Author:		xusongqi@live.com
* 
* Created Time: 2014年05月31日 星期六 11时17分15秒
* 
* FileName:     xml_analyse.h
* 
* Description:  -lxml2
*
*/


#include "../include/ftp_server.h"
#include "../include/xml_translate.h"


/*———————————————————————————————————————
 * 
 * 函数：xml_translate
 *
 * 作用：解析xml包
 *
 *—————————————————————————————————————— */
//void xml_translate(cnt_fd)

int msg_analyse(const char * xml_msg, int msg_len)
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


	/* 对信息头（root元素）进行匹配
	 *
	 * xml_root->name类型为xmlChar，本质上是unsigned char
	 * */
	if( !xmlStrcmp(xml_root->name, BAD_CAST "login") )
	{
		//DOTO
		printf("user: %s\npwd: %s\n", xml_root->xmlChildrenNode->name, xml_root->xmlChildrenNode->next->name);
	}
	else
	{	return ERRX_TAG_NOT_MATCH;}
	
	xmlFreeDoc(xml_fd);
	xmlCleanupParser();

	return 0;
}
