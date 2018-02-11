#include "math_utils/tests/matrix_unittests.h"
#include "common/exceptions.h"
#include <cmath>

namespace cbr {
namespace test {
void matrix_test_1()
{
    const auto p = Point(1., 2.);
    const auto m = Matrix::Eye();
    const auto product = m * p;
    TEST_ASSERT(product == p, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix_test_2()
{
    const auto p = Point(0., 1.);
    const auto m = Matrix::Eye();
    const auto product = m * p;
    TEST_ASSERT(product == p, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix_test_3()
{
    const auto m = Matrix::Eye();
    TEST_ASSERT(std::abs(m.Determinant() - 1.) < 1e-9, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix_test_4()
{
    const auto m = Matrix(1.,2.,3.,4.);
    TEST_ASSERT(std::abs(m.Determinant() - (-2.0)) < 1e-9, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix_test_5()
{
    TEST_ASSERT((Matrix(1.,2.,3.,4.) * Point(7., 9.)).Distance(Point(25., 57.)) < 1e-9, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix_test_6()
{
    TEST_ASSERT(Matrix(1.,2.,0.,1.).Inverse() == Matrix(1., -2., 0., 1.), __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix3_test_1()
{
    const auto p = Point3(1., 2., 3.);
    const auto m = Matrix3::Eye();
    const auto product = m * p;
    TEST_ASSERT(product == p, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix3_test_2()
{
    const auto p = Point3(0., 1., 2.);
    const auto m = Matrix3::Eye();
    const auto product = m * p;
    TEST_ASSERT(product == p, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix3_test_3()
{
    const auto m = Matrix3::Eye();
    TEST_ASSERT(std::abs(m.Determinant() - 1.) < 1e-9, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix3_test_4()
{
    const auto m = Matrix3(1.,2.,3.,4.,5.,6.,7.,8.,9.);
    TEST_ASSERT(std::abs(m.Determinant() - 0.) < 1e-9, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix3_test_5()
{
    const auto m = Matrix3(1.,2.,1.,1.,5.,6.,1.,2.,9.);
    TEST_ASSERT(std::abs(m.Determinant() - 24.) < 1e-9, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix3_test_6()
{
    TEST_ASSERT((Matrix3(1.,2.,3.,4.,5.,6.,7.,8.,9.) * Point3(7., 9., 1.)).Distance(Point3(28., 79., 130.)) < 1e-9, __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void matrix3_test_7()
{
    TEST_ASSERT((Matrix3(1.,3.,2.,0.,9.,5.,1.,1.,1.).Inverse() == Matrix3(4., -1., -3., 5., -1., -5., -9., 2., 9.)), __FUNCTION__);
    LOG("\t" << __FUNCTION__ << " PASSED");
}

void run_matrix_unittests()
{
    LOG("RUNNING MATRIX LIB TEST");
    matrix_test_1();
    matrix_test_2();
    matrix_test_3();
    matrix_test_4();
    matrix_test_5();
    matrix_test_6();

    matrix3_test_1();
    matrix3_test_2();
    matrix3_test_3();
    matrix3_test_4();
    matrix3_test_5();
    matrix3_test_6();
    matrix3_test_7();
    LOG("MATRIX LIB TEST: PASSED");
}
}   // namespace test
}   // namespace test