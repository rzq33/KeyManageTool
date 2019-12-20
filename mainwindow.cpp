#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->BitBox->setEnabled(false);
    ui->ParityBox->setEnabled(false);
    ui->StopBox->setEnabled(false);
    ui->SwitchSafeModButton->setEnabled(false);

    ui->KeyDownloadButton->setEnabled(false);
    ui->KeyDownloadButton_2->setEnabled(false);
    ui->KekDownloadButton->setEnabled(false);
    ui->CADownloadButton->setEnabled(false);
    ui->GenelKeyButton->setEnabled(false);
    ui->cerSaveButton->setEnabled(false);
    ui->cerReadButton->setEnabled(false);
    ui->cerDeleteButton->setEnabled(false);
    ui->cerDelAllButton->setEnabled(false);
    ui->SM2SignButton->setEnabled(false);
    ui->SM2CheckButton->setEnabled(false);
    ui->encryptionButton->setEnabled(false);
    ui->decryptionButton->setEnabled(false);
    //设置各按钮初始状态
    //查找可用的串口
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))//读写方式打开串口
        {
            ui->PortBox->addItem(serial.portName());//添加串口号到portbox
            serial.close();//关闭串口等待人为打开
        }
    }
    qDebug()<<"初始化查找可用串口成功";
    //ui->textEdit->insertPlainText(tr("初始化成功=>>"));
    //ui->textEdit->insertPlainText("\n");

    //设置波特率下拉菜单默认显示第4项，即115200
    ui->BaudBox->setCurrentIndex(4);

}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief on_OpenSerialButton_clicked
 * @a 判断串口的开关状态，并执行相应操作
 */
void MainWindow::on_OpenSerialButton_clicked()
{
    if(ui->OpenSerialButton->text() == tr("打开串口"))
    {
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //设置波特率
        switch (ui->BaudBox->currentText().toInt())
        {
        case 9600:
            serial->setBaudRate(QSerialPort::Baud9600);
            break;
        case 19200:
            serial->setBaudRate(QSerialPort::Baud19200);
            break;
        case 38400:
            serial->setBaudRate(QSerialPort::Baud38400);
            break;
        case 57600:
            serial->setBaudRate(QSerialPort::Baud57600);
            break;
        case 115200:
            serial->setBaudRate(QSerialPort::Baud115200);
            break;
        case 1500000:
            serial->setBaudRate(QSerialPort::Baud1500000);
            break;
        default:
            break;
        }
        //设置数据位数
        switch (ui->BitBox->currentText().toInt())
        {
        case 8:
            serial->setDataBits(QSerialPort::Data8);//设置数据位8
            break;
        default:
            break;
        }
        //设置校验位
        switch (ui->ParityBox->currentText().toInt())
        {
        case 0:
            serial->setParity(QSerialPort::NoParity);
            break;
        default:
            break;
        }
        //设置停止位
        switch (ui->BitBox->currentText().toInt())
        {
        case 1:
            serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
            break;
        default:
            break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //连接信号槽
        QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(ReadData()));

        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->SwitchSafeModButton->setEnabled(true);
        ui->SearchSerialButton->setEnabled(false);//串口打开后，串口相关设置不可更改，功能相关按钮设置可用
        ui->OpenSerialButton->setText(tr("关闭串口"));

        //提示打开成功
        qDebug()<<"打开串口设置成功";
    }
    else if(ui->OpenSerialButton->text() == tr("关闭串口"))
    {
        //关闭串口
        serial->clear();
        serial->close();

        //恢复菜单使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->SwitchSafeModButton->setEnabled(false);

        ui->SearchSerialButton->setEnabled(true);
        ui->OpenSerialButton->setText(tr("打开串口"));
        qDebug()<<"关闭串口设置成功";
    }
}

/**
 * @brief on_SearchSerialButton_clicked
 * @a 重新查找可用串口（手动刷新串口列表，程序不设置自动刷新）
 */
void MainWindow::on_SearchSerialButton_clicked()
{
    ui->PortBox->clear();
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))//读写方式打开串口
        {
            ui->PortBox->addItem(serial.portName());//重新查找可用串口号，添加到portbox
            serial.close();//关闭串口等待人为打开
        }
    }
    qDebug()<<"查找可用串口成功";
}

/**
 * @brief 读取串口收到的数据
 */
void MainWindow::ReadData()
{
    buf.clear();
    qint8 outChar;
    QString str;
    qDebug()<<serial->bytesAvailable();  //显示缓冲区接收到的数据长度
    buf = serial->readAll();

    if (0x02==buf[0]  && buf[buf.length()-1] ==0x03)
    {
        buf2.clear();
    }
    else if (0x02==buf[0]  && buf[buf.length()-1] != 0x03)
    {
        buf2 = buf;
        buf.clear();
    }
    else if (buf[0] != 0x02 &&  0x03 == buf[buf.length()-1])
    {
        buf2 +=buf;
    }
    else if(buf[0] != 0x02 && buf[buf.length()-1] != 0x03)
    {
        buf2 += buf;
        buf.clear();  //每次接受完后应把临时储存数据的变量清空
    }                 //数据出现接收分段的情况，组包，使用全局变量储存，待数据接收完整再打印

//    QDataStream out(&buf,QIODevice::ReadWrite);
//    ui->textEdit->insertPlainText(tr("读取信息为=>>"));
//    ui->textEdit->insertPlainText("\n");
//    while(!out.atEnd())
//    {
//        outChar = 0;
//        out>>outChar;   //每字节填充一次，直到结束
//        //十六进制的转换
//        str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
//        ui->textEdit->insertPlainText(str.toUpper());//大写
//        ui->textEdit->insertPlainText(" ");//每发送两个字符后添加一个空格
//        ui->textEdit->moveCursor(QTextCursor::End);
//    }
//    ui->textEdit->insertPlainText("\n");
}


/**
 * @brief 十进制转十六进制算法
 * @param str
 * @param senddata
 */
void MainWindow::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen;
    hexdatalen = 0;
    int len;
    len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;

    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

/**
 * @brief 十进制转十六进制具体转换函数
 * @param ch
 * @return
 */
char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return ch-ch;//不在0-f范围内的会发送成0
}

/**
 * @brief dataPack 协议打包函数
 * @param inData   入参
 * @param packBuff 出参
 */
void MainWindow::dataPack(QByteArray inData, QByteArray &packBuff)
{

    int i = 0;
    packBuff[0] = 0x02;
    packBuff[1] = 0x00; //发送标志(00 金融读卡器)
    //"银通"内码D2 F8 CD A8   编码方式GB2312
    packBuff[2] = 0xD2;packBuff[3] = 0xF8;packBuff[4] = 0xCD;packBuff[5] = 0xA8;
    //packBuff[6] = (char)((inData.length() & 0xFF)-2);
    for (i=0; i < inData.length(); i++)
    {
        packBuff[6 + i] = inData[i];//填充数据域
    }
    unsigned char crcsrc[packBuff.length()-1];
    for (int j=0;j<packBuff.length()-1;j++) //填充CRC校验数据区
    {
        crcsrc[j]=(unsigned char)packBuff[j+1];
    }
    int crcdst=(int)crc16(crcsrc,sizeof(crcsrc));//crc校验
    QByteArray crcout=intToByte(crcdst);
    packBuff[6+i] = crcout[0];
    packBuff[7+i] = 0x03;
}

/**
 * @brief 硬件安全模块开关
 */
void MainWindow::on_SwitchSafeModButton_clicked()
{

    if(ui->SwitchSafeModButton->text() == tr("打开加密模块"))
    {
        //设置各按钮状态
        ui->KeyDownloadButton->setEnabled(true);
        ui->KeyDownloadButton_2->setEnabled(true);
        ui->KekDownloadButton->setEnabled(true);
        ui->CADownloadButton->setEnabled(true);
        ui->GenelKeyButton->setEnabled(true);
        ui->cerSaveButton->setEnabled(true);
        ui->cerReadButton->setEnabled(true);
        ui->cerDeleteButton->setEnabled(true);
        ui->cerDelAllButton->setEnabled(true);
        ui->SM2SignButton->setEnabled(true);
        ui->SM2CheckButton->setEnabled(true);
        ui->encryptionButton->setEnabled(true);
        ui->decryptionButton->setEnabled(true);
        //
        //
        qDebug()<<"硬件开关打开成功";
        ui->SwitchSafeModButton->setText(tr("关闭加密模块"));
    }
    else if (ui->SwitchSafeModButton->text() == tr("关闭加密模块"))
    {
        //
        //
        //设置各按钮状态
        ui->KeyDownloadButton->setEnabled(false);
        ui->KeyDownloadButton_2->setEnabled(false);
        ui->KekDownloadButton->setEnabled(false);
        ui->CADownloadButton->setEnabled(false);
        ui->GenelKeyButton->setEnabled(false);
        ui->cerSaveButton->setEnabled(false);
        ui->cerReadButton->setEnabled(false);
        ui->cerDeleteButton->setEnabled(false);
        ui->cerDelAllButton->setEnabled(false);
        ui->SM2SignButton->setEnabled(false);
        ui->SM2CheckButton->setEnabled(false);
        ui->encryptionButton->setEnabled(false);
        ui->decryptionButton->setEnabled(false);

        qDebug()<<"硬件开关关闭成功";
        ui->SwitchSafeModButton->setText(tr("打开加密模块"));
    }
}

/**
 * @brief 打开并读取文件函数
 * @param lineStr
 */
void MainWindow::FileOpen(QString &lineStr)
{
    //打开文件选择对话框，选择文件，获取文件路径
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open image file"),
                "./", tr("Key Files(*.txt);;All Files (*.*)"));
    if(fileName.isEmpty())
    {
        qDebug()<<"用户取消操作!!";
    }
    else {
        //ui->textEdit->setText(fileName);//显示文件路径
        QFile f(fileName);
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Fail!!";
            return;
        }
        QTextStream txtInput(&f);
        txtInput.setCodec("utf-8");
        while(!txtInput.atEnd())
        {
            lineStr = lineStr + txtInput.readLine() ;//+ "\n";不加\n
        }
        qDebug()<<lineStr;//显示txt文件内容
        f.close();
    }
}

/**
 * @brief 公钥下载
 */
void MainWindow::on_KeyDownloadButton_clicked()
{

    QString lineStr;
    FileOpen(lineStr);

    QByteArray DataField;
    QByteArray DataLength;
    DataField[0]=0x86; //命令标识
    int length=lineStr.length()+3;
    DataLength=intToByte(length);
    DataField[1]=DataLength[1];
    DataField[2]=DataLength[0];//数据长度位应预留两个字节
    DataField[3]=0x00; //证书管理模式
    DataField[4]=0x00; //证书用途
    DataField[5]=0x00; //证书索引
    for (int i=0;i<lineStr.length();i++)
    {
        QString StrTemp;
        int temp;
        temp=lineStr.at(i).toLatin1();
        char ch=temp;
        StrTemp=QString(ch);
        DataField[i+6]=StrTemp[0].toLatin1();
    } //将base64格式string转换为16进制ascii码，再转为qbytearray储存
    QByteArray sendData;
    dataPack(DataField,sendData);
    serial->write(sendData);
//    if (buf[11]==0x53)
//    {
        qDebug()<<"公钥发送成功!";
        ui->OutputLine1->clear();
        ui->OutputLine1->setText(tr("公钥发送成功!"));
//    }

}

/**
 * @brief 私钥下载
 */
void MainWindow::on_KeyDownloadButton_2_clicked()
{
    QString lineStr;
    FileOpen(lineStr);

    QByteArray DataField;
    QByteArray DataLength;
    DataField[0]=0x86; //命令标识
    int length=lineStr.length()+3;
    DataLength=intToByte(length);
    DataField[1]=DataLength[1];
    DataField[2]=DataLength[0];//数据长度位应预留两个字节
    DataField[3]=0x00; //证书管理模式
    DataField[4]=0x00; //证书用途
    DataField[5]=0x00; //证书索引
    for (int i=0;i<lineStr.length();i++)
    {
        QString StrTemp;
        int temp;
        temp=lineStr.at(i).toLatin1();
        char ch=temp;
        StrTemp=QString(ch);
        DataField[i+6]=StrTemp[0].toLatin1();
    } //将base64格式string转换为16进制ascii码，再转为qbytearray储存
    QByteArray sendData;
    dataPack(DataField,sendData);
    serial->write(sendData);
    qDebug()<<"私钥发送成功!";
    ui->OutputLine1->clear();
    ui->OutputLine1->setText(tr("私钥发送成功!"));
}

/**
 * @brief KEK下载
 */
void MainWindow::on_KekDownloadButton_clicked()
{
    QString lineStr;
    FileOpen(lineStr);

    QByteArray DataField;
    QByteArray DataLength;
    DataField[0]=0x81; //命令标识
    int length=lineStr.length()+3;
    DataLength=intToByte(length);
    DataField[1]=DataLength[1];
    DataField[2]=DataLength[0];//数据长度位应预留两个字节
    DataField[3]=0x00;
    int i=0;
    for (i=0;i<lineStr.length();i++)
    {
        QString StrTemp;
        int temp;
        temp=lineStr.at(i).toLatin1();
        char ch=temp;
        StrTemp=QString(ch);
        DataField[i+4]=StrTemp[0].toLatin1();
    } //将base64格式string转换为16进制ascii码，再转为qbytearray储存
    DataField[i+4+1]=0x00;//1字节密钥类型
    DataField[i+4+2]=0x00;
    DataField[i+4+3]=0x00;//2字节密钥索引
    QByteArray sendData;
    dataPack(DataField,sendData);
    serial->write(sendData);
    qDebug()<<"私钥发送成功!";
    ui->OutputLine1->clear();
    ui->OutputLine1->setText(tr("私钥发送成功!"));
}

/**
 * @brief CA证书下载
 */
void MainWindow::on_CADownloadButton_clicked()
{
    //
    //
}

/**
 * @brief 公私钥对生成
 */
void MainWindow::on_GenelKeyButton_clicked()
{
    //
    //
}

/**
 * @brief SM2签名
 */
void MainWindow::on_SM2SignButton_clicked()
{
    //
    //
}

/**
 * @brief SM2验签
 */
void MainWindow::on_SM2CheckButton_clicked()
{
    //
    //
}

/**
 * @brief 数据加密模块（输入加密配置信息和加密明文）
 */
void MainWindow::on_encryptionButton_clicked()
{
    if (ui->AlgoSelectBox->currentText()=="DES")
    {
        if (ui->ModeSelectBox->currentText()=="ECB")
        {
            //
        }
        else if (ui->ModeSelectBox->currentText()=="CBC")
        {
            //
        }
    }
    else if (ui->AlgoSelectBox->currentText()=="3DES")
    {
        if (ui->ModeSelectBox->currentText()=="ECB")
        {
            //
        }
        else if (ui->ModeSelectBox->currentText()=="CBC")
        {
            //
        }
    }
    else if (ui->AlgoSelectBox->currentText()=="SM2")
    {
        if (ui->ModeSelectBox->currentText()=="ECB")
        {
            //
        }
        else if (ui->ModeSelectBox->currentText()=="CBC")
        {
            //
        }
    }
}

/**
 * @brief 数据解密模块
 */
void MainWindow::on_decryptionButton_clicked()
{
    if (ui->AlgoSelectBox->currentText()=="DES")
    {
        if (ui->ModeSelectBox->currentText()=="ECB")
        {
            //
        }
        else if (ui->ModeSelectBox->currentText()=="CBC")
        {
            //
        }
    }
    else if (ui->AlgoSelectBox->currentText()=="3DES")
    {
        if (ui->ModeSelectBox->currentText()=="ECB")
        {
            //
        }
        else if (ui->ModeSelectBox->currentText()=="CBC")
        {
            //
        }
    }
    else if (ui->AlgoSelectBox->currentText()=="SM2")
    {
        if (ui->ModeSelectBox->currentText()=="ECB")
        {
            //
        }
        else if (ui->ModeSelectBox->currentText()=="CBC")
        {
            //
        }
    }
}



