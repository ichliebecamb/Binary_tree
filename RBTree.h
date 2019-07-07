#pragma once
#include <iostream>


/*
1.红黑树是不是平衡（1）树？不是
2.红黑树节点的左右子树是允许一定的高度差的，红黑树中节点左右子树的高度差
最多是多少？ 长的不超过短的2倍
*/
enum Color
{
	RED,
	BLACK
};
template<typename T>
class RBTree
{
public:
	RBTree() :_root(nullptr) {}
	void insert(const T &val)
	{
		if (_root == nullptr)
		{
			_root = new RBNode(val, BLACK);
			return;
		}

		RBNode *parent = nullptr;
		RBNode *cur = _root;
		while (cur != nullptr)
		{
			parent = cur;
			if (cur->_data > val)
			{
				cur = cur->_left;
			}
			else if (cur->_data < val)
			{
				cur = cur->_right;
			}
			else
			{
				return;
			}
		}

		RBNode *node = new RBNode(val, RED);
		node->_parent = parent;
		if (parent->_data > val)
		{
			parent->_left = node;
		}
		else
		{
			parent->_right = node;
		}

		// 当新插入节点的父节点是红色的时候，红黑树进行调整
		if (parent->_color == RED)
		{
			fixAfterInsert(node);
		}
	}

	void remove(const T &val)
	{
		if (_root == nullptr)
			return;

		RBNode *cur = _root;
		while (cur != nullptr)
		{
			if (val < cur->_data)
			{
				cur = cur->_left;
			}
			else if (val > cur->_data)
			{
				cur = cur->_right;
			}
			else
			{
				break;
			}
		}

		if (cur == nullptr)
		{
			return ;
		}

		// 情况3 -> 情况1，2
		if (cur->_left != nullptr && cur->_right != nullptr)
		{
			RBNode *old = cur;
			cur = cur->_left;
			while (cur->_right != nullptr)
			{
				cur = cur->_right;
			}
			old->_data = cur->_data;
		}

		RBNode *child = cur->_left;
		if (child == nullptr)
		{
			child = cur->_right;
		}

		if (child != nullptr) // 删除的节点有孩子
		{
			child->_parent = cur->_parent;
			if (cur->_parent == nullptr)
			{
				_root = child;
			}
			else
			{
				if (cur == cur->_parent->_left)
				{
					cur->_parent->_left = child;
				}
				else
				{
					cur->_parent->_right = child;
				}
			}

			Color color = cur->_color;
			delete cur;

			// 如果删除的节点是黑色节点，就开始进行调整了
			if (color == BLACK)
			{
				fixAfterRemove(child);
			}
		}
		else // 删除的节点没有孩子
		{
			if (cur->_parent == nullptr)
			{
				_root = nullptr;
			}
			else
			{
				// 如果删除的节点是黑色节点，就开始进行调整了
				if (cur->_color == BLACK)
				{
					fixAfterRemove(cur);
				}

				if (cur->_parent->_left == cur)
				{
					cur->_parent->_left = nullptr;
				}
				else
				{
					cur->_parent->_right = nullptr;
				}
				delete cur;
			}
		}
	}
private:
	struct RBNode
	{
		RBNode(T data, Color color)
			: _data(data)
			, _color(color)
			, _left(nullptr)
			, _right(nullptr)
			, _parent(nullptr)
		{}

		T _data;
		Color _color;
		RBNode *_left;
		RBNode *_right;
		RBNode *_parent;
	};

	// 红黑树的删除调整
	void fixAfterRemove(RBNode *node)
	{
		while (color(node) == BLACK)
		{
			if (node == left(parent(node)))
			{
				// 删除的节点在父节点的左边
				RBNode *sib = right(parent(node));
				if (color(sib) == RED) // 情况1：兄弟节点是红色
				{
					setColor(parent(node), RED);
					setColor(sib, BLACK);
					leftRotate(parent(node));
					sib = right(parent(node));
				}

				if (color(left(sib)) == BLACK  // 情况2：兄弟以及兄弟的左右孩子都是黑色
					&& color(right(sib)) == BLACK)
				{
					setColor(sib, RED);
					node = parent(node);
				}
				else
				{
					if (color(right(sib)) != RED) // 情况3：兄弟的右孩子没有红色，需要进行旋转操作
					{
						setColor(left(sib), BLACK);
						setColor(sib, RED);
						rightRotate(sib);
						sib = right(parent(node));
					}

					// 情况4：兄弟节点的右孩子本身就是红色节点
					setColor(sib, color(parent(node)));
					setColor(parent(node), BLACK);
					setColor(right(sib), BLACK);
					leftRotate(parent(node));
					// 红黑树调整完成，直接跳出
					break;
				}
			}
			else
			{
				// 删除的节点在父节点的右边
				RBNode *sib = left(parent(node));
				if (color(sib) == RED) // 情况1：兄弟节点是红色
				{
					setColor(parent(node), RED);
					setColor(sib, BLACK);
					rightRotate(parent(node));
					sib = left(parent(node));
				}

				if (color(left(sib)) == BLACK  // 情况2：兄弟以及兄弟的左右孩子都是黑色
					&& color(right(sib)) == BLACK)
				{
					setColor(sib, RED);
					node = parent(node);
				}
				else
				{
					if (color(left(sib)) != RED) // 情况3：兄弟的左孩子没有红色，需要进行旋转操作
					{
						setColor(right(sib), BLACK);
						setColor(sib, RED);
						leftRotate(sib);
						sib = left(parent(node));
					}

					// 情况4：兄弟节点的左孩子本身就是红色节点
					setColor(sib, color(parent(node)));
					setColor(parent(node), BLACK);
					setColor(left(sib), BLACK);
					rightRotate(parent(node));
					// 红黑树调整完成，直接跳出
					break;
				}
			}
		}

		// 在当前子树路径上碰见红色节点，上面while循环退出，直接把当前
		// 路径上的一个红色节点置成黑色节点，保证当前路径黑色节点数量不变
		setColor(node, BLACK);
	}

	/*
	红黑树的插入调整
	*/
	void fixAfterInsert(RBNode *node)
	{
		while (color(parent(node)) == RED)
		{
			// 新节点是在祖先的左边还是右边
			if (left(parent(parent(node))) == parent(node))
			{
				// 新插入节点在祖先节点的左边
				RBNode *uncle = right(parent(parent(node)));
				// 叔叔节点是红色
				if (color(uncle) == RED)
				{
					setColor(parent(node), BLACK);
					setColor(uncle, BLACK);
					setColor(parent(parent(node)), RED);
					node = parent(parent(node));
				}
				else
				{
					// 祖先，父亲和插入节点不在一侧
					if (right(parent(node)) == node)
					{
						node = parent(node);
						leftRotate(node);
					}

					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					rightRotate(parent(parent(node)));
					break;
				}
			}
			else
			{
				// 新插入节点在祖先节点的右边
				RBNode *uncle = left(parent(parent(node)));
				// 叔叔节点是红色
				if (color(uncle) == RED)
				{
					setColor(parent(node), BLACK);
					setColor(uncle, BLACK);
					setColor(parent(parent(node)), RED);
					node = parent(parent(node));
				}
				else
				{
					// 祖先，父亲和插入节点不在一侧
					if (left(parent(node)) == node)
					{
						node = parent(node);
						rightRotate(node);
					}

					setColor(parent(node), BLACK);
					setColor(parent(parent(node)), RED);
					leftRotate(parent(parent(node)));
					break;
				}
			}
		}

		// 上面代码向上回溯改变节点颜色，要保证根节点一定是黑色的
		setColor(_root, BLACK);
	}

	// 从node节点开始左旋转操作
	void leftRotate(RBNode *node)
	{
		RBNode *child = node->_right;
		child->_parent = node->_parent;
		if (node->_parent == nullptr)
		{
			_root = child;
		}
		else if (node->_parent->_left == node)
		{
			node->_parent->_left = child;
		}
		else // node->_parent->_right == node
		{
			node->_parent->_right = child;
		}
		node->_right = child->_left;
		if (child->_left != nullptr)
		{
			child->_left->_parent = node;
		}
		child->_left = node;
		node->_parent = child;
	}

	// 从node节点开始右旋转操作
	void rightRotate(RBNode *node)
	{
		RBNode *child = node->_left;
		child->_parent = node->_parent;
		if (node->_parent == nullptr)
		{
			_root = child;
		}
		else if (node->_parent->_left == node)
		{
			node->_parent->_left = child;
		}
		else // node->_parent->_right == node
		{
			node->_parent->_right = child;
		}
		node->_left = child->_right;
		if (child->_right != nullptr)
		{
			child->_right->_parent = node;
		}
		child->_right = node;
		node->_parent = child;
	}

	Color color(RBNode *node)
	{
		return node == nullptr ? BLACK : node->_color;
	}
	void setColor(RBNode *node, Color color) { node->_color = color; }
	RBNode* parent(RBNode *node) { return node->_parent; }
	RBNode* left(RBNode *node) { return node->_left; }
	RBNode* right(RBNode *node) { return node->_right; }

	RBNode *_root;
};


