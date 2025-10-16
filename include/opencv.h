#ifndef OPENCV_H
#define OPENCV_H

#include <QImage>
#ifdef OPENCV_FOUND
#include <opencv2/opencv.hpp>
#include <QVector2D>
#include <QVector3D>
//#include <qvectornd.h>
class opencv
{
public:
    opencv();

    static QImage threshold(QImage inputImage, double threshValue, double maxValue, int thresholdType);
    static QStringList inputNames_threshold;
    static QString outputName_threshold;


    static QImage BGRtoGRAY(QImage inputImage); // 转换灰度图的函数
    static QStringList inputNames_BGRtoGRAY;
    static QString outputName_BGRtoGRAY;

    static QImage BGRtoHSV(QImage inputImage); // 转换HSv颜色空间的函数
    static QStringList inputNames_BGRtoHSV;
    static QString outputName_BGRtoHSV;

    static QImage erode(QImage inputImage,int Size_a,int Size_b); // 腐蚀   |（size指定图形处理矩阵kernel的大小，为奇数）
    static QStringList inputNames_erode;
    static QString outputName_erode;

    static QImage dilate(QImage inputImage,int Size_a,int Size_b); // 膨胀  |
    static QStringList inputNames_dilate;
    static QString outputName_dilate;

    static QImage canny(QImage inputImage,int x,int y); // 边缘检测（x,y越大线条越粗）
    static QStringList inputNames_canny;
    static QString outputName_canny;

    static QImage gaussianblur(QImage inputImage,int Size_a,int Size_b,int a,int b); // 高斯模糊(Size越大越模糊)
    static QStringList inputNames_gaussianblur;
    static QString outputName_gaussianblur;

    // 旋转/倾斜图像（center旋转中心点（做倾斜变换时一般为图像中心点），angle旋转/倾斜角度（度），scale缩放因子）
    static QImage rotateandincline(QImage inputImage,QPoint center,double angle,double scale);
    static QStringList inputNames_rotateandincline;
    static QString outputName_rotateandincline;

    // 透视变换（从图像选取四点tl左上、tr右上、dl左下、dr右下，变换到新图像上,imgsize新图像大小（width，hight））
    static QImage per_trans(QImage inputImage,QPoint tl,QPoint tr,QPoint dl,QPoint dr,QVector2D imgsize);
    static QStringList inputNames_per_trans;
    static QString outputName_per_trans;

    static QImage resize(QImage inputImage,double x_times,double y_times); // 缩放图像大小(x_times水平方向缩放倍数，y_times竖直方向缩放倍数)
    static QStringList inputNames_resize;
    static QString outputName_resize;

    static QImage line(QImage inputImage,QPoint start,QPoint end,QColor color,int thickness); //在图像上画直线（从start画到end，color直线颜色，thick直线粗细）
    static QStringList inputNames_line;
    static QString outputName_Bline;

private:
    // 将 QImage 转换为 cv::Mat
    static cv::Mat QImageToCvMat(const QImage &image);
    // 将 cv::Mat 转换为 QImage
    static QImage CvMatToQImage(const cv::Mat &mat);
};
#endif
#endif // OPENCV_H
