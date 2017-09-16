#pragma once
#include <vector>
#include "opencv2/core.hpp"

// #define CBR_VISUALIZE_FAILING_SQUARE_FILTERING_TEST

#ifdef CBR_VISUALIZE_FAILING_SQUARE_FILTERING_TEST
#include "common/viz2d.h"
#endif

namespace cbr
{
namespace test
{
void TEST_square_filtering_overlap(
    const std::vector<std::vector<cv::Point>> &squares,
    const std::vector<std::vector<cv::Point>> &adjustedSquares);
}
}