#pragma once
class Exception
{
public:
	Exception();
	virtual ~Exception();
private:


};
class OutOfRangeException : Exception {};
class InvalidArgumentException : Exception {};
class NotImplementedException : Exception {};
class NegativeArraySizeException : Exception {};
class NullReferenceException : Exception {};
class DuplicateException : Exception {};
class EndOfFileException : Exception {};
class EndOfStreamException : Exception {};
class ZeroDivisionException : Exception {};
class BadIntegerCastException : Exception {};
class NotFoundException : Exception {};
class BadArgumentException : Exception {};