#include "imgproc/apply_corner_cluster_filtering.h"
#include <utility>
#include <limits>
#include <cstdint>
#include "math_utils/math.h"

namespace cbr 
{

struct CornerCluster
{
    CornerCluster() = default;
    CornerCluster(const cv::Point& corner)
        : mean(cv::Point2f(corner))
    {}
    
    void Update(const cv::Point& newPoint)
    {
        mean = (mean * float(numberOfElementsInCluster) + cv::Point2f(newPoint)) / float(numberOfElementsInCluster + 1);
        numberOfElementsInCluster++;
    }

    double Distance(const cv::Point& corner) const
    {
        return cv::norm(cv::Point2f(corner) - mean);
    }

    cv::Point2f mean;
    int numberOfElementsInCluster = 1;
};

std::pair<size_t, double> find_closest_cluster(
    const std::vector<cv::Point>& corners, 
    const std::vector<CornerCluster>& clusters,
    const cv::Point& corner)
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

std::vector<cv::Point> squares_to_corners(const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<cv::Point> corners;
    corners.reserve(squares.size() *4);
    for (const auto& square : squares) {
        corners.insert(corners.end(), square.begin(), square.end());
    }

    return corners;
}

double compute_square_edge_length_average(const std::vector<std::vector<cv::Point>>& squares)
{
    const double edgeLengthSum = fsum(squares, [](const std::vector<cv::Point>& s) 
    { return cv::norm(s[0] - s[1]) + cv::norm(s[1] - s[2]) + cv::norm(s[2] - s[3]) + cv::norm(s[3] - s[0]);});

    return edgeLengthSum / double(squares.size()) / 4.0;
}

std::vector<cv::Point2f> find_cluster_corners(const std::vector<std::vector<cv::Point>>& squares)
{
    const auto corners = squares_to_corners(squares);
    const double squareEdgeLengthAverage = compute_square_edge_length_average(squares);
    const double clusteringDistance = squareEdgeLengthAverage / 2.0;

    int clusterIndex = 0;
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

    std::vector<cv::Point2f> clusterCenters;
    clusterCenters.reserve(cornerClusters.size());
    for (const auto& cornerCluster : cornerClusters) {
        clusterCenters.push_back(cornerCluster.mean);
    }

    return clusterCenters;
}

std::vector<std::vector<cv::Point>> apply_cluster_filtering(
    const std::vector<std::vector<cv::Point>>& squares)
{
    return std::vector<std::vector<cv::Point>>();
}

}