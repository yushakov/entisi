#include "bst.h"
#include <stdio.h>

BinarySearchTree::BinarySearchTree(int initial_node_count)
{
	node_count = initial_node_count;
	nodes = new TreeNode[node_count];
	root = 0;
	node_idx = 0;
}

BinarySearchTree::~BinarySearchTree()
{
	delete[] nodes;
}

void BinarySearchTree::put(bis key, int val, int order)
{
	if (root)
	{
		root->ins(key, val, order, &nodes[node_idx], node_idx);
	}
	else
	{
		root = &nodes[0];
		root->ins(key, val, order, 0, node_idx);
		node_idx++;
	}
	if (node_idx >= node_count)
	{
		TreeNode *new_memory = new TreeNode[2 * node_count];
		//printf("size: %d\n", node_count);
		for (int i = 0; i < node_idx; i++)
		{
			new_memory[i] = nodes[i];
			new_memory[i].updatePointers(root, &new_memory[0]);
		}
		delete[] nodes;
		nodes = new_memory;
		root = &nodes[0];
		node_count *= 2;
	}
}

void BinarySearchTree::inorder(void(*callback_fun)(bis _key, int _val, void*), void *p)
{
	if (root)
	{
		root->inorder(callback_fun, p);
	}
}

int BinarySearchTree::getDepth(int &dp)
{
	if (root) return root->getDepth(dp);
	return 0;
}

//////////////////////////////////////////////////

int compare(bis a, bis b, int order)
{
	if (a == b) return 0;
	else
	{
#ifdef BITSET_SIZE
		for (int i = order-1; i >= 0; i--)
		{
			if      ( a[i] && !b[i]) return  1; // a > b
			else if (!a[i] &&  b[i]) return -1; // a < b
		}
#else
		if (a > b) return 1;
		if (a < b) return -1;
#endif
	}
}

TreeNode::TreeNode()
{
	key   = -1;
	val   = -1;
	left  =  0;
	right =  0;
}

TreeNode::~TreeNode()
{
	key   = -1;
	val   = -1;
	left  =  0;
	right =  0;
}

void TreeNode::ins(bis _key, int _val, int _order, TreeNode *p, int &idx)
{
	if (!p)
	{
		key = _key;
		val = _val;
		return;
	}
	switch (compare(_key, key, _order)){
	case 0:
	{
		val += _val;
	}
	break;
	case -1:
	{
		if (left)
		{
			left->ins(_key, _val, _order, p, idx);
		}
		else
		{
			left = p;
			idx++;
			left->key = _key;
			left->val = _val;
		}
	}
	break;
	case 1:
	{
		if (right)
		{
			right->ins(_key, _val, _order, p, idx);
		}
		else
		{
			right = p;
			idx++;
			right->key = _key;
			right->val = _val;
		}
	}
	break;
	}
}

int TreeNode::getDepth(int &dp)
{
	int ld = 0, rd = 0;
	if (left)
	{
		ld++;
		left->getDepth(ld);
	}
	if (right)
	{
		rd++;
		right->getDepth(rd);
	}
	if (ld >= rd) dp += ld;
	else dp += rd;
	return 0;
}

void TreeNode::inorder(void(*callback_fun)(bis _key, int _val, void* p), void *p)
{
	if (left)
	{
		left->inorder(callback_fun, p);
	}
	callback_fun(key, val, p);
	if (right)
	{
		right->inorder(callback_fun, p);
	}
}

void TreeNode::updatePointers(TreeNode *old_root, TreeNode *new_root)
{
	if (left)
	{
		left = new_root + (left - old_root);
	}
	if (right)
	{
		right = new_root + (right - old_root);
	}
}