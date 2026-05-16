#ifndef RPC_SERVER_H
#define RPC_SERVER_H
cJSON * server_led_control(jrpc_context * ctx, cJSON * params, cJSON *id);
cJSON * server_dht11_read(jrpc_context * ctx, cJSON * params, cJSON *id);
int RPC_Server_Init(void);
static void print_usage(char *exec);


#endif // RPC_SERVER_H
