//
// Copyright (c) James Quintero 2017
//

#include <iostream>
#include <string>
#include <unistd.h>
#include "global_functions.h"
#include "Tic-Tac-Toe.h"


using namespace std;

TicTacToe::TicTacToe(int user_id)
{
	NN nn;
	NN nn2;

	this->user_id = user_id;

	string base = "user_data";
	board_path = "./"+base+"/"+to_string(user_id)+"_board.txt";
	AI_path = "./"+base+"/"+to_string(user_id)+"_AI_move.txt";
	player_path = "./"+base+"/"+to_string(user_id)+"_player_move.txt";
	game_path = "./"+base+"/"+to_string(user_id)+"_game.txt";
	quit_path = "./"+base+"/"+to_string(user_id)+"_quit.txt";

	//player is O
	player_piece = 2;
	//AI is X
	AI_piece = 1;

	resetGame();
}

//runs the game
void TicTacToe::run(int choice)
{
	nn.loadNet();


	// while(true)
	while(true)
	{
		cout<<endl;
		cout<<"Menu: "<<endl;
		cout<<"1) Human vs AI"<<endl;
		cout<<"2) AI vs AI"<<endl;
		cout<<"3) Print Neural Nets"<<endl;

		// cout<<"Choice: ";
		// int choice;
		// cin>>choice;
		choice=1;


		//Human vs AI
		if(choice==1)
		{
				nn.piece = AI_piece;

				cout<<"New game Human vs AI"<<endl;
				resetGame();

				bool player_won=false;
				bool AI_won=false;
				//success= true as long as there are empty spots on the board
				bool success=true;
				//checks whether player wants to quit
				bool player_quit = false;



				//goes forever until someone wins or until someone has an unsuccessful move (board is full)
				while(player_won==false && AI_won==false && success==true && player_quit==false)
				{

					if(playerWon())
					{
						player_won=true;
						cout<<endl;
						cout<<"Player WON"<<endl;
						cout<<endl<<endl;
						nn.badOutcome();
					}
					else if(AIWon())
					{
						AI_won=true;
						cout<<"AI WON"<<endl;
						cout<<endl<<endl;
						nn.goodOutcome();
					}
					//if no one's yet won, continue the game
					else
					{
						//player moves
						if(isPlayersTurn())
							success = playersMove();
						//AI moves
						else
							success = AIMove(1);

						changeTurn();
					}

					//sleep for 0.5 seconds
					usleep(500000);

					//checks whether player wants to quit
					player_quit = checkPlayerQuit();

				}


				//game was a tie
				if (success==false)
					nn.badOutcome();

		}
		//AI vs AI
		else if(choice==2)
		{
				nn.piece = AI_piece;
				nn2.piece = player_piece;

				int num_games=0;
				cout<<"Num games: ";
				cin>>num_games;

				for(int x =0; x < num_games; x++)
				{
						cout<<"New game AI vs AI"<<endl;
						resetGame();

						bool AI_won=false;
						bool AI2_won=false;
						bool success=true;
						//goes forever until someone wins or until someone has an unsuccessful move (board is full)
						while(AI_won==false && AI2_won==false && success==true)
						{

							if(AIWon())
							{
								AI_won=true;
								cout<<"AI WON"<<endl;
								cout<<endl<<endl;
								nn.goodOutcome();
								nn2.badOutcome();
							}
							//AI #2 counts as player
							else if(playerWon())
							{
								AI2_won=true;
								cout<<endl;
								cout<<"AI2 WON"<<endl;
								cout<<endl<<endl;
								nn2.goodOutcome();
								nn.badOutcome();
							}
							//if no one's yet run, continue the game
							else
							{
								//player moves
								if(isPlayersTurn())
									success = AIMove(2);
								//AI moves
								else
									success = AIMove(1);

								changeTurn();
							}

							// system("pause");

						}

						//game was a tie
						if (success==false)
						{
							nn.badOutcome();
							nn2.badOutcome();
							// nn2.okayOutcome();
						}
				}
				
		}
		//Prints the neural nets
		else
		{
			cout<<"1) AI #1: "<<endl;
			cout<<"2) AI #2: "<<endl;

			cout<<"Choice: ";
			cin>>choice;

			if(choice==1)
				nn.printNet(nn.start);
			else
				nn2.printNet(nn2.start);

		}


		//saves network to db file
		// nn.saveNet();
	
	}

}


//Player's turn to move. returns true if successful
bool TicTacToe::playersMove()
{
	cout<<"Player's move. "<<endl;
	cout<<"Current board: "<<endl;
	vector<string> to_print = getPrintBoard(board, size);
	writeToFile(board_path, to_print);
	

	int row=0;
	int col=0;

	bool valid_move=false;
	while(valid_move==false)
	{

		int** possible_moves = possibleMoves();

		//no possible moves
		if(possible_moves[0][0]==-1)
			return false;

		//have user input be in a file that we check for
		//if it doesn't exist, don't move
		// changeTurn(); so that it's still the player's turn when the run while changes turn and goes again


		// cout<<"Row: ";
		// cin>>row;

		// cout<<"Col: ";
		// cin>>col;
		// cout<<endl;

		if(fileExists player file)
		{
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

	}

	//player places piece
	board[row][col]=player_piece;

	//add player's move to neural net
	nn.playerMove(board);

	cout<<endl<<endl<<endl;

	return true;
}

//AI's turn to move. returns true if successful. 
bool TicTacToe::AIMove(int AI_version)
{
	int** possible_moves = possibleMoves();

	cout<<"AI_version: "<<AI_version<<endl;

	//no possible moves
	if(possible_moves[0][0]==-1)
		return false;


	int** new_board;
	// //only when player vs AI
	// if(AI_version==0)
	// {
	// 	new_board = nn.AIMove(board, possible_moves);
	// }
	//AI #1 moves, and its move gets added to AI #2
	if(AI_version==1)
	{
		new_board = nn.AIMove(board, possible_moves);
		nn2.playerMove(new_board);
	}
	//AI #2 moves, and its move gets added to AI #1
	else if(AI_version==2)
	{
		new_board = nn2.AIMove(board, possible_moves);
		nn.playerMove(new_board);
	}
	board = new_board;

	//prints the board
	cout<<"New board: "<<endl;
	vector<string> to_print = getPrintBoard(board, size);

	cout<<endl<<endl<<endl;


	return true;
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

	//voids rest of list
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

	nn.ptr = nn.start;
	nn2.ptr = nn2.start;
}

//checks for a file that indicates whether the player wants to quit the game
bool TicTacToe::checkPlayerQuit()
{
	// string file = "./user_data/"+
	return false;
}