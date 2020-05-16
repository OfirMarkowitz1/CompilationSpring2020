#include "NodeCaster.hpp"
#include "hw3_output.hpp"
#include <memory>
#include <iostream>

using namespace std;

NumNodePtr NodeCaster::castNum(NodePtr node) const
{
	auto numNode = dynamic_pointer_cast<NumNode>(node);

	if (numNode == nullptr)
	{
		exitWithSyntaxError(node->getLineNumber());
	}

	return numNode;
}

IdentifierNodePtr NodeCaster::castIdentifier(NodePtr node) const
{
	auto identifierNode = dynamic_pointer_cast<IdentifierNode>(node);

	if (identifierNode == nullptr)
	{
		exitWithSyntaxError(node->getLineNumber());
	}

	return identifierNode;
}

TypeNodePtr NodeCaster::castType(NodePtr node) const
{
	auto typeNode = dynamic_pointer_cast<TypeNode>(node);

	if (typeNode == nullptr)
	{
		exitWithSyntaxError(node->getLineNumber());
	}

	if (typeNode->getType() == T_VOID)
	{
		exitWithVoidError(node->getLineNumber());
	}

	return typeNode;
}

TypeNodePtr NodeCaster::castReturnType(NodePtr node) const
{
	auto returnTypeNode = dynamic_pointer_cast<TypeNode>(node);

	if (returnTypeNode == nullptr)
	{
		exitWithSyntaxError(node->getLineNumber());
	}

	return returnTypeNode;
}

ExpressionNodePtr NodeCaster::castExpression(NodePtr node) const
{
	auto experssionNode = dynamic_pointer_cast<ExpressionNode>(node);

	if (experssionNode == nullptr)
	{
		exitWithSyntaxError(node->getLineNumber());
	}

	return experssionNode;
}

ExpressionListNodePtr NodeCaster::castExpressionList(NodePtr node) const
{
	auto experssionListNode = dynamic_pointer_cast<ExpressionListNode>(node);

	if (experssionListNode == nullptr)
	{
		exitWithSyntaxError(node->getLineNumber());
	}

	return experssionListNode;
}

CallNodePtr NodeCaster::castCall(NodePtr node) const
{
	auto callNode = dynamic_pointer_cast<CallNode>(node);

	if (callNode == nullptr)
	{
		exitWithSyntaxError(node->getLineNumber());
	}

	return callNode;
}

void NodeCaster::exitWithSyntaxError(int lineNumber) const
{
	output::errorSyn(lineNumber);
	exit(1);
}

void NodeCaster::exitWithVoidError(int lineNumber) const
{
	cout << "Expected non-void type in line " << lineNumber << endl;
	exit(1);
}
