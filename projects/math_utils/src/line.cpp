#include "math_utils/line.h"


namespace cbr
{

Line2d Line2d::FromTwoPointsOnLine(const Point& p1, const Point& p2)
{
    Line2d ret;
    ret.P = p1;
    ret.v = p2 - p1;
    return ret;
}

Line2d Line2d::FromPointAndDirection(const Point& P_, const Point& v_)
{
    Line2d ret;
    ret.P = P_;
    ret.v = v_;
    return ret;
}

Line2d Line2d::FromAngle(const double angle)
{
    Line2d ret;
    ret.P = Point();
    ret.v = Point(std::cos(angle), std::sin(angle));
    return ret;
}

Line2d Line2d::FromPolarCoordinates(const double rho, const double theta)
{
    const auto a = std::cos(theta); 
    const auto b = std::sin(theta);
    const auto x = a * rho;
    const auto y = b * rho;
    const auto startPoint = Point(x - b, y + a);
    const auto endPoint = Point(x + b, y - a);
    return FromTwoPointsOnLine(startPoint, endPoint);
}

Point Line2d::At(const double t) const
{
    return P + t * v;
}

double Line2d::ClosestTimeArg(const Point& otherPoint) const
{
    const double numerator = (otherPoint - P).Dot(v);
    const double denomRoot = v.Norm();
    return numerator / (denomRoot * denomRoot);
}

Point Line2d::ClosestPoint(const Point& otherPoint) const
{
    const double tMin = ClosestTimeArg(otherPoint);
    return P + v * tMin;
}

double Line2d::DistanceFromPoint(const Point& otherPoint) const
{
    const double tMin = ClosestTimeArg(otherPoint);
    return (otherPoint - P - (v * tMin)).Norm();
}

Point Line2d::Intersection(const Line2d& other) const
{
    const auto det = v.x * other.v.y - v.y * other.v.x;
    if (std::abs(det) < 1e-7) return Point(1e50, 1e50);
    const auto timeArg = (other.v.y * (other.P.x - P.x) - other.v.x * (other.P.y - P.y)) / det;
    return At(timeArg);
}

}