#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../inc/CBRMain/find_black_squares.h"
#include "../inc/CBRMain/board_rotation_experiment.h"

int main()
{
	const std::string fileName = "/home/timarmate/Projects/CBR/input/3d/octagon/0.jpg";
	auto image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
	cv::pyrDown(image, image);
	cv::pyrDown(image, image);
	//cv::namedWindow("DisplayWindow");
	//cv::imshow("DisplayWindow", image);

	cbr::find_board(image);
	//cbr::board_rotation_experiment();

	cv::waitKey(0);
	return 0;
}
