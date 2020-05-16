#include "TypeNodeFactory.hpp"

TypeNodePtr TypeNodeFactory::createInt(int lineNumber) const
{
	return create(lineNumber, T_INT);
}

TypeNodePtr TypeNodeFactory::createByte(int lineNumber) const
{
	return create(lineNumber, T_BYTE);
}

TypeNodePtr TypeNodeFactory::createBool(int lineNumber) const
{
	return create(lineNumber, T_BOOL);
}

TypeNodePtr TypeNodeFactory::createVoid(int lineNumber) const
{
	return create(lineNumber, T_VOID);
}

TypeNodePtr TypeNodeFactory::create(int lineNumber, TType type) const
{
	return std::make_shared<TypeNode>(lineNumber, type);
}