#pragma once
#include <vector>
#include <opencv2/core.hpp>

namespace cbr 
{
    std::vector<std::vector<cv::Point>> apply_cluster_filtering(
        const std::vector<std::vector<cv::Point>>& squares);
}