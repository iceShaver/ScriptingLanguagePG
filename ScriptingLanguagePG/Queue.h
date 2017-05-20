#pragma once
#include "List.h"
#include <cstddef>
template <class Type>
class Queue
{
public:
public:
	Queue();
	Type*Pop();
	Type*Peek();
	void Push(Type *);
	size_t GetSize();
	~Queue();
private:
	List<Type> list;
};

template <class Type>
Queue<Type>::Queue()
{
}

template <class Type>
Type* Queue<Type>::Pop()
{
	return list.PopFirst();
}

template <class Type>
Type* Queue<Type>::Peek()
{
	return list.GetFirst();
}

template <class Type>
void Queue<Type>::Push(Type* object)
{
	list.PushLast(object);
}

template <class Type>
size_t Queue<Type>::GetSize()
{
	return list.GetSize();
}

template <class Type>
Queue<Type>::~Queue()
{
}

