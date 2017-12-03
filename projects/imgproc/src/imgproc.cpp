#include "imgproc/imgproc.h"
#include "common/logging.h"
#include "common/config.h"
#include "imgproc/find_square_contours.h"
#include "imgproc/apply_corner_cluster_filtering.h"
#include "imgproc/apply_size_filtering.h"
#include "imgproc/order_squares.h"
#include "imgproc/complete_missing_squares.h"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <limits>

namespace cbr
{

cv::Mat preprocess_image(const cv::Mat& image)
{
	const auto rawSquares = find_squares(image);
	auto filteredSquares = apply_size_filtering(rawSquares, image.size());
	filteredSquares = apply_cluster_filtering(filteredSquares);
	order_squares(filteredSquares);
	const auto dominantEdgeDirections = find_dominant_edgedirections(filteredSquares);

	// const auto center = cv::Point3f(image.cols / 2.0f, image.rows / 2.0f);
	// const auto rotationMatrix = cv::Mat(cv::Matx22d(dominantEdgeDirections[0].x, dominantEdgeDirections[1].x, 
													// dominantEdgeDirections[0].y, dominantEdgeDirections[1].y));
													
	// cv::Mat rotationMatrix(2, 2, CV_64F);
	// rotationMatrix.at<float>(0, 0) = dominantEdgeDirections[0].x;
	// rotationMatrix.at<float>(0, 1) = dominantEdgeDirections[1].x;
	// rotationMatrix.at<float>(1, 0) = dominantEdgeDirections[0].y;
	// rotationMatrix.at<float>(1, 1) = dominantEdgeDirections[1].y;
	
	const auto& f1 = dominantEdgeDirections[0];
	const auto& f2 = dominantEdgeDirections[1];
	const auto map_point = [&f1, &f2](const int x, const int y) { 
		return cv::Point2d(
			f2.y * x - f2.x * y,
			-f1.y * x + f1.x * y); 
	};

	const std::vector<cv::Point> imageCorners{
		{0, 0},
		{0, image.cols},
		{image.rows, 0},
		{image.rows, image.cols}
	};

	auto mapped_x_min = std::numeric_limits<double>::max();
	auto mapped_y_min = std::numeric_limits<double>::max();
	auto mapped_x_max = std::numeric_limits<double>::lowest();
	auto mapped_y_max = std::numeric_limits<double>::lowest();

	for (const auto& c : imageCorners) {
		const auto mappedPoint = map_point(c.x, c.y);
		LOG(DESC(mappedPoint));
		mapped_x_min = std::min(mapped_x_min, mappedPoint.x);
		mapped_y_min = std::min(mapped_y_min, mappedPoint.y);
		mapped_x_max = std::max(mapped_x_max, mappedPoint.x);
		mapped_y_max = std::max(mapped_y_max, mappedPoint.y);
	}

	// LOG(DESC(mapped_x_min));
	// LOG(DESC(mapped_y_min));
	// LOG(DESC(mapped_x_max));
	// LOG(DESC(mapped_y_max));

	const auto imageSize = cv::Size(int(mapped_x_max - mapped_x_min), int(mapped_y_max - mapped_y_min));
	// LOG(DESC(imageSize));
	cv::Mat rotatedImage(imageSize, image.type(), cv::Scalar(0., 0., 0.));
	// LOG(DESC(image.type()));
	for (int row = 0; row < image.cols; ++row) {
		for (int col = 0; col < image.rows; ++col) {
			// LOG(DESC(image.size()));
			// LOG(DESC(row));
			// LOG(DESC(col));
			// LOG(DESC(image.rows));
			// LOG(DESC(image.cols));
			const auto mappedPoint = cv::Point(map_point(row, col));
			// LOG(DESC(mappedPoint));
			// LOG(DESC((mappedPoint.x - int(mapped_x_min))));
			// LOG(DESC((mappedPoint.y - int(mapped_y_min))));
			rotatedImage.at<uchar>(mappedPoint.x - int(mapped_x_min), mappedPoint.y - int(mapped_y_min)) = image.at<uchar>(row, col);
			// cv::Mat fromImg(image.size(), image.type(), cv::Scalar(0.,0.,0.));
			// cv::Mat toImg(rotatedImage.size(), rotatedImage.type(), cv::Scalar(0.,0.,0.));
			// cv::circle(fromImg, cv::Point(row, col), 3, cv::Scalar(0., 0., 255.));
			// cv::circle(toImg, cv::Point(mappedPoint.x - int(mapped_x_min), mappedPoint.y - int(mapped_y_min)), 3, cv::Scalar(0., 0., 255.));
			// cv::imshow("fromimg", fromImg);
			// cv::imshow("toimg", toImg);
			// cv::waitKey(5);
		}
	}

	if (Config::GetInstance().GetBool("visualizeRotatedImage")) {
		cv::namedWindow("rotimg", 1);
		cv::imshow("rotimg", rotatedImage);
		cv::waitKey();
    }

	return rotatedImage;
}

}


