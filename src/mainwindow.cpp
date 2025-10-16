#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , blueprint(new QBlueprint(this))  // 初始化QBlueprint对象
{
    setCentralWidget(blueprint);
}

MainWindow::~MainWindow()
{
}

