#include "ParserReduceHandler.hpp"
#include "hw3_output.hpp"

using namespace std;

extern int yylineno;

ParserReduceHandler::ParserReduceHandler() :
_symbolTableAsserter(_variablesTable, _functionsTable)
{
	_functionsTable.add("print", T_VOID, { T_STRING });
	_functionsTable.add("printi", T_VOID, { T_INT });
}

void ParserReduceHandler::reduceProgram()
{
	_symbolTableAsserter.assertMainFunctionDefined();

	output::endScope();

	_functionsTable.print();
}

void ParserReduceHandler::addFunctionAndOpenScope(NodePtr retType, NodePtr functionId, NodePtr formals)
{
	IdentifierNodePtr functionIdNode = _nodeCaster.castIdentifier(functionId);
	const string& functionIdString = functionIdNode->getValue();

	_symbolTableAsserter.assertIdentifierUndefined(functionIdString, functionIdNode->getLineNumber());

	const TType retTType = _nodeCaster.castReturnType(retType)->getType();
	
	FormalsNodePtr formalsNode = _nodeCaster.castFormals(formals);

	_functionsTable.add(functionIdString, retTType, formalsNode->getArgTypes());

	_variablesTable.pushFunctionScope(formalsNode->getArguments());

	_functionScopeRetType = retTType;
}

void ParserReduceHandler::reduceReturnType(NodePtr& retType, NodePtr type)
{
	retType = type;
}

void ParserReduceHandler::reduceVoidReturnType(NodePtr& retType)
{
	retType = _typeNodeFactory.createVoid(yylineno);
}

void ParserReduceHandler::reduceEmptyFormals(NodePtr& formals)
{
	formals = _formalsNodeFactory.createFormals(yylineno);
}

void ParserReduceHandler::reduceFormals(NodePtr& formals, NodePtr formalsList)
{
	formals = formalsList;
}

void ParserReduceHandler::reduceFormalsList(NodePtr& formalsList, NodePtr formalDeclaration)
{
	FormalDeclarationNodePtr formalDeclNode = _nodeCaster.castFormalDecl(formalDeclaration);

	FormalsNodePtr formalsNode = _formalsNodeFactory.createFormals(formalDeclNode->getLineNumber());

	formalsNode->pushFront(formalDeclNode);

	formalsList = formalsNode;
}

void ParserReduceHandler::reduceFormalsList(NodePtr& formalsList, NodePtr formalDeclaration, NodePtr subFormalslist)
{
	FormalDeclarationNodePtr formalDeclNode = _nodeCaster.castFormalDecl(formalDeclaration);

	FormalsNodePtr formalsNode = _nodeCaster.castFormals(subFormalslist);

	formalsNode->assertIdDoesNotExist(formalDeclNode->getLineNumber(), formalDeclNode->getId());

	formalsNode->pushFront(formalDeclNode);

	formalsList = formalsNode;
}

void ParserReduceHandler::reduceFormalDeclaration(NodePtr& formalDeclaration, NodePtr formalType, NodePtr formalId)
{
	const TType formalTType = _nodeCaster.castType(formalType)->getType();
	const string formalIdString = _nodeCaster.castIdentifier(formalId)->getValue();

	formalDeclaration = _formalsNodeFactory.createFormalDecl(yylineno, formalTType, formalIdString);
}

void ParserReduceHandler::openScope()
{
	_variablesTable.pushRegularScope();
}

void ParserReduceHandler::closeScope()
{
	output::endScope();

	ConstVariablesScopePtr variablesScope = _variablesTable.popScope();

	variablesScope->print();
}

void ParserReduceHandler::reduceVariableDeclarationStatement(NodePtr variableType, NodePtr variableId)
{
	const TType variableTType = _nodeCaster.castType(variableType)->getType();
	
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();

	_symbolTableAsserter.assertIdentifierUndefined(variableIdString, identifierNode->getLineNumber());

	_variablesTable.add(variableIdString, variableTType);
}

void ParserReduceHandler::reduceAssignedVariableDeclarationStatement(NodePtr variableType, NodePtr variableId, NodePtr assignExpression)
{
	const TType variableTType = _nodeCaster.castType(variableType)->getType();
	
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();

	_symbolTableAsserter.assertIdentifierUndefined(variableIdString, identifierNode->getLineNumber());

	_nodeCaster.castExpression(assignExpression)->assertAssignAllowed(variableTType);

	_variablesTable.add(variableIdString, variableTType);
}

void ParserReduceHandler::reduceAssignedVariableStatement(NodePtr variableId, NodePtr assignExpression)
{
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();

	_symbolTableAsserter.assertVariableDefined(variableIdString, identifierNode->getLineNumber());

	VariableEntryPtr variableEntry = _variablesTable.find(variableIdString);

	_nodeCaster.castExpression(assignExpression)->assertAssignAllowed(variableEntry->getType());
}

void ParserReduceHandler::reduceVoidReturn()
{
	if (_functionScopeRetType != T_VOID)
	{
		output::errorMismatch(yylineno);
		exit(1);
	}
}

void ParserReduceHandler::reduceReturn(NodePtr returnExpression)
{
	_nodeCaster.castExpression(returnExpression)->assertAssignAllowed(_functionScopeRetType);
}

void ParserReduceHandler::handleIfBeforeScope(NodePtr expression)
{
	_nodeCaster.castExpression(expression)->assertBool();

	openScope();
}

void ParserReduceHandler::handleWhileBeforeScope(NodePtr expression)
{
	_nodeCaster.castExpression(expression)->assertBool();

	_loopsScopesCounter++;

	openScope();
}

void ParserReduceHandler::handleWhileAfterScope()
{
	_loopsScopesCounter--;

	closeScope();	
}

void ParserReduceHandler::reduceBreak()
{
	if (_loopsScopesCounter <= 0)
	{
		output::errorUnexpectedBreak(yylineno);
		exit(1);
	}
}

void ParserReduceHandler::reduceContinue()
{
	if (_loopsScopesCounter <= 0)
	{
		output::errorUnexpectedContinue(yylineno);
		exit(1);
	}
}

void ParserReduceHandler::reduceCall(NodePtr& call, NodePtr functionId, NodePtr expressionList)
{
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(functionId);

	const string& functionIdString = identifierNode->getValue();
	const int functionIdLineNumber = identifierNode->getLineNumber();

	_symbolTableAsserter.assertFunctionDefined(functionIdString, functionIdLineNumber);

	FunctionEntryPtr functionEntry = _functionsTable.find(functionIdString);

	_nodeCaster.castExpressionList(expressionList)->assertCall(functionIdLineNumber, functionIdString, functionEntry->getArgTypes());

	call = _callNodeFactory.create(functionIdLineNumber, functionEntry->getRetType());
}

void ParserReduceHandler::reduceNoArgumentsCall(NodePtr& call, NodePtr functionId)
{
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(functionId);

	const string& functionIdString = identifierNode->getValue();
	const int functionIdLineNumber = identifierNode->getLineNumber();

	_symbolTableAsserter.assertFunctionDefined(functionIdString, functionIdLineNumber);

	FunctionEntryPtr functionEntry = _functionsTable.find(functionIdString);

	ExpressionListNode::assertNoArgumentsCall(functionIdLineNumber, functionIdString, functionEntry->getArgTypes());

	call = _callNodeFactory.create(functionIdLineNumber, functionEntry->getRetType());
}

void ParserReduceHandler::reduceExpressionList(NodePtr& expressionList, NodePtr expression)
{
	ExpressionNodePtr expressionNode = _nodeCaster.castExpression(expression);
	
	expressionList = _expressionNodeFactory.createList(expressionNode);
}

void ParserReduceHandler::reduceExpressionList(NodePtr& expressionList, NodePtr expression, NodePtr subExpressionList)
{
	ExpressionNodePtr expressionNode = _nodeCaster.castExpression(expression);
	
	ExpressionListNodePtr expressionListNode = _nodeCaster.castExpressionList(subExpressionList);
	
	expressionListNode->pushFront(expressionNode);

	expressionList = expressionListNode;
}

void ParserReduceHandler::reduceIntType(NodePtr& type)
{
	type = _typeNodeFactory.createInt(yylineno);
}

void ParserReduceHandler::reduceByteType(NodePtr& type)
{
	type = _typeNodeFactory.createByte(yylineno);
}

void ParserReduceHandler::reduceBoolType(NodePtr& type)
{
	type = _typeNodeFactory.createBool(yylineno);
}

void ParserReduceHandler::reduceInParenthesesExpression(NodePtr& expression, NodePtr expressionInParentheses)
{
	expression = expressionInParentheses;
}

void ParserReduceHandler::reduceBinopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs)
{
	ExpressionNodePtr lhsNode = _nodeCaster.castExpression(lhs);
	ExpressionNodePtr rhsNode = _nodeCaster.castExpression(rhs);

	expression = _expressionNodeFactory.createNumericBinop(lhsNode, rhsNode);
}

void ParserReduceHandler::reduceVariableExpression(NodePtr& expression, NodePtr variableId)
{
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();
	const int variableIdLineNumber = identifierNode->getLineNumber();

	_symbolTableAsserter.assertVariableDefined(variableIdString, variableIdLineNumber);

	VariableEntryPtr variableEntry = _variablesTable.find(variableIdString);

	expression = _expressionNodeFactory.create(variableIdLineNumber, variableEntry->getType());
}

void ParserReduceHandler::reduceCallExpression(NodePtr& expression, NodePtr call)
{
	CallNodePtr callNode = _nodeCaster.castCall(call);

	expression = _expressionNodeFactory.create(callNode->getLineNumber(), callNode->getType());
}

void ParserReduceHandler::reduceNumExpression(NodePtr& expression, NodePtr num)
{
	expression = _expressionNodeFactory.createInt(num->getLineNumber());
}

void ParserReduceHandler::reduceByteNumExpression(NodePtr& expression, NodePtr byteNum)
{
	NumNodePtr numNode = _nodeCaster.castNum(byteNum);
	numNode->assertByteValueNotTooLarge();
	
	expression = _expressionNodeFactory.createByte(numNode->getLineNumber());
}

void ParserReduceHandler::reduceStringExpression(NodePtr& expression)
{
	expression = _expressionNodeFactory.createString(yylineno);
}

void ParserReduceHandler::reduceTrueExpression(NodePtr& expression)
{
	expression = _expressionNodeFactory.createBool(yylineno);
}

void ParserReduceHandler::reduceFalseExpression(NodePtr& expression)
{
	expression = _expressionNodeFactory.createBool(yylineno);
}

void ParserReduceHandler::reduceNotExpression(NodePtr& expression, NodePtr operandExpression)
{
	ExpressionNodePtr operandExpressionNode = _nodeCaster.castExpression(operandExpression);
	operandExpressionNode->assertBool();
	
	expression = _expressionNodeFactory.createBool(operandExpressionNode->getLineNumber());
}

void ParserReduceHandler::reduceLogicalBinopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs)
{
	ExpressionNodePtr lhsNode = _nodeCaster.castExpression(lhs);
	ExpressionNodePtr rhsNode = _nodeCaster.castExpression(rhs);

	expression = _expressionNodeFactory.createLogicalBinop(lhsNode, rhsNode);
}

void ParserReduceHandler::reduceRelopExpression(NodePtr& expression, NodePtr lhs, NodePtr rhs)
{
	ExpressionNodePtr lhsNode = _nodeCaster.castExpression(lhs);
	ExpressionNodePtr rhsNode = _nodeCaster.castExpression(rhs);

	expression = _expressionNodeFactory.createRelop(lhsNode, rhsNode);
}
