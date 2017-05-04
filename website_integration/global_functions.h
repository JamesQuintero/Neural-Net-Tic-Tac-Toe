#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// #ifndef GLOBAL_FUNCTIONS_H
// #define GLOBAL_FUNCTIONS_H

using namespace std;

//prints the contents of a 2D array into a tic-tac-toe pattern
inline extern vector<string> getPrintBoard(int** board, int size)
{
	vector<string> to_print;

	for(int x =0; x < size; x++)
	{
		for(int y =0; y < size; y++)
		{
			//1 = X
			if(board[x][y]==1)
			{
				cout<<"[X]";
				to_print.push_back("1");
			}
			else if(board[x][y]==2)
			{
				cout<<"[O]";
				to_print.push_back("2");
			}
			else
			{
				cout<<"[ ]";
				to_print.push_back("0");
			}
		}
		cout<<endl;
	}

	return to_print;

}

//creates a size x size 2D array
inline extern int** createMatrix(int size)
{
	int** rows = new int*[size];
	for(int x =0; x <size; x++)
	{
		int* col = new int[size];

		//places 0
		for(int y =0; y < size; y++)
			col[y]=0;

		rows[x] = col;
	}

	return rows;
}

//returns matrix version of the board if provided with string version
inline extern int** matrixBoard(string board, int side_size)
{
	int** rows = createMatrix(side_size);

	int row=0;
	int col=0;
	for(int x =0; x < board.length(); x++)
	{
		int item = std::stoi(board.substr(x,1));
		rows[row][col]=item;

		col++;

		//goes to next row
		if(col == side_size)
		{
			col=0;
			row++;
		}
	}


	return rows;
}


//returns string version of the board if provided with matrix version
inline extern string stringBoard(int** board, int side_size)
{
	string new_board="";

	int row=0;
	int col=0;
	for(int x =0; x < side_size; x++)
	{
		for(int y =0; y < side_size; y++)
			new_board += to_string(board[x][y]);
	}

	return new_board;
}

inline extern bool checkFileExists(string name)
{
    if (FILE *file = fopen(name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
        return false;
}

//writes the array of strings to the specified file
inline extern void writeToFile(string path, vector<string> lines)
{
	ofstream myfile;
	myfile.open (path);

	for(int x =0; x < lines.size(); x++)
		myfile << lines[x] <<"\n";

	myfile.close();
}

//writes the array of strings to the specified file
inline extern vector<string> readFromFile(string path)
{

	vector<string> output;
	string line;


	ifstream myfile(path);


	if (myfile.is_open())
	{
		while (getline(myfile,line))
		{
			cout << line << '\n';
			output.push_back(line);
		}
	}
	else
		cout<<"Unable to open file"<<endl; 

	myfile.close();

	return output;
}


//returns vector of portions of string s deliminated by char delim
inline extern vector<string> split(string s, char delim)
{
    std::stringstream ss;
    ss.str(s);

    vector<string> result;
    string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}