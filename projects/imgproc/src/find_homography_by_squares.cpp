#include "imgproc/find_homography_by_squares.h"
#include <array>

#include <cmath>

namespace cbr
{

struct ChessBoard3D
{
    enum class Params { 
        PLANE_NORMAL_HORIZONTAL_ANGLE = 0,
        PLANE_NORMAL_VERTICAL_ANGLE = 1,
        UPPER_LEFT_CORNER_X = 2,
        UPPER_LEFT_CORNER_Y = 3,
        UPPER_LEFT_CORNER_Z = 4,
        UPPER_RIGHT_CORNER_X = 5,
        UPPER_RIGHT_CORNER_Y = 6,
        UPPER_RIGHT_CORNER_Z = 7,
        COUNT = UPPER_RIGHT_CORNER_Z + 1
    };

    struct Helper {
        Helper(const std::array<double, int(Params::COUNT)>& parameters) {

            const auto upperLeftPoint = Point3(
                parameters[size_t(Params::UPPER_LEFT_CORNER_X)],
                parameters[size_t(Params::UPPER_LEFT_CORNER_Y)], 
                parameters[size_t(Params::UPPER_LEFT_CORNER_Z)]);

            const auto upperRightPoint = Point3(
                parameters[size_t(Params::UPPER_RIGHT_CORNER_X)],
                parameters[size_t(Params::UPPER_RIGHT_CORNER_Y)], 
                parameters[size_t(Params::UPPER_RIGHT_CORNER_Z)]);

            rowVec = upperRightPoint - upperLeftPoint;

            const auto phi = parameters[size_t(Params::PLANE_NORMAL_HORIZONTAL_ANGLE)];
            const auto theta = parameters[size_t(Params::PLANE_NORMAL_VERTICAL_ANGLE)];

            const auto planeNormal = Point3(
                std::cos(theta) * std::cos(phi),
                std::cos(theta) * std::sin(phi),
                std::sin(theta)).Normalized();

            colVec = rowVec.Cross(planeNormal);

            upperLeftMiddlePoint = upperLeftPoint + (rowVec + colVec) / 8.;
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
    }

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