#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <QDebug>
int fd1;
void mydht11_init(void)
{
    fd1 = open("/dev/mydht11",O_RDWR | O_NONBLOCK);
    if (fd1 == -1) {
        qDebug() << "open /dev/mydht11 failed:" << strerror(errno);
    } else {
        qDebug() << "open /dev/mydht11 success, fd=" << fd1;
    }
}
void mydht11_close(int i)
{
    if (i >= 0) {
        close(i);
        i = -1;
        qDebug() << "DHT11 device closed";
    }
}
int mydht11_read(char *humi, char *temp)
{
    if (fd1 == -1) {
        return -1;
    }

    char buf[2];
    ssize_t ret = read(fd1, buf, 2);

    if (ret == 2) {
        *humi = buf[0];
        *temp = buf[1];
        qDebug() << "Read SUCCESS: humi=" << (int)*humi << " temp=" << (int)*temp;
        return 0;
    } else if (ret < 0 && errno != EAGAIN) {
        // EAGAIN 是非阻塞模式下没有数据时的正常情况，不打印错误
        qDebug() << "Read FAILED: ret=" << ret << " errno=" << errno << " (" << strerror(errno) << ")";
    }

    return -1;
}
