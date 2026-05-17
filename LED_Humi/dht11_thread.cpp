//

#include "dht11_thread.h"
#include <QDebug>
#include "rpc_client.h"
    // 线程核心运行函数（自动调用）
DHT11Thread::DHT11Thread(QObject *parent)
    : QThread(parent)
{
}
DHT11Thread::~DHT11Thread()
{
    stop();
    wait();  // 等待线程结束
}
/*void DHT11Thread::setLabels(QLabel *humiLabel, QLabel *tempLabel)
{
    m_humiLabel = humiLabel;
    m_tempLabel = tempLabel;
}*/
void DHT11Thread::stop()
{
    m_stop = true;
}
void DHT11Thread::run()
{
    int ret;
    char humi;
    char temp;
//   char buf[20];
    int consecutive_failures = 0;


    while (!m_stop) 
     {
        ret = rpc_dht11_read(&humi, &temp);
         if (ret == 0) {
            emit dataReady((int)temp, (int)humi);
/*            if (m_humiLabel) {
                sprintf(buf, "%d%%", (int)humi);
                m_humiLabel->setText(buf);
            }
            if (m_tempLabel) {
                sprintf(buf, "%d℃", (int)temp);
                m_tempLabel->setText(buf);
            }*/
            consecutive_failures = 0;
            qDebug() << "DHT11 data: temp=" << (int)temp << "°C, humi=" << (int)humi << "%";
            
            msleep(2000);
        } else {
       //     emit readFailed();
            consecutive_failures++;
            qDebug() << "Read failed, consecutive:" << consecutive_failures;

        }


    }
}

