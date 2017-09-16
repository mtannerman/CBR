#pragma once
#include "opencv2/core.hpp"
#include <array>

namespace cbr {

bool triangle_contains_point(const std::array<cv::Point, 3>& triangle, const cv::Point& point);
}