/*
 * TODO: reverse itarators
 * TODO: deal with getters consts, destructor
 */
#pragma once
#include "String.h"
#include <iostream>
enum Color { RED, BLACK };
enum Duplicates{FORBID, IGNORE, ALLOW, OVERRIDE};
template <class Key, class Value>
class RedBlackTree
{
	
public:
	class Iterator;
	class Node
	{
	public:
		Value*getValue();
		Value*getKey()const;
	private:
		Node();
		Node(Key* key, Value* value, Node* parent, Node* left, Node* right, Color color);
		Key*key;
		Value*value;
		Node*parent;
		Node*left;
		Node*right;
		Color color;
		friend RedBlackTree;


		friend bool operator==(const Node& lhs, const Node& rhs)
		{
			return lhs.key == rhs.key
				&& lhs.value == rhs.value
				&& lhs.parent == rhs.parent
				&& lhs.left == rhs.left
				&& lhs.right == rhs.right
				&& lhs.color == rhs.color;
		}

		friend bool operator!=(const Node& lhs, const Node& rhs)
		{
			return !(lhs == rhs);
		}


		friend bool operator<(const Node& lhs, const Node& rhs)
		{
			return *lhs.key < *rhs.key;
		}

		friend bool operator<=(const Node& lhs, const Node& rhs)
		{
			return *lhs.key <= *rhs.key;
		}

		friend bool operator>(const Node& lhs, const Node& rhs)
		{
			return *lhs.key > *rhs.key;
		}

		friend bool operator>=(const Node& lhs, const Node& rhs)
		{
			return *lhs.key >= *rhs.key;
		}
	};
	/*explicit*/ RedBlackTree(Duplicates=FORBID);
	~RedBlackTree();
	void push(Key*key, Value*value)throw(DuplicateException);
	void update(Key*key, Value*value)throw(NotFoundException);
	Value* pull(Key*key);
	Value* peek(Key*key);
	void remove(const Key&key);
	void print();
	Iterator begin();
	Iterator end();
	Node*getRoot();
	const Node*getMinNode();
	void iterate();
	void rIterate();
private:
	String cr, cl, cp;
	void printRBT(String sp, String sn, Node*node);
	void DFSRelease(Node*root);
	Node*find(const Key*key);
	Node*min(Node*node);
	Node*max(Node*node);
	Node*successor(const Node*node);
	Node*predecessor(const Node*node);
	void rotateLeft(Node*root);
	void rotateRight(Node*root);
	void remove(Node*nodeToRemove);
	Node sentinel;
	Node*root;
	Duplicates duplicatesBehaviour;
	friend Iterator;
};

template <class Key, class Value>
class RedBlackTree<Key, Value>::Iterator
{
	typedef int OffsetType;
public:
	Iterator();
	Iterator(const Iterator&other);
	~Iterator();

	const Value* operator*()const;
	const Key* getKey()const;
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
	/*explicit*/ Iterator(Node*node, bool end, RedBlackTree*rbt);
	Node*node;
	RedBlackTree * redBlackTree;
	bool end;
	friend RedBlackTree;
};



template <class Key, class Value>
RedBlackTree<Key, Value>::RedBlackTree(Duplicates duplicatesBehaviour)
{
	cr = cl = cp = "  ";
	cr[0] = 218;
	cr[1] = 196;
	cl[0] = 192;
	cl[1] = 196;
	cp[0] = 179;
	sentinel.color = BLACK;
	sentinel.parent = &sentinel;
	sentinel.left = &sentinel;
	sentinel.right = &sentinel;
	root = &sentinel;
	this->duplicatesBehaviour = duplicatesBehaviour;
}

template <class Key, class Value>
RedBlackTree<Key, Value>::~RedBlackTree()
{
	remove(root);
}


template <class Key, class Value>
void RedBlackTree<Key, Value>::push(Key* key, Value* value) throw(DuplicateException)
{
	if(find(key))
	{
		switch (duplicatesBehaviour)
		{
		case FORBID:
			throw DuplicateException();
			break;
		case IGNORE:
			return;
			break;
		case ALLOW:
			break;
		case OVERRIDE:
			update(key, value);
			break;
		default:
			break;
		}
	}
	Node*Y;
	Node * X = new Node;
	X->left = &sentinel;
	X->right = &sentinel;
	X->parent = root;
	X->value = value;
	X->key = key;
	if (X->parent == &sentinel) root = X;
	else
		while (true)
		{
			if (*(key) < *(X->parent->key))
			{
				if (X->parent->left == &sentinel)
				{
					X->parent->left = X;
					break;
				}
				X->parent = X->parent->left;
			}
			else
			{
				if (X->parent->right == &sentinel)
				{
					X->parent->right = X;
					break;
				}
				X->parent = X->parent->right;
			}
		}
	X->color = RED;
	while ((X != root) && (X->parent->color == RED))
	{
		if (X->parent == X->parent->parent->left)
		{
			Y = X->parent->parent->right;

			if (Y->color == RED)
			{
				X->parent->color = BLACK;
				Y->color = BLACK;
				X->parent->parent->color = RED;
				X = X->parent->parent;
				continue;
			}

			if (X == X->parent->right)
			{
				X = X->parent;
				rotateLeft(X);
			}

			X->parent->color = BLACK;
			X->parent->parent->color = RED;
			rotateRight(X->parent->parent);
			break;
		}
		else
		{
			Y = X->parent->parent->left;

			if (Y->color == RED)
			{
				X->parent->color = BLACK;
				Y->color = BLACK;
				X->parent->parent->color = RED;
				X = X->parent->parent;
				continue;
			}

			if (X == X->parent->left)
			{
				X = X->parent;
				rotateRight(X);
			}

			X->parent->color = BLACK;
			X->parent->parent->color = RED;
			rotateLeft(X->parent->parent);
			break;
		}
	}
	root->color = BLACK;

}

template <class Key, class Value>
void RedBlackTree<Key, Value>::update(Key* key, Value* value) throw(NotFoundException)
{
	Node*node = find(key);
	if (!node)throw NotFoundException();
	delete node->value;
	node->value = value;
}

template <class Key, class Value>
Value* RedBlackTree<Key, Value>::pull(Key* key)
{
	Node * foundNode = find(key);
	if (!foundNode) return nullptr;
	Value*result = foundNode->value;
	foundNode->value = nullptr;
	remove(foundNode);
	return result;
}

template <class Key, class Value>
Value* RedBlackTree<Key, Value>::peek(Key* key)
{
	Node*tmp = find(key);
	if (!tmp)return nullptr;
	return tmp->value;
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::remove(const Key& key)
{
	delete find(key);
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::print()
{
	printRBT("", "", root);
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::begin()
{
	return Iterator(min(root), false, this);
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::end()
{
	return Iterator(max(root), true, this);
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::printRBT(String sp, String sn, Node* node)
{
	String t;
	if (node != &sentinel)
	{
		t = sp;
		if (sn == cr) 
			t[t.getLength() - 2] = ' ';
		printRBT(t + cp, cr, node->right);

		t = t.substring(0, sp.getLength() - 2);
		std::cout << t << sn << node->color << ":" << *(node->key) << std::endl;
		t = sp;
		if (sn == cl)
			t[t.getLength() - 2] = ' ';
		printRBT(t + cp, cl, node->left);
	}
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::DFSRelease(Node* root)
{
	if (root != &sentinel)
	{
		DFSRelease(root->left);
		DFSRelease(root->right);
		delete root;
	}
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::find(const Key* key)
{
	if (root == &sentinel)
		return nullptr;
	Node * result = root;
	while ((root != &sentinel)
		&& (*(result->key) != *key)) {
		if (*key < *(result->key))
			result = result->left;
		else
			result = result->right;
		if (result == &sentinel)
			return nullptr;
	}
		return result;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::min(Node* node)
{
	if (node != &sentinel)
		while (node->left != &sentinel)
			node = node->left;
	return node;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::max(Node* node)
{
	if (node != &sentinel)
		while (node->right != &sentinel)
			node = node->right;
	return node;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::successor(const Node* node)
{
	if (node != &sentinel)
	{
		if (node->right != &sentinel) return min(node->right);
		else
		{
			Node * r = node->parent;
			while ((r != &sentinel) && (node == r->right))
			{
				node = r;
				r = r->parent;
			}
			return r;
		}
	}
	return &sentinel;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::predecessor(const Node* node)
{
	if(node!=&sentinel)
	{
		if (node->left != &sentinel)return max(node->left);
		else
		{
			Node*r = node->parent;
			while((r!=&sentinel)&&node==r->left)
			{
				node = r;
				r = r->parent;
			}
			return r;
		}
	}
	return &sentinel;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::getRoot()
{
	return root;
}

template <class Key, class Value>
const typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::getMinNode()
{
	return min(root);
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::iterate()
{
	const Node*node = min(root);
	do
	{
		cout << *(node->key) << endl;
	}
	while ((node=successor(node))!=&sentinel);
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::rIterate()
{
	const Node*node = max(root);
	do
	{
		cout << *(node->key) << endl;
	}
	while ((node=predecessor(node))!=&sentinel);
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::rotateLeft(Node* A)
{
	Node * B = A->right;
	if (B != &sentinel)
	{
		Node *p = A->parent;
		A->right = B->left;
		if (A->right != &sentinel) A->right->parent = A;

		B->left = A;
		B->parent = p;
		A->parent = B;

		if (p != &sentinel)
		{
			if (p->left == A) p->left = B; else p->right = B;
		}
		else root = B;
	}

}

template <class Key, class Value>
void RedBlackTree<Key, Value>::rotateRight(Node* A)
{
	Node * B = A->left;
	if (B != &sentinel)
	{
		Node *p = A->parent;
		A->left = B->right;
		if (A->left != &sentinel) A->left->parent = A;

		B->right = A;
		B->parent = p;
		A->parent = B;

		if (p != &sentinel)
		{
			if (p->left == A) p->left = B; else p->right = B;
		}
		else root = B;
	}
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::remove(Node* nodeToRemove)
{
	Node * W, *Y, *Z;

	if ((nodeToRemove->left == &sentinel) || (nodeToRemove->right == &sentinel)) Y = nodeToRemove;
	else                                    Y = successor(nodeToRemove);

	if (Y->left != &sentinel) Z = Y->left;
	else              Z = Y->right;

	Z->parent = Y->parent;

	if (Y->parent == &sentinel) root = Z;
	else if (Y == Y->parent->left) Y->parent->left = Z;
	else                      Y->parent->right = Z;

	if (Y != nodeToRemove) nodeToRemove->key = Y->key;

	if (Y->color == BLACK) 
		while ((Z != root) && (Z->color == BLACK))
			if (Z == Z->parent->left)
			{
				W = Z->parent->right;

				if (W->color == RED)
				{          
					W->color = BLACK;
					Z->parent->color = RED;
					rotateLeft(Z->parent);
					W = Z->parent->right;
				}

				if ((W->left->color == BLACK) && (W->right->color == BLACK))
				{          
					W->color = RED;
					Z = Z->parent;
					continue;
				}

				if (W->right->color == BLACK)
				{          
					W->left->color = BLACK;
					W->color = RED;
					rotateRight(W);
					W = Z->parent->right;
				}

				W->color = Z->parent->color;
				Z->parent->color = BLACK;
				W->right->color = BLACK;
				rotateLeft(Z->parent);
				Z = root;       
			}
			else
			{                
				W = Z->parent->left;

				if (W->color == RED)
				{            
					W->color = BLACK;
					Z->parent->color = RED;
					rotateRight(Z->parent);
					W = Z->parent->left;
				}

				if ((W->left->color == BLACK) && (W->right->color == BLACK))
				{            
					W->color = RED;
					Z = Z->parent;
					continue;
				}

				if (W->left->color == BLACK)
				{            
					W->right->color = BLACK;
					W->color = RED;
					rotateLeft(W);
					W = Z->parent->left;
				}

				W->color = Z->parent->color; 
				Z->parent->color = BLACK;
				W->left->color = BLACK;
				rotateRight(Z->parent);
				Z = root;       
			}

	Z->color = BLACK;

	delete Y;
}

template <class Key, class Value>
RedBlackTree<Key, Value>::Iterator::Iterator() :node(nullptr), end(false), redBlackTree(nullptr)
{
}

template <class Key, class Value>
RedBlackTree<Key, Value>::Iterator::Iterator(const Iterator& other):node(other.node), end(other.end), redBlackTree(other.redBlackTree)
{
}

template <class Key, class Value>
RedBlackTree<Key, Value>::Iterator::~Iterator()
{
}

template <class Key, class Value>
const Value* RedBlackTree<Key, Value>::Iterator::operator*() const
{
	if (!node || end) throw NullReferenceException();
	return node->value;
}

template <class Key, class Value>
const Key* RedBlackTree<Key, Value>::Iterator::getKey() const
{
	if (!node || end) throw NullReferenceException();
	return node->key;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator& RedBlackTree<Key, Value>::Iterator::operator++()
{
	Node*succ = redBlackTree->successor(node);
	if (succ == &redBlackTree->sentinel)
	{
		end = true;
		return *this;
	}
	node = succ;
	return *this;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::Iterator::operator++(int)
{
	/*
	 * Kandydaci na nastepny element:
	 * 0. minimum prawego syna
	 * 1. rodzic o ile nie lewy
	 * 2. najbli¿szy przodek z prawym synem
	 * 3. 
	 *	
	 */
	Node*succ = redBlackTree->successor(node);
	Iterator iterator = *this;
	if(succ==&redBlackTree->sentinel)
	{
		end = true;
		return iterator;
	}
	node = succ;
	return iterator;

}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator& RedBlackTree<Key, Value>::Iterator::operator--()
{
	Node*pre = redBlackTree->predecessor(node);
	if (pre == &redBlackTree->sentinel) throw OutOfRangeException();
	node = pre;
	end = false;
	return *this;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::Iterator::operator--(int)
{
	Node*pre = redBlackTree->predecessor(node);
	if (pre == &redBlackTree->sentinel) throw OutOfRangeException();
	Iterator iterator = *this;
	node = pre;
	end = false;
	return iterator;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator& RedBlackTree<Key, Value>::Iterator::operator+=(OffsetType offset)
{
	if (offset > 0)
		while (offset--)
			this->operator++();
	else if (offset < 0)
		while (offset++)
			this->operator++();
	return *this;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::Iterator::operator+(OffsetType offset) const
{
	Iterator result = *this;
	result += offset;
	return result;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator& RedBlackTree<Key, Value>::Iterator::operator-=(OffsetType offset)
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
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::Iterator::operator-(OffsetType offset) const
{
	Iterator result = *this;
	result -= offset;
	return result;
}

template <class Key, class Value>
bool RedBlackTree<Key, Value>::Iterator::operator==(const Iterator& other) const
{
	return (node == other.node)&&(end==other.end);
}

template <class Key, class Value>
bool RedBlackTree<Key, Value>::Iterator::operator!=(const Iterator& other) const
{
	return !(operator==(other));
}

template <class Key, class Value>
bool RedBlackTree<Key, Value>::Iterator::operator>(const Iterator& other) const
{
	return *node > *other.node;
}

template <class Key, class Value>
bool RedBlackTree<Key, Value>::Iterator::operator>=(const Iterator& other) const
{
	return *node >= *other.node;
}

template <class Key, class Value>
bool RedBlackTree<Key, Value>::Iterator::operator<(const Iterator& other) const
{
	return *node < *other.node;
}

template <class Key, class Value>
bool RedBlackTree<Key, Value>::Iterator::operator<=(const Iterator& other) const
{
	return *node <= *other.node;
}




template <class Key, class Value>
RedBlackTree<Key, Value>::Iterator::Iterator(Node* node, bool end, RedBlackTree*rbt):node(node),end(end), redBlackTree(rbt)
{
}


template <class Key, class Value>
Value* RedBlackTree<Key, Value>::Node::getValue()
{
	return value;
}

template <class Key, class Value>
Value* RedBlackTree<Key, Value>::Node::getKey()const
{
	return key;
}

template <class Key, class Value>
RedBlackTree<Key, Value>::Node::Node() :key(nullptr), value(nullptr), parent(nullptr), left(nullptr), right(nullptr), color(BLACK)
{
}

template <class Key, class Value>
RedBlackTree<Key, Value>::Node::Node(Key* key, Value* value, Node* parent, Node* left, Node* right, Color color) : key(key), value(value), parent(parent), left(left), right(right), color(color)
{
}
