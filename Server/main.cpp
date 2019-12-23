#include <QApplication>
#include "Server.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setOrganizationName("QtExamples");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    Server cuaSo;
    cuaSo.show();

    return app.exec();
}

