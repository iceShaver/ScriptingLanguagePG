#pragma once
#include "String.h"
#include "Exception.h"
class Console
{
public:
	Console();
	~Console();
	static String readLine()throw(EndOfStreamException);
	static String readInput();
};

