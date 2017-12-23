#pragma once

#include <functional>
#include <cstddef>
#include <vector>

namespace cbr
{


struct Point
{
    double x = 0.;
    double y = 0.;

    Point() = default;
    Point(const double x, const double y);
    Point(const double val);
    Point(const std::vector<double>& v);

    bool IsNull() const;

    Point Normalized() const;
    double Norm() const;

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

}