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
    static Color red()      { return  Color(0, 0, 255); }
    static Color blue()     { return  Color(255, 0, 0); }
    static Color green()    { return  Color(0, 255, 0); }
    static Color white()    { return  Color(255, 255, 255); }
    static Color black()    { return  Color(0, 0, 0); }
    static Color cyan()     { return  Color(255, 255, 0); }
    static Color yellow()   { return  Color(0, 255, 255); }
    static Color magenta()  { return  Color(255, 0, 255); }

    double mBlue;
    double mGreen;
    double mRed;
    Color() = default;
    Color(const double blue, const double green, const double red)
        : mBlue(blue), mGreen(green), mRed(red)
    { }
};

class Visualizer2D
{
public:
    ~Visualizer2D();
    Visualizer2D(const std::string& name);
    void SetAxisMultiplicationFactor(const double mf);
    void SetImageBackgroundColor(const Color& bgColor);
    void SetDiagonalLength(const int diagonalLength);
    void SetMirroring(const bool applyMirroring);

    void AddCircle(const double centerX,
                   const double centerY,
                   const int radius,
                   const Color color = Color(255, 0, 0),
                   const int thickness = 1,
                   const int lineType = 8, 
                   const int shift = 0);

    void AddText(const std::string& text,
                 const double centerX,
                 const double centerY,
                 const int fontFace = 0,
                 const double fontScale = 10.0,
                 const Color color = Color(255, 0, 0),
                 const int thickness = 1,
                 const int lineType = 8, 
                 const bool bottomLeftOrigin = false);

    void AddLine(const double x1,
                 const double y1,
                 const double x2,
                 const double y2,
                 const Color color = Color(255, 0, 0),
                 const int thickness = 1,
                 const int lineType = 8,
                 const int shift = 0);

    void AddArrow(const double x1,
        const double y1,
        const double x2,
        const double y2,
        const Color color = Color(255, 0, 0),
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