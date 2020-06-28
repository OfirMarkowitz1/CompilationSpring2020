#pragma once

#include "SymbolTable.hpp"
#include "SymbolTableAsserter.hpp"
#include "Node.hpp"
#include "NodeCaster.hpp"
#include "TypeNodeFactory.hpp"
#include "ExpressionNodeFactory.hpp"
#include "FormalsNodeFactory.hpp"
#include "FreshVariableFactory.hpp"
#include <stack>

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
	void handleWhileBeforeScope(NodePtr marker, NodePtr expression);
	void handleWhileAfterScope();
	void handleWhileEnded();
	void reduceBreak();
	void reduceContinue();
	void reduceCall(NodePtr& call, NodePtr functionId, NodePtr expressionList);
	void reduceExpressionList(NodePtr& expressionList, NodePtr expression);
	void reduceExpressionList(NodePtr& expressionList, NodePtr expression, NodePtr subExpressionList);
	void reduceIntType(NodePtr& type);
	void reduceByteType(NodePtr& type);
	void reduceBoolType(NodePtr& type);
    void reduceInParenthesesExpression(NodePtr& expression, NodePtr expressionInParentheses);
	void reduceBinopExpression(NodePtr& expression, NodePtr lhs, NodePtr binop, NodePtr rhs);
	void reduceVariableExpression(NodePtr& expression, NodePtr variableId);
	void reduceCallExpression(NodePtr& expression, NodePtr call);
	void reduceNumExpression(NodePtr& expression, NodePtr num);
	void reduceByteNumExpression(NodePtr& expression, NodePtr byteNum);
	void reduceStringExpression(NodePtr& expression, NodePtr stringBaseNode);
	void reduceTrueExpression(NodePtr& expression);
	void reduceFalseExpression(NodePtr& expression);
	void reduceNotExpression(NodePtr& expression, NodePtr operandExpression);
	void reduceAndExpression(NodePtr& expression, NodePtr lhs, NodePtr marker, NodePtr rhs);
	void reduceOrExpression(NodePtr& expression, NodePtr lhs, NodePtr marker, NodePtr rhs);
	void reduceRelopExpression(NodePtr& expression, NodePtr lhs, NodePtr relop, NodePtr rhs);
	void reduceMarker(NodePtr& marker);

private:

	void declareSystemCalls();

	void defineDivisionByZeroErrorString();

	void defineGlobalString(const std::string& stringValue, const std::string& stringName);

	int emitConditionalBranch(const std::string& condVar);
	int emitUnconditionalBranch();

	std::string handleGetBoolVarWithValue(BooleanExpressionNodePtr booleanExpression);
	BooleanExpressionNodePtr createBranchBooleanExpression(int lineNumber, const std::string& condVar);

#pragma mark Print functions definition helper methods

	void handleDefinePrintFunction();
	void handleDefinePrintiFunction();

#pragma mark Fields

	TType _functionScopeRetType = T_VOID;
	std::stack<std::string> _loopConditionLabelStack;
	std::vector<BPItem> _loopNextAddressList;

#pragma mark Symbol Tables

	VariablesTable _variablesTable;
	GlobalScopeFunctionsTable _functionsTable;
	SymbolTableAsserter _symbolTableAsserter;

#pragma mark Node Factories

	TypeNodeFactory _typeNodeFactory;
	ExpressionNodeFactory _expressionNodeFactory;
	FormalsNodeFactory _formalsNodeFactory;

#pragma mark Misc Services

	NodeCaster _nodeCaster;
	FreshVariableFactory _freshVariableFactory;
};
