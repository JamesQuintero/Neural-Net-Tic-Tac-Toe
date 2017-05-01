#include "Node.h"

#ifndef _BINARYSEARCH_HPP_
#define _BINARYSEARCH_HPP_


class BinaryNode;


class BinarySearchTree
{
	private:
		BinaryNode *root;
		

	public:

		BinarySearchTree();

		//inserts node parameter in the tree
		void insert(node* data);

		//This method allows us determine if a specific piece of data is in the tree
		node* find(int node_id);

		//left, right, root
		void postOrderPrint();

		//root, left, right
		void preOrderPrint();

		//left, root, right
		void inOrderPrint();


		//Recursive Functions
		void inOrder(BinaryNode *node);

		void preOrder(BinaryNode *node);

		void postOrder(BinaryNode *node);

};

#endif