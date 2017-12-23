#pragma once

#include <opencv2/core.hpp>
#include <vector>
#include "math_utils/square.h"
#include "math_utils/point.h"

namespace cbr
{

std::vector<Square> find_squares(const cv::Mat& image);

}