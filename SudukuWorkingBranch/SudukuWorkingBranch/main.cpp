#include <iostream>
#include <fstream>
#include "board.h"
using namespace std;

int board::numSolveIterations = 0;

int main()
{
	int boardNumber = 0;
	int iterSum = 0, puzzleCount = 0;
	ifstream fin;

	// Read the sample grid from the file.
	string fileName = "sudoku.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		system("pause");
		return 0;		
	}

	try
	{
		board b1(SquareSize);

		while (fin && fin.peek() != 'Z')
		{	
			cout << "Testing a new Board! " << endl;
			boardNumber++;
			b1.initialize(fin);		
			//system("pause");
			
			b1.print();		
			//system("pause");
			
			bool finalResult = b1.solveBoard();
			cout << "Number of iteratinos " << board::numSolveIterations << endl;
			iterSum += board::numSolveIterations; puzzleCount++;
			//if(finalResult) { std::cout << "The board was solved!!! " << endl; }
			//else { std::cout << "Unable to solve this board :(" << endl; }
			
			std::cout << "Result:" << endl;
			b1.print();
		}

		cout << "All " << puzzleCount << " puzzles attempted in " << iterSum << " iterations.\tAverage: " << iterSum / puzzleCount << endl;
		system("pause");
	}
	catch (indexRangeError &ex)
	{
		std::cout << "In main: " << ex.what() << endl;
	}
	catch (rangeError &ex)
	{
		std::cout << "In main: " << ex.what() << endl;
	}
	catch (invalid_argument &ex)
	{
		std::cout << "In main: " << ex.what() << endl;
	}
	catch (...)
	{
		std::cout << "In main: Unspecified exception" << endl;
	}
	return 0;
}
