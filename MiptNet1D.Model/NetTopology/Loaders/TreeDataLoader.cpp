#include <boost/algorithm/string.hpp>
#include <sstream>
#include "TreeDataLoader.h"

namespace MiptNet1DModel {
	std::vector<std::string>& TreeDataLoader::split(std::string& s, char delim,
		std::vector<std::string>& elems) const	{
		boost::algorithm::trim(s);
		std::stringstream ss(s);
		std::string item;
		while (getline(ss, item, delim)) {
			if (item.length() > 0)
				elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::string> TreeDataLoader::split(std::string& s, char delim) {
		std::vector<std::string> elems;
		boost::algorithm::trim(s);
		split(s, delim, elems);
		return elems;
	}
}