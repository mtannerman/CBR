#include "common/time.h"
#include <ctime>
#include <iomanip>
#include "common/logging.h"
#include "common/exceptions.h"

namespace cbr
{

std::string GetDateTimeStr()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    return STR(std::put_time(&tm, "%Y-%m-%d-%H-%M-%S"));
}
    

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

StopWatch::StopWatch()
{
    Restart();
}

void StopWatch::Restart()
{
    start = std::chrono::system_clock::now();
}

double StopWatch::ElapsedTime(const Unit unit)
{
    switch (unit) {
        case Unit::MILLISEC:
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
        case Unit::NANOSEC:
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - start).count();
        default:
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() / 1000.;
    }

    THROW("unreachable code");

    return 0.;
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

}




std::ostream& operator<<(std::ostream& os, const cbr::Time& t)
{
    return os << t.hour << "::" << 
    cbr::ZeroExtendedString(t.min) << "::" << 
    cbr::ZeroExtendedString(t.sec) << "::" << 
    cbr::ZeroExtendedString(t.millisec, 3);
}