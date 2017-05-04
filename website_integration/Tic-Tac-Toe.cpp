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

	//user wants to play a new AI
	if(choice==1)
	{

	}
	//user wants to play a trained AI
	else
	{
		nn.loadNet();
	}



		nn.piece = AI_piece;

		cout<<"New game Human vs AI"<<endl;
		resetGame();

		bool player_won=false;
		bool AI_won=false;
		//success= true as long as there are empty spots on the board
		bool success=true;
		//checks whether player wants to quit
		bool player_quit = false;


		vector<string> game_outcome;

		int total_wait = 0;

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

				game_outcome.push_back("1");
			}
			else if(AIWon())
			{
				AI_won=true;
				cout<<"AI WON"<<endl;
				cout<<endl<<endl;
				nn.goodOutcome();
				game_outcome.push_back("2");
			}
			//if no one's yet won, continue the game
			else
			{
				//sleep for 0.5 seconds
				usleep(500000);
				total_wait += 500000;

				//player moves
				if(isPlayersTurn())
				{
					//if player did move, reset timer
					if(checkFileExists(player_path))
						total_wait = 0;

					success = playersMove();

					
				}
				//AI moves
				else
				{
					success = AIMove(1);
				}

				changeTurn();
			}

			

			//checks whether player wants to quit, or has been inactive for >=5 minutes
			player_quit = checkPlayerQuit();
			if(player_quit || total_wait>=300000000)
			{
				player_quit = true;
				game_outcome.push_back("3");
			}

		}


		//game was a tie
		if (success==false)
		{
			nn.badOutcome();
			game_outcome.push_back("0");
		}


		writeToFile(game_path, game_outcome);

}


//Player's turn to move. returns true if successful
bool TicTacToe::playersMove()
{
	cout<<"Player's move. "<<endl;
	cout<<"Current board: "<<endl;
	vector<string> to_print = getPrintBoard(board, size);
	// writeToFile(board_path, to_print);
	

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
		// player_path

		// cout<<"Row: ";
		// cin>>row;

		// cout<<"Col: ";
		// cin>>col;
		// cout<<endl;

		vector<string> player_move = readFromFile(player_path);



		//if player did make a move
		if(player_move.size()!=0)
		{
			int slot_number = stoi(player_move[0]);
			row = (int)slot_number/3;
			col = (int)slot_number%3;

			cout<<"row: "<<row<<endl;
			cout<<"col: "<<col<<endl;

			//determines if given move is a possible move
			for(int x =0; x < size*size; x++)
			{
				if(possible_moves[x][0]==row && possible_moves[x][1]==col)
				{
					valid_move=true;
					break;
				}
			}

			//deletes player file
			remove(player_path.c_str());
		}
		//if player hasn't yet moved, change turn to counteract the other changeturn, and have nothing else change to create a loop. 
		else
		{
			cout<<"player move size is 0"<<endl;
			changeTurn();
			return true;
		}

	}

	//player places piece
	board[row][col]=player_piece;

	//add player's move to neural net
	nn.playerMove(board);

	//sends the website the new board
	to_print = getPrintBoard(board, size);
	writeToFile(board_path, to_print);

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

	//gets difference between boards so that AI's move can be sent to player
	//website uses 0-8 slot numbers instead of row and col 
	int slot_num=0;
	for(int x =0; x < size; x++)
	{
		for(int y =0; y < size; y++)
		{
			//if difference found, stop
			if(board[x][y] != new_board[x][y])
				goto label;
			//no difference, so increment
			else
				slot_num++;
		}
	}

	//save AI's move to file
	label:
	vector<string> lines;
	lines.push_back(to_string(slot_num));
	writeToFile(AI_path, lines);


	board = new_board;

	//prints the board
	cout<<"New board: "<<endl;
	vector<string> to_print = getPrintBoard(board, size);
	//sends the website the new board
	writeToFile(board_path, to_print);

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
	//true if player has quit
	bool player_quits = checkFileExists(quit_path);

	//delete file if player quit so that they can start a new game
	if(player_quits)
		remove(quit_path.c_str());

	return player_quits;
}