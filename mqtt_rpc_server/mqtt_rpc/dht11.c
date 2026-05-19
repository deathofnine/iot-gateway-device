#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h> 
static int fd;
static char g_humi,g_temp;
pthread_mutex_t data_mutex;
void *dht11_thread(void *arg)
{
        // ====================== 【信号屏蔽：核心代码】======================
    sigset_t mask;      // 定义信号集合
    sigfillset(&mask);  // 把所有信号都加入屏蔽
    pthread_sigmask(SIG_BLOCK, &mask, NULL); // 线程屏蔽所有信号
    // ==================================================================
    char buf[2];

    // 线程启动先等一下，让驱动准备好（关键！）
    usleep(500000);

    while (1)
    {
        int res = read(fd, buf, 2);
        printf("%d",fd);
        if (res == 2)
        {
            pthread_mutex_lock(&data_mutex);
            g_humi = buf[0];
            g_temp = buf[1];
             pthread_mutex_unlock(&data_mutex);
            printf("读取成功：%d, %d\n", g_humi, g_temp);
            fflush(stdout);
            sleep(2);
        }
        else
        {
            // 失败也不要疯狂读
            printf("读取失败，等待重试\n", g_humi, g_temp);
        }
        // ✅ 核心：每次都等，驱动才不会崩
    }
    return NULL;
}

void mydht11_init(void)
{
    pthread_mutex_init(&data_mutex, NULL);
    pthread_t tid1;
    fd = open("/dev/mydht11", O_RDWR | O_NONBLOCK);
    printf("fd=%d\n",fd);
    if (fd >= 0)
    {
        int ret=pthread_create(&tid1,NULL,dht11_thread,NULL);//创建线程
        if (ret == 0)
        {
            printf("dht11_thread create successful\n");
        }
        else
        {
            printf("dht11 thread create fault! err=%d\n", ret);
        }
    }
}

int mydht11_read(char *humi, char *temp)
{
        pthread_mutex_lock(&data_mutex);
        *humi = g_humi;
        *temp = g_temp;
        pthread_mutex_unlock(&data_mutex);
        return 0;
}

void dht_deinit(void)
{
    if(fd >= 0) close(fd);
    pthread_mutex_destroy(&data_mutex);
}
