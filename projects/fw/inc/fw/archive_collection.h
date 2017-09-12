#pragma once

/*
#include <fstream>
#include "cereal/archives/xml.hpp"
#include "cereal/types/utility.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"
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