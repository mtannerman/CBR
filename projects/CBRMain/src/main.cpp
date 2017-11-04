#include "imgproc/imgproc.h"
#include "common/config.h"
#include "imgproc/read_image.h"
#include "common/logging.h"
#include "math_utils/tests/simplex_optimizer_unittest.h"
#include "common/playground/json_playground.h"

int main()
{
	// cbr::playground::StartJsonPlayground();
	const std::string configFilePath = "/home/timarmate/Projects/CBR/config/config.json";
    ::cbr::Config::GetInstance().ParseFile(configFilePath);
	const std::string fileName = "/home/timarmate/Projects/CBR/input/3d/octagon/0.jpg";
	auto image = cbr::read_image(fileName);
	cbr::find_board(image);

	// cbr::run_simplex_optimizer_unittest();
	LOG("Finished successfully");

	return 0;
}
