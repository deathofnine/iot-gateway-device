#ifndef DHT11_THREAD_H
#define DHT11_THREAD_H
#include <QThread>
#include <QDebug>
#include <QLabel>
class DHT11Thread : public QThread
{
    Q_OBJECT
public:
    // 必须有构造函数声明
    explicit DHT11Thread(QObject *parent = nullptr);
    ~DHT11Thread();  // 添加析构函数
//    void setLabels(QLabel *humiLabel, QLabel *tempLabel);
    void stop();   
protected:
    void run() override;
signals:
    void dataReady(int temp,int humi);
    void readFailed();
private:  
    bool m_stop = false; 
//    QLabel *m_humiLabel;
//    QLabel *m_tempLabel;
};
#endif // DHT11_THREAD_H
