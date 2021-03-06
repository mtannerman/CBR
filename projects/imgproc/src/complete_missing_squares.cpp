#include "math_utils/square.h"
#include "common/exceptions.h"
#include <set>
#include "math_utils/kmeans.h"
#include "common/viz2d.h"
#include "common/config.h"
#include "opencv2/highgui.hpp"
#include "math_utils/geometry.h"
#include "math_utils/functional.h"
#include "math_utils/linear_regression.h"

#include "math_utils/matrix.h"

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
        const std::string imageName = "visualizeDominantEdgeDirections";
        viz::Visualizer2D vizWindow(imageName);
        LOG(DESC(edges.size()));
        for (const auto& point : edges) {
            const auto p = cv::Point(int(point[0]), int(point[1]));
            vizWindow.AddCircle(p.x, p.y, 2, viz::Color::red());
        }
    
        for (const auto& point : dominantEdges) {
            const auto p = cv::Point(int(point[0]), int(point[1]));
            vizWindow.AddCircle(p.x, p.y, 2, viz::Color::yellow());
        }
    
        cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
        const auto image = vizWindow.CreateImage();
        cv::imshow(imageName, image);
        cv::waitKey();
    }
};

void set_edge_vector(std::vector<double>& edgeVec, 
    const Point& p1, 
    const Point& p2)
{
    auto diffVec = p2 - p1;

    edgeVec[0] = diffVec.x;
    edgeVec[1] = diffVec.y;
}

std::array<Point, 2> choose_two_dominant_edge_cluster_centers(
    std::vector<std::vector<double>> edgeClusterCenters)
{
    auto rightMostIt = std::max_element(edgeClusterCenters.begin(), edgeClusterCenters.end(),
        [](const std::vector<double>& v1, const std::vector<double>& v2)
        { return v1[0] < v2[0]; });

    const auto rightMost = Point(*rightMostIt);

    edgeClusterCenters.erase(rightMostIt);

    const auto mostVertical = Point(*std::min_element(edgeClusterCenters.begin(), edgeClusterCenters.end(),
        [rightMost](const std::vector<double>& v1, const std::vector<double>& v2)
        { return compute_rotation_angle(rightMost, Point(v1)) < compute_rotation_angle(rightMost, Point(v2)); }));

    return std::array<Point, 2>{
        rightMost.Normalized(), 
        mostVertical.Normalized()};

}

std::array<Point, 2> find_dominant_edgedirections(
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

enum class Direction { HORIZONTAL, VERTICAL };

Direction OrthogonalBandDirection(Direction b)
{
	if (b == Direction::HORIZONTAL) {
		return Direction::VERTICAL;
	}

	return Direction::HORIZONTAL;
}

Direction edge_to_band_direction(
    const Point& startPoint,
    const Point& endPoint)
{
    const auto difference = endPoint - startPoint;
    if (std::abs(difference.y) < 0.5 * difference.Norm()) {
        return Direction::VERTICAL;
    }
    return Direction::HORIZONTAL;
}


std::array<std::pair<Point, Point>, 2> get_edge_pair_band_direction(
    const Square& square, 
    const Direction b)
{
    std::array<std::pair<Point, Point>, 2> ret;
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

Point compute_normalized_line_difference_vector(
    const std::pair<Point, Point>& edge,
    const Point& point)
{
    const auto diffVec = point - edge.first;
    const auto v = (edge.second - edge.first).Normalized();
    const auto ret = (diffVec - diffVec.Dot(v) * v).Normalized();

    return ret;
}

bool is_square_in_edge_band(
    const Square& centerSquare, 
    const Square& otherSquare, 
    const Direction b)
{
    const auto edgePair = get_edge_pair_band_direction(centerSquare, b);

    const auto d1 = compute_normalized_line_difference_vector(edgePair[0], otherSquare.middle);
    const auto d2 = compute_normalized_line_difference_vector(edgePair[1], otherSquare.middle);

    static const double sqrt2 = std::sqrt(2);
    return d2.Distance(d1) > sqrt2;
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

void visualize_square_completion(
    const std::string& windowName,
    const std::vector<Square>& squares,
    const std::array<std::vector<Line2d>, 2>& middleLines,
    const std::vector<Point>& middleLineIntersections,
    const std::vector<Point>& allMiddlePoints,
    const std::vector<std::pair<Square, Direction>>& squaresWithOneMiddleLine,
    const double averageEdgeLength,
    const std::vector<std::pair<Line2d, Direction>>& extrapolatedLines)
{
    const std::string imageName = STR("square_completion" << windowName);
    viz::Visualizer2D vizWindow(imageName);
    for (const auto& s : squares) {
        vizWindow.AddCircle(s.middle.x, s.middle.y, 4, viz::Color::red());
        for (const auto& c : s.corners) {
            vizWindow.AddCircle(c.x, c.y, 3, viz::Color::blue());
        }
        for (size_t i = 0; i < 4; ++i) {
            vizWindow.AddLine(s[i].x, s[i].y, s[(i + 1) % 4].x, s[(i + 1) % 4].y, viz::Color::blue());
        }
    }

    for (const auto& p : middleLineIntersections) {
        vizWindow.AddCircle(p.x, p.y, 8, viz::Color::magenta());
    }
    for (const auto& p : allMiddlePoints) {
        vizWindow.AddCircle(p.x, p.y, 6, viz::Color::cyan());
    }

    for (const auto& p : squaresWithOneMiddleLine) {
        const double c = 5.;
        const auto& s = p.first;
        vizWindow.AddLine(s.middle.x - c, s.middle.y - c, s.middle.x + c, s.middle.y + c, viz::Color::red()); 
        vizWindow.AddLine(s.middle.x - c, s.middle.y + c, s.middle.x + c, s.middle.y - c, viz::Color::red()); 
    }

    const auto colors = std::vector<viz::Color>{viz::Color::magenta(), viz::Color::yellow()};
    for (const auto bandDir : {Direction::HORIZONTAL, Direction::VERTICAL}) {
        for (const auto& line : middleLines[int(bandDir)]) {
            const auto thres = averageEdgeLength / 3.;
            const auto c = std::count_if(allMiddlePoints.begin(), allMiddlePoints.end(), [thres, &line](const Point& p){ return line.DistanceFromPoint(p) < thres; });
            for (const auto& s : squares) {
                if (line.DistanceFromPoint(s.middle) < thres) {
                    vizWindow.AddArrow(line.At(0.5).x, line.At(0.5).y, s.middle.x, s.middle.y, colors[int(bandDir)]);
                }
            }
            vizWindow.AddText(STR(c), line.At(0.5).x, line.At(0.5).y, 0, 1, colors[int(bandDir)]);
            vizWindow.AddLine(line.P.x, line.P.y, line.At(1.).x, line.At(1.).y, colors[int(bandDir)]);
        }
    }

    for (const auto& lineDir : extrapolatedLines) {
        const auto& line = lineDir.first;
        vizWindow.AddLine(line.P.x, line.P.y, line.At(1.).x, line.At(1.).y, colors[int(lineDir.second)], 3, 5);
    }

    vizWindow.Spin();
}

Matrix compute_rotation_matrix(
    const std::array<Point, 2>& dominantEdgeDirections)
{
    const auto& f1 = dominantEdgeDirections[0];
	const auto& f2 = dominantEdgeDirections[1];

    return Matrix(
        f1.x, f1.y,
        f2.x, f2.y);
}

std::vector<Square> compute_rotated_squares(
    const std::vector<Square>& squares,
    const Matrix& rotation)
{
    auto ret = std::vector<Square>(squares.size(), Square());
    for (size_t iSquare = 0; iSquare < squares.size(); ++iSquare) {
        for (size_t iCorner = 0; iCorner < 4; ++iCorner) {
            ret[iSquare][iCorner] = rotation * squares[iSquare][iCorner];
        }
        ret[iSquare].middle = rotation * squares[iSquare].middle;
    }

    return ret;
}

std::array<std::vector<std::set<size_t>>, 2> compute_band_matches(
    const std::vector<Square>& squares)
{
    std::array<std::vector<std::set<size_t>>, 2> bandMatches;
    for (size_t i = 0; i < squares.size(); ++i) {
        for (size_t j = i + 1; j < squares.size(); ++j) {
            for (const auto b : {Direction::HORIZONTAL, Direction::VERTICAL}) {
                if (is_square_in_edge_band(squares[i], squares[j], b) &&
                    is_square_in_edge_band(squares[j], squares[i], b)) {
                    insert_to_band_matches(bandMatches[int(b)], i, j);
                }
            }
        }
    }

    return bandMatches;
}

std::array<std::vector<Line2d>, 2> compute_middle_lines_from_band_matches(
    const std::vector<Square>& squares,
    const std::array<std::vector<std::set<size_t>>, 2>& bandMatches)
{
    std::array<std::vector<Line2d>, 2> middleLines;
    for (const auto bandDirection : {Direction::HORIZONTAL, Direction::VERTICAL}) {
        for (const auto& matches : bandMatches[int(bandDirection)]) {
            size_t minEdgeIndex, maxEdgeIndex; 
            if (bandDirection == Direction::HORIZONTAL) {
                const auto mm = std::minmax_element(matches.begin(), matches.end(), [&squares](const size_t i1, const size_t i2) { return squares[i1].middle.x < squares[i2].middle.x; });
                minEdgeIndex = *mm.first;
                maxEdgeIndex = *mm.second;
            }
            else {
                const auto mm = std::minmax_element(matches.begin(), matches.end(), [&squares](const size_t i1, const size_t i2) { return squares[i1].middle.y < squares[i2].middle.y; });
                minEdgeIndex = *mm.first;
                maxEdgeIndex = *mm.second;
            }
            if (minEdgeIndex != maxEdgeIndex) {
                const auto newLine = Line2d::FromTwoPointsOnLine(squares[minEdgeIndex].middle, squares[maxEdgeIndex].middle);
                middleLines[int(bandDirection)].push_back(newLine);
            }
        }
    }

    return middleLines;
}

std::vector<Point> compute_middle_points_from_middle_line_intersections(
    const std::vector<Square>& squares,
    const std::array<std::vector<Line2d>, 2>& middleLines,
    const double averageEdgeLength)
{
    std::vector<Point> middleLineIntersections;
    const Point approximateBoardMiddle = f::sum(squares, [](const Square& s){ return s.middle; }) / double(squares.size());
    
    const double maxDistance = 10.0 * averageEdgeLength;
    for (const auto& hLine : middleLines[int(Direction::HORIZONTAL)]) {
        for (const auto& vLine : middleLines[int(Direction::VERTICAL)]) {
            const auto intersection = hLine.Intersection(vLine);

            if (intersection.Distance(approximateBoardMiddle) < maxDistance) {
                middleLineIntersections.push_back(intersection);
            }
        }
    }

    return middleLineIntersections;
}

std::vector<Point> collect_all_middle_points(
    const std::vector<Square>& squares,
    const std::vector<Point>& middleLineIntersections,
    const double averageEdgeLength)
{
    std::vector<Point> ret;
    const double thres = averageEdgeLength / 3.;
    for (const auto& middlePoint : middleLineIntersections) {
        if (std::find_if(ret.begin(), ret.end(), [&](const Point& p){ return p.IsCloserThan(middlePoint, thres); }) == ret.end()) {
            ret.push_back(middlePoint);
        }
    }

    for (const auto& square : squares) {
        const auto& middlePoint = square.middle;
        if (std::find_if(ret.begin(), ret.end(), [&](const Point& p){ return p.IsCloserThan(middlePoint, thres); }) == ret.end()) {
            ret.push_back(middlePoint);
        }
    }

    return ret;
}

void extend_middle_lines(
    std::array<std::vector<Line2d>, 2>& middleLines,
    const std::vector<Point>& allMiddlePoints,
    const double averageEdgeLength)
{
    const double lineDistanceThreshold = averageEdgeLength / 3.;
    for (const auto bandDirection : {Direction::HORIZONTAL, Direction::VERTICAL}) {
        for (auto& line : middleLines[int(bandDirection)]) {
            for (const auto& p : allMiddlePoints) {
                if (line.DistanceFromPoint(p) < lineDistanceThreshold) {
                    const double tmin = line.ClosestTimeArg(p);
                    if (tmin < 0.) {
                        line = Line2d::FromTwoPointsOnLine(p, line.P + line.v);
                    }
                    else if (tmin > 1.) {
                        line = Line2d::FromTwoPointsOnLine(line.P, p);
                    }
                }
            }
        }
    }
}

std::vector<std::pair<Square, Direction>> collect_squares_with_one_middle_line(
    const std::vector<Square>& squares,
    const std::array<std::vector<Line2d>, 2>& middleLines,
    const double averageEdgeLength)
{
    const double thres = averageEdgeLength / 3.;
    std::vector<std::pair<Square, Direction>> ret;
    for (const auto& square : squares) {
        const bool foundHorizontalLine = std::find_if(middleLines[0].begin(), middleLines[0].end(), [thres, &square](const Line2d& l){ return l.DistanceFromPoint(square.middle) < thres; }) != middleLines[0].end();
        const bool foundVerticalLine = std::find_if(middleLines[1].begin(), middleLines[1].end(), [thres, &square](const Line2d& l){ return l.DistanceFromPoint(square.middle) < thres; }) != middleLines[1].end();

        if (!foundHorizontalLine != !foundVerticalLine) {
            const auto b = foundHorizontalLine ? Direction::HORIZONTAL : Direction::VERTICAL;
            ret.push_back({square, b});
        }
    }
    return ret;
}

std::vector<std::pair<Line2d, Direction>> extrapolate_lines(
    const std::vector<std::pair<Square, Direction>>& squaresWithOneMiddleLine,
    const std::array<std::vector<Line2d>, 2>& middleLines,
    const double averageEdgeLength)
{
    const double thres = averageEdgeLength / 3.;
    std::vector<std::pair<Line2d, Direction>> extrapolatedLines;
    for (const auto& squareDirPair : squaresWithOneMiddleLine) {
        const auto& square = squareDirPair.first;
        const auto bandDirection = squareDirPair.second;
        const auto otherBandDirection = OrthogonalBandDirection(bandDirection);

        const auto& middleLinesForOtherDirection = middleLines[int(otherBandDirection)];
        std::vector<Point> distanceAngles;
        constexpr double pi = 3.141592653;
        for (const auto& line : middleLinesForOtherDirection) {
            Point newPoint;
            newPoint.x = line.DistanceFromPoint(square.middle);
            const double angle = line.v.Aligned(Point::UnitVector(size_t(bandDirection))).PolarAngle();
            distanceAngles.push_back({line.DistanceFromPoint(square.middle), angle});
        }

        const auto fitLine = fit_line(distanceAngles);

        const double extrapolatedAngle = fitLine.At(0).y;
        auto newLine = Line2d::FromAngle(extrapolatedAngle);
        newLine.P = square.middle;
        newLine.v *= averageEdgeLength;
        if (std::find_if(extrapolatedLines.begin(), extrapolatedLines.end(), [thres, &newLine](const auto& lineDir){ return newLine.DistanceFromPoint(lineDir.first.P) < thres; }) == extrapolatedLines.end()) {
            extrapolatedLines.push_back({newLine, otherBandDirection});
        }
    }

    return extrapolatedLines;
}

bool is_board_complete(const std::vector<Point>& allMiddlePoints)
{
    return allMiddlePoints.size() == 64;
}

void sort_middle_lines(
    std::array<std::vector<Line2d>, 2>& middleLines)
{
    for (const auto b : {Direction::HORIZONTAL, Direction::VERTICAL}) {
        auto& lines = middleLines[int(b)];
        std::sort(lines.begin(), lines.end(), [b](const Line2d& l1, const Line2d& l2) { 
            return l1.P[int(b)] < l2.P[int(b)];
        });
    }
}

std::array<std::array<Square, 8>, 8> assemble_full_board(
    const std::vector<Square>& squares,
    std::array<std::vector<Line2d>, 2>& middleLines)
{
    std::array<std::array<Square, 8>, 8> ret;
    sort_middle_lines(middleLines);


    const auto& hLines = middleLines[int(Direction::HORIZONTAL)];
    const auto& vLines = middleLines[int(Direction::VERTICAL)];
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const auto intersection = hLines[row].Intersection(vLines[col]);
            ret[row][col] = *std::min_element(squares.begin(), squares.end(), 
                [&intersection](const Square& s1, const Square& s2) 
                { return s1.Distance(intersection) < s2.Distance(intersection); });
        }
    }

    return ret;
}

std::array<std::array<Square, 8>, 8> complete_missing_squares(
    const std::vector<Square>& squares)
{
    const auto dominantEdgeDirections = find_dominant_edgedirections(squares);
    const auto rotationMatrix = compute_rotation_matrix(dominantEdgeDirections);
    const auto rotatedSquares = compute_rotated_squares(squares, rotationMatrix);

	const double averageEdgeLength = f::sum(rotatedSquares, [](const Square& s) { return s.Circumference(); }) / 4. / double(rotatedSquares.size());
	const auto missingSquareCompletionStrategy = Config::GetInstance().GetString("missingSquareCompletionStrategy");
	if (missingSquareCompletionStrategy == "default") {
		const auto bandMatches = compute_band_matches(rotatedSquares);
		auto middleLines = compute_middle_lines_from_band_matches(rotatedSquares, bandMatches);
		auto middleLineIntersections = compute_middle_points_from_middle_line_intersections(rotatedSquares, middleLines, averageEdgeLength);
		auto allMiddlePoints = collect_all_middle_points(rotatedSquares, middleLineIntersections, averageEdgeLength);
		extend_middle_lines(middleLines, allMiddlePoints, averageEdgeLength);
		for (int i = 0; i < 1; ++i) {
			middleLineIntersections = compute_middle_points_from_middle_line_intersections(rotatedSquares, middleLines, averageEdgeLength);
			allMiddlePoints = collect_all_middle_points(rotatedSquares, middleLineIntersections, averageEdgeLength);
			extend_middle_lines(middleLines, allMiddlePoints, averageEdgeLength);
		}

		const auto squaresWithOneMiddleLine = collect_squares_with_one_middle_line(rotatedSquares, middleLines, averageEdgeLength);
		const auto extrapolatedLines = extrapolate_lines(squaresWithOneMiddleLine, middleLines, averageEdgeLength);
		for (const auto& lineDir : extrapolatedLines) {
			middleLines[int(lineDir.second)].push_back(lineDir.first);
		}

		middleLineIntersections = compute_middle_points_from_middle_line_intersections(rotatedSquares, middleLines, averageEdgeLength);
		allMiddlePoints = collect_all_middle_points(rotatedSquares, middleLineIntersections, averageEdgeLength);
		extend_middle_lines(middleLines, allMiddlePoints, averageEdgeLength);

		if (Config::GetInstance().GetBool("visualizeBandMatches")) {
			visualize_square_completion("second", rotatedSquares, middleLines,
				middleLineIntersections, allMiddlePoints,
				squaresWithOneMiddleLine, averageEdgeLength,
				extrapolatedLines);
		}
	}
	else if (missingSquareCompletionStrategy == "middlePointBased") {
		// auto middlePoints = collect_all_middle_points(rotatedSquares, std::vector<Point>(), averageEdgeLength);
		if (Config::GetInstance().GetBool("visualizeBandMatches")) {
			viz::Visualizer2D vizWindow("middlePointBased");
			for (const auto& s : rotatedSquares) {
				vizWindow.AddCircle(s.middle.x, s.middle.y, 4, viz::Color::red());
				for (const auto& c : s.corners) {
					vizWindow.AddCircle(c.x, c.y, 3, viz::Color::blue());
				}
				for (size_t i = 0; i < 4; ++i) {
					vizWindow.AddLine(s[i].x, s[i].y, s[(i + 1) % 4].x, s[(i + 1) % 4].y, viz::Color::blue());
				}
			}
			vizWindow.Spin();
		}
	}
	else {
		THROW(BadProgramInput, "unknown missingSquareCompletionStrategy");
	}

    // ASSERT(is_board_complete(middlePoints), "");

    return std::array<std::array<Square, 8>, 8>();
}



}   // cbr
