#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <stack>
#include <string>
#include "TType.hpp"
#include "FunctionArgumentData.hpp"

class VariablesScope;
typedef std::shared_ptr<VariablesScope> VariablesScopePtr;

class VariableEntry;
typedef std::shared_ptr<VariableEntry> VariableEntryPtr;

class FunctionEntry;
typedef std::shared_ptr<FunctionEntry> FunctionEntryPtr;


class VariablesTable
{
public:
	VariablesTable();

	VariableEntryPtr add(const std::string& id, TType type);

	VariableEntryPtr find(const std::string& id) const;

	bool contains(const std::string& id) const;

	void pushRegularScope();
	void pushFunctionScope(const std::vector<FunctionArgumentDataPtr>& arguments);

	void popScope();

private:

	void assertScopesStackNotEmpty(const std::string& actionDescription) const;

	std::stack<VariablesScopePtr> _scopesStack;
	
	std::unordered_map< std::string, VariableEntryPtr > _varIdToEntryMap;
};

class VariablesScope
{
public:
	VariablesScope(int startingOffset);

	VariableEntryPtr add(const std::string& id, TType type);

	int getCurrentOffset() const;	

	const std::vector<VariableEntryPtr>& getEntries() const;

private:

	void assertFunctionScopeHasEnoughEntries() const;

	std::vector<VariableEntryPtr> _entries;

	int _startingOffset;
};

class GlobalScopeFunctionsTable
{
public:
	GlobalScopeFunctionsTable();

	void add(const std::string& id, TType retType, const std::vector<TType>& argTypes);

	bool contains(const std::string& id) const;

	FunctionEntryPtr find(const std::string& id) const;

private:
	std::vector<FunctionEntryPtr> _entries;

	std::unordered_map< std::string, FunctionEntryPtr > _functionIdToEntryMap;
};

class SymbolEntry
{
public:
	const std::string& getId() const;

protected:
	SymbolEntry(const std::string& id);

    const std::string _id;
};

class VariableEntry : public SymbolEntry
{
public:
	VariableEntry(const std::string& id, TType varType, int offset);

	TType getType() const;

	int getOffset() const;

private:
	const TType _varType;
	const int _offset;
};

class FunctionEntry : public SymbolEntry
{
public:
	FunctionEntry(const std::string& id, TType retType, const std::vector<TType>& argTypes);

	TType getRetType() const;

	const std::vector<TType>& getArgTypes() const;

private:
	const TType _retType;
	const std::vector<TType> _argTypes;
};
