#include "math_utils/square.h"
#include "common/exceptions.h"
#include "math_utils/functional.h"
#include "math_utils/geometry.h"
#include "math_utils/line.h"

namespace cbr
{

Point& Square::operator[](const size_t idx)
{
    return corners[idx];
}

const Point& Square::operator[](const size_t idx) const
{
    return corners[idx];
}

void Square::Recenter()
{
    middle = f::sum(corners) / 4.;
}

// Square::Square(const std::vector<cv::Point>& arg_corners)
// {
//     ASSERT(arg_corners.size() == 4, "");
//     for (int i = 0; i < 4; ++i) {
//         corners[i].x = double(arg_corners[i].x);
//         corners[i].y = double(arg_corners[i].y);
//     }
//     Recenter();
// }



bool Square::Contains(const Point& p) const
{
    for (const int pointLeftOut : {0, 1, 2, 3}) {
        std::array<Point, 3> triangle;
        int triangleIndex = 0;
        for (int i = 0; i < 4; ++i) {
            if (i != pointLeftOut) {
                triangle[triangleIndex++] = corners[i];
            }
        }
        if (triangle_contains_point(triangle, p)) {
            return true;
        }
    }
    
    return false;
}

double Square::Distance(const Point& p) const
{
    if (Contains(p)) {
        return 0.0;
    }

    double ret = std::numeric_limits<double>::max();
    for (int iCorner = 0; iCorner < 4; ++iCorner) {
        const int iNextCorner = (iCorner == 3 ? 0 : (iCorner + 1));
        const auto edge = Line2d::FromTwoPointsOnLine(corners[iCorner], corners[iNextCorner]);
        auto tMin = edge.ClosestTimeArg(p);
        tMin = std::min(1.0, tMin);
        tMin = std::max(0.0, tMin);
        const auto closestEdgePoint = edge.At(tMin);
        const auto distance = closestEdgePoint.Distance(p);
        ret = std::min(ret, distance);
    }

    return ret;
}

double Square::Circumference() const
{
    return corners[0].Distance(corners[1]) + 
           corners[1].Distance(corners[2]) + 
           corners[2].Distance(corners[3]) + 
           corners[3].Distance(corners[0]);
}

}