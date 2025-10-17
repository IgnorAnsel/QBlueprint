// keysimulator.cpp
#include "keysimulator.h"
#include <QWidget>
#include <QDebug>
KeySimulator::KeySimulator(QObject *parent) : QObject(parent), m_targetWindow(nullptr)
{
}

void KeySimulator::simulateKeyPress(int keyCode)
{
    QWidget *target = m_targetWindow ? m_targetWindow : QApplication::focusWidget();
    if (target) {
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, keyCode, Qt::NoModifier);
        QApplication::postEvent(target, pressEvent);
    }
    qDebug() << "simulateKeyPress" << "keyCode:" << keyCode;
}

void KeySimulator::simulateKeyRelease(int keyCode)
{
    QWidget *target = m_targetWindow ? m_targetWindow : QApplication::focusWidget();
    if (target) {
        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, keyCode, Qt::NoModifier);
        QApplication::postEvent(target, releaseEvent);
    }
}

void KeySimulator::simulateKeyClick(int keyCode)
{
    simulateKeyPress(keyCode);
    simulateKeyRelease(keyCode);
}

void KeySimulator::simulateKeyCombination(int modifierKey, int keyCode)
{
    QWidget *target = m_targetWindow ? m_targetWindow : QApplication::focusWidget();
    if (target) {
        QKeyEvent *pressEvent = new QKeyEvent(QEvent::KeyPress, keyCode, Qt::KeyboardModifier(modifierKey));
        QApplication::postEvent(target, pressEvent);
        
        QKeyEvent *releaseEvent = new QKeyEvent(QEvent::KeyRelease, keyCode, Qt::KeyboardModifier(modifierKey));
        QApplication::postEvent(target, releaseEvent);
    }
}

void KeySimulator::simulateTextInput(const QString &text)
{
    QWidget *target = m_targetWindow ? m_targetWindow : QApplication::focusWidget();
    if (target) {
        for (const QChar &c : text) {
            int keyCode = c.unicode();
            simulateKeyClick(keyCode);
        }
    }
}

void KeySimulator::setTargetWindow(QWidget *window)
{
    m_targetWindow = window;
}

QStringList KeySimulator::inputNames_simulateKeyPress = {
    "keyCode (类型: int)\n含义：要模拟按下的按键键码"
};

QStringList KeySimulator::inputNames_simulateKeyRelease = {
    "keyCode (类型: int)\n含义：要模拟释放的按键键码"
};

QStringList KeySimulator::inputNames_simulateKeyClick = {
    "keyCode (类型: int)\n含义：要模拟点击的按键键码"
};

QStringList KeySimulator::inputNames_simulateKeyCombination = {
    "modifierKey (类型: int)\n含义：修饰键键码（如Ctrl、Alt）",
    "keyCode (类型: int)\n含义：要模拟的组合键键码"
};

QStringList KeySimulator::inputNames_simulateTextInput = {
    "text (类型: QString)\n含义：要模拟输入的文本"
};

QStringList KeySimulator::inputNames_setTargetWindow = {
    "window (类型: QWidget*)\n含义：目标窗口指针"
};

QString KeySimulator::outputName_simulateKeyPress = "void";
QString KeySimulator::outputName_simulateKeyRelease = "void";
QString KeySimulator::outputName_simulateKeyClick = "void";
QString KeySimulator::outputName_simulateKeyCombination = "void";
QString KeySimulator::outputName_simulateTextInput = "void";
QString KeySimulator::outputName_setTargetWindow = "void";