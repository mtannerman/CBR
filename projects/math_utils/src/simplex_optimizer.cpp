#include "math_utils/simplex_optimizer.h"
#include "common/exceptions.h"
#include <algorithm>

namespace cbr
{

std::string ParamVecStr(const std::vector<double>& param)
{
    std::stringstream ss;
    for (const auto& p : param) {
        ss << p << ", ";
    }
    return ss.str();
}

struct SimplexOptimizer::Impl
{
    Impl(const size_t nParams, const size_t nSimplexNodes) 
    : nParams(nParams)
    , nSimplexNodes(nSimplexNodes) 
    {
        simplex = std::vector<ParameterVector>(nSimplexNodes, ParameterVector(nParams, 0.0));
        centroid = ParameterVector(nParams, 0.0);
        reflection.value = ParameterVector(nParams, 0.0);
        expansion.value = ParameterVector(nParams, 0.0);
        outsideContraction.value = ParameterVector(nParams, 0.0);
        insideContraction.value = ParameterVector(nParams, 0.0);
        indexErrorPairs = std::vector<IndexErrorPair>(nSimplexNodes);
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
    void Optimize(std::function<double(ParameterVector)> errorFunction, 
    SimplexOptimizer::Config config);

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
    void ComputeOutsideContractionPoint(const double contractionMagnitude);
    void ComputeInsideContractionPoint(const double contractionMagnitude);
    void Shrink(const double shrinkageMagnitude);

    void PrintSimplex(const std::function<double(std::vector<double>)> errorFunction) const
    {
        for (const auto& node : simplex) {
            LOG(ParamVecStr(node) << ": " << errorFunction(node));
        }
    }
}; 

void SimplexOptimizer::Impl::ComputeReflectionPoint(const double reflectionMagnitude)
{
    for (size_t iParam = 0; iParam < nParams; ++iParam) {
        reflection.value[iParam] = centroid[iParam] + 
            reflectionMagnitude * (centroid[iParam] - simplex.back()[iParam]);
    }
}

void SimplexOptimizer::Impl::ComputeExpansionPoint(const double expansionMagnitude)
{
    for (size_t iParam = 0; iParam < nParams; ++iParam) {
        expansion.value[iParam] = centroid[iParam] + 
            expansionMagnitude * (reflection.value[iParam] - centroid[iParam]);
    }
}

void SimplexOptimizer::Impl::ComputeOutsideContractionPoint(const double contractionMagnitude)
{
    for (size_t iParam = 0; iParam < nParams; ++iParam) {
        outsideContraction.value[iParam] = centroid[iParam] + 
            contractionMagnitude * (reflection.value[iParam] - centroid[iParam]);
    }
}

void SimplexOptimizer::Impl::ComputeInsideContractionPoint(const double contractionMagnitude)
{
    for (size_t iParam = 0; iParam < nParams; ++iParam) {
        insideContraction.value[iParam] = centroid[iParam] - 
            contractionMagnitude * (reflection.value[iParam] - centroid[iParam]);
    }
}

void SimplexOptimizer::Impl::Shrink(const double shrinkageMagnitude)
{
    for (size_t iSimplex = 1; iSimplex < nSimplexNodes; ++iSimplex) {
        for (size_t iParam = 0; iParam < nParams; ++iParam) {
            simplex[iSimplex][iParam] = simplex[0][iParam] + 
                shrinkageMagnitude * (simplex[iSimplex][iParam] - simplex[0][iParam]);
        }
    }
}

void SimplexOptimizer::Impl::ComputeCentroid()
{
    for (size_t iParam = 0; iParam < nParams; ++iParam) {
        centroid[iParam] = 0.0;
        for (size_t iSimplex = 0; iSimplex + 1 < nSimplexNodes; ++iSimplex) {
            centroid[iParam] += simplex[iSimplex][iParam];
        }
        centroid[iParam] /= (nSimplexNodes - 1);
    }
}

void SimplexOptimizer::Impl::Optimize(
    std::function<double(ParameterVector)> errorFunction, SimplexOptimizer::Config config)
{
    
    for (size_t iSimplex = 0; iSimplex < nSimplexNodes; ++iSimplex) {
        indexErrorPairs[iSimplex].index = iSimplex;
        indexErrorPairs[iSimplex].error = errorFunction(simplex[iSimplex]);
    }

    for (size_t iIter = 0; iIter < config.maxIterations; ++iIter) {
        simplexCopy = simplex;
        for (size_t iSimplex = 0; iSimplex < nSimplexNodes; ++iSimplex) {
            indexErrorPairs[iSimplex].index = iSimplex;
        }
        std::sort(indexErrorPairs.begin(), indexErrorPairs.end());
        for (size_t iSimplex = 0; iSimplex < nSimplexNodes; ++iSimplex) {
            simplex[iSimplex] = simplexCopy[indexErrorPairs[iSimplex].index];
        }

        ComputeCentroid();
        ComputeReflectionPoint(config.reflection);
        reflection.error = errorFunction(reflection.value);

        bool needToShrink = false;

        if (indexErrorPairs[0].error <= reflection.error &&
            reflection.error < (indexErrorPairs.rbegin() + 1)->error) 
        {
            // LOG("reflection");
            simplex.back() = reflection.value;
            indexErrorPairs.back().error = reflection.error;
        }       
        else if (reflection.error < indexErrorPairs[0].error) {
            ComputeExpansionPoint(config.expansion);
            expansion.error = errorFunction(expansion.value);
            if (expansion.error < reflection.error) {
                // LOG("expansion");
                simplex.back() = expansion.value;
                indexErrorPairs.back().error = expansion.error;
            }
            else {
                // LOG("reflection in expansion");
                simplex.back() = reflection.value;
                indexErrorPairs.back().error = reflection.error;
            }
        }
        else if ((indexErrorPairs.rbegin() + 1)->error <= reflection.error && 
            reflection.error < indexErrorPairs.back().error) 
        {
            ComputeOutsideContractionPoint(config.contraction);
            outsideContraction.error = errorFunction(outsideContraction.value);
            if (outsideContraction.error <= reflection.error) {
                // LOG("outside contraction");
                simplex.back() = outsideContraction.value;
                indexErrorPairs.back().error = outsideContraction.error;
            }
            else {
                // LOG("shrinkage instead of outside contraction");
                needToShrink = true;
            }
        }
        else {
            ComputeInsideContractionPoint(config.contraction);
            insideContraction.error = errorFunction(insideContraction.value);
            if (insideContraction.error < indexErrorPairs.back().error) {
                // LOG("inside contraction");
                simplex.back() = insideContraction.value;
                indexErrorPairs.back().error = insideContraction.error;
            }
            else {
                // LOG("shrinkage instead of inside contraction");
                needToShrink = true;
            }
        }

        if (needToShrink) {
            Shrink(config.shrinkage);
            for (size_t iSimplex = 1; iSimplex < nSimplexNodes; ++iSimplex) {
                indexErrorPairs[iSimplex].error = errorFunction(simplex[iSimplex]);
            }
        }

        LOG("after iter " << iIter);
        PrintSimplex(errorFunction);

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

SimplexOptimizer::SimplexOptimizer(
    const size_t nParams,
    const size_t nSimplexNodes)
    : mImpl(new Impl(nParams, nSimplexNodes))
{
}

SimplexOptimizer::ParameterVector SimplexOptimizer::Optimize(
    std::function<double(ParameterVector)> errorFunction)
{
    ASSERT(isSimplexInitialized, "simplex must be initialized before use");
    mImpl->Optimize(errorFunction, config);
    return mImpl->simplex.front();
}

SimplexOptimizer::~SimplexOptimizer()
{

}

}