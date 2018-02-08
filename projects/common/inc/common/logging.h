#pragma once

#include <string>
#include <sstream>

#define STR(expression) (((::std::ostringstream *)&(::std::ostringstream() << "" << expression))->str())
// #define LOG(msg) ::cbr::common_detail::Log(__PRETTY_FUNCTION__, STR(msg));
#define CBR_FANCY_FUNCTION ::cbr::StripPrettyFunction(__FUNCTION__)
#define LOG(msg) ::cbr::common_detail::Log(CBR_FANCY_FUNCTION, STR(msg));


#define DESC(x) #x << ": " << x

namespace cbr
{

std::string StripPrettyFunction(std::string prettyFunction);

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
void Log(const std::string& function, const std::string& message);
}
}
