// Declarations and functions for project #4

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <exception>
#include <list>
#include <fstream>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank

const int SquareSize = 3;  //  The number of cells in a small square
//  (usually 3).  The board has
//  SquareSize^2 rows and SquareSize^2
//  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

//int numSolutions = 0;
/*****
* Note that the Board assumes that the board matrix goes from 
* 1-9 not 0-8 so value[0][0] will be an error, the first element
* should be value [1][1]
*****/

class board
	// Stores the entire Sudoku board
{
public:
	board(int);	
	void setCell(const int i,const int j,const ValueType newElement);
	void clearCell(const int i, const int j);
	void clear();
	void initialize(ifstream &fin);
	void print();
	bool isBlank(int, int);
	ValueType getCell(int, int);
	int squareNumber(const int i, const int j);
	bool numberFit(const int i, const int j, const ValueType testElement);
	void printConflicts();
	bool solveBoard();
	bool boardSolved();
	bool findMaxContradiction(int &i, int &j);
	board();
	~board();
	static int numSolveIterations; // number of iterations for the boardSolved function for a puzzle

private:

	// The following matrices go from 1 to BoardSize in each
	// dimension.  I.e. they are each (BoardSize+1) X (BoardSize+1)

	matrix<ValueType> value; // holds the sudoku matrix
	matrix<int> rowCheck; // stores count of each number place in each row: [row][ValueType] 
	matrix<int> columnCheck; // stores count of each number place in each column: [column][ValueType]
	matrix<int> squareCheck; // stores count of each number place in each square: [square][ValueType]
	
};



