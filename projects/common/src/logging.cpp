#include "common/logging.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include "common/archive_directory.h" 
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
		return ArchiveDirectory::GetInstance().Get();
	}

	static LogArchiver& GetInstance()
	{
		static LogArchiver instance;
		return instance;
	}

	void Add(const std::string& logString)
	{
		ofs << logString;
	}
private:
	LogArchiver() 
	{
		const auto path = LogArchivePath();
		// std::cout << "Creating LogArchiver" << std::endl;
		THROW_IF(!CbrCreateDirectory(path), FileOperationFailure, STR("Couldn't create directory: " << path));
		ofs.open(STR(path << "/log.txt"));
		THROW_IF(!ofs.is_open(), FileOperationFailure, "Couldn't open log stream");
	}
	std::ofstream ofs;
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
void Log(const std::string& function, const std::string& message)
{
	const auto full = STR("[" << function << "]  " << message << std::endl);
	std::cout << full;
	LogArchiver::GetInstance().Add(full);
}


}   // common_detail
}   // cbr
