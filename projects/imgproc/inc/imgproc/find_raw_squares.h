#pragma once

#include <vector>
#include "math_utils/square.h"
#include "math_utils/point.h"

namespace cv { class Mat; }

namespace cbr
{

std::vector<Square> find_squares(const cv::Mat& image);

}