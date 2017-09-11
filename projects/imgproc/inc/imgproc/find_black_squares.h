#pragma once

#include <opencv2/core.hpp>
#include <vector>

namespace cbr
{

/*class ChessBoard
{
public:
	cv::Rect GetRoiRect(const int rank, const int file, const cv::Mat& image);
private:
	std::array<cv::Point, 4> corners;

}*/

void find_board(const cv::Mat& image);
std::vector<std::vector<cv::Point>> find_squares(const cv::Mat& image);


}

