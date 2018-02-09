#include <stdio.h>
#include <math.h>

int convertBinaryToDecimal(int binaryNum); // I dont want either function to make changes to the original input
int convertDecimalToBinary(int decimalNum); // Name describes what each does

int main(int argc, char **argv)
{
	char conversionChar; // Convert from binary to decimal or vice versa?
	int inputNum; // The number the user wants converted
	printf("Enter B for conversion of Binary to Decimal, OR\nEnter D for conversion of Decimal to Binary: ");
	scanf(" %c", &conversionChar);
	
	if(conversionChar == 'B')
	{ //	User wants to convert binary to decimal:
		printf("Enter your number: ");
		scanf("%d", &inputNum);
		printf("%d in binary = %d in decimal\n", inputNum, convertBinaryToDecimal(inputNum));
		
	} else if(conversionChar == 'D')
	{//		User wants to convert decimal to binary:
		printf("Enter your number: ");
		scanf("%d", &inputNum);
		printf("%d in decimal = %d in binary\n", inputNum, convertDecimalToBinary(inputNum));
		
	} else {
		printf("Invalid input; Goodbye\n");
	}
	
	return 0;
}

int convertBinaryToDecimal(int binaryNum)
{
	int decimalNum = 0; // the decimal value to be calculated
	int binaryAlt = 0;
	
	int i;
	// (int)( log(binaryNum)/log(10) ) is the largest positive int value that 10 can be raised to that is less than the given number
	// i will cycle through each consecutive order of magnitude of 10 that can fit within binaryNum
	for(i = (int)( log(binaryNum)/log(10) ); i >= 0; i--)
	{
		if(binaryNum - binaryAlt >= (int)pow(10,i)) // determine if the remaining amount is able to fit the next largest power of 10
		{
			decimalNum += (int)pow(2,i); // Adding each power of 10 that fits the condition in decimal form
			binaryAlt += (int)pow(10,i); // binary value of decimalNum
		}
	}
	
	return decimalNum; // decimalNum is the final decimal value of binaryNum
}

int convertDecimalToBinary(int decimalNum)
{
	int binaryNum = 0;
	int decimalAlt = 0;
	
	int i;
	for(i= (int)( log(decimalNum)/log(2) ); i >= 0 ; i--)
	{
		if(decimalNum - decimalAlt >= (int)pow(2,i))
		{
			binaryNum += (int)pow(10,i);
			decimalAlt += (int)pow(2,i);
		}
	}
	
	return binaryNum;
}
