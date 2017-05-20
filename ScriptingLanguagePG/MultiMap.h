#pragma once
#include "Map.h"
template <class Key, class Value>
class MultiMap : Map<Key, Value>
{
	typedef Map<Key, Value> base;
	typedef RedBlackTree<Key, Value> nestor;
public:
	MultiMap();
	~MultiMap();
	void push(Key*key, Value*value);
};

template <class Key, class Value>
MultiMap<Key, Value>::MultiMap() : Map<Key, Value>()
{
}

template <class Key, class Value>
MultiMap<Key, Value>::~MultiMap()
{
}

template <class Key, class Value>
void MultiMap<Key, Value>::push(Key* key, Value* value)
{
	nestor::pushMulti(key, value);
}

