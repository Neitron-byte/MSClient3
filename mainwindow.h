#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTCPSocket>
#include <QUdpSocket>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QByteArray>
#include <QDataStream>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotSendDatagram();
    void slotProcessDatagram();

private slots:
    void on_pushButton_clicked();
    void started();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_Discon_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QProcess* m_process = nullptr;
    QString m_IP;
    quint32 m_port;
    QString m_user;
    QString  m_pass;

    QUdpSocket* m_udp;
    QUdpSocket* m_udpReceiver;

    QTcpSocket* m_tcp;

    enum class CommandToServer : quint8
        {
               C = 0x10, // Команда на подключение.

        };

    enum class Type : quint8
        {
               Con = 0x01, // connect
               Dis = 0x2, // disconnect

        };
    void writeDATA(Type);
    void TestWrite();

};


#endif // MAINWINDOW_H
