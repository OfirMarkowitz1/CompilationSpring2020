#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <stack>
#include <string>
#include "TType.h"
#include "FunctionArgumentData.hpp"

class VariablesScope;
typedef std::shared_ptr<VariablesScope> VariablesScopePtr;
typedef std::shared_ptr<const VariablesScope> ConstVariablesScopePtr;

class VariableEntry;
typedef std::shared_ptr<VariableEntry> VariableEntryPtr;

class FunctionEntry;
typedef std::shared_ptr<FunctionEntry> FunctionEntryPtr;


class VariablesTable
{
public:
	VariablesTable();

	void add(const std::string& id, TType type);

	VariableEntryPtr find(const std::string& id) const;

	void pushRegularScope();
	void pushFunctionScope(const std::vector<FunctionArgumentData>& arguments);

	ConstVariablesScopePtr popScope();

private:
	bool contains(const std::string& id) const;

	void assertScopesStackNotEmpty(const std::string& actionDescription) const;

	std::stack<VariablesScope> _scopesStack;
	
	std::unordered_map< std::string, VariableEntryPtr > _varIdToEntryMap;
};

class VariablesScope
{
public:
	VariablesScope(int startingOffset);
	VariablesScope(const std::vector<FunctionArgumentData>& arguments);

	void add(VariableEntryPtr entry);

	bool contains(const std::string& id) const;

	void print() const;

	int getCurrentOffset() const;	

private:
	std::vector<VariableEntryPtr> _entries;

	int _startingOffset;
};

class GlobalScopeFunctionsTable
{
public:
	GlobalScopeSymbolTable();

	void add(const std::string& id, TType retType, const std::vector<TType>& argTypes);

	bool contains(const std::string& id) const;

	FunctionEntryPtr find(const std::string& id) const;

	void print() const;

private:
	std::vector<FunctionEntryPtr> _entries;

	std::unordered_map< std::string, FunctionEntryPtr > _functionIdToEntryMap;
};

class SymbolEntry
{
protected:
	SymbolEntry(const std::string& id);

	const std::string _id
};

class VariableEntry : public SymbolEntry
{
public:
	VariableEntry(const std::string& id, TType varType);

	TType getType() const;

	void print() const;

private:
	const TType _varType;
};

class FunctionEntry : public SymbolEntry
{
public:
	FunctionEntry(const std::string& id, TType retType, const std::vector<TType>& argTypes);

	TType getRetType() const;

	const std::std::vector<TType>& getArgTypes() const;

	void print() const;

private:
	const TType _retType;
	const std::vector<TType> _argTypes;

	static const int OFFSET;
}
