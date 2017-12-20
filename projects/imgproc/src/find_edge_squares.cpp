#include "imgproc/find_edge_squares.h"
#include "math_utils/functional.h"

/// OBSOLETE!!!!!!!!!!!!!!
/// OBSOLETE!!!!!!!!!!!!!!
/// OBSOLETE!!!!!!!!!!!!!!
/// OBSOLETE!!!!!!!!!!!!!!

namespace cbr
{

std::vector<cv::Point2d> find_extremal_square(
    const Edge edge,
    const std::vector<std::vector<cv::Point2d>>& rotatedSquares)
{
    static const std::map<Edge, std::function<double(cv::Point2d)>> extremalityFunctions {
        {Edge::LEFT,   [](const cv::Point2d& p){ return -p.x; }},
        {Edge::TOP,    [](const cv::Point2d& p){ return +p.y; }},
        {Edge::RIGHT,  [](const cv::Point2d& p){ return +p.x; }},
        {Edge::BOTTOM, [](const cv::Point2d& p){ return -p.y; }},
    };

    const auto extremality = extremalityFunctions.at(edge);
    const auto squareExtremality = [extremality](const std::vector<cv::Point2d>& square) {
        return extremality(*std::max_element(square.begin(), square.end(), 
        [extremality](const cv::Point2d& p1, const cv::Point2d& p2){
            return extremality(p1) < extremality(p2);
        }));
    };

    return *std::max_element(rotatedSquares.begin(), rotatedSquares.end(), 
        [squareExtremality](const std::vector<cv::Point2d>& s1, const std::vector<cv::Point2d>& s2) {
            return squareExtremality(s1) < squareExtremality(s2);
    });
}

std::map<Edge, std::vector<std::vector<cv::Point2d>>> find_edge_squares(
    const std::array<cv::Point2d, 2>& dominantEdgeDirections,
    const std::vector<std::vector<cv::Point>>& squares)
{

    const auto& f1 = dominantEdgeDirections[0];
	const auto& f2 = dominantEdgeDirections[1];

	auto rotate = [&f1, &f2](const cv::Point& p){ return cv::Point2d(f1.x * p.x + f1.y * p.y, f2.x * p.x + f2.y * p.y); };

    auto rotatedSquares = std::vector<std::vector<cv::Point2d>>(squares.size(), std::vector<cv::Point2d>(4, cv::Point2d(0., 0.)));
    for (size_t iSquare = 0; iSquare < squares.size(); ++iSquare) {
        for (size_t iCorner = 0; iCorner < 4; ++iCorner) {
            rotatedSquares[iSquare][iCorner] = rotate(squares[iSquare][iCorner]);
        }
    }

    std::map<Edge, std::vector<std::vector<cv::Point2d>>> ret;
    for (const auto edge : {Edge::LEFT, Edge::TOP, Edge::RIGHT, Edge::BOTTOM}) {
        ret[edge].push_back(find_extremal_square(edge, rotatedSquares));
    }

    // const double edgeLengthAverage = fsum(rotatedSquares, [](const std::vector<cv::Point2d>& s) {
    //         return cv::norm(s[0] - s[1]) + cv::norm(s[1] - s[2]) + cv::norm(s[2] - s[3]) + cv::norm(s[3] - s[0]);
    //     }) / 4. / double(squares.size());

    return ret;
}

}
