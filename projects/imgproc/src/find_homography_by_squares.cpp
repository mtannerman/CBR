#include "imgproc/find_homography_by_squares.h"
#include <array>

#include <cmath>

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
            
        }

        Helper(const std::array<double, int(Params::COUNT)>& parameters) {

            const auto upperLeftCorner = GetUpperLeftCorner(parameters);
            const auto upperRightCorner = GetUpperRightCorner(parameters);
            rowVec = upperRightCorner - upperLeftCorner;

            const auto orthogonalAngle = parameters[size_t(Params::ORTHOGONAL_ANGLE)];

            // const auto planeNormal = Point3(
            //     std::cos(theta) * std::cos(phi),
            //     std::cos(theta) * std::sin(phi),
            //     std::sin(theta)).Normalized();

            // colVec = rowVec.Cross(planeNormal);

            // upperLeftMiddlePoint = upperLeftPoint + (rowVec + colVec) / 8.;
        }

        Point3 upperLeftMiddlePoint;
        Point3 rowVec;
        Point3 colVec;

        Point3 MiddlePointPosition(const int rowIndex, const int colIndex) const {
            return upperLeftMiddlePoint + double(rowIndex) * rowVec + double(colIndex) * colVec;
        }

        Point MiddlePointProjection(const int rowIndex, const int colIndex) const {
            const auto p = MiddlePointPosition(rowIndex, colIndex);

            return Point(p.x, p.y) / p.z;
        }
    };

    std::array<double, int(Params::COUNT)> params;

    std::array<std::array<Point, 8>, 8> ProjectToPlane() const
    {
        const Helper helper(params);
        std::array<std::array<Point, 8>, 8> ret;
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                ret[row][col] = helper.MiddlePointProjection(row, col);
            }
        }

        return ret;
    }

    double DistanceFromMiddlePoints(const std::vector<Point>& middlePoints)
    {
        // const auto 
    }

};

std::array<std::array<Point, 8>, 8> find_homography_by_squares(const std::vector<Point>& middlePoints)
{
    return std::array<std::array<Point, 8>, 8>();
}

}   // namespace cbr
