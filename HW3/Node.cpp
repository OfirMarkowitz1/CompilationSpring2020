#include "Node.hpp"
#include "hw3_output.hpp"
#include "FunctionArgumentData.hpp"

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

void NumNode::assertByteValueNotTooLarge() const
{
	if (_value > MAX_BYTE_VALUE)
	{
		output::errorByteTooLarge(_lineNumber, to_string(_value));
		exit(1);
	}
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

ExpressionNode::ExpressionNode(int lineNumber, TType type) :
TypedNode(lineNumber, type)
{
}

ExpressionNode::~ExpressionNode()
{
}

void ExpressionNode::assertBool() const
{
	if (_type != T_BOOL)
	{
		exitWithMismatchError();
	}
}

void ExpressionNode::assertNumeric() const
{
	if ((_type != T_INT) && (_type != T_BYTE))
	{
		exitWithMismatchError();
	}
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
	return (((type != T_INT) && (_type == type)) ||
			((type == T_INT) && ((_type == T_INT) || (_type == T_BYTE))));
}

void ExpressionNode::exitWithMismatchError() const
{
	output::errorMismatch(_lineNumber);
	exit(1);
}

ExpressionListNode::ExpressionListNode(ExpressionNodePtr first) :
Node(first->getLineNumber()),
_expressionList( {first} )
{
}

ExpressionListNode::~ExpressionListNode()
{
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
		if ((*expressionListIter)->isAssignAllowed(*argTypesIter))
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
		vector<string> typesStrings = getTTypesStrings(argTypes);
		output::errorPrototypeMismatch(lineNumber, callId, typesStrings);
		exit(1);
}

CallNode::CallNode(int lineNumber, TType type) :
TypedNode(lineNumber, type)
{
}

CallNode::~CallNode()
{
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
	_arguments.emplace_front(formalDeclarationNode->getId(), formalDeclarationNode->getType());
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
