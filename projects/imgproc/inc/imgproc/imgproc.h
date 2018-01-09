#pragma once

#include <vector>
namespace cv { class Mat; }

namespace cbr
{
    cv::Mat preprocess_image(const cv::Mat& image);
}

