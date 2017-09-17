#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "imgproc/imgproc.h"
#include "imgproc/board_rotation_experiment.h"
#include "imgproc/homography.h"
#include "imgproc/apply_corner_cluster_filtering.h"
#include "imgproc/find_square_contours.h"
#include "imgproc/test/square_filtering_test.h"
#include "common/config.h"

int main()
{
	CBR_INIT_CONFIG("/home/timarmate/Projects/CBR/config/config.json");

	const std::string fileName = "/home/timarmate/Projects/CBR/input/3d/octagon/0.jpg";
	auto image = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
	cv::pyrDown(image, image);
	cv::pyrDown(image, image);
	//cv::namedWindow("DisplayWindow");
	//cv::imshow("DisplayWindow", image);

	cv::Mat grayScaleImage;
	cv::cvtColor(image, grayScaleImage, CV_RGB2GRAY);

	const auto squares = cbr::find_squares(image);
	const auto whatever = cbr::apply_cluster_filtering(squares);
	// const auto homographyOutput = cbr::Homography(image, squares[11]).Apply();

	// cv::namedWindow("DisplayWindow");
	// cv::imshow("DisplayWindow", image);

	// cv::namedWindow("DisplayWindow2");
	// cv::imshow("DisplayWindow2", homographyOutput);

	// cbr::find_board(image);
	//cbr::board_rotation_experiment();

	cv::waitKey(0);
	return 0;
}
