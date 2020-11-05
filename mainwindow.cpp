#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_IP->setText("192.168.8.244");
    ui->lineEdit_PORT->setText("4101");
    ui->lineEdit_User->setText("Manager1");
    ui->lineEdit_Pass->setText("1");


    m_process = new QProcess(this);
    connect(m_process,SIGNAL(started()),this,SLOT(started()));

    m_udp = new QUdpSocket(this);

    m_udpReceiver = new QUdpSocket(this);
    connect(m_udpReceiver,SIGNAL(readyRead()),this, SLOT(slotProcessDatagram()));
    m_udpReceiver->bind(4101);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSendDatagram()
{

}

void MainWindow::slotProcessDatagram()
{
     QByteArray RecDatagram;

//    do {
//        RecDatagram.resize(m_udpReceiver->pendingDatagramSize());
//        m_udpReceiver->readDatagram(RecDatagram.data(), RecDatagram.size());
//    }

//    while(m_udpReceiver->hasPendingDatagrams());

     RecDatagram.resize(m_udpReceiver->pendingDatagramSize());
     m_udpReceiver->readDatagram(RecDatagram.data(), RecDatagram.size());

     for (int i = 0; i < RecDatagram.length(); ++i) {
         qDebug() << QString::number(RecDatagram[i],16);
     }



}


void MainWindow::on_pushButton_clicked()
{
    qDebug()<<QDir::homePath() /*"/file.exe"*/;
    QString programm = "C:/Users/eng1/Documents/ClientMS/MServer/MServer.exe";
    //m_process->setProgram(programm);
    m_process->start(programm);
    qDebug()<< m_process->exitCode();
    qDebug()<< m_process->currentReadChannel();
    qDebug()<< m_process->currentWriteChannel();

}

void MainWindow::started()
{
     qDebug()<<"Process is Started";
}

void MainWindow::on_pushButton_3_clicked()
{
   m_IP = ui->lineEdit_IP->text();
   m_port = ui->lineEdit_PORT->text();
   m_user = ui->lineEdit_User->text();
   m_pass = ui->lineEdit_Pass->text();
   qDebug()<< m_IP << m_port << m_user << m_pass;

}

void MainWindow::on_pushButton_2_clicked()
{

    //this->writeDATA(Type::Con);
    this->TestWrite();

}

void MainWindow::on_pushButton_Discon_clicked()
{
    this->writeDATA(Type::Dis);
}

void MainWindow::writeDATA(Type typeCommand)
{
    QByteArray sendData;
   //sendData.resize(2);
   sendData.append(static_cast<quint8>(typeCommand)); //0x01 - Connect 0x2 - Disconnect

   //User данные
   sendData.append(static_cast<quint8>(0x00));
   sendData.append(static_cast<quint8>(m_user.length()));

   //qDebug() << QString::number(sendData[1],16);
   sendData.append(m_user.toLocal8Bit());


   //Password
   sendData.append(static_cast<quint8>(0x00));
   sendData.append(static_cast<quint8>(m_pass.length()));
   sendData.append(m_pass.toLocal8Bit());

   //IP
   sendData.append(static_cast<quint8>(0x00));
   sendData.append(static_cast<quint8>(m_IP.length()));
   sendData.append(m_IP.toLocal8Bit());

   //Port

   sendData.append(m_port.toLocal8Bit());


//    qDebug()<< "DATA";
//    qDebug()<<"LenghtSendData:" << sendData.length();
//    for (int i = 0; i < sendData.length(); ++i) {
//       qDebug()<< QString::number(sendData[i],16) << i;
//    }

   qDebug()<<"LenghtSendData:" << sendData.length();



   QByteArray Request;
   //Request.resize(8 + sendData.length());
   Request.append(static_cast<quint8>(0x10)); //Команад
   Request.append(static_cast<quint8>(0x00)); //Not used
   Request.append(static_cast<quint8>(0x00)); //Encry index
   Request.append(static_cast<quint8>(0x00)); //Encry index
   Request.append(static_cast<quint8>(0x00)); //Encry value
   Request.append(static_cast<quint8>(0x00)); //Encry value
   Request.append(static_cast<quint8>(0x00)); //Lenght1
   Request.append(static_cast<quint8>(sendData.length())); //Lenght2 данных
   //qDebug()<<"LenghtRequest: " << static_cast<quint16>(sendData.length()) << Request.length();

//    for (int i = 0; i < Request.length(); ++i) {
//              qDebug() << QString::number(Request[i],10) << i;
//        }

   Request.append(sendData);


   qDebug()<<"LenghtRequest: " << Request.length();

   for (int i = 0; i < Request.length(); ++i) {
         qDebug() << QString::number(Request[i],16) << i;
   }


//    m_process->write(Request);
//    m_process->waitForBytesWritten();

//    m_process->waitForReadyRead();
//    qDebug()<< m_process->readAllStandardOutput();

   m_udp->writeDatagram(Request,QHostAddress::LocalHost,4100);

}

void MainWindow::TestWrite()
{

        QByteArray Data;

        Data.resize(31);

        Data[0] = 0x10;

        Data[1] = 0x00;
        Data[2] = 0x00;
        Data[3] = 0x00;
        Data[4] = 0x00;
        Data[5] = 0x00;
        Data[6] = 0x00;
        Data[7] = 0x17;
        Data[8] = 0x01;//type

//        for (int i = 9; i < 34; ++i) {
//             Data[i] = 0x00;
//         }

        Data[9] = 0x00;
        Data[10] = 0x01;//ID
        Data[11] = 0x01;

        Data[12] = 0x00;//pas
        Data[13] = 0x01;
        Data[14] = 0x01;

        Data[15] = 0x00;//IP
        Data[16] = 0x0a;

        Data[17] = 0x01;
        Data[18] = 0x09;
        Data[19] = 0x02;
        //Data[20] = 0x2e;
        Data[20] = 0x01;
        Data[21] = 0x06;
        Data[22] = 0x08;
        //Data[24] = 0x2e;
        Data[23] = 0x08;
        //Data[26] = 0x2e;
        Data[24] = 0x02;
        Data[25] = 0x04;
        Data[26] = 0x04;

        Data[27] = 0x04;
        Data[28] = 0x01;
        Data[29] = 0x00;
        Data[30] = 0x01;

        for (int i = 0; i < Data.length(); ++i) {
            qDebug()<< QString::number(Data[i],16);
        }

        m_udp->writeDatagram(Data,QHostAddress::LocalHost,4100);


}
