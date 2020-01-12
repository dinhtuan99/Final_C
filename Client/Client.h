#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT
public:
    Client(QWidget *parent = nullptr);

private slots:
    void pushConnectButton();
    void pushSendButton();
    void pushEnterButton();
    void receivePackage();
    void connectToServer();
    void disconnectServer();
    void ErrSocket(QAbstractSocket::SocketError err);

private:
    QTcpSocket *socket; // Server
    quint16 size;
    QString nickName;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
