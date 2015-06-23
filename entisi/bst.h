#ifndef BST_H
#define BST_H

class TreeNode
{
public:
	TreeNode();
	TreeNode(int key, int val);
	~TreeNode();
	void ins(int key, int val, TreeNode *p, int &idx);
	void del(int key);
	int  getDepth(int &dp);
	void inorder(void(*callback_fun)(int _key, int _val, void*), void *p);
	void updatePointers(TreeNode *old_root, TreeNode *new_root);
private:
	int key;
	int val;
	TreeNode *left;
	TreeNode *right;
};

class BinarySearchTree
{
public:
	BinarySearchTree(int initial_nodes_count);
	~BinarySearchTree();
	void put(int key, int val);
	int  get(int key);
	void inorder(void(*callback_fun)(int _key, int _val, void*), void *p);
	int  getDepth(int &dp);
	int  getNodeCount() { return node_idx; }
private:
	TreeNode *root;
	TreeNode *nodes;
	int       node_count;
	int       node_idx;
};

#endif