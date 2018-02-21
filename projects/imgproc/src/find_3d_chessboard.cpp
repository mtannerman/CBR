#include "imgproc/find_3d_chessboard.h"

#include "common/exceptions.h"
#include <cmath>
#include <limits>

namespace cbr
{

VectorizedChessBoard3D::VectorizedChessBoard3D(const ChessBoard3D& cb)
{
	const auto upperLeftCorner = cb.upperLeftMiddlePoint - (cb.rowVec + cb.colVec);
	// params[int(ORTHOGONAL_ANGLE)]     = 
	// params[int(UPPER_LEFT_CORNER_X )] = 
	// params[int(UPPER_LEFT_CORNER_Y )] = 
	// params[int(UPPER_LEFT_CORNER_Z )] = 
	// params[int(UPPER_RIGHT_CORNER_X)] = 
	// params[int(UPPER_RIGHT_CORNER_Y)] = 
	// params[int(UPPER_RIGHT_CORNER_Z)] = 
}

Matrix3 ComputeRotationToXUnitVector(const Point3& p)
{
	THROW_IF(p.IsNull(), BadFunctionInput, "");
	const auto v = p.Normalized();

	const double phi = std::atan2(v.y, v.x);
	const double theta = std::atan2(v.z, std::sqrt(v.x*v.x + v.y*v.y));

	const double cosTheta = std::cos(theta);
	const double sinTheta = std::sin(theta);
	const double cosPhi = std::cos(phi);
	const double sinPhi = std::sin(phi);

	return Matrix3(
		cosTheta*cosPhi, cosTheta*sinPhi, sinTheta,
		-sinPhi, cosPhi, 0,
		-cosPhi*sinTheta, -sinPhi*sinTheta, cosTheta);
}

Point3 GetUpperLeftCorner(const VectorizedChessBoard3D::ParamArray& pa)
{
	return Point3(
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_X)],
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_Y)],
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_Z)]);
}

Point3 GetUpperRightCorner(const VectorizedChessBoard3D::ParamArray& pa)
{
	return Point3(
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_X)],
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_Y)],
		pa[size_t(VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_Z)]);
}

double GetOrthogonalAngle(const VectorizedChessBoard3D::ParamArray & pa)
{
	return pa[size_t(VectorizedChessBoard3D::Params::ORTHOGONAL_ANGLE)];
}

Point3 ComputeLowerLeftCorner(const Point3& upperLeftCorner, const Point3& upperRightCorner, const double orthogonalAngle)
{
	const auto rotation = ComputeRotationToXUnitVector(upperRightCorner - upperLeftCorner);

	const auto columnUnitVector =
		rotation.Inverse() * Point3(0., std::cos(orthogonalAngle), std::sin(orthogonalAngle));

	return upperLeftCorner + columnUnitVector * upperLeftCorner.Distance(upperRightCorner);
}

void ChessBoard3D::Initialize(const Point3& upperLeftCorner,
	const Point3& upperRightCorner, const double orthogonalAngle)
{
	rowVec = upperRightCorner - upperLeftCorner;
	const auto lowerLeftCorner = ComputeLowerLeftCorner(upperLeftCorner, upperRightCorner, orthogonalAngle);
	colVec = lowerLeftCorner - upperLeftCorner;

	upperLeftMiddlePoint = upperLeftCorner + (rowVec + colVec) / 8.;

	LOG("initialized chessboard: ");
	LOG(DESC(rowVec));
	LOG(DESC(colVec));
	LOG(DESC(colVec.Norm()));
	LOG(DESC(upperLeftMiddlePoint));
}

ChessBoard3D::ChessBoard3D(const Point3& upperLeftCorner,
	const Point3& upperRightCorner, const double orthogonalAngle)
{
	Initialize(upperLeftCorner, upperRightCorner, orthogonalAngle);
}

ChessBoard3D::ChessBoard3D(const VectorizedChessBoard3D& vectorizedChessBoard) {

	const auto upperLeftCorner = GetUpperLeftCorner(vectorizedChessBoard.params);
	const auto upperRightCorner = GetUpperRightCorner(vectorizedChessBoard.params);
	const auto orthogonalAngle = GetOrthogonalAngle(vectorizedChessBoard.params);
	Initialize(upperLeftCorner, upperRightCorner, orthogonalAngle);
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
	double fullDistance = 0.0;
	auto projectedBoard = ProjectToPlane();
	for (const auto& middle : middlePoints) {
		Point* closestPoint = FindClosestProjectionToMiddlePoint(middle, projectedBoard);
		fullDistance += closestPoint->Distance(middle);
		closestPoint->x = std::numeric_limits<double>::max();
		closestPoint->y = std::numeric_limits<double>::max();
	}
	return fullDistance;
}

VectorizedChessBoard3D ChessBoardFinder::InitialChessBoard()
{
	VectorizedChessBoard3D cb;
	cb.params[(int)VectorizedChessBoard3D::Params::ORTHOGONAL_ANGLE] = 0.;
	cb.params[(int)VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_X] = 2.;
	cb.params[(int)VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_Y] = 2.;
	cb.params[(int)VectorizedChessBoard3D::Params::UPPER_LEFT_CORNER_Z] = 5.;
	cb.params[(int)VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_X] = 2.;
	cb.params[(int)VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_Y] = -2.;
	cb.params[(int)VectorizedChessBoard3D::Params::UPPER_RIGHT_CORNER_Z] = 5.;

	return cb;
}

double ChessBoardFinder::ComputeShrinkageParameter(
	const std::vector<Point>& middlePoints) const
{

}

ChessBoard3D ChessBoardFinder::Find(const std::vector<Point>& middlePoints)
{
	ChessBoard3D cb;

	return ChessBoard3D();
}

}   // namespace cbr
