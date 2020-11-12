#ifndef __RBTREE_H__
#define __RBTREE_H__

#include <iostream>
using namespace std;

// 图例：https://my.oschina.net/u/3272058/blog/1914452

enum Color {
	RED,
	BLACK
};

template<class K, class V>
struct RBTreeNode {
	K _key;
	V _value;
	Color _color;
	RBTreeNode<K, V> *_left;
	RBTreeNode<K, V> *_right;
	RBTreeNode<K, V> *_parent;

	RBTreeNode(const K &key=K(), const V &value=V()) : _key(key), _value(value), _color(RED), _left(NULL), _right(NULL), _parent(NULL){}
};

template<class K, class V>
class RBTree {
public:
	RBTree() : _root(NULL) {}
	bool Insert(const K &key, const V &value);
	void InOrder() {
		_InOrder(_root);
		cout << endl;
	}
	bool Check();
protected:
	void RotateL(RBTreeNode<K, V>*& parent);
	void RotateR(RBTreeNode<K, V>*& parent);
	void _InOrder(RBTreeNode<K, V>* root);
	bool _Check(RBTreeNode<K, V>* root, int blackNum, int CBNum);
protected:
	RBTreeNode<K, V> *_root;
};

template<class K, class V>
bool RBTree<K, V>::Insert(const K &key, const V &value) {
	// first insert
	if(_root == NULL) {
		_root = new RBTreeNode<K, V>(key, value);
		_root->_color = BLACK;
		return true;
	}

	// find position
	RBTreeNode<K, V>* cur = _root;
	RBTreeNode<K, V>* parent = NULL;
	while(cur) {
		if(cur->_key > key) {
			parent = cur;
			cur = cur->_left;
		} else if(cur->_key < key) {
			parent = cur;
			cur = cur->_right;
		} else {
			return false;
		}
	}

	// insert
	cur = new RBTreeNode<K, V>(key, value);
	cur->_parent = parent;
	if(parent->_key > key) {
		parent->_left = cur;
	} else if(parent->_key < key) {
		parent->_right = cur;
	}

	// check
	while(parent && parent->_color == RED) {
		RBTreeNode<K, V>* grandfather = parent->_parent;
		if(parent == grandfather->_left) {
			RBTreeNode<K, V>* uncle = grandfather->_right;
			if(uncle && uncle->_color == RED) {
				grandfather->_color = RED;
				parent->_color = BLACK;
				uncle->_color = BLACK;
				cur = grandfather;
				parent = grandfather->_parent;
			} else if((uncle && uncle->_color == BLACK) || uncle==NULL) {
				if(cur == parent->_left) {
					parent->_color = BLACK;
					grandfather->_color = RED;
					RotateR(grandfather);
				} else {
					RotateL(parent);
					parent->_color = BLACK;
					grandfather->_color = RED;
					RotateR(grandfather);
				}
				break;
			}
		} else if(parent == grandfather->_right) {
			RBTreeNode<K, V>* uncle = grandfather->_left;
			if(uncle && uncle->_color == RED) {
				grandfather->_color = RED;
				parent->_color = BLACK;
				uncle->_color = BLACK;
				cur = grandfather;
				parent = cur->_parent;
			} else if((uncle && uncle->_color == BLACK) || uncle==NULL) {
				if (cur == parent->_right) {
					parent->_color = BLACK;
					grandfather->_color = RED;
					RotateL(grandfather);
				}
				else if (cur==parent->_left) {
					RotateR(parent);
					parent->_color = BLACK;
					grandfather->_color = RED;	
					RotateL(grandfather);
				}
				break;
			}
		}
	}
	_root->_color = BLACK;
	return true;
}

template<class K, class V>
void RBTree<K, V>::RotateL(RBTreeNode<K, V>*& parent) {
	RBTreeNode<K, V>* ppNode = parent->_parent;
	RBTreeNode<K, V>* subR = parent->_right;
	RBTreeNode<K, V>* subRL = subR->_left;

	parent->_right = subRL;
	if(subRL) subRL->_parent = parent;

	subR->_left = parent;
	parent->_parent = subR;

	if(ppNode == NULL) {
		subR->_parent = NULL;
		_root = subR;
	} else {
		subR->_parent = ppNode;
		if(ppNode->_left == parent) {
			ppNode->_left = subR;
		} else if(ppNode->_right == parent) {
			ppNode->_right = subR;
		}
	}

	parent = subR;
}

template<class K, class V>
void RBTree<K, V>::RotateR(RBTreeNode<K, V>*& parent) {
	RBTreeNode<K, V>* ppNode = parent->_parent;
	RBTreeNode<K, V>* subL = parent->_left;
	RBTreeNode<K, V>* subLR = subL->_right;

	parent->_left = subLR;
	if(subLR) subLR->_parent = parent;

	subL->_right = parent;
	parent->_parent = subL;

	if(ppNode==NULL) {
		subL->_parent = NULL;
		_root = subL;
	} else {
		subL->_parent = ppNode;
		if(parent == ppNode->_left) {
			ppNode->_left = subL;
		}
		else {
			ppNode->_right = subL;
		}
	}
	
	parent = subL;
}

template<class K, class V>
void RBTree<K, V>::_InOrder(RBTreeNode<K, V>* root) {
	if (root == NULL) {
		return;
	}
	_InOrder(root->_left);
	cout << root->_key << " ";
	_InOrder(root->_right);

}

template<class K, class V>
bool RBTree<K, V>::Check() {
	int blackNum = 0;
	RBTreeNode<K, V>* cur = _root;
	while (cur) {
		if (cur->_color == BLACK)
			blackNum++;
		cur = cur->_left;
	}
	int CBNum = 0;
	return _Check(_root,blackNum,CBNum);
}

template<class K, class V>
bool RBTree<K, V>::_Check(RBTreeNode<K, V>* root, int blackNum, int CBNum) {
	if (root == NULL)
		return true;
	if (root->_color == BLACK) {
		CBNum++;
		if (root->_left == NULL&&root->_right == NULL) {	
			if (blackNum == CBNum) {
				return true;
			} else {
				cout << "叶子节点为" << root->_key << "路径的黑色节点数目与最左侧支路的黑色节点数目不相等 ！" << endl;
				return false;
			}
		}
	} else if (root->_parent&&root->_parent->_color == RED) {
		cout << root->_parent->_key << " 和 " << root->_key << " 为两个连续的红色节点" << endl;
		return false;
	}
	return _Check(root->_left, blackNum, CBNum) && _Check(root->_right, blackNum, CBNum);
}

#endif
