#include "common/viz2d.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <limits>
#include <utility>
#include "common/exceptions.h"
#include "common/file_operation.h"
#include <map>
#include "common/counter.h"
#include "common/archive_directory.h" 

namespace cbr
{
namespace viz
{

class ImageArchiver
{
public:
    static ImageArchiver& GetInstance()
	{
		static ImageArchiver instance;
		return instance;
	}

	void Archive(const std::string& imageName, const cv::Mat& image)
	{
        mImageIndices[imageName] += 1;
        const auto fileName = STR(mDirectoryName << "/" << imageName << '-' << (mImageIndices[imageName].Get() - 1) << ".png");
        ASSERT(cv::imwrite(fileName, image), STR("couldnt write image to " << fileName));

	}
private:
	ImageArchiver() 
	{
		mDirectoryName = ArchivePath();
		ASSERT(CreateDirectory(mDirectoryName), STR("Couldn't create directory: " << mDirectoryName));
    }
    
    static std::string ArchivePath()
	{
		return ArchiveDirectory::GetInstance().Get("images");
    }
    
    std::map<std::string, Counter> mImageIndices; 
    std::string mDirectoryName;
};


static cv::Scalar ToCvScalar(const Color& c) 
{
    return cv::Scalar(c.mBlue, c.mGreen, c.mRed); 
}

struct Visualizer2D::Impl
{

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

    struct Line
    {
        Line() = default;
        Line(const cv::Point& pt1,
            const cv::Point& pt2,
            const cv::Scalar color,
            const int thickness,
            const int lineType,
            const int shift)
        : pt1(pt1),
          pt2(pt2),
          color(color),
          thickness(thickness),
          lineType(lineType),
          shift(shift)
        {}

        cv::Point pt1;
        cv::Point pt2;
        cv::Scalar color;
        int thickness;
        int lineType;
        int shift;

    };
    struct Arrow
    {
        Arrow() = default;
        Arrow(const cv::Point& pt1,
            const cv::Point& pt2,
            const cv::Scalar color,
            const int thickness,
            const int lineType,
            const int shift,
            const double tipLength)
        : pt1(pt1),
          pt2(pt2),
          color(color),
          thickness(thickness),
          lineType(lineType),
          shift(shift),
          tipLength(tipLength)
        {}

        cv::Point pt1;
        cv::Point pt2;
        cv::Scalar color;
        int thickness;
        int lineType;
        int shift;
        double tipLength;

    };
    std::pair<cv::Point, cv::Point> compute_image_span();
    void translate_widgets_to_origin(const cv::Point& minAxis);
    void mirror_widgets(const cv::Point& minAxis, const cv::Point& maxAxis);
    double compute_dilation_factor(const cv::Point& minAxis, const cv::Point& maxAxis) const;
    void resize_widgets(const double dilationFactor);
    cv::Size compute_image_size(const cv::Point& minAxis, const cv::Point& maxAxis,
        const double dilationFactor) const;
    void Spin();
    cv::Mat CreateImage();
    bool mApplyMirroring = true;
    cv::Scalar mImageBackgroudColor = ToCvScalar(Color::black());
    double mAxisMultiplicationFactor = 1.3;
    int mDiagonalLength = 1000;
    std::vector<Circle> mCircles;
    std::vector<Text> mTexts;
    std::vector<Line> mLines;
    std::vector<Arrow> mArrows;
    std::string mName;
};

std::pair<cv::Point, cv::Point> Visualizer2D::Impl::compute_image_span()
{
    ASSERT(!mLines.empty() || !mCircles.empty() || mTexts.empty() || mArrows.empty(), "No widgets given.");
    const auto maxInt = std::numeric_limits<int>::max();
    const auto minInt = std::numeric_limits<int>::lowest();
    cv::Point maxAxis(minInt, minInt);
    cv::Point minAxis(maxInt, maxInt);
    for (const auto& circle : mCircles) {
        maxAxis.x = std::max(maxAxis.x, circle.center.x + circle.radius);
        maxAxis.y = std::max(maxAxis.y, circle.center.y + circle.radius);
        minAxis.x = std::min(minAxis.x, circle.center.x - circle.radius);
        minAxis.y = std::min(minAxis.y, circle.center.y - circle.radius);
    }
    for (const auto& text : mTexts) {
        maxAxis.x = std::max(maxAxis.x, text.center.x);
        maxAxis.y = std::max(maxAxis.y, text.center.y);
        minAxis.x = std::min(minAxis.x, text.center.x);
        minAxis.y = std::min(minAxis.y, text.center.y);
    }
    for (const auto& line : mLines) {
        maxAxis.x = std::max(maxAxis.x, line.pt1.x);
        maxAxis.y = std::max(maxAxis.y, line.pt1.y);
        minAxis.x = std::min(minAxis.x, line.pt1.x);
        minAxis.y = std::min(minAxis.y, line.pt1.y);
        maxAxis.x = std::max(maxAxis.x, line.pt2.x);
        maxAxis.y = std::max(maxAxis.y, line.pt2.y);
        minAxis.x = std::min(minAxis.x, line.pt2.x);
        minAxis.y = std::min(minAxis.y, line.pt2.y);
    }
    for (const auto& arrow : mArrows) {
        maxAxis.x = std::max(maxAxis.x, arrow.pt1.x);
        maxAxis.y = std::max(maxAxis.y, arrow.pt1.y);
        minAxis.x = std::min(minAxis.x, arrow.pt1.x);
        minAxis.y = std::min(minAxis.y, arrow.pt1.y);
        maxAxis.x = std::max(maxAxis.x, arrow.pt2.x);
        maxAxis.y = std::max(maxAxis.y, arrow.pt2.y);
        minAxis.x = std::min(minAxis.x, arrow.pt2.x);
        minAxis.y = std::min(minAxis.y, arrow.pt2.y);
    }
    return {minAxis, maxAxis};
}

void Visualizer2D::Impl::translate_widgets_to_origin(const cv::Point& minAxis)
{
    for (auto& circle : mCircles) {
        circle.center -= minAxis;
    }
    for (auto& text : mTexts) {
        text.center -= minAxis;
    }
    for (auto& line : mLines) {
        line.pt1 -= minAxis;
        line.pt2 -= minAxis;
    }
    for (auto& arrow : mArrows) {
        arrow.pt1 -= minAxis;
        arrow.pt2 -= minAxis;
    }
}

void Visualizer2D::Impl::mirror_widgets(
    const cv::Point& minAxis,
    const cv::Point& maxAxis)
{
    const auto yAxisDiff = maxAxis.y - minAxis.y;
    for (auto& circle : mCircles) {
        circle.center.y = yAxisDiff - circle.center.y;
    }
    for (auto& text : mTexts) {
        text.center.y = yAxisDiff - text.center.y;
    }
    for (auto& line : mLines) {
        line.pt1.y = yAxisDiff - line.pt1.y;
        line.pt2.y = yAxisDiff - line.pt2.y;
    }
    for (auto& arrow : mArrows) {
        arrow.pt1.y = yAxisDiff - arrow.pt1.y;
        arrow.pt2.y = yAxisDiff - arrow.pt2.y;
    }
}

double Visualizer2D::Impl::compute_dilation_factor(const cv::Point& minAxis,
    const cv::Point& maxAxis) const
{
    const auto fullImageSpan = maxAxis - minAxis;
    const double imageDiagonalLength = cv::norm(fullImageSpan);
    return double(mDiagonalLength) / imageDiagonalLength;
}

void Visualizer2D::Impl::resize_widgets(const double dilationFactor)
{
    for (auto& circle : mCircles) {
        circle.center = cv::Point(cv::Point2d(circle.center) * dilationFactor);
        circle.radius = int(double(circle.radius) * dilationFactor);
    }
    for (auto& text : mTexts) {
        text.center = cv::Point(cv::Point2d(text.center) * dilationFactor);
        text.fontScale *= dilationFactor;
    }
    for (auto& line : mLines) {
        line.pt1 = cv::Point(cv::Point2d(line.pt1) * dilationFactor);
        line.pt2 = cv::Point(cv::Point2d(line.pt2) * dilationFactor);
    }
    for (auto& arrow : mArrows) {
        arrow.pt1 = cv::Point(cv::Point2d(arrow.pt1) * dilationFactor);
        arrow.pt2 = cv::Point(cv::Point2d(arrow.pt2) * dilationFactor);
    }
}

cv::Size Visualizer2D::Impl::compute_image_size(
    const cv::Point& minAxis,
    const cv::Point& maxAxis,
    const double dilationFactor) const
{
    const auto scaledAxisDiff = cv::Point(dilationFactor * cv::Point2d(maxAxis - minAxis));
    return cv::Size(scaledAxisDiff.x, scaledAxisDiff.y);
}

Visualizer2D::~Visualizer2D() {}

Visualizer2D::Visualizer2D(const std::string& name) : mImpl(new Impl())
{
    mImpl->mName = name;
}

void Visualizer2D::SetAxisMultiplicationFactor(const double mf)
{
    mImpl->mAxisMultiplicationFactor = mf;
}

void Visualizer2D::SetImageBackgroundColor(const Color& bgColor)
{
    mImpl->mImageBackgroudColor = ToCvScalar(bgColor);
}

void Visualizer2D::SetDiagonalLength(const int diagonalLength)
{
    mImpl->mDiagonalLength = diagonalLength;
}

void Visualizer2D::SetMirroring(const bool applyMirroring)
{
    mImpl->mApplyMirroring = applyMirroring;
}

void Visualizer2D::AddCircle(const double centerX,
    const double centerY,
    const int radius,
    const Color color,
    const int thickness,
    const int lineType, 
    const int shift)
{
    mImpl->mCircles.push_back(Impl::Circle(cv::Point(int(centerX), int(centerY)), radius, 
        ToCvScalar(color), thickness, lineType, shift));
}

void Visualizer2D::AddText(
    const std::string& text,
    const double centerX,
    const double centerY,
    const int fontFace,
    const double fontScale,
    const Color color,
    const int thickness,
    const int lineType, 
    const bool bottomLeftOrigin)
{
    mImpl->mTexts.push_back(Impl::Text(text, cv::Point(int(centerX), int(centerY)), 
    fontFace, fontScale, ToCvScalar(color), thickness, lineType, 
        bottomLeftOrigin));
}

void Visualizer2D::AddLine(const double x1,
    const double y1,
    const double x2,
    const double y2,
    const Color color,
    const int thickness,
    const int lineType,
    const int shift)
{
    mImpl->mLines.push_back(Impl::Line(
        cv::Point(int(x1), int(y1)), 
        cv::Point(int(x2), int(y2)), 
        ToCvScalar(color), thickness,
        lineType, shift));
}

void Visualizer2D::AddArrow(const double x1,
    const double y1,
    const double x2,
    const double y2,
    const Color color,
    const int thickness,
    const int lineType,
    const int shift,
    const double tipLength)
{
    mImpl->mArrows.push_back(Impl::Arrow(
        cv::Point(int(x1), int(y1)), 
        cv::Point(int(x2), int(y2)),  
        ToCvScalar(color), thickness, lineType, shift, tipLength));
}

void Visualizer2D::Spin()
{
    mImpl->Spin();
}

void Visualizer2D::Impl::Spin()
{
    cv::namedWindow(mName, cv::WINDOW_AUTOSIZE);
    const auto image = CreateImage();
    cv::imshow(mName, image);
    cv::waitKey();
}

void Visualizer2D::ClearWidgets()
{
    mImpl->mCircles.clear();
    mImpl->mTexts.clear();
    mImpl->mLines.clear();
    mImpl->mArrows.clear();
}

cv::Mat Visualizer2D::Impl::CreateImage()
{
    const auto imageSpan = compute_image_span();
    translate_widgets_to_origin(imageSpan.first);
    if (mApplyMirroring) {
        mirror_widgets(imageSpan.first, imageSpan.second);
    }
    const auto dilationFactor = compute_dilation_factor(imageSpan.first, imageSpan.second);
    resize_widgets(dilationFactor);
    const auto imageSize = compute_image_size(imageSpan.first, imageSpan.second, dilationFactor);
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
    for (const auto& line : mLines) {
        cv::line(image, line.pt1, line.pt2, 
            line.color, line.thickness, 
            line.lineType, line.shift);
    }
    for (const auto& arrow : mArrows) {
        cv::arrowedLine(image, arrow.pt1, arrow.pt2, 
            arrow.color, arrow.thickness, 
            arrow.lineType, arrow.shift, arrow.tipLength);
    }

    ImageArchiver::GetInstance().Archive(mName, image);
    return image;
}

cv::Mat Visualizer2D::CreateImage()
{
    return mImpl->CreateImage();
}

}
}