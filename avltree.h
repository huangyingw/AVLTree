#include<iostream>
using namespace std;
#include "/media/myproject/git/c_c++/linux/data_structure/cirQueue/CirQueue.h"

class AVLNode
{
	public:
		int data;
		AVLNode *left,*right;
		int balance;
		int height;//AVL结点的高度
		AVLNode():left(NULL),right(NULL),balance(0){}
		AVLNode(int d,AVLNode *l=NULL,AVLNode *r=NULL):data(d),left(l),right(r),balance(0),height(0){}
};

class AVLTree
{
	public:
		int RefValue;//插入结束的标志
		AVLNode *root;//根结点的指针
		int Height(AVLNode *t)const;
		AVLTree():root(NULL){}
		AVLTree(int Ref):RefValue(Ref),root(NULL){}
		int Insert(AVLNode* &tree, int x,int & taller);//插入
		int PrintAVLHor(AVLNode*current,ostream&out);//打印出BST的视图
		void SetHeight(AVLNode* ptr,int height);//设置当前节点的所有子节点的高度
		void RotateLeft(AVLNode * &Tree, AVLNode * NewTree);//右子树比左子树高：对以Tree为根的AVL树做左单旋转,旋转后新根在NewTree
		void LeftBalance(AVLNode * &Tree, int & taller);//左平衡化
		void RightBalance(AVLNode * &Tree, int & taller);//右平衡化
		int Insert(int x){int taller;return Insert(root,x,taller);}
		void Remove(AVLNode* &tree, int x,int & shorter);//在以tree为根的AVL树中删除元素x,如果删除成功，shorter返回1，否则返回0
		void RightAdjust_Del(AVLNode *&Tree, int &shorter);
		void LeftAdjust_Del(AVLNode *&Tree, int &shorter);
		void RotateRight(AVLNode *&Tree,AVLNode* NewTree);
		void Remove_Ex(AVLNode *&r, const int &x, int &inc);
		AVLNode* Min(AVLNode* ptr);
		friend istream& operator>>(istream& in,AVLTree & Tree);
		friend ostream& operator>>(ostream& out,const AVLTree & Tree);
};

void AVLTree::RotateLeft(AVLNode * &Tree, AVLNode * NewTree)//右子树比左子树高：对以Tree为根的AVL树做左单旋转,旋转后新根在NewTree
{
	Tree->right=NewTree->left;
	NewTree->left=Tree;
	Tree=NewTree;
}

void AVLTree::LeftBalance(AVLNode* &Tree, int &taller)
{
	AVLNode *left_child=Tree->left;
	switch(left_child->balance)//判断左子树的平衡因子
	{
		case -1://左高，修改平衡因子
		{
			Tree->balance = 0;
            left_child->balance = 0;
			RotateRight(Tree,Tree->left);//做右单旋转
			SetHeight(Tree,Tree->right->height);
			taller=0;
			break;
		}
		case 0://没有发生不平衡
		{
			cout<<"LeftBalance error: Tree already balanced. \n";
			break;
		}
		case 1://右高，取左子树的右子树
		{
			AVLNode *right_child=left_child->right;
			switch(right_child->balance)//判断该右子树的平衡因子
			{
				case -1:
				{
					Tree->balance=1;
					left_child->balance=0;
					break;
				}
				case 0:
				{
					Tree->balance=left_child->balance=0;
					break;
				}
				case 1:
				{
					Tree->balance=0;
					left_child->balance=-1;
					break;
				}
			}//调整旋转后各结点的平衡因子
			right_child->balance=0;
			
			RotateLeft(Tree->left,left_child->right);//左单旋转
			SetHeight(Tree->left,Tree->left->height);

			RotateRight(Tree,Tree->left);//右单旋转
			
			SetHeight(Tree,Tree->right->height);

			taller=0;
		}
	}
}

void AVLTree::SetHeight(AVLNode* ptr,int height)//设置当前节点的所有子节点的高度,
{
	if(NULL!=ptr)
	{
		ptr->height=height;
		if(NULL!=ptr->left)
		{
			SetHeight(ptr->left,ptr->height+1);
		}
		if(NULL!=ptr->right)
		{
			SetHeight(ptr->right,ptr->height+1);
		}
	}
}

void AVLTree::RightBalance(AVLNode* &Tree, int &taller)
{
	AVLNode *rightsub=Tree->right,*leftsub;
	switch(rightsub->balance)
	{
		case 1:
		{
			Tree->balance=rightsub->balance=0;

			RotateLeft(Tree,Tree->right);

			SetHeight(Tree,Tree->left->height);
			
			taller=0;
			break;
		}
		case 0:
		{
			cout<<"RightBalance error: Tree already balanced. \n";
			break;
		}
		case -1:
		{
			leftsub=rightsub->left;
			switch(leftsub->balance)
			{
				case 1:
				{
					Tree->balance=-1;
					leftsub->balance=0;
					break;
				}
				case 0:
				{
					Tree->balance=rightsub->balance=0;
					break;
				}
				case -1:
				{
					Tree->balance=0;
					rightsub->balance=1;
					break;
				}
			}
			leftsub->balance=0;
			RotateRight(Tree->right,rightsub->left);

			SetHeight(Tree->right,rightsub->height);
			
			RotateLeft(Tree,Tree->right);

			SetHeight(Tree,Tree->left->height);

			taller=0;
		}
	}
}

int AVLTree::Insert(AVLNode* &tree, int x,int & taller)//在以tree为根的AVL树中插入新元素x,如果插入成功，taller返回1，否则返回0。
{
	int success;
	if(tree==NULL)//原为空树，或某结点的空链域
	{
		tree=new AVLNode(x);//创建新结点并插入
		success=(tree!=NULL)?1:0;//成功标志：存储分配成功为1
		if(success)
			taller=1;
	}
	else if(x<tree->data)//判断是向左插入还是向右插入
	{
		success=Insert(tree->left,x,taller);//插入到左子树
		if(taller)//插入成功
		{
			SetHeight(tree,tree->height);
			switch(tree->balance)//判断平衡因子
			{
				case -1://原左子树高，不平衡，调整
				{
					LeftBalance(tree,taller);
					break;
				}
				case 0://原两子树等高，仅改平衡因子
				{
					tree->balance=-1;
					break;
				}
				case 1://原右子树高，仅改平衡因子
				{
					tree->balance=0;
					taller=0;
					break;
				}
			}
		}
	}
	else if(x>tree->data)
	{
		success=Insert(tree->right,x,taller);//插入到右子树
		if(taller)//插入成功
		{
			SetHeight(tree,tree->height);
			switch(tree->balance)//判断平衡因子
			{
				case -1://原左子树高，仅改平衡因子
				{
					tree->balance=0;
					taller=0;
					break;
				}
				case 0://原两子树等高，仅改平衡因子
				{
					tree->balance=1;
					break;
				}
				case 1://原右子树高，不平衡，调整
				{
					RightBalance(tree,taller);
					break;
				}
			}
		}
	}
	return success;//向上层传送插入成功信息,此变量在此函数中没用，不过，这是传递信息的一种好方法，应该学习，所以没删除
}

AVLNode* AVLTree::Min(AVLNode* ptr)//在ptr中搜寻最小结点
{
	if(NULL!=ptr)
	{
		if(NULL!=ptr->left)
		{
			Min(ptr->left);
		}
		else
		{
			return ptr;
		}
	}
}

void AVLTree::Remove(AVLNode* &tree, int x,int & shorter)//在以tree为根的AVL树中删除元素x,如果删除成功，shorter返回1，否则返回0。
{
	if(tree == NULL)
    {
		shorter = 0;
        return;
	}
    if(tree->data<x)//往右子树删除
    {
		Remove(tree->right,x,shorter);
        if (shorter == 0) return;
		switch ( tree->balance )
        {
			case 0:
			{
				tree->balance = -1;
                shorter=0;
                break;
			}
			case 1:
			{
				tree->balance = 0;
                shorter = 1;
                break;
			}
			case -1:
			{
				LeftAdjust_Del(tree, shorter);
                break;
			}
		}
	}
    else if ( tree->data > x )//往左子树删除
    {
        Remove(tree->left, x, shorter);
        if ( shorter == 0 ) return;
        switch ( tree->balance )
        {
			case 0:
				tree->balance = 1;
                shorter = 0;
                break;
			case -1:
				tree->balance = 0;
                shorter = 1;
                break;
			case 1:
				RightAdjust_Del(tree, shorter);
				break;
		}
	}
    else
    {
		if (tree->left && tree->right)
        {
			int data = Min(tree->right)->data;
            Remove(tree->right, data, shorter);
            tree->data = data;
            if (shorter == 0) return;
            switch (tree->balance)
            {
				case 0:
				{
					tree->balance = -1;
                    shorter=0;
                    break;
				}
				case 1:
				{
					tree->balance = 0;
                    shorter = 1;
                    break;
				}
				case -1:
				{
					LeftAdjust_Del(tree, shorter);
                    break;
				}
            }
            return;
		}
        AVLNode *tmp =tree ;
        tree = tree->left!=0?tree->left:tree->right;
        delete tmp;
        shorter = 1;
        return;
	}
}


void AVLTree::RotateRight(AVLNode *&Tree,AVLNode* NewTree)
{
	Tree->left=NewTree->right;
	NewTree->right=Tree;
	Tree=NewTree;
}

void AVLTree::LeftAdjust_Del(AVLNode *&Tree, int &shorter)
{
	AVLNode *left_child = Tree->left;
    switch ( left_child->balance )
    {
		case -1:
		{
			Tree->balance = 0;
            left_child->balance = 0;
            RotateRight(Tree, Tree->left);
			SetHeight(Tree,Tree->left->height);
            shorter = 1;
            break;
		}
		case 0:
		{
			Tree->balance = -1;
            left_child->balance = 1;
            RotateRight(Tree, left_child);
			SetHeight(Tree,Tree->right->height);
            
			shorter = 0;
            break;
		}
		case 1:
		{
			AVLNode *right_child = left_child->right;
            switch (right_child->balance )
            {
				case 0:
				{
					Tree->balance = 0;
                    left_child->balance = 0;
                    right_child->balance = 0;
                    shorter = 1;
                    break;
				}
				case 1:
				{
                    Tree->balance = -1;
                    left_child->balance = 0;
                    right_child->balance = 0;
                    shorter = 1;
                    break;
				}
                case -1:
				{
					Tree->balance = 0;
                    left_child->balance = 1;
                    right_child->balance = 0;
                    shorter = 1;
                    break;
				}
			}
			RotateLeft(left_child, left_child->right);
			SetHeight(left_child,left_child->right->height);
			RotateRight(Tree, left_child);
			break;
		}
	}
}

void AVLTree::RightAdjust_Del(AVLNode *&Tree, int &shorter)
{
	AVLNode *right_child = Tree->right;
    switch (right_child->balance)
    {
		case 0:
		{
			right_child->balance = -1;
            Tree->balance = 1;
            shorter = 0;
            RotateLeft(Tree, right_child);
			SetHeight(Tree,Tree->left->height);
            break;
		}
        case 1:
		{
			right_child->balance = 0;
            Tree->balance = 0;
            shorter = 1;
            RotateLeft(Tree, right_child);
			SetHeight(Tree,Tree->left->height);
            break;
		}
        case -1:
		{
			AVLNode *left_child = right_child->left;
            switch (left_child->balance)
            {
                case 0:
				{
					Tree->balance = 0;
                    right_child->balance = 0;
                    left_child->balance = 0;
                    shorter = 1;
                    break;
				}
                case 1:
				{
					Tree->balance = -1;
                    right_child->balance = 0;
                    left_child->balance = 0;
                    shorter = 1;
                    break;
				}
                case -1:
				{
                    Tree->balance = 0;
                    right_child->balance = 1;
                    left_child->balance = 0;
                    shorter = 1;
                    break;
                }
                RotateRight(right_child, left_child);
				SetHeight(right_child,Tree->right->height);
            
				
                RotateLeft(Tree, right_child);
                break;
			}
		}
	} 
}

int AVLTree::PrintAVLHor(AVLNode*current,ostream&out)//打印出BST的视图
{
	int currentHeight=0;
	CirQueue<AVLNode*> Q;
	Q.InitQueue();
	if(NULL!=current)
	{
		Q.EnQueue(current);
		while(!Q.QueueEmpty())
		{
			current=Q.DeQueue();
			if(currentHeight<current->height)
			{
				currentHeight=current->height;
				out<<endl;
			}
			out<<current->data<<",";
			if(NULL!=current->left)
			{
				Q.EnQueue(current->left);
			}
			if(NULL!=current->right)
			{
				Q.EnQueue(current->right);
			}
		}
	}
	cout<<endl<<endl;
	return 0;
}
