#include <stdio.h>

int main(int argc, char **argv)
{
	const int NUM_OF_BANDS = 4;
	const int BAND1 = 0;
	const int BAND2 = 1;
	const int MULT_BAND = 2;
	const int TOL_BAND = 3;
	char bands[NUM_OF_BANDS];
	int values[BAND2+1];
	char orderMagnitude = '\0';
	float multiplier;
	float tolerance;
	float resistance;
	
	
	printf("Please enter the first band:");
	scanf(" %c", &bands[BAND1]);
	printf("Please enter the second band:");
	scanf(" %c", &bands[BAND2]);
	printf("Please enter the multiplier band:");
	scanf(" %c", &bands[MULT_BAND]);
	printf("Please enter the tolerance band:");
	scanf(" %c", &bands[TOL_BAND]);
	printf("Resistor bands:");
	
	for(int i=0; i<NUM_OF_BANDS; i++){
		if(bands[i] < 'a'){
			bands[i] += 'a' - 'A';
		}
	}
	
	for(int i=0; i<=BAND2; i++){
		if(bands[i] == 'k'){
			values[i] = 0;
			printf(" Black");
		}else if(bands[i] == 'b'){
			values[i] = 1;
			printf(" Brown");
		}else if(bands[i] == 'r'){
			values[i] = 2;
			printf(" Red");
		}else if(bands[i] == 'o'){
			values[i] = 3;
			printf(" Orange");
		}else if(bands[i] == 'e'){
			values[i] = 4;
			printf(" Yellow");
		}else if(bands[i] == 'g'){
			values[i] = 5;
			printf(" Green");
		}else if(bands[i] == 'u'){
			values[i] = 6;
			printf(" Blue");
		}else if(bands[i] == 'v'){
			values[i] = 7;
			printf(" Violet");
		}else if(bands[i] == 'y'){
			values[i] = 8;
			printf(" Grey");
		}else if(bands[i] == 'w'){
			values[i] = 9;
			printf(" White");
		}
	}
	
	if(bands[MULT_BAND] == 'k'){
		multiplier = 1;
		printf(" Black");
	}else if(bands[MULT_BAND] == 'b'){
		multiplier = 10;
		printf(" Brown");
	}else if(bands[MULT_BAND] == 'r'){
		multiplier = 100;
		printf(" Red");
	}else if(bands[MULT_BAND] == 'o'){
		multiplier = 1000;
		printf(" Orange");
	}else if(bands[MULT_BAND] == 'e'){
		multiplier = 10000;
		printf(" Yellow");
	}else if(bands[MULT_BAND] == 'g'){
		multiplier = 100000;
		printf(" Green");
	}else if(bands[MULT_BAND] == 'u'){
		multiplier = 1000000;
		printf(" Blue");
	}else if(bands[MULT_BAND] == 'v'){
		multiplier = 10000000;
		printf(" Violet");
	}else if(bands[MULT_BAND] == 'l'){
		multiplier = 0.1;
		printf(" Gold");
	}else if(bands[MULT_BAND] == 's'){
		multiplier = 0.01;
		printf(" Silver");
	}
	
	if(bands[TOL_BAND] == 'b'){
		printf(" Brown");
		tolerance = 1;
	}else if(bands[TOL_BAND] == 'r'){
		printf(" Red");
		tolerance = 2;
	}else if(bands[TOL_BAND] == 'g'){
		printf(" Green");
		tolerance = 0.5;
	}else if(bands[TOL_BAND] == 'u'){
		printf(" Blue");
		tolerance = 0.25;
	}else if(bands[TOL_BAND] == 'v'){
		printf(" Violet");
		tolerance = 0.10;
	}else if(bands[TOL_BAND] == 'y'){
		printf(" Grey");
		tolerance = 0.05;
	}else if(bands[TOL_BAND] == 'l'){
		printf(" Gold");
		tolerance = 5;
	}else if(bands[TOL_BAND] == 's'){
		printf(" Silver");
		tolerance = 10;
	}
	printf("\n");
	
	resistance = ( values[BAND1]*10 + values[BAND2] ) * multiplier;
	
	if(resistance > 1000000){
		orderMagnitude = 'M';
		resistance /= 1000000;
	}else if(resistance > 1000){
		orderMagnitude = 'K';
		resistance /= 1000;
	}
	
	printf("Resistance: %.2f %cOhms +/- %.2f%%\n", resistance, orderMagnitude, tolerance);
	
	return 0;
}
