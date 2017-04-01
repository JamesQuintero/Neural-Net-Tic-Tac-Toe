#include <iostream>
#include <string>
#include "global_functions.h"
#include "Tic-Tac-Toe.h"


using namespace std;

TicTacToe::TicTacToe()
{
	cout<<"Initialized"<<endl;

	NN nn;
	// nn.newNode(nn.start, "000010000");
	// nn.printNet(nn.start);

	//player is O
	player_piece=2;
	//AI is X
	AI_piece=1;

	resetGame();
}

//runs the game
void TicTacToe::run()
{

	bool player_won=false;
	bool AI_won=false;
	bool success=true;
	//goes forever until someone wins or until someone has an unsuccessful move (board is full)
	while(player_won==false && AI_won==false && success==true)
	{

		if(playerWon())
			player_won=true;
		else if(AIWon())
			AI_won=true;
		//if no one's yet run, continue the game
		else
		{
			//player moves
			if(isPlayersTurn())
			{
				success = playersMove();
				break;
			}
			//AI moves
			else
			{
				success = AIMove();
				break;
			}

			changeTurn();
		}
	}

}


//Player turn to move. returns true if successful
bool TicTacToe::playersMove()
{
	cout<<"Player's move. "<<endl;
	cout<<"Current board: "<<endl;
	printBoard(board, size);

	int row=0;
	int col=0;

	bool valid_move=false;
	while(valid_move==false)
	{
		cout<<"Row: ";
		cin>>row;

		cout<<"Col: ";
		cin>>col;
		cout<<endl;

		int** possible_moves = possibleMoves();

		//no possible moves
		if(possible_moves[0][0]==-1)
			return false;

		//determines if given move is a possible move
		for(int x =0; x < size*size; x++)
		{
			if(possible_moves[x][0]==row && possible_moves[x][1]==col)
			{
				valid_move=true;
				break;
			}
		}

	}

	//player places piece
	board[row][col]=player_piece;

	cout<<"New board: "<<endl;
	printBoard(board, size);

	return true;
}

bool TicTacToe::AIMove()
{
	cout<<"AI's move. "<<endl;
	cout<<"Current board: "<<endl;
	printBoard(board, size);

	int** possible_moves = possibleMoves();

	cout<<"Possible moves: "<<endl;
	for(int x =0; x < size*size; x++)
	{
		cout<<"("<<possible_moves[x][0]<<", "<<possible_moves[x][1]<<")"<<endl;
	}

	int** new_board = nn.move(board, possible_moves);

	cout<<"New board: "<<endl;
	printBoard(board, size);

	nn.printNet(nn.start);


	return false;
}

//returns coordinates of possible board configurations for next move consideration.
//unneeded parts of the list have coordinates (-1,-1).
int** TicTacToe::possibleMoves()
{
	int** moves = new int*[size*size];
	int index=0;

	for(int x =0; x < size; x++)
	{
		for(int y =0; y < size; y++)
		{
			//open space to move
			if(board[x][y]==0)
			{
				int* pair = new int[2];
				pair[0]=x;
				pair[1]=y;
				moves[index] = pair;
				index++;
			}

		}
	}

	for(int x =index; x < size*size; x++)
	{
		int* pair = new int[2];
		pair[0]=-1;
		pair[1]=-1;
		moves[index]=pair;
		index++;
	}


	return moves;
}





//returns true if player won the game
bool TicTacToe::playerWon()
{
	for(int x=0; x < size; x++)
	{
		//checks rows
		if(board[x][0]==player_piece && (board[x][0]==board[x][1] && board[x][1]==board[x][2]) )
			return true;
		//checks columns
		if(board[0][x]==player_piece && (board[0][x]==board[1][x] && board[1][x]==board[2][x]) )
			return true;
	}

	//checks top down diaganol
	if(board[0][0]==player_piece && (board[0][0]==board[1][1] && board[1][1]==board[2][2]) )
		return true;

	//checks bottom up diaganol
	if(board[2][0]==player_piece && (board[2][0]==board[1][1] && board[1][1]==board[0][2]) )
		return true;


	return false;
}

//returns true if AI won the game
bool TicTacToe::AIWon()
{
	for(int x=0; x < size; x++)
	{
		//checks rows
		if(board[x][0]==AI_piece && (board[x][0]==board[x][1] && board[x][1]==board[x][2]) )
			return true;
		//checks columns
		if(board[0][x]==AI_piece && (board[0][x]==board[1][x] && board[1][x]==board[2][x]) )
			return true;
	}

	//checks top down diaganol
	if(board[0][0]==AI_piece && (board[0][0]==board[1][1] && board[1][1]==board[2][2]) )
		return true;

	//checks bottom up diaganol
	if(board[2][0]==AI_piece && (board[2][0]==board[1][1] && board[1][1]==board[0][2]) )
		return true;


	return false;
}

//returns true if it's player's turn to move.
bool TicTacToe::isPlayersTurn()
{
	return turn;
}

//if it was player's turn, it's now AI's turn
void TicTacToe::changeTurn()
{
	//flips the boolean
	turn = !turn;
}

//resets the game
void TicTacToe::resetGame()
{
	board = createMatrix(size);
	turn = false;
}
