#include "common/logging.h"

#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>

namespace cbr
{

std::string StripPrettyFunction(std::string prettyFunction)
{
	const size_t spaceAfterTypeNamePosition = prettyFunction.find(' ');
	prettyFunction = prettyFunction.substr(spaceAfterTypeNamePosition + 1);
	std::stringstream ss;
	bool inBracketRegion = false;
	char prevChar = ' ';
	for (char c : prettyFunction) {
		if (c == '(') {
			inBracketRegion = true;
		}
		else if (prevChar == ')') {
			inBracketRegion = false;
		}

		if (!inBracketRegion) {
			ss << c;
		}

		prevChar = c;
	}
	
	return ss.str();
}

namespace common_detail
{
void Log(const std::string & function, const std::string & message)
{
	std::cout << "[" << function << "]  " << message << std::endl;
}


}   // common_detail
}   // cbr
