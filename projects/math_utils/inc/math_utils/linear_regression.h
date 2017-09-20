#pragma once

#include "math_utils/line.h"
#include <vector>


namespace cbr
{
    Line2d<int> fit_line(const std::vector<cv::Point2d>& points);
}