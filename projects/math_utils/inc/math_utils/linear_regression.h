#pragma once

#include "math_utils/line.h"
#include <vector>


namespace cbr
{
    Line2d fit_line(const std::vector<Point>& points);
    Line2d fit_line_gradientsearch(const std::vector<Point>& points);
}