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
QStringList opencv::inputNames_threshold = {
"inputImage (类型: QImage)\n\
含义：输入的原始图像，类型为QImage，它是Qt中常用的图像格式之一。\n\
用途：作为需要进行阈值处理的图像，它可以是彩色图像或灰度图像。输入图像将在函数内部转换为OpenCV的cv::Mat格式，以便进行进一步的图像处理。",
"threshValue (类型: double)\n\
含义：用于阈值处理的阈值（通常是灰度值），也就是判断一个像素值是否需要转化为最大值（maxValue）或0（根据thresholdType的不同）。\n\
用途：该值用于确定阈值操作的标准。在灰度图像的情况下，像素值小于这个阈值的会被设置为0，像素值大于或等于此阈值的会被设置为maxValue。",
"maxValue (类型: double)\n\
含义：当一个像素的灰度值大于或等于阈值时，它将被赋值为maxValue。\n\
用途：该参数决定了大于阈值的像素的值。通常它设置为255，表示白色，或者设置为其他值来获得不同的效果。",
"thresholdType (类型: int)\n\
含义：阈值处理的类型，决定了如何应用阈值。OpenCV提供了多种阈值类型，常见的有以下几种：\n\
cv::THRESH_BINARY：如果像素值大于等于threshValue，则设置为maxValue，否则设置为0（黑色）。这是最常用的二值化方式。\n\
cv::THRESH_BINARY_INV：与THRESH_BINARY相反，如果像素值大于等于threshValue，则设置为0，否则设置为maxValue。\n\
cv::THRESH_TRUNC：如果像素值大于等于threshValue，则将像素值截断为threshValue，否则不变。\n\
cv::THRESH_TOZERO：如果像素值大于等于threshValue，则保持原值，否则设置为0。\n\
cv::THRESH_TOZERO_INV：与THRESH_TOZERO相反，像素值大于等于threshValue时设置为0，否则保持原值。",
};
QString opencv::outputName_threshold = "QImage";

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
QStringList opencv::inputNames_BGRtoGRAY = {"QImage"};
QString opencv::outputName_BGRtoGRAY = "QImage";

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
QStringList opencv::inputNames_BGRtoHSV = {"QImage"};
QString opencv::outputName_BGRtoHSV = "QImage";

////////////////////////！！腐蚀！！
QImage opencv::erode(QImage inputImage,int Size_a = 1,int Size_b = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);
    if (Size_a % 2 == 0) Size_a += 1;
    if (Size_b % 2 == 0) Size_b += 1;
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
QStringList opencv::inputNames_erode = {
"inputImage (类型: QImage)\n\
含义：输入的原始图像，类型为QImage，是Qt中常用的图像格式之一。\n\
用途：作为需要进行腐蚀处理的图像，输入图像将在函数内部转换为OpenCV的cv::Mat格式，以便进一步处理。",
"Size_a (类型: int)\n\
含义：腐蚀操作所使用的结构元素矩阵的宽度（即水平方向的尺寸）。\n\
用途：用于确定腐蚀操作中结构元素的宽度，这将影响腐蚀的强度。结构元素矩阵的大小决定了腐蚀操作对图像的局部影响范围。",
"Size_b (类型: int)\n\
含义：腐蚀操作所使用的结构元素矩阵的高度（即垂直方向的尺寸）。\n\
用途：用于确定腐蚀操作中结构元素的高度，同样影响腐蚀效果。结构元素的尺寸决定了在图像上进行腐蚀时，邻域内的像素如何变化。"
};
QString opencv::outputName_erode = "QImage";

////////////////////////！！膨胀！！
QImage opencv::dilate(QImage inputImage,int Size_a = 1,int Size_b = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);
    if (Size_a % 2 == 0) Size_a += 1;
    if (Size_b % 2 == 0) Size_b += 1;
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
QStringList opencv::inputNames_dilate = {
"inputImage (类型: QImage)\n\
含义：输入的原始图像，类型为QImage，它是Qt中常用的图像格式。\n\
用途：这是需要进行膨胀处理的输入图像，函数内部会将其转换为OpenCV的cv::Mat格式，以便进行图像处理。",
"Size_a (类型: int, 默认值: 1)\n\
含义：膨胀操作时所使用的结构元素矩阵的宽度（即水平方向的尺寸）。\n\
用途：控制膨胀操作时结构元素的宽度。这个值越大，膨胀的效果越明显，图像中的亮区域将扩展得更大。",
"Size_b (类型: int, 默认值: 1)\n\
含义：膨胀操作时所使用的结构元素矩阵的高度（即垂直方向的尺寸）。\n\
用途：控制膨胀操作时结构元素的高度。这个值越大，膨胀效果越强，图像中亮区域的扩展程度也越大。"
};
QString opencv::outputName_dilate = "QImage";

////////////////////////！！边缘检测！！
QImage opencv::canny(QImage inputImage, int x = 1, int y = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::Mat edges;
    // 生成新的 Mat 来存储 Canny 输出
    cv::Canny(mat, edges, x, y);

    return CvMatToQImage(edges); // 返回处理后的图像
}

QStringList opencv::inputNames_canny = {
    "inputImage (类型: QImage)\n\
    含义：输入的原始图像，类型为QImage，它是Qt中常用的图像格式。\n\
    用途：这是需要进行Canny边缘检测的输入图像，函数内部会将其转换为OpenCV的cv::Mat格式，以便进行图像处理。",
    "x (类型: int, 默认值: 1)\n\
    含义：Canny算法的低阈值。\n\
    用途：用于设置边缘检测的敏感度，较低的值将允许检测到较多的边缘。",
    "y (类型: int, 默认值: 1)\n\
    含义：Canny算法的高阈值。\n\
    用途：用于设置边缘检测的可靠性，较高的值可以过滤掉不可靠的边缘。"
};

QString opencv::outputName_canny = "QImage";

////////////////////////！！高斯模糊！！
QImage opencv::gaussianblur(QImage inputImage, int Size_a = 1, int Size_b = 1, int a = 0, int b = 0)
{
    // 确保核大小是正的奇数
    if (Size_a <= 0 || Size_b <= 0 || Size_a % 2 == 0 || Size_b % 2 == 0) {
        qWarning("Kernel size must be positive odd numbers.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::Mat mat = QImageToCvMat(inputImage);

    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    cv::Mat blurred;
    // 使用 GaussianBlur 进行模糊
    cv::GaussianBlur(mat, blurred, cv::Size(Size_a, Size_b), a, b);

    return CvMatToQImage(blurred);
}

QStringList opencv::inputNames_gaussianblur = {
    "inputImage (类型: QImage)\n\
    含义：输入的原始图像，类型为QImage，它是Qt中常用的图像格式。\n\
    用途：这是需要进行高斯模糊处理的输入图像，函数内部会将其转换为OpenCV的cv::Mat格式，以便进行图像处理。",

    "Size_a (类型: int, 默认值: 1)\n\
    含义：高斯模糊核的宽度（即水平方向的尺寸）。\n\
    用途：控制高斯模糊核的宽度。模糊核越大，模糊效果越强。必须是正奇数。",

    "Size_b (类型: int, 默认值: 1)\n\
    含义：高斯模糊核的高度（即垂直方向的尺寸）。\n\
    用途：控制高斯模糊核的高度。模糊核越大，模糊效果越强。必须是正奇数。",

    "a (类型: int, 默认值: 0)\n\
    含义：高斯模糊的标准差，在X方向的标准差。\n\
        用途：控制模糊的程度，标准差越大，模糊效果越强。设为0时，OpenCV会自动根据核的大小计算标准差。",

    "b (类型: int, 默认值: 0)\n\
    含义：高斯模糊的标准差，在Y方向的标准差。\n\
        用途：控制模糊的程度，标准差越大，模糊效果越强。设为0时，OpenCV会自动根据核的大小计算标准差。"
};
QString opencv::outputName_gaussianblur = "QImage";

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
QStringList opencv::inputNames_rotateandincline = {
    "inputImage (类型: QImage)\n\
    含义：输入的原始图像，类型为 QImage，表示需要进行旋转和倾斜处理的图像。\n\
        用途：这是函数的输入，函数内部会将其转换为 OpenCV 的 cv::Mat 格式，以便进行图像处理。",

    "center (类型: QPoint)\n\
    含义：旋转的中心点，类型为 QPoint（Qt 中的点类型）。\n\
    用途：指定旋转中心的位置，图像将围绕这个点进行旋转。",

    "angle (类型: double, 默认值: 0.0)\n\
    含义：旋转角度，单位为度（degrees）。\n\
    用途：控制旋转角度，默认值为 0.0，表示不进行旋转。正值表示顺时针旋转，负值表示逆时针旋转。",

    "scale (类型: double, 默认值: 1.0)\n\
    含义：缩放因子。\n\
    用途：控制图像的缩放，1.0 表示不缩放，小于 1.0 表示缩小，超过 1.0 表示放大。"
};
QString opencv::outputName_rotateandincline = "QImage";

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
    cv::Mat edges;
    cv::warpPerspective(mat,edges,martrix,cv::Point(imgsize[0],imgsize[1]));

    return CvMatToQImage(edges);
}
QStringList opencv::inputNames_per_trans = {
    "inputImage (类型: QImage)\n\
    含义：输入的原始图像，类型为 QImage。\n\
    用途：这是需要进行透视变换的输入图像，函数内部会将其转换为 OpenCV 的 cv::Mat 格式，以便进行图像处理。",

    "tl (类型: QPoint)\n\
    含义：图像变换前左上角的点，类型为 QPoint。\n\
    用途：指定透视变换前的图像左上角点坐标。",

    "tr (类型: QPoint)\n\
    含义：图像变换前右上角的点，类型为 QPoint。\n\
    用途：指定透视变换前的图像右上角点坐标。",

    "dl (类型: QPoint)\n\
    含义：图像变换前左下角的点，类型为 QPoint。\n\
    用途：指定透视变换前的图像左下角点坐标。",

    "dr (类型: QPoint)\n\
    含义：图像变换前右下角的点，类型为 QPoint。\n\
    用途：指定透视变换前的图像右下角点坐标。",

    "imgsize (类型: QVector2D)\n\
    含义：变换后图像的大小，类型为 QVector2D，包含宽度和高度。\n\
    用途：指定透视变换后的输出图像大小。"
};
QString opencv::outputName_per_trans = "QImage";

////////////////////////！！缩放大小！！
QImage opencv::resize(QImage inputImage,double x_times = 1,double y_times = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);
    // 确保缩放因子大于 0
    if (x_times <= 0 || y_times <= 0) {
        qWarning("Scale factors must be greater than 0.");
        return QImage(); // 返回一个空的 QImage
    }
    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }
    cv::Mat edges;
    cv::resize(mat,edges,cv::Size(),x_times,y_times);

    return CvMatToQImage(edges);
}
QStringList opencv::inputNames_resize = {
    "inputImage (类型: QImage)\n\
    含义：输入的原始图像，类型为 QImage。\n\
    用途：这是需要进行缩放操作的输入图像，函数内部会将其转换为 OpenCV 的 cv::Mat 格式，以便进行图像处理。",

    "x_times (类型: double, 默认值: 1)\n\
    含义：在水平方向上的缩放倍数。\n\
        用途：控制水平方向的缩放比例，值大于 1.0 时表示放大，值小于 1.0 时表示缩小。",

    "y_times (类型: double, 默认值: 1)\n\
    含义：在垂直方向上的缩放倍数。\n\
        用途：控制垂直方向的缩放比例，值大于 1.0 时表示放大，值小于 1.0 时表示缩小。"
};
QString opencv::outputName_resize = "QImage";

////////////////////////！！画直线！！
QImage opencv::line(QImage inputImage,QPoint start,QPoint end,QColor color,int thickness = 1)
{
    cv::Mat mat = QImageToCvMat(inputImage);
    if (thickness <= 0) {
        qWarning("Thickness must be greater than 0.");
        return QImage(); // 返回一个空的 QImage
    }
    if (mat.empty()) {
        qWarning("Input image is empty or not supported.");
        return QImage(); // 返回一个空的 QImage
    }

    // 数据类型转换
    cv::Point cvstart,cvend;
    cvstart.x = start.x();  cvstart.y = start.y();
    cvend.x = end.x();      cvend.y = end.y();

    cv::Scalar cvcolor(color.red(),color.green(),color.blue());
    // 画线
    cv::line(mat,cvstart,cvend,cvcolor,thickness);

    return CvMatToQImage(mat);
}
QStringList opencv::inputNames_line = {
    "inputImage (类型: QImage)\n\
    含义：输入的原始图像，类型为 QImage。\n\
    用途：这是需要进行直线绘制的输入图像，函数内部会将其转换为 OpenCV 的 cv::Mat 格式，以便进行图像处理。",

    "start (类型: QPoint)\n\
    含义：直线的起点，类型为 QPoint。\n\
    用途：指定直线的起始点坐标。",

    "end (类型: QPoint)\n\
    含义：直线的终点，类型为 QPoint。\n\
    用途：指定直线的结束点坐标。",

    "color (类型: QVector3D)\n\
    含义：直线的颜色，类型为 QVector3D（三个分量代表 R、G、B）。\n\
        用途：控制直线的颜色。",

    "thickness (类型: int, 默认值: 1)\n\
    含义：直线的粗细，类型为 int。\n\
    用途：控制直线的粗细程度，默认为 1。"
};
QString opencv::outputName_Bline = "QImage";

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
