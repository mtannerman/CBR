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
        double shrinkage = 0.9;
    };

    Config config;

    SimplexOptimizer(const size_t nParams,
         const size_t nSimplexNodes);

    ~SimplexOptimizer();

    ParameterVector Optimize(
        std::function<double(ParameterVector)> errorFunction);
    std::vector<ParameterVector>& GetSimplex();
    const std::vector<ParameterVector>& GetSimplex() const;
    bool isSimplexInitialized = false;

private:
    struct Impl;
    impl_ptr<Impl> mImpl;
};

}