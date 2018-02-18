#include "math_utils/matrix.h"
#include "math_utils/point.h"
#include <cmath>

#define FOR_ROW_COL(matrixSize, rowVar, colVar) for (int rowVar = 0; rowVar < matrixSize; ++rowVar) \
 for (int colVar = 0; colVar < matrixSize; ++colVar) 

namespace cbr {

    Matrix::Matrix(const double m11, const double m12,
    const double m21, const double m22)
{
    mat[0][0] = m11;
    mat[0][1] = m12;
    mat[1][0] = m21;
    mat[1][1] = m22;
}

double Matrix::operator()(const int row, const int col) const
{
    return mat[row][col];
}

double& Matrix::operator()(const int row, const int col)
{
    return mat[row][col];
}

Matrix Matrix::Inverse() const
{
    return Matrix(mat[1][1], -mat[0][1], -mat[1][0], mat[0][0]) / Determinant();
}

double Matrix::Determinant() const
{
    return mat[0][0] * mat[1][1] -
        mat[1][0] * mat[0][1];
}

//1e-9

void Matrix::operator/=(const double val)
{
    mat[0][0] /= val;
    mat[1][0] /= val;
    mat[0][1] /= val;
    mat[1][1] /= val;
}

void Matrix::operator*=(const double val)
{
    mat[0][0] *= val;
    mat[1][0] *= val;
    mat[0][1] *= val;
    mat[1][1] *= val;
}

bool Matrix::operator==(const Matrix& other) const
{
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (std::abs(mat[i][j] - other(i,j)) > 1e-9) {
                return false;
            }
        } 
    }

    return true;
}

Matrix operator*(Matrix m, const double val)
{
    m *= val;
    return m;
}

Matrix operator/(Matrix m, const double val)
{
    m /= val;
    return m;
}

Matrix Matrix::Eye()
{
    return Matrix(1., 0., 0., 1.);
}

void Matrix::Chop()
{
    auto maxValue = std::max(std::abs(mat[0][0]), std::abs(mat[0][1]));
    maxValue = std::max(maxValue, std::abs(mat[1][0]));
    maxValue = std::max(maxValue, std::abs(mat[1][1]));

    FOR_ROW_COL(2,row,col) {
        if (mat[row][col] != 0.0 && std::abs(maxValue/mat[row][col]) > 1e6) {
            mat[row][col] = 0.;
        }
    }
}

Matrix Matrix::Chopped() const
{
    auto ret  = *this;
    ret.Chop();
    return ret;
}


Point operator*(const Matrix& m, const Point& p)
{
    return Point(
        m(0,0) * p.x + m(0,1) * p.y,
        m(1,0) * p.x + m(1,1) * p.y);
}

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
    return os << "\n" << m(0, 0) << "," << m(0,1) << "\n" << m(1,0) << "," << m(1,1);
}


Matrix3::Matrix3(const double m11, const double m12, const double m13,
    const double m21, const double m22, const double m23,
    const double m31, const double m32, const double m33)
{
    mat[0][0] = m11; mat[0][1] = m12; mat[0][2] = m13;
    mat[1][0] = m21; mat[1][1] = m22; mat[1][2] = m23;
    mat[2][0] = m31; mat[2][1] = m32; mat[2][2] = m33;
}

double Matrix3::operator()(const int row, const int col) const
{
    return mat[row][col];
}

double& Matrix3::operator()(const int row, const int col)
{
    return mat[row][col];
}

void Matrix3::operator/=(const double val)
{
    mat[0][0] /= val;
    mat[1][0] /= val;
    mat[2][0] /= val;
    mat[0][1] /= val;
    mat[1][1] /= val;
    mat[2][1] /= val;
    mat[0][2] /= val;
    mat[1][2] /= val;
    mat[2][2] /= val;
}

void Matrix3::operator*=(const double val)
{
    mat[0][0] *= val;
    mat[1][0] *= val;
    mat[2][0] *= val;
    mat[0][1] *= val;
    mat[1][1] *= val;
    mat[2][1] *= val;
    mat[0][2] *= val;
    mat[1][2] *= val;
    mat[2][2] *= val;
}

double Matrix3::Determinant() const
{
    return -mat[0][2]*mat[1][1]*mat[2][0] + mat[0][1]*mat[1][2]*mat[2][0] + mat[0][2]*mat[1][0]*mat[2][1] - 
           mat[0][0]*mat[1][2]*mat[2][1] - mat[0][1]*mat[1][0]*mat[2][2] + mat[0][0]*mat[1][1]*mat[2][2];
}

Matrix3 Matrix3::Inverse() const
{
    return Matrix3(
        -mat[1][2]*mat[2][1] + mat[1][1]*mat[2][2], mat[0][2]*mat[2][1] - mat[0][1]*mat[2][2], -mat[0][2]*mat[1][1] + mat[0][1]*mat[1][2],
         mat[1][2]*mat[2][0] - mat[1][0]*mat[2][2], -mat[0][2]*mat[2][0] + mat[0][0]*mat[2][2],  mat[0][2]*mat[1][0] - mat[0][0]*mat[1][2],
        -mat[1][1]*mat[2][0] + mat[1][0]*mat[2][1], mat[0][1]*mat[2][0] - mat[0][0]*mat[2][1], -mat[0][1]*mat[1][0] + mat[0][0]*mat[1][1]) / Determinant();
}

Matrix3 operator*(Matrix3 m, const double val)
{
    m *= val;
    return m;
}

Matrix3 operator/(Matrix3 m, const double val)
{
    m /= val;
    return m;
}

bool Matrix3::operator==(const Matrix3& other) const
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (std::abs(mat[i][j] - other(i,j)) > 1e-9) {
                return false;
            }
        } 
    }

    return true;
}

Matrix3 Matrix3::Eye()
{
    return Matrix3(1., 0., 0., 0., 1., 0., 0., 0., 1.);
}

Point3 operator*(const Matrix3& m, const Point3& p)
{
    return Point3(
        m(0,0) * p.x + m(0,1) * p.y + m(0,2) * p.z,
        m(1,0) * p.x + m(1,1) * p.y + m(1,2) * p.z,
        m(2,0) * p.x + m(2,1) * p.y + m(2,2) * p.z);
}

void Matrix3::Chop()
{
    auto maxValue = mat[0][0];
    FOR_ROW_COL(3,row,col) {
        maxValue = std::max(maxValue, mat[row][col]);
    }

    FOR_ROW_COL(3,row,col) {
        if (mat[row][col] != 0.0 && std::abs(maxValue/mat[row][col]) > 1e6) {
            mat[row][col] = 0.;
        }
    }
}

Matrix3 Matrix3::Chopped() const
{
    auto ret  = *this;
    ret.Chop();
    return ret;
}

std::ostream& operator<<(std::ostream& os, const Matrix3& m)
{
    return os << "\n" << 
        m(0,0) << "," << m(0,1) << "," << m(0,2) << "\n" <<
        m(1,0) << "," << m(1,1) << "," << m(1,2) << "\n" << 
        m(2,0) << "," << m(2,1) << "," << m(2,2) << "";
}

}
