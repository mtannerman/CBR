#pragma once
#include <vector>
#include "opencv2/core.hpp"

namespace cbr {

    struct HomographySetting
    {
        HomographySetting() = default;
        HomographySetting(const int numberOfXPoints, const int numberOfYPoints);

        int numberOfXPoints = 100;
        int numberOfYPoints = 100;
    };

    class Homography
    {
    public:
        Homography(const cv::Mat& image, const std::vector<cv::Point>& sourceSquare, const HomographySetting setting = HomographySetting());
        cv::Mat Apply() const;

    private:
        const cv::Mat& image;
        const std::vector<cv::Point>& sourceSquare;
        
        const HomographySetting setting;

    };


}