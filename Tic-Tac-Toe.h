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
		int player_piece;
		int AI_piece;

		NN nn;

	public:
		TicTacToe();
		

		//runs the game
		void run();
		//if it was player's turn, it's now AI's turn
		void changeTurn();
		//returns true if it's player's turn to move. false otherwise
		bool isPlayersTurn();
		//player's turn to move. returns true if successful
		bool playersMove();
		//AI's turn to move. returns true if successful
		bool AIMove();
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


