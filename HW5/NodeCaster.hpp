#pragma once

#include "Node.hpp"

class NodeCaster
{	
public:
	NumNodePtr castNum(NodePtr node) const;
	StringNodePtr castString(NodePtr node) const;
	IdentifierNodePtr castIdentifier(NodePtr node) const; 
	TypeNodePtr castType(NodePtr node) const;
	TypeNodePtr castReturnType(NodePtr node) const;
	ExpressionNodePtr castExpression(NodePtr node) const;
	NumericExpressionNodePtr castNumericExpression(NodePtr node) const;
	BooleanExpressionNodePtr castBooleanExpression(NodePtr node) const;
	StringExpressionNodePtr castStringExpression(NodePtr node) const;
	ExpressionListNodePtr castExpressionList(NodePtr node) const;
	CallNodePtr castCall(NodePtr node) const;
	FormalDeclarationNodePtr castFormalDecl(NodePtr node) const;
	FormalsNodePtr castFormals(NodePtr node) const;
	BinopNodePtr castBinop(NodePtr node) const;
	RelopNodePtr castRelop(NodePtr node) const;
	MarkerNodePtr castMarker(NodePtr node) const;

private:
	void exitWithCastError(int lineNumber, const std::string& expectedTypeStr) const;
	void exitWithVoidError(int lineNumber) const;
	void exitWithMismatchError(int lineNumber) const;
};