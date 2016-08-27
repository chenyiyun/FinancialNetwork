#define _CRT_SECURE_NO_DEPRECATE 1

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "param.h"
#include "random.h"

#define ThrHld  0.1



/*---------������˷���5.19�Ѳ��Գɹ�-----------*/
void multiMatrix(double a[NUMNODES][NUMNODES], double b[NUMNODES][NUMNODES], double ab[NUMNODES][NUMNODES])
//double *multiMatrix(double (*a)[NUMNODES],double *b)
{
    printf("5.1. multiplying...\n");
    int i, j, k;
    double temp;
    //double *ab;
    //ab = (double*) calloc( NUMNODES, sizeof(ab));

    //printf("5.1.checking ab...\n");
    //printf("   %f\n",ab[3]);

    //printf("\n");

    k = 0;

    for (i = 0; i < NUMNODES; i++)
    {
        for (k = 0; k < NUMNODES; k++) {
            temp = 0.0;
            for (j = 0; j < NUMNODES; j++)
            {
                //temp+=(*(*(a+i)+j))*(*(b+i));
                temp += a[i][j] * b[j][k];
            }
            ab[i][k] = temp;
        }
        //printf("  %f",ab[i]);
    }

    //return ab;
}

/*--------�����������5.19�Ѳ��Գɹ�---------=*/
void subMatrix(double b[NUMNODES][NUMNODES], double a_b[NUMNODES][NUMNODES])
{
    int i, j;
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
        {
            if (i == j) a_b[i][j] = 1 - b[i][j];
            else a_b[i][j] = 0 - b[i][j];
        }

}
/*--------�������ת�á�5.19�Ѳ��Գɹ�---------*/
//void transMatrix(double (*a)[NUMNODES],double (*b)[NUMNODES])
void transMatrix(double gra[NUMNODES][NUMNODES], double (*b)[NUMNODES])
{
    int i, j;
    int t = 1;
    //printf("  %f",b[i][j]);//a,b���Ѵ���
    printf("!!!!!checking a...\n");
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++, t++)
        {
            printf("   %f", gra[i][j]);
            if (t % NUMNODES == 0)
                printf("\n");
        }


    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
            b[j][i] = gra[i][j];
    //a�ɹ�ת��Ϊb
    printf("!!!!!checking b...\n");
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++, t++)
        {
            printf("   %f", b[i][j]);
            if (t % NUMNODES == 0)
                printf("\n");
        }

    printf("transMatrix is done!\n");
}
/*-----����C�⣨1-����⣩���Խ�����ֵ����5.20�Ѳ��Գɹ�-----------*/
//void shpMatrix(double (*a)[NUMNODES],double (*b)[NUMNODES])
void shpMatrix(double gra[NUMNODES][NUMNODES], double b[NUMNODES][NUMNODES]) {
    int i, j;
    int t = 1;
    double *temp;
    temp = (double*)calloc(NUMNODES, sizeof(temp));
    double c[NUMNODES][NUMNODES];

    //C=a�Ѿ���������

    printf("  transforming...");
    transMatrix(gra, c); //����c�Ǿ���a��ת�þ���


    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
        {
            temp[i] += c[i][j]; //�����c��i�еĺͣ������a��i�еĺ�
        }
    //cheking temp[i]

    printf("   %f\n", temp[0]);
    printf("   %f\n", temp[1]);


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
    printf("showing the new sharpC=b...\n");
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++, t++)
        {
            printf("  %f", b[i][j]);
            if (t % NUMNODES == 0)printf("\n");
        }
}
/*-----������Ϊ��������ϵ�з�����5.23�Ѳ��Գɹ�-----------*/

//��OK������һ��չ������|A|
double getA(double arcs[NUMNODES][NUMNODES], int n)
{
    //if n equals one, then result should be the first element
    if (n == 1)
    {
        return arcs[0][0];
    }
    double ans = 0.0;
    double temp[NUMNODES][NUMNODES];
    int i, j, k;
    //i denotes for the column number
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            for (k = 0; k < n - 1; k++)
            {
                temp[j][k] = arcs[j + 1][(k >= i) ? k + 1 : k]; //skip the column that i is at.
            }//recursively calculate the result.
        }
        double t = getA(temp, n - 1);
        if (i % 2 == 0)
        {
            ans += arcs[0][i] * t;
        } else
        {
            ans -= arcs[0][i] * t;
        }
    }
    //��OK������3��ansֵ

    return ans;
}

//��OK������ÿһ��ÿһ�е�ÿ��Ԫ������Ӧ������ʽ�����A*
void getAStar(double arcs[NUMNODES][NUMNODES], int n, double ans[NUMNODES][NUMNODES]) //����ÿһ��ÿһ�е�ÿ��Ԫ������Ӧ������ʽ�����A*
{
    if (n == 1)
    {
        ans[0][0] = 1;
        return;
    }
    int i, j, k, t;
    double temp[NUMNODES][NUMNODES];
    for (i = 0; i < n; i++) //for each element in the matrix
    {
        for (j = 0; j < n; j++) //fill the matrix with element, and skip the i row and j column element.
        {
            for (k = 0; k < n - 1; k++)
            {
                for (t = 0; t < n - 1; t++)
                {
                    temp[k][t] = arcs[k >= i ? k + 1 : k][t >= j ? t + 1 : t];
                }
            }
            //get the temp matrix determinate value and fill it in corresponding position.
            ans[j][i] = getA(temp, n - 1);
            //if i+j is odd, then the result should be reverse.
            if ((i + j) % 2 == 1)
            {
                ans[j][i] = -ans[j][i];
            }
        }
    }
}

//��OK���������arcs->invs
void inversematrix(double arcs[NUMNODES][NUMNODES], double invs[NUMNODES][NUMNODES])
{
    printf("4.1 inversing matrix...\n");
    double astar[NUMNODES][NUMNODES];
    int i, j;
    //int n=3;

    double a = getA(arcs, NUMNODES);//
    if (a == 0)
    {
        printf("can not transform!\n");
    }
    else
    {
        getAStar(arcs, NUMNODES, astar);//
        for (i = 0; i < NUMNODES; i++)//
        {
            for (j = 0; j < NUMNODES; j++)//
            {
                invs[i][j] = (double)astar[i][j] / a;
            }
        }
    }
    printf("4.1 the inverse of matrix I-C is done.\n");
    printf("\n");


}

void Matrix_bV(double **C, double D[NUMNODES][1], double bV[NUMNODES][1])
{
    int i, j, k;
    //int t=1;
    double I[NUMNODES][NUMNODES];
    double tMatrix[NUMNODES][NUMNODES];//I-C
    double tempMatrix[NUMNODES][NUMNODES];//(I-C)-1
    double temp;


    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
        {
            if (i == j)
            {
                I[i][j] = 1.0;
            }
            else
            {
                I[i][j] = 0.0;
            }
        }

    //printf("3.1. calculating the matrix I-C...\n");
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
        {
            tMatrix[i][j] = I[i][j] - C[i][j];
        }
    inversematrix(tMatrix, tempMatrix);

    for (i = 0; i < NUMNODES; i++)
    {
        temp = 0.0;
        for (j = 0; j < NUMNODES; j++)
        {
            //temp+=(*(*(a+i)+j))*(*(b+i));
            temp += tempMatrix[i][j] * bV[j][0];
        }
        bV[i][0] = temp;
        //printf("  %f",ab[i]);
    }
    printf("\n");
    //return bV;

}

void Matrix_mV(double gra[NUMNODES][NUMNODES], double bV[NUMNODES][1], double mV[NUMNODES][1])
{
    int i,j;
    double shpC[NUMNODES][NUMNODES];
    double temp=0.0;

    shpMatrix(gra, shpC);
    for (i = 0; i < NUMNODES; i++)
    {
        temp = 0.0;
        for (j = 0; j < NUMNODES; j++)
        {
            //temp+=(*(*(a+i)+j))*(*(b+i));
            temp += shpC[i][j] * bV[j][0];
        }
        mV[i][0] = temp;
        //printf("  %f",ab[i]);
    }


}

void Matrix_WeightA(double gra[NUMNODES][NUMNODES], double WeightA[NUMNODES][NUMNODES]) {
    double c_hat[NUMNODES][NUMNODES];
    double a_b[NUMNODES][NUMNODES];
    shpMatrix(gra, c_hat);
    subMatrix(gra, a_b);

    multiMatrix(c_hat, a_b, WeightA);
}


