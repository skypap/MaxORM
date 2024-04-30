#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include <stdexcept>
#include <iostream>
#include <algorithm>

class DBObject
{
	friend class MaxDB;
protected:
	std::vector<std::pair< std::pair<std::string, std::string >, std::string>> TableVector;
	void RegisterMember(const std::pair< std::pair<std::string, std::string>, std::string>& Pair);

	virtual void ExportMembers() = 0;

	virtual void ImportMembers(std::vector<std::string> element) = 0;
};

