#include "math_utils/tests/simplex_optimizer_unittest.h"

#include "opencv2/core.hpp"
#include <vector>
#include "common/exceptions.h"
#include "common/logging.h"
#include <sstream>
#include <functional>
#include "common/viz2d.h"
#include "common/cyclic_container.h"
#include "opencv2/highgui.hpp"

namespace cbr
{

void visualize_consecutive_simplexes()
{

    std::vector<std::vector<std::vector<double>>> simplexes;
    
    std::vector<std::vector<double>> initialSimplex(3, std::vector<double>(2));
    initialSimplex[0] = std::vector<double>{-1.0, -1.0};
    initialSimplex[1] = std::vector<double>{1.0, -1.0};
    initialSimplex[2] = std::vector<double>{0.0, 1.0};

    simplexes.push_back(initialSimplex);

    SimplexOptimizer optimizer(2, 3);
    optimizer.GetSimplex() = initialSimplex;

    optimizer.isSimplexInitialized = true;
    optimizer.config.maxIterations = 1;

    for (size_t i = 0; i < 4; ++i) {
        optimizer.Optimize([](const std::vector<double>& params) { 
            return std::sqrt(params[0] * params[0] + params[1] * params[1]); 
        });
        simplexes.push_back(optimizer.GetSimplex());
    }

    const auto colors = CyclicContainer<cv::Scalar>::Construct(
        std::vector<cv::Scalar>{viz::Color::red(), viz::Color::green(), viz::Color::blue()}
    );

    const double dilationFactor = 100.0;
    int i = 0;
    for (const auto& simplex : simplexes) {
        const std::string imageName = STR(__FUNCTION__ << "dbg" << i++);
        viz::Visualizer2D vizWindow(imageName);
        const auto color = colors.Get();
        for (size_t iNode = 0; iNode < simplex.size(); ++iNode) {
            const auto prevPoint = cv::Point(dilationFactor * cv::Point2d(simplex[iNode][0], simplex[iNode][1]));
            const auto nextNodeIdx = ((iNode + 1) == simplex.size() ? 0 : (iNode + 1));
            const auto currPoint = cv::Point(dilationFactor * cv::Point2d(simplex[nextNodeIdx][0], simplex[nextNodeIdx][1]));
            vizWindow.AddLine(prevPoint, currPoint, color);
        }

        vizWindow.AddArrow(cv::Point(-200, 0), cv::Point(200, 0), viz::Color::white());
        vizWindow.AddArrow(cv::Point(0, -200), cv::Point(0, 200), viz::Color::white());

        cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
        const auto image = vizWindow.CreateImage();
        cv::imshow(imageName, image);
    }
    
    cv::waitKey();
}

void run_one_optimization_session()
{
    std::vector<std::vector<std::vector<double>>> simplexes;
    
    std::vector<std::vector<double>> initialSimplex(3, std::vector<double>(2));
    initialSimplex[0] = std::vector<double>{-1.0, -1.0};
    initialSimplex[1] = std::vector<double>{1.0, -1.0};
    initialSimplex[2] = std::vector<double>{0.0, 1.0};

    simplexes.push_back(initialSimplex);

    SimplexOptimizer optimizer(2, 3);
    optimizer.GetSimplex() = initialSimplex;

    optimizer.isSimplexInitialized = true;

    optimizer.Optimize([](const std::vector<double>& params) { 
        return std::sqrt(params[0] * params[0] + params[1] * params[1]); 
    });
}

void run_simplex_optimizer_unittest()
{
    visualize_consecutive_simplexes();
    // run_one_optimization_session();
}

}