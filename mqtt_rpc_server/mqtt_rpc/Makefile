TARGET=mqtt_rpc_server
CC=arm-buildroot-linux-gnueabihf-gcc


TOP_DIR=$(shell pwd)/../
LIBEV_DIR=${TOP_DIR}/libev/tmp/
JSONRPC_DIR=${TOP_DIR}/jsonrpc-c/tmp/
PAHO_MQTT_DIR=${TOP_DIR}/paho.mqtt.c/

CFLAGS += -I${PAHO_MQTT_DIR}/build/src -O
CFLAGS += -I${LIBEV_DIR}/include
CFLAGS += -I${JSONRPC_DIR}/include


LDFLAGS += -L${PAHO_MQTT_DIR}/build/src -lpaho-mqtt3c
LDFLAGS += ${JSONRPC_DIR}/lib/libjsonrpcc.a 
LDFLAGS += ${LIBEV_DIR}/lib/libev.a -lm -lpthread
LDFLAGS += -lm -lpthread

c_files = mqtt_rpc_server.c cfg.c cJSON.c led.c dht11.c

all:
	${CC} ${CFLAGS} -o ${TARGET} ${c_files} ${LDFLAGS}

clean:
	rm -f *.o ${TARGET}
	
