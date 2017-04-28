//
// Copyright (c) James Quintero 2017
//

#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char** argv)
{
  ofstream myfile;
  myfile.open ("testfile.txt");
  myfile << "Hello James";           
  myfile.close();
	
	return 0;
}
