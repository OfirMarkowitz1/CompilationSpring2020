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

class StaticVariableEntry;
typedef std::shared_ptr<StaticVariableEntry> StaticVariableEntryPtr;

class FunctionEntry;
typedef std::shared_ptr<FunctionEntry> FunctionEntryPtr;


class VariablesTable
{
public:
	VariablesTable();

	VariableEntryPtr add(const std::string& id, TType type);
	void addStatic(const std::string& id, TType type, const std::string& place);

	VariableEntryPtr find(const std::string& id) const;
	StaticVariableEntryPtr findStatic(const std::string& id) const;

	bool contains(const std::string& id) const;

	void pushRegularScope();
	void pushFunctionScope(const std::vector<FunctionArgumentDataPtr>& arguments);

	void popScope();

private:

	void assertScopesStackNotEmpty(const std::string& actionDescription) const;

	std::stack<VariablesScopePtr> _scopesStack;
	
	std::unordered_map< std::string, VariableEntryPtr > _varIdToEntryMap;
	std::unordered_map< std::string, StaticVariableEntryPtr > _staticVarIdToEntryMap;
};

class VariablesScope
{
public:
	VariablesScope(int startingOffset);

	VariableEntryPtr add(const std::string& id, TType type);
	StaticVariableEntryPtr addStatic(const std::string& id, TType type, const std::string& place);

	int getCurrentOffset() const;	

	const std::vector<VariableEntryPtr>& getEntries() const;

	const std::vector<StaticVariableEntryPtr> getStaticEntries() const;

private:

	void assertFunctionScopeHasEnoughEntries() const;

	std::vector<VariableEntryPtr> _entries;

	int _startingOffset;

	std::vector<StaticVariableEntryPtr> _staticEntries;
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

class StaticVariableEntry : public SymbolEntry
{
public:
	StaticVariableEntry(const std::string& id, TType varType, const std::string& place);

	TType getType() const;

	const std::string& getPlace() const;

private:
	const TType _varType;
	const std::string _place;
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
