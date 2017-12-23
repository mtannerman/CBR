#include "imgproc/apply_corner_cluster_filtering.h"
#include <utility>
#include <limits>
#include <cstdint>
#include "math_utils/functional.h"
#include "common/logging.h"
#include "common/viz2d.h"
#include "common/config.h"

namespace cbr 
{

struct CornerCluster
{
    CornerCluster() = default;
    CornerCluster(const Point& corner)
        : mean(corner)
    {}
    
    void Update(const Point& newPoint)
    {
        mean = (mean * double(numberOfElementsInCluster) + newPoint) / double(numberOfElementsInCluster + 1);
        numberOfElementsInCluster++;
    }

    double Distance(const Point& corner) const
    {
        return corner.Distance(mean);
    }

    Point mean;
    int numberOfElementsInCluster = 1;
};

std::pair<size_t, double> find_closest_cluster(
    const std::vector<Point>& corners, 
    const std::vector<CornerCluster>& clusters,
    const Point& corner)
{
    double minimalDistance = std::numeric_limits<double>::max();
    size_t closestClusterLabel = 0;
    const size_t nClusters = clusters.size();
    for (size_t iCluster = 0; iCluster < nClusters; ++iCluster) {
        const auto distance = clusters[iCluster].Distance(corner);
        if (distance < minimalDistance) {
            closestClusterLabel = iCluster;
            minimalDistance = distance;
        }
    }

    return {closestClusterLabel, minimalDistance};

}

std::vector<Point> squares_to_corners(const std::vector<Square>& squares)
{
    std::vector<Point> corners;
    corners.reserve(squares.size() * 4);
    for (const auto& square : squares) {
        corners.insert(corners.end(), square.corners.begin(), square.corners.end());
    }

    return corners;
}

double compute_square_edge_length_average(const std::vector<Square>& squares)
{
    const double edgeLengthSum = fsum(squares, [](const Square& s) { return s.Circumference(); });
    return edgeLengthSum / double(squares.size()) / 4.0;
}

std::vector<Point> find_corner_cluster_centers(
    const std::vector<Square>& squares)
{
    const auto corners = squares_to_corners(squares);
    const double squareEdgeLengthAverage = compute_square_edge_length_average(squares);
    const double clusteringDistance = squareEdgeLengthAverage / 2.0;

    auto clusterLabels = std::vector<size_t>(corners.size(), std::numeric_limits<size_t>::max());
    clusterLabels[0] = 0;
    std::vector<CornerCluster> cornerClusters;
    cornerClusters.push_back(CornerCluster(corners[0]));
    const size_t nCorners = corners.size();
    for (size_t iCorner = 1; iCorner < nCorners; ++iCorner) {
        const auto labelDistancePair = find_closest_cluster(corners, cornerClusters, corners[iCorner]);
        const bool isCornerCloseToAnExistingCluster = labelDistancePair.second < clusteringDistance;
        if (isCornerCloseToAnExistingCluster) {
            const size_t clusterLabel = labelDistancePair.first;
            clusterLabels[iCorner] = clusterLabel;
            cornerClusters[clusterLabel].Update(corners[iCorner]);
        }
        else {
            clusterLabels[iCorner] = cornerClusters.size();
            cornerClusters.push_back(CornerCluster(corners[iCorner]));
        }
    }

    std::vector<Point> clusterCenters;
    clusterCenters.reserve(cornerClusters.size());
    for (const auto& cornerCluster : cornerClusters) {
        clusterCenters.push_back(cornerCluster.mean);
    }

    if (Config::GetInstance().GetBool("visualizeSquareFiltering")) {
        viz::Visualizer2D vizWindow("squareFiltering");

        for (const auto& c : cornerClusters) {
            vizWindow.AddCircle(c.mean.x, c.mean.y, 5, viz::Color::blue());
        }

        vizWindow.Spin();
    }


    return clusterCenters;
}

Point find_closest_cluster(
    const std::vector<Point>& cornerClusterCenters,
     const Point& corner)
{
    const auto closestClusterCenter = 
        std::min_element(cornerClusterCenters.begin(), cornerClusterCenters.end(), 
        [=](const Point& lhs, const Point& rhs) { 
            return lhs.Distance(corner) < rhs.Distance(corner); });

    return *closestClusterCenter;
}

void adjust_square_corners_to_closest_cluster(
    const std::vector<Point>& cornerClusterCenters,
    Square& square)
{
    for (auto& corner : square.corners) {
        corner = find_closest_cluster(cornerClusterCenters, corner);
    }
    square.Recenter();
}

bool square_center_already_taken(
    const Point& center,
     std::vector<Point>& squareCenters)
{
    return std::find(squareCenters.begin(), squareCenters.end(), center) != squareCenters.end();
}

std::vector<Square> collect_unique_squares(
    const std::vector<Square>& squares,
     const std::vector<Point>& cornerClusterCenters)
{
    std::vector<Square> uniqueSquares;
    std::vector<Point> squareCenters;
    for (auto square : squares) {
        adjust_square_corners_to_closest_cluster(cornerClusterCenters, square);
        const auto adjustedCenter = square.middle;
        if (!square_center_already_taken(adjustedCenter, squareCenters)) {
            uniqueSquares.push_back(square);
            squareCenters.push_back(adjustedCenter);
        }
    }

    return uniqueSquares;
}

std::vector<Square> apply_cluster_filtering(
    const std::vector<Square>& squares)
{
    const auto cornerClusters = find_corner_cluster_centers(squares);
    const auto uniqueSquares = 
        collect_unique_squares(squares, cornerClusters);

    return uniqueSquares;
}

}