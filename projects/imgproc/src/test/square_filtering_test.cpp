#include "imgproc/test/square_filtering_test.h"
#include "math_utils/functional.h"
#include "math_utils/geometry.h"
#include "common/exceptions.h"

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
    for (const auto square : squares) {
        const cv::Point middle = fsum(square) / 4;
        const auto nOverlappingAdjustedSquares = 
            count_overlapping_adjusted_squares(adjustedSquares, middle);

        ASSERT(nOverlappingAdjustedSquares == 1, "");
    }
}

}   // test
}   // cbr