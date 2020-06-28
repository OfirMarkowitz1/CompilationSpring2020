#include "ExpressionNodeFactory.hpp"

using namespace std;

NumericExpressionNodePtr ExpressionNodeFactory::createInt(int lineNumber, const std::string& varName) const
{
	return createNumeric(lineNumber, T_INT, varName);
}

NumericExpressionNodePtr ExpressionNodeFactory::createByte(int lineNumber, const std::string& varName) const
{
	return createNumeric(lineNumber, T_BYTE, varName);
}

BooleanExpressionNodePtr ExpressionNodeFactory::createBool(int lineNumber, const std::vector<BPItem>& trueList, const std::vector<BPItem>& falseList) const
{
	return make_shared<BooleanExpressionNode>(lineNumber, trueList, falseList);
}

StringExpressionNodePtr ExpressionNodeFactory::createString(int lineNumber, const std::string& varName, int numBytes) const
{
	return make_shared<StringExpressionNode>(lineNumber, varName, numBytes);
}

NumericExpressionNodePtr ExpressionNodeFactory::createNumeric(int lineNumber, TType type, const std::string& varName) const
{
	return make_shared<NumericExpressionNode>(lineNumber, type, varName);
}

ExpressionListNodePtr ExpressionNodeFactory::createList(ExpressionNodePtr first) const
{
	return make_shared<ExpressionListNode>(first);	
}
