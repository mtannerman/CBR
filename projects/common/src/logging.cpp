#include "common/logging.h"

#include <iostream>
#include <sstream>
#include <time.h>
#include <fstream>

namespace cbr
{
namespace common_detail
{

//std::string currentDateTime() {
//	time_t     now = time(0);
//	struct tm  tstruct;
//	char       buf[80];
//	tstruct = *localtime(&now);
//	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
//
//	return buf;
//}

struct Logger
{
	void Log(const std::string& function, const std::string& message)
	{
		std::cout << "[" << function << "]  " << message << std::endl;
		//fullLog << "[" << function << "]  " << message << std::endl;
	}

	/*std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "%Y-%m-%d--%X", &tstruct);
		for (char& s : buf) {
			s = (s == ':') ? '-' : s;
		}

		return buf;
	}*/

	//std::string LogFileName()
	//{
	//	return "..\\..\\..\\process\\logs\\" + currentDateTime() + ".txt";
	//}

	~Logger()
	{
		//std::ofstream s(LogFileName());
		//if (s.is_open()) {
		//	s << fullLog.str();
		//}
		//else {
		//	std::cout << LogFileName() << std::endl;
		//	throw std::invalid_argument("file doesnt exist");
		//}
	}

	//std::stringstream fullLog;
};

void Log(const std::string & function, const std::string & message)
{
	static Logger logger;
	logger.Log(function, message);
}


}   // common_detail
}   // cbr
