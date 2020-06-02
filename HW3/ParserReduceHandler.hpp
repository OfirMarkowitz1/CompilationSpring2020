#pragma once

#include "SymbolTable.hpp"
#include "SymbolTableAsserter.hpp"
#include "Node.hpp"
#include "NodeCaster.hpp"
#include "TypeNodeFactory.hpp"
#include "ExpressionNodeFactory.hpp"
#include "CallNodeFactory.hpp"
#include "FormalsNodeFactory.hpp"

class ParserReduceHandler
{
public:
	ParserReduceHandler();

	void reduceProgram();
	void addFunctionAndOpenScope(NodePtr retType, NodePtr functionId, NodePtr formals);
	void reduceReturnType(NodePtr& retType, NodePtr type);
	void reduceVoidReturnType(NodePtr& retType);
	void reduceEmptyFormals(NodePtr& formals);
	void reduceFormals(NodePtr& formals, NodePtr formalsList);
	void reduceFormalsList(NodePtr& formalsList, NodePtr formalDeclaration);
	void reduceFormalsList(NodePtr& formalsList, NodePtr formalDeclaration, NodePtr subFormalslist);
	void reduceFormalDeclaration(NodePtr& formalDeclaration, NodePtr formalType, NodePtr formalId);
	void openScope();
	void closeScope();
	void reduceVariableDeclarationStatement(NodePtr variableType, NodePtr variableId);
	void reduceAssignedVariableDeclarationStatement(NodePtr variableType, NodePtr variableId, NodePtr assignExpression);
	void reduceAssignedVariableStatement(NodePtr variableId, NodePtr assignExpression);
	void reduceVoidReturn();
	void reduceReturn(NodePtr returnExpression);
	void handleIfBeforeScope(NodePtr expression);
	void handleWhileBeforeScope(NodePtr expression);
	void handleWhileAfterScope();
	void reduceBreak();
	void reduceContinue();
	void reduceCall(NodePtr& call, NodePtr functionId, NodePtr expressionList);
	void reduceNoArgumentsCall(NodePtr& call, NodePtr functionId);
	void reduceExpressionList(NodePtr& expressionList, NodePtr expression);
	void reduceExpressionList(NodePtr& expressionList, NodePtr expression, NodePtr subExpressionList);
	void reduceIntType(NodePtr& type);
	void reduceByteType(NodePtr& type);
	void reduceBoolType(NodePtr& type);
    void reduceInParenthesesExpression(NodePtr& expression, NodePtr expressionInParentheses);
	void reduceBinopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs);
	void reduceVariableExpression(NodePtr& expression, NodePtr variableId);
	void reduceCallExpression(NodePtr& expression, NodePtr call);
	void reduceNumExpression(NodePtr& expression, NodePtr num);
	void reduceByteNumExpression(NodePtr& expression, NodePtr byteNum);
	void reduceStringExpression(NodePtr& expression);
	void reduceTrueExpression(NodePtr& expression);
	void reduceFalseExpression(NodePtr& expression);
	void reduceNotExpression(NodePtr& expression, NodePtr operandExpression);
	void reduceLogicalBinopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs);
	void reduceRelopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs);

private:

#pragma mark Fields

	TType _functionScopeRetType = T_VOID;
	int _loopsScopesCounter = 0;

#pragma mark Symbol Tables

	VariablesTable _variablesTable;
	GlobalScopeFunctionsTable _functionsTable;
	SymbolTableAsserter _symbolTableAsserter;

#pragma mark Node Factories

	TypeNodeFactory _typeNodeFactory;
	ExpressionNodeFactory _expressionNodeFactory;
	CallNodeFactory _callNodeFactory;
	FormalsNodeFactory _formalsNodeFactory;

#pragma mark Misc Services

	NodeCaster _nodeCaster;
};
