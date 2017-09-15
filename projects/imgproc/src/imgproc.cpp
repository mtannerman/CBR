#include "imgproc/imgproc.h"
#include "common/logging.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"

#include "imgproc/find_square_contours.h"

namespace cbr
{

cv::Mat create_thresholded_image(const cv::Mat& image) 
{
	cv::Mat thres;
	constexpr int thresholdValue = 0;
	constexpr int maxBinaryValue = 255;
	constexpr int thresholdType = 3;

	cv::threshold(image, thres, 
		thresholdValue, 
		maxBinaryValue, 
		thresholdType);

	return thres;
}

void find_board(const cv::Mat& image)
{
    const int nRows = image.rows;
	const int nCols = image.cols;

	const auto thresholdedImage = create_thresholded_image(image);

	constexpr int nIntersections = 30;
	const float rowDelta = float(nRows) / nIntersections;
	const float colDelta = float(nCols) / nIntersections;

    cv::namedWindow(__FUNCTION__);
    
    const int imageSize = 800;
    const int boardSize = imageSize / 2;
    const int halfBoardSize = boardSize / 2;
    cv::Mat whiteImage = cv::Mat::ones(imageSize, imageSize, image.type());
    std::vector<cv::Point2f> destinationPoints{
        {halfBoardSize, halfBoardSize},
        {halfBoardSize + boardSize, halfBoardSize},
        {halfBoardSize + boardSize, halfBoardSize + boardSize},
        {halfBoardSize, halfBoardSize + boardSize}
    };

	const std::vector<cv::Scalar> colors{cv::Scalar(255.0, 0.0, 0.0),
	                                     cv::Scalar(0.0, 255.0, 0.0),
	                                     cv::Scalar(0.0, 0.0, 255.0)};
	int colorIndex = 0;
    const auto squares = find_squares(image);
    for (const auto& square : squares) {
        const auto homography = cv::findHomography(squares[4], destinationPoints);

        cv::Mat im_out;
        cv::warpPerspective(image, im_out, homography, whiteImage.size());

        cv::imshow("Warped Source Image", im_out);
        cv::waitKey();
    }
	/* auto imageCpy = image.clone();
	for (const auto& square : squares) {
		const auto color = colors[colorIndex];
		colorIndex = (colorIndex == 2) ? 0 : (colorIndex + 1);
		cv::line(imageCpy, square[0], square[1], color);
		cv::line(imageCpy, square[1], square[2], color);
		cv::line(imageCpy, square[2], square[3], color);
		cv::line(imageCpy, square[3], square[0], color);
	}

	cv::imshow(__FUNCTION__, imageCpy); */
}

}


