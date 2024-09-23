#include "math.h"
#include <stdlib.h>
Math::Math()
{

}

// 加法
double Math::add(double a, double b) {
    return a + b;
}

// 减法
double Math::subtract(double a, double b) {
    return a - b;
}

// 乘法
double Math::multiply(double a, double b) {
    return a * b;
}

// 除法，注意处理除零情况
double Math::divide(double a, double b) {
    if (b == 0) {
        return 0;
    }
    return static_cast<double>(a) / b;
}

// 开方，注意处理负数情况
double Math::sqrt(double a) {
    if (a < 0) {
        return 0;
    }
    return sqrt(static_cast<double>(a));
}

double Math::pow(double base, double exponent)
{
    return pow(static_cast<double>(base), static_cast<double>(exponent));
}

