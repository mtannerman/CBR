#include "imgproc/apply_size_filtering.h"

#include <utility>
#include "common/logging.h"
#include "math_utils/math.h"

namespace cbr
{

double approximate_square_area(const std::vector<cv::Point>& square)
{
    const auto center = cv::Point2f(fsum(square)) / 4.0f;
    double maxDiagonalLength = -1.0;
    for (const auto& corner : square) {
        const auto fpCorner = cv::Point2f(corner);
        const double diagonalLength = cv::norm(center - fpCorner);
        maxDiagonalLength = std::max(maxDiagonalLength, diagonalLength);
    }

    return maxDiagonalLength * maxDiagonalLength * 2.0;
}

bool is_square_too_large_compared_to_image(
    const std::vector<cv::Point>& square,
    const cv::Size& imageSize)
{
    const auto imageArea = double(imageSize.width * imageSize.height);
    const auto approximateSquareArea = approximate_square_area(square);
    return approximateSquareArea > (imageArea / 10.0);
}

double compute_average_square_area(
    const std::vector<std::vector<cv::Point>>& squares)
{
    const auto squareAreaSum = fsum(squares, [=](const std::vector<cv::Point>& square){ return approximate_square_area(square); });
    return squareAreaSum / double(squares.size());
}

std::pair<double, double> compute_square_area_bounds(
    const std::vector<std::vector<cv::Point>>& squares)
{
    // ?? is lambda needed?
    const auto squareAreaSum = fsum(squares, [=](const std::vector<cv::Point>& square){ return approximate_square_area(square); });
    const auto averageSquareArea = squareAreaSum / double(squares.size());
    return {averageSquareArea / 4.0, 4.0 * averageSquareArea};
}


bool is_square_area_difference_too_large(
    const std::vector<cv::Point>& square, 
    const std::pair<double, double>& squareAreaBounds)
{
    const auto squareArea = approximate_square_area(square);
    return (squareArea > squareAreaBounds.second) || 
           (squareArea < squareAreaBounds.first);
}

std::vector<std::vector<cv::Point>> apply_size_filtering(
    const std::vector<std::vector<cv::Point>>& squares,
    const cv::Size& imageSize)
{
    auto filteredSquares = squares;

    for (auto it = filteredSquares.begin(); it != filteredSquares.end();) {
        if (is_square_too_large_compared_to_image(*it, imageSize)) {
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