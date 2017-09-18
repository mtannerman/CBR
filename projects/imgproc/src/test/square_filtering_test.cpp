#include "imgproc/test/square_filtering_test.h"
#include "math_utils/functional.h"
#include "math_utils/geometry.h"
#include "common/exceptions.h"
#include "common/viz2d.h"

namespace cbr {
namespace test {

size_t count_overlapping_adjusted_squares(
    const std::vector<std::vector<cv::Point>>& adjustedSquares,
    const cv::Point& middle)
{
    return std::count_if(adjustedSquares.begin(), adjustedSquares.end(), 
        [=](const std::vector<cv::Point>& sq)
        { return square_contains_point(sq, middle); });
}

void TEST_square_filtering_overlap(
    const std::vector<std::vector<cv::Point>>& squares,
    const std::vector<std::vector<cv::Point>>& adjustedSquares) 
{
    viz::Visualizer2D vizWindow(STR("dbg" << __FUNCTION__));
    cv::Point maxAxis;
    for (const auto& square : squares) {
        for (const auto& corner : square) {
            maxAxis.x = std::max(corner.x, maxAxis.x);
            maxAxis.y = std::max(corner.y, maxAxis.y);
        }
    }
    maxAxis *= 1.2;
    const auto adjustedSquare = adjustedSquares.front();
    auto color = cv::Scalar(0, 255, 255);
    for (const auto& square : squares) {
        const auto middle = fsum(square) / 4;
        if (square_contains_point(adjustedSquare, middle)) {
            color = cv::Scalar(0, 0, 255);
        }
        vizWindow.AddCircle(middle, 5, color);
    }

    for (const auto& corner : adjustedSquare) {
        vizWindow.AddCircle(corner, 5, cv::Scalar(255, 255, 0));
    }
    vizWindow.Spin();
    THROW("after spin");

    for (const auto square : squares) {
        const cv::Point middle = fsum(square) / 4;
        const auto nOverlappingAdjustedSquares = 
            count_overlapping_adjusted_squares(adjustedSquares, middle);

        ASSERT(nOverlappingAdjustedSquares == 1, STR("square filtering overlap test failed " << DESC(nOverlappingAdjustedSquares)));
    }
}

}   // test
}   // cbr