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
													
	cv::Mat rotationMatrix(2, 2, CV_64F);
	rotationMatrix.at<float>(0, 0) = dominantEdgeDirections[0].x;
	rotationMatrix.at<float>(0, 1) = dominantEdgeDirections[1].x;
	rotationMatrix.at<float>(1, 0) = dominantEdgeDirections[0].y;
	rotationMatrix.at<float>(1, 1) = dominantEdgeDirections[1].y;
	
	cv::Mat rotatedImage;
	cv::warpAffine(image, rotatedImage, rotationMatrix, cv::Size(image.cols, image.rows));

	if (Config::GetInstance().GetBool("visualizeRotatedImage")) {
		cv::imshow("rotated image", rotatedImage);
    }

	return rotatedImage;
}

}


