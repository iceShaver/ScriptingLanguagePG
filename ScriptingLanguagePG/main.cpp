#include "String.h"
#include "Vector.h"
#include "Console.h"
#include "Stack.h"
#include "Map.h"
#include "Value.h"
#include <iostream>
using namespace std;

/*
 * TODO: care situation where one var or number is a condition
 * TODO: care unary operators
 * TODO: assignment operator as right-handed, multiple assignments
 * 
 */

unsigned int operationsLimit;
unsigned int operationsCounter = 1;
Map<String, Value> variables(Duplicates::OVERRIDE);
Vector <String> variablesToPrint;
Map <String, int> operators(Duplicates::FORBID);




void printVariables();
void setOperationsLimit();
void setVariablesToPrint();
void init();
void parseInput(String input);
Vector<String> splitOperations(String expression);
Vector<String> splitComplexOperation(String operation);
String convertToRPN(String expression);
Value calculateRPN(String rpnExpression);
String calculate(String command, String lVal, String rVal = "") throw(OperationsLimitExceeded);
String deleteNeedlessWhiteSpaces(String expression);
String minToMinPlus(String input);
String unaryMinusTo$(String input);
void executeLoop(const String& operation);
void executeIf(const String& operation);
bool executeSimpleOperation(const String& operation);
bool performOperation(const String& operation);



void printVariables()
{
	cout << operationsCounter << endl;
	//for (String* var : variablesToPrint)
	for(Vector<String>::Iterator it = variablesToPrint.begin(); it!=variablesToPrint.end();++it)
		std::cout << **it << " " << *variables.peek(*it) << std::endl;
}
void setOperationsLimit()
{
	std::cin >> operationsLimit;
}
void setVariablesToPrint()
{
	variablesToPrint = Console::readLine().split(' ');
	//for (String* var : variablesToPrint)
	for (Vector<String>::Iterator it = variablesToPrint.begin(); it != variablesToPrint.end(); ++it)
		variables.push(new String(**it), new Value());
}
void init()
{
	variables.push(new String("Nul"), new Value());
	operators.push(new String('='), new int(0));
	operators.push(new String('|'), new int(1));
	operators.push(new String('&'), new int(2));
	operators.push(new String("!="), new int(3));
	operators.push(new String("=="), new int(3));
	operators.push(new String('<'), new int(4));
	operators.push(new String('>'), new int(4));
	operators.push(new String("<="), new int(4));
	operators.push(new String(">="), new int(4));
	operators.push(new String('+'), new int(5));
	operators.push(new String('-'), new int(5));
	operators.push(new String('/'), new int(6));
	operators.push(new String('*'), new int(6));
	operators.push(new String('%'), new int(6));
	operators.push(new String('!'), new int(7));
	operators.push(new String("-u"), new int(7));
}



void parseInput(String input)
{
	if(!input)return;
	input = input.trim().replace('\n', ' ');
	//cout << minToMinPlus(deleteNeedlessWhiteSpaces(input)) << endl;
	//exit(0);
	cout << unaryMinusTo$(minToMinPlus(deleteNeedlessWhiteSpaces(input))) << endl;
	Vector<String> operations = splitOperations(minToMinPlus(deleteNeedlessWhiteSpaces(input)));
	//for (String* operation : operations) {
	for(Vector<String>::Iterator it = operations.begin(); it!=operations.end();++it){
		performOperation(**it);
	}
}

Vector<String> splitOperations(String expression)
{
	Vector<String>result;
	expression = expression.trim().replace('\n', ' ');
	String block;
	String operation;
	String condition;
	String body;
	while (true)
	{
		//if loop or condition
		if (expression[0] == '@' || expression[0] == '?')
		{
			operation = expression.substring(0, 1);
			int length;
			int bracketsCounter = 0;

			//read condition
			condition = "";
			int i = 2;
			while (true)
			{
				if (expression[i] == '(')
					++bracketsCounter;
				if (expression[i] == ')') {
					if (bracketsCounter == 0)
						break;
					--bracketsCounter;
				}
				++i;
			}
			length = i - 2;
			condition = expression.substring(2, length);

			//read body
			expression = expression.substring(i + 1).trim();
			i = 1;
			bracketsCounter = 0;
			while (true)
			{
				if (i >= expression.getLength())
					break;
				if (expression[i] == '{')
					++bracketsCounter;
				if (expression[i] == '}')
				{
					if (bracketsCounter == 0)
						break;
					--bracketsCounter;
				}
				++i;
			}
			length = i - 1;
			body = expression.substring(1, length).trim();
			result.pushLast(new String(operation + "(" + condition + ")" + "{" + body + "}"));
			expression = expression.substring(i + 1);
			if (!expression)
				return result;
			expression = expression.trim();
			if (!expression)
				return result;
		}

		if (!expression) return result;


		//assignment or condition operation
		if (expression.isLetter(expression[0]) || expression.isDigit(expression[0]))
		{
			String tmp = expression;
			int i = 0;
			int bracketsCounter;
			while (tmp[i] != ' ' && tmp[i] != '{' && tmp[i] != '?' && tmp[i] != '@'&&tmp[i] != '}')
			{
				++i;
				if (i >= tmp.getLength())break;
			}
			tmp = tmp.substring(0, i);
			result.pushLast(new String(tmp));
			expression = expression.substring(i);
			if (!expression)
				return result;
			expression = expression.trim();
		}
		if (!expression) return result;
	}
}

Vector<String> splitComplexOperation(String expression)
{
	String operation, condition, body;
	Vector<String> result;

	operation = expression.substring(0, 1);
	int length;
	int bracketsCounter = 0;

	//read condition
	condition = "";
	int i = 2;
	while (true)
	{
		if (expression[i] == '(') {
			++bracketsCounter;
		}
		if (expression[i] == ')') {
			if (bracketsCounter == 0)
				break;
			--bracketsCounter;
		}
		++i;
	}
	length = i - 2;
	result.pushLast(new String(expression.substring(2, length)));

	//read body
	expression = expression.substring(i + 1).trim();
	i = 1;
	bracketsCounter = 0;
	while (true)
	{
		if (i >= expression.getLength())
			break;
		if (expression[i] == '{')
			++bracketsCounter;
		if (expression[i] == '}')
		{
			if (bracketsCounter == 0)
				break;
			--bracketsCounter;
		}
		++i;
	}
	length = i - 1;
	result.pushLast(new String(expression.substring(1, length).trim()));
	return result;
}

String convertToRPN(String expression)
{
	Stack<String> stack;
	String result;
	String token;
	while (expression)
	{
		token = expression.readSegment();
		expression = expression.substring(token.getLength());
		if (String::isLetter(token[0]) || String::isDigit(token[0]) || token[0] == '-'&&String::isDigit(token[1]) && token.getLength() >= 2)
			result += token + " ";
		else if (token == "(")
			stack.Push(new String(token));
		else if (token == ")")
			while (stack.GetSize())
			{
				String *tmp = stack.Pop();
				token = *tmp;
				delete tmp;
				if (token == "(") break;
				else result += token + " ";
			}
		else
		{
			const int * priority = operators.peek(&token);
			while (stack.GetSize())
			{
				String*top = stack.Pop();
				if (*top == "(" || *operators.peek(top) < *priority)
				{
					stack.Push(new String(*top));
					delete top;
					break;
				}
				result += *top + " ";
				delete top;
			}
			stack.Push(new String(token));
		}
	}
	while (stack.GetSize())
	{
		String * tmp = stack.Pop();
		result += *tmp + " ";
		delete tmp;
	}
	return result.trim();
}
Value calculateRPN(String rpnExpression)
{
	Stack<String> stack;
	String token, val1, val2;
	String result;
	while (rpnExpression)
	{
		token = rpnExpression.readSegment();
		rpnExpression = rpnExpression.substring(token.getLength() + 1);
		if (String::isLetter(token[0]) || String::isDigit(token[0]) || token[0] == '-'&&String::isDigit(token[1]) && token.getLength() >= 2)//if is operand
			stack.Push(new String(token));
		else
		{
			String * tmp = stack.Pop();
			val2 = *tmp;
			delete tmp;
			tmp = stack.Pop();
			val1 = *tmp;
			delete tmp;
			result = calculate(token, val1, val2);
			stack.Push(new String(result));
		}
	}
	String *tmp = stack.Pop();
	result = *tmp;
	delete tmp;
	return (Value)result;
}


String calculate(String command, String lVal, String rVal)
{
	if (operationsCounter >= operationsLimit)
		throw OperationsLimitExceeded();
	++operationsCounter;



	Value left, right;

	//lVal processing
	if (String::isDigit(lVal[0]) || lVal[0] == '-'&&String::isDigit(lVal[1]) && lVal.getLength() >= 2)//if number or negative number
		left = (Value)lVal;
	else if (String::isLetter(lVal[0]))//if variable name
	{
		const Value*var = variables.peek(&lVal);
		if (!var) //if var does not exist -> create it
		{
			variables.push(new String(lVal), new Value());
		}
		else
		{
			left = *var;
		}
	}

	//rVal processing
	if (rVal == "")
	{
		if (command == "!")
			return !left;
		if (command == "-")
			return -left;
	}
	if (String::isDigit(rVal[0]) || rVal[0] == '-'&&String::isDigit(rVal[1]) && rVal.getLength() >= 2)//if number or negative number
		right = (Value)rVal;
	else if (String::isLetter(rVal[0]))
	{
		const Value*var = variables.peek(&rVal);
		if (!var) //if var does not exist -> create it
			variables.push(new String(rVal), new Value());
		else
			right = *var;
	}



	if (command == "==") return left == right;
	if (command == "!=") return left != right;
	if (command == ">=") return left >= right;
	if (command == "<=") return left <= right;
	if (command == "<") return left < right;
	if (command == ">") return left > right;
	if (command == "+") return left + right;
	if (command == "-") return left - right;
	if (command == "/") return left / right;
	if (command == "*") return left * right;
	if (command == "%") return left % right;
	if (command == "&") return left & right;
	if (command == "|") return left | right;
	if (command == "=")
	{
		variables.update(&lVal, new Value(right));
		return right;
	}
	return Value();
}

String deleteNeedlessWhiteSpaces(String expression)
{
	expression.trim();
	String result;
	while (expression)
	{
		//search operations separator sequence: "var var" || "number number" || "} number" || "} var"
		String segment1, segment2, segment3;
		segment1 = expression.readSegment();
		segment2 = expression.substring(segment1.getLength()).readSegment();
		segment3 = expression.substring(segment1.getLength() + segment2.getLength()).readSegment();
		if ((String::isLetter(segment1[0]) || String::isDigit(segment1[0]) || segment1 == "}" || segment1 == ")")
			&& (segment2 == " ")
			&& (String::isLetter(segment3[0]) || String::isDigit(segment3[0]) || segment3[0] == '('))
		{

			result += expression.substring(0, segment1.getLength() + segment2.getLength() + segment3.getLength());
			expression = expression.substring(segment1.getLength() + segment2.getLength() + segment3.getLength());
			if (!expression)return result;
			while (expression[0] == ' ')
				expression = expression.substring(1);
		}
		else
		{
			String tmp = expression.readSegment();
			result += tmp;
			expression = expression.substring(tmp.getLength());
			if (!expression)return result;
			expression = expression.trim();

		}

		//if result last char and first expression char are var-var, digit-var, var-digit, digit-digit
		//situation caused by two or more white characters one by one
		if (
			(String::isLetter(result[result.getLength() - 1]) || String::isDigit(result[result.getLength() - 1]))
			&&
			(String::isLetter(expression[0]) || String::isDigit(expression[0]))
			)//is var or is number
			result += " ";

	}
	return result;
}


bool performOperation(const String& operation)
{
	if (operation[0] == '@') 
		executeLoop(operation);
	
	else if (operation[0] == '?') 
		executeIf(operation);
	
	else
		return executeSimpleOperation(operation);
	return false;
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!OBSOLETE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
String minToMinPlus(String input)
{
	
	for (int i = 0; i < input.getLength()-2; ++i)
	{
		if ((String::isDigit(input[i])||String::isLetter(input[i])) && input[i + 1] == '-'&&String::isDigit(input[i + 2]))
			input.insertBefore(i + 1, "+");
	}

	return input;
}

String unaryMinusTo$(String input)
{
	return input.replace('-', '$');
}


void executeLoop(const String& operation)
{
	Vector<String> splittedLoop = splitComplexOperation(operation);
	String condition = *splittedLoop[0];
	String body = *splittedLoop[1];
	Vector<String> bodyOperations = splitOperations(body);
	while (performOperation(condition))
	{
		++operationsCounter;
		if (bodyOperations.getSize() == 1)
			performOperation(body);
		else
			//for (String* operation : bodyOperations)
			for(Vector<String>::Iterator it = bodyOperations.begin();it!=bodyOperations.end(); ++it)
				performOperation(**it);
	}

}

void executeIf(const String& operation)
{
	++operationsCounter;
	Vector<String> splittedIf = splitComplexOperation(operation);
	String condition = *splittedIf[0];
	String body = *splittedIf[1];
	Vector<String>bodyOperations = splitOperations(body);
	if (performOperation(condition))
	{

		if (bodyOperations.getSize() == 1)
			performOperation(body);
		else
			//for (String* operation : bodyOperations)
			for(Vector<String>::Iterator it = bodyOperations.begin(); it!=bodyOperations.end();++it)
				performOperation(**it);
	}
}



bool executeSimpleOperation(const String& operation)
{
	return calculateRPN(convertToRPN(operation));
}

int main(int argc, char* argv[])
{
	init();
	setOperationsLimit();
	setVariablesToPrint();
	try {
		parseInput(Console::readInput());
	}
	catch (OperationsLimitExceeded) {}

	printVariables();
	//getchar();
	return 0;
}
