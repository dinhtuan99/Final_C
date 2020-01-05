#ifndef CUASONGUOIDUNG_H
#define CUASONGUOIDUNG_H

#include <QtWidgets>
#include <QtNetwork>
#include "ui_client.h"
#include <QDebug>
class Client : public QWidget, private Ui::Client {
    Q_OBJECT

    public:
        Client();
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
};

#endif
