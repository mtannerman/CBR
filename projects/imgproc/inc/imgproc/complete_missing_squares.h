#pragma once

#include <vector>
#include <array>
#include "opencv2/core.hpp"
#include "math_utils/square.h"
#include "math_utils/point.h"

namespace cbr
{

std::array<Point, 2> find_dominant_edgedirections(const std::vector<Square>& squares);
std::vector<Square> complete_missing_squares(const std::vector<Square>& squares);

}