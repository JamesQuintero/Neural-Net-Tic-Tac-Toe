#include <iostream>
#include <string>
#include <random>
#include "global_functions.h"
#include "NN.h"



using namespace std;

NN::NN()
{
	start = new node();
	start->board = createMatrix(size);

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


//Player moves, so add to neural net
void NN::playerMove(int** board)
{
	bool exists=false;
	for(int x =0; x < ptr->next_index; x++)
	{
		bool board_matches=true;
		for(int y=0; y < size; y++)
		{
			for(int z =0; z < size; z++)
			{
				if(ptr->next[x]->board[y][z] != board[y][z])
				{
					board_matches=false;
					break;
				}
			}
		}


		//if board already exists, move to it
		if(board_matches)
		{
			node * temp = ptr;
			ptr = ptr->next[x];
			ptr->prev = temp;
			exists=true;
		}
	}

	//if neuron doesn't exist, create it
	if(exists==false)
	{
		//creates a copy of the board
		int** new_board = matrixBoard(  stringBoard(board,size) ,size);
		newNode(ptr, new_board);
		ptr = ptr->next[ptr->next_index-1];
	}
}


//neural network determines next best move and returns its coordinates
int** NN::AIMove(int** board, int** possible_moves)
{
	// cout<<"GOT HERE"<<endl;
	// cout<<"Next index: "<<ptr->next_index<<endl;

	// cout<<"ptr's board: "<<endl;
	// printBoard(ptr->board, size);
	// cout<<"Passed in board: "<<endl;
	// printBoard(board, size);
	// cout<<endl;

	// cout<<"Possible moves: "<<endl;
	// for(int x =0; x < size*size; x++)
	// {
	// 	cout<<"("<<possible_moves[x][0]<<", "<<possible_moves[x][1]<<")"<<endl;
	// }

	// cout<<"Road ahead: "<<endl;
	// printNet(ptr);


	//temporary pointer
	node * temp = ptr;

	//gets child node with highest good variable
	double highest=0;
	for(int x =0; x < ptr->next_index; x++)
	{
		// cout<<"X: "<<x<<endl;
		// printNode(ptr->next[x]);


		if(ptr->next[x]->good > highest)
		{
			highest = ptr->next[x]->good;
			temp = ptr->next[x];
			// cout<<"Highest good: "<<highest<<endl;
		}




		//removes next move from possible moves so that only new possible moves are left
		for(int y =0; y < size*size; y++)
		{
			int row = possible_moves[y][0];
			int col = possible_moves[y][1];

			//if the board has a filled in possible move, then it moved there
			if(possible_moves[y][0]!=-1 && ptr->next[x]->board[row][col]!=0)
			{
				possible_moves[y][0]=-1;
				possible_moves[y][1]=-1;
				break;
			}
		}
	}

	// cout<<"GOT HERE 2"<<endl;

	//remaining possible moves are new moves, and should be added to the neural net
	int num_added=0;
	for(int y =0; y < size*size; y++)
	{
		//new move
		if(possible_moves[y][0]!=-1)
		{
			//creates a copy of the board
			int** new_board = matrixBoard(  stringBoard(board,size) ,size);
			//sets new move
			new_board[possible_moves[y][0]][possible_moves[y][1]] = this->piece;
			newNode(ptr, new_board);
			num_added++;
		}
	}

	//if the highest is lower than the default of 0.5, pick random move from new positions
	if(highest<default_good && num_added>0)
	{
		// temp = ptr->next[ptr->next_index-1];
		//random numbers between 0 and next_index. 
		//if neuron at ptr->next[random]==0.5, go there. 
		//else, continue guessing. 
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(ptr->next_index-num_added, ptr->next_index-1); // guaranteed unbiased

		int random_integer=uni(rng);
		temp = ptr->next[random_integer];
	}

	//creates a link from new node to its parent
	temp->prev = ptr;
	//move to new node
	ptr = temp;

	//returns copy of ptr's board
	return matrixBoard( stringBoard(ptr->board,size), size);
	
}

//the series of steps had a good outcome, to add the weights to good for the steps
void NN::goodOutcome()
{
	double increment = perc;

	while (ptr!=start)
	{
		ptr->good += increment*ptr->good;
		//cuts percentage increment
		increment/=2;

		// cout<<"Currently at "<<endl;
		// printNode(ptr);

		//moves up to parent
		ptr = ptr->prev;
	}
}

//the series of steps had a good outcome, to add the weights to good for the steps
void NN::badOutcome()
{
	double increment = perc;

	while (ptr!=start)
	{
		ptr->good -= increment*ptr->good;
		//cuts percentage increment
		increment/=2;

		// cout<<"Currently at "<<endl;
		// printNode(ptr);

		//moves up to parent
		ptr = ptr->prev;
	}
}




//prints node's contents in a readable format
void NN::printNode(node * ptr)
{
	//prints tic-tac-toe board
	printBoard(ptr->board, size);
	cout<<"Num next nodes: "<<ptr->next_index<<endl;
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

