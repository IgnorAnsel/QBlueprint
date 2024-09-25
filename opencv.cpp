#include "opencv.h"
#ifdef OPENCV_FOUND

opencv::opencv()
{

}

QImage opencv::threshold(QImage inputImage, double threshValue, double maxValue, int thresholdType)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::Mat grayMat;
    if (mat.channels() == 3 || mat.channels() == 4) {
        cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY); // 转换为灰度图
    } else {
        grayMat = mat; // 已经是灰度图
    }

    cv::Mat binaryMat;

    cv::threshold(grayMat, binaryMat, threshValue, maxValue, thresholdType);

    QImage binaryQImage = CvMatToQImage(binaryMat);

    return binaryQImage;
}

QImage opencv::convertToGray(QImage inputImage)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::Mat grayMat;
    if (mat.channels() == 3 || mat.channels() == 4) {
        // 如果是彩色图，转换为灰度图
        cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
    } else {
        // 如果已经是灰度图，直接复制
        grayMat = mat;
    }
    return CvMatToQImage(grayMat);
}

cv::Mat opencv::QImageToCvMat(const QImage &image)
{
    switch (image.format()) {
    case QImage::Format_RGB32: {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone(); // 返回一个深拷贝
    }
    case QImage::Format_ARGB32: {  // 处理 QImage::Format_ARGB32 格式
        cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone(); // 深拷贝，避免原数据被修改
    }
    case QImage::Format_RGB888: {
        QImage swapped = image.rgbSwapped(); // Qt 使用 BGR 格式，OpenCV 使用 RGB
        cv::Mat mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine());
        return mat.clone();
    }
    case QImage::Format_Indexed8: {
        cv::Mat mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        return mat.clone();
    }
    default:
        qWarning("QImage format not supported for conversion to cv::Mat.");
        return cv::Mat();
    }
}


QImage opencv::CvMatToQImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC4: {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32).copy();
    }
    case CV_8UC3: {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped().copy(); // OpenCV 使用 BGR，Qt 使用 RGB
    }
    case CV_8UC1: {
        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++) {
            colorTable.push_back(qRgb(i, i, i));
        }
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        image.setColorTable(colorTable);
        return image.copy();
    }
    default:
        qWarning("cv::Mat format not supported for conversion to QImage.");
        return QImage();
    }
}
#endif
