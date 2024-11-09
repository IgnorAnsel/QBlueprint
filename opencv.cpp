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

////////////////////////！！转灰度图！！
QImage opencv::BGRtoGRAY(QImage inputImage)
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

////////////////////////！！转HSV空间！！
QImage opencv::BGRtoHSV(QImage inputImage)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::cvtColor(mat,mat,cv::COLOR_BGR2HSV);

    return CvMatToQImage(mat);
}

////////////////////////！！腐蚀！！
QImage opencv::erode(QImage inputImage,int Size_a,int Size_b)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }
    // kernel图像处理矩阵
    cv::Mat Kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(Size_a,Size_b));
    // 腐蚀
    cv::erode(mat,mat,Kernel);

    return CvMatToQImage(mat);
}

////////////////////////！！膨胀！！
QImage opencv::dilate(QImage inputImage,int Size_a = 1,int Size_b = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }
    // kernel图像处理矩阵
    cv::Mat Kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(Size_a,Size_b));
    // 膨胀
    cv::dilate(mat,mat,Kernel);

    return CvMatToQImage(mat);
}

////////////////////////！！边缘检测！！
QImage opencv::canny(QImage inputImage,int x = 1,int y = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::Canny(mat,mat,x,y);

    return CvMatToQImage(mat);
}

////////////////////////！！高斯模糊！！
QImage opencv::gaussianblur(QImage inputImage,int Size_a = 1,int Size_b = 1,int a = 0,int b = 0)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::GaussianBlur(mat,mat,cv::Size(Size_a,Size_b),a,b);

    return CvMatToQImage(mat);
}

////////////////////////！！旋转！！
QImage opencv::rotateandincline(QImage inputImage,QPoint center,double angle = 0.0,double scale = 1.0)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::Point2f cvcenter;
    cvcenter.x = center.x()/2.0f;
    cvcenter.y = center.y()/2.0f;
    // 计算矩阵,用来对图像进行变换
    cv::Mat rotation_matrix = cv::getRotationMatrix2D(cvcenter, angle, scale);

    // 计算新图像的大小，避免图像超出显示框边缘
    cv::Rect2f new_size = cv::RotatedRect(cv::Point2f(), mat.size(), angle).boundingRect2f();

    // 调整矩阵,使旋转后的图像在显示框居中
    rotation_matrix.at<double>(0, 2) += new_size.width / 2.0 - cvcenter.x;
    rotation_matrix.at<double>(1, 2) += new_size.height / 2.0 - cvcenter.y;

    cv::Mat rotateMat;
    // 变换的函数
    cv::warpAffine(mat, rotateMat, rotation_matrix, new_size.size());

    return CvMatToQImage(rotateMat);
}

////////////////////////！！透视变换！！
QImage opencv::per_trans(QImage inputImage,QPoint tl,QPoint tr,QPoint dl,QPoint dr,QVector2D imgsize)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::Point2f src[4] = {{(float)tl.x(),(float)tl.y()},{(float)tr.x(),(float)tr.y()},
                          {(float)dl.x(),(float)dl.y()},{(float)dr.x(),(float)dr.y()}};
    cv::Point2f dst[4] = {{0,0},{imgsize[0],0},{0,imgsize[1]},{imgsize[0],imgsize[1]}};
    // 变换矩阵
    cv::Mat martrix = cv::getPerspectiveTransform(src,dst);
    // 透视变换
    cv::warpPerspective(mat,mat,martrix,cv::Point(imgsize[0],imgsize[1]));

    return CvMatToQImage(mat);
}

////////////////////////！！缩放大小！！
QImage opencv::resize(QImage inputImage,double x_times = 1,double y_times = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::resize(mat,mat,cv::Size(),x_times,y_times);

    return CvMatToQImage(mat);
}

////////////////////////！！画直线！！
QImage opencv::line(QImage inputImage,QPoint start,QPoint end,QVector3D color,int thickness = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    // 数据类型转换
    cv::Point cvstart,cvend;
    cvstart.x = start.x();  cvstart.y = start.y();
    cvend.x = end.x();      cvend.y = end.y();

    cv::Scalar cvcolor(color[0],color[1],color[2]);
    // 画线
    cv::line(mat,cvstart,cvend,cvcolor,thickness);

    return CvMatToQImage(mat);
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
