#include "imgproc/board_rotation_experiment.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "common/logging.h"
#include <math.h>

namespace cbr
{
	template <typename T>
	cv::Point3_<T> rotated_image(const int row, const int col, const float alpha, const float beta)
	{
		using std::cos;
		using std::sin;
		const auto x = T(row);
		const auto y = T(col);
		cv::Point3_<T> ret;
		ret.x = T(cos(alpha) * x - sin(alpha)* cos(beta) * y);
		ret.y = T(sin(alpha) * x + cos(alpha)* cos(beta) * y);
		ret.z = T(                             sin(beta) * y);
		return ret;
	}

	cv::Point perspective_transformed_image(
		const int row, const int col, 
		const float alpha, const float beta, 
		const float z0)
	{
		using std::cos;
		using std::sin;
		const auto rotatedImage = rotated_image<float>(row, col, alpha, beta);
		const float perspectiveFactor = z0 / (z0 + rotatedImage.z);

		cv::Point ret;
		ret.x = int(perspectiveFactor * rotatedImage.x);
		ret.y = int(perspectiveFactor * rotatedImage.y);
		return ret;

	}


	void board_rotation_experiment()
	{
		constexpr int imageSize = 16 * 55;
		constexpr unsigned char white = 255;
		constexpr unsigned char black = 0;
		constexpr int boardLength = imageSize / 2;
		constexpr int halfBoardLength = boardLength / 2;
		constexpr int squareLength = boardLength / 8;
		// constexpr float pi = 3.14159265359f;

		const float alpha = 0.0f;
		const float beta = 0.0f;
		cv::Mat image = cv::Mat::ones(imageSize, imageSize, CV_8U) * (white + black) / 2;

		for (int row = 0; row < boardLength; ++row) {
			const int rank = row / squareLength;
			for (int col = 0; col < boardLength; ++col) {
				const int file = col / squareLength;
				const bool isEven = (file + rank) % 2 == 0;
				const auto color = isEven ? white : black;

				//LOG((halfBoardLength + row) << ", " << (halfBoardLength + col));

				//image.at<uchar>(halfBoardLength + row, halfBoardLength + col) = color;
				const auto perspectiveTransformedImage = perspective_transformed_image(row, col, alpha, beta, 1000.0f) + cv::Point(1, 1) * halfBoardLength;
				image.at<uchar>(perspectiveTransformedImage) = color;

			}
		}

		cv::namedWindow(__FUNCTION__);
		cv::imshow(__FUNCTION__, image);
	}
}
