#pragma once

/*
#include <fstream>
#include "cereal/cereal.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/utility.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"
...
*/


/*
template <typename Archive>
	void Serialize(Archive& ar, const uint32_t version) {
		ar(CEREAL_NVP(dummy1));
		ar(CEREAL_NVP(dummy2));
		if (version >= 1) {
			ar(CEREAL_NVP(dummy3));
		}
		if (version >= 2) {
				ar(cereal::base_class<DummyBase>(this));
			}
	}
*/

/*
CEREAL_CLASS_VERSION(ns1::ns2::DummyObject, 0);
*/

/*
template <typename CollT>
void ArchiveCollection(const CollT& collection, const std::string& fileName)
{
	std::ofstream s(fileName);
	cereal::XMLOutputArchive ar(s);
	ar(collection);
}

template <typename CollT>
CollT ReadCollection(const std::string& fileName)
{
	std::ifstream s(fileName);
	cereal::XMLInputArchive ar(s);
	CollT coll;
	ar(coll);
	return coll;
}
*/

namespace cbr
{

}