#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "cJSON.h"
#include "cfg.h"
/*
    配置文件内容格式
	{
  	  "username": "6a00bc177f2e6c302f6f8843_dht11_led_001",
  	  "password": "e905968cd5723ff0a8e0d16363af211b3babe18a121bb6637d3c19416f58fbb3",
  	  "clientid": "6a00bc177f2e6c302f6f8843_dht11_led_001_0_0_2026051118",
   	 "URI": "b1d7c0faf7.st1.iotda-device.cn-north-4.myhuaweicloud.com",
   	 "ProductID": "6a00bc177f2e6c302f6f8843",
   	 "DeviceName": "dht11_led_001"
	}

*/
int read_cfg(char *URI, char *clientid, char *username, char *password, char *ProductID, char *DeviceName)
{
    char buf[1000];
    cJSON *ptTemp;
    int fd = open (CFG_FILE,O_RDONLY);
    if(fd < 0)
        return -1;
    int ret = read (fd,buf,sizeof(buf));
    if(ret<=0)
        return -1;
     cJSON *root=cJSON_Parse(buf);
     ptTemp=cJSON_GetObjectItem(root, "URI");
     if(ptTemp)
     {
        strcpy(URI,ptTemp->valuestring);
     }
        ptTemp=cJSON_GetObjectItem(root, "clientid");
    if(ptTemp)
     {
        strcpy(clientid,ptTemp->valuestring);
     }
     ptTemp=cJSON_GetObjectItem(root, "username");
    if(ptTemp)
     {
        strcpy(username,ptTemp->valuestring);
     }
 
     ptTemp=cJSON_GetObjectItem(root, "password");
    if(ptTemp)
     {
        strcpy(password,ptTemp->valuestring);
     }
     ptTemp=cJSON_GetObjectItem(root, "ProductID");
    if(ptTemp)
     {
        strcpy(ProductID,ptTemp->valuestring);
     }
     ptTemp=cJSON_GetObjectItem(root, "DeviceName");
    if(ptTemp)
     {
        strcpy(DeviceName,ptTemp->valuestring);
     }
     cJSON_Delete(root);
     close(fd);
     return 0;
}
