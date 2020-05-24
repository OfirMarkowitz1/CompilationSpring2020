#pragma once

#include <vector>

enum TType
{
    T_INT,
    T_BYTE,
    T_BOOL,
    T_STRING,
    T_VOID
};

std::string getTTypeString(TType type)
{
	switch (type)
	{
	case T_INT:
		return "INT";
	case T_BYTE:
		return "BYTE";
	case T_BOOL:
		return "BOOL";
	case T_STRING:
		return "STRING";
	case T_VOID:
		return "VOID";
	}
}

std::vector<std::string> getTTypesStrings(const std::vector<TType>& types)
{
	std::vector<std::string> typesStrings;	

	for (TType type : types)
	{
		typesStrings.push_back(getTTypeString(type));
	}

	return typesStrings;
}