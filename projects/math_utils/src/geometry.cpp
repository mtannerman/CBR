#include "math_utils/geometry.h"
#include "common/exceptions.h"

namespace cbr {

cv::Matx33f create_coefficient_matrix(const std::array<cv::Point2f, 3>& triangle)
{
    cv::Matx33f ret;
    for (int col = 0; col < 3; ++col) {
        ret(0, col) = triangle[col].x;
        ret(1, col) = triangle[col].y;
        ret(2, col) = triangle[col].z;
    }
    return ret;
}

// cv::Matx31f compute_convex_combination_vector(const cv::Matx33f& coeffMatrix, const cv::Point& point)
// {
//     cv::Matx31f coeffVector;
//     coeffVector(0) = float(point.x);
//     coeffVector(1) = float(point.y);
//     coeffVector(2) = float(point.z);

//     return coeffMatrix * coeffVector;
// }
    
bool triangle_contains_point(const std::array<cv::Point, 3>& triangle,
    const cv::Point& point) 
{
    const std::array<cv::Point2f, 3> fpTriangle(cv::Point2f(triangle[0]), cv::Point2f(triangle[1]), cv::Point2f(triangle[2]));
    const auto coeffMatrix = create_coefficient_matrix(fpTriangle);
    ASSERT(cv::determinant(coeffMatrix) > 1e-4, "triangle is degenerate");
    
    return false;
}
}