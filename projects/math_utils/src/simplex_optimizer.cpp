#include "math_utils/simplex_optimizer.h"
#include "common/exceptions.h"
#include <algorithm>

namespace cbr
{

struct SimplexOptimizer::Impl
{
    Impl(const size_t nParams, const size_t nSimplexNodes) : nParams(nParams), nSimplexNodes(nSimplexNodes) 
    {
        simplex = std::vector<ParameterVector>(nSimplexNodes, ParameterVector(nParams, 0.0));
        reflection.value = ParameterVector(nParams, 0.0);
        expansion.value = ParameterVector(nParams, 0.0);
        outsideContraction.value = ParameterVector(nParams, 0.0);
        insideContraction.value = ParameterVector(nParams, 0.0);
    }

    struct IndexErrorPair
    {
        bool operator<(const IndexErrorPair& other) 
        {
            return error < other.error;
        }
        size_t index;
        double error;
    };

    void ComputeCentroid();
    void Optimize(std::function<double(ParameterVector)> errorFunction, SimplexOptimizer::Config config);

    size_t nParams;
    size_t nSimplexNodes;
    std::vector<double> centroid;

    std::vector<IndexErrorPair> indexErrorPairs;
    std::vector<ParameterVector> simplex;
    std::vector<ParameterVector> simplexCopy;

    struct ParamVectorErrorPair
    {
        ParameterVector value;
        double error;
    };

    ParamVectorErrorPair reflection;
    ParamVectorErrorPair expansion;
    ParamVectorErrorPair outsideContraction;
    ParamVectorErrorPair insideContraction;

    void ComputeReflectionPoint(const double reflectionMagnitude);
    void ComputeExpansionPoint(const double expansionMagnitude);
    void ComputeOutsideContractionPoint();
    void ComputeInsideContractionPoint();
}; 

void SimplexOptimizer::Impl::ComputeReflectionPoint(const double reflectionMagnitude)
{
    for (size_t iParam = 0; iParam < nParams; ++iParam) {
        reflection.value[iParam] = centroid[iParam] + reflectionMagnitude * (centroid[iParam] - simplex.back()[iParam]);
    }
}

void SimplexOptimizer::Impl::ComputeExpansionPoint(const double expansionMagnitude)
{
    for (size_t iParam = 0; iParam < nParams; ++iParam) {
        expansion.value[iParam] = centroid[iParam] + expansionMagnitude * (reflection.value[iParam] - centroid[iParam]);
    }
}

void SimplexOptimizer::Impl::ComputeOutsideContractionPoint()
{

}

void SimplexOptimizer::Impl::ComputeInsideContractionPoint()
{

}

void SimplexOptimizer::Impl::ComputeCentroid()
{
    for (size_t iParam = 0; iParam < nParams; ++iParam) {
        centroid[iParam] = 0.0;
        for (size_t iSimplex = 0; iSimplex < nSimplexNodes; ++iSimplex) {
            centroid[iParam] += simplex[iSimplex][iParam];
        }
        centroid[iParam] /= nSimplexNodes;
    }
}

void SimplexOptimizer::Impl::Optimize(std::function<double(ParameterVector)> errorFunction, SimplexOptimizer::Config config)
{
    simplexCopy = simplex;
    for (size_t iSimplex = 0; iSimplex < nSimplexNodes; ++iSimplex) {
        indexErrorPairs[iSimplex].index = iSimplex;
        indexErrorPairs[iSimplex].error = errorFunction(simplex[iSimplex]);
    }
    std::sort(indexErrorPairs.begin(), indexErrorPairs.end());
    for (size_t iSimplex = 0; iSimplex < nSimplexNodes; ++iSimplex) {
        simplex[iSimplex] = simplexCopy[indexErrorPairs[iSimplex].index];
    }

    for (size_t iIter = 0; iIter < config.maxIterations; ++iIter) {
        


    }

}

std::vector<SimplexOptimizer::ParameterVector>& SimplexOptimizer::GetSimplex()
{
    return mImpl->simplex;
}

const std::vector<SimplexOptimizer::ParameterVector>& SimplexOptimizer::GetSimplex() const
{
    return mImpl->simplex;
}

SimplexOptimizer::SimplexOptimizer(const SimplexOptimizer::ParameterVector& params, const size_t nSimplexNodes)
    : mImpl(new Impl(params.size(), nSimplexNodes))
{

}

SimplexOptimizer::ParameterVector SimplexOptimizer::Optimize(std::function<double(ParameterVector)> errorFunction)
{
    ASSERT(isSimplexInitialized, "simplex must be initialized before use");
    return SimplexOptimizer::ParameterVector();
}

}