#include "imgproc/find_3d_chessboard.h"

#include "math_utils/matrix.h"
#include "common/exceptions.h"

namespace cbr
{



std::array<std::array<Point, 8>, 8> find_3d_chessboard(const std::vector<Point>& middlePoints)
{
    return std::array<std::array<Point, 8>, 8>();
}

inline Point3 GetUpperLeftCorner(const VectorizedChessBoard3D::ParamArray& pa)
{
	return Point3(
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_X)],
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_Y)],
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_Z)]);
}

inline Point3 GetUpperRightCorner(const VectorizedChessBoard3D::ParamArray& pa)
{
	return Point3(
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_X)],
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_Y)],
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_Z)]);
}

inline double GetOrthogonalAngle(const VectorizedChessBoard3D::ParamArray & pa)
{
	return pa[size_t(VectorizedChessBoard3D::Params::ORTHOGONAL_ANGLE)];
}

inline Point3 ComputeLowerLeftCorner(const Point3 & upperLeftCorner, const Point3 & upperRightCorner, const double orthogonalAngle)
{
	const auto v = (upperRightCorner - upperLeftCorner).Normalized();

	const double phi = std::atan2(v.y, v.x);
	const double theta = std::atan2(v.z, std::sqrt(v.x*v.x + v.y*v.y));

	const double cosTheta = std::cos(theta);
	const double sinTheta = std::sin(theta);
	const double cosPhi = std::cos(phi);
	const double sinPhi = std::sin(phi);

	const auto rotation = Matrix3(
		cosTheta*cosPhi, cosTheta*sinPhi, -sinPhi,
		-sinPhi, cosPhi, 0,
		cosPhi*sinTheta, sinPhi*sinTheta, cosTheta);

	const auto columnUnitVector =
		rotation.Inverse() * Point3(0., std::cos(orthogonalAngle), std::sin(orthogonalAngle));

	return upperLeftCorner + columnUnitVector * upperLeftCorner.Distance(upperRightCorner);
}

ChessBoard3D::ChessBoard3D(const VectorizedChessBoard3D& vectorizedChessBoard) {

	const auto upperLeftCorner = GetUpperLeftCorner(vectorizedChessBoard.params);
	const auto upperRightCorner = GetUpperRightCorner(vectorizedChessBoard.params);
	rowVec = upperRightCorner - upperLeftCorner;

	const auto orthogonalAngle = GetOrthogonalAngle(vectorizedChessBoard.params);

	const auto lowerLeftCorner = ComputeLowerLeftCorner(upperLeftCorner, upperRightCorner, orthogonalAngle);
	colVec = lowerLeftCorner - upperLeftCorner;

	upperLeftMiddlePoint = upperLeftCorner + (rowVec + colVec) / 8.;
}

Point3 ChessBoard3D::MiddlePointPosition(const int rowIndex, const int colIndex) const {
	return upperLeftMiddlePoint + double(rowIndex) * rowVec + double(colIndex) * colVec;
}

Point ChessBoard3D::MiddlePointProjection(const int rowIndex, const int colIndex) const {
	const auto p = MiddlePointPosition(rowIndex, colIndex);

	return Point(p.x, p.y) / p.z;
}

std::array<std::array<Point, 8>, 8> ChessBoard3D::ProjectToPlane() const
{
	std::array<std::array<Point, 8>, 8> ret;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			ret[row][col] = MiddlePointProjection(row, col);
		}
	}

	return ret;
}

Point* ChessBoard3D::FindClosestProjectionToMiddlePoint(
	const Point& middle, std::array<std::array<Point, 8>, 8>& projectedBoard) const
{
	Point* closestPoint = &projectedBoard[0][0];
	double closestDifference = projectedBoard[0][0].Distance(middle);
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			const double currentDistance = projectedBoard[row][col].Distance(middle);
			if (closestDifference < currentDistance) {
				closestPoint = &projectedBoard[row][col];
			}
		}
	}

	return closestPoint;
}

double ChessBoard3D::DistanceFromMiddlePoints(const std::vector<Point>& middlePoints) const
{
	auto projectedBoard = ProjectToPlane();
	for (const auto& middle : middlePoints) {
		Point& closestPoint = *FindClosestProjectionToMiddlePoint(middle, projectedBoard);
		//// need to find closest projection
		//static_assert(false, "continue here");
	}
	return 0.0;
}

}   // namespace cbr
