#include "FormalsNodeFactory.hpp"

using namespace std;

FormalDeclarationNodePtr FormalsNodeFactory::createFormalDecl(int lineNumber, TType type, const std::string& id) const
{
	return make_shared<FormalDeclarationNode>(lineNumber, type, id);
}

FormalsNodePtr FormalsNodeFactory::createFormals(int lineNumber) const
{
	return make_shared<FormalsNode>(lineNumber);
}
