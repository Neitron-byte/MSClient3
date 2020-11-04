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
}

MainWindow::~MainWindow()
{
    delete ui;
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
   m_port = ui->lineEdit_PORT->text().toInt();
   m_user = ui->lineEdit_User->text();
   m_pass = ui->lineEdit_Pass->text();
   qDebug()<< m_IP << m_port << m_user << m_pass;


}

void MainWindow::on_pushButton_2_clicked()
{

   //const char myData [] = {0x10,0x00,0x00,0x00,0x00,0x00, длина, Data };

    QByteArray sendData;
    //sendData.resize(2);
    sendData+=0x01;//0x01 - Connect

    //User данные
    sendData+=static_cast<quint16>(m_user.length());
    qDebug() << QString::number(sendData[1],16);
    sendData += m_user.toLocal8Bit();

    //Password
    sendData+=static_cast<quint16>(m_pass.length());
    sendData += m_pass.toLocal8Bit();

    //IP
    sendData+=static_cast<quint16>(m_IP.length());
    sendData += m_IP.toLocal8Bit();

    //Port

    sendData+=static_cast<quint32>(m_port);


    //qDebug()<<"LenghtSendData:" << sendData.length();

    for (int i = 0; i < sendData.length(); ++i) {
          qDebug() << QString::number(sendData[i],10) << i;
    }




    QByteArray Request;
    Request.resize(8 + sendData.length());
    Request[0] = 0x10; //Команад
    Request[1] = 0x00; //Not used
    Request[2] = 0x00; //Encry index
    Request[3] = 0x00; //Encry index
    Request[4] = 0x00; //Encry value
    Request[5] = 0x00; //Encry value
    Request += static_cast<quint16>(sendData.length()); //Lenght данных
    Request += sendData;


    qDebug()<<"LenghtRequest: " << Request.length();

    m_process->write(Request);
    m_process->waitForBytesWritten();

    m_process->waitForReadyRead();
    qDebug()<< m_process->readAll();

    

}
