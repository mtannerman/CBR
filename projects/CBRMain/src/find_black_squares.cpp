#include "../inc/CBRMain/find_black_squares.h"
#include "../inc/CBRMain/logging.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <algorithm>


namespace cbr
{

void rotation_experiment()
{
	// initialize chessboard and rotate it
}


constexpr int thresh = 50, N = 11;

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


	for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        cv::mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                cv::Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                cv::dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            cv::findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

            std::vector<cv::Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    std::fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
                    cv::isContourConvex(cv::Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = std::fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = std::max(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }

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

	//cv::Mat grayScale;
	//cv::cvtColor(image, grayScale, cv::COLOR_RGB2GRAY);

	const std::vector<cv::Scalar> colors{cv::Scalar(255.0, 0.0, 0.0),
	                                     cv::Scalar(0.0, 255.0, 0.0),
	                                     cv::Scalar(0.0, 0.0, 255.0)};


	int colorIndex = 0;
	const auto squares = findSquares(image);
	auto imageCpy = image.clone();
	for (const auto& tier : squares) {
		const auto color = colors[colorIndex];
		colorIndex = (colorIndex == 2) ? 0 : (colorIndex + 1);
		cv::line(imageCpy, tier[0], tier[1], color);
		cv::line(imageCpy, tier[1], tier[2], color);
		cv::line(imageCpy, tier[2], tier[3], color);
		cv::line(imageCpy, tier[3], tier[0], color);
		/*for (const auto& square : tier) {
			const auto color = cv::Scalar(0.0, 0.0, 255.0);
			cv::circle(imageCpy, square, 3, color, -1);
		}*/
	}

	/*
	LOG(DESC(image.size()));
	LOG(DESC(rowDelta));
	LOG(DESC(colDelta));

	auto imageCpy = image.clone();
	for (int i = 0; i < nIntersections; ++i) {
		cv::Point center;
		center.y = int(rowDelta / 2.0f + i * rowDelta);
		for (int j = 0; j < nIntersections;++j) {
			center.x = int(colDelta / 2.0f + j * colDelta);
			//LOG("adding circle to center " << center);
			const auto color = (i == 11 && j == 11) ? cv::Scalar(0.0, 255.0, 0.0) : cv::Scalar(0.0, 0.0, 255.0);
			cv::circle(imageCpy, center, 3, color, -1);

			if (i == 11 && j == 11) {
				LOG(center);
			}
		}
	}
	*/

	cv::imshow(__FUNCTION__, imageCpy);

}

}


