#include <stdio.h>
#include <stdbool.h>

void printBoard(char board[][26], int n);
void printPotentialMoves(char board[][26], int n, char colour);
bool positionInBounds(int n, char row, char col);
bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol);

int main(int argc, char **argv)
{
//	char playerColour;
	int n;
	printf("Enter the board dimension: ");
	scanf("%d", &n);
	if(n % 2 != 0) return 0;
//	printf("Computer plays (B/W): ");
//	scanf(" %c", &playerColour);
	
	if(n < 4 || n > 26 || n % 2 != 0)
	{
		printf("Invalid board size.\n");
		return 0;
	}
	char board[26][26];
	
	char row;
	char col;
	for(row = 'a'; row < 'a'+ n; row++)
	{
		for(col = 'a'; col < 'a'+ n; col++)
		{
			if((row - 'a' == n/2 && col - 'a' == n/2) || (row - 'a' == n/2 - 1 && col - 'a' == n/2 - 1))
			{
				board[row-'a'][col-'a'] = 'W';
			}else if((row-'a' == n/2 && col-'a' == n/2 - 1) || (row-'a' == n/2 - 1 && col-'a' == n/2))
			{
				board[row-'a'][col-'a'] = 'B';
			}else{
				board[row-'a'][col-'a'] = 'U';
			}
		}
	}
	row = 'a';
	col = 'a';
	char colour = 'O';
	
	printBoard(board, n);
	printf("Enter board configuration:\n");
	
	do{
		scanf(" %c%c%c", &colour, &row, &col);
		if(board[row-'a'][col-'a'] == 'U')
			board[row-'a'][col-'a'] = colour;
	} while(row != '!' && col != '!' && colour != '!');
	printBoard(board, n);
	
	// Making moves
	printPotentialMoves(board, n, 'W');
	printPotentialMoves(board, n, 'B');
	
	printf("Enter a move:\n");
	scanf(" %c%c%c", &colour, &row, &col);
	
	int deltaRow, deltaCol;
	int dRow, dCol;
	bool validMove = false;
	
	if(positionInBounds(n, row, col) && (colour == 'W' || colour == 'B')){
		if(board[row-'a'][col-'a'] == 'U'){
			for(deltaRow = -1; deltaRow <= 1; deltaRow++)
			{
				for(deltaCol = -1; deltaCol <= 1; deltaCol++)
				{
					if(checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol))
					{
						for(dRow = deltaRow, dCol = deltaCol; board[row-'a'+dRow][col-'a'+dCol] != colour; dRow+=deltaRow, dCol+=deltaCol )
						{
							board[row-'a'+dRow][col-'a'+dCol] = colour;
						}
						validMove = true;
					}
				}
			}
			board[row-'a'][col-'a'] = colour;
		}
	}
	
	if(validMove){
		printf("Valid move.\n");
	}else{
		printf("Invalid move.\n");
	}
	printBoard(board, n);
	
	return 0;
}

void printPotentialMoves(char board[][26], int n, char colour)
{
	int row;
	int col;
	printf("Available moves for %c:\n", colour);
	for(row='a'; row < 'a'+n; row++)
	{
		for(col='a'; col<'a'+n; col++)
		{
			if( board[row-'a'][col-'a'] == 'U'
			&& (checkLegalInDirection(board, n, row, col, colour, -1, -1) || checkLegalInDirection(board, n, row, col, colour, 0, -1)
			|| checkLegalInDirection(board, n, row, col, colour, 1, -1) || checkLegalInDirection(board, n, row, col, colour, -1, 0)
			|| checkLegalInDirection(board, n, row, col, colour, 1, 0) || checkLegalInDirection(board, n, row, col, colour, -1, 1)
			|| checkLegalInDirection(board, n, row, col, colour, 0, 1) || checkLegalInDirection(board, n, row, col, colour, 1, 1) ))
				printf("%c%c\n", row, col);
		}
	}
}

void printBoard(char board[][26], int n)
{
	int i;
	int j;
	char letter = 'a';
	printf("  ");
	for(i=0; i<n; i++)
	{
		printf("%c", letter+i);
	}
	printf("\n");
	
	for(i=0; i<n; i++)
	{
		printf("%c ", letter+i);
		for(j=0; j<n; j++)
		{
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
}

bool positionInBounds(int n, char row, char col)
{
	return (row - 'a' >= 0 && row - 'a' < n && col - 'a' >= 0 && col - 'a' < n);
}

bool checkLegalInDirection(char board[][26], int n, char row, char col, char colour, int deltaRow, int deltaCol)
{
	int dRow, dCol;
	bool flippableColours = false;
	bool endColour = false;
	
	for(dRow = deltaRow, dCol = deltaCol; positionInBounds(n, row+dRow, col+dCol) && !endColour; dRow += deltaRow, dCol += deltaCol)
	{
		if(colour == board[row-'a'+dRow][col-'a'+dCol])
		{
			endColour = true;
		}else if(board[row-'a'+dRow][col-'a'+dCol] == 'U' && !endColour){
			return false;
		}else if(colour != board[row-'a'+dRow][col-'a'+dCol] && !endColour && board[row-'a'+dRow][col-'a'+dCol] != 'U'){
			flippableColours = true;
		}
	}
	
	return flippableColours && endColour;
}
