#pragma once

#include "Node.hpp"

class ExpressionNodeFactory
{
public:
	NumericExpressionNodePtr createInt(int lineNumber, const std::string& varName) const;
	NumericExpressionNodePtr createByte(int lineNumber, const std::string& varName) const;
	BooleanExpressionNodePtr createBool(int lineNumber, const std::vector<BPItem>& trueList, const std::vector<BPItem>& falseList) const;
	StringExpressionNodePtr createString(int lineNumber, const std::string& varName, int numBytes) const;

	NumericExpressionNodePtr createNumeric(int lineNumber, TType type, const std::string& varName) const;

	ExpressionListNodePtr createList(ExpressionNodePtr first) const;
};
