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
        "username": "6a00bc177f2e6c302f6f8843_dht11_led_001",就是device_id
        "password": "55281965568752b8594d6549cd989ade56b08019b0d0b0f83c8429c566b79e90",
        "clientid": "6a00bc177f2e6c302f6f8843_dht11_led_001_0_0_2026051017",
        "URI": "b1d7c0faf7.st1.iotda-device.cn-north-4.myhuaweicloud.com",
        "ProductID":"6a00bc177f2e6c302f6f8843"
        "DeviceName":"dht11_led_001"
        "port": 8883,
        "protocol": "MQTTS"
       
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
      //将字符串转换为cJSON
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
