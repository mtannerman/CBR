#include "imgproc/complete_missing_squares.h"
#include <array>


namespace cbr
{

std::array<bool, 4> compute_number_of_neighbors(
    const std::vector<std::vector<cv::Point>>& squares,
    const size_t iSquare)
{
    for (int iCorner = 0; iCorner < 4; ++iCorner) {
        for (size_t i = 0; i < squares.size(); ++i) {
            if (i == iSquare) {
                continue;
            }


        }
    }

    return std::array<bool, 4>();
}

std::vector<std::vector<cv::Point>> complete_missing_squares(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<std::vector<cv::Point>> fullBoard;
    return fullBoard;
}



}   // cbr