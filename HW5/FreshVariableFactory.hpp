#pragma once

#include <string>

class FreshVariableFactory
{
public:
	FreshVariableFactory();

	std::string createFreshVariable();

	void reset();

private:

	int _nextVariableSuffix = 0;
};