#include "SymbolTable.hpp"
#include "hw3_output.hpp"
#include <iostream>

using namespace std;

VariablesTable::VariablesTable()
{
}

void VariablesTable::add(const std::string& id, TType type)
{
	assertScopesStackNotEmpty("add");

	auto entry = make_shared<VariableEntry>(id, type);

	_scopesStack.top()->add(entry);

	_varIdToEntryMap[id] = entry;
}

bool VariablesTable::contains(const std::string& id) const
{
	return (_varIdToEntryMap.find(id) != _varIdToEntryMap.end());
}

VariableEntryPtr VariablesTable::find(const std::string& id) const
{
	auto varIdToEntryMapIter = _varIdToEntryMap.find(id);

	return (varIdToEntryMapIter != _varIdToEntryMap.end() ? varIdToEntryMapIter->second : nullptr)
}

void VariablesTable::pushRegularScope()
{
	const int startingOffset = (_scopesStack.empty() ? 0 : _scopesStack.top()->getCurrentOffset());

	_scopesStack.push(make_shared<VariablesScope>(startingOffset));
}

void VariablesTable::pushFunctionScope(const std::vector<FunctionArgumentData>& arguments)
{
	_scopesStack.push(make_shared<VariablesScope>(arguments));
}

ConstVariablesScopePtr VariablesTable::popScope()
{
	assertScopesStackNotEmpty("pop");

	ConstVariablesScopePtr lastScope = _scopesStack.top();

	_scopesStack.pop();

	return lastScope;
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

VariablesScope::VariablesScope(const std::vector<FunctionArgumentData>& arguments) :
_startingOffset(-arguments.size())
{
	for (auto reverseIter = arguments.rbegin(); reverseIter != arguments.rend(); ++reverseIter)
	{
		add(make_shared<VariableEntry>(reverseIter->getId(), reverseIter->getType()));
	}
}

void VariablesScope::add(VariableEntryPtr entry)
{
	_entries.push_back(entry);
}

void VariablesScope::print() const
{
	int currOffset = _startingOffset;

	for (const VariableEntryPtr& entry : _entries)
	{
		entry->print(currOffset++);
	}
}

int VariablesScope::getCurrentOffset() const
{
	return _startingOffset + _entries.size();
}

GlobalScopeFunctionsTable::GlobalScopeSymbolTable()
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

void GlobalScopeFunctionsTable::print() const
{
	for (const FunctionEntryPtr& entry : _entries)
	{
		entry->print();
	}
}

bool GlobalScopeFunctionsTable::contains(const std::string& id) const
{
	return (_functionIdToEntryMap.find(id) != _functionIdToEntryMap.end());
}

SymbolEntry::SymbolEntry(const std::string& id) :
_id(id)
{
}

VariableEntry::VariableEntry(const std::string& id, TType varType) :
SymbolEntry(id),
_varType(varType)
{
}

TType VariableEntry::getType() const
{
	return _varType;
}

void VariableEntry::print(int offset) const
{
	output::printID(_id, offset, getTTypeString(_varType));
}

const int FunctionEntry::OFFSET = 0;

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

const std::std::vector<TType>& FunctionEntry::getArgTypes() const
{
	return _argTypes;
}

void FunctionEntry::print() const
{
	const string functionTypeString = output::makeFunctionType(_retType, getTTypesStrings(_argTypes));

	output::printID(_id, OFFSET, functionTypeString);
}
