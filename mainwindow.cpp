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


    m_tcp = new QTcpSocket(this);



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
   m_port = ui->lineEdit_PORT->text().toUInt();
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

   quint32 user = static_cast<quint32>(m_user.toUInt());

   sendData.append(static_cast<quint8>(sizeof(quint32)));

   //qDebug() << QString::number(sendData[1],16);
   sendData.append(user);


   //Password
   sendData.append(static_cast<quint8>(0x00));
   sendData.append(static_cast<quint8>(m_pass.length()));
   sendData.append(m_pass.toLocal8Bit());

   //IP
   sendData.append(static_cast<quint8>(0x00));
   sendData.append(static_cast<quint8>(sizeof(quint32)));
   //sendData.append(m_IP.toLocal8Bit());


   QHostAddress loop(m_IP);
   quint32 address = loop.toIPv4Address();

   sendData.append ( reinterpret_cast<char *>(&address) , sizeof(quint32) );

   //Port
   sendData.append(reinterpret_cast<char *>(& m_port) , sizeof(quint32) );


    qDebug()<< "DATA";
//    qDebug()<<"LenghtSendData:" << sendData.length();
    for (int i = 0; i < sendData.length(); ++i) {
       qDebug()<< QString::number(sendData[i],16) << i;
    }

   qDebug()<<"LenghtSendData:" << sendData.length();



   QByteArray Request;

   Request.append(static_cast<quint8>(0x10)); //Команад
   Request.append(static_cast<quint8>(0x00)); //Not used
   Request.append(static_cast<quint8>(0x00)); //Encry index
   Request.append(static_cast<quint8>(0x00)); //Encry index
   Request.append(static_cast<quint8>(0x00)); //Encry value
   Request.append(static_cast<quint8>(0x00)); //Encry value
   Request.append(static_cast<quint8>(0x00)); //Lenght1
   Request.append(static_cast<quint8>(sendData.length())); //Lenght2 данных

//    for (int i = 0; i < Request.length(); ++i) {
//              qDebug() << QString::number(Request[i],16) << i;
//        }

   Request.append(sendData);


   qDebug()<<"LenghtRequest: " << Request.length();



   m_udp->writeDatagram(Request,QHostAddress::LocalHost,4100);

}


void MainWindow::TestWrite()
{


        QByteArray Data (17,2);

        //Data.resize(24);

        Data[0] = 0x10;

        Data[1] = 0x00;//Reserved

        Data[2] = 0x00;//Encry_ind
        Data[3] = 0x00;

        Data[4] = 0x00;//Encry_val
        Data[5] = 0x00;

        Data[6] = 0x00;
        Data[7] = 0x11; // размер данных x16 17 байт!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



        Data[8] = 0x01;//type


        Data[9] = 0x00;//ID length
        Data[10] = 0x01;//ID length
        Data[11] = 0x01;//ID

        Data[12] = 0x00;//pas - l
        Data[13] = 0x01;//pas - l
        Data[14] = 0x01;//password

        Data[15] = 0x00;//IP - l
        Data[16] = 0x04;//IP - l

//---------------IP--------------// OK

        QHostAddress loop(m_IP);
        quint32 address = loop.toIPv4Address();
        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);
        ds << address;
        Data.append (ba);



        QByteArray ba2;
        QDataStream ds2(&ba2, QIODevice::WriteOnly);
        ds2 << m_port;
        Data.append (ba2);


        for (int i = 0; i < Data.length(); ++i) {
            qDebug()<< QString::number(Data[i],16) << i;
        }

        m_udp->writeDatagram(Data,QHostAddress::LocalHost,4100);


}

void MainWindow::on_pushButton_4_clicked()
{

    m_tcp->connectToHost(QHostAddress("192.168.8.244"),4101,QIODevice::ReadWrite);
    if (m_tcp->waitForConnected(30000)){
        qDebug()<<"Connect to Server is successfull";

    } else{
        qDebug()<<"Connect to Server is Fail";
        qDebug() << m_tcp->errorString();
    }

}
