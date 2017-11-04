#include "imgproc/complete_missing_squares.h"
#include <array>
#include "common/exceptions.h"
#include <cmath>
#include "math_utils/kmeans.h"
#include "common/viz2d.h"
#include "common/config.h"

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
        return kMeansComputer.Fit(edges);
    }
};

void visualize_dominant_edge_directions(
    const std::vector<std::vector<cv::Point>>& squares,
    const std::vector<std::vector<double>>& edgeClusterCenters)
{
    viz::Visualizer2D vizWindow(STR("dbg" << __FILE__ << " " << __PRETTY_FUNCTION__));
    
    cv::Point center;
    double edgeLength = 0;
    for (const auto& square : squares) {
        for (const auto& corner : square) {
            vizWindow.AddCircle(corner, 5, cv::Scalar(255, 0, 0));
            center += corner;
        }
        for (int i = 0; i < 4; ++i) {
            const auto& prevCorner = square[i];
            const auto nextIndex = ((i + 1) == 4 ? 0 : 1);
            const auto& currCorner = square[nextIndex];
            edgeLength += cv::norm(currCorner - prevCorner);
        }
    }

    center /= int(4 * squares.size());
    edgeLength /= (4.0 * double(squares.size()));

    for (int i = 0; i < 4; ++i) {
        const auto edge = cv::Point(int(edgeClusterCenters[i][0]), (edgeClusterCenters[i][1]));
        vizWindow.AddArrow(center, center + 4 * edgeLength * cv::Point(edge), cv::Scalar(0, 0, 255));
    }

    vizWindow.Spin();
}

void set_edge_vector(std::vector<double>& edgeVec, const cv::Point& p1, const cv::Point& p2)
{
    edgeVec[0] = p2.x - p1.x;
    edgeVec[1] = p2.y - p1.y;
}

std::array<cv::Point2d, 2> find_dominant_edgedirections(
    const std::vector<std::vector<cv::Point>>& squares)
{
    const size_t numberOfEdges = squares.size() * 4;
    std::vector<std::vector<double>> edges(2 * numberOfEdges, std::vector<double>(2, 0.0));
    size_t iEdge = 0;
    std::vector<double> edgeVec(2);
    for (const auto& square : squares) {
        for (int i = 0; i < 4; ++i) {
            const int nextIndex = (i < 3) ? (i + 1) : 0;
            set_edge_vector(edgeVec, square[i], square[nextIndex]);
            edges[iEdge][0] = edgeVec[0]; 
            edges[iEdge][1] = edgeVec[1]; 
            ++iEdge;
            edges[iEdge][0] = -edgeVec[0]; 
            edges[iEdge][1] = -edgeVec[1];
            ++iEdge;            
        }
    }


    const auto edgeClusterCenters = EdgeAngleFitter().Fit(edges);

    if (Config::GetInstance().visualizeDominantEdgeDirections) {
        visualize_dominant_edge_directions(squares, edgeClusterCenters);
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