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

	auto rotate = [&f1, &f2](const cv::Point& p){ return cv::Point2d(f1.x * p.x + f1.y * p.y, f2.x * p.x + f2.y * p.y); };
	cv::Mat rotMx(2, 3, CV_64F, cv::Scalar(0.0));
	rotMx.at<double>(0, 0) = f1.x;
	rotMx.at<double>(0, 1) = f1.y;
	rotMx.at<double>(1, 0) = f2.x;
	rotMx.at<double>(1, 1) = f2.y;
	const auto size = cv::Size(2 * image.cols, 2 * image.rows);
	cv::Mat rotatedImage;
	cv::warpAffine(image, rotatedImage, rotMx, size, cv::INTER_LINEAR, cv::BORDER_CONSTANT);

	const auto edgeSquares = find_edge_squares(dominantEdgeDirections, filteredSquares);
	if (Config::GetInstance().GetBool("visualizeRotatedImage")) {
        for (const auto& square : filteredSquares) {
          for (int i = 0; i < 4; ++i) {
            const auto point = rotate(square.corners[i]);
            const auto nextPoint = rotate(square.corners[i < 3 ? (i + 1) : 0]);
            cv::line(rotatedImage, point, nextPoint, cv::Scalar(255., 0., 0.), 2);
          }
        }
		for (const auto& e : edgeSquares) {
			const auto& square = e.second.front();
			for (int i = 0; i < 4; ++i) {
            	const auto point = square[i];
            	const auto nextPoint = square[i < 3 ? (i + 1) : 0];
				const auto defaultAngle = std::acos((nextPoint - point).x / cv::norm(nextPoint - point));
				const auto angle = std::min(defaultAngle, CV_PI - defaultAngle) * 180.0 / CV_PI;
            	cv::line(rotatedImage, point, nextPoint, cv::Scalar(0., 0., 255.), 4);
				cv::putText(rotatedImage, STR(int(angle)), cv::Point(nextPoint + point) / 2, 1, 1, cv::Scalar(255., 0., 0.), 2);
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


