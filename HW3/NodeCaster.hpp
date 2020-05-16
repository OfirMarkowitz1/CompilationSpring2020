#pragma once

#include "Node.hpp"

class NodeCaster
{	
public:
	NumNodePtr castNum(NodePtr node) const;
	IdentifierNodePtr castIdentifier(NodePtr node) const; 
	TypeNodePtr castType(NodePtr node) const;
	TypeNodePtr castReturnType(NodePtr node) const;
	ExpressionNodePtr castExpression(NodePtr node) const;
	ExpressionListNodePtr castExpressionList(NodePtr node) const;
	CallNodePtr castCall(NodePtr node) const;

private:
	void exitWithSyntaxError(int lineNumber) const;
	void exitWithVoidError(int lineNumber) const;
};