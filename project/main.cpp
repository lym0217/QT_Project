#include "mainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 000,000 원 설정
    QLocale::setDefault(QLocale(QLocale::Korean, QLocale::SouthKorea));
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
