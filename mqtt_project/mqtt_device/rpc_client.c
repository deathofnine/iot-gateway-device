
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "cJSON.h"
#include "rpc.h"
static int g_iSocketClient;

int rpc_led_control(int on)
{
    char buf[100];
    int iLen;
    int ret=-1;
    int iSocketClient=g_iSocketClient;
    sprintf(buf, "{\"method\": \"led_control\", \"params\": [%d], \"id\": \"2\" }", on);
    iLen = send(iSocketClient, buf, strlen(buf), 0);
    if (iLen ==  strlen(buf))
    {
        while (1)
        {
            iLen = read(iSocketClient, buf, sizeof(buf));
            buf[iLen] = 0;
            if (iLen == 1 && (buf[0] == '\r' || buf[0] == '\n'))
                continue;
            else
                break;
        }

        if (iLen > 0)
        {
            cJSON *root = cJSON_Parse(buf);
            cJSON *result = cJSON_GetObjectItem(root, "result");
            ret = result->valueint;
            cJSON_Delete(root);
            return ret;
        }
        else
        {
            printf("read rpc reply err : %d\n", iLen);
            return -1;
        }
    }
    else
    {
        printf("send rpc request err : %d, %s\n", iLen, strerror(errno));
        return -1;
    }
}

int rpc_dht11_read( char *humi,char *temp)
{
    char buf[300];
    int iLen;

    sprintf(buf, "{\"method\": \"dht11_read\"," \
                   "\"params\":[0], \"id\": \"2\" }");
    int iSocketClient=g_iSocketClient;
    iLen = send(iSocketClient, buf, strlen(buf), 0);
    if (iLen ==  strlen(buf))
    {
        while (1)
        {
            iLen = read(iSocketClient, buf, sizeof(buf));
            buf[iLen] = 0;
            if (iLen == 1 && (buf[0] == '\r' || buf[0] == '\n'))
                continue;
            else
                break;
        }

        if (iLen > 0)
        {
            cJSON *root = cJSON_Parse(buf);
            cJSON *result = cJSON_GetObjectItem(root, "result");
            if (result)
            {
                cJSON * a = cJSON_GetArrayItem(result,0);
                cJSON * b = cJSON_GetArrayItem(result,1);
                *humi=a->valueint;
                *temp=b->valueint;


                cJSON_Delete(root);
                return 0;
            }
            else
            {
                cJSON_Delete(root);
                return -1;
            }
        }
        else
        {
            printf("read rpc reply err : %d\n", iLen);
            return -1;
        }
    }
    else
    {
        printf("send rpc request err : %d, %s\n", iLen, strerror(errno));
        return -1;
    }
}

/* 连接RPC Server
 * 返回值: (>0)socket, (-1)失败
 */
int RPC_Client_Init(void)
{
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;
    int iRet;

    iSocketClient = socket(AF_INET, SOCK_STREAM, 0);
    tSocketServerAddr.sin_family      = AF_INET;  //IPv4
    tSocketServerAddr.sin_port        = htons(PORT);  //端口号
    //tSocketServerAddr.sin_addr.s_addr = INADDR_ANY; 
    inet_aton("127.0.0.1", &tSocketServerAddr.sin_addr); //服务器IP=本机 
    memset(tSocketServerAddr.sin_zero, 0, 8);


    iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet)
    {
        printf("connect error!\n");
        return -1;
    }
    g_iSocketClient=iSocketClient;
    return iSocketClient;
}

static void print_usage(char *exec)
{
    printf("Usage:\n");
    printf("%s led <0|1>\n", exec);
    printf("%s dht11\n", exec);
}

