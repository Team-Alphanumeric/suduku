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
			b1.numberFit(1, 2, 3);
			cout << "Does 3 fit in 1,2? " << b1.numberFit(1, 2, 3) << endl;
			b1.setCell(1, 2, 3);		
			cout << "Does 7 fit in 5,4? " << b1.numberFit(5, 4, 7) << endl;
			b1.setCell(5, 4, 7);
			b1.print();
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
