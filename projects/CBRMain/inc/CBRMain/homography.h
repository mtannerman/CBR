#pragma once
#include <array>
#include "opencv2/core.hpp"

namespace cbr {

    struct HomographySetting
    {
        HomographySetting() = default;
        HomographySetting(const int numberOfXPoints, const int numberOfYPoints);

        int numberOfXPoints = 100;
        int numberOfYPoints = 100;
        int destSquareEdgeSize = 100;
    }

    class Homography
    {
    public:
        Homography(const cv::Mat& image, const std::array<cv::Point, 4>& sourceSquare, const HomographySetting setting = HomographySetting());
        cv::Mat Apply() const;

    private:
        const cv::Mat& image;
        const std::array<cv::Point, 4>& sourceSquare;
        
        const HomographySetting setting;

    };


}