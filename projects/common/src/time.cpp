#include "common/time.h"

namespace cbr
{
    int64_t Time::ToSeconds() const
    {
		return 3600LL * hour + 60LL * min + sec;
    }
    

    Time Time::FromSeconds(int64_t seconds)
    {
		int64_t h, m, s;
		h = seconds / 3600LL;
		seconds -= h * 3600LL;

		m = seconds / 60LL;
		seconds -= m * 60LL;

		s = seconds;

		return Time(h, m, s, 0LL);
    }
    
    Time Time::FromMilliSeconds(int64_t seconds)
    {
		const auto millisecs = seconds % 1000LL;
		auto t = FromSeconds(seconds / 1000LL);
		t.millisec = millisecs;
		return t;
    }
}

std::string ZeroExtendedString(const int64_t val, const int nDigits = 2)
{
    const std::string str = std::to_string(val);
    const int missing = nDigits - int(str.size());
    if (missing > 0) {
        const std::string prefix(missing, '0');
        return prefix + str;
    }

    return str;
}

std::ostream& operator<<(std::ostream& os, const cbr::Time& t)
{
    return os << t.hour << "::" << 
    ZeroExtendedString(t.min) << "::" << 
    ZeroExtendedString(t.sec) << "::" << 
    ZeroExtendedString(t.millisec, 3);
}