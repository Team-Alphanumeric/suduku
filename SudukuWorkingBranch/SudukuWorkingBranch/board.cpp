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
	return (value[i][j] == Blank) && (rowCheck[i][testElement]==0) && 
		   (columnCheck[j][testElement]==0) && (squareCheck[squareNumber(i, j)][testElement]==0);
}

void board::clear()
{
	//loop through each element in the table and set all of the value to blank so -1.
	//Also loop through all the check matrices and clear contradiction vectors.
	for (int i = 1; i <= BoardSize; i++)
	{
		for (int j = 1; j <= BoardSize; j++)
		{
			//every possible value is set back to true because it is
			//a blank board so every number can go in every position.
			setCell(i, j, Blank);
			rowCheck[i][j] = 0;
			columnCheck[i][j] = 0;
			squareCheck[i][j] = 0;
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
	
	// check if the element should be placed in this position
	// if not, send a passive-agressive warning message
	if( (newElement!=Blank) && (!numberFit(i, j, newElement)) )
	{ std::cout << "Warning! Contradiction placing " << newElement << " at position ("<<i<<","<<j<<")!" << endl; }
	
	// always store the element
	value[i][j] = newElement;
	
	if (newElement != Blank) // modify contradiction vectors appropriately
	{
		rowCheck[i][newElement]++;
		columnCheck[j][newElement]++;
		squareCheck[squareNumber(i, j)][newElement]++;
	}
	return;	
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
	{	throw rangeError("In board::clearCell: invalid index");	}
	if (value[i][j] != Blank)
	{		
		// lower the number of contradications related to this cell's value
		rowCheck[i][value[i][j]]--; 
		columnCheck[j][value[i][j]]--;
		squareCheck[squareNumber(i, j)][value[i][j]]--;
	
		// check for sensical number of conflicts
		if( (rowCheck[i][value[i][j]]<0) || (columnCheck[j][value[i][j]]<0) || (squareCheck[squareNumber(i, j)][value[i][j]]<0) )
		{ throw expressionError("In board::clearCell: invalid number of conflicts"); }
	
		// then make the cell blank
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
	numSolveIterations = 0;
}

/****
* board solved- checks to see if the board is fully solved.
 It does that by checking the conflict vectors to see they all have exactly one conflict. 
 If otherwise, either a value can still be placed or there are multiple conflicts, which is 
 and incorrectly solved puzzle
****/
bool board::boardSolved()
{
	bool result = true;
	for (int r = 1; r < 10; r++)
	{
		for (int c = 1; c < 10; c++)
		{
			for (int k = 1; k < 10; k++)
			{
				/*cout << "Value: " << k;
				cout << "\tRow: " << rowCheck[r][k];
				cout << "\tColumn: " << columnCheck[c][k];
				cout << "\tSquare: " << squareCheck[squareNumber(r, c)][k] << endl;*/
				result = (result) && (value[r][c] != Blank) && (rowCheck[r][k] == 1) && (squareCheck[squareNumber(r,c)][k] == 1) && (columnCheck[c][k] == 1);
				//if (!result) { cout << "result false at position (" << r << "," << c << ")" << endl; system("pause"); }
			}
		}
	}	
	if (result) { std::cout << ("The game is not solved.") << endl; }
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

/*
	functional template:
	These comments outline the approach that I'm taking to tackle this problem.
	I wasn't exactly sure how you approached it, but I know you thought through 
	the specifics. These comments speak to the generalized approach for which I'm
	using the specific functions that you developed.
	Also, you may have missed the class where the professor explained this approach,
	because I'm not sure it's what you are using here.

bool board::solveBoard()
{
	// find the first empty cell in the matrix
	// test all (reasonable) values for this empty cell
		// find (reasonable) test value
		// input a test value into that cell (keep track of location)
		// verify if the board can be solved with that test value (stop if so)
	// increment count of recursive call (for this function)
	// remove test value if unsolved
	// report up
}

*/
bool board::solveBoard()

{

	// increment count of calls to this function for this puzzle
	numSolveIterations++;
	// find the best empty cell in the matrix: set to (r,c)
	// if function returns false because no acceptable blank cell is found, check if the board is solved
	// 'numberFit' should ensure only a valid number is placed
	int r, c; if (!findMaxContradiction(r, c)) 
	{ return boardSolved(); }
	// otherwise, test all (reasonable) values for this empty cell
	for (int i = 1; i <= 9; i++)
	{
		// find (reasonable) test value
		int test = i; // for now we assume any test number is "reasonable"
		// move on to next iteration in the for loop if the test number contradicts
		if (!numberFit(r, c, test)) { continue; }
		// input a test value into that cell
		setCell(r, c, test);
		// if the board can be solved with that test value, then it is solved using this year value (stop searching and return true)
		if (solveBoard()){ return true; }
		// otherwise, clear the cell to try with a different value
		clearCell(r, c);
	}
	//cout << "Out of options " << endl;
	// if still unsolved report unsolvable from here
	return false;
}


/*
// Solves the Board using a brute force method of testing values
bool board::solveBoard()
{
	// increment count of calls to this function for this puzzle
	numSolveIterations++;
	cout << "Num of iterations " << numSolveIterations << ".\n";
	if (numSolveIterations > 250000)
	{
		print();
		system("pause");
	}
	
	
	// find the first empty cell in the matrix: set to (r,c)
	// if no empty cells are found, double check if the board is solved
	// 'boardSolved' here should actually always return true because 
	// 'numberFit' should ensure noly a valid number is placed
	int r; int c; if(!findEmpty(r,c)) {return boardSolved(); } 
	
	// otherwise, test all (reasonable) values for this empty cell
	for(int i=1; i<=9; i++)
	{
		// find (reasonable) test value
		int test = i; // for now we assume any test number is "reasonable"
		
		// move on to next iteration in the for loop if the test number conflicts
		if(!numberFit(r,c,test)) { continue; }
		
		// input a test value into that cell
		setCell(r,c,test);
		
		// if the board can be solved with that test value, then it is solved (stop searching and return true)
		if(solveBoard()){ return true; }
		
		// otherwise, clear the cell to try with a differnet value
		clearCell(r,c);
	}
	// if still unsolved report unsolvable from here
	return false;
}

// sets the value of the inputs to the indices of the first empty cell
// and returns whether it has found an empty cell
bool board::findEmpty(int &i, int &j)
{
	// search through all rows and columns
	for(int r=1; r<=9; r++)
	{
		for(int c=1; c<=9; c++)
		{
			// if blank, set indices to value and quit function
			if(getCell(r,c)==Blank) {i=r; j=c; return true;}
		}
	}
	// base case: no blank cell found
	return false;
}*/

// sets the value of the inputs to the indices of the first empty cell with the most contradictions

// and returns true if has found a maximum contradiction cell
// returns false if a blank cell is a total contradiction (none of the value are valid) or if there are no blank cells

bool board::findMaxContradiction(int &i, int &j)
{	// initialize i and j to default values
	i = 0; j = 0;
	int cmax = 0; // maximum contradictions for the board
	int cntd = 0; // contradictions for a blank cell
	// search through all rows and columns
	for (int r = 1; r <= 9; r++)
	{
		for (int c = 1; c <= 9; c++)
		{
			cntd = 0;
			// if blank, find the number of contradictions
			if (getCell(r, c) == Blank)
			{
				// find the number of contradictions
				for (int k = 1; k <= 9; k++)
				{
					cntd += (rowCheck[r][k] || columnCheck[c][k] || squareCheck[squareNumber(r, c)][k]);					
				}				
				// if this is a higher number of contradictions, use this position, and set the max to this number of contradictions
				if (cntd>cmax) {
					i = r; j = c; cmax = cntd; }
				switch (cntd)
				{
				case 8: return true; break; // return cases of a single option remaining immediately
				case 9: return false; break; // return cases of a total contradiction as no acceptable blank cell found
				default: break; // all other numbers are acceptable counts that need not disturb the function
				}
			} // if get cell == blank
		} // for columns
	} // for rows
	// after iterating through all of the row and columns, return whether an acceptable cell was found (if so, i & j are both nonzero because they are uninitialized)
	return (i || j);
}
