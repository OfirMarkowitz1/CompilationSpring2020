#pragma once

#include "Node.hpp"

class TypeNodeFactory
{	
public:
	TypeNodePtr createInt(int lineNumber) const;
	TypeNodePtr createByte(int lineNumber) const;
	TypeNodePtr createBool(int lineNumber) const;
	TypeNodePtr createVoid(int lineNumber) const;

private:
	TypeNodePtr create(int lineNumber, TType type) const;
};