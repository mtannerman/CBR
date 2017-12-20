#pragma once

#include <array>
#include <vector>
#include "opencv2/core.hpp"

namespace cbr
{

struct Square
{
    Square() = default;
    Square(const std::vector<cv::Point>& arg_corners); 

    double Circumference() const;
    std::array<cv::Point2d, 4> corners;
    cv::Point2d middle;
};

}