#pragma once

#include <opencv2/core.hpp>
#include <vector>
#include "imgproc/square.h"

namespace cbr
{

std::vector<Square> find_squares(const cv::Mat& image);

}