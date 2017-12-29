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

#include "math_utils/linear_regression.h"

#include "math_utils/point.h"

namespace cbr
{

bool run_simplex_test_1(const bool visualize)
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

    for (size_t i = 0; i < 10; ++i) {
        optimizer.Optimize([](const std::vector<double>& params) { 
            return std::sqrt((params[0] - 2.0) * (params[0] - 2.0) + params[1] * params[1]); 
        });
        simplexes.push_back(optimizer.GetSimplex());
    }

    if (visualize) {
        const auto colors = CyclicContainer<viz::Color>::Construct(
            std::vector<viz::Color>{viz::Color::red(), viz::Color::green(), viz::Color::blue()}
        );

        const double dilationFactor = 100.0;
        int i = 0;
        for (const auto& simplex : simplexes) {
            const std::string imageName = STR(__FUNCTION__ << "dbg" << i++);
            viz::Visualizer2D vizWindow(imageName);
            const auto color = colors.Get();
            for (size_t iNode = 0; iNode < simplex.size(); ++iNode) {
                const auto prevPoint = dilationFactor * cv::Point2d(simplex[iNode][0], simplex[iNode][1]);
                const auto nextNodeIdx = ((iNode + 1) == simplex.size() ? 0 : (iNode + 1));
                const auto currPoint = dilationFactor * cv::Point2d(simplex[nextNodeIdx][0], simplex[nextNodeIdx][1]);
                vizWindow.AddLine(prevPoint.x, prevPoint.y, currPoint.x, currPoint.y, color);
            }

            vizWindow.AddArrow(-200., 0., 200., 0., viz::Color::white());
            vizWindow.AddArrow(0., -200., 0., 200., viz::Color::white());

            cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
            const auto image = vizWindow.CreateImage();
            cv::imshow(imageName, image);
        }

        cv::waitKey();
    }

    const auto result = cv::Point2d(optimizer.GetSimplex()[0][0], optimizer.GetSimplex()[0][1]);
    const auto analyticResult = cv::Point2d(2., 0.);

    return cv::norm(result - analyticResult) < 0.05;
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
        return std::sqrt((params[0] - 2.0) * (params[0] - 2.0) + params[1] * params[1]); 
    });
}

bool run_simplex_optimizer_unittest()
{
    run_simplex_test_1(false);
    // run_one_optimization_session();
    return true;
}


void run_simplex_optimizer_playground()
{
    const std::vector<Point> points{
        {58.3968, 80.3811},
        {177.667, 88.487},
        {297.502, 83.7718},
        {407.288, 76.3632},
        {119.904, 83.8463},
        {239.126, 87.7218},
        {354.287, 80.4709}};
    

    const auto fitLine = fit_line(points);
    LOG(DESC(fitLine.P) << ", " << DESC(fitLine.v));
    const std::string imageName = STR(CBR_FANCY_FUNCTION << "dbg");
    viz::Visualizer2D vizWindow(imageName);
    for (const auto& p : points) {
        vizWindow.AddCircle(p.x, p.y, 1, viz::Color::white());
    }
    const auto lineMin = fitLine.At(-fitLine.P.x / fitLine.v.x);
    const auto lineMax = fitLine.At((points.back().x - fitLine.P.x) / fitLine.v.x);
    vizWindow.AddLine(lineMin.x, lineMin.y, lineMax.x, lineMax.y, viz::Color::blue(), 4);
    vizWindow.Spin();

}

}