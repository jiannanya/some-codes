struct Tree *pre = NULL;    //全局变量 
 
void Inthreading(struct  Tree  *p)
{
    Inthreading(p->lchild);    //递归左子树线索化
 
    if(!p->lchild)            //没有左孩子
    {
        p->L = 1;
        p->lchild = pre;    //本来指向左孩子的指针指向前驱结点 
    }
 
    if(!pre->rchild)        //没有右孩子
    {
        pre->R = 1;
        pre->rchild = p;    //本来指向右孩子的指针指向后继结点 
    }
 
    pre = p;    //保持pre指向p的前驱
 
    Inthreading(p->rchild);    //递归右子树线索化
}
