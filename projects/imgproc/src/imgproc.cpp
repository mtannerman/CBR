#include "imgproc/imgproc.h"
#include "common/logging.h"
#include "common/config.h"
#include "imgproc/find_raw_squares.h"
#include "imgproc/apply_corner_cluster_filtering.h"
#include "imgproc/apply_size_filtering.h"
#include "imgproc/order_squares.h"
#include "imgproc/complete_missing_squares.h"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <limits>
#include "common/exceptions.h"
#include "imgproc/find_edge_squares.h"

namespace cbr
{

cv::Mat preprocess_image(const cv::Mat& image)
{
	const auto rawSquares = find_squares(image);
	auto filteredSquares = apply_size_filtering(rawSquares, image.size());
	filteredSquares = apply_cluster_filtering(filteredSquares);
	order_squares(filteredSquares);
	const auto dominantEdgeDirections = find_dominant_edgedirections(filteredSquares);

	const auto& f1 = dominantEdgeDirections[0];
	const auto& f2 = dominantEdgeDirections[1];

	auto rotate = [&f1, &f2](const Point& p){ return Point(f1.x * p.x + f1.y * p.y, f2.x * p.x + f2.y * p.y); };
	cv::Mat rotMx(2, 3, CV_64F, cv::Scalar(0.0));
	rotMx.at<double>(0, 0) = f1.x;
	rotMx.at<double>(0, 1) = f1.y;
	rotMx.at<double>(1, 0) = f2.x;
	rotMx.at<double>(1, 1) = f2.y;
	const auto size = cv::Size(2 * image.cols, 2 * image.rows);
	cv::Mat rotatedImage;
	cv::warpAffine(image, rotatedImage, rotMx, size, cv::INTER_LINEAR, cv::BORDER_CONSTANT);

	if (Config::GetInstance().GetBool("visualizeRotatedImage")) {
        for (const auto& square : filteredSquares) {
          for (int i = 0; i < 4; ++i) {
            const auto point = rotate(square.corners[i]);
            const auto nextPoint = rotate(square.corners[i < 3 ? (i + 1) : 0]);
            cv::line(rotatedImage, cv::Point(point.x, point.y), cv::Point(nextPoint.x, nextPoint.y), cv::Scalar(255., 0., 0.), 2);
          }
        }
        cv::namedWindow("rotimg", 1);
        cv::imshow("rotimg", rotatedImage);
        cv::waitKey();
    }

	complete_missing_squares(filteredSquares);
	

	return rotatedImage;
}

}


