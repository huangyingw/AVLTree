#include<iostream>
using namespace std;
#include "D:\myproject\c-c++\windows\data structure\ѭ������\CirQueue.h"

class AVLNode
{
	public:
		int data;
		AVLNode *left,*right;
		int balance;
		int height;//AVL���ĸ߶�
		AVLNode():left(NULL),right(NULL),balance(0){}
		AVLNode(int d,AVLNode *l=NULL,AVLNode *r=NULL):data(d),left(l),right(r),balance(0),height(0){}
};

class AVLTree
{
	public:
		int RefValue;//��������ı�־
		AVLNode *root;//������ָ��
		int Height(AVLNode *t)const;
		AVLTree():root(NULL){}
		AVLTree(int Ref):RefValue(Ref),root(NULL){}
		int Insert(AVLNode* &tree, int x,int & taller);//����
		int PrintAVLHor(AVLNode*current,ostream&out);//��ӡ��BST����ͼ
		void SetHeight(AVLNode* ptr,int height);//���õ�ǰ�ڵ�������ӽڵ�ĸ߶�
		void RotateLeft(AVLNode * &Tree, AVLNode * NewTree);//���������������ߣ�����TreeΪ����AVL��������ת,��ת���¸���NewTree
		void LeftBalance(AVLNode * &Tree, int & taller);//��ƽ�⻯
		void RightBalance(AVLNode * &Tree, int & taller);//��ƽ�⻯
		int Insert(int x){int taller;return Insert(root,x,taller);}
		void Remove(AVLNode* &tree, int x,int & shorter);//����treeΪ����AVL����ɾ��Ԫ��x,���ɾ���ɹ���shorter����1�����򷵻�0
		void RightAdjust_Del(AVLNode *&Tree, int &shorter);
		void LeftAdjust_Del(AVLNode *&Tree, int &shorter);
		void RotateRight(AVLNode *&Tree,AVLNode* NewTree);
		void Remove_Ex(AVLNode *&r, const int &x, int &inc);
		AVLNode* Min(AVLNode* ptr);
		friend istream& operator>>(istream& in,AVLTree & Tree);
		friend ostream& operator>>(ostream& out,const AVLTree & Tree);
};

void AVLTree::RotateLeft(AVLNode * &Tree, AVLNode * NewTree)//���������������ߣ�����TreeΪ����AVL��������ת,��ת���¸���NewTree
{
	Tree->right=NewTree->left;
	NewTree->left=Tree;
	Tree=NewTree;
}

void AVLTree::LeftBalance(AVLNode* &Tree, int &taller)
{
	AVLNode *left_child=Tree->left;
	switch(left_child->balance)//�ж���������ƽ������
	{
		case -1://��ߣ��޸�ƽ������
		{
			Tree->balance = 0;
            left_child->balance = 0;
			RotateRight(Tree,Tree->left);//���ҵ���ת
			SetHeight(Tree,Tree->right->height);
			taller=0;
			break;
		}
		case 0://û�з�����ƽ��
		{
			cout<<"LeftBalance error: Tree already balanced. \n";
			break;
		}
		case 1://�Ҹߣ�ȡ��������������
		{
			AVLNode *right_child=left_child->right;
			switch(right_child->balance)//�жϸ���������ƽ������
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
			}//������ת�������ƽ������
			right_child->balance=0;
			
			RotateLeft(Tree->left,left_child->right);//����ת
			SetHeight(Tree->left,Tree->left->height);

			RotateRight(Tree,Tree->left);//�ҵ���ת
			
			SetHeight(Tree,Tree->right->height);

			taller=0;
		}
	}
}

void AVLTree::SetHeight(AVLNode* ptr,int height)//���õ�ǰ�ڵ�������ӽڵ�ĸ߶�,
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

int AVLTree::Insert(AVLNode* &tree, int x,int & taller)//����treeΪ����AVL���в�����Ԫ��x,�������ɹ���taller����1�����򷵻�0��
{
	int success;
	if(tree==NULL)//ԭΪ��������ĳ���Ŀ�����
	{
		tree=new AVLNode(x);//�����½�㲢����
		success=(tree!=NULL)?1:0;//�ɹ���־���洢����ɹ�Ϊ1
		if(success)
			taller=1;
	}
	else if(x<tree->data)//�ж���������뻹�����Ҳ���
	{
		success=Insert(tree->left,x,taller);//���뵽������
		if(taller)//����ɹ�
		{
			SetHeight(tree,tree->height);
			switch(tree->balance)//�ж�ƽ������
			{
				case -1://ԭ�������ߣ���ƽ�⣬����
				{
					LeftBalance(tree,taller);
					break;
				}
				case 0://ԭ�������ȸߣ�����ƽ������
				{
					tree->balance=-1;
					break;
				}
				case 1://ԭ�������ߣ�����ƽ������
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
		success=Insert(tree->right,x,taller);//���뵽������
		if(taller)//����ɹ�
		{
			SetHeight(tree,tree->height);
			switch(tree->balance)//�ж�ƽ������
			{
				case -1://ԭ�������ߣ�����ƽ������
				{
					tree->balance=0;
					taller=0;
					break;
				}
				case 0://ԭ�������ȸߣ�����ƽ������
				{
					tree->balance=1;
					break;
				}
				case 1://ԭ�������ߣ���ƽ�⣬����
				{
					RightBalance(tree,taller);
					break;
				}
			}
		}
	}
	return success;//���ϲ㴫�Ͳ���ɹ���Ϣ,�˱����ڴ˺�����û�ã����������Ǵ�����Ϣ��һ�ֺ÷�����Ӧ��ѧϰ������ûɾ��
}

AVLNode* AVLTree::Min(AVLNode* ptr)//��ptr����Ѱ��С���
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

void AVLTree::Remove(AVLNode* &tree, int x,int & shorter)//����treeΪ����AVL����ɾ��Ԫ��x,���ɾ���ɹ���shorter����1�����򷵻�0��
{
	if(tree == NULL)
    {
		shorter = 0;
        return;
	}
    if(tree->data<x)//��������ɾ��
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
    else if ( tree->data > x )//��������ɾ��
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

int AVLTree::PrintAVLHor(AVLNode*current,ostream&out)//��ӡ��BST����ͼ
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
