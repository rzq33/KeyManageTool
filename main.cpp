#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug()<<"----程序启动----";
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("密钥下载工具");
    w.show();

    return a.exec();
}
