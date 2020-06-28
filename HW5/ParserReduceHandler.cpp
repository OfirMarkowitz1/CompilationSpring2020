#include "ParserReduceHandler.hpp"
#include "hw3_output.hpp"
#include "bp.hpp"
#include <iostream>

using namespace std;

extern int yylineno;

const string DEVISION_BY_ZERO_STRING_VAR = "div_by_zero_error";

const string STACK_POINTER_NAME = "stack_array_ptr";

const string STACK_ARRAY_TYPE = "[50 x i32]";

ParserReduceHandler::ParserReduceHandler() :
_symbolTableAsserter(_variablesTable, _functionsTable)
{
	declareSystemCalls();

	defineDivisionByZeroErrorString();

	handleDefinePrintFunction();
 	handleDefinePrintiFunction();
}

void ParserReduceHandler::reduceProgram()
{
	_symbolTableAsserter.assertMainFunctionDefined();
}

void ParserReduceHandler::addFunctionAndOpenScope(NodePtr retType, NodePtr functionId, NodePtr formals)
{
	IdentifierNodePtr functionIdNode = _nodeCaster.castIdentifier(functionId);
	const string& functionIdString = functionIdNode->getValue();

	_symbolTableAsserter.assertIdentifierUndefined(functionIdString, functionIdNode->getLineNumber());

	const TType retTType = _nodeCaster.castReturnType(retType)->getType();
	
	FormalsNodePtr formalsNode = _nodeCaster.castFormals(formals);

	vector<TType> argTypes = formalsNode->getArgTypes();
	_functionsTable.add(functionIdString, retTType, argTypes);

	_variablesTable.pushFunctionScope(formalsNode->getArguments());

	_functionScopeRetType = retTType;

	CodeBuffer::instance().emit("%" + STACK_POINTER_NAME + " = alloca " + STACK_ARRAY_TYPE);

	for (TType argType : argTypes)
	{
		CodeBuffer::instance().emit("store i32 0, i32* %1");
	}
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

	_symbolTableAsserter.assertIdentifierUndefined(formalIdString, yylineno);

	formalDeclaration = _formalsNodeFactory.createFormalDecl(yylineno, formalTType, formalIdString);
}

void ParserReduceHandler::openScope()
{
	_variablesTable.pushRegularScope();
}

void ParserReduceHandler::closeScope()
{
	_variablesTable.popScope();
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

void ParserReduceHandler::handleWhileBeforeScope(NodePtr marker, NodePtr expression)
{
	BooleanExpressionNodePtr booleanExpression = _nodeCaster.castBooleanExpression(expression);

	MarkerNodePtr markerNode = _nodeCaster.castMarker(expression);

	_loopConditionLabelStack.push(markerNode->getLabel());

	openScope();

	const string boolVar = handleGetBoolVarWithValue(booleanExpression);
}

void ParserReduceHandler::handleWhileAfterScope()
{
	_loopConditionLabelStack.pop();

	closeScope();	
}

void ParserReduceHandler::handleWhileEnded()
{
	CodeBuffer::instance().bpatch(_loopNextAddressList, CodeBuffer::instance().genLabel());

	_loopNextAddressList.clear();
}

void ParserReduceHandler::reduceBreak()
{
	if (_loopConditionLabelStack.empty())
	{
		output::errorUnexpectedBreak(yylineno);
		exit(1);
	}

	_loopNextAddressList.emplace_back(emitUnconditionalBranch(), FIRST);
}

void ParserReduceHandler::reduceContinue()
{
	if (_loopConditionLabelStack.empty())
	{
		output::errorUnexpectedContinue(yylineno);
		exit(1);
	}

	CodeBuffer::instance().emit("br label %" + _loopConditionLabelStack.top());
}

void ParserReduceHandler::reduceCall(NodePtr& call, NodePtr functionId, NodePtr expressionList)
{
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(functionId);

	const string& functionIdString = identifierNode->getValue();
	const int lineNumber = identifierNode->getLineNumber();

	_symbolTableAsserter.assertFunctionDefined(functionIdString, lineNumber);

	FunctionEntryPtr functionEntry = _functionsTable.find(functionIdString);

	string argListString = "";
	if (expressionList != nullptr)
	{
		ExpressionListNodePtr expressionListNode = _nodeCaster.castExpressionList(expressionList);
		expressionListNode->assertCall(lineNumber, functionIdString, functionEntry->getArgTypes());

		for (const auto& expression : expressionListNode->getExpressionList())
		{
			if (!argListString.empty())
			{
				argListString += ", ";
			}

			switch (expression->getType())
			{
			case T_INT:
			case T_BYTE:
			{
				NumericExpressionNodePtr numericExpression = _nodeCaster.castNumericExpression(expression);
				
				argListString += "i32 %" + numericExpression->getVariableName();			
			} break;

			case T_BOOL:
			{
				BooleanExpressionNodePtr booleanExpression = _nodeCaster.castBooleanExpression(expression);

				const string boolVar = handleGetBoolVarWithValue(booleanExpression);

				argListString += "i32 %" + boolVar;
			} break;

			case T_STRING:
			{
				StringExpressionNodePtr springExpression = _nodeCaster.castStringExpression(expression);

				const string arrayType = "[" + to_string(springExpression->getNumBytes()) + " x i8]";

				argListString += "i8* getelementptr (" + arrayType + ", " + arrayType + "* @." +
								 springExpression->getVariableName() + ", i32 0, i32 0)";
			} break;

			case T_VOID:
				cout << "Error: argument is of void type" << endl;
				exit(1);
			}
		}
	}
	else
	{
		ExpressionListNode::assertNoArgumentsCall(lineNumber, functionIdString, functionEntry->getArgTypes());
	}

	const TType retType = functionEntry->getRetType();

	if (retType == T_VOID)
	{
		call = make_shared<CallNode>(lineNumber);

		CodeBuffer::instance().emit("call void @" + functionEntry->getId() + "(" + argListString + ")");
	}
	else
	{
		const string callVar = _freshVariableFactory.createFreshVariable();

		call = make_shared<CallNode>(lineNumber, retType, callVar);

		CodeBuffer::instance().emit("%" + callVar + " = call i32 @" + functionEntry->getId() + "(" + argListString + ")");
	}
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

void ParserReduceHandler::reduceBinopExpression(NodePtr& expression, NodePtr lhs, NodePtr binop, NodePtr rhs)
{
	NumericExpressionNodePtr lhsNode = _nodeCaster.castNumericExpression(lhs);
	NumericExpressionNodePtr rhsNode = _nodeCaster.castNumericExpression(rhs);

	BinopNodePtr binopNode = _nodeCaster.castBinop(binop);
		
	const string& binopStr = binopNode->getBinopString();

	const string initialVat = _freshVariableFactory.createFreshVariable();

	const TType resType = (((lhsNode->getType() == T_INT) || (rhsNode->getType() == T_INT)) ? T_INT : T_BYTE);

	CodeBuffer::instance().emit("%" + initialVat + " = " + binopStr + " i32 %" +
								lhsNode->getVariableName() + ", %" + rhsNode->getVariableName());

	string finalVar = initialVat;

	if (resType == T_BYTE)
	{
		const string truncatedVar = _freshVariableFactory.createFreshVariable();

		CodeBuffer::instance().emit("%" + truncatedVar + " = trunc i32 %" + initialVat + " to i8");

		const string zextedVar = _freshVariableFactory.createFreshVariable();

		CodeBuffer::instance().emit("%" + zextedVar + " = zext i8 %" + truncatedVar + " to i32");

		finalVar = zextedVar;
	}

	expression = _expressionNodeFactory.createNumeric(lhsNode->getLineNumber(), resType, finalVar);
}

void ParserReduceHandler::reduceVariableExpression(NodePtr& expression, NodePtr variableId)
{
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();
	const int lineNumber = identifierNode->getLineNumber();

	_symbolTableAsserter.assertVariableDefined(variableIdString, lineNumber);

	VariableEntryPtr variableEntry = _variablesTable.find(variableIdString);

	const string tempPtr = _freshVariableFactory.createFreshVariable();

	CodeBuffer::instance().emit("%" + tempPtr + " = getelementptr " + STACK_ARRAY_TYPE + 
								", " + STACK_ARRAY_TYPE + "* " + STACK_POINTER_NAME +
								", i32 0, i32 " + to_string(variableEntry->getOffset()));
		
	const string tempVar = _freshVariableFactory.createFreshVariable();

	CodeBuffer::instance().emit("%" + tempVar + " = load i32, i32* " + tempPtr);

	const TType varType = variableEntry->getType();

	switch (varType)
	{
	case T_INT:
	case T_BYTE:
		expression = _expressionNodeFactory.createNumeric(lineNumber, varType, tempVar);
		break;

	case T_BOOL:
		expression = createBranchBooleanExpression(lineNumber, tempVar);
		break;

	case T_VOID:
	case T_STRING:
		cout << "Error variable type is void or string" << endl;
		exit(1);
	}
}

void ParserReduceHandler::reduceCallExpression(NodePtr& expression, NodePtr call)
{
	CallNodePtr callNode = _nodeCaster.castCall(call);

	const int lineNumber = call->getLineNumber();
	const TType callRetType = callNode->getType();
	const string& callRetVar = callNode->getVariableName();

	switch (callRetType)
	{
	case T_INT:
	case T_BYTE:
		expression = _expressionNodeFactory.createNumeric(lineNumber, callRetType, callRetVar);
		break;

	case T_BOOL:
		expression = createBranchBooleanExpression(lineNumber, callRetVar);
		break;

	case T_VOID:
		output::errorMismatch(lineNumber);
		exit(1);

	case T_STRING:
		cout << "Error call return type is string" << endl;
		exit(1);
	}
}

void ParserReduceHandler::reduceNumExpression(NodePtr& expression, NodePtr num)
{
	NumNodePtr numNode = _nodeCaster.castNum(num);

	const string varName = _freshVariableFactory.createFreshVariable();

	expression = _expressionNodeFactory.createInt(numNode->getLineNumber(), varName);

	const string intValueString = to_string(numNode->getValue());

	CodeBuffer::instance().emit("%" + varName + " = add i32 0, " + intValueString);
}

void ParserReduceHandler::reduceByteNumExpression(NodePtr& expression, NodePtr byteNum)
{
	NumNodePtr numNode = _nodeCaster.castNum(byteNum);
	numNode->assertByteValueNotTooLarge();
	
	const string varName = _freshVariableFactory.createFreshVariable();

	expression = _expressionNodeFactory.createByte(numNode->getLineNumber(), varName);

	const string intValueString = to_string(numNode->getValue());

	CodeBuffer::instance().emit("%" + varName + " = add i32 0, " + intValueString);
}

void ParserReduceHandler::reduceStringExpression(NodePtr& expression, NodePtr stringBaseNode)
{
	StringNodePtr stringNode = _nodeCaster.castString(stringBaseNode);

	const string varName = _freshVariableFactory.createFreshVariable();

	const string stringValue = stringNode->getValue();
	const int numBytes = stringValue.length() + 1;

	expression = _expressionNodeFactory.createString(yylineno, varName, numBytes);

	defineGlobalString(stringValue, varName);
}

void ParserReduceHandler::reduceTrueExpression(NodePtr& expression)
{
	const int branchAddress = emitUnconditionalBranch();

	const auto trueList = CodeBuffer::makelist(make_pair(branchAddress, FIRST));

	expression = _expressionNodeFactory.createBool(yylineno, trueList, {});
}

void ParserReduceHandler::reduceFalseExpression(NodePtr& expression)
{
	const int branchAddress = emitUnconditionalBranch();

	const auto falseList = CodeBuffer::makelist(make_pair(branchAddress, FIRST));

	expression = _expressionNodeFactory.createBool(yylineno, {}, falseList);
}

void ParserReduceHandler::reduceNotExpression(NodePtr& expression, NodePtr operandExpression)
{
	BooleanExpressionNodePtr operandExpressionNode = _nodeCaster.castBooleanExpression(operandExpression);
		
	const auto& trueList = operandExpressionNode->getFalseList();
	const auto& falseList = operandExpressionNode->getTrueList();

	expression = _expressionNodeFactory.createBool(operandExpressionNode->getLineNumber(), trueList, falseList);
}

void ParserReduceHandler::reduceAndExpression(NodePtr& expression, NodePtr lhs, NodePtr marker, NodePtr rhs)
{
	BooleanExpressionNodePtr lhsNode = _nodeCaster.castBooleanExpression(lhs);
	BooleanExpressionNodePtr rhsNode = _nodeCaster.castBooleanExpression(rhs);

	MarkerNodePtr markerNode = _nodeCaster.castMarker(marker);

	CodeBuffer::instance().bpatch(lhsNode->getTrueList(), markerNode->getLabel());

	const auto& trueList = rhsNode->getTrueList();
	const auto falseList = CodeBuffer::merge(lhsNode->getFalseList(), rhsNode->getFalseList());

	expression = _expressionNodeFactory.createBool(lhsNode->getLineNumber(), trueList, falseList);
}

void ParserReduceHandler::reduceOrExpression(NodePtr& expression, NodePtr lhs, NodePtr marker, NodePtr rhs)
{
	BooleanExpressionNodePtr lhsNode = _nodeCaster.castBooleanExpression(lhs);
	BooleanExpressionNodePtr rhsNode = _nodeCaster.castBooleanExpression(rhs);

	MarkerNodePtr markerNode = _nodeCaster.castMarker(marker);
	
	CodeBuffer::instance().bpatch(lhsNode->getFalseList(), markerNode->getLabel());

	const auto trueList = CodeBuffer::merge(lhsNode->getTrueList(), rhsNode->getTrueList());
	const auto& falseList = rhsNode->getFalseList();

	expression = _expressionNodeFactory.createBool(lhsNode->getLineNumber(), trueList, falseList);
}

void ParserReduceHandler::reduceRelopExpression(NodePtr& expression, NodePtr lhs, NodePtr relop, NodePtr rhs)
{
	NumericExpressionNodePtr lhsNode = _nodeCaster.castNumericExpression(lhs);
	NumericExpressionNodePtr rhsNode = _nodeCaster.castNumericExpression(rhs);

	RelopNodePtr relopNode = _nodeCaster.castRelop(relop);

	const string relopString = relopNode->getRelopString();

	const string relopResultVar = _freshVariableFactory.createFreshVariable();

	CodeBuffer::instance().emit("%" + relopResultVar + " = icmp " + relopString + " i32 %" +
								lhsNode->getVariableName() + ", %" + rhsNode->getVariableName());

	expression = createBranchBooleanExpression(lhsNode->getLineNumber(), relopResultVar);
}

void ParserReduceHandler::reduceMarker(NodePtr& marker)
{
	marker = make_shared<MarkerNode>(yylineno, CodeBuffer::instance().genLabel());
}

#pragma mark Private helper methods

void ParserReduceHandler::declareSystemCalls()
{
	CodeBuffer::instance().emit("declare i32 @printf(i8*, ...)");
	CodeBuffer::instance().emit("declare void @exit(i32)");
}

void ParserReduceHandler::defineDivisionByZeroErrorString()
{
	defineGlobalString(DEVISION_BY_ZERO_STRING_VAR, "Error division by zero");
}

void ParserReduceHandler::defineGlobalString(const std::string& stringValue, const std::string& stringName)
{
	const string stringNumBytesStr = to_string(stringValue.length() + 1);

	CodeBuffer::instance().emitGlobal("@." + stringName + " = constant [" + stringNumBytesStr + "] x i8] c\"" + stringValue + "\\00\"");
}

int ParserReduceHandler::emitConditionalBranch(const std::string& condVar)
{
	return CodeBuffer::instance().emit("br i1 %" + condVar + ", label @, label @");
}

int ParserReduceHandler::emitUnconditionalBranch()
{
	return CodeBuffer::instance().emit("br label @");
}

std::string ParserReduceHandler::handleGetBoolVarWithValue(BooleanExpressionNodePtr booleanExpression)
{
	vector<BPItem> setBoolValueAddressList;

	const string trueLabel = CodeBuffer::instance().genLabel();
	CodeBuffer::instance().bpatch(booleanExpression->getTrueList(), trueLabel);
	setBoolValueAddressList.emplace_back(emitUnconditionalBranch(), FIRST);

	const string falseLabel = CodeBuffer::instance().genLabel();
	CodeBuffer::instance().bpatch(booleanExpression->getFalseList(), falseLabel);
	setBoolValueAddressList.emplace_back(emitUnconditionalBranch(), FIRST);

	CodeBuffer::instance().bpatch(setBoolValueAddressList, CodeBuffer::instance().genLabel());

	const string boolVar = _freshVariableFactory.createFreshVariable();

	CodeBuffer::instance().emit("%" + boolVar + " = phi i1 [1, %" + trueLabel +
								"], [0, %" + falseLabel + "]");

	return boolVar;
}

BooleanExpressionNodePtr ParserReduceHandler::createBranchBooleanExpression(int lineNumber, const std::string& condVar)
{
	const int branchAddress = emitConditionalBranch(condVar);

	const auto trueList = CodeBuffer::makelist(make_pair(branchAddress, FIRST));
	const auto falseList = CodeBuffer::makelist(make_pair(branchAddress, SECOND));

	return _expressionNodeFactory.createBool(lineNumber, trueList, falseList);
}

void ParserReduceHandler::handleDefinePrintFunction()
{
	_functionsTable.add("print", T_VOID, { T_STRING });

	defineGlobalString("str_specifier", "%s\\0A");

	auto& codeBuffer = CodeBuffer::instance();
	codeBuffer.emit("define void @print(i8*) {");
	codeBuffer.emit("	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)");
 	codeBuffer.emit("	ret void");
 	codeBuffer.emit("}");
}

void ParserReduceHandler::handleDefinePrintiFunction()
{
	_functionsTable.add("printi", T_VOID, { T_INT });

	defineGlobalString("int_specifier", "%d\\0A");

	auto& codeBuffer = CodeBuffer::instance();
	codeBuffer.emit("define void @printi(i32) {");
	codeBuffer.emit("	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)");
	codeBuffer.emit("	ret void");
	codeBuffer.emit("}");
}
