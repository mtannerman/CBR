#pragma once

#include <string>
#include <sstream>

#define STR(expression) (((::std::ostringstream *)&(::std::ostringstream() << "" << expression))->str())
#define LOG(msg) ::cbr::common_detail::Log(__FUNCTION__, STR(msg));

#define DESC(x) #x << ": " << x

namespace cbr
{

template <typename CollectionT>
std::string CollectionToStr(const CollectionT& collection, const std::string delimiter = "")
{
    std::stringstream ss;
    for (const auto& elem : collection) {
        ss << elem << delimiter;
    }

    return ss.str();
}

template <typename CollectionT, typename ToStringF>
std::string CollectionToStr(const CollectionT& collection, ToStringF toStringFunction, const std::string delimiter = "")
{
    std::stringstream ss;
    for (const auto& elem : collection) {
        ss << toStringFunction(elem) << delimiter;
    }

    return ss.str();
}

namespace common_detail
{
void Log(const std::string &function, const std::string &message);
}
}
