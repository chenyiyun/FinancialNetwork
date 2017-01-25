#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "param.h"
#include "random.h"
#include "matrixCalculator.h"

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
    
    if ( y > th )
        return 1;
    else
        return 0;
}

void normalize(double gra[NUMNODES][NUMNODES]){
    double sum[NUMNODES];
    int i=0,j=0,k=0;
    
    for(i=0;i<NUMNODES;i++){
        for(j=0;j<NUMNODES;j++){
            sum[j]+=gra[i][j];
        }
    }
    
    for(k=0;k<NUMNODES;k++){
        double salt=rand()*1.0/ RAND_MAX *sum[k];
        sum[k]+=salt;
    }
    
    for(i=0;i<NUMNODES;i++){
        for(j=0;j<NUMNODES;j++){
            gra[i][j]=gra[i][j]/sum[j];
        }
    }
    
}

double *generate_matrixD(double *matrixD)//give the value to direct value
{
    int i;
    
    for (i = 0; i < NUMNODES; i++)
    {
        matrixD[i] = 1 + rand() % 100;
    }
    
    return matrixD;
}
