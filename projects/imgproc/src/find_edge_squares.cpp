#include "imgproc/find_edge_squares.h"
#include "math_utils/functional.h"

namespace cbr
{

cv::Point2d find_extremal_corner(
    const Edge edge,
    const std::vector<std::vector<cv::Point2d>>& rotatedSquares)
{  
    // switch (edge) {
    //     case Edge::LEFT:
    //         for (const auto& s : rotatedSquares) {

    //         } 
    //     case Edge::TOP:
    //     case Edge::RIGHT:
    //     case Edge::BOTTOM:
    // }

    auto ret = rotatedSquares[0][0];
    if (edge == Edge::LEFT) {
        auto minimalXCoord = ret.x;
        for (const auto& s : rotatedSquares) {
            for (const auto& c : s) {
                if (c.x < minimalXCoord) {
                    ret = c; 
                    minimalXCoord = c.x;
                }
            }
        }
    }
    else if (edge == Edge::TOP) {
        auto maximalYCoord = ret.y;
        for (const auto& s : rotatedSquares) {
            for (const auto& c : s) {
                if (c.y > maximalYCoord) {
                    ret = c; 
                    maximalYCoord = c.y;
                }
            }
        }
    }
    else if (edge == Edge::RIGHT) {
        auto maximalXCoord = ret.x;
        for (const auto& s : rotatedSquares) {
            for (const auto& c : s) {
                if (c.x > maximalXCoord) {
                    ret = c; 
                    maximalXCoord = c.x;
                }
            }
        }
    }
    else { // BOTTOM
        auto minimalYCoord = ret.y;
        for (const auto& s : rotatedSquares) {
            for (const auto& c : s) {
                if (c.y < minimalYCoord) {
                    ret = c; 
                    minimalYCoord = c.y;
                }
            }
        }
    }

    return ret;
}

std::map<Edge, std::vector<std::vector<cv::Point>>> find_edge_squares(
    const std::array<cv::Point2d, 2>& dominantEdgeDirections,
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::map<Edge, std::vector<std::vector<cv::Point>>> ret;
    const double edgeLengthAverage = fsum(squares, [](const std::vector<cv::Point>& s) {
            return cv::norm(s[0] - s[1]) + cv::norm(s[1] - s[2]) + cv::norm(s[2] - s[3]) + cv::norm(s[3] - s[0]);
        }) / 4. / double(squares.size());

    const auto& f1 = dominantEdgeDirections[0];
	const auto& f2 = dominantEdgeDirections[1];

	auto rotate = [&f1, &f2](const cv::Point& p){ return cv::Point2d(f1.x * p.x + f1.y * p.y, f2.x * p.x + f2.y * p.y); };

    auto rotatedSquares = std::vector<std::vector<cv::Point2d>>(squares.size(), std::vector<cv::Point2d>(4, cv::Point2d(0., 0.)));
    for (size_t iSquare = 0; iSquare < squares.size(); ++iSquare) {
        for (size_t iCorner = 0; iCorner < 4; ++iCorner) {
            rotatedSquares[iSquare][iCorner] = rotate(squares[iSquare][iCorner]);
        }
    }



    for (const auto edge : {Edge::LEFT, Edge::TOP, Edge::RIGHT, Edge::BOTTOM}) {

    }
    return std::map<Edge, std::vector<std::vector<cv::Point>>>();
}

}
