#pragma once

#include "Node.hpp"

class ExpressionNodeFactory
{
public:
	ExpressionNodePtr createInt(int lineNumber) const;
	ExpressionNodePtr createByte(int lineNumber) const;
	ExpressionNodePtr createBool(int lineNumber) const;
	ExpressionNodePtr createString(int lineNumber) const;

	ExpressionNodePtr createNumericBinop(ExpressionNodePtr lhs, ExpressionNodePtr rhs) const;
	ExpressionNodePtr createLogicalBinop(ExpressionNodePtr lhs, ExpressionNodePtr rhs) const;
	ExpressionNodePtr createRelop(ExpressionNodePtr lhs, ExpressionNodePtr rhs) const;

	ExpressionNodePtr create(int lineNumber, TType type) const;

	ExpressionListNodePtr createList(ExpressionNodePtr first) const;
};
