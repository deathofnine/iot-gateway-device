/*******************************************************************************
 * Copyright (c) 2012, 2023 IBM Corp., Ian Craggs
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"
#include "cfg.h"
#include "rpc_client.h"
#include "cJSON.h"

#define QOS         1
#define TIMEOUT     10000L

#define USER_NAME "100ask"
#define PASSWORD  "100ask"

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int on;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    cJSON *root=cJSON_Parse((char*)message->payload);
    cJSON *ptTemp=cJSON_GetObjectItem(root, "paras");
    if(ptTemp)
    {
        cJSON *led1=cJSON_GetObjectItem(ptTemp, "status");
        if(led1)
        {
            if(strcmp(led1->valuestring, "ON")==0)
                on=1;
            else
                on=0;
            rpc_led_control(on);
        }
    }
    cJSON_Delete(root);

    /*{
  "paras": {
    "status": "ON"
  },
  "service_id": "DeviceData",
  "command_name": "CONTROL_LED1"
}*/
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    if (cause)
    	printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    /*读配置文件 /etc/*/
    char URI[1000];
    char clientid[1000];
    char username[1000];
    char password[1000];
    char ProductID[1000];
    char DeviceName[1000];

    char address[1000];
    char pub_topic[1000];//发布: $oc/devices/{deviceId}/sys/properties/report
    char sub_topic[1000];//订阅: $oc/devices/{deviceId}/sys/commands/#
    if(0!=read_cfg(URI, clientid, username, password, ProductID, DeviceName))
    {
        printf("read cfg err\n");
        return -1;
    }
    printf("Config loaded:\n");
    printf("  URI: %s\n", URI);
    printf("  clientid: %s\n", clientid);
    printf("  username: %s\n", username);
    printf("  password: %s\n", password);
    printf("  ProductID: %s\n", ProductID);
    printf("  DeviceName: %s\n", DeviceName);


    /*init rpc: connect rpc server*/

    sprintf(address, "tcp://%s:1883", URI);
    sprintf(pub_topic,"$oc/devices/%s/sys/properties/report",username);
    sprintf(sub_topic,"$oc/devices/%s/sys/commands/#",username);

    
    if(-1==RPC_Client_Init())
    {
        printf("RPC_Client_Init err\n");
        return -1;
    }
    else
    {
        printf("RPC client initialized successfully\n");
    }

    printf("\nMQTT connection parameters:\n");
    printf("  Address: %s\n", address);
    printf("  ClientID: %s\n", clientid);
    printf("  Username: %s\n", username);
    printf("  Password: %s\n", password);
    printf("  KeepAliveInterval: %d\n", conn_opts.keepAliveInterval);
    printf("  CleanSession: %d\n", conn_opts.cleansession);
    printf("\nCreating MQTT client...\n");

    
    if ((rc = MQTTClient_create(&client,address , clientid,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to create client, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto exit;
    }
    printf("MQTT client created successfully\n");
    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }
    conn_opts.MQTTVersion = 4;
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;
    conn_opts.username = username;
    conn_opts.password = password;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        rc = EXIT_FAILURE;
        goto destroy_exit;
    }

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", sub_topic, clientid, QOS);
    if ((rc = MQTTClient_subscribe(client, sub_topic, QOS)) != MQTTCLIENT_SUCCESS)
    {
    	printf("Failed to subscribe, return code %d\n", rc);
    	rc = EXIT_FAILURE;
    }
    else
    {
    	int ch;
        int cnt = 0;
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        char buf[1000];
        MQTTClient_deliveryToken token;
        
    	while (1)
    	{
        	/*rpc_dht11_read*/
            char humi;
            char temp;
            while(0!=rpc_dht11_read(&humi,&temp));

            sprintf(buf, "{\"services\":[{\"service_id\":\"DeviceData\",\"properties\":{\"temp_value\":%d,\"humi_value\":%d}}]}", temp, humi);
            pubmsg.payload = buf;
            pubmsg.payloadlen = (int)strlen(buf);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;
            /*发布消息*/
            if ((rc = MQTTClient_publishMessage(client, pub_topic, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
            {
                 printf("Failed to publish message, return code %d\n", rc);
                 continue;
            }
            
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
            printf("Message with delivery token %d delivered\n", token);    
            sleep(5);                    
    	} 

        if ((rc = MQTTClient_unsubscribe(client, sub_topic)) != MQTTCLIENT_SUCCESS)
        {
        	printf("Failed to unsubscribe, return code %d\n", rc);
        	rc = EXIT_FAILURE;
        }
    }

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
    	printf("Failed to disconnect, return code %d\n", rc);
    	rc = EXIT_FAILURE;
    }
destroy_exit:
    MQTTClient_destroy(&client);
exit:
    return rc;
}
