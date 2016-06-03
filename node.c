#define _CRT_SECURE_NO_DEPRECATE 1

#define RECORD_GRAPH

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include "param.h"
#include "random.h"
#include "node.h"
#include "matrixCalculator.h"

#define NumNodes 10
#define Beta 0.30

Node ori_graph;

/*-------------------5/18成功生成Cij不对称双方向矩阵---------------------*/
double **initial_graph()
{
	double **gra;
	int i,j;

	printf("Initialize Graph...");
	gra = (double**) calloc( NumNodes, sizeof(*gra) );
	for(i=0; i<NumNodes; i++)
		gra[i] = (double*) calloc( NumNodes, sizeof(**gra) );

	for(i=0; i<NumNodes; i++)
		for(j=0; j<NumNodes; j++)
			gra[i][j] = 0.0;

	for(i=0; i<NumNodes-1; i++)      //生成上三角矩阵0,1模板
		for(j=i+1; j<NumNodes; j++)
			if( check_threshold(CONNECT_PARAM) ){
			gra[i][j] = 1;
			}else{
			gra[i][j] = 0;
			}

	for(i=0; i<NumNodes-1; i++)      //按照上三角矩阵0,1模板填充整个C矩阵
		for(j=i+1; j<NumNodes; j++)
		{
			if(gra[i][j]==1)
			gra[i][j]=generate_weight();
			gra[j][i]=generate_weight();
		}
	printf("initial_graph is done.\n");

	return gra;
}

//done
void destroy_graph( double **gra )//清空网络
{
	int i;
	for(i=0; i<NumNodes; i++)
		free(gra[i]);
	free(gra);
}

/*-------------------5/18成功执行并打印---------------------*/
void connection_remedy( double **gra )//连通度补救，如果发现网络不连通，用此function
{
	int i, j, k, m, goon=1;
	int x[NumNodes],y[NumNodes];

	printf("Remedy connectivity...");
	for(i=0; i<NumNodes; i++)
	{
		x[i] = 0;
		y[i] = 0;
	}

	x[0] = 1;

	while(goon == 1)
	{
		goon = 0;
		for(i=0; i<NumNodes; i++)
			if(x[i]==1 && y[i]==0)
			{
				goon = 1;
				y[i] = 1;
				for(j=0; j<NumNodes; j++)
					if(gra[i][j]>0)
						x[j] = 1;
			}
	}

	for(m=0; m<NumNodes; m++)
		if(x[m]==0)
		{
			k=rand()%2;

			if(k==0)
					gra[m][m-1]=0-generate_weight();
				else
					gra[m][m-1]=generate_weight();

				gra[m-1][m] = 0 - gra[m][m-1];


			x[m] = 1;
			goon = 1;
			while(goon == 1)
			{
				goon = 0;
				for(i=0; i<NumNodes; i++)
					if(x[i]==1 && y[i]==0)
					{
						goon = 1;
						y[i] = 1;
						for(j=0; j<NumNodes; j++)
							if(gra[i][j]>0)
								x[j] = 1;
					}
			}
		}

	printf("connection_remedy is done.\n");
}

//done
void record_graph(double **gra) //将初始好的网络录入文件，释放内存
{
	FILE *file;
	int i,j;

	file = fopen("recordGraph", "w");

	for(i=0; i<NumNodes; i++)
	{
		for(j=0; j<NumNodes; j++)
			fprintf(file, "  %lf", gra[i][j]);
		fprintf(file, "\n");
	}

	fclose(file);
}

//done
Node newnode()
{
	Node node;

	node = (Node)malloc(sizeof(*node));

	node->id = 0;
	node->Drhld = 0.0;
	node->Crhld = 0.0;
	node->bV = 0.0;
	node->mV = 0.0;
	node->infln = 0;

	node->numNeib = 0;
	node->neighbor = NULL;
	node->neibID = NULL;
	node->weight = NULL;
	//node->whereNeibPutMe = NULL;
	//node->sumweight = 0.0;
	node->neibInfln = NULL;

	node->next = NULL;
	return node;
}

//done
Node getNodepointer(Node graphnodes, int id)//根据链表头结点graphnodes找到具体id的结点
{
	Node node;
	int i;

	node = graphnodes;

//	if(id>1)
	for(i=1; i<id; i++)
		node = node->next;

	return node;
}

//6.2 done untest
/*--------------初始化节点性质----------------*/
 Node transform_graph(double **gra)
{
    //srand((unsigned) time(NULL));
	Node node, retnode, tempnode;//node是活动着的点，retnode是固定的头结点，tempnode是临时的点
	int i,j,k, count;
	//int t=1;
	//double sum;
    double **C;
    double *D;
    double bV[NumNodes];
    double mV[NumNodes];


	//【1】生成Crhld矩阵
	//C=initializeGraph();
    /*
    printf("1. showing the matrix C...\n");
    for(i=0;i<NumNodes;i++)
       for(j=0;j<NumNodes;j++,t++)
    {
        printf("  %f",C[i][j]);
        if(t%NumNodes==0)printf("\n");
    }
    printf("\n");
	*/

    //【2】生成Drhld向量
    D=generate_matrixD();
    /*
    printf("2. showing the matrix D...\n");
	for(i=0;i<NumNodes;i++)
    {
        printf("  %f",D[i]);
    }
    printf("\n");
    */

    //【3】生成bV账面价值
    Matrix_bV(gra,D,bV);
    /*
    printf("6.print bV in main.\n");
    for(i=0;i<NumNodes;i++)
    {
        printf("  %f",bV[i]);
    }
    */

    //【4】生成mV市场价值
    Matrix_mV(gra,bV,mV);


    //【5】将网络转化成链表，给链表中的每个结点赋值
	retnode = newnode();//retnode 是头结点
	node = retnode;//头结点指向id为1的第一个结点node地址

    //【6】给头结点赋值
    node->id = 1;
	node->Drhld = D[0];
	for(i=0;i<NumNodes;i++)
    {
        node->Crhld[0][i]=C[0][i];
    }
    node->bV = bV[0];
    node->mV = mV[0];
    node->infln = 0;

    //【7】给链表后面的结点 自有性质 赋值
	for(i=2; i<=NumNodes; i++)
	{
		tempnode = newnode();
		tempnode->id = i;
		tempnode->Drhld = D[i-1];
		for(j=0;j<NumNodes;j++)
        {
            tempnode->Crhld[i-1][j]=C[i-1][i];
        }
		tempnode->bV = bV[i-1];
		tempnode->mV = mV[i-1];
		tempnode->infln = 0;

		node->next = tempnode;
		node = tempnode;
	}

    //【8】给链表后面的结点 邻居信息 赋值
	node = retnode;//回到链表中的头结点
	for(i=0; i<NumNodes; i++)
	{	count = 0;
		for(j=0; j<NumNodes; j++)//遍历每个节点有多少邻居，数量为count
			if(gra[i][j]>0.0)
				count++;
		node->numNeib = count;

		//给结构中五个邻居数组分配count个空间
		node->neighbor = (Node*)calloc(count, sizeof(Node));//Node *neighbor,指向邻居的指针
		node->neibID = (int*)calloc(count, sizeof(int));//int *neibID，邻居的ID
		node->weight = (double*)calloc(count, sizeof(double));//double *weight,与每一个邻居的边的权重
		//node->whereNeibPutMe = (int*)calloc(count, sizeof(int));//int *whereNeibPutMe，邻居把我放在数组的哪里
		node->neibInfln = (int*)calloc(count, sizeof(int));//邻居是否被传染破产

		//给这几个邻居数组附上相应的邻居节点指针
		k = 0;
		for(j=0; j<NumNodes; j++)
			if(gra[i][j]>0.0)
			{
				node->neighbor[k] = getNodepointer(retnode, j+1);//j+1是邻居的id号，抓取这个邻居的地址
				node->neibID[k] = j+1;
				node->weight[k] = gra[i][j];
				node->neibInfln[k] = 0;
				k++;
			}
    /*
		sum = 0.0;
		for(k=0; k<count; k++)
			sum += node->weight[k];
		node->sumweight = sum; */
//		node->thrhld = 0.0;

		node = node->next;
	}

	//++++whereNeibPutMe+++++
	/*
	node = retnode;
	for(i=1; i<=NumNodes; i++)
	{
		for(j=0; j<node->numNeib; j++)
		{
			for(k=0; k<node->neighbor[j]->numNeib; k++)
				if(node->neighbor[j]->neibID[k] == i)
				{
					node->whereNeibPutMe[j] = k;
					break;
				}
		}
		node = node->next;
	}*/
	//+++++++++

	return retnode;
}

//一个主体首次破产，网络传播过程
void influenceOne(Node graphnodes, int id)
{
	Node infln_node, neib_node;
	int i,j;

	infln_node = getNodepointer(graphnodes, id);
	if(infln_node->infln == 1)
		printf("ddnError: Have been influenced!\n");
	infln_node->infln = 1;

	for(i=0; i<infln_node->numNeib; i++)
	{
		neib_node = infln_node->neighbor[i];
		j = infln_node->whereNeibPutMe[i];
		neib_node->neibInfln[j] = 1;

		neib_node->thrhld += neib_node->weight[j] / neib_node->sumweight;	//printf("\nsum=%f\n", neib_node->thrhld);
	}
}

//过个主体首次同时破产，网络传播过程


/*-------------------5/18成功执行并打印---------------------*/
//修改中……
//double initializeGraph()
double **initializeGraph()
{
	double **gra;
	//int i,j,t=0;
	printf("1.initializing Graph...\n");
	gra = initial_graph();
	connection_remedy(gra);

	printf("1.initializeGraph is done\n");


	return gra;
/*
	printf("print out the graph\n");

	for(i=0; i<NumNodes; i++){
		for(j=0; j<NumNodes; j++,t++)
			printf("%4.2f\t",gra[i][j]);
		if(t%5==0){
			printf("\n");}

	}
*/
}












/*
Node ori_graph;

double **dissim_graph;
int dissim_type = DISSIMTYPE;




Node newnode()  //新建节点
{
	Node node;

	node = (Node)malloc(sizeof(*node));

	node->id = 0;
	node->numNeib = 0;
	node->neighbor = NULL;
	node->neibID = NULL;
	node->weight = NULL;
	node->sumweight = 0.0;
	//+++++++++
	node->whereNeibPutMe = NULL;
	//+++++++++
	node->infln = 0;
	node->neibInfln = NULL;
	node->thrhld = 0.0;
	node->next = NULL;

	return node;
}

Node getNodepointer(Node graphnodes, int id)
{
	Node node;
	int i;

	node = graphnodes;

//	if(id>1)
	for(i=1; i<id; i++)
		node = node->next;

	return node;
}



void influenceOne(Node graphnodes, int id)
{
	Node infln_node, neib_node;
	int i,j;

	infln_node = getNodepointer(graphnodes, id);
	if(infln_node->infln == 1)
		printf("ddnError: Have been influenced!\n");
	infln_node->infln = 1;

	for(i=0; i<infln_node->numNeib; i++)
	{
		neib_node = infln_node->neighbor[i];
		j = infln_node->whereNeibPutMe[i];
		neib_node->neibInfln[j] = 1;

		neib_node->thrhld += neib_node->weight[j] / neib_node->sumweight;	//printf("\nsum=%f\n", neib_node->thrhld);
	}
}


void destroy_graph_nodes(Node graphnodes)
{
	Node node, temp_node;
	int i;

	temp_node = graphnodes;
	for(i=0; i<NumNodes; i++)
	{
		free(temp_node->neighbor);
		free(temp_node->neibID);
		free(temp_node->weight);
		free(temp_node->neibInfln);
		//++++++++
		free(temp_node->whereNeibPutMe);
		//++++++++
		temp_node = temp_node->next;
	}

	node = graphnodes;
	temp_node = node;
	for(i=0; i<NumNodes; i++)
	{
		node = temp_node->next;
		free(temp_node);
		temp_node = node;
	}
}

Node copynode(Node ori_node)	// do not copy "->next" or "->neighbor[i]"
{
	Node node;
	int i;

	node = newnode();

	node->id = ori_node->id;
	node->numNeib = ori_node->numNeib;
	node->sumweight = ori_node->sumweight;
	node->infln = ori_node->infln;
	node->thrhld = ori_node->thrhld;

	node->neighbor = (Node*)calloc(node->numNeib, sizeof(Node));
	node->neibID = (int*)calloc(node->numNeib, sizeof(int));
	node->weight = (double*)calloc(node->numNeib, sizeof(double));
	node->neibInfln = (int*)calloc(node->numNeib, sizeof(int));

	node->whereNeibPutMe = (int*)calloc(node->numNeib, sizeof(int));

	for(i=0; i<node->numNeib; i++)
	{
//		node->neighbor[i] = ori_node->neighbor[i];
		node->neibID[i] = ori_node->neibID[i];
		node->weight[i] = ori_node->weight[i];
		node->neibInfln[i] = ori_node->neibInfln[i];

		node->whereNeibPutMe[i] = ori_node->whereNeibPutMe[i];
	}

	return node;
}

Node copy_graph_nodes(Node graphnodes)
{
	Node retgraphnodes, node, ori_node, old_node;
	int i;

	ori_node = graphnodes;
	retgraphnodes = copynode(ori_node);
	node = retgraphnodes;
	old_node = node;

	ori_node = ori_node->next;

	while(ori_node != NULL)
	{
		node = copynode(ori_node);
		old_node->next = node;
		old_node = node;
		ori_node = ori_node->next;
	}

	node = retgraphnodes;
	while(node != NULL)
	{
		for(i=0; i<node->numNeib; i++)
			node->neighbor[i] = getNodepointer(retgraphnodes, node->neibID[i]);
		node = node->next;
	}

	return retgraphnodes;
}

double dissimilarity(int dtype, double **gra, int i, int j) //定义权重方法，将权重（边的数量）转换成节点对临节点的影响作用
{
	double sum;
	int k;

	if(gra[i][j]==0.0)
		return( (double)LARGE_NUM );
	if(dtype == 1)
		return( 1/gra[i][j] );
	if(dtype == 2)
	{
		sum = 0.0;
		for(k=0; k<NumNodes; k++)
			sum += gra[k][j];
		return( sum/gra[i][j] );
	}
	if(dtype == 3)
		return( UPPER_WEIGHT -gra[i][j] );

	printf("Wrong Dissimilarity Type!\n");
	return(-1*(double)LARGE_NUM);
}


double **initial_dissim_graph( double **gra, int dtype )
{
	double **gra_temp;
	int i,j;

	if(dtype==0)  return NULL;

	gra_temp = (double**) calloc( NumNodes, sizeof(*gra_temp) );
	for(i=0; i<NumNodes; i++)
		gra_temp[i] = (double*) calloc( NumNodes, sizeof(**gra_temp) );


	for(i=0; i<NumNodes; i++)
		for(j=0; j<NumNodes; j++)
			gra_temp[i][j] = dissimilarity(dtype, gra, i, j);

	for(i=0; i<NumNodes; i++)
		gra_temp[i][i] = 0.0;

	return gra_temp;
}





//	#ifdef RECORD_GRAPH
//	record_graph(gra);
//	#endif

//	ori_graph = transform_graph(gra);

//	dissim_graph = initial_dissim_graph(gra, dissim_type);

//	destroy_graph(gra);
}


void loadGraph()
{
	FILE *file;
	int i,j;
	double **gra;

	gra = (double**) calloc( NumNodes, sizeof(*gra) );
	for(i=0; i<NumNodes; i++)
		gra[i] = (double*) calloc( NumNodes, sizeof(**gra) );

	file = fopen("recordGraph", "r");

	for(i=0; i<NumNodes; i++)
		for(j=0; j<NumNodes; j++)
			fscanf(file, "  %lf", &gra[i][j]);

	fclose(file);

	ori_graph = transform_graph(gra);

	dissim_graph = initial_dissim_graph(gra, dissim_type);

	destroy_graph(gra);
}

*/
