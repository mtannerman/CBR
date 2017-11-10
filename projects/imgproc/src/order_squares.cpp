#include "imgproc/order_squares.h"
#include "math_utils/functional.h"
#include "math_utils/geometry.h"

namespace cbr
{

void order_square(std::vector<cv::Point>& square)
{
    const auto referencePoint = square[0];

    std::sort(square.begin() + 1, square.end(), 
    [=](const cv::Point& c1, const cv::Point& c2) { 
        return compute_rotation_angle(referencePoint, c1) < compute_rotation_angle(referencePoint, c2); });
    
}

void order_squares(std::vector<std::vector<cv::Point>>& squares)
{
    for (auto& square : squares) {
        order_square(square);
    }
}


}   // cbr