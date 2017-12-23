#include "math_utils/geometry.h"
#include "common/exceptions.h"
#include "common/logging.h"
#include "math_utils/line.h"
#include <cmath>

namespace cbr {

double arc_tangent(const double cosine, const double sine)
{
    constexpr double pi = 3.14159265;

    if (cosine == 0.) {
        if (sine > 0.) {
            return pi / 2.;
        }
        return -pi / 2.;
    }

    if (sine >= 0.0) {
        if (cosine >= 0.0) {
            return std::atan(sine / cosine);
        }
        return pi - std::atan(-sine / cosine);
    }
    else {
        if (cosine >= 0.0) {
            return 2*pi - std::atan(-sine / cosine);
        }
        return pi + std::atan(sine / cosine);
    }

    ASSERT(false, "should not reach this");
    return 0.;
}

cv::Matx33f create_coefficient_matrix(
    const std::array<Point, 3>& triangle)
{
    cv::Matx33d ret;
    for (int col = 0; col < 3; ++col) {
        ret(0, col) = triangle[col].x;
        ret(1, col) = triangle[col].y;
        ret(2, col) = 1.;   // requiring combination coeff sum up to 1
    }
    return ret;
}

cv::Matx31f compute_convex_combination_coefficients(
    const cv::Matx33d& coeffMatrix, 
    const Point& point)
{
    cv::Matx31d coeffVector;
    coeffVector(0) = float(point.x);
    coeffVector(1) = float(point.y);
    coeffVector(2) = 1.;

    return coeffMatrix.inv() * coeffVector;
}
    
bool triangle_contains_point(
    const std::array<Point, 3>& triangle,
    const Point& point) 
{
    const auto coeffMatrix = create_coefficient_matrix(triangle);
    ASSERT(std::abs(cv::determinant(coeffMatrix)) > 1e-4, "triangle is degenerate");
    const auto coeffVector = compute_convex_combination_coefficients(coeffMatrix, point);
    
    const auto allElementsArePositive = (coeffVector(0) >= 0.) && (coeffVector(1) >= 0.) && (coeffVector(2) >= 0.);
    return allElementsArePositive;
}

double compute_rotation_angle(const Point& start, const Point& dest)
{
    ASSERT(!start.IsNull() && !dest.IsNull(), "");
    const auto s = start.Normalized();
    const auto d = dest.Normalized();

    const double sine = -s.y * d.x + s.x * d.y;
    const double cosine = s.x * d.x + s.y * d.y;

    return arc_tangent(cosine, sine);
}

}