#include "SymbolTable.hpp"
#include <iostream>

using namespace std;

VariablesTable::VariablesTable()
{
}

void VariablesTable::add(const std::string& id, TType type)
{
	assertScopesStackNotEmpty("add");

	VariableEntryPtr entry = _scopesStack.top()->add(id, type);

	_varIdToEntryMap[id] = entry;
}

bool VariablesTable::contains(const std::string& id) const
{
	return (_varIdToEntryMap.find(id) != _varIdToEntryMap.end());
}

VariableEntryPtr VariablesTable::find(const std::string& id) const
{
	auto varIdToEntryMapIter = _varIdToEntryMap.find(id);

	return (varIdToEntryMapIter != _varIdToEntryMap.end() ? varIdToEntryMapIter->second : nullptr);
}

void VariablesTable::pushRegularScope()
{
	assertScopesStackNotEmpty("push non function scope");

	_scopesStack.push(make_shared<VariablesScope>(_scopesStack.top()->getCurrentOffset()));
}

void VariablesTable::pushFunctionScope(const std::vector<FunctionArgumentDataPtr>& arguments)
{
	_scopesStack.push(make_shared<VariablesScope>(0));

	for (const auto& argument : arguments)
	{
		add(argument->getId(), argument->getType());
	}
}

void VariablesTable::popScope()
{
	assertScopesStackNotEmpty("pop");

	VariablesScopePtr lastScope = _scopesStack.top();

	_scopesStack.pop();

	for (VariableEntryPtr entry : lastScope->getEntries())
	{
		_varIdToEntryMap.erase(entry->getId());
	}
}

void VariablesTable::assertScopesStackNotEmpty(const std::string& actionDescription) const
{
	if (_scopesStack.empty())
	{
		cout << "Error: scopes stack is empty while trying to " << actionDescription << endl;
		exit(1);
	}
}

VariablesScope::VariablesScope(int startingOffset) :
_startingOffset(startingOffset)
{
}

VariableEntryPtr VariablesScope::add(const std::string& id, TType type)
{
	auto entry = make_shared<VariableEntry>(id, type, getCurrentOffset());

	_entries.push_back(entry);

	return entry;
}

int VariablesScope::getCurrentOffset() const
{
	return _startingOffset + _entries.size();
}

const std::vector<VariableEntryPtr>& VariablesScope::getEntries() const
{
	return _entries;
}

void VariablesScope::assertFunctionScopeHasEnoughEntries() const
{
	if (_entries.size() < -_startingOffset)
	{
		cout << "Not enough entries scope" << endl;
		exit(1);
	}
}

GlobalScopeFunctionsTable::GlobalScopeFunctionsTable()
{
}

void GlobalScopeFunctionsTable::add(const std::string& id, TType retType, const std::vector<TType>& argTypes)
{
	auto entry = make_shared<FunctionEntry>(id, retType, argTypes);

	_entries.push_back(entry);

	_functionIdToEntryMap[id] = entry;
}

FunctionEntryPtr GlobalScopeFunctionsTable::find(const std::string& id) const
{
	auto functionIdToEntryMapIter = _functionIdToEntryMap.find(id);

	return (functionIdToEntryMapIter != _functionIdToEntryMap.end() ? functionIdToEntryMapIter->second : nullptr);
}

bool GlobalScopeFunctionsTable::contains(const std::string& id) const
{
	return (_functionIdToEntryMap.find(id) != _functionIdToEntryMap.end());
}

SymbolEntry::SymbolEntry(const std::string& id) :
_id(id)
{
}

const std::string& SymbolEntry::getId() const
{
	return _id;
}

VariableEntry::VariableEntry(const std::string& id, TType varType, int offset) :
SymbolEntry(id),
_varType(varType),
_offset(offset)
{
}

TType VariableEntry::getType() const
{
	return _varType;
}

int VariableEntry::getOffset() const
{
	return _offset;
}

FunctionEntry::FunctionEntry(const std::string& id, TType retType, const std::vector<TType>& argTypes) :
SymbolEntry(id),
_retType(retType),
_argTypes(argTypes)
{
}

TType FunctionEntry::getRetType() const
{
	return _retType;
}

const std::vector<TType>& FunctionEntry::getArgTypes() const
{
	return _argTypes;
}
