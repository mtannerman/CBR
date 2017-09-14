#pragma once

#include <opencv2/core.hpp>
#include <vector>

namespace cbr
{

std::vector<std::vector<cv::Point>> find_squares(const cv::Mat& image);

}