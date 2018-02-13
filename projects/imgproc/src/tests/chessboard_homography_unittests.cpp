#include "imgproc/tests/chessboard_homography_unittests.h"
#include <vector>
#include "common/viz2d.h"
#include "common/logging.h"

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
	const auto upperLeftCorner = Point3(-1., 1., 10.);
	const auto upperRightCorner = Point3(1., 1., 10.);
	const double orthogonalAngle = 0.2;

	LOG("here");

	const auto chessBoard = ChessBoard3D(upperLeftCorner, upperRightCorner, orthogonalAngle);
	visualize_projected_chessboard(chessBoard);
}

void run_chessboard_homography_tests()
{
	run_test_1();
}

}   // namespace test
}   // namespace cbr