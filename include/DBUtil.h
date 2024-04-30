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
#define TEST_MAKE_PAIR(Column,Type) MakePair(#Column, Column, Type)
#define REGISTER_MEMBER(Column, Type) RegisterMember(MakePair(#Column, Column, Type))
#define REGISTER_MEMBER_WITH_CONSTRAINT(Column, Type,Constraint) RegisterMember(MakePair(#Column, Column, Type, Constraint))

enum class ESQLResult {
	OK = 0,
	NOT_OK = 1
};
enum class ESQLDataType : int
{
	INTEGER,
	REAL,
	TEXT,
	BLOB
};
#undef DEFAULT

enum class ESQLConstraint :int
{
	PRIMARY_KEY,
	FORIEGN_KEY,
	UNIQUE,
	NOT_NULL,
	DEFAULT,
	CHECK,
	NONE,
};

static std::map<ESQLDataType, std::string> TypeMap = { {ESQLDataType::INTEGER , "INTEGER"},
												{ESQLDataType::REAL , "REAL" },
												{ESQLDataType::TEXT , "TEXT"},
												{ESQLDataType::BLOB , "BLOB"} };



static std::map<ESQLConstraint, std::string> ConstraintMap = {
								{ESQLConstraint::PRIMARY_KEY, "PRIMARY KEY"},
								{ESQLConstraint::FORIEGN_KEY, "FORIEGN KEY"},
								{ESQLConstraint::UNIQUE, "UNIQUE"},
								{ESQLConstraint::NOT_NULL, "NOT NULL"},
								{ESQLConstraint::DEFAULT, "DEFAULT"},
								{ESQLConstraint::CHECK, "CHECK"},
								{ESQLConstraint::NONE, ""} };


template <class C>
std::pair<std::pair<std::string, std::string>, std::string> MakePair(const std::string& Column, C Value, ESQLDataType Type, ESQLConstraint Constraint)
{
	std::ostringstream str;




	std::string TypeStr = TypeMap[Type];

	std::string ConstraintStr = ConstraintMap[Constraint];

	if (Type == ESQLDataType::TEXT)
		str << "'" << Value << "'";

	else
		str << Value;

	std::string Val = str.str();

 	std::pair<std::string, std::string> ColumnPair = { Column, TypeStr + " " + ConstraintStr };

	return { ColumnPair, Val };
}

template <class C>
std::pair<std::pair<std::string, std::string>, std::string> MakePair(const std::string& Column, C Value, ESQLDataType Type)
{
	std::ostringstream str;


	std::string TypeS = TypeMap[Type];

	if (Type == ESQLDataType::TEXT)
		str << "'" << Value << "'";

	else
		str << Value;

	std::string Val = str.str();

	std::pair<std::string, std::string> ColumnPair = { Column, TypeS };

	return { ColumnPair, Val };
}

namespace SQLite_ORM_CPP
{
	//Function returns Name of Class
	template<class C>
	std::string getClassName();
}

#define EXPORT_CLASS(ClName)					\
template<>										\
std::string SQLite_ORM_CPP::getClassName<ClName>()	    \
{	std::string temp(#ClName);std::replace(temp.begin(), temp.end(), ':', '_');return temp;}
