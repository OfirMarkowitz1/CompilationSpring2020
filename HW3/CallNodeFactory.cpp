#include "CallNodeFactory.hpp"
#include <memory>

CallNodePtr CallNodeFactory::create(TType returnType, IdentifierNodePtr identifier) const
{
	return std::make_shared<CallNode>(returnType, identifier);
}