#ifndef MATRIXCALCULATOR_H_INCLUDED
#define MATRIXCALCULATOR_H_INCLUDED

void multiMatrix(double a[NUMNODES][NUMNODES], double b[NUMNODES][NUMNODES], double ab[NUMNODES][NUMNODES]);
void subMatrix(double C[NUMNODES][NUMNODES], double I_C[NUMNODES][NUMNODES]);
void transMatrix(double gra[NUMNODES][NUMNODES], double (*b)[NUMNODES]);
void hatMatrix(double gra[NUMNODES][NUMNODES], double b[NUMNODES][NUMNODES]);
void inversematrix(double A[NUMNODES][NUMNODES]);
void print_matrix(double matrix[NUMNODES][NUMNODES]);
void Matrix_bV(double gra[NUMNODES][NUMNODES], double *D, double bV[NUMNODES]);
void Matrix_mV(double matrixA[NUMNODES][NUMNODES], double D[NUMNODES], double mV[NUMNODES]);
void Matrix_WeightA(double gra[NUMNODES][NUMNODES], double WeightA[NUMNODES][NUMNODES]);

#endif
