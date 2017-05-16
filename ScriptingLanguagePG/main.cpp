#include "krLIB/String.h"
#include "krLIB/Vector.h"
#include "krLIB/Console.h"
#include "krLIB/Stack.h"
#include "krLIB/Map.h"
#include "Value.h"
#include <iostream>
using namespace std;
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
Vector<String> splitOperations(String expression);
String convertToRPN(String expression);
Value calculateRPN(String rpnExpression);
String calculate(String command, String lVal, String rVal);
String deleteNeedlessWhiteSpaces(String expression);


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
	input = input.trim().replace('\n', ' ');
	std::cout << input << std::endl;
	std::cout << deleteNeedlessWhiteSpaces(input) << std::endl;
	Vector<String> operations = splitOperations(deleteNeedlessWhiteSpaces(input));
	for (String* operation : operations)
	{
		cout << *operation << endl;
	}
	while (input)
	{
		String block;
		do {
			block = input.readSegment();
			input = input.substring(block.getLength());
		} while (block == " ");
		std::cout << block << std::endl;
	}
}
void condition(String conditionCommand)
{

}
void loop(String loopCommand) {

}
Vector<String> splitOperations(String expression)//TODO: fix missing closing brackets at the end of expression for test 4
//TODO: fix test 8 (brackets and operations)
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
			expression = expression.trim();//TODO:care if string empty for trim
			if (!expression)
				return result;
		}
		if (!expression) return result;


		//assignment or condition operation
		if (expression.isLetter(expression[0]) || expression.isDigit(expression[0]))
		{
			String tmp = expression;
			int i = 0;
			while (tmp[i] != ' ' && tmp[i] != '{' && tmp[i] != '?' && tmp[i] != '@' && tmp[i] != ')'&&tmp[i] != '}')
			{
				++i;
				if(i>=tmp.getLength())break;
			}
			tmp = tmp.substring(0, i);
			result.pushLast(new String(tmp));
			expression = expression.substring(i);
			if (!expression)
				return result;
			expression = expression.trim();
		}
		if (!expression) return result;

		//TODO: number is too in condition operation

	}
	//return Vector<String>();
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
	else if (String::isLetter(lVal[0]))
	{
		const Value*var = variables.peek(&lVal);
		if (!var)
		{
			variables.push(new String(lVal), new Value());
		}
		else
		{
			left = *var;
		}
	}
	if (String::isDigit(rVal[0]))
		right = (Value)rVal;
	else if (String::isLetter(rVal[0]))
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

String deleteNeedlessWhiteSpaces(String expression)
{
	expression.trim();
	String result;
	while (expression)
	{	//TODO: add situation where between there are two spaces between
		//search operations separator sequence: "var var" || "number number" || "} number" || "} var"
		String segment1, segment2, segment3;
		segment1 = expression.readSegment();
		segment2 = expression.substring(segment1.getLength()).readSegment();
		segment3 = expression.substring(segment1.getLength() + segment2.getLength()).readSegment();
		if ((String::isLetter(segment1[0]) || String::isDigit(segment1[0]) || segment1 == "}" || segment1 == ")")
			&& (segment2 == " ")
			&& (String::isLetter(segment3[0]) || String::isDigit(segment3[0])/* || segment3 == "}" || segment3 == ")"*/))
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
		//situation caused by two white characters one by one
		if (
			(String::isLetter(result[result.getLength() - 1]) || String::isDigit(result[result.getLength() - 1]))
			&& 
			(String::isLetter(expression[0]) || String::isDigit(expression[0]))
			)//is var or is number
			result += " ";

	}
	return result;
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
