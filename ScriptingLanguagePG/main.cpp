#include "Program.h"

/*
* TODO:DONE: care situation where one var or number is a condition
* TODO: care unary operators
* TODO: assignment operator as right-handed, multi assignments
*
*/

int main(int argc, char* argv[])
{
	Program::initialize();
	Program::introduceOperators();
	Program::readOperationsLimit();
	Program::readVariablesToPrint();
	Program::readAndPrepareCode();
	Program::execute();
	Program::printOuput();
	Program::close();
	getchar();
	return 0;
}