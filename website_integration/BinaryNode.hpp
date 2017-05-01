#include "Node.h"


#ifndef _BINARYNODE_HPP_
#define _BINARYNODE_HPP_


#include <cstddef>


class BinaryNode
{
	private:
		node* data;

		BinaryNode *leftChild;
		BinaryNode *rightChild;

	public:
		BinaryNode() : data(NULL), leftChild(NULL), rightChild(NULL)
		{}

		BinaryNode(node* data) : data(data), leftChild(NULL), rightChild(NULL)
		{}

		~BinaryNode()
		{
			leftChild = NULL;
			rightChild = NULL;
		}

		node* getNode() { return data; }
		void setNode(node* data) { data = data; }

		BinaryNode* getLeftChild() { return leftChild; }
		BinaryNode* getRightChild() { return rightChild; }

		void setLeftChild(BinaryNode* node) { leftChild = node; }
		void setRightChild(BinaryNode* node) { rightChild = node; }

};

#endif