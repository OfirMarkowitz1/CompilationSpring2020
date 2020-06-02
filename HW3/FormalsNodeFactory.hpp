#pragma once

#include "Node.hpp"

class FormalsNodeFactory
{	
public:
	FormalDeclarationNodePtr createFormalDecl(int lineNumber, TType type, const std::string& id) const;
	FormalsNodePtr createFormals(int lineNumber) const;
};