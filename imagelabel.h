#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>

class ImageLabel : public QLabel {
    Q_OBJECT
public:
    explicit ImageLabel(QWidget* parent = nullptr)
        : QLabel(parent), imageDialog(nullptr) {

        createButton();
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        // 保留默认的 QLabel 行为
        QLabel::mousePressEvent(event);
    }

private slots:
    void toggleImageDialog() {
        if (imageDialog && imageDialog->isVisible()) {
            // 如果图像对话框已打开，则关闭
            imageDialog->close();
            imageDialog->deleteLater();
            imageDialog = nullptr;
        } else {
            // 创建并显示放大的图像
            imageDialog = new QDialog();
            imageDialog->setWindowFlags(Qt::Popup);

            QLabel* enlargedLabel = new QLabel();
            enlargedLabel->setPixmap(pixmap().scaled(400, 400, Qt::KeepAspectRatio));

            QPushButton* closeButton = new QPushButton("X", imageDialog);
            closeButton->setFixedSize(30, 30);

            // 设置关闭按钮的点击事件
            connect(closeButton, &QPushButton::clicked, [this]() {
                if (imageDialog) {
                    imageDialog->close();
                    imageDialog->deleteLater();
                    imageDialog = nullptr;
                }
            });

            QVBoxLayout* layout = new QVBoxLayout(imageDialog);
            QHBoxLayout* topLayout = new QHBoxLayout();
            topLayout->addWidget(closeButton);
            topLayout->addStretch();

            layout->addLayout(topLayout);
            layout->addWidget(enlargedLabel);
            imageDialog->setLayout(layout);

            // 设置对话框的位置在主窗口旁边
            QPoint globalPos = this->mapToGlobal(this->rect().topLeft());
            imageDialog->move(globalPos.x() + this->width(), globalPos.y());

            imageDialog->show();
        }
    }
public:
    void setOpenButtonPos(QPointF buttonPos)
    {
        openButton->move(buttonPos.x(), buttonPos.y());
    }
private:
    QPushButton* openButton;    // 控制打开/关闭的按钮
    QDialog* imageDialog;       // 用于显示放大图像的对话框

    void createButton() {
        // 在 QLabel 的左上角创建一个按钮
        openButton = new QPushButton("O", this);
        openButton->setFixedSize(30, 30);
        openButton->move(0, 0); // 按钮位于左上角

        // 设置按钮的点击事件
        connect(openButton, &QPushButton::clicked, this, &ImageLabel::toggleImageDialog);
    }
};







#endif // IMAGELABEL_H
