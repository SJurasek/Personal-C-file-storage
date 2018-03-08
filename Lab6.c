#include <stdio.h>
#include <stdbool.h>

void printBoard(char board[][26], int n); // Prints board of size n in its current state
void printPotentialMoves(char board[][26], int n, char colour); // Prints each position that a colour can go that is legal
bool positionInBounds(int n, char row, char col); // Determines if the given position is within variable board size n

// Check if a given placement of a colour is legal in an arbitrary direction
bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol);

int main(int argc, char **argv)
{
//	char playerColour;
	int n;
	printf("Enter the board dimension: ");
	scanf("%d", &n); // Assigns board dimension to n
	
	if(n < 4 || n > 26 || n % 2 != 0) // Just making sure the board size is even and within bound
	{
		printf("Invalid board size.\n");
		return 0;
	}
	static char board[26][26];
	
	char row;
	char col;
	
	// Initialize board to default starting setup.
	
	for(row = 'a'; row < 'a'+ n; row++)
	{
		for(col = 'a'; col < 'a'+ n; col++)
		{
			// Conditions set the 2x2 checker pattern at the center of the board
			if((row - 'a' == n/2 && col - 'a' == n/2) || (row - 'a' == n/2 - 1 && col - 'a' == n/2 - 1))
				board[row-'a'][col-'a'] = 'W';
			else if((row-'a' == n/2 && col-'a' == n/2 - 1) || (row-'a' == n/2 - 1 && col-'a' == n/2))
				board[row-'a'][col-'a'] = 'B';
			else
				board[row-'a'][col-'a'] = 'U'; // All other spaces are "unnoccupied"
		}
	}
	printBoard(board, n);
	
	printf("Enter board configuration:\n");
	row = 'a';
	col = 'a';
	char colour;
	
	// Keep repeating the same
	do{
		scanf(" %c%c%c", &colour, &row, &col); // <colour><row><col>
		if(positionInBounds(n, row, col) && (colour == 'W' || colour == 'B')) // Check if its an actual placement just in case
			board[row-'a'][col-'a'] = colour;
	} while(row != '!' && col != '!' && colour != '!'); // Keep going until user inputs !!!
	printBoard(board, n);
	
	// Print out all possible moves in the current setup for both colours. White first.
	printPotentialMoves(board, n, 'W');
	printPotentialMoves(board, n, 'B');
	
	int deltaRow, deltaCol; // Defines direction of line
	int dRow, dCol; // Total change in position from desired placement of tile (Sorry these are bad names)
	bool validMove = false;
	
	printf("Enter a move:\n");
	scanf(" %c%c%c", &colour, &row, &col); // User inputs where they'd like to move
	// Make sure it is a possible move. Otherwise invalid move.
	if(positionInBounds(n, row, col) && (colour == 'W' || colour == 'B') && board[row-'a'][col-'a'] == 'U'){ // If row,col is not a position on the board it wont reach the last condition.
		for(deltaRow = -1; deltaRow <= 1; deltaRow++)
		{
			for(deltaCol = -1; deltaCol <= 1; deltaCol++) 	// Nested for loops loop through each of the 8 directions that tiles may lay in.
			{												// When deltaRow and deltaCol both equal 0, checkLegalInDirection will return false.
				if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol))
				{
					// If there is a valid line, for loop flips all consequent tiles up to the position where the colour on the board is the same as the given colour
					for(dRow = 0, dCol = 0; board[row-'a'+dRow][col-'a'+dCol] != colour; dRow+=deltaRow, dCol+=deltaCol )
					{
						board[row-'a'+dRow][col-'a'+dCol] = colour; // Changes chosen tile and concurrent line of opposing colour
					}
					validMove = true; // If tiles were flipped, a validMove was made
				}
			}
		}
	}
	
	if(validMove) // Self explanatory
		printf("Valid move.\n");
	else
		printf("Invalid move.\n");
	
	printBoard(board, n);
	// End Program
	return 0;
}

/**
 * @brief Prints out any and all possible moves for a certain colour (i.e. Places on the board where tiles will be flipped in any direction).
 * @param board
 * @param n
 * @param colour
 */
void printPotentialMoves(char board[][26], int n, char colour)
{
	int row;
	int col;
	printf("Available moves for %c:\n", colour);
	for(row='a'; row < 'a'+n; row++)
	{
		for(col='a'; col<'a'+n; col++) // Nested for loop goes through every position on the board. Starts at earliest row, goes through each column starting at 'a', then goes to next row.
		{
			if( board[row-'a'][col-'a'] == 'U' /* Is the position unoccupied? */
			&& (checkLegalInDirection(board, n, row, col, colour, -1, -1) || checkLegalInDirection(board, n, row, col, colour, 0, -1)
			|| checkLegalInDirection(board, n, row, col, colour, 1, -1) || checkLegalInDirection(board, n, row, col, colour, -1, 0)
			|| checkLegalInDirection(board, n, row, col, colour, 1, 0) || checkLegalInDirection(board, n, row, col, colour, -1, 1)
			|| checkLegalInDirection(board, n, row, col, colour, 0, 1) || checkLegalInDirection(board, n, row, col, colour, 1, 1) )) // Hardcode if theres a legal move for any direction.
				printf("%c%c\n", row, col);
		}
	}
}

/**
 * @brief Prints out an n by n 2D array (board) of given values with alphabetical-notated columns and rows.
 * @param board
 * @param n
 */
void printBoard(char board[][26], int n)
{
	char row;
	char col;
	printf("  "); // Whitespace
	for(col='a'; col<'a'+n; col++)
	{
		printf("%c", col); // Print out column identifiers
	}
	printf("\n");
	
	for(row='a'; row<'a'+n; row++) //Print rest of board
	{
		printf("%c ", row); // Print current row header
		for(col='a'; col<'a'+n; col++)
		{
			printf("%c", board[row-'a'][col-'a']); // Print out the contents of the current row of the board
		}
		printf("\n"); // Next line
	}
}

/**
 * @brief Returns true if the given position (row, col) lies within a board of n by n dimensions. If not returns false.
 * @param n
 * @param row
 * @param col
 * @return 
 */
bool positionInBounds(int n, char row, char col)
{
	// Make sure given coordinate is >= a and less than 'a'+n
	return ( row - 'a' >= 0 && row - 'a' < n ) && ( col - 'a' >= 0 && col - 'a' < n );
}

/**
 * @brief Returns true if the placement of a tile of colour 'colour' is legal given arbitrary directions within the rules of Reversi.
 * @param board
 * @param n
 * @param row
 * @param col
 * @param colour
 * @param deltaRow
 * @param deltaCol
 * @return 
 */
bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol)
{
	int dRow, dCol; // Total spaces travelled in a direction on the board. Sum of total deltaRows/deltaCols
	bool flippableColours = false; // Are there opposing colours present?
	bool endColour = false; // Is there an terminating tile of the same colour present?
	
	// For loop makes sure each increment in a direction is within bounds and increments by one tile in the specified direction each time.
	// Stops if the scanner goes out of bounds or an end tile of similar colour is identified (End of line)
	for(dRow = deltaRow, dCol = deltaCol; positionInBounds(n, row+dRow, col+dCol) && !endColour; dRow += deltaRow, dCol += deltaCol)
	{
		if(colour == board[row-'a'+dRow][col-'a'+dCol]) //If the next tile is of similar colour
			endColour = true;
		else if(board[row-'a'+dRow][col-'a'+dCol] == 'U' && !endColour) // return false if there is a space before the line is terminated
			return false;
		else if(colour != board[row-'a'+dRow][col-'a'+dCol] && !endColour && board[row-'a'+dRow][col-'a'+dCol] != 'U') //If next tile is opposing colour and line has not been terminated yet
			flippableColours = true;
	}
	
	return flippableColours && endColour; // If there isnt any opposing tiles found in a line or no terminating tile of same colour this will return false. 
}
