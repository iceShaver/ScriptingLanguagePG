#pragma once
#include "krLIB/String.h"

enum OperationType
{
	LOOP, IF, ASSIGNMENT, CONDITION
};
class Operation
{
public:

	Operation(OperationType operationType, const String& operation)
		: operationType(operationType),
		  operation(operation)
	{
	}

	Operation();
	~Operation();
private:
	OperationType operationType;
	String operation;
};

