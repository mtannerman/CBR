#include "cbr_test/test.h"
#include "common/logging.h"
#include "common/config.h"

#include "math_utils/tests/simplex_optimizer_unittest.h"
#include "math_utils/tests/matrix_unittests.h"
#include "imgproc/tests/chessboard_homography_unittests.h"
#include "common/exceptions.h"

namespace cbr {
namespace test {
    void RunTests()
    {
        if (Config::GetInstance().GetBool("testFindChessBoard3D")) {
            run_chessboard_homography_tests();
        }
        if (Config::GetInstance().GetBool("testSimplexOptimizer")) {
            TEST_ASSERT(run_simplex_optimizer_unittest(), "simplex optimizer unittest failed.");
        }
        if (Config::GetInstance().GetBool("testMatrixLib")) {
            run_matrix_unittests();
        }
        
    }
}
}