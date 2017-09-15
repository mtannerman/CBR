#include "common/viz2d.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

namespace cbr
{
namespace viz
{
    Visualizer2D::Visualizer2D(const std::string& name) : mName(name) {}

    void Visualizer2D::AddImage(const cv::Mat& img)
    {
        mImage = img.clone();
    }

    void Visualizer2D::AddCircle(const cv::Point& center,
        const int radius,
        const cv::Scalar color,
        const int thickness,
        const int lineType, 
        const int shift)
    {
        cv::circle(mImage, center, radius, color, thickness, lineType, shift);
    }

    void Visualizer2D::AddText(
        const std::string& text,
        const cv::Point& center,
        const int fontFace,
        const double fontScale,
        const cv::Scalar color,
        const int thickness,
        const int lineType, 
        const bool bottomLeftOrigin)
    {
        cv::putText(mImage, text, center, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin);
    }

    void Visualizer2D::Spin()
    {
        cv::namedWindow(mName, cv::WINDOW_AUTOSIZE);
        cv::imshow(mName, mImage);
        cv::waitKey();
    }

    void Visualizer2D::ClearWidgets()
    {
        mImage = cv::Mat();
    }

}
}