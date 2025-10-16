#ifndef MATH_H
#define MATH_H


class Math
{
public:
    Math();
    static double add(double a, double b);
    static double subtract(double a, double b);
    static double multiply(double a, double b);
    static double divide(double a, double b);  // 返回浮点数结果
    static double sqrt(double a);  // 开平方
    static double pow(double base, double exponent);  // 幂运算
};

#endif // MATH_H
