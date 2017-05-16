//
// Created by kamil on 31.03.17.
//
#include "String.h"
#include "Exception.h"
#include <cstdio>
#include <iostream>
String::String() : array(new char[1]), capacity(1), length(0) {}

String::String(const char * cstring) : capacity(1) {
	length = getCStringLength(cstring);
	capacity = length;
	if (!capacity) capacity = 1;
	array = new char[capacity];
	copy(cstring, array, length);
}

String::operator const char*() const
{
	if (!this) throw NullReferenceException();
	char *result = new char[length + 1];
	copy(array, result, length);
	*(result + length) = '\0';
	return result;
}

String::operator long long() const
{
	long long result;
	if (!sscanf_s(array, "%d", &result)) throw BadIntegerCastException();
	return result;
}


String::operator unsigned long long() const
{
	throw NotImplementedException();
}

String::operator double() const
{
	throw NotImplementedException();
}

String::operator bool() const
{
	if (length)return true;
	return false;
}

String::String(const String& newString)
{
	if (newString == nullptr)
	{
		String();
		return;
	}
	//if (array)delete[]array;
	length = newString.length;
	capacity = newString.capacity;
	array = new char[capacity];
	copy(newString.array, array, length);
}

bool String::isDigit(const char& character)
{
	if (character >= 48 && character <= 57)return true;
	return false;
}

bool String::isLetter(const char& character)
{
	if (isCapitalLetter(character) || isLowerCaseLetter(character))
		return true;
	return false;
}

bool String::isCapitalLetter(const char& character)
{
	if (character >= 65 && character <= 90)
		return true;
	return false;
}

bool String::isLowerCaseLetter(const char& character)
{
	if (character >= 97 && character <= 122)
		return true;
	return false;

}

bool String::isOperatorSymbol(const char& character)
{
	if (character == '!'
		|| character == '%'
		|| character == '&'
		|| character == '*'
		|| character == '+'
		|| character == '-'
		|| character == '/'
		|| character == '<'
		|| character == '='
		|| character == '>'
		|| character == '^'
		|| character == '|'
		|| character == ')'
		|| character == '('
		|| character == '}'
		|| character == '{'
		)
		return true;
	return false;
}

bool String::isOperator(String symbol)
{
	if (symbol == "="
		|| symbol == "=="
		|| symbol == "!="
		|| symbol == ">="
		|| symbol == "<="
		|| symbol == "<"
		|| symbol == ">"
		|| symbol == "+"
		|| symbol == "-"
		|| symbol == "/"
		|| symbol == "*"
		|| symbol == "%"
		|| symbol == "&"
		|| symbol == "|"
		|| symbol == "!"
		|| symbol == "("
		|| symbol == ")"
		|| symbol == "{"
		|| symbol == "}"
		) return true;
	return false;
}

char String::toLower(const char& character)
{
	return character + 32;
}

char String::toUpper(const char& character)
{
	return character - 32;
}

String String::parseInt(const long long& number)
{
	char * tmp = new char[128];
	int n = sprintf_s(tmp, 128, "%d", number);
	String result;
	result.array = new char[n];
	copy(tmp, result.array, n);
	delete[] tmp;
	result.capacity = result.length = n;
	return result;
}

String String::parseUInt(const unsigned long long& number)
{
	char * tmp = new char[128];
	int n = sprintf_s(tmp, 128, "%d", number);
	String result;
	result.array = new char[n];
	copy(tmp, result.array, n);
	delete[] tmp;
	result.capacity = result.length = n;
	return result;
}

String String::parseCString(const char*cstring)
{
	size_t size = getCStringLength(cstring);
	String result;
	result.array = new char[size];
	result.capacity = result.length = size;
	copy(cstring, result.array, size);
	return result;
}

String String::parseDouble(const long double& number, const char *const format)
{
	char * tmp = new char[128];
	int n;
	if (format)
		n = sprintf_s(tmp, 128, format, number);
	else
		n = sprintf_s(tmp, 128, "%f", number);
	String result;
	result.array = new char[n];
	copy(tmp, result.array, n);
	delete[] tmp;
	result.capacity = result.length = n;
	return result;
}

String String::parseChar(const char& character)
{
	String result;
	result.array = new char[1];
	result.length = result.capacity = 1;
	result.array[0] = character;
	return result;
}

String::~String()
{
	delete[] array;
}

size_t String::getCStringLength(const char * cstring) {
	if (!cstring) return 0;
	size_t length = 0;
	const char * ptr = cstring;
	while (*(ptr++)) length++;
	return length;
}

String::Iterator String::begin() const
{
	return Iterator(array);
}

String::Iterator String::end() const
{
	return  Iterator(array + length);
}

String::String(const int&number)
{
	array = nullptr;
	*this = parseInt(number);
}

String::String(const long long&number)
{
	array = nullptr;
	*this = parseInt(number);
}

String::String(const unsigned long long&number)
{
	array = nullptr;
	*this = parseUInt(number);
}

String::String(const float&number)
{
	array = nullptr;
	*this = parseDouble(number);
}

String::String(const double&number)
{
	array = nullptr;
	*this = parseDouble(number);
}

String::String(const char&character)
{
	array = nullptr;
	*this = parseChar(character);
}

void String::resize() {
	if (capacity == 0)
		capacity = 1;
	capacity = 2 * capacity;
	if (array) {
		char * newArray = new char[capacity];
		char*newPtr = newArray, *oldPtr = array;
		copy(array, newArray, length);
		delete[] array;
		array = newArray;
	}
}

String& String::operator+=(const String& appendedString)
{
	return append(appendedString);
}

String& String::operator=(const String&other)
{
	if(array)
	delete[] array;
	capacity = other.length;
	length = other.length;
	array = new char[capacity];
	copy(other.array, array, length);
	return *this;
}

char& String::operator[](size_t index) const throw(OutOfRangeException)
{
	if (index >= length) throw OutOfRangeException();
	return *(array + index);
}

bool String::operator==(const String& other) const
{
	if (compare(*this, other) == 0)return true;
	return false;
}

bool String::operator!=(const String& other) const
{
	return !operator==(other);
}

bool String::operator>(const String& other) const
{
	if (compare(*this, other) == 1)return true;
	return false;

}

bool String::operator<(const String& other) const
{
	if (compare(*this, other) == -1)return true;
	return false;
}

bool String::operator<=(const String& other) const
{
	int compareResult = compare(*this, other);
	if (compareResult == 0 || compareResult == -1) return true;
	return false;
}

bool String::operator>=(const String& other) const
{
	int compareResult = compare(*this, other);
	if (compareResult == 0 || compareResult == 1)return true;
	return false;
}

String& String::append(const String& appendedString)
{
	while (length + appendedString.length > capacity) resize();
	char * newString = new char[capacity];
	copy(array, newString, length);
	copy(appendedString.array, newString + length, appendedString.length);
	delete[] array;
	array = newString;
	length += appendedString.length;
	return *this;
}

String& String::prepend(const String&prependedString)
{
	while (length + prependedString.length > capacity) resize();
	char * newString = new char[capacity];
	copy(prependedString.array, newString, prependedString.length);
	copy(array, newString + prependedString.length, length);
	delete[]array;
	array = newString;
	length = prependedString.length + length;
	return *this;
}

String String::substring(size_t beginIndex, size_t count) const throw(OutOfRangeException)
{
	if (beginIndex >= length) return String();
	if (count > length)count = length - beginIndex;
	//if (beginIndex > count) throw InvalidArgumentException();
	String result;
	if (result.array)
		delete[]result.array;
	result.capacity = result.length = count;
	result.array = new char[capacity];
	copy(array + beginIndex, result.array, result.length);
	return result;
}

String String::substring(size_t beginIndex) const throw(OutOfRangeException)
{
	if (beginIndex >= length) return String();
	String result;
	if (result.array)
		delete[]result.array;
	result.capacity = result.length = length - beginIndex;
	result.array = new char[capacity];
	copy(array + beginIndex, result.array, result.length);
	return result;
}

String String::replace(const char& from, const char& to) const
{
	String result = *this;
	for (int i = 0; i < result.length; ++i)
			if (result.array[i] == from)
				result.array[i] = to;

	return result;
}

Vector<String> String::split(const char &splitter)
{
	Vector<String> result;
	short i = 0;
	for (char c : *this)
	{
		if (!result[i])
			result[i] = new String;
		if (c != splitter)
			*(result[i]) += c;
		else
			i++;
	}
	return result;
}

String String::readOnlyLettersWord() const
{
	size_t count = 0;
	for (char character : *this)
	{
		if (!isLetter(character)) break;
		count++;
	}
	return substring(0, count);
}

String String::readDecimalIntegerNumber() const
{
	size_t count = 0;
	for (char character : *this)
	{
		if (!isDigit(character)) break;
		count++;
	}
	return substring(0, count);
}

/**
 * \brief read and return string of char of one "type"
 * if "type" not recognized returns one char
 * \return String of matching chars
 */
String String::readSegment() const
{
	//TODO: ' ' ommiting
	if (isDigit(array[0])) return readDecimalIntegerNumber();
	if (isLetter(array[0])) return readOnlyLettersWord();
	if (isOperatorSymbol(array[0])) return readOperator();
	return substring(0, 1);
}

String String::readOperator() const
{
	size_t count = 0;
	for (char c : *this)
	{
		if (!isOperatorSymbol(c)) break;
		count++;
	}
	String result = substring(0, count);
	while (!isOperator(result))
	{
		result = result.substring(0, result.getLength() - 1);
		if (result == "") return result;
	}
	return result;
}

String& String::toLower()
{
	char*ptr = array;
	for (int i = 0; i < length; ++i)
	{
		if (isCapitalLetter(*ptr))
			*ptr = toLower(*ptr);
		ptr++;
	}
	return *this;
}

String& String::toUpper()
{
	char*ptr = array;
	for (int i = 0; i < length; ++i)
	{
		if (isLowerCaseLetter(*ptr))
			*ptr = toUpper(*ptr);
		ptr++;
	}
	return *this;
}

String String::trim() const//TODO: care if string is empty
{
	String result = *this;
	size_t trimStartIndex = 0;
	char*ptr = array;
	for (int i = 0; i < length; ++i)
	{
		if (*ptr != ' '&&*ptr != '\n')break;
		trimStartIndex++;
		ptr++;
	}
	if (trimStartIndex != 0)
	{
		int newLength, newCapacity;
		newLength = newCapacity = length - trimStartIndex;
		delete[] result.array;
		result.array = new char[newCapacity];
		copy(array + trimStartIndex, result.array, newLength);
		result.length = newLength;
		result.capacity = newCapacity;
		//delete[] array;
		//array = newArray;
	}
	ptr = array + length - 1;
	size_t trimStopIndex = length - 1;
	for (int i = 0; i < length; ++i)
	{
		if (*ptr != ' '&&*ptr != '\n') break;
		trimStopIndex--;
		ptr--;
	}
	if (trimStopIndex != length - 1)
	{
		result.length = trimStopIndex + 1;
	}
	return result;
}

const char* String::c_str() const
{
	return static_cast<const char*>(*this);
}

size_t String::getLength() const
{
	return length;
}

size_t String::getCapacity() const
{
	return capacity;
}

void String::copy(const char * src, char * dst, size_t length)
{
	const char * srcPtr = src;
	char * dstPtr = dst;
	for (size_t i = 0; i < length; ++i)
		*(dstPtr++) = *(srcPtr++);
}

bool String::areEqual(const char* left, const char* right, size_t length)
{
	for (int i = 0; i < length; ++i)
	{
		if (left[i] > right[i]) return false;
		if (left[i] < right[i]) return false;
	}
	return true;

}

int String::compare(const String& left, const String& right)
{
	size_t minLength = (left.length > right.length ? right.length : left.length);
	for (int i = 0; i < minLength; ++i)
	{
		if (left[i] < right[i]) return -1;
		if (left[i] > right[i])return 1;
	}
	if (left.length == right.length) return 0;
	return (left.length > right.length) ? 1 : -1;
}

std::ostream& operator<<(std::ostream& ostr, const String& string)
{
	const char * tmp = string.c_str();
	ostr << tmp;
	delete[] tmp;
	return ostr;
}

String operator+(const String& left, const String& right)
{
	String result = left;
	return result.append(right);
}

String operator+(String& left, String& right)
{
	String result = left;
	return result.append(right);
}
//__________________________________Iterator implementation______________________________
String::Iterator::Iterator() :ptr(nullptr)
{
}

String::Iterator::Iterator(const Iterator& other) : ptr(other.ptr)
{
}

String::Iterator::~Iterator()
{
}

String::Iterator::Type& String::Iterator::operator*() const
{
	if (!ptr)throw NullReferenceException();
	return *ptr;
}

String::Iterator& String::Iterator::operator++()
{
	ptr++;
	return *this;
}

String::Iterator String::Iterator::operator++(int)
{
	Iterator tmp = *this;
	ptr++;
	return tmp;
}

String::Iterator& String::Iterator::operator--()
{
	ptr--;
	return *this;
}

String::Iterator String::Iterator::operator--(int)
{
	Iterator tmp = *this;
	ptr--;
	return tmp;
}

String::Iterator& String::Iterator::operator+=(OffsetType offset)
{
	ptr += offset;
	return *this;
}

String::Iterator String::Iterator::operator+(OffsetType offset) const
{
	Iterator newIt = *this;
	newIt += offset;
	return newIt;
}

String::Iterator& String::Iterator::operator-=(OffsetType offset)
{
	ptr -= offset;
	return *this;
}

String::Iterator String::Iterator::operator-(OffsetType offset) const
{
	Iterator newIt = *this;
	newIt -= offset;
	return newIt;
}

bool String::Iterator::operator==(const Iterator& other) const
{
	return ptr == other.ptr;
}

bool String::Iterator::operator!=(const Iterator& other) const
{
	return ptr != other.ptr;
}

bool String::Iterator::operator>(const Iterator& other) const
{
	return ptr > other.ptr;
}

bool String::Iterator::operator>=(const Iterator& other) const
{
	return ptr >= other.ptr;
}

bool String::Iterator::operator<(const Iterator& other) const
{
	return ptr < other.ptr;
}

bool String::Iterator::operator<=(const Iterator& other) const
{
	return ptr <= other.ptr;
}

String::Iterator::Iterator(Type* ptr) :ptr(ptr)
{

}
