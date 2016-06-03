#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

extern double random_uniform(double a, double b);
extern double generate_weight();
extern int check_threshold( double th );
extern double generate_directHld();
//extern void generate_matrixD(double *matrixD);
extern double *generate_matrixD();

//extern double getSecs();

//extern void present( double v, int *xv );



extern double ddntime;

#endif // RANDOM_H_INCLUDED
