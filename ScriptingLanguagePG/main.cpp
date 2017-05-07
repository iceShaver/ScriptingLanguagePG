#include "krLIB/String.h"
#include "krLIB/Vector.h"
#include "krLIB/Console.h"
#include "krLIB/Stack.h"
#include "krLIB/Map.h"
#include "Value.h"
#include <iostream>
unsigned int operationsConstraint;
unsigned int operationsLimit;
Map<String, Value> variables(Duplicates::OVERRIDE);
Vector <String> variablesToPrint;
Map <String, int> operators(Duplicates::FORBID);
void printVariables();
void setOperationsConstraint();
void setVariablesToPrint();
void init();
void parseInput(String input);
void condition(String conditionCommand);
void loop(String loopCommand);
String convertToRPN(String expression);
Value calculateRPN(String rpnExpression);
String calculate(String command, String lVal, String rVal);

void printVariables()
{
	for (String* var : variablesToPrint)
		std::cout << *var << " " << *variables.peek(var) << std::endl;
}

void setOperationsConstraint()
{
	std::cin >> operationsConstraint;
}

void setVariablesToPrint()
{
	variablesToPrint = Console::readLine().split(' ');
	for (String* var : variablesToPrint)
		variables.push(new String(*var), new Value());
}

void init()
{
	variables.push(new String("Nul"), new Value());

	operators.push(new String('='), new int(0));
	operators.push(new String('|'), new int(1));
	operators.push(new String('&'), new int(2));
	operators.push(new String('!='), new int(3));
	operators.push(new String('=='), new int(3));
	operators.push(new String('<'), new int(4));
	operators.push(new String('>'), new int(4));
	operators.push(new String('<='), new int(4));
	operators.push(new String('>='), new int(4));
	operators.push(new String('+'), new int(5));
	operators.push(new String('-'), new int(5));
	operators.push(new String('/'), new int(6));
	operators.push(new String('*'), new int(6));
	operators.push(new String('%'), new int(6));
	operators.push(new String('!'), new int(7));
	operators.push(new String('-u'), new int(7));
}

void parseInput(String input)
{
	std::cout << (input = input.trim().replace('\n', ' ')) << std::endl;
	while(true)
	{
		String block = input.readSegment();
		input = input.substring(block.getLength());
		std::cout << block << std::endl;
	}
}

void condition(String conditionCommand)
{

}

void loop(String loopCommand) {

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
		if (String::isLetter(token[0]) || String::isDigit(token[0]))
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
		if (String::isLetter(token[0]) || String::isDigit(token[0]))
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

	Value left, right;
	if (String::isDigit(lVal[0]))
		left = (Value)lVal;
	else if(String::isLetter(lVal[0]))
	{
		const Value*var = variables.peek(&lVal);
		if(!var)
		{
			variables.push(new String(lVal), new Value());
		}else
		{
			left = *var;
		}
	}
	if (String::isDigit(rVal[0]))
		right = (Value)rVal;
	else if(String::isLetter(rVal[0]))
	{
		const Value*var = variables.peek(&rVal);
		if (!var)
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
	return Value();
}




int main(int argc, char* argv[])
{
	init();
	setOperationsConstraint();
	setVariablesToPrint();
	parseInput(Console::readInput());


	printVariables();
	getchar();
	return 0;
}
