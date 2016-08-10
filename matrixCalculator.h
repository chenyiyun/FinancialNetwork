#ifndef MATRIXCALCULATOR_H_INCLUDED
#define MATRIXCALCULATOR_H_INCLUDED
#define NUMNODES 10
#define ThrHld  0.1

//extern int invMatrix();
extern void multiMatrix(double (*a)[NUMNODES],double *b,double *ab);
//extern double *multiMatrix(double (*a)[NUMNODES],double *b);
extern void subMatrix(double (*a)[NUMNODES],double (*b)[NUMNODES],double (*a_b)[NUMNODES]);
extern void transMatrix(double (*a)[NUMNODES],double (*b)[NUMNODES]);
//extern void shpMatrix(double (*a)[NUMNODES],double (*b)[NUMNODES]);
extern void shpMatrix(double gra[NUMNODES][NUMNODES],double (*b)[NUMNODES]);

extern double getA(int arcs[NUMNODES][NUMNODES], int n);
extern void getAStar(int arcs[NUMNODES][NUMNODES], int n, int ans[NUMNODES][NUMNODES]);
extern void invermatrix(double arcs[NUMNODES][NUMNODES],double invs[NUMNODES][NUMNODES]);
//extern double *Matrix_bV(double **C,double *D);
extern void Matrix_bV(double **C,double D[NUMNODES],double bV[NUMNODES]);
extern void Matrix_mV(double gra[NUMNODES][NUMNODES],double bV[NUMNODES],double mV[NUMNODES]);
extern void Matrix_A(double (*a)[NUMNODES],double (*b)[NUMNODES],double (*c)[NUMNODES]);

#endif