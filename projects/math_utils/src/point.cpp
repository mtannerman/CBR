#include "math_utils/point.h"
#include "common/exceptions.h"
#include <cmath>

namespace cbr
{

Point::Point(const double x, const double y)
    : x(x), y(y)
{ }

Point::Point(const double val)
    : x(val), y(val)
{ }

Point::Point(const std::vector<double>& v)
{
    ASSERT(v.size() == 2, "");
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
    return x == 0. && y == 0.;
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
        case 0:
            return x;
        case 1:
            return y;
        default:
            THROW(STR("index mismatch for point: idx = " << idx));
    }
    // unreacheable code
    return x;
}

double& Point::operator[](const size_t idx)
{
    switch (idx) {
        case 0:
            return x;
        case 1:
            return y;
        default:
            THROW(STR("index mismatch for point: idx = " << idx));
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
    return x == other.x && y == other.y;
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

}