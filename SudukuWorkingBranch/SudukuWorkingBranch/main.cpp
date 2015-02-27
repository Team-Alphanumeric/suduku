#include <iostream>
#include <fstream>
#include "board.h"
using namespace std;

int main()
{
	ifstream fin;

	// Read the sample grid from the file.
	string fileName = "sudoku.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		system("pause");
		exit(1);
	}

	try
	{
		board b1(SquareSize);

		while (fin && fin.peek() != 'Z')
		{							
			b1.initialize(fin);			
			b1.print();			
			b1.printConflicts();  
			system("pause");
		}
	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl;
		system("puase");
		exit(1);
	}
}
