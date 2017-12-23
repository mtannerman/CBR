#include "imgproc/find_raw_squares.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"

namespace cbr
{

constexpr int thresh = 200, N = 10;

double angle(const cv::Point& pt1, 
    const cv::Point& pt2, 
    const cv::Point& pt0)
{
    const auto dx1 = pt1.x - pt0.x;
    const auto dy1 = pt1.y - pt0.y;
    const auto dx2 = pt2.x - pt0.x;
    const auto dy2 = pt2.y - pt0.y;
    const auto numerator = dx1 * dx2 + dy1 * dy2;
    const auto denominator = sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
    return numerator / denominator;
}

std::vector<Square> find_squares(const cv::Mat& image)
{
    std::vector<Square> squares;
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
                        Square newSquare;
                        for (size_t i = 0; i < 4; ++i) {
                            newSquare[i].x = double(approx[i].x);
                            newSquare[i].y = double(approx[i].y);
                        }
                        newSquare.Recenter();
                        squares.push_back(newSquare);
                    }
                }
            }
        }
    }
    return squares;
}

}   // cbr