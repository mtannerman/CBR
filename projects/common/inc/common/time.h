#pragma once

#include <cstdint>
#include "cereal/cereal.hpp"
#include <string>

namespace cbr
{

std::string GetDateTimeStr();

struct Time
{
	int64_t ToSeconds() const;

	Time() = default;
	Time(const int64_t hour, 
		const int64_t min, 
		const int64_t sec, 
		const int64_t millisec) 
		: hour(hour)
		, min(min), sec(sec)
		, millisec(millisec) 
	{}

	static Time FromSeconds(int64_t seconds);

	static Time FromMilliSeconds(int64_t seconds);

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

}   //cbr



std::ostream& operator<<(std::ostream& os, const cbr::Time& t);
