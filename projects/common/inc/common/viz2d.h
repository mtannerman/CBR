#pragma once

#include "opencv2/core.hpp"
#include <string>
#include "common/pimpl.h"

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
    ~Visualizer2D();
    Visualizer2D(const std::string& name);
    void SetAxisMultiplicationFactor(const double mf);
    void SetImageBackgroundColor(const cv::Scalar& bgColor);
    void SetDiagonalLength(const int diagonalLength);
    void SetMirroring(const bool applyMirroring);

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

    void AddLine(const cv::Point& pt1,
        const cv::Point& pt2,
        const cv::Scalar color = cv::Scalar(255, 0, 0),
        const int thickness = 1,
        const int lineType = 8,
        const int shift = 0);

    void AddArrow(const cv::Point& pt1,
        const cv::Point& pt2,
        const cv::Scalar color = cv::Scalar(255, 0, 0),
        const int thickness = 1,
        const int lineType = 8,
        const int shift = 0,
        const double tipLength = 0.1);
                 
    cv::Mat CreateImage();

    void Spin();
    void ClearWidgets();
private:
    struct Impl;
    impl_ptr<Impl> mImpl;
};

}
}