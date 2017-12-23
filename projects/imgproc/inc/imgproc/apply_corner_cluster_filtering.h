#pragma once
#include <vector>
#include <opencv2/core.hpp>
#include "math_utils/square.h"

namespace cbr 
{
    std::vector<Square> apply_cluster_filtering(
        const std::vector<Square>& squares);
}