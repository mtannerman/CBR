#include "imgproc/complete_missing_squares.h"
#include <array>
#include "common/exceptions.h"
#include <cmath>
#include "math_utils/kmeans.h"
#include "common/viz2d.h"
#include "common/config.h"
#include "opencv2/highgui.hpp"
#include "math_utils/geometry.h"

namespace cbr
{

struct EdgeAngleFitter
{
    std::vector<std::vector<double>> Fit(const std::vector<std::vector<double>>& edges)
    {
        KMeansComputer kMeansComputer(4);
        auto& centers = kMeansComputer.GetCenters();
        for (int i = 0; i < 4; ++i) {
            centers[i] = edges[i];
        }

        kMeansComputer.AreCentersInitialized() = true;
        const auto dominantEdges = kMeansComputer.Fit(edges);

        if (Config::GetInstance().GetBool("visualizeDominantEdgeDirections")) {
            VisualizeKmeans(edges, dominantEdges);
        }

        return dominantEdges;
    }

    void VisualizeKmeans(const std::vector<std::vector<double>>& edges, 
        const std::vector<std::vector<double>>& dominantEdges)
    {
        const std::string imageName = STR(CBR_FANCY_FUNCTION << "dbg");
        viz::Visualizer2D vizWindow(imageName);
        LOG(DESC(edges.size()));
        for (const auto& point : edges) {
            const auto p = cv::Point(int(point[0]), int(point[1]));
            vizWindow.AddCircle(p, 2, viz::Color::red());
        }
    
        for (const auto& point : dominantEdges) {
            const auto p = cv::Point(int(point[0]), int(point[1]));
            vizWindow.AddCircle(p, 2, viz::Color::yellow());
        }
    
        cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
        const auto image = vizWindow.CreateImage();
        cv::imshow(imageName, image);
        cv::waitKey();
    }
};

void set_edge_vector(std::vector<double>& edgeVec, 
    const cv::Point& p1, 
    const cv::Point& p2)
{
    auto diffVec = cv::Point2d(p2 - p1);

    edgeVec[0] = diffVec.x;
    edgeVec[1] = diffVec.y;
}

cv::Point2d vec2Point2d(const std::vector<double>& v)
{
    ASSERT(v.size() == 2, "");
    return cv::Point2d(v[0], v[1]);  
}

std::array<cv::Point2d, 2> choose_two_dominant_edge_cluster_centers(
    std::vector<std::vector<double>> edgeClusterCenters)
{
    auto rightMostIt = std::max_element(edgeClusterCenters.begin(), edgeClusterCenters.end(),
        [](const std::vector<double>& v1, const std::vector<double>& v2)
        { return v1[0] < v2[0]; });

    const auto rightMost = vec2Point2d(*rightMostIt);

    edgeClusterCenters.erase(rightMostIt);

    const auto mostVertical = vec2Point2d(*std::min_element(edgeClusterCenters.begin(), edgeClusterCenters.end(),
        [rightMost](const std::vector<double>& v1, const std::vector<double>& v2)
        { return compute_rotation_angle(rightMost, vec2Point2d(v1)) < compute_rotation_angle(rightMost, vec2Point2d(v2)); }));

    return std::array<cv::Point2d, 2>{
        rightMost / cv::norm(rightMost), 
        mostVertical / cv::norm(mostVertical)};

}

std::array<cv::Point2d, 2> find_dominant_edgedirections(
    const std::vector<std::vector<cv::Point>>& squares)
{
    const size_t numberOfEdges = squares.size() * 4;
    std::vector<std::vector<double>> edges(numberOfEdges, std::vector<double>(2, 0.0));
    size_t iEdge = 0;
    std::vector<double> edgeVec(2);
    for (const auto& square : squares) {
        for (int i = 0; i < 4; ++i) {
            const int nextIndex = (i < 3) ? (i + 1) : 0;
            set_edge_vector(edgeVec, square[i], square[nextIndex]);
            edges[iEdge][0] = edgeVec[0]; 
            edges[iEdge][1] = edgeVec[1]; 
            ++iEdge;
        }
    }

    const auto edgeClusterCenters = EdgeAngleFitter().Fit(edges);

    return choose_two_dominant_edge_cluster_centers(edgeClusterCenters);
}

std::vector<std::vector<cv::Point>> complete_missing_squares(
    const std::vector<std::vector<cv::Point>>& squares)
{
    std::vector<std::vector<cv::Point>> fullBoard;
    return fullBoard;
}



}   // cbr