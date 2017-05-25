#pragma once
#include "Exception.h"
enum Duplicates { FORBID, IGNORE, ALLOW, OVERRIDE };

//_____________________________BinaryTree header______________________________________
template <class Key, class Value>
class BinaryTree
{
	class Node;
	class Iterator;
public:

	BinaryTree(Duplicates duplicatesBehaviour=ALLOW);
	BinaryTree(const BinaryTree&other);
	~BinaryTree();

	void push(Key*key, Value*value);
	void remove(Key*key, Value*value);//TODO: implement
	void update(Key*key, Value*value)throw(NotFoundException);
	Value*pull(Key*key, Value*value);//TODO: implement
	Value*peek(Key*key);

	Iterator begin();
	Iterator end();
	void print();


private:
	Node*find(const Key*key)throw(NullReferenceException);
	Node*min(Node*node);
	Node*max(Node*node);
	Node*successor(const Node*node)throw(NullReferenceException);
	Node*predecessor(const Node*node)throw(NullReferenceException);
	Node*removeNode(const Node*node);//TODO: implement
	void removeTree(Node*root);

	Node*root;
	Duplicates duplicatesBehaviour;
	friend Iterator;
};


//__________________________________Node header______________________________________
template <class Key, class Value> class BinaryTree<Key, Value>::Node
{
	Node();
	Node(Key*key=nullptr,Value*value=nullptr, Node*left = nullptr, Node*right = nullptr, Node*up = nullptr);
	~Node();
	Key*key;
	Value*value;
	Node*left;
	Node*right;
	Node*up;
	friend BinaryTree;
	bool operator==(const Node&other)const;
	bool operator!=(const Node&other)const;
	bool operator>=(const Node&other)const;
	bool operator<=(const Node&other)const;
	bool operator>(const Node&other)const;
	bool operator<(const Node&other)const;
};



//__________________________________Iterator header_____________________________________

template <class Key, class Value> class BinaryTree<Key, Value>::Iterator
{
	typedef int OffsetValue;
public:
	Iterator();
	Iterator(const Iterator&other);
	~Iterator();

	const Value* operator*()const;
	//Value* operator->()const;
	Iterator& operator++();
	Iterator operator++(int);
	Iterator& operator--();
	Iterator operator--(int);
	Iterator& operator+=(OffsetValue offset);
	Iterator operator+(OffsetValue offset)const;
	Iterator& operator-=(OffsetValue offset);
	Iterator operator-(OffsetValue offset)const;
	bool operator==(const Iterator&other)const;
	bool operator!=(const Iterator&other)const;
	bool operator>(const Iterator&other)const;
	bool operator>=(const Iterator&other)const;
	bool operator<(const Iterator&other)const;
	bool operator<=(const Iterator&other)const;



private:
	/*explicit*/ Iterator(Node*node, bool end, BinaryTree*tree) :node(node), end(end), tree(tree) {}
	Node*node;
	BinaryTree* tree;
	bool end;
	friend BinaryTree;
};



///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////implementation////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


//_____________________________________BinaryTree implementation_____________________________


template <class Key, class Value>
BinaryTree<Key, Value>::BinaryTree(Duplicates duplicatesBehaviour):root(nullptr), duplicatesBehaviour(duplicatesBehaviour)
{
}

template <class Key, class Value>
BinaryTree<Key, Value>::BinaryTree(const BinaryTree& other)
{

}

template <class Key, class Value>
BinaryTree<Key, Value>::~BinaryTree()
{
	removeTree(root);
}

template <class Key, class Value>
void BinaryTree<Key, Value>::push(Key* key, Value* value)
{
	if (!key)
		throw NullReferenceException();
	if (find(key))
		switch (duplicatesBehaviour)
		{
		case FORBID:
			//delete objectToPush;
			throw DuplicateException();
			break;
		case IGNORE:
			//delete objectToPush;
			return;
		case ALLOW: break;
		case OVERRIDE:
			update(key, value);
			return;
			break;
		default:;
		}
	Node*newNode = new Node(key, value, nullptr, nullptr, nullptr);
	if (!root) {
		root = newNode;
		return;
	}
	Node*node = root;
	while (true)
	{
		if (*newNode > *node) {
			if (!node->right)
			{
				node->right = newNode;
				newNode->up = node;
				return;
			}
			node = node->right;
		}
		else
		{
			if (!node->left)
			{
				node->left = newNode;
				newNode->up = node;
				return;
			}
			node = node->left;
		}
	}
}

template <class Key, class Value>
void BinaryTree<Key, Value>::remove(Key* key, Value* value)
{
	throw NotImplementedException();

}

template <class Key, class Value>
void BinaryTree<Key, Value>::update(Key* key, Value* value) throw(NotFoundException)
{
	if (!key)
		throw NullReferenceException();
	Node*node = find(key);
	if (!node) {
		throw NotFoundException();
	}
	delete node->value;
	node->value = value;
}

template <class Key, class Value>
Value* BinaryTree<Key, Value>::pull(Key* key, Value* value)
{
	throw NotImplementedException();
}

template <class Key, class Value>
Value* BinaryTree<Key, Value>::peek(Key* key)
{
	Node*result = find(key);
	if (result)return result->value;
	return nullptr;
}


template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator BinaryTree<Key, Value>::begin()
{
	return Iterator(min(root), false, this);
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator BinaryTree<Key, Value>::end()
{
	return Iterator(max(root), true, this);
}



template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::find(const Key* key) throw(NullReferenceException)
{
	if (!key)
		throw NullReferenceException();
	if (!root)
		return nullptr;
	Node*result = root;
	while (*result->key != *key)
	{
		if (*key < *result->key)
			result = result->left;
		else
			result = result->right;
		if (!result)
			return nullptr;
	}
	return result;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::min(Node*node)
{
	if (!node)
		return nullptr;
	Node*result;
	result = node;
	while (result->left)
		result = result->left;
	return result;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::max(Node*node)
{
	if (!node)
		return nullptr;
	Node*result;
	result = node;
	while (result->right)
		result = result->right;
	return result;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::successor(const Node* node)
{
	if (!node)
		throw NullReferenceException();
	if (node->right)
		return min(node->right);
	Node*upNode = node->up;
	while (upNode && node == upNode->right)
	{
		node = upNode;
		upNode = upNode->up;
	}
	return upNode;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::predecessor(const Node* node)
{
	if (!node)
		throw NullReferenceException();
	if (node->left)
		return max(node->left);
	Node*upNode = node->up;
	while (upNode && node == upNode->left)
	{
		node = upNode;
		upNode = upNode->up;
	}
	return upNode;
}

/**
 * \brief remove node from tree structure
 * \param node
 * \return ptr to deleted node
 */
template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::removeNode(const Node* node)
{
	if (!node)
		throw NullReferenceException();
	if (!root)
		return nullptr;
	//1. No children
	if (!node->left && !node->right)
	{
		if (node == root)
			root = nullptr;
		else
			if (node->up->left == node)
				node->up->left = nullptr;
			else if (node->up->right == node)
				node->up->right = nullptr;
		return node;
	}
	//2. Only one child
	if ((node->left && !node->right) || (!node->left&&node->right))
	{
		if (node == root)
			root = (node->left) ? node->left : node->right;
		else
			if (node->up->left == node)
				node->up->left = (node->left) ? node->left : node->right;
			else if (node->up->right == node)
				node->up->right = (node->left) ? node->left : node->right;
		return node;
	}
	//3. Two children present
	Node*result = removeNode(successor(node));
	Node*toRemove = node;
	node = result;
	return toRemove;
}

template <class Key, class Value>
void BinaryTree<Key, Value>::removeTree(Node* root)
{
	if (!root)
		return;
	if (root->left)
		removeTree(root->left);
	if (root->right)
		removeTree(root->right);
	if (!root->left && !root->right)
		delete root;
}

template <class Key, class Value>
void BinaryTree<Key, Value>::print()
{
	Node*tmp = min(root);
	while (tmp)
	{
		cout << *tmp->value << endl;
		tmp = successor(tmp);
	}
}




//__________________________________________Node implementation______________________________



template <class Key, class Value>
BinaryTree<Key, Value>::Node::Node()
{
}

template <class Key, class Value>
BinaryTree<Key, Value>::Node::Node(Key* key, Value* value, Node* left, Node* right, Node* up):key(key),value(value),left(left),right(right),up(up)
{
}

template <class Key, class Value>
BinaryTree<Key, Value>::Node::~Node()
{
	if (value)
		delete value;
	if (key)
		delete key;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Node::operator==(const Node& other) const
{
	return *key == *other.key;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Node::operator!=(const Node& other) const
{
	return *key != *other.key;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Node::operator>=(const Node& other) const
{
	return *key >= *other.key;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Node::operator<=(const Node& other) const
{
	return *key <= *other.key;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Node::operator>(const Node& other) const
{
	return *key > *other.key;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Node::operator<(const Node& other) const
{
	return *key < *other.key;
}



//_______________________________________Iterator implementation______________________________


template <class Key, class Value>
BinaryTree<Key, Value>::Iterator::Iterator() :node(nullptr), end(false), tree(nullptr)
{

}

template <class Key, class Value>
BinaryTree<Key, Value>::Iterator::Iterator(const Iterator& other) :node(other.node), end(other.end), tree(other.tree)
{
}

template <class Key, class Value>
BinaryTree<Key, Value>::Iterator::~Iterator()
{
}

template <class Key, class Value>
const Value* BinaryTree<Key, Value>::Iterator::operator*() const
{
	if (!node || end)return nullptr;
	return node->value;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator& BinaryTree<Key, Value>::Iterator::operator++()
{
	Node*succ = tree->successor(node);
	if (!succ)
	{
		end = true;
		return *this;
	}
	node = succ;
	return *this;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator BinaryTree<Key, Value>::Iterator::operator++(int)
{
	Node*succ = tree->successor(node);
	Iterator iterator = *this;
	if (!succ)
	{
		end = true;
		return iterator;
	}
	node = succ;
	return iterator;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator& BinaryTree<Key, Value>::Iterator::operator--()
{
	Node*pre = tree->predecessor(node);
	if (!pre)
		throw OutOfRangeException();
	node = pre;
	end = false;
	return *this;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator BinaryTree<Key, Value>::Iterator::operator--(int)
{
	Node*pre = tree->predecessor(node);
	if (!pre)
		throw OutOfRangeException();
	Iterator iterator = *this;
	node = pre;
	end = false;
	return iterator;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator& BinaryTree<Key, Value>::Iterator::operator+=(OffsetValue offset)
{
	if (offset > 0)
		while (offset--)
			this->operator++();
	else if (offset < 0)
		while (offset++)
			this->operator--();
	return *this;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator BinaryTree<Key, Value>::Iterator::operator+(OffsetValue offset) const
{
	Iterator result = *this;
	result += offset;
	return result;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator& BinaryTree<Key, Value>::Iterator::operator-=(OffsetValue offset)
{
	if (offset > 0)
		while (offset--)
			this->operator--();
	else if (offset < 0)
		while (offset++)
			this->operator++();
	return *this;
}

template <class Key, class Value>
typename BinaryTree<Key, Value>::Iterator BinaryTree<Key, Value>::Iterator::operator-(OffsetValue offset) const
{
	Iterator result = *this;
	result -= offset;
	return result;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Iterator::operator==(const Iterator& other) const
{
	return (node == other.node) && (end == other.end);
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Iterator::operator!=(const Iterator& other) const
{
	return !(operator==(other));
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Iterator::operator>(const Iterator& other) const
{
	return *node > *other.node;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Iterator::operator>=(const Iterator& other) const
{
	return *node >= *other.node;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Iterator::operator<(const Iterator& other) const
{
	return *node < *other.node;
}

template <class Key, class Value>
bool BinaryTree<Key, Value>::Iterator::operator<=(const Iterator& other) const
{
	return *node <= *other.node;
}
