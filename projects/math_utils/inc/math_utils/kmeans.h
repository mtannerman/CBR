#pragma once

#include <vector>
#include "common/pimpl.h"

namespace cbr
{

// std::vector<std::vector<double>> find_kmeans_centers(
//     const std::vector<std::vector<double>>& points)
// {

class KMeansComputer
{
public:
    KMeansComputer(const size_t k);
    std::vector<std::vector<double>> Fit(
        const std::vector<std::vector<double>>& points);

    struct Config {
        size_t maxIterations = 10;
    };

    std::vector<std::vector<double>>& GetCenters(); 
    const std::vector<std::vector<double>>& GetCenters() const;

    bool& AreCentersInitialized();
    const bool AreCentersInitialized() const;

    Config config;

private:
    struct Impl;
    impl_ptr<Impl> mImpl;

};


}