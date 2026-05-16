#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <QDebug>
int fd=-1;

void led_init(void)
{
    if (fd >= 0)
         close(fd);
    fd = open("/dev/100ask_led", O_RDWR);
    qDebug() << "LED init, fd =" << fd;
    if (fd == -1)
        {

            qDebug()<<"open /dev/100ask_led failed";
        }
    qDebug() << "LED init success!";

}

void led_control(int on)
{
    if (fd == -1)
        {
            qDebug() << "LED control failed: fd is invalid (设备未打开)";
            led_init();  // 尝试重新初始化
            return;
        }
        else{
            qDebug()<<"正在嘗試點燈";
        }
    char buf[2];
    buf[0] = 0;

    if (on)
        buf[1] = 0;
    else
        buf[1] = 1;

    ssize_t ret = write(fd, buf, 2);
    if (ret != 2)
    {
        qDebug() << "LED write failed: ret=" << ret << ", errno=" << strerror(errno);
    }
    else
    {
        qDebug() << "LED control: " << (on ? "亮" : "灭");
    }
}
