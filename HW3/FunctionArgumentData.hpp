#pragma once

#include <string>
#include <memory>
#include "TType.h"

class FunctionArgumentData;
typedef std::shared_ptr<FunctionArgumentData> FunctionArgumentDataPtr;

class FunctionArgumentData
{
public:
	FunctionArgumentData(const std::string& id, TType type);

	const std::string& getId() const;
	TType getType() const;

private:
	std::string _id;
	TType _type;	
};