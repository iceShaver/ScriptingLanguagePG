//
// Created by Kamil Królikowski on 3/19/17.
// kamil-krolikowski@outlook.com
//

#ifndef KRLIB_LIST_H
#define KRLIB_LIST_H
#include "Exception.h"

//________________________________List header____________________________________
template<class Type>
class List {
	class Node;
public:
	class Iterator;
	//TOD:ADD copy constr
	List();
	~List();
	bool PushFirst(Type *object);
	bool PushLast(Type * object);
	int GetSize() const;
	Type *Get(int index) const;
	Type *Pop(int index);
	Type *GetLast() const;
	Type *GetFirst() const;
	Type *PopFirst();
	Type *PopLast();
	bool Delete(int index);
	Iterator begin();
	Iterator end();
private:
	Node *getElement(int index);
	int elementsNumber;
	long indexCounter;
	Node *firstPtr;
	Node *lastPtr;
	friend Iterator;
};

//____________________________Node header__________________________________
template <class Type>
class List<Type>::Node {
public:
	Node();
	~Node();
private:
	Node *nextPtr;
	Node *prevPtr;
	Type *object;
	long index;
	bool operator==(const Node&other)const;
	bool operator!=(const Node&other)const;
	bool operator>(const Node&other)const;
	bool operator>=(const Node&other)const;
	bool operator<(const Node&other)const;
	bool operator<=(const Node&other)const;

	friend class List;

};

//__________________________Iterator header_________________________________
template <class Type>
class List<Type>::Iterator
{
	typedef int OffsetType;
public:
	Iterator();
	Iterator(const Iterator&other);
	~Iterator();

	Type* operator*()const;
	//Type* operator->()const;
	Iterator& operator++();
	Iterator operator++(int);
	Iterator& operator--();
	Iterator operator--(int);
	Iterator& operator+=(OffsetType offset);
	Iterator operator+(OffsetType offset)const;
	Iterator& operator-=(OffsetType offset);
	Iterator operator-(OffsetType offset)const;
	bool operator==(const Iterator&other)const;
	bool operator!=(const Iterator&other)const;
	bool operator>(const Iterator&other)const;
	bool operator>=(const Iterator&other)const;
	bool operator<(const Iterator&other)const;
	bool operator<=(const Iterator&other)const;
private:
	Iterator(Node*node, bool end);
	Node*node;
	bool end;
	friend List;
};



//______________________Node implementation______________________________
template <class Type>
List<Type>::Node::Node()
{
	//printf("New list\n");
	index = 0;
	nextPtr = nullptr;
	prevPtr = nullptr;
	object = nullptr;
}

template <class Type>
List<Type>::Node::~Node()
{
	if (object)
		delete object;
}

template <class Type>
bool List<Type>::Node::operator==(const Node& other) const
{
	return object == other.object;
}

template <class Type>
bool List<Type>::Node::operator!=(const Node& other) const
{
	return object != other.object;
}

template <class Type>
bool List<Type>::Node::operator>(const Node& other) const
{
	return index > other.index;
}

template <class Type>
bool List<Type>::Node::operator>=(const Node& other) const
{
	return index >= other.index;
}

template <class Type>
bool List<Type>::Node::operator<(const Node& other) const
{
	return index < other.index;
}

template <class Type>
bool List<Type>::Node::operator<=(const Node& other) const
{
	return index <= other.index;
}

//_________________________Iterator implementation______________________
template <class Type>
List<Type>::Iterator::Iterator() : node(nullptr), end(false)
{
}

template <class Type>
List<Type>::Iterator::Iterator(const Iterator& other) : node(other.node), end(other.end)
{
}

template <class Type>
List<Type>::Iterator::~Iterator()
{
}

template <class Type>
Type* List<Type>::Iterator::operator*() const
{
	if (!node || end)throw NullReferenceException();
	return node->object;
}

template <class Type>
typename List<Type>::Iterator& List<Type>::Iterator::operator++()
{
	if (!node->nextPtr) {
		end = true;
		return *this;
	}
	end = false;
	node = node->nextPtr;
	return *this;
}

template <class Type>
typename List<Type>::Iterator List<Type>::Iterator::operator++(int)
{
	if (end) throw OutOfRangeException();
	if (!node->nextPtr)
	{
		end = true;
		return *this;
	}
	end = false;
	Iterator tmp = *this;
	node = node->nextPtr;
	return tmp;
}

template <class Type>
typename List<Type>::Iterator& List<Type>::Iterator::operator--()
{
	if (!node->prevPtr) throw OutOfRangeException();
	node = node->prevPtr;
	end = false;
	return *this;
}

template <class Type>
typename List<Type>::Iterator List<Type>::Iterator::operator--(int)
{
	if (!node->prevPtr)throw OutOfRangeException();
	Iterator tmp = *this;
	node = node->prevPtr;
	end = false;
	return tmp;
}

template <class Type>
typename List<Type>::Iterator& List<Type>::Iterator::operator+=(OffsetType offset)
{
	if (offset > 0)
		while (offset--)
			this->operator++();
	else if (offset < 0)
		while (offset++)
			this->operator--();
	return *this;

}

template <class Type>
typename List<Type>::Iterator List<Type>::Iterator::operator+(OffsetType offset) const
{
	Iterator tmp = *this;
	tmp += offset;
	return tmp;
}

template <class Type>
typename List<Type>::Iterator& List<Type>::Iterator::operator-=(OffsetType offset)
{
	if (offset > 0)
		while (offset--)
			this->operator--();
	else if (offset < 0)
		while (offset++)
			this->operator++();
	return *this;
}

template <class Type>
typename List<Type>::Iterator List<Type>::Iterator::operator-(OffsetType offset) const
{
	Iterator tmp = *this;
	tmp -= offset;
	return tmp;
}

template <class Type>
bool List<Type>::Iterator::operator==(const Iterator& other) const
{
	return (node == other.node && end == other.end);
}

template <class Type>
bool List<Type>::Iterator::operator!=(const Iterator& other) const
{
	return !this->operator==(other);
}

template <class Type>
bool List<Type>::Iterator::operator>(const Iterator& other) const
{
	return node->index > other->node->index;
}

template <class Type>
bool List<Type>::Iterator::operator>=(const Iterator& other) const
{
	return node->index >= other->node->index;
}

template <class Type>
bool List<Type>::Iterator::operator<(const Iterator& other) const
{
	return node->index < other->node->index;

}

template <class Type>
bool List<Type>::Iterator::operator<=(const Iterator& other) const
{
	return node->index <= other->node->index;

}

template <class Type>
List<Type>::Iterator::Iterator(Node* node, bool end):node(node), end(end)
{
}


//_______________________List implementation______________________________
template<class Type>
List<Type>::List() {
	elementsNumber = 0;
	indexCounter = 0;
	firstPtr = nullptr;
	lastPtr = nullptr;
}

template<class Type>
List<Type>::~List() {
	Node *el = firstPtr;
	Node *tmp;
	while (el) {
		tmp = el;
		el = el->nextPtr;
		delete tmp;
	}
}

template<class Type>
bool List<Type>::PushFirst(Type *object) {
	if (object == nullptr) {
		throw NullReferenceException();
	}
	Node * element = new Node;
	elementsNumber++;
	//If there is no elements in the list
	if (firstPtr == nullptr) {
		firstPtr = element;
		lastPtr = element;
		element->prevPtr = nullptr;
		element->nextPtr = nullptr;
	}
	else {
		firstPtr->prevPtr = element;
		element->prevPtr = nullptr;
		element->nextPtr = firstPtr;
		firstPtr = element;
	}
	element->object = object;
	element->index = -(indexCounter++);
	return true;
}

template <class Type>
bool List<Type>::PushLast(Type *object) {
	Node *element = new Node;
	if (object == nullptr) {
		throw NullReferenceException();
	}
	elementsNumber++;
	//If there is no elements in the list
	if (firstPtr == nullptr) {
		firstPtr = element;
		lastPtr = element;
		element->prevPtr = nullptr;
		element->nextPtr = nullptr;
	}
	else {
		lastPtr->nextPtr = element;
		element->nextPtr = nullptr;
		element->prevPtr = lastPtr;
		lastPtr = element;
	}
	element->object = object;
	element->index = indexCounter++;
	return true;
}
template<class Type>
Type *List<Type>::Get(int index) const {
	Node *element = firstPtr;
	int i = 0;
	while (element != nullptr) {
		if (i++ == index) return element->object;
		element = element->nextPtr;
	}
	return nullptr;
}

template<class Type>
Type *List<Type>::Pop(int index) {
	Node *element = firstPtr;
	bool found = false;
	int i = 0;
	while (element != nullptr) {
		if (i++ == index) {
			found = true;
			break;
		};
		element = element->nextPtr;
	}
	if (found) {
		if (element == firstPtr)
			firstPtr = element->nextPtr;
		else
			element->prevPtr->nextPtr = element->nextPtr;
		if (element == lastPtr)
			lastPtr = element->prevPtr;
		else
			element->nextPtr->prevPtr = element->prevPtr;
		elementsNumber--;
		Type*object = element->object;
		element->object = nullptr;
		delete element;
		return object;
	}

	return nullptr;
}


template<class Type>
Type *List<Type>::GetLast() const {
	if (!lastPtr) return nullptr;
	return lastPtr->object;
}




template<class Type>
Type *List<Type>::GetFirst() const {
	if (!firstPtr) return nullptr;
	return firstPtr->object;
}
template<class Type>
Type *List<Type>::PopFirst() {
	if (firstPtr == nullptr) return nullptr;
	Node *element = firstPtr;
	firstPtr = element->nextPtr;
	if (lastPtr == element)
		lastPtr = nullptr;
	if (element->nextPtr)element->nextPtr->prevPtr = nullptr;
	elementsNumber--;
	Type*object = element->object;
	element->object = nullptr;
	delete element;
	return object;
}
template<class Type> Type *List<Type>::PopLast() {
	if (!lastPtr) return nullptr;
	Node *element = lastPtr;
	lastPtr = element->prevPtr;
	if (firstPtr == element)
		firstPtr = nullptr;
	if (element->prevPtr)element->prevPtr->nextPtr = nullptr;
	elementsNumber--;
	Type*object = element->object;
	element->object = nullptr;
	delete element;
	return object;
}
template<class Type>
bool List<Type>::Delete(int index) {
	Node *element = getElement(index);
	if (element == nullptr) return false;


	if (element->prevPtr != nullptr && element->nextPtr != nullptr) {
		element->prevPtr->nextPtr = element->nextPtr;
		element->nextPtr->prevPtr = element->prevPtr;
	}
	else {
		if (element->prevPtr == nullptr && element->nextPtr == nullptr) {
			firstPtr = nullptr;
			lastPtr = nullptr;
		}
		else {
			if (element->prevPtr == nullptr) {
				firstPtr = element->nextPtr;
				element->nextPtr->prevPtr = nullptr;
			}
			if (element->nextPtr == nullptr) {
				lastPtr = element->prevPtr;
				element->prevPtr->nextPtr = nullptr;
			}
		}
	}
	elementsNumber--;
	delete element;
	return true;
}

template <class Type>
typename List<Type>::Iterator List<Type>::begin()
{
	return Iterator(firstPtr, false);
}

template <class Type>
typename List<Type>::Iterator List<Type>::end()
{
	return Iterator(lastPtr, true);
}

template<class Type>
typename List<Type>::Node *List<Type>::getElement(int index) {
	Node *element = firstPtr;
	int i = 0;
	while (element != nullptr) {
		if (i++ == index) return element;
		element = element->nextPtr;
	}
	return nullptr;
}
template <class Type>
int List<Type>::GetSize() const {
	return elementsNumber;
}


#endif //KRLIB_LIST_H 