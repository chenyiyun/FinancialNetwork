#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lapacke.h"
#include "param.h"
#include "random.h"

extern double gra[NUMNODES][NUMNODES];

//ab=a*b
void multiMatrix(double a[NUMNODES][NUMNODES], double b[NUMNODES][NUMNODES], double ab[NUMNODES][NUMNODES])
{
    int i, j, k = 0;
    double temp;
    
    for (i = 0; i < NUMNODES; i++)
    {
        for (k = 0; k < NUMNODES; k++) {
            temp = 0.0;
            for (j = 0; j < NUMNODES; j++)
            {
                temp += a[i][j] * b[j][k];
            }
            ab[i][k] = temp;
        }
    }
}

//I_C=I-C
void subMatrix(double C[NUMNODES][NUMNODES], double I_C[NUMNODES][NUMNODES])
{
    int i, j;
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
        {
            if (i == j) I_C[i][j] = 1 - C[i][j];
            else I_C[i][j] = 0 - C[i][j];
        }
    
}

//b=tranpose(gra);
void transMatrix(double gra[NUMNODES][NUMNODES], double (*b)[NUMNODES])
{
    int i, j;
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
            b[j][i] = gra[i][j];
}

//get C hat
void hatMatrix(double gra[NUMNODES][NUMNODES], double b[NUMNODES][NUMNODES]) {
    int i, j;
    double *temp;
    
    temp = (double*)calloc(NUMNODES, sizeof(double));
    
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
        {
            temp[j] += gra[i][j];
        }
    
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
        {
            if (j == i)
            {
                b[i][j] = 1 - temp[i];
            }
            else
            {
                b[i][j] = 0.0;
            }
        } 
}

//get inverse(A), use lapack to speed up the process
void inversematrix(double A[NUMNODES][NUMNODES])
{
    int IPIV[NUMNODES];
    lapack_int m,n,lda,info;
    m = NUMNODES;
    n = NUMNODES;
    lda = NUMNODES;

    info=LAPACKE_dgetrf(LAPACK_ROW_MAJOR,m,n,*A,lda,IPIV);
    if(info==0){
        info=LAPACKE_dgetri(LAPACK_ROW_MAJOR,n,*A,lda,IPIV);
        if(info!=0) exit(info);
        return;
    }else{
        exit(info);
    }
}

//helper function to print out the matrix
void print_matrix(double matrix[NUMNODES][NUMNODES]){
    int i = 0;
    int j = 0;
    for (i = 0; i < NUMNODES; i++) {
        for (j = 0; j < NUMNODES; j++) {
            printf("%f, ", matrix[i][j]);
        }
        printf("\n");
    }
}

//bv=inverse(I-C) * D
void Matrix_bV(double gra[NUMNODES][NUMNODES], double *D, double bV[NUMNODES])
{
    int i=0, j=0;
    double tMatrix[NUMNODES][NUMNODES];//I-C
    double temp;
    
    subMatrix(gra,tMatrix);
    inversematrix(tMatrix);
    for (i = 0; i < NUMNODES; i++)
    {
        temp = 0.0;
        for (j = 0; j < NUMNODES; j++)
        {
            temp += tMatrix[i][j] * D[j];
        }
        bV[i] = temp;
    }
}

//mV=c_hat * bv = matrixA * D
void Matrix_mV(double matrixA[NUMNODES][NUMNODES], double D[NUMNODES], double mV[NUMNODES])
{
    int i=0, j=0;
    double tmp;
    for (i = 0; i < NUMNODES; i++)
    {
        tmp = 0.0;
        for (j = 0; j < NUMNODES; j++)
        {
            tmp += matrixA[i][j] * D[j];
        }
        mV[i] = tmp;
    }

}

//weighA=c_hat * inverse(I-C)
void Matrix_WeightA(double gra[NUMNODES][NUMNODES], double WeightA[NUMNODES][NUMNODES])
{
    double c_hat[NUMNODES][NUMNODES];
    double I_C[NUMNODES][NUMNODES];
    
    hatMatrix(gra, c_hat);
    subMatrix(gra, I_C);
    inversematrix(I_C);
    multiMatrix(c_hat, I_C, WeightA);
}


