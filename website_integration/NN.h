//
// Copyright (c) James Quintero 2017
//

#include <iostream>
#include <string>
#include "sqlite/sqlite3.h"
#include "BinarySearch.hpp"
#include "Node.h"


#ifndef NN_H
#define NN_H

using namespace std;



class NN
{
	private:
		//side length of the tic-tac-toe board
		static const int size = 3;
		//percentage of the "neuron" that gets increment if its path results in a good outcome
		static constexpr double perc = 0.25;
		

	public:
		int piece=0;

		node * start;
		node * ptr;
		int total_node_ids = 1;


		NN();
		//links a new node to ptr, with initialized board
		void newNode(node * ptr, int** board);

		//returns a new board containing NN's next move
		int** AIMove(int** board, int** possible_moves);
		void playerMove(int** board);
		double getRatio(node * ptr);

		//increments the good variables if the outcome was good
		void goodOutcome();
		//decrements the good variables if the outcome was good
		void badOutcome();
		//decrements the good variables only half if the outcome was okay
		void okayOutcome();

		//recursively saves neural network to db file
		void saveNet();
		void saveNode(sqlite3* db, node * ptr);

		//uses Binary Search Tree to properly load neural network from db file
		void loadNet();
		node* loadNode(BinarySearchTree* bst, sqlite3* db, node * ptr, int node_id);

		void printNode(node * ptr);
		void printNet(node * ptr);


		int dataReturn(void* notUsed, int count, char** data, char** szColName);


};



#endif


