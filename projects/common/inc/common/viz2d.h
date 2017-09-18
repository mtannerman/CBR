#pragma once

#include "opencv2/core.hpp"
#include <string>

namespace cbr
{
namespace viz
{

struct Color
{
    static cv::Scalar red() { return cv::Scalar(0, 0, 255); }
    static cv::Scalar blue() { return cv::Scalar(255, 0, 0); }
    static cv::Scalar green() { return cv::Scalar(0, 255, 0); }
    static cv::Scalar white() { return cv::Scalar(255, 255, 255); }
    static cv::Scalar black() { return cv::Scalar(0, 0, 0); }
    static cv::Scalar cyan() { return cv::Scalar(255, 255, 0); }
    static cv::Scalar yellow() { return cv::Scalar(0, 255, 255); }
    static cv::Scalar magenta() { return cv::Scalar(255, 0, 255); }

private:
    Color() = default;
};

class Visualizer2D
{
public:
    Visualizer2D(const std::string& name);
    void SetAxisMultiplicationFactor(const double mf);
    void SetImageBackgroundColor(const cv::Scalar& bgColor);

    void AddCircle(const cv::Point& center,
                   const int radius,
                   const cv::Scalar color = cv::Scalar(255, 0, 0),
                   const int thickness = 1,
                   const int lineType = 8, 
                   const int shift = 0);

    void AddText(const std::string& text,
                 const cv::Point& center,
                 const int fontFace = 0,
                 const double fontScale = 10.0,
                 const cv::Scalar color = cv::Scalar(255, 0, 0),
                 const int thickness = 1,
                 const int lineType = 8, 
                 const bool bottomLeftOrigin = false);
                 
    void Spin();
    void ClearWidgets();
public:
    struct Circle
    {
        Circle() = default;
        Circle(const cv::Point& center,
            const int radius,
            const cv::Scalar& color,
            const int thickness,
            const int lineType,
            const int shift) 
        : center(center)
        , radius(radius)
        , color(color)
        , thickness(thickness)
        , lineType(lineType)
        , shift(shift) {}

        cv::Point center;
        int radius;
        cv::Scalar color;
        int thickness;
        int lineType;
        int shift;
    };

    struct Text
    {
        Text() = default;
        Text(const std::string& text,
            const cv::Point& center,
            const int fontFace,
            const double fontScale,
            const cv::Scalar& color,
            const int thickness,
            const int lineType,
            const bool bottomLeftOrigin)
        : text(text)
        , center(center)
        , fontFace(fontFace)
        , fontScale(fontScale)
        , color(color)
        , thickness(thickness)
        , lineType(lineType)
        , bottomLeftOrigin(bottomLeftOrigin) {}

        std::string text;
        cv::Point center;
        int fontFace;
        double fontScale;
        cv::Scalar color;
        int thickness;
        int lineType;
        bool bottomLeftOrigin;
    };
private:
    cv::Scalar mImageBackgroudColor = Color::black();
    double mAxisMultiplicationFactor = 1.3;
    std::vector<Circle> mCircles;
    std::vector<Text> mTexts;
    const std::string mName;
};

}
}