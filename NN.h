#include <iostream>
#include <string>



#ifndef NN_H
#define NN_H

using namespace std;



class NN
{
	private:

		static const int size = 3;
		//every new "neuron" has a good weight of 0.5, meaning its path is not good or bad
		static constexpr double default_good = 0.5;
		//percentage of the "neuron" that gets increment if its path results in a good outcome
		static constexpr double perc = 0.25;

		
		struct node
		{
			// string board="000000000";
			int** board;
			double good = default_good;
			
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
		//int** matrixBoard(string board);

		//returns a new board containing NN's next move
		int** AIMove(int** board, int** possible_moves);
		void playerMove(int** board);
		void printNode(node * ptr);
		void printNet(node * ptr);
		//increments the good variables if the outcome was good
		void goodOutcome();
		//decrements the good variables if the outcome was good
		void badOutcome();



};



#endif


