#pragma once

#include <string>

#define STR(expression) (((::std::ostringstream*)&(::std::ostringstream() << "" << expression))->str())
#define LOG(msg) ::cbr::Log(__FUNCTION__, STR(msg)); 

#define DESC(x) #x << ": " << x 

namespace cbr
{
	void Log(const std::string& function, const std::string& message);

}