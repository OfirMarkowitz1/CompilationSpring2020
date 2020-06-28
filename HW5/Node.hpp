#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <utility>
#include "TType.hpp"
#include "FunctionArgumentData.hpp"
#include "bp.hpp"

class Node;
typedef std::shared_ptr<Node> NodePtr;

class NumNode;
typedef std::shared_ptr<NumNode> NumNodePtr;

class StringNode;
typedef std::shared_ptr<StringNode> StringNodePtr;

class IdentifierNode;
typedef std::shared_ptr<IdentifierNode> IdentifierNodePtr;

class TypeNode;
typedef std::shared_ptr<TypeNode> TypeNodePtr;

class ExpressionNode;
typedef std::shared_ptr<ExpressionNode> ExpressionNodePtr;

class NumericExpressionNode;
typedef std::shared_ptr<NumericExpressionNode> NumericExpressionNodePtr;

class BooleanExpressionNode;
typedef std::shared_ptr<BooleanExpressionNode> BooleanExpressionNodePtr;

class StringExpressionNode;
typedef std::shared_ptr<StringExpressionNode> StringExpressionNodePtr;

class ExpressionListNode;
typedef std::shared_ptr<ExpressionListNode> ExpressionListNodePtr;

class CallNode;
typedef std::shared_ptr<CallNode> CallNodePtr;

class FormalDeclarationNode;
typedef std::shared_ptr<FormalDeclarationNode> FormalDeclarationNodePtr;

class FormalsNode;
typedef std::shared_ptr<FormalsNode> FormalsNodePtr;

class BinopNode;
typedef std::shared_ptr<BinopNode> BinopNodePtr;

class RelopNode;
typedef std::shared_ptr<RelopNode> RelopNodePtr;

class MarkerNode;
typedef std::shared_ptr<MarkerNode> MarkerNodePtr;

typedef std::pair<int, BranchLabelIndex> BPItem;

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

    int getValue() const;

    void assertByteValueNotTooLarge() const;

private:
	const int _value;
};

class StringNode : public Node
{
public:
    StringNode(int lineNumber, const std::string& value);
    virtual ~StringNode();
    
    const std::string& getValue() const;

private:
    const std::string _value;
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

class ExpressionNode : public Node
{
public:
    ExpressionNode(int lineNumber);
    virtual ~ExpressionNode();
    
	virtual TType getType() const = 0;

    void assertAssignAllowed(TType type) const;

    bool isAssignAllowed(TType type) const;

private:

	void exitWithMismatchError() const;
};

class NumericExpressionNode : public ExpressionNode
{
public:
    NumericExpressionNode(int lineNumber, TType type, const std::string& variableName);
    virtual ~NumericExpressionNode();

    TType getType() const override;

    const std::string& getVariableName() const;

private:

    const TType _type;
    const std::string _variableName;
};

class BooleanExpressionNode : public ExpressionNode
{
public:
    BooleanExpressionNode(int lineNumber,
                          const std::vector<BPItem>& trueList,
                          const std::vector<BPItem>& falseList);

    virtual ~BooleanExpressionNode();

    TType getType() const override;

    const std::vector<BPItem>& getTrueList() const;
    const std::vector<BPItem>& getFalseList() const;

private:

    const std::vector<BPItem> _trueList;
    const std::vector<BPItem> _falseList;
};

class StringExpressionNode : public ExpressionNode
{
public:
    StringExpressionNode(int lineNumber, const std::string& variableName, int numBytes);
    virtual ~StringExpressionNode();

    TType getType() const override;

    const std::string& getVariableName() const;

    int getNumBytes() const;

private:

    const std::string _variableName;
    const int _numBytes;
};

class ExpressionListNode : public Node
{
public:
	ExpressionListNode(ExpressionNodePtr first);
    virtual ~ExpressionListNode();

    const std::list<ExpressionNodePtr>& getExpressionList() const;

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
    CallNode(int lineNumber);
    CallNode(int lineNumber, TType type, const std::string& variableName);
    virtual ~CallNode();

    const std::string& getVariableName() const;

private:

    const std::string _variableName;
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

class BinopNode : public Node
{
public:
    BinopNode(int lineNumber, char operatorChar);
    virtual ~BinopNode();

    const std::string& getBinopString() const;

private:

    static std::string createBinopString(char operatorChar);

    const std::string _binopString;
};

class RelopNode : public Node
{
public:
    RelopNode(int lineNumber, const std::string& operatorString);
    virtual ~RelopNode();

    const std::string& getRelopString() const;

private:

    static std::string createRelopString(const std::string& operatorString);

    const std::string _relopString;
};

class MarkerNode : public Node
{
public:
    MarkerNode(int lineNumber, const std::string& label);
    virtual ~MarkerNode();

    const std::string& getlabel() const;

private:

    const std::string _label;
};
