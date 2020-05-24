#include "SymbolTableAsserter.hpp"
#include "hw3_output.hpp"

using namespace std;

static const string MAIN_FUNCTION_ID = "main";
static const TType MAIN_FUNCTION_RET_TYPE = T_VOID;

SymbolTableAsserter::SymbolTableAsserter(const VariablesTable& variablesTable, const GlobalScopeFunctionsTable& functionsTable) :
_variablesTable(variablesTable),
_functionsTable(functionsTable)
{
}

void SymbolTableAsserter::assertVariableDefined(const std::string& id, int lineNumber) const
{
	if (!_variablesTable.contains(id))
	{
		output::errorUndef(lineNumber, id);
		exit(1);
	}
}

void SymbolTableAsserter::assertFunctionDefined(const std::string& id, int lineNumber) const
{
	if (!_functionsTable.contains(id))
	{
		output::errorUndefFunc(lineNumber, id);
		exit(1);
	}
}

void SymbolTableAsserter::assertIdentifierUndefined(const std::string& id, int lineNumber) const
{
	if (_variablesTable.contains(id))
	{
		output::errorDef(lineNumber, id);
		exit(1);
	}
}

void SymbolTableAsserter::assertMainFunctionDefined() const
{
	FunctionEntryPtr functionEntry = _functionsTable.find(MAIN_FUNCTION_ID);
	
	if ((functionEntry == nullptr) ||
		(functionEntry->getRetType() == MAIN_FUNCTION_RET_TYPE) ||
		(functionEntry->getArgTypes().empty()))
	{
		output::errorMainMissing();
		exit(1);
	}
}
