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
	rowCheck.resize(10,10);
	columnCheck.resize(10,10);
	squareCheck.resize(10,10);
	value.resize(10, 10); 
}
/***
* printConflicts prints out the vectors out the three vectors which indicate which
* numbers are still available in each vector
****/
void board::printConflicts()
{
	cout << "The possible numbers for each row " << endl;
	for (int i = 1; i <= BoardSize; i++)
	{
		cout << " Row " << i << ": ";
		for (int j = 1; j <= BoardSize; j++)
		{
			if (rowCheck[i][j])
			{
				cout << j << " ";
			}
		}
		cout << endl;
	}
	cout << "The possible numbers for each column " << endl;
	for (int i = 1; i <= BoardSize; i++)
	{
		cout << " Column " << i << ": ";
		for (int j = 1; j <= BoardSize; j++)
		{
			if (columnCheck[i][j])
			{
				cout << j << " ";
			}
		}
		cout << endl;
	}
	cout << "The possible numbers for each square " << endl;
	for (int i = 1; i <= BoardSize; i++)
	{
		cout << " Square " << i << ": ";
		for (int j = 1; j <= BoardSize; j++)
		{
			if (squareCheck[i][j])
			{
				cout << j << " ";
			}
		}
		cout << endl;
	}
}
/***
*  takes in a position and a number and sees if there is a conflict at that position
***/
bool board::numberFit(int i, int j, int testNum)
{
	return rowCheck[i][testNum] && columnCheck[j][testNum] && squareCheck[squareNumber(i, j)][testNum];
}

void board::clear()
{
	//loop through each element in the table and set all of the value to blank so -1.
	//Also loop through all the check matrices and set all the possible values to true.
	for (int i = 1; i <= BoardSize; i++)
	{
		for (int j = 1; j <= BoardSize; j++)
		{
			//every possible value is set back to true because it is
			//a blank board so every number can go in every position.
			setCellInit(i, j, Blank);
			rowCheck[i][j] = true;
			columnCheck[i][j] = true;
			squareCheck[i][j] = true;
		}
	}
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
	for (int i = 1; i <= BoardSize; i++)
	{
		//in each row, iterate through all the cells and if a number is contained in that row then 
		//set the corresponding number false 
		for (int j = 1; j <= BoardSize; j++)
		{
			//if the element in the current cell is not a dot, then set the corresponding number false
			//since the numbers in the matrix are direct-addresed to its corresponding number, the number
			//5 is location 5 in the element of the matrix
			(getCell(i, j) != Blank) ? (rowCheck[i][getCell(i, j)] = false) : true;
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
	for (int i = 1; i <= BoardSize; i++)
	{
		//in each Column, iterate through all the cells and if a number is contained in that column then 
		//set the corresponding number false 
		for (int j = 1; j <= BoardSize; j++)
		{
			//if the element in the current cell is not a dot, then set the corresponding number false
			//since the numbers in the matrix are direct-addresed to its corresponding number, the number
			//5 is location 5 in the element of the matrix
			(getCell(j, i) != Blank) ? (columnCheck[i][getCell(j, i)] = false) : false;
		}
	}	
}
/*
* setSquareNums does the same thing as setRowNums except it sets which numbers are still available for each square
*/
void board::setSquareNums()
{
	// iterate through every possible element starting from row 1 and iterating through each element in the row and then
	// each column
	for (int i = 1; i <= BoardSize; i++)
	{
		for (int j = 1; j <= BoardSize; j++)
		{
			// if the location that is currently being checked is not a dot, then find out which square it is in
			// with the squareNumber function and set that corresponding value to false. This will indicate
			// which numbers are still currently available in each square.
			(getCell(i, j) != Blank) ? (squareCheck[squareNumber(i,j)][getCell(i, j)] = false) : false;
		}
	}	
}

void board::setCellInit(const int i, const int j, const int newNum)
{
	value[i][j] = newNum;
}

void board::setCell(const int i, const int j, const int newNum)
{
	cout << "Testing position " << i << "," << j << " and for " << newNum << ".\n";
	if (numberFit(i, j, newNum))
	{
		cout << "Num does not have a conflict " << endl;
		cout << "Adding " << newNum << " to the board at position " << i << "," << j << ".\n";
		value[i][j] = newNum;
		rowCheck[i][newNum] = false;
		columnCheck[j][newNum] = false;
		squareCheck[squareNumber(i, j)][newNum] = false;
		cout << "Square number " << squareNumber(i,j) << endl;
		printConflicts();
	}
	else
	{
		cout << "Num has a conflict" << endl;
	}
	
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
			setCellInit(i, j, ch - '0');   // Convert char to int
		}
		//if the value is a dot then put a -1 in its data value
		// Blank = -1
		else
		{
			setCellInit(i, j, Blank);
		}
	}	
	setRowNums();
	setColumnNums();
	setSquareNums();	
}

int board::squareNumber(int i, int j)
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
	for (unsigned int i = 0; i < v.size(); i++)
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
	//since a value of -1 in the data value means blank, when
	//the cell returns a value of -1, then the function returns true sicne
	//it is blank, if the cell does not equal -1, then it returns false
	//because then it is not empty
	return (getCell(i, j) == Blank);
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

