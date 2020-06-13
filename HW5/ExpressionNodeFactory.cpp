#include "ExpressionNodeFactory.hpp"

ExpressionNodePtr ExpressionNodeFactory::createInt(int lineNumber) const
{
	return create(lineNumber, T_INT);
}

ExpressionNodePtr ExpressionNodeFactory::createByte(int lineNumber) const
{
	return create(lineNumber, T_BYTE);
}

ExpressionNodePtr ExpressionNodeFactory::createBool(int lineNumber) const
{
	return create(lineNumber, T_BOOL);
}

ExpressionNodePtr ExpressionNodeFactory::createString(int lineNumber) const
{
	return create(lineNumber, T_STRING);
}

ExpressionNodePtr ExpressionNodeFactory::createNumericBinop(ExpressionNodePtr lhs, ExpressionNodePtr rhs) const
{
	lhs->assertNumeric();
	rhs->assertNumeric();

	TType resultNumericType = (((lhs->getType() == T_INT) || (rhs->getType() == T_INT)) ? T_INT : T_BYTE);

	return create(lhs->getLineNumber(), resultNumericType);
}

ExpressionNodePtr ExpressionNodeFactory::createLogicalBinop(ExpressionNodePtr lhs, ExpressionNodePtr rhs) const
{
	lhs->assertBool();
	rhs->assertBool();

	return createBool(lhs->getLineNumber());
}

ExpressionNodePtr ExpressionNodeFactory::createRelop(ExpressionNodePtr lhs, ExpressionNodePtr rhs) const
{
	lhs->assertNumeric();
	rhs->assertNumeric();

	return createBool(lhs->getLineNumber());
}

ExpressionNodePtr ExpressionNodeFactory::create(int lineNumber, TType type) const
{
	return std::make_shared<ExpressionNode>(lineNumber, type);
}

ExpressionListNodePtr ExpressionNodeFactory::createList(ExpressionNodePtr first) const
{
	return std::make_shared<ExpressionListNode>(first);	
}