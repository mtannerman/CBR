#pragma once

#include "math_utils/line.h"
#include "math_utils/square.h"
#include <vector>

namespace cv { class Mat; }

namespace cbr {

std::vector<Line2d> find_incomplete_square_edges(
    const cv::Mat& image,
    const std::vector<Square>& squares);

}