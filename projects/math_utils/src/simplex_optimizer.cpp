#include "math_utils/simplex_optimizer.h"
#include "common/exceptions.h"
#include <algorithm>

namespace cbr
{

struct SimplexOptimizer::Impl
{
    Impl(const size_t nParams, const size_t nSimplexNodes) : nParams(nParams), nSimplexNodes(nSimplexNodes) {}
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
    void SortSimplex();

    size_t nParams;
    size_t nSimplexNodes;
    std::vector<double> centroid;

    std::vector<IndexErrorPair> indexErrorPairs;
    std::vector<Parameters> simplex;
    std::vector<Parameters> simplexCopy;
}; 

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

void SimplexOptimizer::Impl::SortSimplex()
{
    std::sort(indexErrorPairs.begin(), indexErrorPairs.end());
}

std::vector<SimplexOptimizer::Parameters>& SimplexOptimizer::GetSimplex()
{
    return mImpl->simplex;
}

const std::vector<SimplexOptimizer::Parameters>& SimplexOptimizer::GetSimplex() const
{
    return mImpl->simplex;
}

SimplexOptimizer::SimplexOptimizer(const SimplexOptimizer::Parameters& params, const size_t nSimplexNodes)
    : mImpl(new Impl(params.size(), nSimplexNodes))
{

}

SimplexOptimizer::Parameters SimplexOptimizer::Optimize(std::function<double(Parameters)> errorFunction)
{
    ASSERT(isSimplexInitialized, "simplex must be initialized before use");
    return SimplexOptimizer::Parameters();
}

}