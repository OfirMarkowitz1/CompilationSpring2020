#include "FunctionArgumentData.hpp"

FunctionArgumentData::FunctionArgumentData(const std::string& id, TType type) :
_id(id),
_type(type)

const std::string& FunctionArgumentData::getId() const
{
	return _id;
}

TType FunctionArgumentData::getType() const
{
	return _type;
}