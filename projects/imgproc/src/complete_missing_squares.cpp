#include "imgproc/complete_missing_squares.h"
#include <array>
#include "common/exceptions.h"
#include <cmath>
#include "math_utils/simplex_optimizer.h"

namespace cbr
{

template<typename T>
struct MinimalValue
{
    MinimalValue(const T& value_)
    {
        value = value_;
    }
    void Min(const T& value_)
    {
        value = std::min(value, value_);
    }

    T value;
};

struct EdgeAngleFitter
{
    std::array<double, 2> Fit(const std::vector<double>& edgeAngles)
    {
        constexpr size_t nParams = 2;
        constexpr size_t nSimplexNodes = 3;
        constexpr double pi = 3.141592653;
        const std::vector<std::vector<double>> initialSimplex{
            std::vector<double>{0.0, pi/3},
            std::vector<double>{pi/3, 2*pi/3},
            std::vector<double>{2*pi/3, pi}
        };

        SimplexOptimizer optimizer(nParams, nSimplexNodes);
        optimizer.GetSimplex() = initialSimplex;
        optimizer.isSimplexInitialized = true;

        const auto optimalAnglePair = optimizer.Optimize(
            [&edgeAngles](const std::vector<double>& anglePair) { 
                return ComputeAnglePairError(anglePair, edgeAngles);
            }
        );

        return std::array<double, 2>{optimalAnglePair[0], optimalAnglePair[1]};
    }

    static double ComputeAnglePairError(
        const std::vector<double>& anglePair, 
        const std::vector<double>& edgeAngles)
    {
        double errorSum = 0.0;
        constexpr double twopi = 6.283185;
        for (const auto& e : edgeAngles) {
            MinimalValue<double> minAngleDiffSquare(std::numeric_limits<double>::max());
            for (int iAngleParam = 0; iAngleParam < 2; ++iAngleParam) {
                minAngleDiffSquare.Min(std::pow(e - anglePair[iAngleParam], 2.0));
                minAngleDiffSquare.Min(std::pow(e - twopi + anglePair[iAngleParam], 2.0));
            }
            errorSum += minAngleDiffSquare.value;
        }

        return errorSum;
    }
};


double to_edgeangle(cv::Point2d p)
{
    const double pNorm = cv::norm(p);
    ASSERT(pNorm != 0.0, "");
    p /= pNorm;
    return std::atan2(p.y, p.x);
}

cv::Point2d angle_to_unitvector(const double edgeAngle)
{
    return cv::Point2d(std::cos(edgeAngle), std::sin(edgeAngle));
}

std::array<double, 2> square_edgeangles(const std::vector<cv::Point>& square)
{
    std::array<double, 2> ret;
    ret[0] = to_edgeangle(cv::Point2d((square[1] - square[0]) + (square[2] - square[3])));
    ret[1] = to_edgeangle(cv::Point2d((square[2] - square[1]) + (square[3] - square[0])));

    return ret;
}

std::array<cv::Point2d, 2> find_dominant_edgedirections(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<double> edgeAngles;
    edgeAngles.reserve(2 * squares.size());
    for (const auto& square : squares) {
        const auto anglePair = square_edgeangles(square);
        edgeAngles.push_back(anglePair[0]);
        edgeAngles.push_back(anglePair[1]);
    }

    const auto bestEdgeAnglePair = EdgeAngleFitter().Fit(edgeAngles);

    return std::array<cv::Point2d, 2>{
        angle_to_unitvector(bestEdgeAnglePair[0]),
        angle_to_unitvector(bestEdgeAnglePair[1])
    };
}

std::vector<std::vector<cv::Point>> complete_missing_squares(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<std::vector<cv::Point>> fullBoard;
    return fullBoard;
}



}   // cbr