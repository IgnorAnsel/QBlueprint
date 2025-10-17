// keysimulator.h
#ifndef KEYSIMULATOR_H
#define KEYSIMULATOR_H

#include <QObject>
#include <QKeyEvent>
#include <QApplication>

class KeySimulator : public QObject
{
    Q_OBJECT
public:
    explicit KeySimulator(QObject *parent = nullptr);
    
    // 模拟按键按下
    Q_INVOKABLE void simulateKeyPress(int keyCode);
    
    // 模拟按键释放
    Q_INVOKABLE void simulateKeyRelease(int keyCode);
    
    // 模拟按键点击（按下并释放）
    Q_INVOKABLE void simulateKeyClick(int keyCode);
    
    // 模拟组合键（如Ctrl+C）
    Q_INVOKABLE void simulateKeyCombination(int modifierKey, int keyCode);
    
    // 模拟文本输入
    Q_INVOKABLE void simulateTextInput(const QString &text);
    
    // 设置目标窗口（可选）
    Q_INVOKABLE void setTargetWindow(QWidget *window);
    // 输入名称列表
    static QStringList inputNames_simulateKeyPress;
    static QStringList inputNames_simulateKeyRelease;
    static QStringList inputNames_simulateKeyClick;
    static QStringList inputNames_simulateKeyCombination;
    static QStringList inputNames_simulateTextInput;
    static QStringList inputNames_setTargetWindow;

    // 输出名称
    static QString outputName_simulateKeyPress;
    static QString outputName_simulateKeyRelease;
    static QString outputName_simulateKeyClick;
    static QString outputName_simulateKeyCombination;
    static QString outputName_simulateTextInput;
    static QString outputName_setTargetWindow;
private:
    QWidget *m_targetWindow;
};

#endif // KEYSIMULATOR_H