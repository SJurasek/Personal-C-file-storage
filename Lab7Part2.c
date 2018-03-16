#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "lab7part2lib.h"

bool initBoard(char board[][26], int *pSize, char *pCompColour); // Prompts user and initializes board and computer's colour
char findOpposite(char colour); // Finds opposite player colour
void copyBoard(char initialBoard[][26], char boardCopy[][26], int n); // Copys a board of size n to another 2d array, maintaining size n.
void printBoard(char board[][26], int n); // Prints board of size n in its current state
bool positionInBounds(int n, int row, int col); // Determines if the given position is within variable board size n
bool isValidMove(char board[][26], int n, int row, int col, char colour); // Checks if a player's move is legal
bool arePotentialMoves(char board[][26], int n, char colour); // Checks if there is an available move for a colour.
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol); // Check if a given placement of a colour is legal in an arbitrary direction
char greaterColour(char board[][26], int n); // Checks which colour occurs more often on the board.
void moveCalculator(char board[][26], int n, int *pRow, int *pCol, char colour, int turnDepth); // Calculates the optimal position for the computer to move
int checkValidAndFlip(char board[][26], int row, int col, char colour, int n, bool flip); // Calculates the number of tiles that can be flipped
int scoreCalculator(char board[][26], char boardCopy[][26], int n, int row, int col, char colour, int turnDepth); // Calculates score for a tile
int availableMoves(char board[][26], int n, char colour); // Calculates number of available moves 

int main(int argc, char **argv)
{
	char cPlay, cComp; // Colour of player, colour of computer
	bool isPlayerMove;
	int n, row, col; // board size, row and column values.
	
	static char board[26][26];
	if(!initBoard(board, &n, &cComp)){
		return 0;
	}
	
	// Set up who is what colour, and that whoever is 'B' starts first.
	if(cComp == 'W'){
		cPlay = 'B';
		isPlayerMove = true;
	}else{ // cComp == 'B'
		cPlay = 'W';
		isPlayerMove = false;
	}
	
	printBoard(board, n);
	
	struct rusage usage; // a structure to hold "resource usage" (including time)
	struct timeval start, end; // will hold the start and end times
	getrusage(RUSAGE_SELF, &usage);
	start = usage.ru_utime;
	double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds
	
	bool playerPotential = true;
	bool computerPotential = true;
	
	while( playerPotential || computerPotential ){
		
		if(isPlayerMove){
			if(playerPotential){
				// Put in smart move stuff
				findSmarterMove(board, n, cPlay, &row, &col);
				checkValidAndFlip(board, row, col, cPlay, n, true);
				
				printf("Testing AI move (row, col): %c%c\n", row + 'a', col + 'a');
				
				printBoard(board, n);
			}else{
				printf("%c player has no valid move.\n", cPlay);
			}
		}else{
			if(computerPotential){
				// Put in computer calculations
				moveCalculator(board, n, &row, &col, cComp, 10);
				
				printf("Computer places %c at %c%c.\n", cComp, row + 'a', col + 'a');
				
				if(checkValidAndFlip(board, row, col, cComp, n, true) == 0){
					printf("Invalid move.\n%c player wins.\n", cPlay);
					return 0;
				}
				
				printBoard(board, n);
			}else{
				printf("%c player has no valid move.\n", cComp);
			}
		}
		
		isPlayerMove = !isPlayerMove;
		
		playerPotential = arePotentialMoves(board, n, cPlay);
		computerPotential = arePotentialMoves(board, n, cComp);
	}
	
	char winner = greaterColour(board, n);
	if(winner == 'D')
		printf("Draw!\n");
	else
		printf("%c player wins.\n", winner);
	// End Program
	
	getrusage(RUSAGE_SELF, &usage);
	end = usage.ru_utime;
	double timeEnd = end.tv_sec + end.tv_usec / 1000000.0; // in seconds
	double totalTime = timeEnd - timeStart;
	// totalTime now holds the time (in seconds) it takes to run your code
	printf("Total time: %lf\n", totalTime);
	
	return 0;
}

/**
 * @brief Returns an initialized board and board size. Returns false if user input is invalid, true if otherwise.
 * @param board
 * @param pSize
 */
bool initBoard(char board[][26], int *pSize, char *pCompColour){
	printf("Enter the board dimension: ");
	scanf("%d", pSize); // Assigns board dimension to n
	
	if(*pSize < 4 || *pSize > 26 || *pSize % 2 != 0){ // Just making sure the board size is even and within bound
		printf("Invalid board size.\n");
		return false;
	}
	
	int row, col;
	
	// Initialize board to default starting setup.
	
	for(row = 0; row < *pSize; row++)
	{
		for(col = 0; col < *pSize; col++)
		{
			// Conditions set the 2x2 checker pattern at the center of the board
			if((row == *pSize/2 && col == *pSize/2) || (row == *pSize/2 - 1 && col == *pSize/2 - 1))
				board[row][col] = 'W';
			else if((row == *pSize/2 && col == *pSize/2 - 1) || (row == *pSize/2 - 1 && col == *pSize/2))
				board[row][col] = 'B';
			else
				board[row][col] = 'U'; // All other spaces are "unnoccupied"
		}
	}
	
	printf("Computer plays (B/W): ");
	scanf(" %c", pCompColour);
	
	if(*pCompColour != 'B' && *pCompColour != 'W'){
		printf("Invalid player colour.\n");
		return false;
	}else{
		return true;
	}
}

char findOpposite(char colour){
	if(colour == 'B')
		return 'W';
	else
		return 'B';
}

/**
 * @brief Copies values of initialBoard to boardCopy
 * @param initialBoard
 * @param boardCopy
 * @param n
 */
void copyBoard(char initialBoard[][26], char boardCopy[][26], int n){
	int row,col;
	for(row=0; row<n; row++){
		for(col=0; col<n; col++){
			boardCopy[row][col] = initialBoard[row][col];
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
	int row, col;
	printf("  "); // Whitespace
	for(col=0; col<n; col++)
	{
		printf("%c", col + 'a'); // Print out column identifiers
	}
	printf("\n");
	
	for(row=0; row<n; row++) //Print rest of board
	{
		printf("%c ", row + 'a'); // Print current row header
		for(col=0; col<n; col++)
		{
			printf("%c", board[row][col]); // Print out the contents of the current row of the board
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
bool positionInBounds(int n, int row, int col)
{
	// Make sure given coordinate is >= a and less than 'a'+n
	return ( row >= 0 && row < n ) && ( col >= 0 && col < n );
}

/**
 * @brief Returns true if the user given colour, row, and col produce a legal move on the board of size n. Returns false otherwise.
 * @param board
 * @param n
 * @param row
 * @param col
 * @param colour
 * @return 
 */
bool isValidMove(char board[][26], int n, int row, int col, char colour)
{
	int deltaRow, deltaCol;
	
	if( (colour == 'W' || colour == 'B') && positionInBounds(n, row, col) && board[row][col] == 'U'){
		for(deltaRow = -1; deltaRow <= 1; deltaRow++){
			for(deltaCol = -1; deltaCol <= 1; deltaCol++){
				if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol))
					return true;
			}
		}
	}
	return false;
}

/**
 * @brief Returns true if the given colour has available moves on the board. Returns false if not.
 * @param board
 * @param n
 * @param colour
 * @return 
 */
bool arePotentialMoves(char board[][26], int n, char colour)
{
	int row, col;
	for(row = 0; row < n; row++){
		for(col = 0; col < n; col++){
			if(isValidMove(board, n, row, col, colour))
				return true;
		}
	}
	return false;
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
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol)
{
	int dRow, dCol; // Total spaces travelled in a direction on the board. Sum of total deltaRows/deltaCols
	bool flippableColours = false; // Are there opposing colours present?
	bool endColour = false; // Is there an terminating tile of the same colour present?
	
	// For loop makes sure each increment in a direction is within bounds and increments by one tile in the specified direction each time.
	// Stops if the scanner goes out of bounds or an end tile of similar colour is identified (End of line)
	for(dRow = deltaRow, dCol = deltaCol; positionInBounds(n, row+dRow, col+dCol) && !endColour; dRow += deltaRow, dCol += deltaCol)
	{
		if(colour == board[row+dRow][col+dCol]) //If the next tile is of similar colour
			endColour = true;
		else if(board[row+dRow][col+dCol] == 'U' && !endColour) // return false if there is a space before the line is terminated
			return false;
		else if(colour != board[row+dRow][col+dCol] && !endColour && board[row+dRow][col+dCol] != 'U') //If next tile is opposing colour and line has not been terminated yet
			flippableColours = true;
	}
	
	return flippableColours && endColour; // If there isnt any opposing tiles found in a line or no terminating tile of same colour this will return false. 
}

/**
 * @brief Returns 'B' or 'W' depending on which one occurs more on the board. Returns 'D' if there is a tie.
 * @param board
 * @param n
 * @return 
 */
char greaterColour(char board[][26], int n)
{
	int row, col;
	int difference = 0; // Termining difference between num of Ws and num of Bs (Positive favors W)
	for(row = 0; row < n; row++){
		for(col = 0; col < n; col++){
			if(board[row][col] == 'W')
				difference++;
			else if(board[row][col] == 'B')
				difference--;
		}
	}
	// Use the difference to determine which colour appears more often
	if(difference > 0)
		return 'W';
	else if(difference < 0)
		return 'B';
	else
		return 'D';
}

/**
 * @brief Returns the desired row and col for the colour to move.
 * @param board
 * @param n
 * @param pRow
 * @param pCol
 * @param colour
 */
void moveCalculator(char board[][26], int n, int *pRow, int *pCol, char colour, int turnDepth){
	int highScore = 0;
	int tempScore;
	// Loop only to go through valid moves
	char boardCopy[26][26];
	
	int row,col;
	for(row=0; row< n; row++){
		for(col=0; col< n; col++){
			if(isValidMove(board, n, row, col, colour)){
				tempScore = scoreCalculator(board, boardCopy, n, row, col, colour, turnDepth);
				if(tempScore > highScore){
					*pRow = row;
					*pCol = col;
					highScore = tempScore;
				}
			}
		}
	}
}

/**
 * @brief Determines if a move is valid and returns the number of tiles flipped to a colour if its a valid move. Flips the tiles if flip is true.
 * @param board
 * @param row
 * @param col
 * @param colour
 * @param n
 * @param flip
 * @return 
 */
int checkValidAndFlip(char board[][26], int row, int col, char colour, int n, bool flip){
	int numOfFlippableTiles = 0; // Tile counter
	
	if(isValidMove(board, n, row, col, colour)){
		if(flip)
			board[row][col] = colour;
		
		int deltaRow, deltaCol, dRow, dCol;
		// Same loops for actually flipping tiles in main().
		for(deltaRow = -1; deltaRow <= 1; deltaRow++){
			for(deltaCol = -1; deltaCol <= 1; deltaCol++){
				if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)){
					for(dRow = deltaRow, dCol = deltaCol; board[row+dRow][col+dCol] != colour; dRow+=deltaRow, dCol+=deltaCol ){
							numOfFlippableTiles++; // Increment by 1 each time a tile is "turned"
							if(flip)
								board[row+dRow][col+dCol] = colour;
					}
				}
			}
		}
	}
		
	return numOfFlippableTiles;
}

int scoreCalculator(char board[][26], char boardCopy[][26], int n, int row, int col, char colour, int turnDepth){
	// Factors that affect score:
	// Number of flips for a move
	// Is the move on a corner/wall
	// Number of available moves opponent has
	// Anticipating where opponent will play
	// Plan multiple moves into the future
	copyBoard(board, boardCopy, n);
	if(checkValidAndFlip(boardCopy, row, col, colour, n, true) == 0)
		return 0;

	int changeInMoves = ;

	int score = 2 * checkValidAndFlip(board, row, col, colour, n, false);
	score += 1000 * ( (row == 0 || row == n-1) && (col == 0 || col == n-1));
	score += 2000 * ( availableMoves(boardCopy, n, findOpposite(colour)) == 0 );
	score += 10 * (row==0 || row==n-1 || col==0 || row==n-1);
	score += 5 * (availableMoves(boardCopy, n, findOpposite(colour)) - availableMoves(board, n, findOpposite(colour)));
	
	if(score < 0)
		score = 1;
	// Relatively simple computer program. Needs testing. THIS IS NOT MINIMAX
	
	return score;
}

/**
 * @brief Returns number of available moves for a certain colour
 * @param board
 * @param n
 * @param colour
 * @return 
 */
int availableMoves(char board[][26], int n, char colour){
	int row,col;
	int numOfMoves = 0;
	
	for(row=0; row<n; row++){
		for(col=0; col<n; col++){
			if(isValidMove(board, n, row, col, colour))
				numOfMoves++;
		}
	}
	return numOfMoves;
}
