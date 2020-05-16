#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <stack>
#include <string>
#include "TType.h"
#include "FunctionArgumentData.hpp"

class ScopeSymbolTable;
typedef std::shared_ptr<ScopeSymbolTable> ScopeSymbolTablePtr;

class SymbolEntry;
typedef std::shared_ptr<SymbolEntry> SymbolEntryPtr;

class SymbolType;
typedef std::shared_ptr<SymbolType> SymbolTypePtr;

class SymbolTable
{
public:
	SymbolTable(const int& lineNumber);

	void add(const std::string& id, SymbolTypePtr type) const;

	void pushRegularScope();
	void pushFunctionScope(const std::vector<FunctionArgumentData>& arguments);

	void popScope();

	void assertDefinedVariable(const std::string& id) const;
	void assertDefinedFunction(const std::string& id) const;

	void assertNotDefined(const std::string& id) const;

private:
		
	void pushScope(const std::vector<FunctionArgumentData>& arguments);

	SymbolEntryPtr find(const std::string& id) const;

	bool contains(const std::string& id) const;

	void assertScopeTablesStackNotEmpty() const;

	std::stack<ScopeSymbolTablePtr> _scopeTablesStack;
	
	std::unordered_map< std::string, SymbolEntryPtr > _symbolIdToEntryMap;

	const int& _lineNumber;
};

class ScopeSymbolTable
{
public:
	ScopeSymbolTable(int startingOffset, const std::vector<FunctionArgumentData>& arguments = {});

	void add(const std::string& id, SymbolTypePtr type);

	const std::vector<SymbolEntryPtr>& getSymbolEntries() const;

	int getCurrentOffset() const;

private:
	std::vector<SymbolEntryPtr> _entries;
	int _currentOffset;
};

class SymbolEntry
{
public:
	SymbolEntry(const std::string& id, int offset, SymbolTypePtr type);

	const std::string& getId() const;
	SymbolTypePtr getType() const;

	void printEntry() const;

private:
	const std::string _id;
	const SymbolTypePtr _type;
	const int _offset
};

class SymbolType
{
public:
	virtual ~SymbolType();

	virtual bool isFunctionType() const = 0;

	virtual std::string toString() const = 0;

private:
	std::string getTTypeString(TType type) const;
};

class VariableSymbolType : public SymbolType
{
public:
	VariableSymbolType(TType type);
	virtual ~VariableSymbolType();

	bool isFunctionType() const override;

	std::string toString() const override;

private:
	const TType _type;
};

class FunctionSymbolType : public SymbolType
{
public:
	FunctionSymbolType(TType returnType, const std::vector<TType>& argumentTypes);
	virtual ~FunctionSymbolType();

	bool isFunctionType() const override;

	std::string toString() const override;

private:
	const TType _returnType;
	const std::vector<TType> _argumentTypes;
};
