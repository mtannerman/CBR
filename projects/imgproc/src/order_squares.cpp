#include "imgproc/order_squares.h"
#include "math_utils/functional.h"

namespace cbr
{

void order_square(std::vector<cv::Point>& square)
{
    const auto middle = cv::Point2d(fsum(square)) / 4.0;
    
}

void order_squares(std::vector<std::vector<cv::Point>>& squares)
{
}


}   // cbr