#include "BinarySearch.hpp"
#include "BinaryNode.hpp"
#include <iostream>

using namespace std;

BinarySearchTree::BinarySearchTree()
{
	root=NULL;
}


void BinarySearchTree::insert(node* data)
{
	BinaryNode *leaf = new BinaryNode(data);
	int node_id = data->node_id;

	//sets as root if new tree
	if(root == NULL)
	{
		root = leaf;
		return;
	}


	BinaryNode *current = root;
	BinaryNode *parent;
	
	while(current != NULL)
	{
		parent = current;
		
		int cur_id = current->getNode()->node_id;

		if(node_id <= cur_id)
			current = current->getLeftChild();
		else
			current = current->getRightChild();
	}

	//current = leaf;
	if(leaf->getNode()->node_id <= parent->getNode()->node_id)
		parent->setLeftChild(leaf);
	else
		parent->setRightChild(leaf);

	
}

//returns pointer to node in BST
node* BinarySearchTree::find(int node_id)
{
	if (root == NULL)
		return NULL;

	BinaryNode *current = root;
	BinaryNode *parent;
	while (current != NULL)
	{
		int cur_node_id = current->getNode()->node_id;

		//returns node if it matches
		if (node_id == cur_node_id)
			return current->getNode();
		//traverse binary search tree to the left
		else if(node_id < cur_node_id)
			current = current->getLeftChild();
		//traverses binary search tree to the right
		else
			current = current->getRightChild();
	}

	return NULL;
}

void BinarySearchTree::postOrderPrint()
{
	std::cout << "Post Order Print" << std::endl;
	postOrder(root);
	std::cout << std::endl;

}

void BinarySearchTree::postOrder(BinaryNode *node)
{
	if(node == NULL)
		return;
	if(node->getLeftChild() != NULL)
		postOrder(node->getLeftChild());
	if(node->getRightChild() != NULL)
		postOrder(node->getRightChild());

	std::cout << " " << node->getNode()->node_id << " ";

}

void BinarySearchTree::preOrderPrint()
{
	std::cout << "Pre Order Print" << std::endl;
	preOrder(root);
	std::cout << std::endl;

}

void BinarySearchTree::preOrder(BinaryNode *node)
{
	if (node == NULL)
		return;
	
	std::cout << " " << node->getNode()->node_id << " ";

	if (node->getLeftChild() != NULL)
		inOrder(node->getLeftChild());	

	if (node->getRightChild() != NULL)
		inOrder(node->getRightChild());
}

void BinarySearchTree::inOrderPrint()
{
	std::cout << "In Order Print" << std::endl;
	inOrder(root);
	std::cout << std::endl;

}

void BinarySearchTree::inOrder(BinaryNode *node)
{
	if (node == NULL)
		return;
	
	
	if (node->getLeftChild() != NULL)
		inOrder(node->getLeftChild());	

	std::cout << " " << node->getNode()->node_id << " ";

	if (node->getRightChild() != NULL)
		inOrder(node->getRightChild());	

}