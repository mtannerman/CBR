#include "math_utils/kmeans.h"
#include <algorithm>
#include "common/exceptions.h"
#include <cmath>

namespace cbr
{

double norm(const std::vector<double>& p1, const std::vector<double>& p2)
{
    ASSERT(p1.size() == p2.size(), "");
    double sum = 0;
    for (size_t i = 0; i < p1.size(); ++i) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }

    return std::sqrt(sum);
}

struct KMeansComputer::Impl
{
    Impl(const size_t k) : k(k) {}

    std::vector<std::vector<double>> Fit(
        const std::vector<std::vector<double>>& points,
        KMeansComputer::Config& config);

    size_t k;

    void AssignLabels(const std::vector<std::vector<double>>& points);

    std::vector<size_t> labels;
    std::vector<std::vector<double>> centers;
};

void KMeansComputer::Impl::AssignLabels(const std::vector<std::vector<double>>& points)
{
    for (size_t i = 0; i < points.size(); ++i) {
        const auto closest = std::min_element(centers.begin(), centers.end(),
            [i](const std::vector<double>& p1, const std::vector<double>& p2)
        {});
        // labels[i] = std::distance(std::min_element(centers.begin(), centers.end(), 
        //     [i](const std::vector<double>& p1, const std::vector<double>& p2)
        //     {

        //     };), centers.begin());
    }
}


std::vector<std::vector<double>> KMeansComputer::Impl::Fit(
    const std::vector<std::vector<double>>& points,
    KMeansComputer::Config& config)
{
    return std::vector<std::vector<double>>();
}

std::vector<std::vector<double>> KMeansComputer::Fit(
    const std::vector<std::vector<double>>& points)
{
    return mImpl->Fit(points, config);
}

std::vector<std::vector<double>>& KMeansComputer::GetCenters()
{
    return mImpl->centers;
}
const std::vector<std::vector<double>>& KMeansComputer::GetCenters() const
{
    return mImpl->centers;
}


KMeansComputer::KMeansComputer(const size_t k)
    : mImpl(new Impl(k))
{
}

}