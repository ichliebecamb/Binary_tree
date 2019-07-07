#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

// 二叉平衡树 由BST树发展而来 +  平衡
// 每一个节点的左右子树的高度差不超过1

template<typename T>
class AVLTree
{
public:
	AVLTree() :_root(nullptr){}
	//~AVLTree();

	void insert(const T& val)
	{
		_root = insert(_root, val);
	}

	void remove(const T&val)
	{
		_root = remove(_root, val);
	}

	void preOrder()
	{
		std::cout << "前序遍历" << std::endl;
		preOrder(_root);
		std::cout <<  std::endl;
	}

	void inOrder()
	{
		std::cout << "中序遍历" << std::endl;
		inOrder(_root);
		std::cout << std::endl;
	}

private:
	struct AVLNode
	{
		AVLNode(T data = T())
			: _data(data)
			, _left(nullptr)
			, _right(nullptr)
			, _height(1)
		{}
		T _data;
		int _height;
		AVLNode* _left;
		AVLNode* _right;
	};

	int height(AVLNode *node)
	{
		return node == nullptr ? 0 : node->_height; 
	}

	void preOrder(AVLNode* node)
	{
		if (node != nullptr)
		{
			std::cout << node->_data << " ";
			preOrder(node->_left);
			preOrder(node->_right);
		}
	}

	void inOrder(AVLNode* node)
	{
		if (node != nullptr)
		{
			inOrder(node->_left);
			std::cout << node->_data << " ";
			inOrder(node->_right);
		}
	}

	AVLNode* leftRotate(AVLNode* node)
	{
		AVLNode* child = node->_right;
		node->_right = child->_left;
		child->_left = node;

		node->_height = MaxHeight(node->_left, node->_right) + 1;
		child->_height = MaxHeight(child->_left, child->_right) + 1;
		return child;
	}

	AVLNode* rightRotate(AVLNode* node)
	{
		
		AVLNode* child = node->_left;
		node->_left = child->_right;
		child->_right = node;
		node->_height = MaxHeight(node->_left, node->_right) + 1;
		child->_height = MaxHeight(child->_left, child->_right) + 1;
		return child;
	}

	AVLNode* leftBalance(AVLNode* node)
	{
		node->_left = leftRotate(node->_left);
		return rightRotate(node);
	}

	AVLNode* rightBalance(AVLNode* node)
	{
		node->_right = leftRotate(node->_right);
		return rightRotate(node);
	}

	int MaxHeight(AVLNode* left, AVLNode* right)
	{
		return height(left) > height(right) ?
			height(left) : height(right);
	}

	AVLNode* insert(AVLNode *root, const T &val)
	{
		if (root == nullptr)
		{
			return new AVLNode(val);
		}

		if (root->_data > val)
		{
			root->_left = insert(root->_left, val);
			
			// 左加左高
			root = leftUnBalance(root);
		}
		else if (root->_data < val)
		{
			root->_right = insert(root->_right, val);
			// 右加右高
			root = rightUnBalance(root);
		}
		else
		{
			;
		}

		// 从叶子节点回溯到根节点的过程中，要跟新节点的高度
		root->_height = MaxHeight(root->_left, root->_right) + 1;

		return root;
	}

	AVLNode* remove(AVLNode* node, const T &val)
	{
		if (node == nullptr) return node;

		if (node->_data > val)
		{
			node->_left = remove(node->_left, val);
			// 左删右高
			node = rightUnBalance(node);
		}
		else if (node->_data < val)
		{
			node->_right = remove(node->_right, val);
			// 右删左高
			node = leftUnBalance(node);
		}
		else
		{
			// 用前驱节点替换有两个孩子的节点的删除
			// 谁高用删谁
			if (node->_left != nullptr && node->_right != nullptr)
			{
				if (height(node->_left) >= height(node->_right))
				{
					//左子树高，用前驱节点代替
					AVLNode *pre = node->_left;
					while (pre->_right != nullptr)
					{
						pre = pre->_right;
					}
					node->_data = pre->_data;
					node->_left = remove(node->_left, pre->_data);
				}
				else
				{
					//右子树高，用后继节点代替
					AVLNode *last = node->_right;
					while (last->_left != nullptr)
					{
						last = last->_left;
					}
					node->_data = last->_data;
					node->_right = remove(node->_right, last->_data);
				}
			}
			else
			{
				if (node->_left != nullptr)
				{
					AVLNode*  p = node->_left;
					delete node;
					return p;
				}

				if (node->_right != nullptr)
				{
					AVLNode*  p = node->_right;
					delete node;
					return p;
				}

				delete node;
				return nullptr;
			}
		}

		// 从当前节点回溯到根节点的过程中，要跟新节点的高度
		node->_height = MaxHeight(node->_left, node->_right) + 1;
		return node;
	}

	AVLNode* leftUnBalance(AVLNode* root)
	{
		// 由于左子树太高引起失衡
		if (height(root->_left) - height(root->_right) > 1)
		{
			if (height(root->_left->_left)
				>= height(root->_left->_right))
			{
				// 左孩子的左子树太高
				root = rightRotate(root);
			}
			else
			{
				// 左孩子的右子树太高
				root = leftBalance(root);
			}
		}

		return root;
	}

	AVLNode* rightUnBalance(AVLNode* root)
	{
		// 由于右子树太高引起失衡
		if (height(root->_right) - height(root->_left) > 1)
		{
			if (height(root->_right->_right)
				>= height(root->_right->_left))
			{
				// 右孩子的右子树
				root = leftRotate(root);
			}
			else
			{
				// 右孩子的左子树
				root = rightBalance(root);
			}
		}

		return root;
	}
	AVLNode* _root;
};
