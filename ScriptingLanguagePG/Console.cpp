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
	//fseek(stdin, 0, SEEK_END);
	char tmp[1002];
	//if (!fgets(tmp, 1001, stdin))
	//	throw EndOfStreamException();
	/*if (tmp[0] == '\n')
		throw EndOfStreamException();*/
	//fgets(tmp, 1001, stdin);
	std::cin.get();
	std::cin.getline(tmp, 1001);
	String result(tmp);
	//std::cout << result << std::endl;
	return result.trim();
}

String Console::readInput()
{
	String result;
	char tmp[1002];
	//fseek(stdin, 0, SEEK_END);
	while (fgets(tmp, 1001, stdin))
	{
		//if (tmp[0] == '\n') break;
		result += tmp;
	}
	//std::cout << result << std::endl;
	//exit(0);
	if (!result)return String();
	return result;//TODO: String maintenance needed: why returning this gives 0xcc...
}
