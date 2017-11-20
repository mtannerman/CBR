#pragma once
#include "opencv2/core.hpp"
#include <array>

namespace cbr {
bool triangle_contains_point(const std::array<cv::Point, 3>& triangle, const cv::Point& point);
bool square_contains_point(const std::vector<cv::Point>& square, const cv::Point& point);
double compute_square_point_distance(const std::vector<cv::Point>& square, const cv::Point& point);
double compute_rotation_angle(const cv::Point& start, const cv::Point& dest);
double arc_tangent(const double sine, const double cosine);
}