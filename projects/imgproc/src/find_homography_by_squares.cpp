#include "imgproc/find_homography_by_squares.h"

#include "math_utils/matrix.h"
#include "common/exceptions.h"

namespace cbr
{

struct ChessBoard3D
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

    struct Helper {
        static Point3 GetUpperLeftCorner(
            const std::array<double, int(Params::COUNT)>& parameters)
        {
            return Point3(
                parameters[size_t(Params::UPPER_LEFT_CORNER_X)],
                parameters[size_t(Params::UPPER_LEFT_CORNER_Y)], 
                parameters[size_t(Params::UPPER_LEFT_CORNER_Z)]);
        }

        static Point3 GetUpperRightCorner(
            const std::array<double, int(Params::COUNT)>& parameters)
        {
            return Point3(
                parameters[size_t(Params::UPPER_RIGHT_CORNER_X)],
                parameters[size_t(Params::UPPER_RIGHT_CORNER_Y)], 
                parameters[size_t(Params::UPPER_RIGHT_CORNER_Z)]);
        }

        static Point3 ComputeLowerLeftCorner(const Point3& upperLeftCorner,
            const Point3& upperRightCorner,
            const double orthogonalAngle)
        {
            const auto v = (upperRightCorner - upperLeftCorner).Normalized();

            const double phi = std::atan2(v.y, v.x);            
            const double theta = std::atan2(v.z, std::sqrt(v.x*v.x + v.y*v.y));

            const double cosTheta = std::cos(theta);
            const double sinTheta = std::sin(theta);
            const double cosPhi = std::cos(phi);
            const double sinPhi = std::sin(phi);

            const auto rotation = Matrix3(
                cosTheta*cosPhi, cosTheta*sinPhi,-sinPhi,
                -sinPhi,cosPhi,0,
                cosPhi*sinTheta,sinPhi*sinTheta,cosTheta);

            const auto columnUnitVector = 
                rotation.Inverse() * Point3(0., std::cos(orthogonalAngle), std::sin(orthogonalAngle));

            return upperLeftCorner + columnUnitVector * upperLeftCorner.Distance(upperRightCorner);          
        }

        Helper(const std::array<double, int(Params::COUNT)>& parameters) {

            const auto upperLeftCorner = GetUpperLeftCorner(parameters);
            const auto upperRightCorner = GetUpperRightCorner(parameters);
            rowVec = upperRightCorner - upperLeftCorner;

            const auto orthogonalAngle = parameters[size_t(Params::ORTHOGONAL_ANGLE)];

            const auto lowerLeftCorner = ComputeLowerLeftCorner(upperLeftCorner, upperRightCorner, orthogonalAngle);
            colVec = lowerLeftCorner - upperLeftCorner;

            upperLeftMiddlePoint = upperLeftCorner + (rowVec + colVec) / 8.;
        }

        Point3 upperLeftMiddlePoint;
        Point3 rowVec;
        Point3 colVec;

        Point3 MiddlePointPosition(const int rowIndex, const int colIndex) const {
            return upperLeftMiddlePoint + double(rowIndex) * rowVec + double(colIndex) * colVec;
        }

        struct PointUsedBoolPair
        {
            PointUsedBoolPair() = default;
            PointUsedBoolPair(const Point& p, const bool usedAlready)
                : p(p), usedAlready(usedAlready) 
            {}

            Point p;
            bool usedAlready = false;

        };

        Point MiddlePointProjection(const int rowIndex, const int colIndex) const {
            const auto p = MiddlePointPosition(rowIndex, colIndex);

            return Point(p.x, p.y) / p.z;
        }
    };

    std::array<double, int(Params::COUNT)> params;

    std::array<std::array<Helper::PointUsedBoolPair, 8>, 8> ProjectToPlane() const
    {
        const Helper helper(params);
        std::array<std::array<Helper::PointUsedBoolPair, 8>, 8> ret;
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                ret[row][col].p = helper.MiddlePointProjection(row, col);
            }
        }

        return ret;
    }

	double FindClosestProjectionToMiddlePointDistance(const Point& middle,
		const std::vector<Helper::PointUsedBoolPair>& projectedBoard) const
	{
		auto closestProjection = projectedBoard.end();
		for (auto it = projectedBoard.begin(); it != projectedBoard.end(); ++it) {
			if (!it->usedAlready) {
				closestProjection = it;
				break;
			}
		}

		ASSERT(closestProjection != projectedBoard.end(), "");

		for (auto it = std::next(closestProjection); it != projectedBoard.end(); ++it) {

		}

		//double ret;
		//for (const auto&)
	}

    double DistanceFromMiddlePoints(const std::vector<Point>& middlePoints) const
    {
        const auto projectedBoard = ProjectToPlane();
        for (const auto& middle : middlePoints) {
            //// need to find closest projection
            //static_assert(false, "continue here");
        }
        return 0.0;
    }

};

std::array<std::array<Point, 8>, 8> find_homography_by_squares(const std::vector<Point>& middlePoints)
{
    return std::array<std::array<Point, 8>, 8>();
}

}   // namespace cbr
