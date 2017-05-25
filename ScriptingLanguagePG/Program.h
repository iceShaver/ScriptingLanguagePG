#pragma once
#include "BinaryTree.h"
#include "String.h"
#include "Value.h"
#include "Vector.h"

class Program
{
public:

	static void initialize();
	static void introduceOperators();
	static void readOperationsLimit();
	static void readVariablesToPrint();
	static void readAndPrepareCode();
	static void execute();
	static void printOuput();
	static void close();

private:
	static Vector<String> splitOperations(String expression);
	static Vector<String> splitComplexOperation(String expression);
	static String convertToRPN(String expression);
	static Value calculateRPN(String rpnExpression);
	static String calculateOperation(String command, String lVal, String rVal = "")throw(OperationsLimitExceeded);
	static String deleteNeedlessWhiteSpaces(String expression);
	static String minToMinPlus(String input);
	static void executeLoop(const String& operation);
	static void executeIf(const String& operation);
	static bool executeSimpleOperation(const String& operation);
	static bool performOperation(const String& operation);
	

	static unsigned int operationsLimit;
	static unsigned int operationsCounter;
	static BinaryTree<String, Value> *variables;
	static Vector <String>* variablesToPrint;
	static BinaryTree <String, int> *operators;
	static Vector<String> *operationsToPerform;
};

