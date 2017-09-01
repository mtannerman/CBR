#include "../inc/CBRMain/find_black_squares.h"
#include "../inc/CBRMain/logging.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include <vector>
#include <algorithm>


namespace cbr
{


constexpr int thresh = 200, N = 1;

double angle(const cv::Point& pt1, const cv::Point& pt2, const cv::Point& pt0)
{
    const auto dx1 = pt1.x - pt0.x;
    const auto dy1 = pt1.y - pt0.y;
    const auto dx2 = pt2.x - pt0.x;
    const auto dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

std::vector<std::vector<cv::Point>> findSquares(const cv::Mat& image)
{
	std::vector<std::vector<cv::Point>> squares;
	cv::Mat pyr, timg, gray0(image.size(), CV_8U), gray;
	cv::pyrDown(image, pyr, cv::Size(image.cols/2, image.rows/2));
	cv::pyrUp(pyr, timg, image.size());

	std::vector<std::vector<cv::Point>> contours;


	for( int c = 0; c < 3; c++ ) {
        const int ch[] = {c, 0};
        cv::mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            if( l == 0 ) {
                cv::Canny(gray0, gray, 0, thresh, 5);
                cv::dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
            }
            else {
                gray = gray0 >= (l+1)*255/N;
            }

            cv::findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

            std::vector<cv::Point> approx;

            for ( const auto& contour : contours )
            {
                cv::approxPolyDP(cv::Mat(contour), approx, cv::arcLength(cv::Mat(contour), true)*0.02, true);

                if ( approx.size() == 4 &&
                    std::fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
                    cv::isContourConvex(cv::Mat(approx)) ) {
                    double maxCosine = 0;

                    for ( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        const double cosine = std::fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = std::max(maxCosine, cosine);
                    }

                    if ( maxCosine < 0.3 ) {
                        squares.push_back(approx);
                    }
                }
            }
        }
    }
    LOG("found " << squares.size() << " squares.");
    return squares;
}



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
    const auto squares = findSquares(image);
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


