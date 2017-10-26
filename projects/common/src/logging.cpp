#include "common/logging.h"

#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>

namespace cbr
{
namespace common_detail
{

void Log(const std::string & function, const std::string & message)
{
	std::cout << "[" << function << "]  " << message << std::endl;
}


}   // common_detail
}   // cbr
