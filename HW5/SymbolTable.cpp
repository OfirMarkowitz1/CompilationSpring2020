#include "SymbolTable.hpp"
#include <iostream>

using namespace std;

VariablesTable::VariablesTable()
{
}

VariableEntryPtr VariablesTable::add(const std::string& id, TType type)
{
	assertScopesStackNotEmpty("add");

	VariableEntryPtr entry = _scopesStack.top()->add(id, type);

	_varIdToEntryMap[id] = entry;

	return entry;
}

void VariablesTable::addStatic(const std::string& id, TType type, const std::string& place)
{
	assertScopesStackNotEmpty("add static");

	StaticVariableEntryPtr entry = _scopesStack.top()->addStatic(id, type, place);

	_staticVarIdToEntryMap[id] = entry;
}

bool VariablesTable::contains(const std::string& id) const
{
	return ((_varIdToEntryMap.find(id) != _varIdToEntryMap.end()) ||
			(_staticVarIdToEntryMap.find(id) != _staticVarIdToEntryMap.end()));
}

VariableEntryPtr VariablesTable::find(const std::string& id) const
{
	auto varIdToEntryMapIter = _varIdToEntryMap.find(id);

	return (varIdToEntryMapIter != _varIdToEntryMap.end() ? varIdToEntryMapIter->second : nullptr);
}

StaticVariableEntryPtr VariablesTable::findStatic(const std::string& id) const
{
	auto staticVarIdToEntryMapIter = _staticVarIdToEntryMap.find(id);

	return (staticVarIdToEntryMapIter != _staticVarIdToEntryMap.end() ? staticVarIdToEntryMapIter->second : nullptr);
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

	for (StaticVariableEntryPtr entry : lastScope->getStaticEntries())
	{
		_staticVarIdToEntryMap.erase(entry->getId());
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

StaticVariableEntryPtr VariablesScope::addStatic(const std::string& id, TType type, const std::string& place)
{
	auto entry = make_shared<StaticVariableEntry>(id, type, place);

	_staticEntries.push_back(entry);

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

const std::vector<StaticVariableEntryPtr> VariablesScope::getStaticEntries() const
{
	return _staticEntries;
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

StaticVariableEntry::StaticVariableEntry(const std::string& id, TType varType, const std::string& place) :
SymbolEntry(id),
_varType(varType),
_place(place)
{
}

TType StaticVariableEntry::getType() const
{
	return _varType;
}

const std::string& StaticVariableEntry::getPlace() const
{
	return _place;
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
