#pragma once

#include <string>

#define STR(expression) (((::std::ostringstream*)&(::std::ostringstream() << "" << expression))->str())
#define LOG(msg) ::cbr::fw::Log(__FUNCTION__, STR(msg)); 

#define DESC(x) #x << ": " << x 

namespace cbr
{
namespace fw
{
	void Log(const std::string& function, const std::string& message);
}
}
