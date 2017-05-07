#pragma once
#include "List.h"
#include <cstddef>
template <class Type>
class Stack
{
public:
	Stack();
	Type*Pop();
	Type*Peek();
	void Push(Type *);
	size_t GetSize();
	~Stack();
private:
	List<Type> list;
};

template <class Type>
Stack<Type>::Stack()
{
}

template <class Type>
Type* Stack<Type>::Pop()
{
	return list.PopFirst();
}

template <class Type>
Type* Stack<Type>::Peek()
{
	return list.GetFirst();
}

template <class Type>
void  Stack<Type>::Push(Type*object)
{
	list.PushFirst(object);
}

template <class Type>
size_t Stack<Type>::GetSize()
{
	return list.GetSize();
}

template <class Type>
Stack<Type>::~Stack()
{
}



