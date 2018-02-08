#include "math_utils/linear_regression.h"
#include "common/exceptions.h"
#include "math_utils/simplex_optimizer.h"

namespace cbr
{


Line2d InitializeLine(const std::vector<Point>& points)
{
    const auto xMinMax = std::minmax_element(points.begin(), points.end(), [](const Point& p1, const Point& p2){ return p1.x < p2.x; });
    const auto yMinMax = std::minmax_element(points.begin(), points.end(), [](const Point& p1, const Point& p2){ return p1.y < p2.y; });

    const double xMin = xMinMax.first->x;
    const double xMax = xMinMax.second->x;
    const double yMin = xMinMax.first->y;
    const double yMax = xMinMax.second->y;

    return Line2d::FromTwoPointsOnLine(Point(xMin, yMin), Point(xMax, yMax));
}

struct LineFitterWithSimplexOptimization
{
    Line2d Fit(const std::vector<Point>& points);
    Line2d line;
};

std::vector<double> line_to_vector(const Line2d& line)
{
    return std::vector<double>{line.P.x, line.P.y, line.v.x, line.v.y};
}

Line2d vector_to_line(const std::vector<double>& v)
{
    return Line2d::FromPointAndDirection(Point(v[0], v[1]), Point(v[2], v[3]));
}

Line2d LineFitterWithSimplexOptimization::Fit(
    const std::vector<Point>& points)
{
    ASSERT(!points.empty(), "Not enough points for fitting");
    if (points.size() > 1) {
        line = InitializeLine(points);
        std::vector<std::vector<double>> initialSimplex(5, line_to_vector(line));
        for (int i = 1; i < 5; ++i) {
            initialSimplex[i][i - 1] *= -1.;
        }

        SimplexOptimizer optimizer(4, 5);
        optimizer.GetSimplex() = initialSimplex;
        optimizer.isSimplexInitialized = true;
        optimizer.config.maxIterations = 40;

        optimizer.Optimize([&points](const std::vector<double>& p) {
            double sum = 0.;
            const double& Px = p[0];
            const double& Py = p[1];
            const double& vx = p[2];
            const double& vy = p[3];
            for (const auto& point : points) {
                double dotProd = ((point.x - Px) * vx + (point.y - Py) * vy) / (vx * vx + vy * vy);
    
                const double xErr = Px - point.x + vx * dotProd;
                const double yErr = Py - point.y + vy * dotProd;
                sum += xErr * xErr + yErr * yErr;
            }
            return sum;
        });
        line = vector_to_line(optimizer.GetSimplex().front());
        line.v.Normalize();
    }
    else {
        line.P = points.front();
        line.v.ZeroOut();
    }
    return line;
}

struct LineFitterWithGradientSearch
{
    Line2d Fit(const std::vector<Point>& points);
    void UpdateGradients(const std::vector<Point>& points);
    void UpdateLineParameters(const std::vector<Point>& points);

    void UpdateError(const std::vector<Point>& points);
    
    Line2d line; 

    Point ComputeError(const Point& point);

    Point gradP;
    Point gradv;
};

Point LineFitterWithGradientSearch::ComputeError(const Point& point)
{
    return line.P - point + 
        line.v * (point - line.P).Dot(line.v) / line.v.SquaredNorm();
}

void LineFitterWithGradientSearch::UpdateGradients(const std::vector<Point>& points)
{
    gradP.ZeroOut();
    gradv.ZeroOut();

    for (size_t i = 0; i < points.size(); ++i) {
        const auto error = ComputeError(points[i]);
        const auto v_over_v_squared = line.v / line.v.SquaredNorm();
        for (const size_t alpha : {0, 1}) {
            gradP[alpha] += error.Dot(Point::UnitVector(alpha) + line.v[alpha] * v_over_v_squared);
        }

        const auto r = points[i] - line.P;
        const auto one_over_v4 = std::pow(line.v.SquaredNorm(), -2.);
        const auto v_square_diff = line.v.x * line.v.x - line.v.y * line.v.y;
        const Point xDerivative(
            line.v.y * (2. * r.x * line.v.x * line.v.y - r.y * v_square_diff),
            line.v.y * (-2. * r.y * line.v.x * line.v.y - r.x * v_square_diff)
        );

        const Point yDerivative(
            line.v.x * (-2. * r.x * line.v.x * line.v.y + r.y * v_square_diff),
            line.v.x * (2. * r.y * line.v.x * line.v.y + r.x * v_square_diff)
        );

        gradv.x += one_over_v4 * error.Dot(xDerivative); 
        gradv.y += one_over_v4 * error.Dot(yDerivative); 
    }
}

void LineFitterWithGradientSearch::UpdateLineParameters(const std::vector<Point>& points)
{
    constexpr double learningRate = 0.1;
    line.P -= learningRate * gradP;
    line.v -= learningRate * gradv;
}

Line2d LineFitterWithGradientSearch::Fit(const std::vector<Point>& points)
{
    ASSERT(!points.empty(), "Not enough points for fitting");
    if (points.size() > 1) {
        line = InitializeLine(points);
        LOG(DESC(line.P));
        LOG(DESC(line.v));
        for (int i = 0; i < 4; ++i) {
            UpdateGradients(points);
            LOG(DESC(gradP));
            LOG(DESC(gradv));
            LOG(DESC(line.P));
            LOG(DESC(line.v));
            UpdateLineParameters(points);
        }
        line.v.Normalize();
    }
    else {
        line.P = points.front();
        line.v.ZeroOut();
    }

    return line;
}

Line2d fit_line(const std::vector<Point>& points)
{
    // return LineFitterWithGradientSearch().Fit(points);
    return LineFitterWithSimplexOptimization().Fit(points);
}

 Line2d fit_line_gradientsearch(const std::vector<Point>& points)
 {
     return LineFitterWithGradientSearch().Fit(points);
 }
}