#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QTime>
#include <QString>
#include<QFileDialog>
#include <tool.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QSerialPort *serial = new QSerialPort();
    QByteArray buf;
    QByteArray buf2;
    ~MainWindow();

    \

private slots:
    void ReadData();

    void dataPack(QByteArray inData, QByteArray &packBuff);

    void StringToHex(QString str, QByteArray &senddata);

    char ConvertHexChar(char ch);

    void FileOpen(QString &lineStr);

    void on_OpenSerialButton_clicked();

    void on_SearchSerialButton_clicked();

    void on_SwitchSafeModButton_clicked();

    void on_KeyDownloadButton_clicked();

    void on_CADownloadButton_clicked();

    void on_GenelKeyButton_clicked();

    void on_SM2SignButton_clicked();

    void on_SM2CheckButton_clicked();

    void on_encryptionButton_clicked();

    void on_decryptionButton_clicked();

    void on_KeyDownloadButton_2_clicked();

    void on_KekDownloadButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
