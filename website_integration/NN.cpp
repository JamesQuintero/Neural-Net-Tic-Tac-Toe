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
	start->next = new node*[size*size];
	start->node_id = total_node_ids;
	total_node_ids++;

	this->ptr = start;


	srand(time(0));  // needed once per program run
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
		int** new_board = matrixBoard( stringBoard(board,size) ,size);
		::newNode(ptr, new_board, size);
		ptr = ptr->next[ptr->next_index-1];
		ptr->node_id = total_node_ids;
		total_node_ids++;
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
			int** new_board = matrixBoard( stringBoard(board,size) ,size);
			//sets new move
			new_board[possible_moves[y][0]][possible_moves[y][1]] = this->piece;
			::newNode(ptr, new_board, size);
			ptr->next[ptr->next_index-1]->node_id = total_node_ids;
			total_node_ids++;

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


void NN::loadNet()
{
	cout<<"loadNet();"<<endl;

	int rc;
	char* szErrMsg = 0;

	//if net already exists, remove default start node so that it can be loaded from net
	if (FILE *file = fopen("ttt_NN.db", "r"))
	{
        fclose(file);

        //releases old start
        delete start;
        start=NULL;

        total_node_ids--;
        cout<<"Net already exists, so remove default head pointer"<<endl;
    }


	//opens sqlite file
	sqlite3 *db;
	rc = sqlite3_open("ttt_NN.db", &db);

	if(rc!=0)
	{
		cout<<"Can't open database file"<<endl;
		return;
	}


	BinarySearchTree net;

	//starts by getting the start node, which is where node_id=1.
	//recursively gets the current node's next nodes from the db
	// node* copy = start;
	start = loadNode(&net, db, start, 1);

	//start at start
	this->ptr = start;

	//global node id should be one higher than the highest loaded node id
	total_node_ids++;




	cout<<"Total node id: "<<total_node_ids<<endl;

	if(db)
		sqlite3_close(db);
}

//binary search tree points to existing nodes when reconstructing the neural network
node* NN::loadNode(BinarySearchTree* bst, sqlite3* db, node * ptr, int node_id)
{
	// cout<<"loadNode();"<<endl;

	//returns pointer to node if it exists
	node* existing_node = bst->find(node_id);
	//true if it exists
	bool existed = true;

	//stores node_ids to be loaded
	vector<int> next_node_ids;

	//if node doesn't already exist, load it from db
	if(existing_node == NULL)
	{
		existed = false;
		// cout<<"Node doesn't exist, so load from db"<<endl;


        sqlite3_stmt* stmt;
        int s = SQLITE_ROW;

        string selectQuery = "SELECT * FROM ttt_neural_net WHERE node_id="+to_string(node_id)+" LIMIT 1";
        sqlite3_stmt *selectStmt;

        if(sqlite3_prepare_v2(db, selectQuery.c_str(), selectQuery.size()+1, &selectStmt, NULL) == SQLITE_OK)
        {
        	sqlite3_step (selectStmt);

	       	int node_id = sqlite3_column_int(selectStmt, 0);
	       	string board = (char*)sqlite3_column_text(selectStmt, 1);
	       	double good = sqlite3_column_double(selectStmt, 2);
	       	double bad = sqlite3_column_double(selectStmt, 3);
	       	double okay = sqlite3_column_double(selectStmt, 4);
	       	int next_index = sqlite3_column_double(selectStmt, 5);
	       	string next = (char*)sqlite3_column_text(selectStmt, 6);

	        sqlite3_finalize(selectStmt);

	        // cout<<"node_id retrieved: "<<node_id<<endl;
	        // cout<<"Board: "<<board<<endl;
	        // cout<<"Good: "<<good<<endl;
	        // cout<<"Bad: "<<bad<<endl;
	        // cout<<"Okay: "<<okay<<endl;
	        // cout<<"next_index: "<<next_index<<endl;
	        // cout<<"Next: "<<next<<endl;

	        vector<string> next_vector = split(next, ',');
	        for(int x =0; x < next_vector.size(); x++)
	        {
	        	next_node_ids.push_back(std::stoi(next_vector[x]));
	        	// cout<<x<<") "<<next_vector[x]<<endl;
	        }

	        existing_node = new node();
	        existing_node->node_id = node_id;
	        existing_node->board = matrixBoard(board, size);
	        existing_node->good = good;
	        existing_node->bad = bad;
	        existing_node->okay = okay;
	        existing_node->next_index = 0;
	        existing_node->next = new node*[size*size];

	        //adds to binary search tree for easier linking later on
			bst->insert(existing_node);
        }
        else
        {
        	cout<<"Error with select query: "<<selectQuery<<endl;
        	return NULL;
        }
		
	}

	//add node if it already existed or now exists
	if(existing_node!=NULL)
	{
		// cout<<"Node exists"<<endl;

		//global node id should be higher than the highest loaded node id
		if(total_node_ids < existing_node->node_id)
			total_node_ids = existing_node->node_id;

		if(ptr!=NULL)
		{
			ptr->next[ptr->next_index] = existing_node;
			ptr->next_index++;
		}
		//ptr is only null when loading the head pointer
		else
			ptr = existing_node;
	}

	//gets next ids of next node if it didn't already exist in BST (implying its next nodes were already loaded)
	//this avoids infinite recursion on a circularly linked list
	if(existed == false)
	{
		for(int x =0; x < next_node_ids.size(); x++)
		{
			node* copy = existing_node;
			loadNode(bst, db, copy, next_node_ids[x]);
		}
	}

	return ptr;
}

//saves the neural network to a db file for later retrieval
void NN::saveNet()
{
	cout<<"saveNet();"<<endl;

	int rc;
	char* szErrMsg = 0;

	//opens sqlite file
	sqlite3 *db;
	rc = sqlite3_open("ttt_NN.db", &db);

	if(rc!=0)
	{
		cout<<"Can't open database file"<<endl;
		return;
	}
	

	//creates table
	string create_table = "CREATE TABLE IF NOT EXISTS ttt_neural_net (node_id INTEGER PRIMARY KEY, board VARCHAR(9), good FLOAT, bad FLOAT, okay FLOAT, num_next INTEGER, next TEXT)";
	sqlite3_stmt* statement;
	sqlite3_prepare(db, create_table.c_str(), create_table.size(), &statement, NULL);
	rc = sqlite3_step(statement);

	if(rc != SQLITE_DONE)
	{
		cout<<"SQL Error: "<<szErrMsg<<endl;
		sqlite3_free(szErrMsg);
	}
	else
		cout<<"Everything worked"<<endl;


	//recursively saves neural network
	saveNode(db, start);

	if(db)
		sqlite3_close(db);
     
}

//saves node to db file
void NN::saveNode(sqlite3* db, node * ptr)
{
	// cout<<"saveNode();"<<endl;


	int node_id = ptr->node_id;
	// cout<<"Node ID: "<<node_id<<endl;
	string board = stringBoard(ptr->board, size);
	// cout<<"Board: "<<board<<endl;
	double good = ptr->good;
	double bad = ptr->bad;
	double okay = ptr->okay;
	// cout<<"Good: "<<good<<", Bad: "<<bad<<", Okay: "<<okay<<endl;
	int num_next = ptr->next_index;
	// cout<<"Num next: "<<num_next<<endl;

	//combines ptr's next's node_ids into string
	//goes to num_next-1 so that the last next node_id can be added after the for loop to avoid also adding a comma
	string next = "";
	for(int x =0; x < num_next-1; x++)
	{
		next+= to_string(ptr->next[x]->node_id)+",";
	}
	if(num_next>0)
		next += to_string(ptr->next[num_next-1]->node_id);
	// cout<<"Next: "<<next<<endl;

	int rc;
	char* szErrMsg = 0;

	//inserts node if it doesn't exist
	string insert_node = string("INSERT INTO ttt_neural_net ") +
	string("(node_id, board, good, bad, okay, num_next, next) ") +
	string("VALUES ("+to_string(node_id)+", \""+string(board)+"\", "+to_string(good)+", "+to_string(bad)+", "+to_string(okay)+", "+to_string(num_next)+", \""+string(next)+"\");");
	// cout<<"Select Query: "<<insert_node<<endl;
	sqlite3_stmt* statement;

	sqlite3_prepare_v2(db, insert_node.c_str(), insert_node.size(), &statement, NULL);
	rc = sqlite3_step(statement);

	//insert failed because record already exists
	if(rc == SQLITE_CONSTRAINT)
	{
		//inserts updates node if it needs updating
		string update_node = string("UPDATE ttt_neural_net SET ") +
		string("good="+to_string(good)+", bad="+to_string(bad)+", okay="+to_string(okay)+", num_next="+to_string(num_next)+", next=\""+string(next)+"\"  ") +
		string("WHERE node_id=="+to_string(node_id)+" AND (good!="+to_string(good)+" OR bad!="+to_string(bad)+" OR okay!="+to_string(okay)+" OR num_next!="+to_string(num_next)+" OR next!=\""+string(next)+"\");");
		// cout<<"Update Query: "<<update_node<<endl;

		sqlite3_prepare_v2(db, update_node.c_str(), update_node.size(), &statement, NULL);
		rc = sqlite3_step(statement);

		if(rc != SQLITE_DONE)
			cout<<"SQL Update Error: "<<sqlite3_errmsg(db)<<endl;
	}
	else if(rc != SQLITE_DONE)
		cout<<"SQL Select Error: "<<sqlite3_errmsg(db)<<endl;


	// cout<<"Result code: "<<rc<<endl;

	//recursively saves ptr's next nodes
	for(int x = 0; x < num_next; x++)
	{
		node* copy = ptr->next[x];
		saveNode(db, copy);
	}

}

int NN::dataReturn(void *varNotUsed, int count, char** data, char** szColName)
{
	for(int x = 0; x < count; x++)
	{
		cout<<szColName[x]<<" = "<<data[x]<<endl;
	}
	cout<<endl;

	return 0;
}


//prints node's contents in a readable format
void NN::printNode(node * ptr)
{
	//prints tic-tac-toe board
	// printBoard(ptr->board, size);
	
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

