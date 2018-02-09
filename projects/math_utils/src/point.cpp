#include "math_utils/point.h"
#include "common/exceptions.h"
#include <cmath>

namespace cbr
{

// Point =================================================

Point::Point(const double x, const double y)
    : x(x), y(y)
{ }

Point::Point(const std::vector<double>& v)
{
    THROW_IF(v.size() != 2LL, SizeMismatch, "");
    x = v[0];
    y = v[1]; 
}

Point Point::UnitVector(const size_t i)
{
    Point ret;
    ret[i] = 1.;
    return ret;
}

Point& Point::Transform(std::function<void(const double&)> f)
{
    f(x); f(y);
    return *this;
}

bool Point::IsNull() const
{
    return Norm() < 1e-9;
}

void Point::ZeroOut()
{
    x = 0.;
    y = 0.;
}

void Point::AlignTo(const Point& p)
{
    if (Dot(p) < 0.) {
        x = -x;
        y = -y;
    }
}

Point Point::Aligned(const Point& p) const
{
    auto ret = *this;
    ret.AlignTo(p);
    return ret;
}

double Point::PolarAngle() const
{
    return std::atan2(y, x);
}

double Point::SquaredNorm() const
{
    return x * x + y * y;
}

double Point::Norm() const
{
    return std::sqrt(SquaredNorm());
}

Point Point::Normalized() const
{
    return *this / Norm();
}

void Point::Normalize()
{
    *this /= Norm();
}

double Point::Distance(Point other) const
{
    other.x -= x;
    other.y -= y;
    return other.Norm();
}

const double& Point::operator[](const size_t idx) const
{
    switch (idx) {
        case 0LL:
            return x;
        case 1LL:
            return y;
        default:
            THROW(IndexError, STR("index mismatch for point: idx = " << idx));
    }
    // unreacheable code
    return x;
}

double& Point::operator[](const size_t idx)
{
    switch (idx) {
        case 0LL:
            return x;
        case 1LL:
            return y;
        default:
            THROW(IndexError, STR("index mismatch for point: idx = " << idx));
    }
    // unreacheable code
    return x;
}

Point& Point::operator+=(const Point& other)
{
    x += other.x; 
    y += other.y;
    return *this;
}

Point& Point::operator-=(const Point& other)
{
    x -= other.x; 
    y -= other.y;
    return *this;
}

bool Point::operator==(const Point& other) const
{
    return (*this-other).IsNull();
}

bool Point::IsInVicinity(const Point& other, const double radius) const
{
    return Distance(other) <= radius;
}

bool Point::IsCloserThan(const Point& other, const double radius) const
{
    return Distance(other) < radius;
}

Point& Point::operator*=(const double d)
{
    x *= d; 
    y *= d;
    return *this;
}

Point Point::operator*(const double d) const
{
    return Point(x * d, y * d);
}

Point& Point::operator/=(const double d)
{
    x /= d; 
    y /= d;
    return *this;
}

Point Point::operator/(const double d) const
{
    return Point(x / d, y / d);
}

double Point::Dot(const Point& other) const
{
    return x * other.x + y * other.y;
}

Point operator*(const double d, const Point& p)
{
    return p * d;
}

Point operator+(Point lhs, const Point& rhs)
{
    lhs += rhs;
    return lhs;
}

Point operator-(Point lhs, const Point& rhs)
{
    lhs -= rhs;
    return lhs;
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
    return os << '[' << p.x << ", " << p.y << ']';
}

// Point3 =================================================

Point3::Point3(const double x, const double y, const double z)
    : x(x), y(y), z(z)
{ }

Point3::Point3(const std::vector<double>& v)
{
    THROW_IF(v.size() != 3LL, SizeMismatch, "");
    x = v[0];
    y = v[1]; 
    z = v[2]; 
}

Point3 Point3::UnitVector(const size_t i)
{
    Point3 ret;
    ret[i] = 1.;
    return ret;
}

Point3& Point3::Transform(std::function<void(const double&)> f)
{
    f(x); f(y); f(z);
    return *this;
}

bool Point3::IsNull() const
{
    return Norm() < 1e-9;
}

void Point3::ZeroOut()
{
    x = 0.;
    y = 0.;
    z = 0.;
}

void Point3::AlignTo(const Point3& p)
{
    if (Dot(p) < 0.) {
        x = -x;
        y = -y;
        z = -z;
    }
}

Point3 Point3::Aligned(const Point3& p) const
{
    auto ret = *this;
    ret.AlignTo(p);
    return ret;
}

double Point3::SquaredNorm() const
{
    return x*x + y*y + z*z;
}

double Point3::Norm() const
{
    return std::sqrt(SquaredNorm());
}

Point3 Point3::Normalized() const
{
    return *this / Norm();
}

void Point3::Normalize()
{
    *this /= Norm();
}

double Point3::Distance(Point3 other) const
{
    other.x -= x;
    other.y -= y;
    other.z -= z;
    return other.Norm();
}

const double& Point3::operator[](const size_t idx) const
{
    switch (idx) {
        case 0LL:
            return x;
        case 1LL:
            return y;
        case 2LL:
            return z;
        default:
            THROW(IndexError, STR("index mismatch for point: idx = " << idx));
    }
    // unreacheable code
    return x;
}

double& Point3::operator[](const size_t idx)
{
    switch (idx) {
        case 0LL:
            return x;
        case 1LL:
            return y;
        case 2LL:
            return z;
        default:
            THROW(IndexError, STR("index mismatch for point: idx = " << idx));
    }
    // unreacheable code
    return x;
}

Point3& Point3::operator+=(const Point3& other)
{
    x += other.x; 
    y += other.y;
    z += other.z;
    return *this;
}

Point3& Point3::operator-=(const Point3& other)
{
    x -= other.x; 
    y -= other.y;
    z -= other.z;
    return *this;
}

bool Point3::operator==(const Point3& other) const
{
    return (*this-other).IsNull();
}

bool Point3::IsInVicinity(const Point3& other, const double radius) const
{
    return Distance(other) <= radius;
}

bool Point3::IsCloserThan(const Point3& other, const double radius) const
{
    return Distance(other) < radius;
}

Point3& Point3::operator*=(const double d)
{
    x *= d; 
    y *= d;
    z *= d;
    return *this;
}

Point3 Point3::operator*(const double d) const
{
    return Point3(x * d, y * d, z * d);
}

Point3& Point3::operator/=(const double d)
{
    x /= d; 
    y /= d;
    z /= d;
    return *this;
}

Point3 Point3::operator/(const double d) const
{
    return Point3(x / d, y / d, z / d);
}

double Point3::Dot(const Point3& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Point3 Point3::Cross(const Point3& other) const
{
    return Point3(y*other.z - z*other.y,
                  z*other.x - x*other.z,
                  x*other.y - y*other.x);
}

Point3 operator*(const double d, const Point3& p)
{
    return p * d;
}

Point3 operator+(Point3 lhs, const Point3& rhs)
{
    lhs += rhs;
    return lhs;
}

Point3 operator-(Point3 lhs, const Point3& rhs)
{
    lhs -= rhs;
    return lhs;
}

std::ostream& operator<<(std::ostream& os, const Point3& p)
{
    return os << '[' << p.x << ", " << p.y << ", " << p.z << ']';
}

}
