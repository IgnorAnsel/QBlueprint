#ifndef OPENCV_H
#define OPENCV_H

#include <QImage>
#ifdef OPENCV_FOUND
#include <opencv2/opencv.hpp>
class opencv
{
public:
    opencv();
    static QImage threshold(QImage inputImage, double threshValue, double maxValue, int thresholdType);
    static QImage convertToGray(QImage inputImage); // 转换为灰度图的函数
private:
    // 将 QImage 转换为 cv::Mat
    static cv::Mat QImageToCvMat(const QImage &image);
    // 将 cv::Mat 转换为 QImage
    static QImage CvMatToQImage(const cv::Mat &mat);
};
#endif
#endif // OPENCV_H
