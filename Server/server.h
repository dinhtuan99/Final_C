#ifndef CUASOMAYCHU_H
#define CUASOMAYCHU_H

#include <QtWidgets>
#include <QtNetwork>

class Server : public QWidget {
    Q_OBJECT

    public:
        Server();
        void sendPackageForAll(const QString &package);

    private slots:
        void newConnectFromClient();
        void receivePackageFromClient();
        void disconnectClient();

    private:
        QLabel *status;
        QTextEdit *user;
        QPushButton *exit;

        QTcpServer *server;
        QList<QTcpSocket *> users;
        quint16 size;
        quint16 type;
        // QList<QString> nickName;
        QMap<QString,QTcpSocket *> client;
        QTcpSocket* tempSocket;
        QString clientSend;
};

#endif
