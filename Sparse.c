#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Matrix.h"
#define MAX_LEN 127

int main(int argc, char* argv[])
{
	int one, two, row, column, i, n;
	double data;
	FILE* in; // file handle input
 	FILE* out; // file handle output
 	Matrix A, B, C, D, E, F, G, H, I, J;

 	// check for correct number of arguments
	if( argc != 3 )
	{
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		exit(1);
	}

	// open input file for reading
	in = fopen(argv[1], "r");
	if( in==NULL )
	{
		printf("Unable to read from file %s\n", argv[1]);
		exit(1);
	}

	// open output file for writing
	out = fopen(argv[2], "w");
	if( out==NULL )
	{
		printf("Unable to write to file %s\n", argv[2]);
		exit(1);
	}

	// scan Matrix
	fscanf(in, " %d", &n);
	fscanf(in, " %d", &one);
	fscanf(in, " %d", &two);

	A = newMatrix(n);
	B = newMatrix(n);

	// scan and add entries to matrices
	for(i=0; i<one; i++)
	{
		fscanf(in, " %d", &row);
		fscanf(in, " %d", &column);
		fscanf(in, " %lf", &data);
		changeEntry(A, row, column, data);
	}
	for(i=0; i<two; i++)
	{
		fscanf(in, " %d", &row);
		fscanf(in, " %d", &column);
		fscanf(in, " %lf", &data);
		changeEntry(B, row, column, data);
	}

	// printing output
	fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
	printMatrix(out, A);

	fprintf(out, "\nB has %d non-zero entries:\n", NNZ(B));
	printMatrix(out, B);

	fprintf(out, "\n(1.5)*A =\n");
	printMatrix(out, C = scalarMult(1.5, A));

	fprintf(out, "\nA+B =\n");
	printMatrix(out, D = sum(A, B));

	fprintf(out, "\nA+A =\n");
	printMatrix(out, E = sum(A, A));

	fprintf(out, "\nB-A =\n");
	printMatrix(out, F = diff(B, A));

	fprintf(out, "\nA-A =\n");
	printMatrix(out, G = diff(A, A));

	fprintf(out, "\nTranspose(A) =\n");
	printMatrix(out, H = transpose(A));

	fprintf(out, "\nA*B =\n");
	printMatrix(out, I = product(A, B));

	fprintf(out, "\nB*B =\n");
	printMatrix(out, J = product(B, B));

	// freeing
	freeMatrix(&A);
	freeMatrix(&B);
	freeMatrix(&C);
	freeMatrix(&D);
	freeMatrix(&E);
	freeMatrix(&F);
	freeMatrix(&G);
	freeMatrix(&H);
	freeMatrix(&I);
	freeMatrix(&J);

	// close files
	fclose(in);
  fclose(out);

 	return(0);
}
