//
// Copyright (c) James Quintero 2017
//

#include <iostream>
#include <string>



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

		
		struct node
		{
			int** board;
			// double good = default_good;
			double good=0;
			double bad=0;
			double okay=0;
			
			int next_index=0;
			int prev_index=0;
			node** next = new node*[size*size];
			node* prev;
		};
		

	public:
		int piece=0;

		node * start;
		node * ptr;


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

		void saveNet();
		void saveNode();

		void printNode(node * ptr);
		void printNet(node * ptr);



};



#endif


