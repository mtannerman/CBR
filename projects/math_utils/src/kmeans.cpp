#include "math_utils/kmeans.h"
#include <algorithm>
#include "common/exceptions.h"
#include <cmath>
#include "math_utils/functional.h"

namespace cbr
{

double norm(const std::vector<double>& p1, const std::vector<double>& p2)
{
    THROW_IF(p1.size() != p2.size(), SizeMismatch, "");
    double sum = 0;
    for (size_t i = 0; i < p1.size(); ++i) {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }

    return std::sqrt(sum);
}

void Add(std::vector<double>& v1, const std::vector<double>& v2)
{
	THROW_IF(v1.size() != v2.size(), SizeMismatch, "");
    for (size_t i = 0; i < v1.size(); ++i) {
        v1[i] += v2[i];
    }
}

template <typename T>
void Divide(std::vector<double>& v, const T denominator) 
{
    const auto doubleDenominator = double(denominator);
    for (auto& elem : v) {
        elem /= doubleDenominator;
    }
}

struct KMeansComputer::Impl
{
    Impl(const size_t k) : k(k) {
        numberOfLabelOccurrence = std::vector<size_t>(k);
        centers = std::vector<std::vector<double>>(k);
    }

    std::vector<std::vector<double>> Fit(
        const std::vector<std::vector<double>>& points,
        KMeansComputer::Config& config);

    size_t k;

    void AssignLabels(const std::vector<std::vector<double>>& points);
    void AdjustCenters(const std::vector<std::vector<double>>& points);
    void DoIteration(const std::vector<std::vector<double>>& points);

    bool areCentersInitialized = false;
    std::vector<size_t> labels;
    std::vector<size_t> numberOfLabelOccurrence;
    std::vector<std::vector<double>> centers;
};

void KMeansComputer::Impl::AssignLabels(
    const std::vector<std::vector<double>>& points)
{
    f::FillCollection(numberOfLabelOccurrence, 0);
    for (size_t i = 0; i < points.size(); ++i) {
        const auto closest = std::min_element(centers.begin(), centers.end(),
        [i, &points](const std::vector<double>& c1, const std::vector<double>& c2)
        {
            return norm(c1, points[i]) < norm(c2, points[i]);
        });
        
        const size_t centerIdx = std::distance(centers.begin(), closest);
        labels[i] = centerIdx;
        numberOfLabelOccurrence[centerIdx]++;
    }
}

void KMeansComputer::Impl::AdjustCenters(
        const std::vector<std::vector<double>>& points)
{
    for (size_t iCenter = 0; iCenter < centers.size(); ++iCenter) {
        if (numberOfLabelOccurrence[iCenter] != 0) {
            f::FillCollection(centers[iCenter], 0.0); 
        }
    }

    for (size_t i = 0; i < points.size(); ++i) {
        Add(centers[labels[i]], points[i]);
    }

    for (size_t iCenter = 0; iCenter < centers.size(); ++iCenter) {
        if (numberOfLabelOccurrence[iCenter] != 0) {
            Divide(centers[iCenter], numberOfLabelOccurrence[iCenter]); 
        }
    }
}

void KMeansComputer::Impl::DoIteration(
    const std::vector<std::vector<double>>& points)
{
    AssignLabels(points);
    AdjustCenters(points);
}

std::vector<std::vector<double>> KMeansComputer::Impl::Fit(
    const std::vector<std::vector<double>>& points,
    KMeansComputer::Config& config)
{
    
    labels = std::vector<size_t>(points.size());
    ASSERT(areCentersInitialized, "");

    for (size_t iIter = 0; iIter < config.maxIterations; ++iIter) {
        DoIteration(points);
    }

    return centers;
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

bool& KMeansComputer::AreCentersInitialized()
{
    return mImpl->areCentersInitialized;
}

const bool KMeansComputer::AreCentersInitialized() const
{
    return mImpl->areCentersInitialized;
}

KMeansComputer::KMeansComputer(const size_t k)
    : mImpl(new Impl(k))
{
}

KMeansComputer::~KMeansComputer()
{}

}
