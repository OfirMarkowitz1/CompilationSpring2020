#pragma once

#include "SymbolTable.hpp"

class SymbolTableAsserter
{
public:
	SymbolTableAsserter(const VariablesTable& variablesTable, const GlobalScopeFunctionsTable& functionsTable);

	void assertVariableDefined(const std::string& id, int lineNumber) const;
	void assertFunctionDefined(const std::string& id, int lineNumber) const;
	
	void assertIdentifierUndefined(const std::string& id, int lineNumber) const;

	void assertMainFunctionDefined() const;

private:
	const VariablesTable& _variablesTable;
	const GlobalScopeFunctionsTable& _functionsTable;
};
