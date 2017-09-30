#include "imgproc/complete_missing_squares.h"
#include <array>
#include "common/exceptions.h"

namespace cbr
{

// std::array<bool, 4> compute_number_of_neighbors(
//     const std::vector<std::vector<cv::Point>>& squares,
//     const size_t iSquare)
// {
//     std::vector<int> unsetCorners{0, 1, 2, 3};   
//     std::array<bool, 4> ret{false, false, false, false}; 
//     for (size_t i = 0; i < squares.size(); ++i) {
//         if (i == iSquare) {
//             continue;
//         }
//         for (const auto& corner : squares[i]) {
//             for (auto it = unsetCorners.begin(); it != unsetCorners.end();) {
//                 if (squares[iSquare][*it] == corner) {
//                     ret[*it] = true;
//                     it = unsetCorners.erase(it);
//                 }
//                 else {
//                     ++it;
//                 }
//             }
//         }
//         if (unsetCorners.empty()) {
//             return ret;
//         }
//     }

//     return ret;
// }

// double compute_convex_hull_score(
//     const std::vector<std::vector<cv::Point>>& squares,
//     const std::vector<cv::Point>& hullSquare)
// {
//     return 0.0;
// }

// std::vector<cv::Point> find_convex_hull_square(
//     const std::vector<std::vector<cv::Point>>& squares)
// {
//     return std::vector<cv::Point>();
// }

double initialize_phi(const std::vector<cv::Point2d>& edgeDirections)
{
    for (const auto& d : edgeDirections) {
        if (std::abs(d.x) > 1e-2) {
            return std::atan(d.y / d.x);
        }
    }
    THROW("Should be unreachable code. Couldn't initialize angle phi.");

    return 0.0;
}

double compute_angle_pair_error(
    const std::vector<double>& edgeDirections,
    const double phi, const double omega)
{
    double errorSum = 0;
    for (const double edgeAngle : edgeDirections) {
        double angleDiffMin = std::abs(phi - edgeAngle);
        angleDiffMin = std::min(angleDiffMin, std::abs(phi  + edgeAngle));
        angleDiffMin = std::min(angleDiffMin, std::abs(omega - edgeAngle));
        angleDiffMin = std::min(angleDiffMin, std::abs(omega + edgeAngle));

        errorSum += errorSum;
    }
    return errorSum;
}

std::array<cv::Point2d, 2> find_two_projective_line_means(
    const std::vector<double>& edgeDirections)
{
    // double phi = initialize_phi(edgeDirections);
    // constexpr double phiOver2 = 1.5707963;
    // double omega = phiOver2;

    // constexpr int nIterations = 10;
    // constexpr double phiDelta = 0.01;
    // constexpr double omegaDelta = 0.01;

    // for (int i = 0; i < nIterations; ++i) {
        
    // }

    // std::array<std::array<>>


    return std::array<cv::Point2d, 2>();
}

double to_edgedirection(cv::Point2d p)
{
    const double pNorm = cv::norm(p);
    ASSERT(pNorm != 0.0, "");
    p /= pNorm;
    return std::atan2(p.y, p.x);
}

std::array<double, 2> square_edgedirections(const std::vector<cv::Point>& square)
{
    std::array<double, 2> ret;
    ret[0] = to_edgedirection(cv::Point2d((square[1] - square[0]) + (square[2] - square[3])));
    ret[1] = to_edgedirection(cv::Point2d((square[2] - square[1]) + (square[3] - square[0])));

    return ret;
}

std::array<cv::Point2d, 2> find_dominant_edgedirections(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<double> edgeDirections;
    edgeDirections.reserve(2 * squares.size());
    for (const auto& square : squares) {
        const auto edgeDirectionsOfSquare = square_edgedirections(square);
        edgeDirections.push_back(edgeDirectionsOfSquare[0]);
        edgeDirections.push_back(edgeDirectionsOfSquare[1]);
    }

    return std::array<cv::Point2d, 2>();
}

std::vector<std::vector<cv::Point>> complete_missing_squares(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<std::vector<cv::Point>> fullBoard;
    return fullBoard;
}



}   // cbr