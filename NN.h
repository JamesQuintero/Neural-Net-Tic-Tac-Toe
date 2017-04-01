#include <iostream>
#include <string>



#ifndef NN_H
#define NN_H

using namespace std;



class NN
{
	private:

		static const int size=3;

		
		struct node
		{
			// string board="000000000";
			int** board;
			double good=0.5;
			
			int next_index=0;
			int prev_index=0;
			node** next = new node*[size*size];
			node* prev;
		};
		

	public:

		node * start;
		node * ptr;


		NN();
		//links a new node to ptr, with initialized board
		void newNode(node * ptr, int** board);
		//int** matrixBoard(string board);

		//returns a new board containing NN's next move
		int** move(int** board, int** possible_moves);
		void printNode(node * ptr);
		void printNet(node * ptr);



};



#endif


