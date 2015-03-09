#include "board.h"


board::board()
{
}


board::~board()
{
}

board::board(int sqSize) : value(BoardSize + 1, BoardSize + 1)
// Board constructor
{
	rowCheck.resize(10,10);
	columnCheck.resize(10,10);
	squareCheck.resize(10,10);
}

/****
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
bool board::numberFit(const int i, const int j, const ValueType testElement)
{	
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
	{	throw rangeError("In board::numberFit: index out of range");	}
	return (value[i][j] == Blank) && rowCheck[i][testElement] && columnCheck[j][testElement] && squareCheck[squareNumber(i, j)][testElement];
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
			setCell(i, j, Blank);
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
		//in each row, iterate through all the columns and if a number is contained in that row then 
		//set the corresponding number false 
		for (int j = 1; j <= BoardSize; j++)
		{
			//if the element in the current cell is not a dot, then set the corresponding number false
			//since the numbers in the matrix are direct-addresed to its corresponding number, the number
			//5 is location 5 in the element of the matrix
			if (getCell(i, j) != Blank) { rowCheck[i][getCell(i, j)] = false;  }
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
		if(getCell(j, i) != Blank) { columnCheck[i][getCell(j, i)] = false;	}
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
			if (getCell(i, j) != Blank) { squareCheck[squareNumber(i, j)][getCell(i, j)] = false; }
		}
	}	
}
/**
* setCell- This attempts to set a particular cell to a number. If there is no conflicts, and the position is currently blank,
 then the number is added to the position. Also, this applies to the cause for making the board, because if the newNum is Blank, 
 as in intiaizing the board, it sets the position to blank. This covers all cases, where a number needs to be added to a board, 
 either through trying to place a new number to solve the board or to initalize the board to blanks.
**/
void board::setCell(const int i, const int j, const ValueType newElement)
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
	{	throw rangeError("In board::setCell: invalid index");	}
	if (newElement == Blank) // always store a blank element
	{
		value[i][j] = Blank;
	}
	else if (numberFit(i, j, newElement)) // place the new element if there aren't conflicts
	{
		value[i][j] = newElement;
		rowCheck[i][newElement] = false;
		columnCheck[j][newElement] = false;
		squareCheck[squareNumber(i, j)][newElement] = false;
	}
	else // send a passive-agressive message if there is a conflict
	{
		std::cout << "Warning! Invalid placement: " << newElement << " was not placed in position ("<<i<<","<<j<<")!" << endl;
	}
	
}

/****
* clearCell takes in a position and sets the value back to one.
This assumes that the number set in the position does not conflict with any other value
in that row, column and squrae. Therefore, the old number is set to true in all of the conflict
vectors and then the data is set back to blank.
****/
void board::clearCell(const int i, const int j)
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
	{	throw rangeError("In board::setCell: invalid index");	}
	if (value[i][j] != Blank)
	{		
		rowCheck[i][value[i][j]] = true;
		columnCheck[j][value[i][j]] = true;
		squareCheck[squareNumber(i, j)][value[i][j]] = true;
		value[i][j] = Blank;
	}
}

/****
* initialize - initailizes the board to the input file
First it clears the board to all 0's and then sets the 
positions in the data vector to the values from the
input file. 0's is read as -1's in the data vector
Also, it reads in the board from 1-9 not 0-8. After
setting the board it updates the conflict vectors to the 
starting values
****/
void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{	
	char ch;
	
	clear();
	for (int i = 1; i <= BoardSize; i++)
	for (int j = 1; j <= BoardSize; j++)
	{
		fin >> ch;

		//if the value is a dot then put a -1 in its data value
		// Blank = -1
		if (ch == '.')
		{
			setCell(i, j, Blank);
		}
		// If the read char is not Blank
		else if (ch != '.')
		{
			setCell(i, j, ch - '0');   // Convert char to int
		}
		else { throw invalid_argument("In Board::initialize(): Invalid sudoku puzzle with values outside ValueType"); }
	}	
	setRowNums();
	setColumnNums();
	setSquareNums();	
}

/****
* board solved- checks to see if the board is fully solved.
 It does that by checking the conflict vectors to see if they are all false. Because if there is a conflict
 vector with a true value still, then that means that a number can still be placed into the board and thus
 the board is not solved
****/
bool board::boardSolved()
{
	bool result = false;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			result = result || rowCheck[i][j] || squareCheck[i][j] || columnCheck[i][j];			
		}
	}	
	std::cout << (	(result) ? ("The game is not solved.") : ("The game is solved")	) << endl;
	return result;
}

/****
* SquareNumber takes in the position and determines
which square that position is in and returns the square number
****/
int board::squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
	// Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
	// coordinates of the square that i,j is in.  

	return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}


/***
* overloaded operator to output a line of the sudoku
***/
ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
	for (unsigned int i = 0; i < v.size(); i++)
		ostr << v[i] << " ";
	ostr << endl;
	return ostr;
}


/****
* getCell -it given a position and returns the current position
if the position is out of range for the board, it returns out of range
****/
ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
	{throw rangeError("In board::setCell: invalid index");}
	else 
	{ return value[i][j]; }
}

/****
* isBlank determines is a given cell is blank or not
****/
bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
	{	throw rangeError("In board::setCell: invalid index");	}
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
			std::cout << " -";
			for (int j = 1; j <= BoardSize; j++)
				std::cout << "---";
			std::cout << "-";
			std::cout << endl;
		}
		for (int j = 1; j <= BoardSize; j++)
		{
			if ((j - 1) % SquareSize == 0)
				std::cout << "|";
			if (!isBlank(i, j))
				std::cout << " " << getCell(i, j) << " ";
			else
				std::cout << "   ";
		}
		std::cout << "|";
		std::cout << endl;
	}

	std::cout << " -";
	for (int j = 1; j <= BoardSize; j++)
		std::cout << "---";
	std::cout << "-";
	std::cout << endl;
}

bool board::solveBoard(const int i, const int j)
{
	static int numIterations = 0;
	numIterations++;
	//if the number fits
	//set the cell with the number that fits
	//if that number is in position 9,9 then the board is solved
	//else recursively call this funciton to solve the next element on the board
	// with (i % 9) + 1 to allow the row to wrap for each column
	// for the return value of the call if it is true then keep the change else if false then
	// undue the change and change the conflict variable
	// Only attempt to test and place a number if the Cell is blank
	if (isBlank(i, j))
	{
		//test every possible element that could go into the Cell
		for (int l = 1; l < 10; l ++)
		{
			//if the current number fits into the cell place the value into that cell, 
			// The function setCell automatically sets the conflict variables
			// then after the new number is set, call the solveBoard function recursively
			// to set the next cell, if the value returned from the next cell is false, which
			//indicates that there are no numbers that currently work with the next Cell, then clear that cell and 
			// contiunue with placing elements at that cell from where the function left off. If the testing number gets to 9 and
			// that number does not work, then return false so that the previous cell can remove its number and try the next
			if (numberFit(i, j, l))
			{
				setCell(i, j, l);
				//system("pause");
				//print();
				cout << "Number of Iterations " << numIterations << endl;
				//cout << " i and j " << i << " " << j << endl;
				if (i == 9 && j == 9)
				{
					return true;
				}
				(solveBoard((i % 9) + 1, ((i % 9) + 1 == 1) ? j + 1 : j)) ? true : (clearCell(i, j));
				
			}
		}
		//return false indicates that there are no numbers that will satisfy the current test numbers
		return false;
	}
	// if the current cell is not blank then call solve Board on the next Cell and this cell
	// returns whatever it is given
	else
	{
		return solveBoard((i % 9) + 1, ((i % 9) + 1 == 1) ? j + 1 : j);

	}
	/*
	for (int l = 1; l < 10; l++)
	{
		
		if (numberFit(i, j, l))
		{
			cout << "Value of l " << l << endl;
			setCell(i, j, l);
			if (boardSolved())
			{
				return true;
			}
			else
			{
				(solveBoard((i % 9) + 1, j)) ? true : (clearCell(i, j), false);
			}
		}
		
	}
	// if no numbers fit then return false
	return false;
	*/
}
// checks for the number of conflicts in a partciular Cell
// all three arrays must contain that particular number as a conflict in order it to be a conflict for that Cell
int board::numConflict(const int i, const int j)
{
	int conflictNum = 0;
	for (int l = 1; l < 10; l++)
	{
		(rowCheck[i][l] && rowCheck[j][l] && squareCheck[squareNumber(i, j)][l]) ? conflictNum++ : conflictNum;
	}
	return conflictNum;
}
