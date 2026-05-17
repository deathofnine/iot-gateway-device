#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QTest>
#include "dht11_thread.h"
#include "rpc_client.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->LED_pushButton->setEnabled(true);
    ui->LED_pushButton->raise();
    // 1. 固定你的7寸屏大小
    
       this->setFixedSize(1024, 600);

       // 2. 背景 Label 铺满整个窗口
       ui->label_bg->setGeometry(0, 0, 1024, 600);

       // 3. 设置背景图片 + 自动拉伸填满
       QPixmap bg(":/bg/1024_600.jpg");  // 你的图片路径
       ui->label_bg->setPixmap(bg);
       ui->label_bg->setScaledContents(true); // 自动适配屏幕
       ui->LED_pushButton->show();
       ui->LED_pushButton->setAttribute(Qt::WA_TransparentForMouseEvents, false);  // 确保没有透明
        

    connect(&dhtThread, &DHT11Thread::dataReady, this, &MainWindow::onDataReady);
//    connect(&dhtThread, &DHT11Thread::readFailed,this, &MainWindow::onReadFailed);

    // ====================== 温度湿度折线图 ======================
    ui->plot->addGraph(); // 温度曲线(0号)
    ui->plot->graph(0)->setPen(QPen(QColor(0, 160, 230), 2));
    ui->plot->graph(0)->setName("温度");

    ui->plot->addGraph(); // 湿度曲线(1号)
    ui->plot->graph(1)->setPen(QPen(QColor(255, 80, 80), 2));
    ui->plot->graph(1)->setName("湿度");

    // X/Y轴说明
    ui->plot->xAxis->setLabel("时间");
    ui->plot->yAxis->setLabel("数值");

    // 显示图例
    ui->plot->legend->setVisible(true);

    // 允许鼠标拖动、缩放
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // 设置Y轴范围
    ui->plot->yAxis->setRange(0, 100);
    dhtThread.start();
    dhtThread.setPriority(QThread::HighPriority);
}
void MainWindow::updatePlot(double temp, double humi)
{
    static int x = 0;

    // 添加数据
    ui->plot->graph(0)->addData(x, temp);
    ui->plot->graph(1)->addData(x, humi);

    // 只显示最近 30 个点
    ui->plot->xAxis->setRange(x - 30, x);

    // 重绘
    ui->plot->replot();
    x++;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::onReadFailed()
{
    // DHT11读取失败时，模拟触摸屏事件来帮助恢复
    qDebug() << "DHT11 read failed, simulating touch screen...";
    simulateTouchScreen();
}*/
/*void MainWindow::simulateTouchScreen()
{
    // 尝试打开触摸屏设备
    int fd = open("/dev/input/event1", O_RDWR);
    if (fd < 0) {
        fd = open("/dev/input/event0", O_RDWR);
        if (fd < 0) {
            qDebug() << "Cannot open touch screen device";
            return;
        }
    }

    struct input_event ev;

    // 发送一个空的同步事件，产生中断
    memset(&ev, 0, sizeof(ev));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;

    if (write(fd, &ev, sizeof(ev)) != sizeof(ev)) {
        qDebug() << "Failed to write touch event";
    }

    ::close(fd);
}
*/
void MainWindow::onDataReady(int temp,int humi)
{
    ui->humi->setText(QString("湿度：%1 %RH").arg(humi));
    ui->temp->setText(QString("温度：%1 ℃").arg(temp));
    updatePlot(temp, humi);
}

void MainWindow::on_LED_pushButton_clicked()
{
    static int status=1;
    qDebug()<<"LED clicked";
    rpc_led_control(status);
    status = !status;
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}
