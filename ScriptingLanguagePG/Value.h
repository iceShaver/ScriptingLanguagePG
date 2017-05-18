#pragma once
#include <iostream>
#include "krLIB/String.h"
class Value
{
	
public:
	Value();
	Value(int value);
	Value(bool boolean);
	explicit Value(const String&str);
	//funkcje zmieniajace wartosc obecnej wartosci
	Value& operator=(const Value&other);

	//funkcje zwracaj¹ce nowe wartoœci
	//podwojne
	Value operator|(const Value&other) const;
	Value operator&(const Value&other) const;
	Value operator+(const Value&other) const;
	Value operator-(const Value&other) const;
	Value operator*(const Value&other) const;
	Value operator/(const Value&other) const;
	Value operator%(const Value&other) const;
	//pojedyncze
	Value operator!() const;
	Value operator-()const;
	Value operator+()const;

	//operatory porownania
	Value operator==(const Value&other) const;
	Value operator!=(const Value&other) const;
	Value operator<(const Value&other) const;
	Value operator>(const Value&other) const;
	Value operator<=(const Value&other) const;
	Value operator>=(const Value&other) const;

	//operatory konwertujace
	operator String();
	operator bool();


private:
	int value;
	bool isNul;
	friend std::ostream& operator<<(std::ostream& os, const Value& obj);
};

