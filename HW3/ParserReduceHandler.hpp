#pragma once

#include "SymbolTable.hpp"
#include "SymbolTableAsserter.hpp"
#include "Node.hpp"
#include "NodeCaster.hpp"
#include "TypeNodeFactory.hpp"
#include "ExpressionNodeFactory.hpp"
#include "CallNodeFactory.hpp"

class ParserReduceHandler
{
public:
	ParserReduceHandler();

	reduceProgram();
	reduceFunctionDeclaration(NodePtr& functionDeclaration, NodePtr retType, NodePtr functionId, NodePtr formals);
	reduceReturnType(NodePtr& retType, NodePtr type);
	reduceVoidReturnType(NodePtr& retType);
	reduceEmptyFormals(NodePtr& formals);
	reduceFormals(NodePtr& formals, NodePtr formalsList);
	reduceFormalsList(NodePtr& formalslist, NodePtr formalDeclaration);
	reduceFormalsList(NodePtr& formalslist, NodePtr subFormalslist, NodePtr formalDeclaration);
	reduceFormalDeclaration(NodePtr& formalDeclaration, NodePtr formalType, NodePtr formalId);
	reduceVariableDeclarationStatement(NodePtr variableType, NodePtr variableId);
	reduceAssignedVariableDeclarationStatement(NodePtr variableType, NodePtr variableId, NodePtr assignExpression);
	reduceAssignedVariableStatement(NodePtr variableId, NodePtr assignExpression);
	reduceVoidReturn();
	reduceReturn(NodePtr returnExpression);
	reduceBreak();
	reduceContinue();
	reduceCall(NodePtr& call, NodePtr functionId, NodePtr expressionList);
	reduceNoArgumentsCall(NodePtr& call, NodePtr functionId);
	reduceExpressionList(NodePtr& expressionList, NodePtr expression);
	reduceExpressionList(NodePtr& expressionList, NodePtr expression, NodePtr subExpressionList);
	reduceIntType(NodePtr& type);
	reduceByteType(NodePtr& type);
	reduceBoolType(NodePtr& type);
    reduceInParenthesesExpression(NodePtr& expression);
	reduceBinopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs);
	reduceIdentifierExpression(NodePtr& expression, NodePtr identifier);
	reduceCallExpression(NodePtr& expression, NodePtr call);
	reduceNumExpression(NodePtr& expression, NodePtr num);
	reduceByteNumExpression(NodePtr& expression, NodePtr byteNum);
	reduceStringExpression(NodePtr& expression);
	reduceTrueExpression(NodePtr& expression);
	reduceFalseExpression(NodePtr& expression);
	reduceNotExpression(NodePtr& expression, NodePtr expression);
	reduceLogicalBinopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs);
	reduceRelopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs);

private:


#pragma mark Symbol Tables
	VariablesTable _variablesTable;
	GlobalScopeFunctionsTable _functionsTable;

	SymbolTableAsserter _symbolTableAsserter;

	NodeCaster _nodeCaster;

#pragma mark Node Factories
	TypeNodeFactory _typeNodeFactory;
	ExpressionNodeFactory _expressionNodeFactory;
	CallNodeFactory _callNodeFactory;
};