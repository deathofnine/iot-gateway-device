#include <cJSON.h>
#include <jsonrpc-c.h>
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
#include "rpc.h"
#include "led.h"
#include "dht11.h"
static struct jrpc_server my_server;

// ------------------------------
// RPC 方法：LED控制
// ------------------------------
/*參數：{"prarams"：[0|1]}*/
cJSON * server_led_control(jrpc_context * ctx, cJSON * params, cJSON *id) {
    
    cJSON * status = cJSON_GetArrayItem(params,0);
    led_control(status->valueint);
    //返回0表示执行成功
    return cJSON_CreateNumber(0);
}

// ------------------------------
// RPC 方法：读取DHT11
// ------------------------------
/*參數：{"prarams"：null}*/
cJSON * server_dht11_read(jrpc_context * ctx, cJSON * params, cJSON *id) {
    int array[2]={0};
     
    while(0!=mydht11_read((char *)&array[0],(char *)&array[1]));


//-----------------------
//返回形式为{ "result": [44,25] }
// ---------------------
    return cJSON_CreateIntArray(array,2);
}
/* 连接RPC Server
 * 返回值: (>0)socket, (-1)失败
 */

// ------------------------------
// RPC 服务初始化
// ------------------------------
int RPC_Server_Init(void) 
{
    int err;
    
    err = jrpc_server_init(&my_server, PORT);
    if (err)
    {
        printf("jrpc_server_init err : %d\n", err);
    }
    //注册函数
    jrpc_register_procedure(&my_server, server_led_control, "led_control", NULL );
    jrpc_register_procedure(&my_server, server_dht11_read, "dht11_read", NULL );
    //启动服务器，自动监听所有ip
    jrpc_server_run(&my_server);
    jrpc_server_destroy(&my_server);

    return 0;
}

static void print_usage(char *exec)
{
    printf("Usage:\n");
    printf("%s \n", exec);
    printf("%s &\n", exec);
}

int main(int argc, char **argv)
{
    print_usage(argv[0]);
    led_init();
    mydht11_init();
    RPC_Server_Init();
    return 0;
}


