#pragma once

#include <opencv2/core.hpp>
#include <vector>
#include <utility>
#include "math_utils/square.h"
#include "math_utils/point.h"

namespace cbr
{

std::pair<std::vector<Square>, std::vector<IncompleteSquare>> find_squares(const cv::Mat& image);

}