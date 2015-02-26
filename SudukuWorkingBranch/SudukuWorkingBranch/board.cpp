#include "board.h"


board::board()
{
}


board::~board()
{
}

board::board(int sqSize)
: value(BoardSize + 1, BoardSize + 1)
// Board constructor
{
}

void board::clear()
{
}
/*
* setRowNums - set the available numbers for each row, should be a Matrix of size 9 by 9
with a boolean indicating whether a number is in that row or not. For example, rowNum[2][5]
would return true if their is not a 5 in row 5 of the suduku, or false if there was a 5
in row 5 of the suduku
*/
void board::setRowNums()
{
	//for each row starting from 1 and going to 9 iterate through each row of the rowCheck matrix
	for (int i = 0; i < BoardSize; i++)
	{
		//in each row, iterate through all the cells and if a number is contained in that row then 
		//set the corresponding number false 
		for (int j = 0; j < BoardSize; j++)
		{
			//if the element in the current cell is not a dot, then set the corresponding number false
			//since the numbers in the matrix are direct-addresed to its corresponding number, the number
			//5 is location 5 in the element of the matrix
			(getCell(i, j) != '.') ? (rowCheck[i][getCell(i, j)] = false) : false;
		}
	}
}
/*
 *setColumnNUms does the exact same function as setRowNums except it will be setting the numbers that are
 available for each column
*/
void board::setColumnNums()
{
	//for each column starting from 1 and going to BoardSize iterate through each column of the column Checks matrix
	for (int i = 0; i < BoardSize; i++)
	{
		//in each Column, iterate through all the cells and if a number is contained in that column then 
		//set the corresponding number false 
		for (int j = 0; j < BoardSize; j++)
		{
			//if the element in the current cell is not a dot, then set the corresponding number false
			//since the numbers in the matrix are direct-addresed to its corresponding number, the number
			//5 is location 5 in the element of the matrix
			(getCell(j, i) != '.') ? (columnCheck[i][getCell(j, i)] = false) : false;
		}
	}
}
/*
* setSquareNums does the same thing as setRowNums except it sets which numbers are still available for each square
*/
void board::setSquareNums()
{

}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
	char ch;

	clear();
	for (int i = 1; i <= BoardSize; i++)
	for (int j = 1; j <= BoardSize; j++)
	{
		fin >> ch;

		// If the read char is not Blank
		if (ch != '.')
		{
			setCell(i, j, ch - '0');   // Convert char to int
		}
	}
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
	// Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
	// coordinates of the square that i,j is in.  

	return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
	for (int i = 0; i < v.size(); i++)
		ostr << v[i] << " ";
	ostr << endl;
	return ostr;
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
		return value[i][j];
	else
		throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
		throw rangeError("bad value in setCell");
}

void board::print()
// Prints the current board.
{
	for (int i = 1; i <= BoardSize; i++)
	{
		if ((i - 1) % SquareSize == 0)
		{
			cout << " -";
			for (int j = 1; j <= BoardSize; j++)
				cout << "---";
			cout << "-";
			cout << endl;
		}
		for (int j = 1; j <= BoardSize; j++)
		{
			if ((j - 1) % SquareSize == 0)
				cout << "|";
			if (!isBlank(i, j))
				cout << " " << getCell(i, j) << " ";
			else
				cout << "   ";
		}
		cout << "|";
		cout << endl;
	}

	cout << " -";
	for (int j = 1; j <= BoardSize; j++)
		cout << "---";
	cout << "-";
	cout << endl;
}

