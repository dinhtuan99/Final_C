#ifndef CUASOMAYCHU_H
#define CUASOMAYCHU_H

#include <QtWidgets>
#include <QtNetwork>

class Server : public QWidget {
    Q_OBJECT

    public:
        Server();
        void guiTinNhanChoMoiNguoi(const QString &tinNhan, QTcpSocket*socket);

    private slots:
        void ketNoiMoi();
        void nhanDuLieu();
        void ngatKetNoi();

    private:
        QLabel *status;
        QPushButton *exit;

        QTcpServer *server;
        //QList<QTcpSocket *> nguoiDung;
        quint16 size;
        quint16 type;
        // QList<QString> nickName;
         QMap<QString,QTcpSocket *> client;
         QTcpSocket* tempSocket;
        QString clientSend;
};

#endif
