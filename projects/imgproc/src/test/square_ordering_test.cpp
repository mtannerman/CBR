#include "imgproc/test/square_ordering_test.h"
#include "common/viz2d.h"
#include "common/logging.h"
#include "math_utils/functional.h"
#include <array>

namespace cbr
{

void run_square_ordering_test(const std::vector<std::vector<cv::Point>>& squares)
{

    viz::Visualizer2D vizWindow(STR(__FUNCTION__ << "dgb"));

    std::array<cv::Scalar, 3> colors{viz::Color::cyan(), viz::Color::yellow(), viz::Color::magenta()};
    int iColor = 0;

    for (const auto& square : squares) {
        for (int i = 0; i < 4; ++i) {
            const int nextIndex = (i == 3) ? 0 : (i + 1);
            vizWindow.AddArrow(square[i], (2 * square[i] + square[nextIndex]) / 3, colors[iColor]);
            vizWindow.AddLine(square[i], square[nextIndex]);
        }
        iColor = (iColor == 2) ? 0 : (iColor + 1);
    }

    vizWindow.Spin();

}


}   // cbr