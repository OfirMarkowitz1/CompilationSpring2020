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
		exitWithCastError(node->getLineNumber(), "num");
	}

	return numNode;
}

StringNodePtr NodeCaster::castString(NodePtr node) const
{
	auto stringNode = dynamic_pointer_cast<StringNode>(node);

	if (stringNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "string");
	}

	return stringNode;
}

IdentifierNodePtr NodeCaster::castIdentifier(NodePtr node) const
{
	auto identifierNode = dynamic_pointer_cast<IdentifierNode>(node);

	if (identifierNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "id");
	}

	return identifierNode;
}

TypeNodePtr NodeCaster::castType(NodePtr node) const
{
	auto typeNode = dynamic_pointer_cast<TypeNode>(node);

	if (typeNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "type");
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
		exitWithCastError(node->getLineNumber(), "ret type");
	}

	return returnTypeNode;
}

ExpressionNodePtr NodeCaster::castExpression(NodePtr node) const
{
	auto experssionNode = dynamic_pointer_cast<ExpressionNode>(node);

	if (experssionNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "exp");
	}

	return experssionNode;
}

NumericExpressionNodePtr NodeCaster::castNumericExpression(NodePtr node) const
{
	auto numericExperssionNode = dynamic_pointer_cast<NumericExpressionNode>(node);

	if (numericExperssionNode == nullptr)
	{
		exitWithMismatchError(node->getLineNumber());
	}

	return numericExperssionNode;
}

BooleanExpressionNodePtr NodeCaster::castBooleanExpression(NodePtr node) const
{
	auto booleanExperssionNode = dynamic_pointer_cast<BooleanExpressionNode>(node);

	if (booleanExperssionNode == nullptr)
	{
		exitWithMismatchError(node->getLineNumber());
	}

	return booleanExperssionNode;
}

StringExpressionNodePtr NodeCaster::castStringExpression(NodePtr node) const
{
	auto stringExperssionNode = dynamic_pointer_cast<StringExpressionNode>(node);

	if (stringExperssionNode == nullptr)
	{
		exitWithMismatchError(node->getLineNumber());
	}

	return stringExperssionNode;
}

ExpressionListNodePtr NodeCaster::castExpressionList(NodePtr node) const
{
	auto experssionListNode = dynamic_pointer_cast<ExpressionListNode>(node);

	if (experssionListNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "exp list");
	}

	return experssionListNode;
}

CallNodePtr NodeCaster::castCall(NodePtr node) const
{
	auto callNode = dynamic_pointer_cast<CallNode>(node);

	if (callNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "call");
	}

	return callNode;
}

FormalDeclarationNodePtr NodeCaster::castFormalDecl(NodePtr node) const
{
	auto formalDeclarationNode = dynamic_pointer_cast<FormalDeclarationNode>(node);

	if (formalDeclarationNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "formal declaration");
	}

	return formalDeclarationNode;
}

FormalsNodePtr NodeCaster::castFormals(NodePtr node) const
{
	auto formalsNode = dynamic_pointer_cast<FormalsNode>(node);

	if (formalsNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "formals");
	}

	return formalsNode;
}

BinopNodePtr NodeCaster::castBinop(NodePtr node) const
{
	auto binopNode = dynamic_pointer_cast<BinopNode>(node);

	if (binopNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "binop");
	}

	return binopNode;
}

RelopNodePtr NodeCaster::castRelop(NodePtr node) const
{
	auto relopNode = dynamic_pointer_cast<RelopNode>(node);

	if (relopNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "relop");
	}

	return relopNode;
}

MarkerNodePtr NodeCaster::castMarker(NodePtr node) const
{
	auto markerNode = dynamic_pointer_cast<MarkerNode>(node);

	if (markerNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "marker");
	}

	return markerNode;
}

BackPatchNodePtr NodeCaster::castBackPatch(NodePtr node) const
{
	auto backPatchNode = dynamic_pointer_cast<BackPatchNode>(node);

	if (backPatchNode == nullptr)
	{
		exitWithCastError(node->getLineNumber(), "back patch");
	}

	return backPatchNode;
}

void NodeCaster::exitWithCastError(int lineNumber, const std::string& expectedTypeStr) const
{
	cout << "Expected " << expectedTypeStr << " in line number " << lineNumber << endl;
	exit(1);
}

void NodeCaster::exitWithVoidError(int lineNumber) const
{
	cout << "Expected non-void type in line " << lineNumber << endl;
	exit(1);
}

void NodeCaster::exitWithMismatchError(int lineNumber) const
{
	output::errorMismatch(lineNumber);
	exit(1);
}