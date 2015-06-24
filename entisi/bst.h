#ifndef BST_H
#define BST_H

typedef unsigned long long bis;

class TreeNode
{
public:
	TreeNode();
	TreeNode(bis key, int val);
	~TreeNode();
	void ins(bis key, int val, int order, TreeNode *p, int &idx);
	void del(bis key);
	int  getDepth(int &dp);
	void inorder(void(*callback_fun)(bis _key, int _val, void*), void *p);
	void updatePointers(TreeNode *old_root, TreeNode *new_root);
private:
	bis key;
	int val;
	TreeNode *left;
	TreeNode *right;
};

class BinarySearchTree
{
public:
	BinarySearchTree(int initial_nodes_count);
	~BinarySearchTree();
	void put(bis key, int val, int order);
	int  get(bis key);
	void inorder(void(*callback_fun)(bis _key, int _val, void*), void *p);
	int  getDepth(int &dp);
	int  getNodeCount() { return node_idx; }
private:
	TreeNode *root;
	TreeNode *nodes;
	int       node_count;
	int       node_idx;
};

#endif