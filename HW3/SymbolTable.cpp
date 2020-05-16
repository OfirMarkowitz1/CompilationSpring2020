#include "SymbolTable.hpp"
#include "hw3_output.hpp"
#include <iostream>

using namepace std;

SymbolTable::SymbolTable(const int& lineNumber) :
_lineNumber(lineNumber)
{
}

void SymbolTable::add(const std::string& id, SymbolTypePtr type) const
{
	assertScopeTablesStackNotEmpty();
	assertNotContains(id);

	_scopeTablesStack.top()->add(id, type);
}

void SymbolTable::pushRegularScope()
{
	pushScope({});
}


void SymbolTable::pushFunctionScope(const std::vector<FunctionArgumentData>& arguments)
{
	pushScope(arguments);
}

void SymbolTable::pushScope(const std::vector<FunctionArgumentData>& arguments)
{	
	int scopeStartingOffset = (_scopeTablesStack.empty() ? 0 : _scopeTablesStack.top()->getCurrentOffset());

	_scopeTablesStack.push(make_shared<ScopeSymbolTable>(scopeStartingOffset, arguments));
}

void SymbolTable::popScope()
{
	assertScopeTablesStackNotEmpty();

	output::endScope();

	for (SymbolEntryPtr symbolEntry : _scopeTablesStack.top()->getSymbolEntries())
	{
		symbolEntry->printEntry();
		_symbolIdToEntryMap.erase(symbolEntry->getId());
	}

	_scopeTablesStack.pop();
}

SymbolEntryPtr SymbolTable::find(const std::string& id) const
{
	auto entryIter = _symbolIdToEntryMap.find(id);

	if (entryIter != _symbolIdToEntryMap.end())
	{
		return entryIter->second;
	}

	return nullptr;
}

bool SymbolTable::contains(const std::string& id) const
{
	return (_symbolIdToEntryMap.find(id) != _symbolIdToEntryMap.end());
}

void SymbolTable::assertDefinedVariable(const std::string& id) const
{
	SymbolEntryPtr symbolEntry = find(id);

	if ((symbolEntry == nullptr) || symbolEntry->getType()->isFunctionType())
	{
		output::errorUndef(_lineNumber, id);
		exit(1);
	}
}

void SymbolTable::assertDefinedFunction(const std::string& id) const
{
	SymbolEntryPtr symbolEntry = find(id);

	if ((symbolEntry == nullptr) || !symbolEntry->getType()->isFunctionType())
	{
		output::errorUndefFunc(_lineNumber, id);
		exit(1);
	}
}

void SymbolTable::assertNotDefined(const std::string& id) const
{
	if (contains(id))
	{
		output::errorDef(_lineNumber, id);
		exit(1);
	}
}

void SymbolTable::assertScopeTablesStackNotEmpty() const
{
	if (_scopeTablesStack.empty())
	{
		cout << "Expected to be an existing scope " << endl
		exit(1);	
	}
}

ScopeSymbolTable::ScopeSymbolTable(int startingOffset, const std::vector<FunctionArgumentData>& arguments) :
_currentOffset(startingOffset)
{
	int currentArgumentOffset = -1;

	for (const auto& argument : arguments)
	{
		_entries.emplace_back(argument.getId(), currentArgumentOffset, argument.getType());	

		currentArgumentOffset--;
	}
}

void ScopeSymbolTable::add(const std::string& id, SymbolTypePtr type)
{
	_entries.emplace_back(id, _currentOffset, type);

	_currentOffset++;
}

const std::vector<SymbolEntryPtr>& ScopeSymbolTable::getSymbolEntries() const
{
	return _entries;
}

int ScopeSymbolTable::getCurrentOffset() const
{
	return _currentOffset;
}

SymbolEntry::SymbolEntry(const std::string& id, int offset, SymbolTypePtr type) :
_id(id),
_offset(offset),
_type(type)
{
}

const std::string& SymbolEntry::getId() const
{
	return _id;
}

SymbolTypePtr SymbolEntry::getType() const
{
	return _type;
}

void SymbolEntry::printEntry() const
{
	output::printID(_id, _offset, _type->toString());
}

std::string SymbolEntry::toString() const
{
	return output::print
}

SymbolType::~SymbolType()
{
}

std::string SymbolType::getTTypeString(TType type) const
{
	switch (type)
	{
	case T_INT:
		return "INT";
	case T_BYTE:
		return "BYTE";
	case T_BOOL:
		return "BOOL";
	case T_STRING:
		return "STRING";
	case T_VOID:
		return "VOID";
	}
}

VariableSymbolType::VariableSymbolType(TType type) :
_type(type)
{
}

VariableSymbolType::~VariableSymbolType()
{
}

bool VariableSymbolType::isFunctionType() const
{
	return false;
}

std::string VariableSymbolType::toString() const
{
	return getTTypeString(_type);
}

FunctionSymbolType::FunctionSymbolType(TType returnType, const std::vector<TType>& argumentTypes) :
_returnType(returnType),
_argumentTypes(argumentTypes)
{
}

FunctionSymbolType::~FunctionSymbolType()
{
}

bool FunctionSymbolType::isFunctionType() const
{
	return true;
}

std::string FunctionSymbolType::toString() const
{
	return output::makeFunctionType(_returnType, _argumentTypes);
}
