#include "imgproc/find_raw_squares.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "common/viz2d.h"
#include "math_utils/line.h"
#include "math_utils/geometry.h"
#include "common/config.h"
#include "common/exceptions.h"

namespace cbr
{

bool opposite_corner_angle_differences_are_small(const std::vector<cv::Point>& square)
{
    ASSERT(square.size() == 4, "");

    const double cornerAngles[] = {
        std::acos(angle_cosine_between_three_points(square[3], square[1], square[0])),
        std::acos(angle_cosine_between_three_points(square[0], square[2], square[1])),
        std::acos(angle_cosine_between_three_points(square[1], square[3], square[2])),
        std::acos(angle_cosine_between_three_points(square[2], square[0], square[3]))
    };
    
    const double oppositeCornerAngleDifferences[] = {
        std::abs(cornerAngles[0] - cornerAngles[2]),
        std::abs(cornerAngles[1] - cornerAngles[3])
    };

    constexpr double ANGLES = 3.14 / 180.;
    constexpr double threshold = 10. * ANGLES;

    return oppositeCornerAngleDifferences[0] < threshold && 
        oppositeCornerAngleDifferences[1] < threshold;
}


std::vector<Square> find_squares(const cv::Mat& image)
{
    constexpr int thresh = 200;
    constexpr int N = 10;
    std::vector<Square> squares;
	cv::Mat pyr, timg, gray0(image.size(), CV_8U), gray;
	cv::pyrDown(image, pyr, cv::Size(image.cols/2, image.rows/2));
	cv::pyrUp(pyr, timg, image.size());

	std::vector<std::vector<cv::Point>> contours;
    struct D{ Square s; double maxCosine; };

	for(const int iChannel : {0, 1, 2}) {
        const auto channelFromToArray = std::array<int, 2>{iChannel, 0};
        constexpr int numberOfMatricesInImages = 1;
        cv::mixChannels(&timg, numberOfMatricesInImages, &gray0, numberOfMatricesInImages, channelFromToArray.data(), 1);

        for( int l = 0; l < N; l++ ) {
            if( l == 0 ) {
                constexpr double lowerCannyHysteresisThreshold = 0.;
                constexpr double upperCannyHysteresisThreshold = 200.;
                constexpr int cannyApertureSize = 5;
                cv::Canny(gray0, gray, lowerCannyHysteresisThreshold, upperCannyHysteresisThreshold, cannyApertureSize);
                cv::dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
            }
            else {
                gray = gray0 >= double(l * 255) / N;
            }

            cv::findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
            std::vector<cv::Point> approx;

            std::vector<D> debugBatch;

            for ( const auto& contour : contours ) {
                cv::approxPolyDP(cv::Mat(contour), approx, cv::arcLength(cv::Mat(contour), true)*0.02, true);

                if ( approx.size() == 4 &&
                    std::fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
                    cv::isContourConvex(cv::Mat(approx)) ) {
                    double maxCosine = 0;

                    // for ( int j = 2; j < 5; j++ ) {
                    //     // find the maximum cosine of the angle between joint edges
                        
                    //     const double cosine = std::abs(angle_cosine_between_three_points(approx[j%4], approx[j-2], approx[j-1]));
                    //     maxCosine = std::max(maxCosine, cosine);
                    // }

                    for (int iCorner = 1; iCorner < 4; ++iCorner) {
                        const int nextIndex = iCorner < 3 ? (iCorner + 1) : 0; 
                        // find the maximum cosine of the angle between joint edges
                        
                        const double cosine = std::abs(angle_cosine_between_three_points(approx[nextIndex], approx[iCorner - 1], approx[iCorner]));
                        maxCosine = std::max(maxCosine, cosine);
                    }


                    if (maxCosine < 0.3 && opposite_corner_angle_differences_are_small(approx)) {
                        Square newSquare;
                        for (size_t i = 0; i < 4; ++i) {
                            newSquare[i].x = double(approx[i].x);
                            newSquare[i].y = double(approx[i].y);
                        }
                        newSquare.Recenter();
                        squares.push_back(newSquare);
                        debugBatch.push_back({newSquare, maxCosine});
                    }
                }
            }

            if (Config::GetInstance().GetBool("visualizeRawSquares")) {

                auto grayCpy = gray.clone();
                cv::cvtColor(grayCpy, grayCpy, cv::COLOR_GRAY2BGR);
                cv::putText(grayCpy, STR(DESC(iChannel) << "  " << DESC(debugBatch.size())), cv::Point(30, 30), 1, 2., cv::Scalar(0., 0., 255.), 4);
                for (const auto& d : debugBatch) {
                    const auto& s = d.s;
                    for (int iCorner = 0; iCorner < 4; ++iCorner) {
                        const int nextIndex = iCorner < 3 ? (iCorner + 1) : 0;
                        cv::line(grayCpy, cv::Point(s[iCorner].x, s[iCorner].y), cv::Point(s[nextIndex].x, s[nextIndex].y), cv::Scalar(0.,0.,255.));
                        // cv::putText(grayCpy, STR(int(100. * d.maxCosine)), cv::Point(s.middle.x, s.middle.y), 0, 0.3, cv::Scalar(0.,0.,255.));
                    }

                    auto to_point = [](const Square& s, const size_t idx) { 
                            const auto& p1 = s[idx];
                            const auto& p2 = s.middle;
                            auto newPoint = (p1 * 2. + p2) / 3.0;
                            return cv::Point(newPoint.x, newPoint.y); 
                        };

                    cv::putText(grayCpy, STR(int(180.0 / 3.14 * std::acos(angle_cosine_between_three_points(s[0], s[2], s[1])))), to_point(s, 1), 0, 0.5, cv::Scalar(0.,0.,255.));
                    cv::putText(grayCpy, STR(int(180.0 / 3.14 * std::acos(angle_cosine_between_three_points(s[1], s[3], s[2])))), to_point(s, 2), 0, 0.5, cv::Scalar(0.,0.,255.));
                    cv::putText(grayCpy, STR(int(180.0 / 3.14 * std::acos(angle_cosine_between_three_points(s[2], s[0], s[3])))), to_point(s, 3), 0, 0.5, cv::Scalar(0.,0.,255.));
                    cv::putText(grayCpy, STR(int(180.0 / 3.14 * std::acos(angle_cosine_between_three_points(s[3], s[1], s[0])))), to_point(s, 0), 0, 0.5, cv::Scalar(0.,0.,255.));
                }

                cv::namedWindow("dsad", cv::WINDOW_AUTOSIZE);
                cv::imshow("dsad", grayCpy);
                cv::waitKey();
            }
        }
    }
    return squares;
}

}   // cbr
