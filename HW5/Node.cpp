#include "Node.hpp"
#include "hw3_output.hpp"
#include "FunctionArgumentData.hpp"
#include <iostream>

using namespace std;

static const int MAX_BYTE_VALUE = 255;

Node::Node(int lineNumber) :
_lineNumber(lineNumber)
{
}

Node::~Node()
{
}

int Node::getLineNumber() const
{
	return _lineNumber;
}

NumNode::NumNode(int lineNumber, int value) :
Node(lineNumber),
_value(value)
{
}

NumNode::~NumNode()
{
}

int NumNode::getValue() const
{
	return _value;
}

void NumNode::assertByteValueNotTooLarge() const
{
	if (_value > MAX_BYTE_VALUE)
	{
		output::errorByteTooLarge(_lineNumber, to_string(_value));
		exit(1);
	}
}

StringNode::StringNode(int lineNumber, const std::string& value) :
Node(lineNumber),
_value(value)
{
}

StringNode::~StringNode()
{
}

const std::string& StringNode::getValue() const
{
	return _value;
}

IdentifierNode::IdentifierNode(int lineNumber, const std::string& value) :
Node(lineNumber),
_value(value)
{
}

IdentifierNode::~IdentifierNode()
{
}

const std::string& IdentifierNode::getValue() const
{
	return _value;
}

TypedNode::TypedNode(int lineNumber, TType type) :
Node(lineNumber),
_type(type)
{
}

TypedNode::~TypedNode()
{
}
    
TType TypedNode::getType() const
{
	return _type;
}

TypeNode::TypeNode(int lineNumber, TType type) :
TypedNode(lineNumber, type)
{
}

TypeNode::~TypeNode()
{
}

ExpressionNode::ExpressionNode(int lineNumber) :
Node(lineNumber)
{
}

ExpressionNode::~ExpressionNode()
{
}

void ExpressionNode::assertAssignAllowed(TType type) const
{
	if (!isAssignAllowed(type))
	{
		exitWithMismatchError();
	}
}

bool ExpressionNode::isAssignAllowed(TType type) const
{
	const TType myType = getType();

	return (((type != T_INT) && (myType == type)) ||
			((type == T_INT) && ((myType == T_INT) || (myType == T_BYTE))));
}

void ExpressionNode::exitWithMismatchError() const
{
	output::errorMismatch(_lineNumber);
	exit(1);
}

NumericExpressionNode::NumericExpressionNode(int lineNumber, TType type, const std::string& variableName) :
ExpressionNode(lineNumber),
_type(type),
_variableName(variableName)
{
	if ((type != T_INT) && (type != T_BYTE))
	{
		cout << "Error: numeric expression is constructed with type mismatch" << endl;
		exit(1);
	}
}

NumericExpressionNode::~NumericExpressionNode()
{
}

TType NumericExpressionNode::getType() const
{
	return _type;
}

const std::string& NumericExpressionNode::getVariableName() const
{
	return _variableName;
}

BooleanExpressionNode::BooleanExpressionNode(int lineNumber,
                       					     const std::vector<BPItem>& trueList,
                          					 const std::vector<BPItem>& falseList) :
ExpressionNode(lineNumber),
_trueList(trueList),
_falseList(falseList)
{
}

BooleanExpressionNode::~BooleanExpressionNode()
{
}

TType BooleanExpressionNode::getType() const
{
	return T_BOOL;
}

const std::vector<BPItem>& BooleanExpressionNode::getTrueList() const
{
	return _trueList;
}

const std::vector<BPItem>& BooleanExpressionNode::getFalseList() const
{
	return _falseList;
}

StringExpressionNode::StringExpressionNode(int lineNumber, const std::string& variableName, int numBytes) :
ExpressionNode(lineNumber),
_variableName(variableName),
_numBytes(numBytes)
{
}

StringExpressionNode::~StringExpressionNode()
{
}

TType StringExpressionNode::getType() const
{
	return T_STRING;
}

const std::string& StringExpressionNode::getVariableName() const
{
	return _variableName;
}

int StringExpressionNode::getNumBytes() const
{
	return _numBytes;
}

ExpressionListNode::ExpressionListNode(ExpressionNodePtr first) :
Node(first->getLineNumber()),
_expressionList( {first} )
{
}

ExpressionListNode::~ExpressionListNode()
{
}

const std::list<ExpressionNodePtr>& ExpressionListNode::getExpressionList() const
{
	return _expressionList;
}

void ExpressionListNode::pushFront(ExpressionNodePtr expression)
{
	_expressionList.push_front(expression);
}

void ExpressionListNode::assertCall(int callIdLineNumber, const std::string& callId, const std::vector<TType>& argTypes) const
{
	if (_expressionList.size() != argTypes.size())
	{
		exitWithPrototypeMismatchError(callIdLineNumber, callId, argTypes);
	}

	auto expressionListIter = _expressionList.begin();
	auto argTypesIter = argTypes.begin();

	for (;(expressionListIter != _expressionList.end()) && (argTypesIter != argTypes.end());
		 ++expressionListIter, ++argTypesIter)
	{	
		if (!(*expressionListIter)->isAssignAllowed(*argTypesIter))
		{
			exitWithPrototypeMismatchError((*expressionListIter)->getLineNumber(), callId, argTypes);
		}	
	}
}

void ExpressionListNode::assertNoArgumentsCall(int callIdLineNumber, const std::string& callId, const std::vector<TType>& argTypes)
{
	if (!argTypes.empty())
	{
		exitWithPrototypeMismatchError(callIdLineNumber, callId, argTypes);
	}
}

void ExpressionListNode::exitWithPrototypeMismatchError(int lineNumber, const std::string& callId, const std::vector<TType>& argTypes)
{
		vector<string> typesStrings = TTypeUtils::getTTypesStrings(argTypes);
		output::errorPrototypeMismatch(lineNumber, callId, typesStrings);
		exit(1);
}

CallNode::CallNode(int lineNumber) :
TypedNode(lineNumber, T_VOID)
{
}

CallNode::CallNode(int lineNumber, TType type, const std::string& variableName) :
TypedNode(lineNumber, type),
_variableName(variableName)
{
}

CallNode::~CallNode()
{
}

const std::string& CallNode::getVariableName() const
{
	return _variableName;
}

FormalDeclarationNode::FormalDeclarationNode(int lineNumber, TType type, const std::string& id) :
TypedNode(lineNumber, type),
_id(id)
{
}

FormalDeclarationNode::~FormalDeclarationNode()
{
}

const std::string& FormalDeclarationNode::getId() const
{
	return _id;
}

FormalsNode::FormalsNode(int lineNumber) :
Node(lineNumber)
{
}

FormalsNode::~FormalsNode()
{
}

void FormalsNode::pushFront(FormalDeclarationNodePtr formalDeclarationNode)
{
	FunctionArgumentDataPtr argument = make_shared<FunctionArgumentData>(formalDeclarationNode->getId(), formalDeclarationNode->getType());
	
	_arguments.push_front(argument);

	_argumentsIds.insert(formalDeclarationNode->getId());
}

std::vector<FunctionArgumentDataPtr> FormalsNode::getArguments() const
{
	vector<FunctionArgumentDataPtr> argumentsVec;
	
	for (const auto& argument : _arguments)
	{
		argumentsVec.push_back(argument);
	}

	return argumentsVec;
}

std::vector<TType> FormalsNode::getArgTypes() const
{
	vector<TType> argTypes;

	for (const auto& argument : _arguments)
	{
		argTypes.push_back(argument->getType());
	}

	return argTypes;
}

void FormalsNode::assertIdDoesNotExist(int lineNumber, const std::string& id) const
{
	if (_argumentsIds.find(id) != _argumentsIds.end())
	{
		output::errorDef(lineNumber, id);
		exit(1);
	}
}

BinopNode::BinopNode(int lineNumber, char operatorChar) :
Node(lineNumber),
_binopString(createBinopString(operatorChar))
{
}

BinopNode::~BinopNode()
{
}

const std::string& BinopNode::getBinopString() const
{
	return _binopString;
}

std::string BinopNode::createBinopString(char operatorChar)
{
	switch (operatorChar)
	{
	case '+':
		return "add";
	case '-':
		return "sub";
	case '*':
		return "mul";
	case '/':
		return "sdiv";
	default:
		cout << "Error: unidentified binop - " << operatorChar << endl;
		exit(1);
	}
}

RelopNode::RelopNode(int lineNumber, const std::string& operatorString) :
Node(lineNumber),
_relopString(createRelopString(operatorString))
{
}

RelopNode::~RelopNode()
{
}

const std::string& RelopNode::getRelopString() const
{
	return _relopString;
}

std::string RelopNode::createRelopString(const std::string& operatorString)
{
	if (operatorString == "==")
	{
		return "eq";
	}
	
	if (operatorString == "!=")
	{
		return "ne";
	}
	
	if (operatorString == "<")
	{
		return "slt";
	}
	
	if (operatorString == ">")
	{
		return "sgt";
	}
	
	if (operatorString == "<=")
	{
		return "sle";
	}
	
	if (operatorString == ">=")
	{
		return "sge";
	}

	cout << "Error: unidentified relop - " << operatorString << endl;
	exit(1);
}

MarkerNode::MarkerNode(int lineNumber, const std::string& label) :
Node(lineNumber),
_label(label)
{
}

MarkerNode::~MarkerNode()
{
}

const std::string& MarkerNode::getlabel() const
{
	return _label;
}
