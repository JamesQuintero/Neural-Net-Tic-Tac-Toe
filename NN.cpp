//
// Copyright (c) James Quintero 2017
//

#include <iostream>
#include <string>
#include <ctime>
#include "sqlite/sqlite3.h"

#include "global_functions.h"
#include "NN.h"



using namespace std;

NN::NN()
{
	start = new node();
	start->board = createMatrix(size);
	this->ptr = start;

	srand(time(0));  // needed once per program run
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

	//pointer to parent
	next->prev = ptr;	
}


//Player moves, so add to neural net
void NN::playerMove(int** board)
{

	bool exists=false;
	for(int x =0; x < ptr->next_index; x++)
	{
		//checks if the node's board matches the parameter
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


		//if board does match, move to it
		if(board_matches)
		{
			node * temp = ptr;
			ptr = ptr->next[x];
			ptr->prev = temp;
			exists=true;
			break;
		}
	}

	//if node with parameter board doesn't exist, create it
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

	//temporary pointer
	node * temp = ptr;

	//gets child node with highest ratio of good to bad
	double highest = -100;
	int num_ties = 0;
	for(int x =0; x < ptr->next_index; x++)
	{
		
		double ratio = getRatio(ptr->next[x]);

		//tracks the child node with the highest good
		if(ratio > highest)
		{
			highest = ratio;
			temp = ptr->next[x];
			num_ties = 0;
		}
		//counts number of ties so that the AI can pick between them randomly
		else if(ratio == highest)
			num_ties++;

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
	if(num_ties>=1 || num_added>0)
	{
		//if the current highest isn't the actual highest after adding new nodes
		if(num_added>0)
			highest = 0;

		int random_integer;
		double ratio = highest-1; //initialize so that the while loop is run at least once

		while(ratio < highest)
		{
			random_integer = (rand() % ptr->next_index);			
			ratio = getRatio(ptr->next[random_integer]);
		}

		temp = ptr->next[random_integer];
	}

	//creates a link from new node to its parent
	temp->prev = ptr;
	//move to new node
	ptr = temp;

	//returns copy of ptr's board
	return matrixBoard( stringBoard(ptr->board,size), size);
	
}

//gets ratio of the weights
double NN::getRatio(node * ptr)
{
	double good = ptr->good;
	double bad = ptr->bad;
	double okay = ptr->okay;
	double ratio = (good+okay)-bad;
	return ratio;
}

//the series of steps had a good outcome, to add the weights to good for the steps
void NN::goodOutcome()
{
	double increment = perc;
	while (ptr!=start)
	{
		ptr->good += increment;
		increment/=2;

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
		ptr->bad += increment;
		increment/=2;

		//moves up to parent
		ptr = ptr->prev;
	}
}

//the series of steps had a good outcome, to add the weights to good for the steps
void NN::okayOutcome()
{
	double increment = perc;
	while (ptr!=start)
	{
		ptr->okay += increment;
		increment/=2;

		//moves up to parent
		ptr = ptr->prev;
	}
}



//saves the neural network to a db file for later retrieval
void NN::saveNet()
{
	//opens sqlite file
	sqlite3 *db;
	sqlite3_open("ttt_NN.db", &db);

 //  string createQuery = "CREATE TABLE IF NOT EXISTS items
 //   (userid INTEGER PRIMARY KEY, ipaddr TEXT,username TEXT,useradd TEXT,userphone INTEGER,age INTEGER, time TEXT NOT NULL DEFAULT (NOW()));";

	// sqlite3_stmt *createStmt;
	// cout << "Creating Table Statement" << endl;
	// sqlite3_prepare(db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);

	// cout << "Stepping Table Statement" << endl;
	// if (sqlite3_step(createStmt) != SQLITE_DONE) cout << "Didn't Create Table!" << endl;

	// string insertQuery = "INSERT INTO items 
	// (time, ipaddr,username,useradd,userphone,age) VALUES 
	// ('7:30', '192.187.27.55','vivekanand','kolkatta','04456823948',74);"; // WORKS!

	// sqlite3_stmt *insertStmt;
	// cout << "Creating Insert Statement" << endl;
	// sqlite3_prepare(db, insertQuery.c_str(), insertQuery.size(), &insertStmt, NULL);
	// cout << "Stepping Insert Statement" << endl;
	// if (sqlite3_step(insertStmt) != SQLITE_DONE) cout << "Didn't Insert Item!" << endl;  
	// 	return 0;	
     
}

//saves node to db file
void NN::saveNode()
{

}


//prints node's contents in a readable format
void NN::printNode(node * ptr)
{
	//prints tic-tac-toe board
	printBoard(ptr->board, size);
	cout<<"Num next nodes: "<<ptr->next_index<<endl;
	cout<<"Good: "<<ptr->good<<endl;
	cout<<"Bad: "<<ptr->bad<<endl;
	cout<<"Okay: "<<ptr->okay<<endl;
}

//prints the neural network in one long output.
void NN::printNet(node * ptr)
{
	printNode(ptr);
	cout<<endl;

	for(int x =0; x < ptr->next_index; x++)
	{
		printNet(ptr->next[x]);
	}

}

