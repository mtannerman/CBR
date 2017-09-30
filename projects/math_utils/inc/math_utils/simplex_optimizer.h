#pragma once

#include <functional>
#include <vector>

namespace cbr
{

class SimplexOptimizer
{
    typedef std::vector<double> Parameters;
public:
    struct Config
    {
        Config(const size_t simplexSize) : simplexSize(simplexSize) {}
        size_t maxIterations = 10;
        double reflection = 1.0;
        double expansion = 2.0;
        double contraction = 0.5;
        bool initialize = false;
        size_t simplexSize;
    };

    const Config config;

    SimplexOptimizer(const Parameters& params)
        : config(params.size()) {}

    Parameters Optimize();
private:
};

}