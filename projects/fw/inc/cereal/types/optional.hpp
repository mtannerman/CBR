#pragma once

#include <cereal/cereal.hpp>
#include "optional/optional.hpp"

namespace cereal {
	template<class Archive, typename T>
	void SerializeSave(Archive& ar, const std::experimental::optional<T>& opt)
	{
		bool valid = (bool)opt;

		ar(valid);
		if (valid) {
			ar(*opt);
		}
	};

	template<class Archive, typename T>
	void SerializeLoad(Archive& ar, std::experimental::optional<T>& opt)
	{
		bool valid;
		ar(valid);
		if (valid) {
			T tmp;
			ar(tmp);
			opt = tmp;
		}
	};
} //namespace cereal

