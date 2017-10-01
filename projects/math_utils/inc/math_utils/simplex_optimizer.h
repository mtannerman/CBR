#pragma once

#include <functional>
#include <vector>
#include "common/pimpl.h"

namespace cbr
{

class SimplexOptimizer
{
public:
    typedef std::vector<double> ParameterVector;

    struct Config
    {
        size_t maxIterations = 10;
        double reflection = 1.0;
        double expansion = 2.0;
        double contraction = 0.5;
    };

    const Config config;

    SimplexOptimizer(const ParameterVector& params, const size_t nSimplexNodes);

    ParameterVector Optimize(std::function<double(ParameterVector)> errorFunction);
    std::vector<ParameterVector>& GetSimplex();
    const std::vector<ParameterVector>& GetSimplex() const;
    bool isSimplexInitialized = false;

private:
    struct Impl;
    impl_ptr<Impl> mImpl;
};

}