#pragma once
#include <iostream>
#include "String.h"
class Value
{
	
public:
	Value();
	Value(int value);
	Value(bool boolean);
	/*explicit*/ Value(const String&str);
	Value& operator=(const Value&other);


	Value operator|(const Value&other) const;
	Value operator&(const Value&other) const;
	Value operator+(const Value&other) const;
	Value operator-(const Value&other) const;
	Value operator*(const Value&other) const;
	Value operator/(const Value&other) const;
	Value operator%(const Value&other) const;
	Value operator!() const;
	Value operator-()const;
	Value operator+()const;
	Value operator==(const Value&other) const;
	Value operator!=(const Value&other) const;
	Value operator<(const Value&other) const;
	Value operator>(const Value&other) const;
	Value operator<=(const Value&other) const;
	Value operator>=(const Value&other) const;

	operator String();
	operator bool();


private:
	int value;
	bool isNul;
	friend std::ostream& operator<<(std::ostream& os, const Value& obj);
};

