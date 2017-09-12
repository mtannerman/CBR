#pragma once

namespace cbr
{
	
}


/*
struct Time
{
	int64_t ToSeconds() const
	{
		return 3600LL * hour + 60LL * min + sec;
	}

	Time() = default;
	Time(const int64_t hour, const int64_t min, const int64_t sec, const int64_t millisec) : hour(hour), min(min), sec(sec), millisec(millisec) {}
	static Time FromSeconds(int64_t seconds)
	{
		int64_t h, m, s;
		h = seconds / 3600LL;
		seconds -= h * 3600LL;

		m = seconds / 60LL;
		seconds -= m * 60LL;

		s = seconds;

		return Time(h, m, s, 0LL);
	}

	static Time FromMilliSeconds(int64_t seconds)
	{
		const auto millisecs = seconds % 1000LL;
		auto t = FromSeconds(seconds / 1000LL);
		t.millisec = millisecs;
		return t;
	}

	template <typename Archive>
	void Serialize(Archive& ar, const uint32_t version) {
		ar(CEREAL_NVP(hour));
		ar(CEREAL_NVP(min));
		ar(CEREAL_NVP(sec));
		ar(CEREAL_NVP(millisec));
	}

	int64_t hour;
	int64_t min;
	int64_t sec;
	int64_t millisec;
};

template <typename IntegerT>
std::string ZeroExtendedString(const IntegerT val, const int nDigits = 2)
{
	const std::string str = std::to_string(val);
	const int missing = nDigits - int(str.size());
	if (missing > 0) {
		const std::string prefix(missing, '0');
		return prefix + str;
	}

	return str;
}

std::ostream& operator<<(std::ostream& os, const Time& t)
{
	return os << t.hour << "::" << ZeroExtendedString(t.min) << "::" << ZeroExtendedString(t.sec) << "::" << ZeroExtendedString(t.millisec, 3);
}


*/