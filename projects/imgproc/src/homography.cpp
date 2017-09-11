#include "imgproc/homography.h"


namespace cbr
{
    HomographySetting::HomographySetting(const int numberOfXPoints, const int numberOfYPoints)
        : numberOfXPoints(numberOfXPoints)
        , numberOfYPoints(numberOfYPoints)
        {}

    Homography::Homography(const cv::Mat& image, 
        const std::vector<cv::Point>& sourceSquare,
        const HomographySetting setting/*  = HomographySetting() */)
        : image(image)
        , sourceSquare(sourceSquare)
        , setting(setting)
        {
        }


    float convex_combination(const float value1, const float value2, const float lambda)
    {
        return lambda * value1 + (1.0f - lambda) * value2;
    }

    cv::Point2f convex_combination(const cv::Point2f& p1, const cv::Point2f& p2, const float lambda)
    {
        return cv::Point2f(
            convex_combination(p1.x, p2.x, lambda),
            convex_combination(p1.y, p2.y, lambda)
        );
    }
    
    cv::Mat Homography::Apply() const
    {
        cv::Mat ret(setting.numberOfXPoints, setting.numberOfYPoints, 0);
        const auto fpSquares = std::array<cv::Point2f, 4>{ cv::Point2f(sourceSquare[0]), cv::Point2f(sourceSquare[1]), cv::Point2f(sourceSquare[2]), cv::Point2f(sourceSquare[3]) };
        for (int iy = 0; iy < setting.numberOfYPoints; ++iy) {
            const float lambdaY = float(iy) / float(setting.numberOfYPoints);
            const auto leftPoint = convex_combination(fpSquares[0], fpSquares[3], lambdaY);
            const auto rightPoint = convex_combination(fpSquares[1], fpSquares[2], lambdaY);
            for (int ix = 0; ix < setting.numberOfXPoints; ++ix) {
                const float lambdaX = float(ix) / float(setting.numberOfYPoints);
                const auto fpCenterPoint = convex_combination(leftPoint, rightPoint, lambdaX);
                const auto centerPoint = cv::Point(int(fpCenterPoint.x), int(fpCenterPoint.y));
                ret.at<uchar>(ix, iy) = image.at<uchar>();
            }
        }

        return ret;
    }

}