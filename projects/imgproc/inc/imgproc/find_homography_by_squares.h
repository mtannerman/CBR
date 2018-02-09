#pragma once

#include <array>
#include "math_utils/point.h"

namespace cbr
{
    
std::array<std::array<Point, 8>, 8> find_homography_by_squares(const std::vector<Point>& middlePoints);

}   // namespace cbr