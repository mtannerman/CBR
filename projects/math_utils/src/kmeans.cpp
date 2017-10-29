#include "math_utils/kmeans.h"


namespace cbr
{

struct KMeansComputer::Impl
{
    Impl(const size_t k) : k(k) {}

    std::vector<std::vector<double>> Fit(
        const std::vector<std::vector<double>>& points,
        KMeansComputer::Config& config);

    size_t k;

    std::vector<std::vector<double>> centers;
};

std::vector<std::vector<double>> KMeansComputer::Fit(
    const std::vector<std::vector<double>>& points)
{
    return std::vector<std::vector<double>>();
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