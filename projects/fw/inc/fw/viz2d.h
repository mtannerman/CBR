#pragma once

#include "opencv2/core.hpp"
#include <string>

namespace cbr
{
namespace viz
{
    
class Visualizer2D
{
public:
    Visualizer2D(const std::string& name);
    void AddImage(const cv::Mat& img);
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
private:
    const std::string mName;
    cv::Mat mImage;
};

}
}