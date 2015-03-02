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
		//system("pause");
		//exit(1);
	}

	try
	{
		board b1(SquareSize);

		while (fin && fin.peek() != 'Z')
		{	
			cout << "Testing a new Board! " << endl;
			b1.initialize(fin);			
			b1.print();			
			cout << "\nPrinting conflicts: " << endl;
			b1.printConflicts();  
			cout << "\nDoes 3 fit in 1,2? " << ((b1.numberFit(1, 2, 3)) ? "yes" : "no" ) << "." << endl;
			b1.setCell(1, 2, 3);
			system("pause");
			b1.print();
			cout << "\nClearing cell(1,2)" << endl;
			b1.clearCell(1, 2);				
			b1.print();
			b1.boardSolved();
			system("pause");
		}
	}
	catch (indexRangeError &ex)
	{
		cout << "In main: " << ex.what() << endl;
		//system("puase");
		//exit(1);
	}
	catch (invalid_argument &ex)
	{
		cout << "In main: " << ex.what() << endl;
		//system("puase");
		//exit(1);
	}
	catch (...)
	{
		cout << "In main: Unspecified exception" << endl;
	}
	return 0;
}
