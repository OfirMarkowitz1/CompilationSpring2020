#pragma once

#include "Node.hpp"

class CallNodeFactory
{
public:		
	CallNodePtr create(TType returnType, IdentifierNodePtr identifier) const;
};