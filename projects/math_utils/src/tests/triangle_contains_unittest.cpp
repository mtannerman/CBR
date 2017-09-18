#include "math_utils/tests/triangle_contains_unittest.h"
#include "common/viz2d.h"
#include "math_utils/geometry.h"
#include "common/logging.h"

namespace cbr
{
namespace test
{

void run_triangle_contains_point_unittest()
{
    std::vector<cv::Point> square{cv::Point(0, 0), cv::Point(100, 0), cv::Point(100, 100), cv::Point(0, 100)};
    // const std::vector<cv::Point> testPoints{
    //     cv::Point(10, 50)
    // };

    std::vector<cv::Point> testPoints;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            testPoints.push_back(cv::Point(i * 20, j * 20));
        }
    }

    viz::Visualizer2D vizWindow(STR(__FUNCTION__));
    for (const auto& p : square) {
        vizWindow.AddCircle(p, 5, viz::Color::cyan());
    }

    for (const auto& testPoint : testPoints) {
        const auto color = square_contains_point(square, testPoint) ? viz::Color::red() : viz::Color::green();
        vizWindow.AddCircle(testPoint, 5, color);
    }
    vizWindow.Spin();
}

}   // test 
}   // cbr