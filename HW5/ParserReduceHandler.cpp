#include "ParserReduceHandler.hpp"
#include "hw3_output.hpp"
#include "bp.hpp"
#include <iostream>

using namespace std;

extern int yylineno;

const string PRINT_STRING_SPECIFIER = "str_specifier";
const string PRINTI_STRING_SPECIFIER = "int_specifier";

const string STATIC_IS_DEFINED_SUFFIX = "_isDefined";

const string DEVISION_BY_ZERO_STRING_VAR = "div_by_zero_error";

const string STACK_POINTER_NAME = "stack_array_ptr";

const string DEVISION_BY_ZERO_STRING = "Error division by zero";

const int VARIABLES_ARRAY_SIZE = 50;

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

	CodeBuffer::instance().printGlobalBuffer();
	cout << endl;
	CodeBuffer::instance().printCodeBuffer();
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

	const string typeString = (retTType == T_VOID ? "void" : "i32");

	string argListAtring = "";
	for (int i = 0; i < argTypes.size(); i++)
	{
		if (!argListAtring.empty())
		{
			argListAtring += ", ";
		}

		argListAtring += getTypedVarString(argTypes[i], "arg" + to_string(i), -1);
	}

	CodeBuffer::instance().emit("define " + typeString + " @" + functionIdString + "(" + argListAtring + ") {");

	emitWithIndent("%" + STACK_POINTER_NAME + " = alloca " + createArrayType(VARIABLES_ARRAY_SIZE, 32));

	for (int i = 0; i < argTypes.size(); i++)
	{	
		storeVarialbe(i, "i32 %arg" + to_string(i));
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

void ParserReduceHandler::closeFunctionScope()
{
	closeScope();

	const string retOperandString = (_functionScopeRetType == T_VOID ? "void" : "i32 0");

 	emitWithIndent("ret " + retOperandString);
	CodeBuffer::instance().emit("}\n");
}

void ParserReduceHandler::reduceVariableDeclarationStatement(NodePtr variableType, NodePtr variableId)
{
	const TType variableTType = _nodeCaster.castType(variableType)->getType();
	
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();

	_symbolTableAsserter.assertIdentifierUndefined(variableIdString, identifierNode->getLineNumber());

	VariableEntryPtr variableEntry = _variablesTable.add(variableIdString, variableTType);
	
	storeVarialbe(variableEntry->getOffset(), "i32 0");
}

void ParserReduceHandler::reduceAssignedVariableDeclarationStatement(NodePtr variableType, NodePtr variableId, NodePtr assignExpression)
{
	const TType variableTType = _nodeCaster.castType(variableType)->getType();
	
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();

	_symbolTableAsserter.assertIdentifierUndefined(variableIdString, identifierNode->getLineNumber());

	ExpressionNodePtr expressionNode = _nodeCaster.castExpression(assignExpression);
	expressionNode->assertAssignAllowed(variableTType);

	VariableEntryPtr variableEntry = _variablesTable.add(variableIdString, variableTType);
	
	assignToVariable(variableEntry->getOffset(), expressionNode);
}

void ParserReduceHandler::reduceStaticVariableDeclarationStatement(NodePtr variableType, NodePtr variableId)
{
	const TType variableTType = _nodeCaster.castType(variableType)->getType();
	
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();

	_symbolTableAsserter.assertIdentifierUndefined(variableIdString, identifierNode->getLineNumber());

	const string staticVarPlace = _freshVariableFactory.createFreshVariable();
	const string staticVarIsDefinedPlace = staticVarPlace + STATIC_IS_DEFINED_SUFFIX;

	_variablesTable.addStatic(variableIdString, variableTType, staticVarPlace);
	
	CodeBuffer::instance().emitGlobal("@_" + staticVarPlace + " = internal global i32 0");
	CodeBuffer::instance().emitGlobal("@_" + staticVarIsDefinedPlace + " = internal global i32 1");
}

void ParserReduceHandler::reduceStaticAssignedVariableDeclarationStatement(NodePtr variableType, NodePtr variableId, NodePtr assignExpression)
{
	const TType variableTType = _nodeCaster.castType(variableType)->getType();
	
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();

	_symbolTableAsserter.assertIdentifierUndefined(variableIdString, identifierNode->getLineNumber());

	ExpressionNodePtr expressionNode = _nodeCaster.castExpression(assignExpression);
	expressionNode->assertAssignAllowed(variableTType);

	const string staticVarPlace = _freshVariableFactory.createFreshVariable();
	const string staticVarIsDefinedPlace = staticVarPlace + STATIC_IS_DEFINED_SUFFIX;

	_variablesTable.addStatic(variableIdString, variableTType, staticVarPlace);
	
	const string typedVarString = handleExtractVarFromExpression(expressionNode);

	CodeBuffer::instance().emitGlobal("@_" + staticVarPlace + " = internal global i32 0");
	CodeBuffer::instance().emitGlobal("@_" + staticVarIsDefinedPlace + " = internal global i32 0");

	const string tempVar = _freshVariableFactory.createFreshVariable();

	emitWithIndent("%" + tempVar + " = load i32, i32* @_" + staticVarIsDefinedPlace);

	const string eqResVar = _freshVariableFactory.createFreshVariable();

	emitWithIndent("%" + eqResVar + " = icmp eq i32 %" + tempVar + ", 0");
	const int condBranchAddress = emitConditionalBranch(eqResVar);

	CodeBuffer::instance().bpatch(CodeBuffer::makelist(make_pair(condBranchAddress, FIRST)), 
								  CodeBuffer::instance().genLabel());

	emitWithIndent("store " + typedVarString + ", i32* @_" + staticVarPlace);
	emitWithIndent("store i32 1, i32* @_" + staticVarIsDefinedPlace);

	const int uncondBranchAddress = emitUnconditionalBranch();;

	CodeBuffer::instance().bpatch({ make_pair(condBranchAddress, SECOND),
									make_pair(uncondBranchAddress, FIRST) }, CodeBuffer::instance().genLabel());
}

void ParserReduceHandler::reduceAssignedVariableStatement(NodePtr variableId, NodePtr assignExpression)
{
	IdentifierNodePtr identifierNode = _nodeCaster.castIdentifier(variableId);

	const string& variableIdString = identifierNode->getValue();

	_symbolTableAsserter.assertVariableDefined(variableIdString, identifierNode->getLineNumber());

	ExpressionNodePtr expressionNode = _nodeCaster.castExpression(assignExpression);

	VariableEntryPtr variableEntry = _variablesTable.find(variableIdString);

	if (variableEntry != nullptr)
	{
		expressionNode->assertAssignAllowed(variableEntry->getType());

		assignToVariable(variableEntry->getOffset(), expressionNode);

		return;
	}

	StaticVariableEntryPtr staticVarEntry = _variablesTable.findStatic(variableIdString);

	expressionNode->assertAssignAllowed(staticVarEntry->getType());
	
	const string typedVarString = handleExtractVarFromExpression(expressionNode);

	emitWithIndent("store " + typedVarString + ", i32* @_" + staticVarEntry->getPlace());
}

void ParserReduceHandler::reduceVoidReturn()
{
	if (_functionScopeRetType != T_VOID)
	{
		output::errorMismatch(yylineno);
		exit(1);
	}

	emitWithIndent("ret void");
}

void ParserReduceHandler::reduceReturn(NodePtr returnExpression)
{
	ExpressionNodePtr expressionNode = _nodeCaster.castExpression(returnExpression);
	expressionNode->assertAssignAllowed(_functionScopeRetType);

	emitWithIndent("ret " + handleExtractVarFromExpression(expressionNode));
}

void ParserReduceHandler::handleIfBeforeScope(NodePtr& ifFalseBackPatch, NodePtr expression)
{
	BooleanExpressionNodePtr booleanExpression = _nodeCaster.castBooleanExpression(expression);

	CodeBuffer::instance().bpatch(booleanExpression->getTrueList(), CodeBuffer::instance().genLabel());

	ifFalseBackPatch = make_shared<BackPatchNode>(expression->getLineNumber(), booleanExpression->getFalseList());

	openScope();
}

void ParserReduceHandler::reduceIfWithoutElse(NodePtr ifFalseBackPatch)
{
	closeScope();

	vector<BPItem> backPatchItemsAfterIf = _nodeCaster.castBackPatch(ifFalseBackPatch)->getItems();
	
	backPatchItemsAfterIf.emplace_back(emitUnconditionalBranch(), FIRST);
		
	CodeBuffer::instance().bpatch(backPatchItemsAfterIf, CodeBuffer::instance().genLabel());	
}

void ParserReduceHandler::handleBetweenIfAndElse(NodePtr& afterElseBackPatch, NodePtr ifFalseBackPatch)
{
	closeScope();

	afterElseBackPatch = make_shared<BackPatchNode>(yylineno, CodeBuffer::makelist(make_pair(emitUnconditionalBranch(), FIRST)));

	BackPatchNodePtr ifFalseBackPatchNode = _nodeCaster.castBackPatch(ifFalseBackPatch);

	CodeBuffer::instance().bpatch(ifFalseBackPatchNode->getItems(), CodeBuffer::instance().genLabel());

	openScope();
}

void ParserReduceHandler::reduceIfWithElse(NodePtr afterElseBackPatch)
{
	closeScope();

	vector<BPItem> backPatchItemsAfterElse = _nodeCaster.castBackPatch(afterElseBackPatch)->getItems();
	
	backPatchItemsAfterElse.emplace_back(emitUnconditionalBranch(), FIRST);
		
	CodeBuffer::instance().bpatch(backPatchItemsAfterElse, CodeBuffer::instance().genLabel());	
}

void ParserReduceHandler::handleWhileBeforeScope(NodePtr& whileFalseBackPatch, NodePtr beforeExpressionMarker, NodePtr expression)
{
	BooleanExpressionNodePtr booleanExpression = _nodeCaster.castBooleanExpression(expression);

	MarkerNodePtr beforeExpressionMarkerNode = _nodeCaster.castMarker(beforeExpressionMarker);

	_loopConditionLabelStack.push(beforeExpressionMarkerNode->getLabel());
	
	_afterLoopAddressListStack.push({});

	CodeBuffer::instance().bpatch(booleanExpression->getTrueList(), CodeBuffer::instance().genLabel());

	whileFalseBackPatch = make_shared<BackPatchNode>(expression->getLineNumber(), booleanExpression->getFalseList());
	openScope();

}

void ParserReduceHandler::reduceWhileWithoutElse(NodePtr whileFalseBackPatch)
{
	closeScope();

	vector<BPItem> backPatchItemsAfterWhile = CodeBuffer::merge(_nodeCaster.castBackPatch(whileFalseBackPatch)->getItems(), _afterLoopAddressListStack.top());

	_afterLoopAddressListStack.pop();

	CodeBuffer::instance().bpatch(CodeBuffer::makelist(make_pair(emitUnconditionalBranch(), FIRST)), _loopConditionLabelStack.top());
		
	_loopConditionLabelStack.pop();

	CodeBuffer::instance().bpatch(backPatchItemsAfterWhile, CodeBuffer::instance().genLabel());	
}

void ParserReduceHandler::handleBetweenWhileAndElse(NodePtr& afterElseBackPatch, NodePtr whileFalseBackPatch)
{
	closeScope();

	afterElseBackPatch = make_shared<BackPatchNode>(yylineno, _afterLoopAddressListStack.top());

	_afterLoopAddressListStack.pop();

	CodeBuffer::instance().bpatch(CodeBuffer::makelist(make_pair(emitUnconditionalBranch(), FIRST)), _loopConditionLabelStack.top());
		
	_loopConditionLabelStack.pop();

	BackPatchNodePtr whileFalseBackPatchNode = _nodeCaster.castBackPatch(whileFalseBackPatch);

	CodeBuffer::instance().bpatch(whileFalseBackPatchNode->getItems(), CodeBuffer::instance().genLabel());

	openScope();
}

void ParserReduceHandler::reduceWhileWithElse(NodePtr afterElseBackPatch)
{
	closeScope();

	vector<BPItem> backPatchItemsAfterElse = _nodeCaster.castBackPatch(afterElseBackPatch)->getItems();
	
	backPatchItemsAfterElse.emplace_back(emitUnconditionalBranch(), FIRST);
		
	CodeBuffer::instance().bpatch(backPatchItemsAfterElse, CodeBuffer::instance().genLabel());	
}

void ParserReduceHandler::reduceBreak()
{
	if (_loopConditionLabelStack.empty() || _afterLoopAddressListStack.empty())
	{
		output::errorUnexpectedBreak(yylineno);
		exit(1);
	}

	_afterLoopAddressListStack.top().emplace_back(emitUnconditionalBranch(), FIRST);
}

void ParserReduceHandler::reduceContinue()
{
	if (_loopConditionLabelStack.empty() || _afterLoopAddressListStack.empty())
	{
		output::errorUnexpectedContinue(yylineno);
		exit(1);
	}

	emitWithIndent("br label %" + _loopConditionLabelStack.top());
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
		
		for (const auto& argExpression : expressionListNode->getExpressionList())
		{
			if (!argListString.empty())
			{
				argListString += ", ";
			}

			argListString += argExpression->getArgumentString();
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

		emitWithIndent("call void @" + functionEntry->getId() + "(" + argListString + ")");
	}
	else
	{
		const string callVar = _freshVariableFactory.createFreshVariable();

		call = make_shared<CallNode>(lineNumber, retType, callVar);

		emitWithIndent("%" + callVar + " = call i32 @" + functionEntry->getId() + "(" + argListString + ")");
	}
}

void ParserReduceHandler::handleExpressionInList(NodePtr expression)
{
	ExpressionNodePtr expressionNode = _nodeCaster.castExpression(expression);
	
	expressionNode->setArgumentString(handleExtractVarFromExpression(expressionNode));
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

	if (binopStr == "sdiv")
	{
		handleExitIfDevidedByZero(rhsNode->getVariableName());
	}

	const string initialVar = _freshVariableFactory.createFreshVariable();

	const TType resType = (((lhsNode->getType() == T_INT) || (rhsNode->getType() == T_INT)) ? T_INT : T_BYTE);

	emitWithIndent("%" + initialVar + " = " + binopStr + " i32 %" +
				   lhsNode->getVariableName() + ", %" + rhsNode->getVariableName());

	string finalVar = initialVar;

	if (resType == T_BYTE)
	{
		const string truncatedVar = _freshVariableFactory.createFreshVariable();

		emitWithIndent("%" + truncatedVar + " = trunc i32 %" + initialVar + " to i8");

		const string zextedVar = _freshVariableFactory.createFreshVariable();

		emitWithIndent("%" + zextedVar + " = zext i8 %" + truncatedVar + " to i32");

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

	const string tempVar = _freshVariableFactory.createFreshVariable();

	TType varType = T_VOID;

	VariableEntryPtr variableEntry = _variablesTable.find(variableIdString);

	if (variableEntry != nullptr)
	{
		const string tempPtr = _freshVariableFactory.createFreshVariable();

		emitWithIndent("%" + tempPtr + " = " + createStackVariablesGetElementPointer(variableEntry->getOffset()));
		emitWithIndent("%" + tempVar + " = load i32, i32* %" + tempPtr);	

		varType = variableEntry->getType();
	}
	else
	{
		StaticVariableEntryPtr staticVarEntry = _variablesTable.findStatic(variableIdString);

		emitWithIndent("%" + tempVar + " = load i32, i32* @_" + staticVarEntry->getPlace());

		varType = staticVarEntry->getType();
	}

	switch (varType)
	{
	case T_INT:
	case T_BYTE:
		expression = _expressionNodeFactory.createNumeric(lineNumber, varType, tempVar);
		break;

	case T_BOOL:
		expression = createBranchBooleanExpression(lineNumber, truncateBoolVar(tempVar));
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

		expression = createBranchBooleanExpression(lineNumber, truncateBoolVar(callRetVar));
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

	emitWithIndent("%" + varName + " = add i32 0, " + intValueString);
}

void ParserReduceHandler::reduceByteNumExpression(NodePtr& expression, NodePtr byteNum)
{
	NumNodePtr numNode = _nodeCaster.castNum(byteNum);
	numNode->assertByteValueNotTooLarge();
	
	const string varName = _freshVariableFactory.createFreshVariable();

	expression = _expressionNodeFactory.createByte(numNode->getLineNumber(), varName);

	const string intValueString = to_string(numNode->getValue());

	emitWithIndent("%" + varName + " = add i32 0, " + intValueString);
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

	emitWithIndent("%" + relopResultVar + " = icmp " + relopString + " i32 %" +
				   lhsNode->getVariableName() + ", %" + rhsNode->getVariableName());

	expression = createBranchBooleanExpression(lhsNode->getLineNumber(), relopResultVar);
}

void ParserReduceHandler::reduceMarker(NodePtr& marker)
{
	const int branchAddress = emitUnconditionalBranch();

	const string markerLabel =  CodeBuffer::instance().genLabel();

	CodeBuffer::instance().bpatch(CodeBuffer::makelist(make_pair(branchAddress, FIRST)), markerLabel);

	marker = make_shared<MarkerNode>(yylineno, markerLabel);
}

#pragma mark Private helper methods

void ParserReduceHandler::declareSystemCalls()
{
	CodeBuffer::instance().emit("declare i32 @printf(i8*, ...)");
	CodeBuffer::instance().emit("declare void @exit(i32)\n");
}

void ParserReduceHandler::defineDivisionByZeroErrorString()
{
	defineGlobalString(DEVISION_BY_ZERO_STRING, DEVISION_BY_ZERO_STRING_VAR);
}

void ParserReduceHandler::defineGlobalString(const std::string& stringValue, const std::string& stringName)
{
	defineGlobalString(stringValue, stringName, -1);
}

void ParserReduceHandler::defineGlobalString(const std::string& stringValue,
											 const std::string& stringName, int forcedNumBytes)
{
	const int numBytes = (forcedNumBytes == -1 ? (stringValue.length() + 1) : forcedNumBytes);

	CodeBuffer::instance().emitGlobal("@." + stringName + " = constant " + createArrayType(numBytes, 8) + " c\"" + stringValue + "\\00\"");
}

int ParserReduceHandler::emitConditionalBranch(const std::string& condVar)
{
	return emitWithIndent("br i1 %" + condVar + ", label @, label @");
}

int ParserReduceHandler::emitUnconditionalBranch()
{
	return emitWithIndent("br label @");
}

int ParserReduceHandler::emitWithIndent(const std::string& string)
{
	return CodeBuffer::instance().emit("\t" + string);
}

void ParserReduceHandler::emitComment(const std::string& comment)
{
	CodeBuffer::instance().emit("; " + comment);
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

	emitWithIndent("%" + boolVar + " = phi i32 [1, %" + trueLabel + "], [0, %" + falseLabel + "]");

	return boolVar;
}

BooleanExpressionNodePtr ParserReduceHandler::createBranchBooleanExpression(int lineNumber, const std::string& condVar)
{
	const int branchAddress = emitConditionalBranch(condVar);

	const auto trueList = CodeBuffer::makelist(make_pair(branchAddress, FIRST));
	const auto falseList = CodeBuffer::makelist(make_pair(branchAddress, SECOND));

	return _expressionNodeFactory.createBool(lineNumber, trueList, falseList);
}

std::string ParserReduceHandler::truncateBoolVar(const std::string& boolVar)
{
	const string truncatedVar = _freshVariableFactory.createFreshVariable();

	emitWithIndent("%" + truncatedVar + " = trunc i32 %" + boolVar + " to i1");

	return truncatedVar;
}

std::string ParserReduceHandler::handleExtractVarFromExpression(ExpressionNodePtr expression)
{
	const TType argType = expression->getType();

	switch (argType)
	{
	case T_INT:
	case T_BYTE:
	{
		NumericExpressionNodePtr numericExpression = _nodeCaster.castNumericExpression(expression);

		return getTypedVarString(argType, numericExpression->getVariableName(), -1);
	}

	case T_BOOL:
	{
		BooleanExpressionNodePtr booleanExpression = _nodeCaster.castBooleanExpression(expression);

		const string boolVar = handleGetBoolVarWithValue(booleanExpression);

		return getTypedVarString(argType, boolVar, -1);
	}

	case T_STRING:
	{
		StringExpressionNodePtr stringExpression = _nodeCaster.castStringExpression(expression);

		return getTypedVarString(argType, stringExpression->getVariableName(), stringExpression->getNumBytes());
	}

	case T_VOID:
		cout << "Error: argument is of void type" << endl;
		exit(1);
	}
}

std::string ParserReduceHandler::getTypedVarString(TType type, const std::string& varName, int numBytes)
{
	switch (type)
	{
	case T_INT:
	case T_BYTE:
	case T_BOOL:
		return "i32 %" + varName;

	case T_STRING:
		return createGlobalStringGetElementPointer(varName, numBytes);

	case T_VOID:
		return "void";
	}
}

void ParserReduceHandler::assignToVariable(int offset, ExpressionNodePtr assignExpression)
{	
	const string typedVarString = handleExtractVarFromExpression(assignExpression);

	storeVarialbe(offset, typedVarString);
}

void ParserReduceHandler::storeVarialbe(int offset, const std::string& typedVarString)
{
	const string tempPtr = _freshVariableFactory.createFreshVariable();

	emitWithIndent("%" + tempPtr + " = " + createStackVariablesGetElementPointer(offset));
	emitWithIndent("store " + typedVarString + ", i32* %" + tempPtr);	
}

void ParserReduceHandler::handleExitIfDevidedByZero(const std::string& rhsVar)
{
	const string tempVar = _freshVariableFactory.createFreshVariable();

	emitWithIndent("%" + tempVar + " = icmp eq i32 0, %" + rhsVar);
	
	const int branchAddress = emitConditionalBranch(tempVar);
	
	CodeBuffer::instance().bpatch(CodeBuffer::makelist(make_pair(branchAddress, FIRST)), CodeBuffer::instance().genLabel());

	emitWithIndent("call void @print(" + createGlobalStringGetElementPointer(DEVISION_BY_ZERO_STRING_VAR, DEVISION_BY_ZERO_STRING.length() + 1) + ")");
	emitWithIndent("call void @exit(i32 1)");
	emitWithIndent("unreachable");

	CodeBuffer::instance().bpatch(CodeBuffer::makelist(make_pair(branchAddress, SECOND)), CodeBuffer::instance().genLabel());
}

std::string ParserReduceHandler::createStackVariablesGetElementPointer(int index)
{
	const string stackArrayType = createArrayType(VARIABLES_ARRAY_SIZE, 32);

	return "getelementptr " + stackArrayType +  ", " + stackArrayType + "* %" +
			STACK_POINTER_NAME + ", i32 0, i32 " + to_string(index);
}

std::string ParserReduceHandler::createGlobalStringGetElementPointer(const std::string& stringName, int numBytes)
{
	const string arrayType = createArrayType(numBytes, 8);

	return "i8* getelementptr (" + arrayType + ", " + arrayType + "* @." + stringName + ", i32 0, i32 0)";
}

std::string ParserReduceHandler::createArrayType(int size, int numBits)
{
	if (size < 0)
	{
		cout << "Error: size of array is too small" << endl;
		exit(1);
	}

	return "[" + to_string(size) + " x i" + to_string(numBits) + "]";
}

void ParserReduceHandler::handleDefinePrintFunction()
{
	_functionsTable.add("print", T_VOID, { T_STRING });

	defineGlobalString("%s\\0A", PRINT_STRING_SPECIFIER, 4);

	auto& codeBuffer = CodeBuffer::instance();
	codeBuffer.emit("define void @print(i8*) {");
	codeBuffer.emit("	call i32 (i8*, ...) @printf(" +
					createGlobalStringGetElementPointer(PRINT_STRING_SPECIFIER, 4) + ", i8* %0)");
 	codeBuffer.emit("	ret void");
 	codeBuffer.emit("}\n");
}

void ParserReduceHandler::handleDefinePrintiFunction()
{
	_functionsTable.add("printi", T_VOID, { T_INT });

	defineGlobalString("%d\\0A", PRINTI_STRING_SPECIFIER, 4);

	auto& codeBuffer = CodeBuffer::instance();
	codeBuffer.emit("define void @printi(i32) {");
	codeBuffer.emit("	call i32 (i8*, ...) @printf(" +
					createGlobalStringGetElementPointer(PRINTI_STRING_SPECIFIER, 4) + ", i32 %0)");
	codeBuffer.emit("	ret void");
	codeBuffer.emit("}\n");
}
