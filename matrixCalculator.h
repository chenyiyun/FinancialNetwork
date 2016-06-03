#ifndef MATRIXCALCULATOR_H_INCLUDED
#define MATRIXCALCULATOR_H_INCLUDED
#define NumNodes 10
//extern int invMatrix();
extern void multiMatrix(double (*a)[NumNodes],double *b,double *ab);
//extern double *multiMatrix(double (*a)[NumNodes],double *b);
extern void subMatrix(double (*a)[NumNodes],double (*b)[NumNodes],double (*a_b)[NumNodes]);
extern void transMatrix(double (*a)[NumNodes],double (*b)[NumNodes]);
//extern void shpMatrix(double (*a)[NumNodes],double (*b)[NumNodes]);
extern void shpMatrix(double **a,double (*b)[NumNodes]);

extern double getA(int arcs[NumNodes][NumNodes], int n);
extern void getAStar(int arcs[NumNodes][NumNodes], int n, int ans[NumNodes][NumNodes]);
extern void invermatrix(double arcs[NumNodes][NumNodes],double invs[NumNodes][NumNodes]);
//extern double *Matrix_bV(double **C,double *D);
extern void Matrix_bV(double **C,double D[NumNodes],double bV[NumNodes]);
extern void Matrix_mV(double **C,double bV[NumNodes],double mV[NumNodes]);
extern void Matrix_A(double (*a)[NumNodes],double (*b)[NumNodes],double (*c)[NumNodes]);

#endif // MATRIXCALCULATOR_H_INCLUDED
