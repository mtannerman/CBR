#include "imgproc/read_image.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace cbr
{

cv::Mat read_image(const std::string& fileName)
{
    auto image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
	cv::pyrDown(image, image);
    cv::pyrDown(image, image);
    
    return image;
}

}