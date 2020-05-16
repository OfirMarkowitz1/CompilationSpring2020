#pragma once

#include <string>
#include "TType.h"

class FunctionArgumentData
{
public:
	FunctionArgumentData(const std::string& id, TType type)

	const std::string& getId() const;
	TType getType() const;

private:
	const std::string _id;
	const TType _type;	
};