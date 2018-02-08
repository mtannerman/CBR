#pragma once

#include <functional>
#include <cstddef>
#include <vector>
#include <ostream>

namespace cbr
{


struct Point
{
    double x = 0.;
    double y = 0.;

    Point() = default;
    Point(const double x, const double y);
    Point(const std::vector<double>& v);

    static Point UnitVector(const size_t i);

    bool IsNull() const;
    void ZeroOut();

    void AlignTo(const Point& p);
    Point Aligned(const Point& p) const;

    double PolarAngle() const;

    Point Normalized() const;
    double Norm() const;
    double SquaredNorm() const;

    void Normalize();

    Point& Transform(std::function<void(const double&)> f);
    double Distance(Point other) const;
    const double& operator[](const size_t idx) const;
    double& operator[](const size_t idx);

    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);

    Point& operator*=(const double d);
    Point operator*(const double d) const;
    Point& operator/=(const double d);
    Point operator/(const double d) const;
    
    bool operator==(const Point& other) const;

    double Dot(const Point& other) const;

    bool IsInVicinity(const Point& other, const double radius) const;
    bool IsCloserThan(const Point& other, const double radius) const;
};

Point operator+(Point lhs, const Point& rhs);
Point operator-(Point lhs, const Point& rhs);
Point operator*(const double d, const Point& p);

std::ostream& operator<<(std::ostream& os, const Point& p);

}