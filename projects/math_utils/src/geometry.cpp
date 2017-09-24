#include "math_utils/geometry.h"
#include "common/exceptions.h"
#include "common/logging.h"
#include "math_utils/line.h"

namespace cbr {

cv::Matx33f create_coefficient_matrix(
    const std::array<cv::Point2f, 3>& triangle)
{
    cv::Matx33f ret;
    for (int col = 0; col < 3; ++col) {
        ret(0, col) = triangle[col].x;
        ret(1, col) = triangle[col].y;
        ret(2, col) = 1.0f;   // requiring combination coeff sum up to 1
    }
    return ret;
}

cv::Matx31f compute_convex_combination_coefficients(
    const cv::Matx33f& coeffMatrix, 
    const cv::Point& point)
{
    cv::Matx31f coeffVector;
    coeffVector(0) = float(point.x);
    coeffVector(1) = float(point.y);
    coeffVector(2) = 1.0f;

    return coeffMatrix.inv() * coeffVector;
}
    
bool triangle_contains_point(
    const std::array<cv::Point, 3>& triangle,
    const cv::Point& point) 
{
    const std::array<cv::Point2f, 3> fpTriangle{cv::Point2f(triangle[0]), cv::Point2f(triangle[1]), cv::Point2f(triangle[2])};
    const auto coeffMatrix = create_coefficient_matrix(fpTriangle);
    ASSERT(std::abs(cv::determinant(coeffMatrix)) > 1e-4, "triangle is degenerate");
    const auto coeffVector = compute_convex_combination_coefficients(coeffMatrix, point);
    
    const auto allElementsArePositive = (coeffVector(0) >= 0.0f) && (coeffVector(1) >= 0.0f) && (coeffVector(2) >= 0.0f);
    return allElementsArePositive;
}

bool square_contains_point(
    const std::vector<cv::Point>& square,
    const cv::Point& point)
{
    for (const int pointLeftOut : {0, 1, 2, 3}) {
        std::array<cv::Point, 3> triangle;
        int triangleIndex = 0;
        for (int i = 0; i < 4; ++i) {
            if (i != pointLeftOut) {
                triangle[triangleIndex++] = square[i];
            }
        }
        if (triangle_contains_point(triangle, point)) {
            return true;
        }
    }
    
    return false;
}


double compute_square_point_distance(const std::vector<cv::Point>& square, const cv::Point& point)
{
    if (square_contains_point(square, point)) {
        return 0.0;
    }

    double ret = std::numeric_limits<double>::max();
    for (int iCorner = 0; iCorner < 4; ++iCorner) {
        const int iNextCorner = (iCorner == 3 ? 0 : (iCorner + 1));
        const auto edge = Line2d<int>::FromTwoPointsOnLine(square[iCorner], square[iNextCorner]);
        auto tMin = edge.ClosestTimeArg(point);
        tMin = std::min(1.0, tMin);
        tMin = std::max(0.0, tMin);
        const auto closestEdgePoint = edge.At(tMin);
        const auto distance = cv::norm(closestEdgePoint - cv::Point2d(point));
        ret = std::min(ret, distance);
    }

    return ret;
}

}