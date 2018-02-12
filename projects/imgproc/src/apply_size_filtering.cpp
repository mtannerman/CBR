#include "imgproc/apply_size_filtering.h"

#include <utility>
#include "common/logging.h"
#include "math_utils/functional.h"

namespace cbr
{

double approximate_square_area(const Square& square)
{
    double maxDiagonalLength = -1.0;
    for (const auto& corner : square.corners) {
        const double diagonalLength = square.middle.Distance(corner);
        maxDiagonalLength = std::max(maxDiagonalLength, diagonalLength);
    }

    return maxDiagonalLength * maxDiagonalLength * 2.0;
}

bool is_square_too_large_compared_to_image(
    const Square& square,
    const double imageArea)
{
    
    const auto approximateSquareArea = approximate_square_area(square);
    return approximateSquareArea > (imageArea / 10.0);
}

double compute_average_square_area(
    const std::vector<Square>& squares)
{
    const auto squareAreaSum = f::sum(squares, [=](const Square& square){ return approximate_square_area(square); });
    return squareAreaSum / double(squares.size());
}

std::pair<double, double> compute_square_area_bounds(
    const std::vector<Square>& squares)
{
    // ?? is lambda needed?
    const auto squareAreaSum = f::sum(squares, [=](const Square& square){ return approximate_square_area(square); });
    const auto averageSquareArea = squareAreaSum / double(squares.size());
    return {averageSquareArea / 4.0, 4.0 * averageSquareArea};
}


bool is_square_area_difference_too_large(
    const Square& square, 
    const std::pair<double, double>& squareAreaBounds)
{
    const auto squareArea = approximate_square_area(square);
    return (squareArea > squareAreaBounds.second) || 
           (squareArea < squareAreaBounds.first);
}

std::vector<Square> apply_size_filtering(
    const std::vector<Square>& squares,
    const cv::Size& imageSize)
{
    auto filteredSquares = squares;
    const double imageArea = double(imageSize.width * imageSize.height);
    for (auto it = filteredSquares.begin(); it != filteredSquares.end();) {
        if (is_square_too_large_compared_to_image(*it, imageArea)) {
            it = filteredSquares.erase(it);
        }
        else {
            ++it;
        }
    }

    const auto squareAreaBounds = compute_square_area_bounds(squares);
    for (auto it = filteredSquares.begin(); it != filteredSquares.end();) {
        if (is_square_area_difference_too_large(*it, squareAreaBounds)) {
            it = filteredSquares.erase(it);
        }
        else {
            ++it;
        }
    }

    return filteredSquares;
}

}   //cbr