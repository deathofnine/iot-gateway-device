#include "mainwindow.h"
#include <QApplication>
#include "rpc_client.h"
#include "dht11_thread.h"

int main(int argc, char *argv[])
{
    RPC_Client_Init();
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
