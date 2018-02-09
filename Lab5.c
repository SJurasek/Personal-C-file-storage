#include <stdio.h>

void triangle(int numRows); // Creates a 
int choose(int n, int r); // Calculates nCr given n and r. Returns -1 if the inputs are not valid.
int factorial(int n); // Calculates n! given n. Returns -1 if the input is invalid (less than 0)

// Adds spaces between values/numbers in each row.
void spaces(int value, int currentRow, int nextCol, int numRows);

int main(int argc, char **argv)
{
	int rowInput; // Number of rows that will be printed
	printf("Enter the number of rows: ");
	scanf("%d", &rowInput); // Get first input to determine if it will run program at least once
	
	while(rowInput >= 0 && rowInput <= 13)
	{
		triangle(rowInput); // make a pascal triangle using first input, then consequential inputs.
		printf("Enter the number of rows: ");
		scanf("%d", &rowInput); // Take in a new row value
	}
	printf("Goodbye.\n"); // Program end
	
	return 0;
}

void triangle(int numRows)
{
	int row;
	int col;
	int value = 0; // Individual value in the triangle
	for(row = 1; row <= numRows; row++) // Start from row 1 and cycle through each row
	{
		for(col = 1; col <= row; col++) // Start from the first col (which value in the row) and cycle through
		{
			spaces(value, row, col, numRows); // Put initial spacing, and then spacing for future values
			value = choose(row-1, col-1); // nCr values starts at 0C0 for row 1 col 1, therefore subtract 1 from row/col.
			printf("%d", value);
		}
		printf("\n"); // New line for new row
	}
}

int choose(int n, int r)
{
	if( n < r || n < 0 || r < 0) // Make sure both values are positive and that n >= r
	{
		printf("Invalid nCr format\n");
		return -1;
	}else if( n >= r)
	{
		return factorial(n)/(factorial(r) * factorial(n-r)); // n!/(r!(n-r)1) = nCr; use factorial(int n)
	}
}

int factorial(int n)
{
	int totalFactorial = 1; // factorial is 1 or greater for any int >= 0
	
	int i;
	for(i = 1; i <= n; i++) // let i cycle through all positive integers less than and equal to n
	{
		totalFactorial *= i; // factorial calculation
	}
	
	if(n >= 0) // Determine if the given number was not negative
	{
		return totalFactorial; // return the factorial
	}else{
		printf("Invalid factorial\n");
		return -1;
	}
}

void spaces(int value, int currentRow, int nextCol, int numRows)
{
	int i;
	
	if(nextCol == 1) // If this is the first column being printed in the triangle
	{
		for(i=0; i < numRows - currentRow; i++) // Determine position from last row, then print two spaces for each row it is away from the end
		{
			printf("  "); // Every row from the bottom has its first column two spaces to the right of the previous row's first col
		}
	}else{ // hardcode spaces next to numbers with 1, 2, or 3 digits. 4 digit numbers will not have spaces dividing it from the next col
		if(value < 10){
			printf("   ");
		}else if(value < 100){
			printf("  ");
		}else if(value < 1000){
			printf(" ");
		}
	}
}
