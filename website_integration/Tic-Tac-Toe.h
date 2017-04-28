//
// Copyright (c) James Quintero 2017
//

#include <iostream>
#include <string>
#include "NN.h"



#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

using namespace std;



class TicTacToe
{
	private:
		int** board;
		int size=3;
		//if true, player goes first. AI otherwise
		bool turn;
		//2, or O
		int player_piece;
		//1, or X
		int AI_piece;

		//AI #1
		NN nn;
		//AI #2
		NN nn2;

	public:
		int user_id;
		string board_path;
		string AI_path;
		string player_path;
		string game_path;

		TicTacToe(int user_id);
		

		//runs the game
		void run();
		//if it was player's turn, it's now AI's turn
		void changeTurn();
		//returns true if it's player's turn to move. false otherwise
		bool isPlayersTurn();
		//player's turn to move. returns true if successful
		bool playersMove();
		//AI's turn to move. returns true if successful
		bool AIMove(int AI_version);
		//returns true if player won the game
		bool playerWon();
		//returns true if AI won the game
		bool AIWon();
		//returns array of possible moves in string format
		int** possibleMoves();
		//reset's game
		void resetGame();

};



#endif


