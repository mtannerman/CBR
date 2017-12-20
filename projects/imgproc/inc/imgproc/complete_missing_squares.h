#pragma once

#include <vector>
#include <array>
#include "opencv2/core.hpp"
#include "imgproc/square.h"

namespace cbr
{

std::array<cv::Point2d, 2> find_dominant_edgedirections(const std::vector<std::vector<cv::Point>>& squares);
std::vector<Square> complete_missing_squares(const std::vector<Square>& squares);

}