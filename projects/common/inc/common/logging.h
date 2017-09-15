#pragma once

#include <string>

#define STR(expression) (((::std::ostringstream*)&(::std::ostringstream() << "" << expression))->str())
#define LOG(msg) ::cbr::common_detail::Log(__FUNCTION__, STR(msg)); 

#define DESC(x) #x << ": " << x 

namespace cbr
{
namespace common_detail
{
	void Log(const std::string& function, const std::string& message);
}
}
