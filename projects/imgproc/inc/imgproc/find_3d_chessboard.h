#pragma once

#include <array>
#include "math_utils/point.h"

namespace cbr
{

struct VectorizedChessBoard3D
{
	enum class Params {
		ORTHOGONAL_ANGLE = 0,
		UPPER_LEFT_CORNER_X = 1,
		UPPER_LEFT_CORNER_Y = 2,
		UPPER_LEFT_CORNER_Z = 3,
		UPPER_RIGHT_CORNER_X = 4,
		UPPER_RIGHT_CORNER_Y = 5,
		UPPER_RIGHT_CORNER_Z = 6,
		COUNT = UPPER_RIGHT_CORNER_Z + 1
	};

	typedef std::array<double, int(Params::COUNT)> ParamArray;

	std::array<double, int(Params::COUNT)> params;

	
};

static Point3 GetUpperLeftCorner(
	const VectorizedChessBoard3D::ParamArray& pa);

static Point3 GetUpperRightCorner(
	const VectorizedChessBoard3D::ParamArray& pa);

static double GetOrthogonalAngle(
	const VectorizedChessBoard3D::ParamArray& pa);

static Point3 ComputeLowerLeftCorner(const Point3& upperLeftCorner,
	const Point3& upperRightCorner,
	const double orthogonalAngle);

struct ChessBoard3D
{
	ChessBoard3D(const VectorizedChessBoard3D& vectorizedChessBoard);
	Point3 upperLeftMiddlePoint;
	Point3 rowVec;
	Point3 colVec;

	Point3 MiddlePointPosition(const int rowIndex, const int colIndex) const;
	Point MiddlePointProjection(const int rowIndex, const int colIndex) const;

	std::array<std::array<Point, 8>, 8> ProjectToPlane() const;

	Point* FindClosestProjectionToMiddlePoint(const Point& middle, 
		std::array<std::array<Point, 8>, 8>& projectedBoard) const;

	double DistanceFromMiddlePoints(const std::vector<Point>& middlePoints) const;
};

std::array<std::array<Point, 8>, 8> find_3d_chessboard(const std::vector<Point>& middlePoints);

}   // namespace cbr