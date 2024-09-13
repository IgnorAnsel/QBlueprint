#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , blueprint(new QBlueprint(this))  // 初始化QBlueprint对象
{
    ui->setupUi(this);
    setCentralWidget(blueprint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

