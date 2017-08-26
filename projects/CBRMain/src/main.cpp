#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat fourier_transformed_image(const cv::Mat& input) 
{
	cv::Mat padded;
	const int m = cv::getOptimalDFTSize(input.rows);
	const int n = cv::getOptimalDFTSize(input.cols);
	cv::copyMakeBorder(input, padded, 0, m - input.rows, 0, n - input.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
	cv::Mat complexI;
	cv::merge(planes, 2, complexI);
	cv::dft(complexI, complexI);
	cv::split(complexI, planes);
	cv::magnitude(planes[0], planes[1], planes[0]);
	cv::Mat magI = planes[0];
	magI += cv::Scalar::all(1);
	cv::log(magI, magI);

	magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
	int cx = magI.cols/2;
    int cy = magI.rows/2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy)); 
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy));

    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);  
    q2.copyTo(q1);
    tmp.copyTo(q2);

    cv::normalize(magI, magI, 0, 1, CV_MINMAX);

    return magI;
}


int main()
{
	const std::string fileName = "/home/timarmate/Projects/CBR/input/3d/octagon/0.jpg";

	auto image = cv::imread(fileName, 0);
	cv::pyrDown(image, image);
	cv::pyrDown(image, image);
	cv::namedWindow("DisplayWindow");
	cv::imshow("DisplayWindow", image);

	//const auto ftImage = fourier_transformed_image(image);
	//cv::namedWindow("ft");
	//cv::imshow("ft", ftImage);

	cv::waitKey(0);
	return 0;
}
