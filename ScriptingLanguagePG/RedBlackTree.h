/*
 * TODO: reverse itarators
 * TODO: deal with getters consts, destructor
 */
#pragma once
#include "String.h"
#include <iostream>
enum Color { RED, BLACK };
enum Duplicates { FORBID, IGNORE, ALLOW, OVERRIDE };


//______________________________________________RedBlackTree class header_______________________________________________
template <class Key, class Value>
class RedBlackTree
{

public:
	class Iterator;
	//____________________________________Node class header________________________________________________________
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
	/*explicit*/ RedBlackTree(Duplicates = FORBID);
	~RedBlackTree();
	void push(Key*key, Value*value)throw(DuplicateException);
	void update(Key*key, Value*value)throw(NotFoundException);
	Value* pull(Key*key);
	Value* peek(Key*key);
	void remove(const Key&key);
	Iterator begin();
	Iterator end();
	Node*getRoot();
	const Node*getMinNode();
private:
	void removeTreeDFS(Node*node);
	Node*find(const Key*key);
	Node*getMin(Node*node);
	Node*getMax(Node*node);
	Node*successor(const Node*node);
	Node*predecessor(const Node*node);
	void rotateLeft(Node*axis);
	void rotateRight(Node*axis);
	void removeNode(Node*nodeToRemove);
	Node terminator;
	Node*root;
	Duplicates duplicatesBehaviour;
	friend Iterator;
};


//_______________________________________Iterator class header________________________________________________
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


//___________________________________________RedBlackTree class implementation______________________________________________
template <class Key, class Value>
RedBlackTree<Key, Value>::RedBlackTree(Duplicates duplicatesBehaviour)
{
	terminator.color = BLACK;
	terminator.parent = &terminator;
	terminator.left = &terminator;
	terminator.right = &terminator;
	root = &terminator;
	this->duplicatesBehaviour = duplicatesBehaviour;
}

template <class Key, class Value>
RedBlackTree<Key, Value>::~RedBlackTree()
{
	removeNode(root);
}


template <class Key, class Value>
void RedBlackTree<Key, Value>::push(Key* key, Value* value) throw(DuplicateException)
{
	if (find(key))
	{
		switch (duplicatesBehaviour)
		{
		case FORBID:
			throw DuplicateException();
		case IGNORE:
			return;
		case ALLOW:
			break;
		case OVERRIDE:
			update(key, value);
			break;
		default:
			break;
		}
	}
	Node*tmp;
	Node * newNode = new Node;
	newNode->left = &terminator;
	newNode->right = &terminator;
	newNode->parent = root;
	newNode->value = value;
	newNode->key = key;
	if (newNode->parent == &terminator)
		root = newNode;
	else
		while (true)
		{
			if (*(key) < *(newNode->parent->key))
			{
				if (newNode->parent->left == &terminator)
				{
					newNode->parent->left = newNode;
					break;
				}
				newNode->parent = newNode->parent->left;
			}
			else
			{
				if (newNode->parent->right == &terminator)
				{
					newNode->parent->right = newNode;
					break;
				}
				newNode->parent = newNode->parent->right;
			}
		}


	newNode->color = RED;
	while ((newNode != root) && (newNode->parent->color == RED))
	{
		if (newNode->parent == newNode->parent->parent->left)
		{
			tmp = newNode->parent->parent->right;

			if (tmp->color == RED)
			{
				newNode->parent->color = BLACK;
				tmp->color = BLACK;
				newNode->parent->parent->color = RED;
				newNode = newNode->parent->parent;
				continue;
			}

			if (newNode == newNode->parent->right)
			{
				newNode = newNode->parent;
				rotateLeft(newNode);
			}

			newNode->parent->color = BLACK;
			newNode->parent->parent->color = RED;
			rotateRight(newNode->parent->parent);
			break;
		}

		tmp = newNode->parent->parent->left;

		if (tmp->color == RED)
		{
			newNode->parent->color = BLACK;
			tmp->color = BLACK;
			newNode->parent->parent->color = RED;
			newNode = newNode->parent->parent;
			continue;
		}

		if (newNode == newNode->parent->left)
		{
			newNode = newNode->parent;
			rotateRight(newNode);
		}
		newNode->parent->color = BLACK;
		newNode->parent->parent->color = RED;
		rotateLeft(newNode->parent->parent);
		break;

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
	removeNode(foundNode);
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
void RedBlackTree<Key, Value>::remove(const Key& key)//TODO: absolutely fix that!!!!!!!!!!!!!!!
{
	delete find(key);
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::removeTreeDFS(Node* node)
{
	if (node != &terminator)
	{
		removeTreeDFS(node->left);
		removeTreeDFS(node->right);
		delete node;
	}
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::find(const Key* key)
{
	if (root == &terminator)
		return nullptr;
	Node * result = root;
	while (*(result->key) != *key) {
		if (*key < *(result->key))
			result = result->left;
		else
			result = result->right;
		if (result == &terminator)
			return nullptr;
	}
	return result;
}



template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::getMin(Node* node)
{
	if (node != &terminator)
		while (node->left != &terminator)
			node = node->left;
	return node;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::getMax(Node* node)
{
	if (node != &terminator)
		while (node->right != &terminator)
			node = node->right;
	return node;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::successor(const Node* node)
{
	if (node != &terminator)
	{
		if (node->right != &terminator)
			return getMin(node->right);
		{
			Node * r = node->parent;
			while ((r != &terminator) && (node == r->right))
			{
				node = r;
				r = r->parent;
			}
			return r;
		}
	}
	return &terminator;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::predecessor(const Node* node)
{
	if (node != &terminator)
	{
		if (node->left != &terminator)
			return getMax(node->left);
		Node*r = node->parent;
		while ((r != &terminator) && node == r->left)
		{
			node = r;
			r = r->parent;
		}
		return r;

	}
	return &terminator;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::getRoot()
{
	return root;
}

template <class Key, class Value>
const typename RedBlackTree<Key, Value>::Node* RedBlackTree<Key, Value>::getMinNode()
{
	return getMin(root);
}



template <class Key, class Value>
void RedBlackTree<Key, Value>::rotateLeft(Node* axis)
{
	Node * tmp = axis->right;
	if (tmp != &terminator)
	{
		Node *parent = axis->parent;
		axis->right = tmp->left;
		if (axis->right != &terminator) axis->right->parent = axis;
		tmp->left = axis;
		tmp->parent = parent;
		axis->parent = tmp;
		if (parent != &terminator)
			if (parent->left == axis)
				parent->left = tmp;
			else 
				parent->right = tmp;
		else 
			root = tmp;
	}

}

template <class Key, class Value>
void RedBlackTree<Key, Value>::rotateRight(Node* axis)
{
	Node * tmp = axis->left;
	if (tmp != &terminator)
	{
		Node *parent = axis->parent;
		axis->left = tmp->right;
		if (axis->left != &terminator) axis->left->parent = axis;
		tmp->right = axis;
		tmp->parent = parent;
		axis->parent = tmp;
		if (parent != &terminator)
			if (parent->left == axis)
				parent->left = tmp;
		else 
			parent->right = tmp;
		else 
			root = tmp;
	}
}

template <class Key, class Value>
void RedBlackTree<Key, Value>::removeNode(Node* nodeToRemove)
{
	Node * tmpNode1, *tmpNode2, *tmpNode3;

	if ((nodeToRemove->left == &terminator) || (nodeToRemove->right == &terminator))
		tmpNode2 = nodeToRemove;
	else
		tmpNode2 = successor(nodeToRemove);

	if (tmpNode2->left != &terminator)
		tmpNode3 = tmpNode2->left;
	else
		tmpNode3 = tmpNode2->right;
	tmpNode3->parent = tmpNode2->parent;
	if (tmpNode2->parent == &terminator)
		root = tmpNode3;
	else if (tmpNode2 == tmpNode2->parent->left)
		tmpNode2->parent->left = tmpNode3;
	else
		tmpNode2->parent->right = tmpNode3;

	if (tmpNode2 != nodeToRemove)
		nodeToRemove->key = tmpNode2->key;

	if (tmpNode2->color == BLACK)
		while ((tmpNode3 != root) && (tmpNode3->color == BLACK))
			if (tmpNode3 == tmpNode3->parent->left)
			{
				tmpNode1 = tmpNode3->parent->right;

				if (tmpNode1->color == RED)
				{
					tmpNode1->color = BLACK;
					tmpNode3->parent->color = RED;
					rotateLeft(tmpNode3->parent);
					tmpNode1 = tmpNode3->parent->right;
				}

				if ((tmpNode1->left->color == BLACK) && (tmpNode1->right->color == BLACK))
				{
					tmpNode1->color = RED;
					tmpNode3 = tmpNode3->parent;
					continue;
				}

				if (tmpNode1->right->color == BLACK)
				{
					tmpNode1->left->color = BLACK;
					tmpNode1->color = RED;
					rotateRight(tmpNode1);
					tmpNode1 = tmpNode3->parent->right;
				}

				tmpNode1->color = tmpNode3->parent->color;
				tmpNode3->parent->color = BLACK;
				tmpNode1->right->color = BLACK;
				rotateLeft(tmpNode3->parent);
				tmpNode3 = root;
			}
			else
			{
				tmpNode1 = tmpNode3->parent->left;

				if (tmpNode1->color == RED)
				{
					tmpNode1->color = BLACK;
					tmpNode3->parent->color = RED;
					rotateRight(tmpNode3->parent);
					tmpNode1 = tmpNode3->parent->left;
				}

				if ((tmpNode1->left->color == BLACK) && (tmpNode1->right->color == BLACK))
				{
					tmpNode1->color = RED;
					tmpNode3 = tmpNode3->parent;
					continue;
				}

				if (tmpNode1->left->color == BLACK)
				{
					tmpNode1->right->color = BLACK;
					tmpNode1->color = RED;
					rotateLeft(tmpNode1);
					tmpNode1 = tmpNode3->parent->left;
				}

				tmpNode1->color = tmpNode3->parent->color;
				tmpNode3->parent->color = BLACK;
				tmpNode1->left->color = BLACK;
				rotateRight(tmpNode3->parent);
				tmpNode3 = root;
			}

	tmpNode3->color = BLACK;

	delete tmpNode2;
}


//__________________________________________Iterator class implementation____________________________________
template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::begin()
{
	return Iterator(getMin(root), false, this);
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::end()
{
	return Iterator(getMax(root), true, this);
}


template <class Key, class Value>
RedBlackTree<Key, Value>::Iterator::Iterator() :node(nullptr), end(false), redBlackTree(nullptr)
{
}

template <class Key, class Value>
RedBlackTree<Key, Value>::Iterator::Iterator(const Iterator& other) : node(other.node), end(other.end), redBlackTree(other.redBlackTree)
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
	if (succ == &redBlackTree->terminator)
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
	if (succ == &redBlackTree->terminator)
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
	if (pre == &redBlackTree->terminator) throw OutOfRangeException();
	node = pre;
	end = false;
	return *this;
}

template <class Key, class Value>
typename RedBlackTree<Key, Value>::Iterator RedBlackTree<Key, Value>::Iterator::operator--(int)
{
	Node*pre = redBlackTree->predecessor(node);
	if (pre == &redBlackTree->terminator) throw OutOfRangeException();
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
			this->operator--();
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
	return (node == other.node) && (end == other.end);
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
RedBlackTree<Key, Value>::Iterator::Iterator(Node* node, bool end, RedBlackTree*rbt) :node(node), end(end), redBlackTree(rbt)
{
}

//_______________________________________Node class implementation________________________________________________
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
