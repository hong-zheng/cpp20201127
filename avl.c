#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <string.h>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;

template<class T>
struct AVLNode{
	T _value;
	int _bf;
	AVLNode<T>* _left;
	AVLNode<T>* _right;
	AVLNode<T>* _parent;

	AVLNode(const T& val = T())
		:_value(val)
		, _bf(0)
		, _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr){}
};

template<class T>
class AVLTree{
public:
	typedef AVLNode<T> Node;

	AVLTree()
		:_root(nullptr){}

	bool insert(const T& val)
	{
		// 搜索树的插入
		if (_root == nullptr)
		{
			_root = new Node(val);
			return true;
		}
		
		Node* cur = _root;
		Node* parent = nullptr;
		while (cur)
		{
			parent = cur;
			// 相同结点不能插入二叉搜索树
			if (cur->_value == val)
			{
				return false;
			}
			else if (cur->_value < val)
			{
				cur = cur->_right;
			}
			else
			{
				cur = cur->_left;
			}
		}
		// 插入当前元素
		cur = new Node(val);
		if (parent->_value < val)
			parent->_right = cur;
		else
			parent->_left = cur;
		// 当前结点的父节点就是上一个结点
		cur->_parent = parent;

		// 更新+调整
		while (parent)
		{
			//0、插入完之后，对父节点肯定有影响
			//	1、更新parent平衡因子
			if (parent->_left == cur)
			{
				// 左边插入一个结点
				--parent->_bf;
			}
			else if (parent->_right == cur)
			{
				// 插入父节点的右边，因此平衡因子需要++
				++parent->_bf;
			}

			// 2、更新完之后，判断是否需要继续更新
			if (parent->_bf == 0)
			{// 表示短的子树被补齐，则
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				// 表示parent子树的高度发生了改变，需要继续向上更新
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == -2 || parent->_bf == 2)
			{
				// 此时就不能只靠更新获得平衡，需要对结构进行调整，但是调整完之后就直接结束，不必再向上更新
				// 调整
				if (parent->_bf == -2 && cur->_bf == -1)
				{
					// 左边的左边高，右旋
					// 以父亲为轴，进行右旋
					RotateR(parent);
					cout << val << "-->右旋" << ",parent-->" << parent->_value << endl;
				}
				else if (parent->_bf == 2 && cur->_bf == 1)
				{
					// 右边的右边高，需要进行左旋
					RotateL(parent);
					cout << val << "-->左旋" << ",parent-->" << parent->_value << endl;
				}
				// 右边的左边高：先右旋再左旋
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					Node* subR = parent->_right;
					Node* subRL = subR->_left;
					int bf = subRL->_bf;
					// parent , subR(cur) , subRL
					// 右边的左边高：右左双旋
					RotateR(cur); // 以subR为轴右旋
					RotateL(parent); // 以parent为轴左旋
					cout << val << "-->右左双旋"  <<",parent-->" << parent->_value << endl;
					
					// 对结点的平衡因子进行调整
					if (bf == 1)
					{
						// subRL右子树高
						subR->_bf = 0;
						parent->_bf = -1;
					}
					else if (bf == -1)
					{
						parent->_bf = 0;
						subR->_bf = 1;
					}
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					Node* subL = parent->_left;
					Node* subLR = subL->_right;
					int bf = subLR->_bf;

					// 左边的右边高：左右双旋
					RotateL(cur);
					RotateR(parent);
					cout << val << "-->左右双旋" << ",parent-->" << parent->_value << endl;
					
					// 对本结构平衡因子进行调整
					if (bf == 1)
					{
						subL->_bf = 0;
						parent->_bf = 1;
					}
					else if (bf == -1)
					{
						subL->_bf = -1;
						parent->_bf = 0;
					}
				}
				break;
			}
		}
		return true;
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		subL->_right = parent;
		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		if (parent == _root)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			Node* g = parent->_parent;
			subL->_parent = g;
			if (g->_left == parent)
			{
				g->_left = subL;
			}
			else
			{
				g->_right = subL;
			}
		}
		parent->_parent = subL;
		parent->_bf = subL->_bf = 0;
	}

	void RotateL(Node* parent)
	{
		// 右边的右边高，取右边的孩子
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		subR->_left = parent;
		parent->_right = subRL;

		if (subRL)
			subRL->_parent = parent;
		if (parent == _root)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			Node* g = parent->_parent;
			subR->_parent = g;
			if (g->_left == parent)
			{
				g->_left = subR;
			}
			else
			{
				g->_right = subR;
			}
		}
		parent->_parent = subR;
		parent->_bf = subR->_bf = 0;
	}

	// 中序遍历
	void inorder()
	{
		_inorder(_root);
		cout << endl;
	}

	void _inorder(Node* _root)
	{
		if (!_root)
			return;
		_inorder(_root->_left);
		cout << _root->_value << " ";
		_inorder(_root->_right);
	}

	// 判断是否为平衡因子
	bool isBalance()
	{
		return _isBalance(_root);
	}

	bool _isBalance(Node* root)
	{
		if (root == nullptr)
			return true;
		//左右子树的高度差是否和平衡因子相等
		int subL = Height(root->_left);
		int subR = Height(root->_right);
		if (root->_bf != subR - subL)
		{
			cout << "结点" << root->_value << "异常: bf" << root->_bf << "高度差-->" << subR - subL << endl;
			return false;
		}
		
		return abs(root->_bf) < 2
			&& _isBalance(root->_left)
			&& _isBalance(root->_right);
	}

	int Height(Node* root)
	{
		if (root == nullptr)
			return 0;
		int left = Height(root->_left);
		int right = Height(root->_right);
		return left > right ? (left + 1) : (right + 1);
	}
private:
	Node* _root;
};
void testAVL(){
	int arr[] = { 0,9,1,4,3,5,6,8,7,2 };
	AVLTree<int> avl;
	for (const auto& e : arr){
		avl.insert(e);
	}
	avl.inorder();
	//cout << endl;
	cout << "是否为平衡树-->"<<avl.isBalance() << endl;
}
int main(){
	testAVL();
	system("pause");
	return 0;
}
