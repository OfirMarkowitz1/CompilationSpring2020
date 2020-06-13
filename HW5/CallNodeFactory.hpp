#pragma once

#include "Node.hpp"

class CallNodeFactory
{
public:		
	CallNodePtr create(int lineNumber, TType returnType) const;
};