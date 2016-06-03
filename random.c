#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "param.h"
#include "random.h"
#include "matrixCalculator.h"

#define NumNodes 10
#define Beta 0.30

double ddntime;


/*--------�ѳɹ�����----------*/
double generate_weight()    // ������ֹ�C��Ȩֵ�����ո������һ����λ����1/3ȡ50-90���ع��ӹ�˾��,2/3ȡ10-40
{
	double x,y;
	x=rand()%3;
	if(x==0)
	{y=(1+rand()%49)*0.01;}
	else
	{y=(1+rand()%99)*0.001;}
	return y;
}

/*--------�ѳɹ�����----------*/
int check_threshold( double th )  //����ͨ�Բ��ԡ�
{
	double y;

	y = (double)rand()/(RAND_MAX);

	if( y < th )
		return 1;
	else
		return 0;
}

/*--------5.19�ѳɹ�����----------*/
double *generate_matrixD()//��direct holding��ֵ���������һά����
{
	printf("2. generating the matrix D...\n");

	int i;
	double *matrixD;
    matrixD = (double*) calloc( NumNodes, sizeof(matrixD));

	for(i=0;i<NumNodes;i++)
	{
		matrixD[i]=1+rand()%100;
	}

    printf("2. matrixD is done...\n");
    return matrixD;
}

/*--------��beta�ݶ�Ϊ������ �����Ʋ��ɱ���------------*/
/*double generate_beta()
{
    double y;
    y=
}
*/


/*
double random_uniform(double a, double b)                   // Uniform Distribution
{
  double y;
  y = (double)rand()/(RAND_MAX);
  return (a+(b-a)*y);
}







*/
/*
double getSecs( )
{
// Get total CPU time in seconds.
  clock_t t;

  t = clock();

  return ( ((double) t)/CLK_TCK );

}







void present( double v, int *xv )
{
	FILE *file;
	int i;
	char filename[2048]="";
	char temp[30];

	// network
	sprintf( temp, "N%d_", NumNodes);
	strcat( filename, temp );
	sprintf( temp, "p%.3f_", CONNECT_PARAM);
	strcat( filename, temp );

	// problem
	sprintf( temp, "k%d_", TARGET_NUM);
	strcat( filename, temp );

	// algorithm
	sprintf( temp, "STY%d_", SOLVE_TYPE);
	strcat( filename, temp );
	sprintf( temp, "w%d_", DISSIMTYPE);
	strcat( filename, temp );
	sprintf( temp, "r%d", TOPK);
	strcat( filename, temp );

	sprintf( temp, ".txt");
	strcat( filename, temp );



	file = fopen(filename, "w");

	fprintf(file, "The optimal value is:  %f\n\n", v);
	fprintf(file, "A=[");

	for(i=0; i<NumNodes; i++)
		if( xv[i] == 1 )
			fprintf(file, "%d, ", i+1);
	fprintf(file, "]\n\n");

	fprintf(file, "time=%.3fs", ddntime);

	fclose(file);
}

*/
