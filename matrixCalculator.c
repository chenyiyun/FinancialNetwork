#define _CRT_SECURE_NO_DEPRECATE 1
#include<stdio.h>
#include<math.h>
//#include <malloc.h>
#include <stdlib.h>
#include "param.h"
#include "random.h"

#define NumNodes 10
#define Beta 0.30



/*---------【矩阵乘法】5.19已测试成功-----------*/
void multiMatrix(double (*a)[NumNodes],double *b,double *ab)
//double *multiMatrix(double (*a)[NumNodes],double *b)
{
    printf("5.1. multiplying...\n");
	int i,j;
	double temp;
	//double *ab;
	//ab = (double*) calloc( NumNodes, sizeof(ab));

    //printf("5.1.checking ab...\n");
    //printf("   %f\n",ab[3]);

    //printf("\n");



	for(i=0;i<NumNodes;i++)
     {
        temp = 0.0;
        for(j=0;j<NumNodes;j++)
		{
		    //temp+=(*(*(a+i)+j))*(*(b+i));
		    temp+=a[i][j]*b[i];
		}
		ab[i]=temp;
		//printf("  %f",ab[i]);
     }
     printf("5.1. multiply is done.\n");

     printf("5.1. showing the multi of ab...\n");

    for(i=0;i<NumNodes;i++)
    {
        printf("  %f",ab[i]);
    }
    printf("\n");

    //return ab;
}

/*--------【矩阵减法】5.19已测试成功---------=*/
void subMatrix(double (*a)[NumNodes],double (*b)[NumNodes],double (*a_b)[NumNodes])
{
    int i,j;
    for(i=0;i<NumNodes;i++)
       for(j=0;j<NumNodes;j++)
       {
        a_b[i][j]=a[i][j]-b[i][j];
       }

}
/*--------【求矩阵转置】5.19已测试成功---------*/
//void transMatrix(double (*a)[NumNodes],double (*b)[NumNodes])
void transMatrix(double **a,double (*b)[NumNodes])
{
    int i,j;
    int t=1;
    //printf("  %f",b[i][j]);//a,b均已传入
    printf("!!!!!checking a...\n");
    for (i=0; i<NumNodes; i++)
        for (j=0; j<NumNodes; j++,t++)
        {
            printf("   %f",a[i][j]);
            if(t%NumNodes==0)
                printf("\n");
        }


    for (i=0; i<NumNodes; i++)
        for (j=0; j<NumNodes; j++)
		    b[j][i] = a[i][j];
    //a成功转置为b
    printf("!!!!!checking b...\n");
    for (i=0; i<NumNodes; i++)
        for (j=0; j<NumNodes; j++,t++)
        {
            printf("   %f",b[i][j]);
            if(t%NumNodes==0)
                printf("\n");
        }

    printf("transMatrix is done!\n");
}
/*-----【求C尖（1-矩阵尖）（对角线有值）】5.20已测试成功-----------*/
//void shpMatrix(double (*a)[NumNodes],double (*b)[NumNodes])
void shpMatrix(double **a,double (*b)[NumNodes])
{
    int i,j;
    int t=1;
    double *temp;
    temp = (double*)calloc(NumNodes,sizeof(temp));
    double c[NumNodes][NumNodes];

    //C=a已经传进来了

    printf("  transforming...");
    transMatrix(a,c);//矩阵c是矩阵a的转置矩阵


    for(i=0;i<NumNodes;i++)
        for(j=0;j<NumNodes;j++)
        {
            temp[i]+=c[i][j];//求矩阵c第i行的和，求矩阵a第i列的和
        }
    //cheking temp[i]

    printf("   %f\n",temp[0]);
    printf("   %f\n",temp[1]);


    for(i=0;i<NumNodes;i++)
        for(j=0;j<NumNodes;j++)
        {
            if(j==i)
            {
                b[i][j]=1-temp[i];
            }
            else
            {
                b[i][j]=0.0;
            }
        }
    printf("showing the new sharpC=b...\n");
    for(i=0;i<NumNodes;i++)
       for(j=0;j<NumNodes;j++,t++)
    {
        printf("  %f",b[i][j]);
        if(t%NumNodes==0)printf("\n");
    }
}
/*-----【以下为求逆矩阵的系列方法】5.23已测试成功-----------*/

//【OK】按第一行展开计算|A|
double getA(double arcs[NumNodes][NumNodes], int n)
{
    //if n equals one, then result should be the first element
    if(n==1)
    {
        return arcs[0][0];
    }
    double ans=0.0;
    double temp[NumNodes][NumNodes];
    int i,j,k;
    //i denotes for the column number
    for(i=0;i<n;i++)
    {
       for(j=0;j<n-1;j++)
       {
          for(k=0;k<n-1;k++)
          {
              temp[j][k]=arcs[j+1][(k>=i)?k+1:k];//skip the column that i is at.
          }//recursively calculate the result.
       }
       double t=getA(temp,n-1);
       if(i%2==0)
       {
           ans+=arcs[0][i]*t;
       }else
       {
           ans-=arcs[0][i]*t;
       }
    }
    //【OK】返回3个ans值

    return ans;
}

//【OK】计算每一行每一列的每个元素所对应的余子式，组成A*
void getAStar(double arcs[NumNodes][NumNodes], int n, double ans[NumNodes][NumNodes]) //计算每一行每一列的每个元素所对应的余子式，组成A*
{
    if(n==1)
    {
        ans[0][0]=1;
        return;
    }
    int i,j,k,t;
    double temp[NumNodes][NumNodes];
    for(i=0;i<n;i++)//for each element in the matrix
    {
        for(j=0;j<n;j++)//fill the matrix with element, and skip the i row and j column element.
        {
           for(k=0;k<n-1;k++)
           {
              for(t=0;t<n-1;t++)
              {
                 temp[k][t]=arcs[k>=i?k+1:k][t>=j?t+1:t];
              }
           }
           //get the temp matrix determinate value and fill it in corresponding position.
           ans[j][i]=getA(temp,n-1);
           //if i+j is odd, then the result should be reverse.
           if((i+j)%2==1)
           {
               ans[j][i]=-ans[j][i];
           }
        }
    }
}

//【OK】求逆矩阵，arcs->invs
void invermatrix(double arcs[NumNodes][NumNodes],double invs[NumNodes][NumNodes])
{
    printf("4.1 inversing matrix...\n");
    double astar[NumNodes][NumNodes];
    int i, j;
    //int n=3;

    double a = getA(arcs, NumNodes);//
    if (a == 0)
    {
        printf("can not transform!\n");
    }
    else
    {
        getAStar(arcs, NumNodes, astar);//
        for (i = 0; i < NumNodes; i++)//
        {
            for (j = 0; j < NumNodes; j++)//
            {
                invs[i][j]=(double)astar[i][j] / a;
            }
        }
    }
    printf("4.1 the inverse of matrix I-C is done.\n");
    printf("\n");


}

//【OK】求账面价值
/*-------求账面价值bV=（I-C）取逆*矩阵D----------*/
void Matrix_bV(double **C,double D[NumNodes],double bV[NumNodes])
//double *Matrix_bV(double **C,double *D)
{
    int i,j;
    //int t=1;
    double I[NumNodes][NumNodes];
    double tMatrix[NumNodes][NumNodes];//I-C
    double tempMatrix[NumNodes][NumNodes];//(I-C)-1


    //C能传进来
    //D能传进来

    //生成单位矩阵
    //printf("3. generating the matrix I...\n");
    for(i=0;i<NumNodes;i++)
        for(j=0;j<NumNodes;j++)
    {
        if(i==j)
        {
            I[i][j]=1.0;
        }else
        {
            I[i][j]=0.0;
        }
    }
    printf("3. Matrix I is done.\n");

    //求方程中的一部分 tMatrix=I-C
    //printf("3.1. calculating the matrix I-C...\n");
    for(i=0;i<NumNodes;i++)
        for(j=0;j<NumNodes;j++)
        {
            tMatrix[i][j]=I[i][j]-C[i][j];
        }
    printf("3.1. matrix I-C is done.\n");

    //计算（I-C）的逆矩阵
    printf("4. calculating the inverse of matrix I-C...\n");
    invermatrix(tMatrix,tempMatrix);
    /*
    printf("4. showing the inverse of matrix I-C.\n");
    for(i=0;i<NumNodes;i++)
        for(j=0;j<NumNodes;j++,t++)
        {
            printf("  %f",tempMatrix[i][j]);
            if(t%NumNodes==0)
            {
                printf("\n");
            }
        }
    */
    //计算（I-C）逆*D
    printf("5. calculating the multi of matrix (I-C逆*D...\n");
    multiMatrix(tempMatrix,D,bV);
    //bV=multiMatrix(tempMatrix,D);
    /*
    printf("5. showing the multi = bV...\n");

    for(i=0;i<NumNodes;i++)
    {
        printf("  %f",bV[i]);
    }
    */
    printf("\n");
    //return bV;

}

//【OK】求市场价值
/*-------求市场价值mV=C尖bV-----------------*/
void Matrix_mV(double **C,double bV[NumNodes],double mV[NumNodes])
{
    int i;
    //int t=1;
    double shpC[NumNodes][NumNodes];

    //C，shpC在这里是对的

    shpMatrix(C,shpC);
/*
    printf("showing the new sharpC...\n");
    for(i=0;i<NumNodes;i++)
       for(j=0;j<NumNodes;j++,t++)
    {
        printf("  %f",shpC[i][j]);
        if(t%NumNodes==0)printf("\n");
    }
*/
    multiMatrix(shpC,bV,mV);

    printf("checking the bV...\n");
    for(i=0;i<NumNodes;i++)
    {
        printf("  %f",bV[i]);
    }

    printf("Showing the mV...\n");
    for(i=0;i<NumNodes;i++)
    {
        printf("  %f",mV[i]);
    }




}

//【未测试】求C的邻接矩阵A=C尖*（I-C）逆
void Matrix_A(double (*a)[NumNodes],double (*b)[NumNodes],double (*c)[NumNodes])
{
    printf("   generating the matrix A from C尖*（I-C）逆...");
    int i,j,k;
    double temp;
    for(i=0;i<NumNodes;i++)
    {
        for(j=0;j<NumNodes;j++)
        {
            temp = 0.0;
            for(k=0;k<NumNodes;k++)
            {
                temp+=a[i][k]*b[k][j];
            }
            c[i][j]=temp;
        }
    }

}







/*
    printf("showing the MA...\n");
    for(i=0;i<NumNodes;i++)
       for(j=0;j<NumNodes;j++,t++)
    {
        printf("  %f",a[i][j]);
        if(t%3==0)printf("\n");
    }
    printf("showing the MB...\n");
    for(i=0;i<NumNodes;i++)
       for(j=0;j<NumNodes;j++,t++)
    {
        printf("  %f",b[i][j]);
        if(t%3==0)printf("\n");
    }

*/

