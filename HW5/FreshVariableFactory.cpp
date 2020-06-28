#include "FreshVariableFactory.hpp"

const std::string baseVariableName = "var";

FreshVariableFactory::FreshVariableFactory()
{
	reset();
}

std::string FreshVariableFactory::createFreshVariable()
{
	return baseVariableName + std::to_string(_nextVariableSuffix++);
}

void FreshVariableFactory::reset()
{
	_nextVariableSuffix = 0;
}