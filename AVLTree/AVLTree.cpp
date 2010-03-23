// AVLTree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AVLTree.h"

int _tmain(int argc, _TCHAR* argv[])
{
	AVLTree *avlTree=new AVLTree();
	int taller=1;
	int shorter=1;
	avlTree->Insert(avlTree->root,16,taller);
	avlTree->Insert(3);
	avlTree->Insert(7);
	avlTree->Insert(11);
	avlTree->Insert(9);
	avlTree->Insert(26);
	avlTree->Insert(18);
	avlTree->Insert(14);
	avlTree->Insert(15);
	avlTree->PrintAVLHor(avlTree->root,cout);

	avlTree->Remove(avlTree->root,18,shorter);
	avlTree->PrintAVLHor(avlTree->root,cout);
	return 0;
}

