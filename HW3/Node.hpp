#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "TType.h"

class Node;
typedef std::shared_ptr<Node> NodePtr;

class NumNode;
typedef std::shared_ptr<NumNode> NumNodePtr;

class IdentifierNode;
typedef std::shared_ptr<IdentifierNode> IdentifierNodePtr;

class TypeNode;
typedef std::shared_ptr<TypeNode> TypeNodePtr;

class ExpressionNode;
typedef std::shared_ptr<ExpressionNode> ExpressionNodePtr;

class ExpressionListNode;
typedef std::shared_ptr<ExpressionListNode> ExpressionListNodePtr;

class CallNode;
typedef std::shared_ptr<CallNode> CallNodePtr;

#define YYSTYPE NodePtr

class Node
{
public:
    Node(int lineNumber);
    virtual ~Node();

    int getLineNumber() const;

protected:
	const int _lineNumber;
};

class NumNode : public Node
{
public:
    NumNode(int lineNumber, int value);
    virtual ~NumNode();

    void assertByteValueNotTooLarge() const;

private:
	const int _value;
};

class IdentifierNode : public Node
{
public:
	IdentifierNode(int lineNumber, const std::string& value);
	virtual ~IdentifierNode();

	const std::string& getValue() const;

private:
	const std::string _value;
};

class TypedNode : public Node
{
public:
    virtual ~TypedNode();
    
    TType getType() const;

protected:
    TypedNode(int lineNumber, TType type);

    const TType _type;
};

class TypeNode : public TypedNode
{
public:
	TypeNode(int lineNumber, TType type);
    virtual ~TypeNode();
};

class ExpressionNode : public TypedNode
{
public:
    ExpressionNode(int lineNumber, TType type);
    virtual ~ExpressionNode();
    
	void assertBool() const;
    void assertNumeric() const;

    void assertAssignAllowed(TType type) const;

private:
	void exitWithMismatchError() const;
};

class ExpressionListNode : public Node
{
public:
	ExpressionListNode(ExpressionNodePtr first);
    virtual ~ExpressionListNode();

    void append(ExpressionNodePtr expression);

    void assertCall(int callLineNumber, const std::string& callId, const std::vector<TType>& argTypes) const;

private:
     std::function<void()> getExitWithPrototypeMismatchErrorClosure(
        int callLineNumber, const std::string& callId, const std::vector<TType>& argTypes
    ) const;

	std::vector<ExpressionNodePtr> _expressionList;
};

class CallNode : public TypedNode
{
public:
    CallNode(TType type, IdentifierNodePtr identifierNode);
    virtual ~CallNode();

    const std::string& getId() const;

private:
    IdentifierNodePtr _identifierNode;
};
