#pragma once

#include <vector>
#include "opencv2/core.hpp"

namespace cbr
{

std::vector<std::vector<cv::Point>> complete_missing_squares(const std::vector<std::vector<cv::Point>>& squares);

}