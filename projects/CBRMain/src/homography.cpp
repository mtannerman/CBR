#include "../inc/CBRMain/homography.h"


namespace cbr
{
    HomographySetting::HomographySetting(const int numberOfXPoints, const int numberOfYPoints)
        : numberOfXPoints(numberOfXPoints)
        , numberOfYPoints(numberOfYPoints)
        {}

    Homography::Homography(const cv::Mat& image, 
        const std::array<cv::Point, 4>& sourceSquare)
        : image(image)
        , sourceSquare(sourceSquare)
        , setting(setting)
        {}

    cv::Mat Homography::Apply() const
    {
        cv::Mat ret;
        cv::Point2f fpSquares { cv::Point2f(sourceSquare[0]), cv::Point2f(sourceSquare[1]), cv::Point2f(sourceSquare[2]), cv::Point2f(sourceSquare[3]) };
        for (int iy = 0; iy < setting.numberOfYPoints; ++iy) {
            const float lambdaY = float(iy) / float(setting.numberOfYPoints);
            for (int ix = 0; ix < setting.numberOfXPoints; ++ix) {
                const float lambdaX = float(ix) / float(setting.numberOfYPoints);
                
            }
        }

        return ret;
    }

}