#include "imgproc/imgproc.h"
#include "common/config.h"
#include "imgproc/read_image.h"

int main()
{
	CBR_INIT_CONFIG("/home/timarmate/Projects/CBR/config/config.json");

	const std::string fileName = "/home/timarmate/Projects/CBR/input/3d/octagon/0.jpg";
	auto image = cbr::read_image(fileName);
	cbr::find_board(image);
	// const auto squares = cbr::find_squares(image);
	// const auto whatever = cbr::apply_cluster_filtering(squares);
	
	return 0;
}
