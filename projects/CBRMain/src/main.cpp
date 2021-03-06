#include "imgproc/imgproc.h"
#include "common/config.h"
#include "imgproc/read_image.h"
#include "common/logging.h"
#include "common/playground/json_playground.h"
#include "cbr_test/test.h"
#include "common/file_operation.h"


int main(int argc, char *argv[])
{
	const std::string configFilePath = argv[1];
	::cbr::Config::GetInstance().ParseFile(configFilePath);

	::cbr::test::RunTests();

	const auto images = ::cbr::Config::GetInstance().GetStringList("images");
	const std::string fileName = images[0];
	auto image = cbr::read_image(fileName);
	cbr::preprocess_image(image);

	LOG("Finished successfully");

	return 0;
}
