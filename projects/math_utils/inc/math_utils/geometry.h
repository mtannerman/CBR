#pragma once
#include "opencv2/core.hpp"
#include <array>
#include "math_utils/point.h"

namespace cbr {

template <typename P>
double angle_cosine_between_three_points(P p1, P p2, const P& center)
{
    p1 -= center;
    p2 -= center;
    
    const auto dotProd = double(p1.x * p2.x + p1.y * p2.y);
    const auto normalizationFactor = sqrt(double((p1.x*p1.x + p1.y*p1.y)*(p2.x*p2.x + p2.y*p2.y)) + 1e-10);
    return dotProd / normalizationFactor;
}

bool triangle_contains_point(const std::array<Point, 3>& triangle, const Point& point);
double compute_rotation_angle(const Point& start, const Point& dest);
double arc_tangent(const double sine, const double cosine);
}