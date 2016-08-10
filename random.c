#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "param.h"
#include "random.h"
#include "matrixCalculator.h"

#define NumNodes 10
#define ThrHld  0.1

double ddntime;



double generate_weight()
{
	double x, y;
	x = rand() % 3;
	if (x == 0)
	{y = (1 + rand() % 49) * 0.01;}
	else
	{y = (1 + rand() % 99) * 0.001;}
	return y;
}

int check_threshold( double th )
{
	double y;

	y = (double)rand() / (RAND_MAX);
	//printf("get y is %f\n",y);

	if ( y > th )
		return 1;
	else
		return 0;
}

double *generate_matrixD()
{
	printf("2. generating the matrix D...\n");

	int i;
	double *matrixD;
	matrixD = (double*) calloc( NumNodes, sizeof(matrixD));

	for (i = 0; i < NumNodes; i++)
	{
		matrixD[i] = 1 + rand() % 100;
	}

	printf("2. matrixD is done...\n");
	return matrixD;
}
