#include "Value.h"
#include "Exception.h"

std::ostream& operator<<(std::ostream& os, const Value& obj)
{
	if (obj.isNul)
		return os << "Nul";
	return os << obj.value;
}

Value::Value() :value(0), isNul(true)
{
}

Value::Value(int value) : value(value), isNul(false)
{
}

Value::Value(bool boolean)
{
	if(boolean)
	{
		isNul = false;
		value = 0;
	}else
	{
		isNul = true;
		value = 0;
	}
}

Value::Value(const String& str)
{
	if(str=="Nul")
	{
		value = 0;
		isNul = true;
	}else
	{
		value = (long long)str;
		isNul = false;
	}

}

Value& Value::operator=(const Value& other)
{
	value = other.value;
	isNul = other.isNul;
	return *this;
}

Value Value::operator|(const Value& other) const
{
	if (isNul || other.isNul) return Value();
	return value | other.value;
}

Value Value::operator&(const Value& other) const
{
	if (isNul || other.isNul)return Value();
	return value&other.value;
}

Value Value::operator+(const Value& other) const
{
	if (isNul || other.isNul)return Value();
	return value + other.value;
}

Value Value::operator-(const Value& other) const
{
	if (isNul || other.isNul)return Value();
	return value - other.value;
}

Value Value::operator*(const Value& other) const
{
	if (isNul || other.isNul)return Value();
	return value*other.value;
}

Value Value::operator/(const Value& other) const
{
	if (isNul || other.isNul)return Value();
	if (other.value == 0) return Value();//throw ZeroDivisionException();
	return value / other.value;
}

Value Value::operator%(const Value& other) const
{
	if (isNul || other.isNul)return Value();
	if (other.value == 0) throw ZeroDivisionException();
	return value%other.value;
}

Value Value::operator!() const
{
	if (isNul) return 0;
	return Value();
}

Value Value::operator-() const
{
	if (isNul) return Value();
	return -value;
}

Value Value::operator+() const
{
	if (isNul)return Value();
	return value;
}

Value Value::operator==(const Value& other) const
{
	if (isNul && other.isNul)return 0;
	if (isNul || other.isNul)return Value();
	return value == other.value;
}

Value Value::operator!=(const Value& other) const
{
	if (isNul || other.isNul) return Value();
	return !(*this == other);
}

Value Value::operator<(const Value& other) const
{
	if (isNul || other.isNul) return Value();
	return value < other.value;
}

Value Value::operator>(const Value& other) const
{
	if (isNul || other.isNul) return Value();
	return value > other.value;
}

Value Value::operator<=(const Value& other) const
{
	if (isNul || other.isNul) return Value();
	return value <= other.value;
}

Value Value::operator>=(const Value& other) const
{
	if (isNul || other.isNul) return Value();
	return value >= other.value;
}

Value::operator String()
{
	if (isNul) return String("Nul");
	return String(value);
}

Value::operator bool()
{
	if (isNul)return false;
	return true;
}
