#pragma once

#include <ostream>
#include <array>
#include "math_utils/point.h"

namespace cbr {

struct Matrix
{
    Matrix() = default;
    Matrix(const double m11, const double m12,
        const double m21, const double m22);

    double operator()(const int row, const int col) const;
    double& operator()(const int row, const int col);

    void operator/=(const double val);
    void operator*=(const double val);

    bool operator==(const Matrix& other) const;

    static Matrix Eye();

    Matrix Inverse() const;

    double Determinant() const;

    void Chop();
    Matrix Chopped() const;

    std::array<std::array<double, 2>, 2> mat;
};

Matrix operator/(Matrix m, const double val);
Matrix operator*(Matrix m, const double val);
Point operator*(const Matrix& m, const Point& p);
std::ostream& operator<<(std::ostream& os, const Matrix& m);

struct Matrix3
{
    Matrix3() = default;
    Matrix3(const double m11, const double m12, const double m13,
        const double m21, const double m22, const double m23,
        const double m31, const double m32, const double m33);

    double operator()(const int row, const int col) const;
    double& operator()(const int row, const int col);

    void operator/=(const double val);
    void operator*=(const double val);

    bool operator==(const Matrix3& other) const;

    static Matrix3 Eye();

    Matrix3 Inverse() const;

    double Determinant() const;

    void Chop();
    Matrix3 Chopped() const;

    std::array<std::array<double, 3>, 3> mat;
};

Matrix3 operator/(Matrix3 m, const double val);
Matrix3 operator*(Matrix3 m, const double val);
Point3 operator*(const Matrix3& m, const Point3& p);
std::ostream& operator<<(std::ostream& os, const Matrix3& m);

}   // namespace cbr