#include "cbr_test/test.h"
#include "common/logging.h"
#include "common/config.h"

#include "math_utils/tests/simplex_optimizer_unittest.h"
#include "common/exceptions.h"

namespace cbr {
namespace test {
    void RunTests()
    {
        if (Config::GetInstance().GetBool("testSimplexOptimizer")) {
            TEST_ASSERT(run_simplex_optimizer_unittest(), "simplex optimizer unittest failed.");
        }
        
    }
}
}