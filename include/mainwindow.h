#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qblueprint.h"
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QBlueprint* getBlueprint() const { return blueprint; }
    ~MainWindow();

private:
    QBlueprint *blueprint;
};
#endif // MAINWINDOW_H
