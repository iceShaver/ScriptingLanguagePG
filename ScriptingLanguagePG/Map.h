#pragma once
#include "RedBlackTree.h"
template <class Key, class Value>
class Map : private RedBlackTree<Key, Value>
{
	typedef RedBlackTree<Key, Value> base;
public:
	typedef RedBlackTree<Key, Value>::Iterator Iterator;
	/*explicit*/ Map(Duplicates = FORBID);
	~Map();
	void push(Key*key, Value*value) throw(DuplicateException);
	void update(Key*key, Value*value) throw(NotFoundException);
	void remove(Key*key);

	Iterator begin() { return base::begin(); }
	Iterator end() { return base::end(); };
	Value*pull(Key*key);
	const Value*peek(Key*key);
	void print();
};

template <class Key, class Value>
Map<Key, Value>::Map(Duplicates duplicatesBahaviour) :RedBlackTree<Key, Value>(duplicatesBahaviour)
{
}

template <class Key, class Value>
Map<Key, Value>::~Map()
{
}

template <class Key, class Value>
void Map<Key, Value>::push(Key* key, Value* value)
{
	base::push(key, value);
}

template <class Key, class Value>
void Map<Key, Value>::update(Key* key, Value* value) throw(NotFoundException)
{
	base::update(key, value);
}

template <class Key, class Value>
void Map<Key, Value>::remove(Key* key)
{
	base::remove(key);
}

template <class Key, class Value>
Value* Map<Key, Value>::pull(Key* key)
{
	return base::pull(key);
}

template <class Key, class Value>
const Value* Map<Key, Value>::peek(Key* key)
{
	return base::peek(key);
}

template <class Key, class Value>
void Map<Key, Value>::print()
{
	base::print();
}

