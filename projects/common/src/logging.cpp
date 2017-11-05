#include "common/logging.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include "common/time.h" 
#include <memory>
#include "common/file_operation.h"
#include "common/exceptions.h"

namespace cbr
{

class LogArchiver
{
public:
	static std::string LogArchivePath()
	{
		return STR("../process/" << GetDateTimeStr());
	}

	static LogArchiver& GetInstance()
	{
		static LogArchiver instance;
		return instance;
	}

	void Add(const std::string& logString)
	{
		ofs->operator<<(logString.c_str());
	}
private:
	LogArchiver() 
	{
		const auto path = LogArchivePath();
		ASSERT(CreateDirectory(path), STR("Couldn't create directory: " << path));
		ofs.reset(new std::ofstream(STR(path << "/log.txt"), std::ofstream::out | std::ofstream::app));
	}
	std::unique_ptr<std::ofstream> ofs;
};


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
	LogArchiver::GetInstance().Add(STR("[" << function << "]  " << message << std::endl));
}


}   // common_detail
}   // cbr
