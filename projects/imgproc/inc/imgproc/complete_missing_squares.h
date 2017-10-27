#pragma once

#include <vector>
#include <array>
#include "opencv2/core.hpp"

namespace cbr
{

std::array<cv::Point2d, 2> find_dominant_edgedirections(const std::vector<std::vector<cv::Point>>& squares);
std::vector<std::vector<cv::Point>> complete_missing_squares(const std::vector<std::vector<cv::Point>>& squares);

}