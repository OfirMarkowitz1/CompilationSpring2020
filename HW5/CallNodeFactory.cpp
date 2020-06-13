#include "CallNodeFactory.hpp"
#include <memory>

CallNodePtr CallNodeFactory::create(int lineNumber, TType returnType) const
{
	return std::make_shared<CallNode>(lineNumber, returnType);
}