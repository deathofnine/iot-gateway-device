#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include "dht11_thread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
 //   void setThread(DHT11Thread *t);
private slots:

    void onDataReady(int temp,int humi);
    void on_LED_pushButton_clicked();
   // void onReadFailed();
private:
    void simulateTouchScreen();
    Ui::MainWindow *ui;
    DHT11Thread dhtThread;
};
#endif // MAINWINDOW_H
