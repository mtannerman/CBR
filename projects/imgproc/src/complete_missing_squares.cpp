#include "imgproc/complete_missing_squares.h"
#include <array>


namespace cbr
{

std::array<bool, 4> compute_number_of_neighbors(
    const std::vector<std::vector<cv::Point>>& squares,
    const size_t iSquare)
{
    std::vector<int> unsetCorners{0, 1, 2, 3};   
    std::array<bool, 4> ret{false, false, false, false}; 
    for (size_t i = 0; i < squares.size(); ++i) {
        if (i == iSquare) {
            continue;
        }
        for (const auto& corner : squares[i]) {
            for (auto it = unsetCorners.begin(); it != unsetCorners.end();) {
                if (squares[iSquare][*it] == corner) {
                    ret[*it] = true;
                    it = unsetCorners.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        if (unsetCorners.empty()) {
            return ret;
        }
    }

    return ret;
}

double compute_convex_hull_score(
    const std::vector<std::vector<cv::Point>>& squares,
    const std::vector<cv::Point>& hullSquare)
{
    return 0.0;
}

std::vector<cv::Point> find_convex_hull_square(
    const std::vector<std::vector<cv::Point>>& squares)
{
    return std::vector<cv::Point>();
}

std::vector<std::vector<cv::Point>> complete_missing_squares(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<std::vector<cv::Point>> fullBoard;
    return fullBoard;
}



}   // cbr