//中序遍历线索化二叉树
#include<stdio.h>
#include<stdlib.h>
typedef struct Thread {
	struct Thread* left_node, *right_node;//左右指针
	int data;//需要存放的数据
	/*默认0代表左右孩子 1代表前驱或者后继*/
	int left_type;//类型标志
	int right_type;//类型标志
}Node;
 
Node* pre;//前驱结点的变量
Node* head;//头指针 指向某种遍历的第一个结点
 
void inOrderThreadTree(Node* node)
{
	//如果当前结点为NULL 直接返回
	if (node == NULL) {
		return;
	}
	//先处理左子树
	inOrderThreadTree(node->left_node);
	if (node->left_node == NULL)
	{
		//设置前驱结点
		node->left_type = 1;
		node->left_node = pre;
	}
	//如果结点的右子节点为NULL 处理前驱的右指针
	if (pre !=NULL && pre->right_node == NULL)
	{
		//设置后继
		pre->right_node = node;
		pre->right_type = 1;
	}
	//每处理一个节点 当前结点是下一个节点的前驱
	pre = node;
	//最后处理右子树
	inOrderThreadTree(node->right_node);
}
 
void inOrderTraverse(Node* root)
{
	//从根节点开始先找到最左边
	if (root == NULL)
	{
		return;
	}
	Node* temp = root;
	//先找到最左边结点 然后根据线索化直接向右遍历
	while (temp != NULL && temp->left_type == 0)
	{
		temp = temp->left_node;
	}
	while (temp != NULL)
	{
		//输出
		temp = temp->right_node;
	}
 
}
