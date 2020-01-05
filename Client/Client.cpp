#include "client.h"

Client::Client() {
    setupUi(this);
    cuocHoiThoai->setEnabled(false);
    khungSoanThao->setEnabled(false);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(receivePackage()));
    connect(socket, SIGNAL(connected()), this, SLOT(connectToServer()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectServer()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(ErrSocket(QAbstractSocket::SocketError)));

    connect(nutKetNoi, SIGNAL(clicked()), this, SLOT(pushConnectButton()));
    connect(nutGuiTin, SIGNAL(clicked()), this, SLOT(pushSendButton()));
    connect(khungSoanThao, SIGNAL(returnPressed()), this, SLOT(pushEnterButton()));

    size = 0;
}

// Thiet lap ket noi den may chu
void Client::pushConnectButton() {

    QDialog dialog(this);
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Register: "));

    // Add the lineEdits with their respective labels
    QList<QLineEdit *> fields;

        QLineEdit *lineEditHost = new QLineEdit(&dialog);
        QString labelHost = QString("Host Address: ");
        lineEditHost->setText("127.0.0.1");
        form.addRow(labelHost, lineEditHost);
        fields << lineEditHost;

        QLineEdit *lineEditNickName = new QLineEdit(&dialog);
        QString labelNickname = QString("Nickname: ");
        form.addRow(labelNickname, lineEditNickName);
        fields << lineEditNickName;


    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields
       const QString hostAddress = lineEditHost->text();
          nickName = lineEditNickName->text();
         // Thong bao la ket noi dang duoc thuc hien
         cuocHoiThoai->append(tr("<em>Đang kết nối...</em>"));

         nutKetNoi->setEnabled(false);

         socket->abort();
        socket->connectToHost(hostAddress,12399); // Ket noi toi may chu
       // socket->write(nickName);
    }
    QByteArray package;
    QDataStream out(&package, QIODevice::WriteOnly);

    // Chuan bi goi tin de gui di
    QString tinGuiDi = nickName;
    out << (quint16) 0;
    out << (quint16) 1;
    out << tinGuiDi;
    out.device()->seek(0);
    out << (quint16) (package.size() - sizeof(quint16)-sizeof(quint16));

    socket->write(package); // Gui goi tin
    qDebug()<<"11111111111";
}

// Gui tin den may chu
void Client::pushSendButton() {
    QByteArray package;
    QDataStream out(&package, QIODevice::WriteOnly);

    // Chuan bi goi tin de gui di
    const QString timestamp = QDateTime::currentDateTime().toString("dd-MM hh:mm ap");
    QString tinGuiDi = tr("<strong>") + nickName +tr("</strong>, ") +timestamp+": "+ khungSoanThao ->text();
    out << (quint16) 0;
    out << (quint16) 2;
    out << tinGuiDi;
    out.device()->seek(0);
    out << (quint16) (package.size() - sizeof(quint16)-sizeof(quint16));

    socket->write(package); // Gui goi tin

    khungSoanThao ->clear(); // Xoa tin vua gui khoi khung soan thao
    khungSoanThao ->setFocus();
    cuocHoiThoai->append(tinGuiDi);
}

void Client::pushEnterButton() {
    pushSendButton();
}

void Client::receivePackage() {
    QDataStream in(socket);
    if (size == 0) {
         if (socket->bytesAvailable() < (int)sizeof(quint16)) { //Kich thuoc goi tin nho hon kich thuc kieu so nguyen
             return;
        }
        in >> size; // Neu nhan duoc kich thuoc tin nhan thi lay ra gia tri do
    }
    // Biet kich thuoc, chung ta se kiem tra xem da nhan duoc toan bo tin nhan chua
    if (socket->bytesAvailable() < size) { // Neu chua nhan du tin nhan thi thoat xu ly
        return;
    }
    QString tinNhan;
    in >> tinNhan;

    cuocHoiThoai->append(tinNhan);

    // Dat lai kich thuoc la 0 de cho tin nhan tiep theo
    size = 0;

}

// Slot kich hoat khi ket noi thanh cong
void Client::connectToServer() {
    cuocHoiThoai->append(tr("<em>Kết nối thành công !</em>"));
    nutKetNoi->setEnabled(false);
    cuocHoiThoai->setEnabled(true);
    khungSoanThao->setEnabled(true);


}

// Slot kich hoat khi thoat ket noi
void Client::disconnectServer() {
    cuocHoiThoai->append(tr("<em>Tạm biệt, hẹn gặp lại sau !</em>"));
}

// Slot kich hoat khi co loi socket
void Client::ErrSocket(QAbstractSocket::SocketError err) {
    switch(err) { // Hien thi thong bao khac nhau tuy theo loi gap phai

        case QAbstractSocket::HostNotFoundError:
            cuocHoiThoai->append(tr("<em>LỖI : Không thể kết nối tới máy chủ ! Vui lòng kiểm tra lại địa chỉ IP và cổng truy cập.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            cuocHoiThoai->append(tr("<em>LỖI : Máy chủ từ chối truy cập ! Vui lòng kiểm tra chắc chắn là máy chủ đã được khởi động. Lưu ý đồng thời lỗi địa chỉ IP và cổng truy cập.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            cuocHoiThoai->append(tr("<em>LỖI : Máy chủ đã ngắt kết nối !</em>"));
            break;
        default:
            cuocHoiThoai->append(tr("<em>LỖI : ") + socket->errorString() + tr("</em>"));
    }

    nutKetNoi->setEnabled(true);
}
