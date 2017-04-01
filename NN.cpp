#include <iostream>
#include <string>
#include "global_functions.h"
#include "NN.h"



using namespace std;

NN::NN()
{
	start = new node();

	this->ptr = start;

	// start->next[start->next_index] = new node();
	// start->next_index++;

	// start->next[start->next_index] = new node();
	// start->next_index++;

	// ptr = start->next[0];
	// ptr->board="000010000";

	// ptr->next = new node*[9];
	// ptr->next[ptr->next_index] = new node();
	// ptr->next_index++;
	// ptr->next[0]->board="000010100";

	// ptr = start->next[1];
	// ptr->board="000012000";
}

//links a new node to ptr, with initialized board
void NN::newNode(node * ptr, int** board)
{
	ptr->next[ptr->next_index] = new node();

	//ptr to new next node
	node * next = ptr->next[ptr->next_index];
	next->board = board;

	//increments number of next nodes
	ptr->next_index++;

	next->prev = ptr;	
}



//neural network determines next best move and returns its coordinates
int** NN::move(int** board, int** possible_moves)
{

	cout<<"GOT HERE"<<endl;
	cout<<"Next index: "<<start->next_index<<endl;

	printNet(start);

	return board;

	//temporary pointer
	node * temp = ptr;

	//gets child node with highest good variable
	double highest=0;
	for(int x =0; x < size*size; x++)
	{
		if(ptr->next[x]!=NULL && ptr->next[x]->good > highest)
		{
			highest = temp->good;
			temp = ptr->next[x];
			cout<<"Highest good: "<<highest<<endl;

		}




		//removes next move from possible moves so that only new possible moves are left
		for(int y =0; y < size*size; y++)
		{
			int row = possible_moves[y][0];
			int col = possible_moves[y][1];

			//if the board has a filled in possible move, then it moved there
			if(ptr->next[x]->board[row][col]!=0)
			{
				int* new_pair = new int[2];
				new_pair[0]=-1;
				new_pair[1]=-1;
				possible_moves[y]=new_pair;
			}
		}
	}

	//remaining possible moves are new moves, and should be added to the neural net
	bool added_new=false;
	for(int y =0; y < size*size; y++)
	{
		//new move
		if(possible_moves[y][0]!=-1)
		{
			//creates a copy of the board
			int** new_board = matrixBoard(  stringBoard(board,size) ,size);
			new_board[possible_moves[y][0]][possible_moves[y][1]]=1;
			newNode(ptr, new_board);
			added_new=true;
		}
	}

	//if the highest is lower than the default of 0.5, pick move from new positions
	if(highest<0.5 && added_new)
		temp = ptr->next[ptr->next_index-1];

	return temp->board;
	
}




//prints node's contents in a readable format
void NN::printNode(node * ptr)
{
	// //converts string board to 2D array board
	// int** matrix_board = matrixBoard(ptr->board, size);

	//prints tic-tac-toe board
	printBoard(ptr->board, size);
	cout<<"Good: "<<ptr->good<<endl;
}

//prints the neural network. 
void NN::printNet(node * ptr)
{
	printNode(ptr);
	cout<<endl;

	for(int x =0; x < ptr->next_index; x++)
	{
		printNet(ptr->next[x]);
	}

}

