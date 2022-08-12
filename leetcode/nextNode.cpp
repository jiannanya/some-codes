
题目描述:

给定一个二叉树和其中的一个结点，请找出中序遍历顺序的下一个结点并且返回。注意，树中的结点不仅包含左右子结点，同时包含指向父结点的指针。




思路：

首先这道题给出的是中序遍历这个二叉树，那么就是左根右。我们在求一个结点的下一个结点，那么这个时候我们需要分情况讨论：

1、如果该结点有右子树，则该结点的下一个结点为该结点的右子树的最左结点。

2、如果该结点没有右子树，则又分两种情况讨论：

情况一：如果该结点为该结点的父结点的左孩子，则该结点的父结点pNode.next则为下一个结点。

情况二：如果该结点为该结点的父结点的右孩子，则该结点的父结点的父结点的父结点，直到其中的一个父结点是这个父结点的左孩子，则该父结点的父结点为下一个结点。



实现：

/*
public class TreeLinkNode {
    int val;
    TreeLinkNode left = null;
    TreeLinkNode right = null;
    TreeLinkNode next = null;
    TreeLinkNode(int val) {
        this.val = val;
    }
}
*/
public class Solution {
    public TreeLinkNode GetNext(TreeLinkNode pNode)
    {
        /*这里需要注意的是pNode.next是pNode结点的父结点
        1、如果有右子树，那么下一个结点就是右子树最左边的节点。
        2、如果没有右子树，分两种情况，如果该结点的为父结点的左孩子，则该结点的父节点pNode.next则为
        该结点的下一个结点。
        第二种情况则是如果该结点的为父节点的右孩子，则向上找父节点，直到父节点为该父节点的左孩子，则该父节点的父节点
        为下一个结点。
        */
        if(pNode == null){
            return null;
        }
        if(pNode.right != null){//1、如果有右子树，那么下一个结点就是右子树最左边的节点。
            pNode = pNode.right;
            while(pNode.left != null) pNode = pNode.left;
            return pNode;
        }
        while(pNode.next != null){//这个则是在没有右子树的情况下，求下一个结点。下面if
            if(pNode.next.left == pNode) return pNode.next;
            pNode = pNode.next;
        }
        return pNode;
    }
}
