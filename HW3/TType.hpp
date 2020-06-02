#pragma once

#include <vector>
#include <string>

enum TType
{
    T_INT,
    T_BYTE,
    T_BOOL,
    T_STRING,
    T_VOID
};

class TTypeUtils
{
public:
	static std::string getTTypeString(TType type);

	static std::vector<std::string> getTTypesStrings(const std::vector<TType>& types);
};


