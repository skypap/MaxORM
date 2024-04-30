#include "DBObject.h"
void DBObject::RegisterMember(const std::pair< std::pair<std::string, std::string >, std::string>& Pair)
{
	TableVector.push_back(Pair);
}