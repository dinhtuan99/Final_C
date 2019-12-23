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
        void anNutKetNoi();
        void anNutGuiTin();
        void anEnterGuiTin();
        void nhanDuLieu();
        void ketNoi();
        void ngatKetNoi();
        void loiSocket(QAbstractSocket::SocketError loi);
    private:
        QTcpSocket *socket; // May chu
        quint16 size;
        QString nickName;
};

#endif
