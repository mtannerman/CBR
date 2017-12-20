#pragma once

#include <map>
#include <vector>
#include <opencv2/core.hpp>
#include <array>

namespace cbr
{

enum class Edge { LEFT, TOP, RIGHT, BOTTOM };

std::map<Edge, std::vector<std::vector<cv::Point2d>>> find_edge_squares(
    const std::array<cv::Point2d, 2>& dominantEdgeDirections,
    const std::vector<std::vector<cv::Point>>& squares);

}