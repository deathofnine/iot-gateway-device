#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
static int fd;
static char g_humi,g_temp;
pthread_mutex_t data_mutex;  
//------------------------
//DHT11采集线程
//-----------------------
void *dht11_thread(void *arg)
{
    char buf[2];
    while (1)
    {
        if (2 == read(fd, buf, 2))
        {
            pthread_mutex_lock(&data_mutex);
            g_humi = buf[0];
            g_temp = buf[1];
            pthread_mutex_unlock(&data_mutex);
            sleep(1);
        }
    }

}
}
//-------------------------
//初始化
//-----------------------
void mydht11_init(void)
{
    pthread_t tid1;
    pthread_mutex_init(&data_mutex, NULL);
    fd = open("/dev/mydht11", O_RDWR | O_NONBLOCK);
    if (fd >= 0)
    {
        int ret = pthread_create(&tid1,NULL,dht11_thread,NULL);//创建线程
    }
}
//-------------------------
//对外读取接口
//-----------------------
int mydht11_read(char *humi, char *temp)
{
        pthread_mutex_lock(&data_mutex);
        *humi = g_humi;
        *temp = g_temp;
        pthread_mutex_unlock(&data_mutex);
        return 0;
}


// -----------------------
//统一销毁释放
//-------------------------
void mydht11_deinit(void)
{

    if(fd >= 0)
    {
        close(fd);
        fd = -1;
    }
    pthread_mutex_destroy(&data_mutex);
}
