//
// Copyright (c) James Quintero 2017
//

#include <iostream>
#include <sstream>
#include "Tic-Tac-Toe.h"

using namespace std;


int main(int argc, char** argv)
{

	// int user_id = (int)*argv[1] - 48;
	cout<<"Remeber that a game ID must be passed in"<<endl;


	istringstream ss(argv[1]);
	int user_id;
	if (!(ss >> user_id))
	    cerr << "User_id Invalid number: " << argv[1] << '\n';

	//if choice==1, user wants to play untrained AI.
	//if choice==2, user wants to play trained AI.
	istringstream ss2(argv[2]);
	int choice;
	if (!(ss2 >> choice))
	    cerr << "Choice Invalid number: " << argv[2] << '\n';


	cout<<"User_id: "<<user_id<<endl;
	cout<<"Choice: "<<choice<<endl;

	TicTacToe ttt(user_id);
	ttt.run(choice);

	
	return 0;
}
