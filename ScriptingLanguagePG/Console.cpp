#include "Console.h"
#include <cstdio>
#include <iostream>

Console::Console()
{
}


Console::~Console()
{
}

String Console::readLine()
{
	char tmp[1002];

	std::cin.get();
	std::cin.getline(tmp, 1001);
	String result(tmp);
	return result.trim();
}

String Console::readInput()
{
	String result;
	char tmp[1002];
	while (fgets(tmp, 1001, stdin))
		result += tmp;
	if (!result)return String();
	return result;//TODO: String maintenance needed: why returning this gives 0xcc...
}
