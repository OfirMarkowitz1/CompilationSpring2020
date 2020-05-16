#include "Node.hpp"
#include "hw3_output.hpp"

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
	if (((type != T_INT) && (_type != type)) ||
		((type == T_INT) && ((_type != T_INT) && (_type != T_BYTE))))
	{
		exitWithMismatchError();
	}
}

void ExpressionNode::exitWithMismatchError() const
{
	output::errorMismatch(_lineNumber);
	exit(1);
}

ExpressionListNode::ExpressionListNode(int lineNumber, ExpressionNodePtr first) :
Node(lineNumber),
_expressionList( {first} )
{
}

ExpressionListNode::~ExpressionListNode()
{
}

void ExpressionListNode::append(ExpressionNodePtr expression)
{
	_expressionList.push_back(expression);
}

CallNode::CallNode(TType returnType, IdentifierNodePtr identifier) :
TypedNode(identifier->getLineNumber(), returnType)
{
}

CallNode::~CallNode()
{
}
