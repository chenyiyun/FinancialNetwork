#define S_GA
//#define S_HG

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "param.h"
#include "node.h"
#include "random.h"
#include "matrixCalculator.h"

#define NumNodes 10
#define Beta 0.30
void main()
{

	srand( (unsigned) time( NULL ) );

	double **C;
	Node retnode;
	C=initializeGraph();

    retnode = Node transform_graph(C);








}


/*【测试矩阵】
void main()
{
    srand((unsigned) time(NULL));
    int i,j,t=1;
    double **C;
    double *D;
    double bV[NumNodes];
    double mV[NumNodes];


    //生成C矩阵
    C=initializeGraph();
    printf("1. showing the matrix C...\n");
    for(i=0;i<NumNodes;i++)
       for(j=0;j<NumNodes;j++,t++)
    {
        printf("  %f",C[i][j]);
        if(t%NumNodes==0)printf("\n");
    }
    printf("\n");

    //生成D向量
    D=generate_matrixD();
    printf("2. showing the matrix D...\n");
	for(i=0;i<NumNodes;i++)
    {
        printf("  %f",D[i]);
    }
    printf("\n");

    //生成账面价值bV
    //已测试成功
    Matrix_bV(C,D,bV);
    /*
    printf("6.print bV in main.\n");
    for(i=0;i<NumNodes;i++)
    {
        printf("  %f",bV[i]);
    }
    */
    //生成C尖
    //shpMatrix(C,shpC);

    //生成市场价值mV
    /*
    printf("6. generating the matrix mV...\n");

    Matrix_mV(C,bV,mV);

    printf("6. mV is done.\n");



}

    /*求逆矩阵
    double arcs[NumNodes][NumNodes]={{2.0,2.0,3.0},{4.0,5.0,6.0},{7.0,8.0,9.0}};
    double inversematrix[NumNodes][NumNodes];

    invermatrix(arcs,inversematrix);

    */

    //return 0;






/*
int main()
{
    double arcs[NumNodes][NumNodes]={{1.0,2.0,3.0},{4.0,5.0,6.0},{7.0,8.0,9.0}};
    int i,j;
    int t=1;
    double a = getA(arcs, NumNodes);
    printf("showing the matrix A...\n");
    for(i=0;i<NumNodes;i++)
       for(j=0;j<NumNodes;j++,t++)
    {
        printf("  %f",arcs[i][j]);
        if(t%3==0)printf("\n");
    }
    printf("showing the |A|...\n");
    printf("%f\n",a);


}*/
 /*   int i;
    double **MB;
    MB = (double**) calloc( NumNodes, sizeof(*MB));
	for(i=0; i<NumNodes; i++)
		MB[i] = (double*) calloc( NumNodes, sizeof(**MB));

    //printf("%f",MB[0][0]);
    //double MA[NumNodes][NumNodes]={{0.0,0.5},{0.5,0.0}};
    double MA[NumNodes][NumNodes]={{0.0,0.5},{0.5,0.0}};
    //printf("%f",MA[0][1]);
	//double MB[3][3]={{1.0,2.0,3.0},{4.0,5.0,6.0},{7.0,8.0,9.0}};
    //printf("MA is %f\n",*(*(MA+1)+2));
    shpMatrix(MA,MB);

    return 0;*/





