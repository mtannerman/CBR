#include "imgproc/complete_missing_squares.h"
#include <array>
#include "common/exceptions.h"
#include <cmath>
#include <map>
#include <set>
#include "math_utils/kmeans.h"
#include "common/viz2d.h"
#include "common/config.h"
#include "opencv2/highgui.hpp"
#include "math_utils/geometry.h"
#include "math_utils/line.h"
#include "math_utils/functional.h"
#include <utility>

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
    const cv::Point2d& p1, 
    const cv::Point2d& p2)
{
    auto diffVec = p2 - p1;

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
    const std::vector<Square>& squares)
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

enum class BandDirection { HORIZONTAL, VERTICAL };

BandDirection edge_to_band_direction(
    const cv::Point2d& startPoint,
    const cv::Point2d& endPoint)
{
    const auto difference = endPoint - startPoint;
    if (std::abs(difference.y) < 0.5 * cv::norm(difference)) {
        return BandDirection::VERTICAL;
    }
    return BandDirection::HORIZONTAL;
}


std::array<std::pair<cv::Point2d, cv::Point2d>, 2> get_edge_pair_band_direction(
    const std::vector<cv::Point2d>& square, 
    const BandDirection b)
{
    std::array<std::pair<cv::Point2d, cv::Point2d>, 2> ret;
    auto it = ret.begin();
    for (int i = 0; i < 4; ++i) {
        const int nextIndex = i < 3 ? (i + 1) : 0;
        if (edge_to_band_direction(square[i], square[nextIndex]) == b) {
            it->first = square[i];
            it->second = square[nextIndex];
            ASSERT(it != ret.end(), "");
            ++it;
        }
    }

    return ret;
}

cv::Point2d compute_normalized_line_difference_vector(
    const std::pair<cv::Point2d, cv::Point2d>& edge,
    const cv::Point2d& point)
{
    const auto diffVec = point - edge.first;
    auto v = edge.second - edge.first;
    v /= cv::norm(v);
    auto ret = diffVec - diffVec.dot(v) * v;
    ret /= cv::norm(ret);

    return ret;
}

bool is_square_in_edge_band(
    const std::vector<cv::Point2d>& centerSquare, 
    const std::vector<cv::Point2d>& otherSquare, 
    const BandDirection b)
{

    const auto edgePair = get_edge_pair_band_direction(centerSquare, b);
    const auto otherSquareMiddle = fsum(otherSquare) / 4.;

    const auto d1 = compute_normalized_line_difference_vector(edgePair[0], otherSquareMiddle);
    const auto d2 = compute_normalized_line_difference_vector(edgePair[1], otherSquareMiddle);
    static const double sqrt2 = std::sqrt(2);

    return cv::norm(d2 - d1) > sqrt2;
}

void insert_to_band_matches(
    std::vector<std::set<size_t>>& matches,
    const size_t idx1,
    const size_t idx2)
{
    auto idx1Set = std::find_if(matches.begin(), matches.end(), [idx1](const std::set<size_t>& s){ return s.count(idx1) == 1; });
    if (idx1Set != matches.end()) {
        idx1Set->insert(idx2);
    }
    else {
        auto idx2Set = std::find_if(matches.begin(), matches.end(), [idx2](const std::set<size_t>& s){ return s.count(idx2) == 1; });
        if (idx2Set != matches.end()) {
            idx2Set->insert(idx1);
        }
        else {
            matches.push_back(std::set<size_t>());
            matches.back().insert(idx1);
            matches.back().insert(idx2);
        }
    }
}

std::vector<Square> complete_missing_squares(
    const std::vector<Square>& squares)
{
    std::vector<Square> fullBoard;
    const auto dominantEdgeDirections = find_dominant_edgedirections(squares);
    const auto& f1 = dominantEdgeDirections[0];
	const auto& f2 = dominantEdgeDirections[1];

    const auto rotate = [&f1, &f2](const cv::Point2d& p){ return f1.x * p.x + f1.y * p.y, f2.x * p.x + f2.y * p.y; };

    auto rotatedSquares = std::vector<Square>(squares.size(), Square());
    for (size_t iSquare = 0; iSquare < squares.size(); ++iSquare) {
        for (size_t iCorner = 0; iCorner < 4; ++iCorner) {
            rotatedSquares[iSquare][iCorner] = rotate(squares[iSquare][iCorner]);
        }
    }

    std::array<std::vector<std::set<size_t>>, 2> bandMatches;
    for (size_t i = 0; i < rotatedSquares.size(); ++i) {
        for (size_t j = i + 1; j < rotatedSquares.size(); ++j) {
            for (const auto bandDirection : {BandDirection::HORIZONTAL, BandDirection::VERTICAL}) {
                if (is_square_in_edge_band(rotatedSquares[i], rotatedSquares[j], bandDirection) &&
                    is_square_in_edge_band(rotatedSquares[j], rotatedSquares[i], bandDirection)) {
                    insert_to_band_matches(bandMatches[int(bandDirection)], i, j);
                }
            }
        }
    }

    std::array<std::vector<std::pair<cv::Point2d, cv::Point2d>>, 2> middleLines;
    for (const auto bandDirection : {BandDirection::HORIZONTAL, BandDirection::VERTICAL}) {
        for (const auto& matches : bandMatches[int(bandDirection)]) {
            size_t minEdgeIndex, maxEdgeIndex; 
            switch(bandDirection) {
                case BandDirection::HORIZONTAL:
                    minEdgeIndex = *std::min_element(matches.begin(), matches.end(), [&rotatedSquares](const size_t i1, const size_t i2) { return fsum(rotatedSquares[i1]).x < fsum(rotatedSquares[i2]).x; });
                    maxEdgeIndex = *std::max_element(matches.begin(), matches.end(), [&rotatedSquares](const size_t i1, const size_t i2) { return fsum(rotatedSquares[i1]).x < fsum(rotatedSquares[i2]).x; });
                break;
                case BandDirection::VERTICAL:
                    minEdgeIndex = *std::min_element(matches.begin(), matches.end(), [&rotatedSquares](const size_t i1, const size_t i2) { return fsum(rotatedSquares[i1]).y < fsum(rotatedSquares[i2]).y; });
                    maxEdgeIndex = *std::max_element(matches.begin(), matches.end(), [&rotatedSquares](const size_t i1, const size_t i2) { return fsum(rotatedSquares[i1]).y < fsum(rotatedSquares[i2]).y; });
                break;
            }
            middleLines[int(bandDirection)].push_back({fsum(rotatedSquares[minEdgeIndex]) / 4., fsum(rotatedSquares[maxEdgeIndex]) / 4.});
        }
    }

    std::vector<cv::Point2d> newMiddlePoints;
    const cv::Point2d approximateBoardMiddle = fsum(rotatedSquares, [](const auto& squareVec){ return fsum(squareVec); }) / 4. / double(rotatedSquares.size());
    const double averageEdgeLength = fsum(rotatedSquares, [](const auto& s){ return cv::norm(s[0] - s[1]) + cv::norm(s[1] - s[2]); }) / 2. / double(rotatedSquares.size());
    const double maxDistance = 10.0 * averageEdgeLength;
    for (const auto& hLineEnds : middleLines[int(BandDirection::HORIZONTAL)]) {
        const auto hLine = Line2d<double>::FromTwoPointsOnLine(hLineEnds.first, hLineEnds.second);
        for (const auto& vLineEnds : middleLines[int(BandDirection::VERTICAL)]) {
            const auto vLine = Line2d<double>::FromTwoPointsOnLine(vLineEnds.first, vLineEnds.second);
            const auto intersection = hLine.Intersection(vLine);
            // const std::string imageName = STR(CBR_FANCY_FUNCTION << "dbg");
            // viz::Visualizer2D vizWindow(imageName);
            // for (const auto& s : rotatedSquares) {
            //     vizWindow.AddCircle(cv::Point(fsum(s)) / 4, 4, viz::Color::red());
            //     for (const auto& c : s) {
            //         vizWindow.AddCircle(cv::Point(c), 3, viz::Color::blue());
            //     }
            //     for (size_t i = 0; i < 4; ++i) {
            //         vizWindow.AddLine(cv::Point(s[i]), cv::Point(s[(i + 1) % 4]), viz::Color::blue());
            //     }
            // }
            // vizWindow.AddLine(cv::Point(hLine.At(-1.)), cv::Point(hLine.At(1.)), viz::Color::blue(), 4);
            // vizWindow.AddLine(cv::Point(vLine.At(-1.)), cv::Point(vLine.At(1.)), viz::Color::yellow(), 4);

            if (cv::norm(intersection - approximateBoardMiddle) < maxDistance) {
                newMiddlePoints.push_back(intersection);
                // vizWindow.AddCircle(cv::Point(intersection), 2, viz::Color::magenta());
                // vizWindow.AddCircle(cv::Point(intersection), 4, viz::Color::magenta());
                // vizWindow.AddCircle(cv::Point(intersection), 6, viz::Color::magenta());
                // vizWindow.AddCircle(cv::Point(intersection), 8, viz::Color::magenta());
            }
            
            // vizWindow.Spin();
        }
    }

    if (Config::GetInstance().GetBool("visualizeBandMatches")) {
        const std::string imageName = STR(CBR_FANCY_FUNCTION << "dbg");
        viz::Visualizer2D vizWindow(imageName);
        for (const auto& s : rotatedSquares) {
            vizWindow.AddCircle(cv::Point(fsum(s)) / 4, 4, viz::Color::red());
            for (const auto& c : s) {
                vizWindow.AddCircle(cv::Point(c), 3, viz::Color::blue());
            }
            for (size_t i = 0; i < 4; ++i) {
                vizWindow.AddLine(cv::Point(s[i]), cv::Point(s[(i + 1) % 4]), viz::Color::blue());
            }
        }
        const auto colors = std::vector<cv::Scalar>{viz::Color::blue(), viz::Color::yellow()};
        for (const auto bandDir : {BandDirection::HORIZONTAL, BandDirection::VERTICAL}) {
            for (const auto& line : middleLines[int(bandDir)]) {
                vizWindow.AddLine(line.first, line.second, colors[int(bandDir)]);
            }
        }

        for (const auto& p : newMiddlePoints) {
            vizWindow.AddCircle(cv::Point(p), 8, viz::Color::green());
        }

        vizWindow.Spin();    
    }




    return fullBoard;
}



}   // cbr