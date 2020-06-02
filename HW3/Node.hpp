#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <set>
#include "TType.h"
#include "FunctionArgumentData.hpp"

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

class FormalDeclarationNode;
typedef std::shared_ptr<FormalDeclarationNode> FormalDeclarationNodePtr;

class FormalsNode;
typedef std::shared_ptr<FormalsNode> FormalsNodePtr;

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

    bool isAssignAllowed(TType type) const;

private:

	void exitWithMismatchError() const;
};

class ExpressionListNode : public Node
{
public:
	ExpressionListNode(ExpressionNodePtr first);
    virtual ~ExpressionListNode();

    void pushFront(ExpressionNodePtr expression);

    void assertCall(int callIdLineNumber, const std::string& callId, const std::vector<TType>& argTypes) const;

    static void assertNoArgumentsCall(int callIdLineNumber, const std::string& callId, const std::vector<TType>& argTypes);

private:
    static void exitWithPrototypeMismatchError(int lineNumber, const std::string& callId, const std::vector<TType>& argTypes);

	std::list<ExpressionNodePtr> _expressionList;
};

class CallNode : public TypedNode
{
public:
    CallNode(int lineNumber, TType type);
    virtual ~CallNode();
};

class FormalDeclarationNode : public TypedNode
{
public:
    FormalDeclarationNode(int lineNumber, TType type, const std::string& id);
    virtual ~FormalDeclarationNode();

    const std::string& getId() const;

private:
    const std::string _id;
};

class FormalsNode : public Node
{
public:
    FormalsNode(int lineNumber);
    virtual ~FormalsNode();

    void pushFront(FormalDeclarationNodePtr formalDeclarationNode);

    std::vector<FunctionArgumentDataPtr> getArguments() const;

    std::vector<TType> getArgTypes() const;

    void assertIdDoesNotExist(int lineNumber, const std::string& id) const;

private:

    std::list<FunctionArgumentDataPtr> _arguments;

    std::set<std::string> _argumentsIds;
};
