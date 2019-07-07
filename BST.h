#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

template<typename T>
class BST
{
public:
	BST() :_root(nullptr) {};

	void noninsert(const T& val)
	{
		if (nullptr == _root)
		{
			_root = new Node(val);
		}
		else
		{
			Node* tem = _root;
			Node* fa = _root;
			while (nullptr != tem)
			{
				fa = tem;
				if (val == tem->_data) return;
				if (val < tem->_data)
				{
					tem = tem->_left;
				}
				else
				{
					tem = tem->_right;
				}
			}
			if (val < fa->_data)
			{
				fa->_left = new Node(val);
			}
			else
			{
				fa->_right = new Node(val);
			}
		}
	}
	void insert(const T& val)
	{
		_root = insert(_root, val);
	}

	void nonremove(const T& val)
	{
		// 两个孩子节点 用前驱节点或者后继代替该节点 然后删除前驱或者后继
		// 前驱 比他小的最大值 后继 比他大的最小值

		// 找待删除节点  
		if (nullptr == _root) return;
		Node *parent = nullptr;
		Node *cur = _root;
		while (nullptr != cur)
		{
			if (val < cur->_data)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (val > cur->_data)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				break;
			}
		}
		if (nullptr == cur) return;
		// 复杂情况转为简单情况

		if (nullptr != cur->_left && nullptr != cur->_right)
		{
			// 找前驱
			Node* Tfa = cur;
			Node* Ttem = cur->_left;

			while (nullptr != Ttem)
			{
				Tfa = Ttem;
				Ttem = Ttem->_right;
			}
			cur->_data = Ttem->_data;

			cur = Ttem;
			parent = Tfa; // ?
		}

		// 后续操作

		Node* child = cur->_left != nullptr ? cur->_left : cur->_right;

		if (parent == nullptr)
		{
			_root = child;
		}
		else
		{
			if (cur->_data < parent->_data)
			{
				// letf
				parent->_left = child;
			}
			else
			{
				//right
				parent->_right = child;
			}
		}

		delete cur;
	}
	void remove(const T& val)
	{
		_root = remove(_root, val);
	}
	void preOrder()
	{
		preOrder(_root);
	}
	// 非递归的前序遍历
	void nonPreOrder()
	{
		if (nullptr != _root)
		{
			std::cout << std::endl;
			return;
		}
		std::stack<Node*> mystack;
		mystack.push(_root);
		while (!mystack.empty())
		{
			Node* ptem = mystack.top();
			mystack.pop();
			std::cout << ptem->_data << " ";
			if (nullptr != ptem->_right)
			{
				mystack.push(ptem->_right);
			}
			if (nullptr != ptem->_left)
			{
				mystack.push(ptem->_left);
			}
		}
		std::cout << std::endl;
	}

	void inOrder()
	{
		inOrder(_root);
	}
	// 非递归的中序遍历
	void nonInOrder()
	{
		if (nullptr == _root)
		{
			std::cout << std::endl;
			return;
		}
		std::stack<Node*> sta;
		Node* pcur = _root;
		while (!sta.empty() || nullptr != pcur)
		{
			if (pcur != nullptr)
			{
				sta.push(pcur);
				pcur = pcur->_left;
				continue;
			}

			pcur = sta.top();
			std::cout << pcur->_data << " ";
			sta.pop();
			pcur = pcur->_right();
		}
		std::cout << std::endl;
	}

	void postOrder()
	{
		postOrder(_root);
	}
	// 非递归的后序遍历
	void nonPostOrder()
	{
		if (nullptr == _root)
		{
			std::cout << std::endl;
			return;
		}
		std::stack<Node*> s1, s2;
		s1.push(_root);
		while (!s1.empty())
		{
			Node* ptop1 = s1.top();
			s1.pop();
			s2.push(ptop1);
			if (nullptr != ptop1->_left)
			{
				s1.push(ptop1->_left);
			}
			if (nullptr != ptop1->_right)
			{
				s1.push(ptop1->_right);
			}
		}
		while (!s2.empty())
		{
			std::cout << s2.top()->_data << " ";
			s2.pop();
		}
		std::cout << std::endl;
	}

	void levelOrder()
	{
		std::cout << "BST树的中序遍历:";
		inOrder(_root);
		std::cout << std::endl;
	}
	void nonlevelOrder()
	{
		//非递归的层序遍历
		std::queue<Node*> myqueue;
		myqueue.push(_root);
		while (!myqueue.empty())
		{
			// 不空进队
			// 防止入队出队数据量最大
			if (nullptr != myqueue.front())
			{
				std::cout << myqueue.front()->_data << " ";
				myqueue.push(myqueue.front()->_left);
				myqueue.push(myqueue.front()->_right);
			}
			myqueue.pop();
		}
		std::cout << std::endl;
	}

	void levelOrders()
	{

	}

	int number()
	{
		return number(_root);
	}

	int level()
	{
		return level(_root);
	}

	void mirror()
	{
		mirror(_root);
	}

	// 判断一棵二叉树是否是BST数 左 《 跟 《 右
	bool isBST()
	{
		//中序遍历的前一个值
		Node *pre = nullptr;
		return isbst(_root, pre);
	}

	//在BST树中找first 到 last 中间的值
	void findAreaData(int first, int last)
	{
		fAD(first, last, _root);
		std::cout << std::endl;
	}

	//在BST树中，找两个值的最近公共祖先
	int getLCA(int a, int b)
	{
		Node* out = getLCA(_root, a, b);
		if (out != nullptr)
		{
			return out->_data;
		}
		else
		{
			return -1;
		}

	}

	//
	bool isChildTree(BST<T> &bst)
	{
		if (bst._root == nullptr) return true;
		Node* fa = query(_root, bst._root->_data);
		if (fa == nullptr) return false;

		return isSame(_root, bst._root);
	}

	//找 BST树中，中序遍历导数第K个节点值并返回
	int getLastK(int K)
	{
		int num = number(_root);
		if (num < K)
		{
			return -1;
		}
		else
		{
			int i = 0;
			Node* p = getLastK(_root, num - K + 1, i);

		}
	}

	void rebulid(T *arr, int len1, T *brr, int len2)
	{
		if (len1 == 0 || len2 == 0) return;
		if (len2 != len1) return;
		_root = reBulid(arr, len1, brr, len2);
	}
private:
	struct Node
	{
		Node(T data = T())
			: _data(data),
			_left(nullptr),
			_right(nullptr)
		{}
		T _data;
		Node* _left;
		Node* _right;
	};
	Node* insert(Node* node, const T& val)
	{
		if (node == nullptr)
		{
			return new Node(val);
		}
		if (node->_data > val)
		{
			node->_left = insert(node->_left, val);
		}
		else if (node->_data < val)
		{
			node->_right = insert(node->_right, val);
		}

		return node;
	}
	Node* remove(Node* node, const T& val)
	{
		if (node == nullptr) return node;

		if (node->_data > val)
		{
			node->_left = remove(node->_left, val);
		}
		else if (node->_data < val)
		{
			node->_right = remove(node->_right, val);
		}
		else
		{
			// 用前驱节点替换有两个孩子的节点的删除
			if (node->_left != nullptr && node->_right != nullptr)
			{
				Node *pre = node->_left;
				while (pre->_right != nullptr)
				{
					pre = pre->_right;
				}
				node->_data = pre->_data;
				node->_left = remove(node->_left, pre->_data);
			}
			else
			{
				if (node->_left != nullptr)
				{
					Node*  p = node->_left;
					delete node;
					return p;
				}

				if (node->_right != nullptr)
				{
					Node*  p = node->_right;
					delete node;
					return p;
				}

				delete node;
				return nullptr;
			}
		}
		return node;
	}
	void preOrder(Node* node)
	{
		if (node != nullptr)
		{
			std::cout << node->_data << std::endl;
			preOrder(node->_left);
			preOrder(node->_right);
		}
	}
	void inOrder(Node* node)
	{
		if (node != nullptr)
		{
			inOrder(node->_left);
			std::cout << node->_data << std::endl;
			inOrder(node->_right);
		}
	}
	void postOrder(Node* node)
	{
		if (node != nullptr)
		{
			postOrder(node->_left);
			postOrder(node->_right);
			std::cout << node->_data << std::endl;
		}
	}
	void levelOrder(Node *node, int level)
	{
		if (node == nullptr)
			return;

		if (level == 0)
		{
			std::cout << node->_data << " ";
			return;
		}
		levelOrder(node->_left, level - 1);
		levelOrder(node->_right, level - 1);
	}
	int number(Node* node)
	{
		if (nullptr != node)
		{
			return 1 + number(node->_left) + number(node->_right);
		}
		else
		{
			return 0;
		}
	}
	int level(Node* node)
	{
		if (node != nullptr)
		{
			return 1 + (level(node->_left) > level(node->_right) ?
				level(node->_left) : level(node->_right));
		}
		else
		{
			return 0;
		}
	}
	void mirror(Node* node)
	{
		if (nullptr == node) return;
		Node* tem = node->_left;
		node->_left = node->_right;
		node->_right = tem;
		mirror(node->_left);
		mirror(node->_right);
	}

	bool isbst(Node* node , Node * pre)
	{
		if (node == nullptr)
		{
			return true;
		}

		if (!isbst(node->_left, pre))
		{
			return false;
		}

		if (pre != nullptr && pre->_data > node->_data)
		{
			return false;
		}

		pre = node;
		return isbst(node->_right, pre);
	}

	void fAD(int first, int last, Node* node)
	{
		if (node != nullptr)
		{
			fAD(first, last, node->_left);
			if (node->_data > last)
			{
				return;
			}
			if (node->_data >= first && node->_data <= last)
			{
				std::cout << node->_data << std::endl;
			}
			fAD(first, last, node->_right);
		}
	}

	Node* query(Node* node, const T &val)
	{
		if (node == nullptr) return nullptr;

		if (node->_data > val)
		{
			return query(node->_left, val);
		}
		else if (node->_data < val)
		{
			return query(node->_right, val);
		}
		else
		{
			return node;
		}
	}

	Node*  getLCA(Node* node, const T & a, const T & b)
	{
		if (nullptr == node)
		{
			return nullptr;
		}

		if (node->_data > a && node->_data > b)
		{
			return getLCA(node->_left, a, b);
		}
		else if (node->_data < a && node->_data < b)
		{
			return getLCA(node->_right, a, b);
		}
		else
		{
			return node;
		}

	}

	bool isChildTree(Node* node1, Node* node2)
	{
		if (node1 == nullptr && node2 == nullptr)
		{
			return true;
		}

		if (node1 == nullptr)
		{
			return false;
		}

		if (node2 == nullptr)
		{
			return true;
		}

		if (node1->_data != node2->_data)
		{
			return false;
		}
		return isChildTree(node1->_left, node2->_left)
			&& isChildTree(node1->_right, node2->_right);
	}

	Node* getLastK(Node* node, int n, int& i)
	{
		if (node == nullptr) return nullptr;

		Node* pret = getLastK(node->_left, n, i);
		if (nullptr != pret)
		{
			return pret;
		}

		if (++i == n )
		{
			return node;
		}

		return getLastK(node->_right, n, i);
	}

	Node* reBulid(T *arr, int len1, int *brr, T len2)
	{
		if (len1 == 0 && len2 == 0) return nullptr;
		Node* out = new Node(arr[0]);

		int lenl = 0, lenr = 0;

		int i = 0;

		while (_root != brr[i])
		{
			i++;
		}
		lenl = i ;
		out->_left = reBulid(arr + 1, lenl, brr, lenl);
		lenr = len2 - i - 1;
		out->_right = reBulid(arr + 1 + lenl, lenr, brr + 1 + lenl, lenr);

		return out;
	}

	Node* _root;
		
};
