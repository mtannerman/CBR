#include "imgproc/square.h"
#include "common/exceptions.h"
#include "math_utils/functional.h"

namespace cbr
{

Square::Square(const std::vector<cv::Point>& arg_corners)
{
    ASSERT(arg_corners.size() == 4, "");
    for (int i = 0; i < 4; ++i) {
        corners[i].x = double(arg_corners[i].x);
        corners[i].y = double(arg_corners[i].y);
    }
    middle = fsum(corners) / 4.;
}

double Square::Circumference() const
{
    return cv::norm(corners[0] - corners[1]) + 
        cv::norm(corners[1] - corners[2]) + 
        cv::norm(corners[2] - corners[3]) + 
        cv::norm(corners[3] - corners[0]);
}

}