#include "server.h"

Server::Server () {
    // Tao va sap xep widget
    status = new QLabel();
    exit = new QPushButton(tr("Thoát"));
    user = new QTextEdit("Chưa có người nào trong phòng");
    connect(exit, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *lop = new QVBoxLayout();
    lop->addWidget(status);
    lop->addWidget(exit);
    setLayout(lop);

    setWindowTitle(tr("Server"));

    // Quan ly may chu
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, 12399)) { // Khoi dong may chu o cong 12399 và tren tat ca cac dia chi IP cua may
        // Neu may chu chua duoc khoi dong
        status->setText(tr("Máy chủ chưa khởi động được. Lý do :<br />") +server->errorString());
    } else {
        // Neu may chu duoc khoi dong thanh cong
        qDebug()<<"Máy chủ được mở trên cổng " + QString::number(server->serverPort());
        status->setText(tr("Máy chủ được mở trên cổng <strong>") + QString::number(server->serverPort()));
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnectFromClient()));
    }
    size = 0;
}

void Server::newConnectFromClient() {


    QTcpSocket *newUsers = server->nextPendingConnection();
    users << newUsers;
    qDebug()<<"aaaaaaaaa";

    connect(newUsers, SIGNAL(readyRead()), this, SLOT(receivePackageFromClient()));
    connect(newUsers, SIGNAL(disconnected()), this, SLOT(disconnectClient()));
    sendPackageForAll("Một người vừa tham gia room chat !");
}

void Server::receivePackageFromClient() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) { // Neu khong xac dinh duoc nguon phat, chung ta se dung xu ly
        return;
    }
    QDataStream in(socket);

    if (size == 0) { //Neu chua biet kich thuoc tin nhan thi chung ta se thu tim trong goi du lieu vua toi
        if (socket->bytesAvailable() < (int)sizeof(quint16)) {
            return;
        }
        qDebug()<<"nhan size";
        in >> size; // Neu nhan duoc kich thuoc tin nhan thi lay ra gia tri do
        qDebug()<<size;
    }

        qDebug()<<"nhan tin nhan";
        // Biet kich thuoc, chung ta se kiem tra xem da nhan duoc toan bo tin nhan chua
        if (socket->bytesAvailable() < size) {
            return;
        }
        QString tinNhan;
        in >> tinNhan;

        qDebug()<<tinNhan;

        sendPackageForAll(tinNhan);

        // Dat lai kich thuoc la 0 de cho tin nhan tiep theo
        size = 0;
    }

void Server::disconnectClient() {

    qDebug()<<"ngat ket noi";
    // Xac dinh xem ai la nguoi ngat ket noi
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Neu khong tim thay nguoi gui tin hieu thi huy bo xu ly
        return;
    sendPackageForAll(client.key(socket)+tr("  vừa mới rời đi"));

    users.removeOne(socket);
    client.remove(client.key(socket));

    socket->deleteLater();
}
void Server::sendPackageForAll(const QString &package) {
    // Chuan bi tin nhan gui di
    QByteArray goiTinNhan;
    QDataStream out(&goiTinNhan, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << package; // Viet noi dung tin nhan vao goi tin
    out.device()->seek(0); // Quay ve dau goi tin
    out << (quint16) (goiTinNhan.size() - sizeof(quint16));

    // Gui tin cho tat ca nguoi dung ket noi
    QList<QTcpSocket*> values = client.values();

    qDebug()<<"length:",users.length();
    for (int i = 0; i < users.size(); i++) {
        users[i]->write(goiTinNhan);
    }
    qDebug()<<"Gui tin nhan cho moi nguoi";
}
