#include <stdio.h>
#include <stdbool.h>

bool initBoard(char board[][26], int *pSize, char *pCompColour); // Prompts user and initializes board and computer's colour
void printBoard(char board[][26], int n); // Prints board of size n in its current state
bool positionInBounds(int n, char row, char col); // Determines if the given position is within variable board size n
bool isValidMove(char board[][26], int n, char row, char col, char colour); // Checks if a player's move is legal
bool arePotentialMoves(char board[][26], int n, char colour); // Checks if there is an available move for a colour.
bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol); // Check if a given placement of a colour is legal in an arbitrary direction
char greaterColour(char board[][26], int n); // Checks which colour occurs more often on the board.
void moveCalculator(char board[][26], int n, char *pRow, char *pCol, char colour); // Calculates the optimal position for the computer to move
int checkValidAndFlip(char board[][26], int row, int col, char colour, int n, bool flip); // Calculates the number of tiles that can be flipped

int main(int argc, char **argv)
{
	char cPlay, cComp, row, col; // Colour of player, colour of computer; row and column values.
	bool isPlayerMove;
	int n;
	
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
	
	while( arePotentialMoves(board, n, cPlay) || arePotentialMoves(board, n, cComp) ){
		
		if(isPlayerMove){
			if(arePotentialMoves(board, n, cPlay)){
				printf("Enter a move for colour %c (RowCol): ", cPlay);
				scanf(" %c%c", &row, &col); // User inputs where they'd like to move
				
				// Check if the number of tiles flipped is 0 (Implies invalid move). Otherwise it flips tiles.
				if(checkValidAndFlip(board, row, col, cPlay, n, true) == 0){
					printf("Invalid move.\n%c player wins.\n", cComp);
					return 0;
				}
				
			}else{
				printf("%c player has no move.\n", cPlay);
			}
		}else{
			if(arePotentialMoves(board, n, cComp)){
				// Put in computer calculations
				moveCalculator(board, n, &row, &col, cComp);
				checkValidAndFlip(board, row, col, cComp, n, true);
				
				printf("Computer places %c at %c%c.\n", cComp, row, col);
			}else{
				printf("%c player has no move.\n", cComp);
			}
		}
		
		printBoard(board, n);
		isPlayerMove = !isPlayerMove;
	}
	
	char winner = greaterColour(board, n);
	if(winner == 'D')
		printf("Draw!\n");
	else
		printf("%c player wins.\n", winner);
	// End Program
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
	
	char row, col;
	
	// Initialize board to default starting setup.
	
	for(row = 'a'; row < 'a'+ *pSize; row++)
	{
		for(col = 'a'; col < 'a'+ *pSize; col++)
		{
			// Conditions set the 2x2 checker pattern at the center of the board
			if((row - 'a' == *pSize/2 && col - 'a' == *pSize/2) || (row - 'a' == *pSize/2 - 1 && col - 'a' == *pSize/2 - 1))
				board[row-'a'][col-'a'] = 'W';
			else if((row-'a' == *pSize/2 && col-'a' == *pSize/2 - 1) || (row-'a' == *pSize/2 - 1 && col-'a' == *pSize/2))
				board[row-'a'][col-'a'] = 'B';
			else
				board[row-'a'][col-'a'] = '.'; // All other spaces are "unnoccupied"
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
 * @brief Returns true if the user given colour, row, and col produce a legal move on the board of size n. Returns false otherwise.
 * @param board
 * @param n
 * @param row
 * @param col
 * @param colour
 * @return 
 */
bool isValidMove(char board[][26], int n, char row, char col, char colour)
{
	int deltaRow, deltaCol;
	
	if( (colour == 'W' || colour == 'B') && positionInBounds(n, row, col) && board[row-'a'][col-'a'] == '.'){
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
	char row, col;
	for(row = 'a'; row < 'a'+n; row++){
		for(col = 'a'; col < 'a'+n; col++){
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
		else if(board[row-'a'+dRow][col-'a'+dCol] == '.' && !endColour) // return false if there is a space before the line is terminated
			return false;
		else if(colour != board[row-'a'+dRow][col-'a'+dCol] && !endColour && board[row-'a'+dRow][col-'a'+dCol] != '.') //If next tile is opposing colour and line has not been terminated yet
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
	char row, col;
	int difference = 0; // Termining difference between num of Ws and num of Bs (Positive favors W)
	for(row = 'a'; row < 'a'+n; row++){
		for(col = 'a'; col < 'a'+n; col++){
			if(board[row-'a'][col-'a'] == 'W')
				difference++;
			else if(board[row-'a'][col-'a'] == 'B')
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
void moveCalculator(char board[][26], int n, char *pRow, char *pCol, char colour){
	int highScore = 0;
	int tempScore;
	
	char row,col;
	for(row='a'; row< 'a'+n; row++){
		for(col='a'; col< 'a'+n; col++){
			tempScore = checkValidAndFlip(board, row, col, colour, n, false);
			if(tempScore > highScore){
				*pRow = row;
				*pCol = col;
				highScore = tempScore;
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
			board[row-'a'][col-'a'] = colour;
		
		int deltaRow, deltaCol, dRow, dCol;
		// Same loops for actually flipping tiles in main().
		for(deltaRow = -1; deltaRow <= 1; deltaRow++){
			for(deltaCol = -1; deltaCol <= 1; deltaCol++){
				if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)){
					for(dRow = deltaRow, dCol = deltaCol; board[row-'a'+dRow][col-'a'+dCol] != colour; dRow+=deltaRow, dCol+=deltaCol ){
							numOfFlippableTiles++; // Increment by 1 each time a tile is "turned"
							if(flip)
								board[row-'a'+dRow][col-'a'+dCol] = colour;
					}
				}
			}
		}
	}
		
	return numOfFlippableTiles;
}
