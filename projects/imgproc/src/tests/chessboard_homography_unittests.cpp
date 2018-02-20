#include "imgproc/tests/chessboard_homography_unittests.h"
#include <vector>
#include "common/viz2d.h"
#include "common/logging.h"
#include "common/exceptions.h"

namespace cbr {
namespace test {

void visualize_projected_chessboard(const ChessBoard3D& cb)
{
	
	const std::string imageName = STR("run_chessboard_homography_tests");
    viz::Visualizer2D vizWindow(imageName);

	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			const auto middlePoint = 500. * cb.MiddlePointProjection(row, col);
			LOG(DESC(middlePoint));
			vizWindow.AddCircle(int(middlePoint.x), int(middlePoint.y), 5, viz::Color::red());
		}
	}

	vizWindow.Spin();
}

void run_test_1()
{
	const auto p1 = Point3(1., 1., 1.).Normalized();
	const auto rotation = ComputeRotationToXUnitVector(p1);
	const double phi = std::atan2(p1.y, p1.x);
	const double theta = std::atan2(p1.z, std::sqrt(p1.x*p1.x + p1.y*p1.y));

	const auto cosPhi =  std::cos(phi);
	const auto sinPhi =  std::cos(phi);

	// LOG(DESC(p1));
	const auto product = Matrix3(cosPhi, sinPhi,0.,-sinPhi,cosPhi,0.,0.,0.,1.)*p1;
	// LOG(DESC(product));
	// LOG(DESC(product.Norm()));
	// LOG(DESC(std::atan2(product.z, product.x)));

	// LOG(DESC(phi));
	// LOG(DESC(theta));
	// LOG(DESC(rotation.Determinant()));
	// LOG(DESC(rotation.Chopped()));
	// LOG(DESC(rotation*p1));
	TEST_ASSERT(Point3(1.0, 0., 0.) == rotation*p1, "");
}

void run_test_2()
{
	const auto p1 = Point3(1., 2., 1.).Normalized();
	TEST_ASSERT(Point3(1.0, 0., 0.) == ComputeRotationToXUnitVector(p1)*p1, "");
}

void run_test_3()
{
	const auto p1 = Point3(1., -2., 0.).Normalized();
	TEST_ASSERT(Point3(1.0, 0., 0.) == ComputeRotationToXUnitVector(p1)*p1, "");
}

void run_test_4()
{
	const auto p1 = Point3(1., 0., 0.).Normalized();
	TEST_ASSERT(Point3(1.0, 0., 0.) == ComputeRotationToXUnitVector(p1)*p1, "");
}

void run_test_5()
{
	const auto p1 = Point3(0., 0., 1.).Normalized();
	TEST_ASSERT(Point3(1.0, 0., 0.) == ComputeRotationToXUnitVector(p1)*p1, "");
}

void run_test_6()
{
	const auto p1 = Point3(0., 0., 10.);
	TEST_ASSERT(Point3(10.0, 0., 0.) == ComputeRotationToXUnitVector(p1)*p1, "");
}

void run_test_7()
{
	const auto p1 = Point3(0., 0., 0.);
	ASSERT_THROWN_EXCEPTION(ComputeRotationToXUnitVector(p1), BadFunctionInput, "");
}

void run_visualization()
{
	const auto upperLeftCorner = Point3(-1., 1., 10.);
	const auto upperRightCorner = Point3(1., 1., 10.);
	const double orthogonalAngle = 0.2;

	const auto chessBoard = ChessBoard3D(upperLeftCorner, upperRightCorner, orthogonalAngle);
	visualize_projected_chessboard(chessBoard);
}

void run_chessboard_homography_tests()
{
	run_test_1();
	run_test_2();
	run_test_3();
	run_test_4();
	run_test_5();
	run_test_6();
	run_test_7();
}

}   // namespace test
}   // namespace cbr