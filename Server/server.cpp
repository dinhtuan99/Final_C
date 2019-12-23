#include "Server.h"

Server::Server () {
    // Tao va sap xep widget
    status = new QLabel();
    exit = new QPushButton(tr("Thoát"));
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
        connect(server, SIGNAL(newConnection()), this, SLOT(ketNoiMoi()));
    }
    size = 0;
}

void Server::ketNoiMoi() {

    QTcpSocket *nguoiDungMoi = server->nextPendingConnection();
    // nguoiDung << nguoiDungMoi;

    connect(nguoiDungMoi, SIGNAL(readyRead()), this, SLOT(nhanDuLieu()));
    //connect(nguoiDungMoi, SIGNAL(readyRead()), this, SLOT(nhanNickName()));
    connect(nguoiDungMoi, SIGNAL(disconnected()), this, SLOT(ngatKetNoi()));

   //s guiTinNhanChoMoiNguoi (tr("<em>Một người mới  vừa tham gia với chúng ta !</em>"),nguoiDungMoi);
}

void Server::nhanDuLieu() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) { // Neu khong xac dinh duoc nguon phat, chung ta se dung xu ly
        return;
    }
    QDataStream in(socket);
    if (size == 0) { //Neu chua biet kich thuoc tin nhan thi chung ta se thu tim trong goi du lieu vua toi
        if (socket->bytesAvailable() < (int)sizeof(quint16)) { //Kich thuoc goi tin nho hon kich thuc kieu so nguyen
            return;
        }
        qDebug()<<"sucess0";
        in >> size; // Neu nhan duoc kich thuoc tin nhan thi lay ra gia tri do
        qDebug()<<size;
    }

    if (type == 0) { //Neu chua biet kich thuoc tin nhan thi chung ta se thu tim trong goi du lieu vua toi
        if (socket->bytesAvailable() < (int)sizeof(quint16)) { //Kich thuoc goi tin nho hon kich thuc kieu so nguyen
            return;
        }
        qDebug()<<"nhan kieu di lieu";
        in >> type; // Neu nhan duoc kich thuoc tin nhan thi lay ra gia tri do
        qDebug()<<type;
    }
    if((quint16) type == 1){
        if (socket->bytesAvailable() < size) { // Neu chua nhan du tin nhan thi thoat xu ly
            return;
        }
        QString name;
        in >> name;
        //nickName.append(tinNhan);
        client.insert(name,socket);

        qDebug()<<"nhan ten";
        type =0;
        size = 0;
        //QTcpSocket *tempSocket = socket;
        //nguoiDung.remove(socket);
        tempSocket = socket;
        guiTinNhanChoMoiNguoi ((name+(tr(" vừa tham gia với chúng ta !"))),socket);

    }else{
        qDebug()<<"nhan tin nhan";
        // Biet kich thuoc, chung ta se kiem tra xem da nhan duoc toan bo tin nhan chua
        if (socket->bytesAvailable() < size) { // Neu chua nhan du tin nhan thi thoat xu ly
            return;
        }
        QString tinNhan;
        in >> tinNhan;

        guiTinNhanChoMoiNguoi(tinNhan,socket);

        // Dat lai kich thuoc la 0 de cho tin nhan tiep theo
        size = 0;
        type =0;
    }
}
void Server::ngatKetNoi() {

    qDebug()<<"ngat ket noi";
    // Xac dinh xem ai la nguoi ngat ket noi
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Neu khong tim thay nguoi gui tin hieu thi huy bo xu ly
        return;
    guiTinNhanChoMoiNguoi(client.key(socket)+tr("  vừa mới rời đi"),socket);
    //nguoiDung.removeOne(socket);
    client.remove(client.key(socket));
    socket->deleteLater();
}
void Server::guiTinNhanChoMoiNguoi(const QString &tinNhan,QTcpSocket *socket) {
    // Chuan bi tin nhan gui di
    QByteArray goiTinNhan;
    QDataStream out(&goiTinNhan, QIODevice::WriteOnly);

    out << (quint16) 0; // Viet gia tri 0 o dau goi tin de dat truoc cho de viet kich thuoc tin nhan
    out << tinNhan; // Viet noi dung tin nhan vao goi tin
    out.device()->seek(0); // Quay ve dau goi tin
    out << (quint16) (goiTinNhan.size() - sizeof(quint16)); // Thay 0 bang gia tri kich thuoc that cua tin nhan

    // Gui tin cho tat ca nguoi dung ket noi
    QList<QTcpSocket*> values = client.values();
    for (int i = 0; i < values.size(); i++) {
        if(socket != values[i] )
        values[i]->write(goiTinNhan);
    }
   // qDebug()<<"gui tin nhan ve client tru "+nguoiDung.key(socket);
    qDebug()<<"Gui tin nhan cho moi nguoi";
}
