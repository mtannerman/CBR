#include "imgproc/find_edge_squares.h"
#include "math_utils/functional.h"


namespace cbr
{

Square find_extremal_square(
    const Edge edge,
    const std::vector<Square>& squares)
{
    static const std::map<Edge, std::function<double(Point)>> extremalityFunctions {
        {Edge::LEFT,   [](const Point& p){ return -p.x; }},
        {Edge::TOP,    [](const Point& p){ return +p.y; }},
        {Edge::RIGHT,  [](const Point& p){ return +p.x; }},
        {Edge::BOTTOM, [](const Point& p){ return -p.y; }},
    };

    const auto extremality = extremalityFunctions.at(edge);
    const auto squareExtremality = [extremality](const Square& square) {
        return extremality(*std::max_element(square.corners.begin(), square.corners.end(), 
        [extremality](const Point& p1, const Point& p2){
            return extremality(p1) < extremality(p2);
        }));
    };

    return *std::max_element(squares.begin(), squares.end(), 
        [squareExtremality](const Square& s1, const Square& s2) {
            return squareExtremality(s1) < squareExtremality(s2);
    });
}

std::map<Edge, std::vector<Square>> find_edge_squares(
    const std::array<Point, 2>& dominantEdgeDirections,
    const std::vector<Square>& squares)
{
    std::map<Edge, std::vector<Square>> ret;
    for (const auto edge : {Edge::LEFT, Edge::TOP, Edge::RIGHT, Edge::BOTTOM}) {
        ret[edge].push_back(find_extremal_square(edge, squares));
    }

    return ret;
}

}
