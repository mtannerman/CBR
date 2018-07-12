#include "imgproc/read_image.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/core/core.hpp>

#include "common/file_operation.h"
#include "common/exceptions.h"

namespace cbr
{

cv::Mat read_image(const std::string& fileName)
{
	THROW_IF(!IsDirectoryOrFileExist(fileName), FileOperationFailure, "");

    auto image = cv::imread(fileName, cv::IMREAD_COLOR);
	cv::pyrDown(image, image);
    cv::pyrDown(image, image);
    
    return image;
}

}