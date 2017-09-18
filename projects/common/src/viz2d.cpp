#include "common/viz2d.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <limits>
#include <utility>
#include "common/exceptions.h"

namespace cbr
{
namespace viz
{
    std::pair<cv::Point, cv::Point> compute_image_span(
        const std::vector<Visualizer2D::Circle>& circles,
        const std::vector<Visualizer2D::Text>& texts)
    {
        ASSERT(!circles.empty() || !texts.empty(), "No widgets given.");
        const auto maxInt = std::numeric_limits<int>::max();
        const auto minInt = std::numeric_limits<int>::lowest();
        cv::Point maxAxis(minInt, minInt);
        cv::Point minAxis(maxInt, maxInt);
        for (const auto& circle : circles) {
            maxAxis.x = std::max(maxAxis.x, circle.center.x + circle.radius);
            maxAxis.y = std::max(maxAxis.y, circle.center.y + circle.radius);
            minAxis.x = std::min(minAxis.x, circle.center.x - circle.radius);
            minAxis.y = std::min(minAxis.y, circle.center.y - circle.radius);
        }

        for (const auto& text : texts) {
            maxAxis.x = std::max(maxAxis.x, text.center.x);
            maxAxis.y = std::max(maxAxis.y, text.center.y);
            minAxis.x = std::min(minAxis.x, text.center.x);
            minAxis.y = std::min(minAxis.y, text.center.y);
        }

        return {minAxis, maxAxis};
    }

    void translate_widgets_to_origin(
        std::vector<Visualizer2D::Circle>& circles,
        std::vector<Visualizer2D::Text>& texts,
        const cv::Point& minAxis)
    {
        for (auto& circle : circles) {
            circle.center -= minAxis;
        }

        for (auto& text : texts) {
            text.center -= minAxis;
        }
    }

    Visualizer2D::Visualizer2D(const std::string& name) : mName(name) {}

    void Visualizer2D::SetAxisMultiplicationFactor(const double mf)
    {
        mAxisMultiplicationFactor = mf;
    }

    void Visualizer2D::SetImageBackgroundColor(const cv::Scalar& bgColor)
    {
        mImageBackgroudColor = bgColor;
    }

    void Visualizer2D::AddCircle(const cv::Point& center,
        const int radius,
        const cv::Scalar color,
        const int thickness,
        const int lineType, 
        const int shift)
    {
        mCircles.push_back(Circle(center, radius, 
            color, thickness, lineType, shift));
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
        mTexts.push_back(Text(text, center, fontFace, 
            fontScale, color, thickness, lineType, 
            bottomLeftOrigin));
    }

    void Visualizer2D::Spin()
    {
        cv::namedWindow(mName, cv::WINDOW_AUTOSIZE);
        const auto imageSpan = compute_image_span(mCircles, mTexts);
        translate_widgets_to_origin(mCircles, mTexts, imageSpan.first);
        cv::Size imageSize(imageSpan.second.x - imageSpan.first.x, imageSpan.second.y - imageSpan.first.y);
        const auto image = cv::Mat(imageSize, CV_8UC3, mImageBackgroudColor);

        for (const auto& circle : mCircles) {
            cv::circle(image, circle.center, 
                circle.radius, circle.color, 
                circle.thickness, circle.lineType, circle.shift);
        }

        for (const auto& text : mTexts) {
            cv::putText(image, text.text, text.center, 
                text.fontFace, text.fontScale, text.color, text.thickness, 
                text.lineType, text.bottomLeftOrigin);
        }

        cv::imshow(mName, image);
        cv::waitKey();
    }

    void Visualizer2D::ClearWidgets()
    {
        mCircles.clear();
        mTexts.clear();
    }

}
}