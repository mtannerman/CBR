#pragma once
#include "opencv2/core.hpp"
#include "common/logging.h"
#include "math_utils/point.h"

namespace cbr
{

struct Line2d
{
    Line2d() = default;
    static Line2d FromTwoPointsOnLine(const Point& p1, const Point& p2);
    static Line2d FromPointAndDirection(const Point& P_, const Point& v_);
    static Line2d FromAngle(const double angle);
    Point At(const double t) const;
    double ClosestTimeArg(const Point& otherPoint) const;
    Point ClosestPoint(const Point& otherPoint) const;
    double DistanceFromPoint(const Point& otherPoint) const;
    Point Intersection(const Line2d& other) const;

    Point P;
    Point v;
};

}