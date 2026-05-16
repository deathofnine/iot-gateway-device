#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H
int rpc_led_control(int on);
int rpc_dht11_read(char *humi,char *temp);
int RPC_Client_Init(void);
#endif // RPC_CLIENT_H
