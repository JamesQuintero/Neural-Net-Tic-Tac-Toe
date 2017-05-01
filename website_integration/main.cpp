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
	    cerr << "Invalid number " << argv[1] << '\n';


	cout<<"User_id: "<<user_id<<endl;

	TicTacToe ttt(user_id);
	ttt.run();

	
	// system("pause");
	return 0;
}
