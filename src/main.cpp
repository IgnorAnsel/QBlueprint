#include "mainwindow.h"
#include "ForLoopTester.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // // 启动测试
    // QTimer::singleShot(1000, [&]() {
    //     ForLoopTester tester(w.getBlueprint());
    //     tester.runAllTests();
    // });

    return a.exec();
}