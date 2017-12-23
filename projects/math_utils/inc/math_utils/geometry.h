#pragma once
#include "opencv2/core.hpp"
#include <array>
#include "math_utils/point.h"

namespace cbr {
bool triangle_contains_point(const std::array<Point, 3>& triangle, const Point& point);
double compute_rotation_angle(const Point& start, const Point& dest);
double arc_tangent(const double sine, const double cosine);
}