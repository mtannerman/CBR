#pragma once

#include <vector>
#include <opencv2/core.hpp>

namespace cbr
{

std::vector<std::vector<cv::Point>> apply_size_filtering(
    const std::vector<std::vector<cv::Point>>& squares,
    const int nImageRows, const int nImageCols);

}