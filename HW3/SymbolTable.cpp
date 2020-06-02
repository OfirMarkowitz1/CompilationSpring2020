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

	return (varIdToEntryMapIter != _varIdToEntryMap.end() ? varIdToEntryMapIter->second : nullptr);
}

void VariablesTable::pushRegularScope()
{
	assertScopesStackNotEmpty("push non function scope");

	_scopesStack.push(make_shared<VariablesScope>(_scopesStack.top()->getCurrentOffset()));
}

void VariablesTable::pushFunctionScope(const std::vector<FunctionArgumentDataPtr>& arguments)
{
	_scopesStack.push(make_shared<VariablesScope>(-arguments.size()));

	for (const auto& argument : arguments)
	{
		add(argument->getId(), argument->getType());
	}
}

ConstVariablesScopePtr VariablesTable::popScope()
{
	assertScopesStackNotEmpty("pop");

	ConstVariablesScopePtr lastScope = _scopesStack.top();

	_scopesStack.pop();

	for (VariableEntryPtr entry : lastScope->getEntries())
	{
		_varIdToEntryMap.erase(entry->getId());
	}

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

void VariablesScope::add(VariableEntryPtr entry)
{
	_entries.push_back(entry);
}

void VariablesScope::print() const
{
	auto entriesIter = _entries.begin();

	if (_startingOffset < 0)
	{
		assertFunctionScopeHasEnoughEntries();
		
		for (int currNegativeOffset = -1; currNegativeOffset >= _startingOffset; --currNegativeOffset, ++entriesIter)
		{
			(*entriesIter)->print(currNegativeOffset);
		}

		for (int currOffset = 0; entriesIter != _entries.end(); ++currOffset, ++entriesIter)
		{
			(*entriesIter)->print(currOffset);
		}
	}
	else
	{
		for (int currOffset = _startingOffset; entriesIter != _entries.end(); ++currOffset, ++entriesIter)
		{
			(*entriesIter)->print(currOffset);
		}	
	}
	
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

const std::string& SymbolEntry::getId() const
{
	return _id;
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
	output::printID(_id, offset, TTypeUtils::getTTypeString(_varType));
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

const std::vector<TType>& FunctionEntry::getArgTypes() const
{
	return _argTypes;
}

void FunctionEntry::print() const
{
	vector<string> typesStrings = TTypeUtils::getTTypesStrings(_argTypes);
	const string retTypeString = TTypeUtils::getTTypeString(_retType);

	const string functionTypeString = output::makeFunctionType(retTypeString, typesStrings);

	output::printID(_id, OFFSET, functionTypeString);
}
