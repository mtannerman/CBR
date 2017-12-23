#pragma once

#include <map>
#include <vector>
#include <opencv2/core.hpp>
#include <array>
#include "math_utils/square.h"

namespace cbr
{

enum class Edge { LEFT, TOP, RIGHT, BOTTOM };

std::map<Edge, std::vector<Square>> find_edge_squares(
    const std::array<cv::Point2d, 2>& dominantEdgeDirections,
    const std::vector<Square>& squares);

}