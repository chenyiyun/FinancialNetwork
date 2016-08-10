#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "param.h"
#include "random.h"
#include "node.h"
#include "matrixCalculator.h"

#define ThrHld  0.1
#define _CRT_SECURE_NO_DEPRECATE 1
#define RECORD_GRAPH

void initial_graph();
void printmatrix();
void printGraph(Node **node_array);
Node **transform_graph();
Node *newnode();



Node ori_graph;
double gra[NUMNODES][NUMNODES];
// Two global variables to store address of front and rear nodes.

/*initalize the graph*/
void initial_graph()
{
    int i, j;

    /*initialize the number in 2d array to 0.0*/
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
            gra[i][j] = 0.0;

    /*generate the connection matrix*/
    for (i = 0; i < NUMNODES - 1; i++)
        for (j = i + 1; j < NUMNODES; j++)
            if (check_threshold(CONNECT_PARAM) ) {
                gra[i][j] = 1.0;
                gra[j][i] = 1.0;
            } else {
                gra[i][j] = 0.0;
                gra[j][i] = 0.0;
            }

    /*intilialize the weight*/
    for (i = 0; i < NUMNODES - 1; i++)
        for (j = i + 1; j < NUMNODES; j++)
        {
            if (gra[i][j] == 1.0) {
                gra[i][j] = generate_weight();
                gra[j][i] = generate_weight();
            }
        }
    printf("initial_graph is done.\n");
}


void printmatrix()
{
    int i = 0;
    int j = 0;
    printf("matrix are show below\n");
    for (i = 0; i < NUMNODES; i++) {
        for (j = 0; j < NUMNODES; j++) {
            printf("%f, ", gra[i][j]);
        }
        printf("\n");
    }
}


void printGraph(Node **node_array)
{
    int i = 0, j = 0;
    for (i = 0; i < NUMNODES; i++) {
        Node *node = node_array[i];
        printf("********************Node %d *******************\n", i);
        printf("mV is %f,", node->mV);
        printf(" infln is %d,", node->infln);
        printf(" beta is %f,", node->beta);
        printf(" numNeib is %d\n", node->numNeib);
        for (j = 0; j < (node->numNeib); j++) {
            printf("neighbors' ids are %d", node->neibIDs[j]);
            printf("  this neighbor weight is %f", node->weightA[j]);
            printf("  neibInfln is %d\n", node->neibInfln[j]);
        }
    }
}


int main() {
    initial_graph();
    printmatrix();
    Node **node_array = transform_graph();
    printGraph(node_array);
    return 0;
}


// //done
// void destroy_graph( double **gra )//Çå¿ÕÍøÂç
// {
//     int i;
//     for (i = 0; i < NUMNODES; i++)
//         free(gra[i]);
//     free(gra);
// }

// /*-------------------5/18³É¹¦Ö´ÐÐ²¢´òÓ¡---------------------*/
// void connection_remedy( double **gra )//Á¬Í¨¶È²¹¾È£¬Èç¹û·¢ÏÖÍøÂç²»Á¬Í¨£¬ÓÃ´Ëfunction
// {
//     int i, j, k, m, goon = 1;
//     int x[NUMNODES], y[NUMNODES];

//     printf("Remedy connectivity...");
//     for (i = 0; i < NUMNODES; i++)
//     {
//         x[i] = 0;
//         y[i] = 0;
//     }

//     x[0] = 1;

//     while (goon == 1)
//     {
//         goon = 0;
//         for (i = 0; i < NUMNODES; i++)
//             if (x[i] == 1 && y[i] == 0)
//             {
//                 goon = 1;
//                 y[i] = 1;
//                 for (j = 0; j < NUMNODES; j++)
//                     if (gra[i][j] > 0)
//                         x[j] = 1;
//             }
//     }

//     for (m = 0; m < NUMNODES; m++)
//         if (x[m] == 0)
//         {
//             k = rand() % 2;

//             if (k == 0)
//                 gra[m][m - 1] = 0 - generate_weight();
//             else
//                 gra[m][m - 1] = generate_weight();

//             gra[m - 1][m] = 0 - gra[m][m - 1];


//             x[m] = 1;
//             goon = 1;
//             while (goon == 1)
//             {
//                 goon = 0;
//                 for (i = 0; i < NUMNODES; i++)
//                     if (x[i] == 1 && y[i] == 0)
//                     {
//                         goon = 1;
//                         y[i] = 1;
//                         for (j = 0; j < NUMNODES; j++)
//                             if (gra[i][j] > 0)
//                                 x[j] = 1;
//                     }
//             }
//         }

//     printf("connection_remedy is done.\n");
// }

//done
// void record_graph(double **gra) //½«³õÊ¼ºÃµÄÍøÂçÂ¼ÈëÎÄ¼þ£¬ÊÍ·ÅÄÚ´æ
// {
//     FILE *file;
//     int i, j;

//     file = fopen("recordGraph", "w");

//     for (i = 0; i < NUMNODES; i++)
//     {
//         for (j = 0; j < NUMNODES; j++)
//             fprintf(file, "  %lf", gra[i][j]);
//         fprintf(file, "\n");
//     }

//     fclose(file);
// }

//done
Node *newnode()
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->id = 0;
    node->mV = 0.0;
    node->infln = 0;
    node->beta = 0.0;
    node->numNeib = 0;
    node->neighbors = NULL;
    node->neibIDs = NULL;
    node->weightA = NULL;
    node->neibInfln = NULL;
    node->next = NULL;

    return node;
}

// //done
// Node getNodepointer(Node graphnodes, int id)//¸ù¾ÝÁ´±íÍ·½áµãgraphnodesÕÒµ½¾ßÌåidµÄ½áµã
// {
//     Node node;
//     int i;

//     node = graphnodes;

// //  if(id>1)
//     for (i = 1; i < id; i++)
//         node = node->next;

//     return node;
// }

/*--------------transform from graph to linked list----------------*/
Node **transform_graph()
{
    //head pointer for linkedlist
    Node *head, *pre, *cur;
    int i = 0, j = 0, k = 0;
    double mv_array[NUMNODES];
    double bv_array[NUMNODES];
    int count = 0;

    Node **node_array = (Node **)malloc(NUMNODES * sizeof(Node));

    //get Nodes' mv
    Matrix_mV(gra, bv_array, mv_array);


    for (i = 0; i < NUMNODES; i++) {
        Node *node = newnode();
        count=0;
        k=0;
        node->id = i;
        node->mV = mv_array[i];
        //iterate through gra matrix to get size of neighbors firstly
        for (j = 0; j < NUMNODES; j++) {
            //connection exists.
            //incoming node is the neighbor of this node
            if (gra[j][i] != 0) {
                count++;
            }
        }

        node->numNeib = count;
        //malloc memory for fixed size of neighbors
        node->neibIDs = (int *)malloc(count * sizeof(int));
        node->weightA = (double *)malloc(count * sizeof(double));
        node->neibInfln = (int *)malloc(count * sizeof(int));
        //give values to this two memory block
        for (j = 0; j < NUMNODES; j++) {
            if (gra[j][i] != 0) {
                node->neibIDs[k] = j;
                node->weightA[k] = gra[j][i];
                node->neibInfln[k]=0;
                k++;
            }
        }

        node_array[i] = node;
    }

    //give value to neighbors field
    for (i = 0; i < NUMNODES; i++) {
        k = 0;
        Node *node = node_array[i];

        for (j = 0; j < NUMNODES; j++) {
            //connection exists.
            //incoming node is the neighbor of this node
            if (gra[j][i] != 0) {
                count++;
            }
        }
        node->neighbors = (Node **)malloc(count * sizeof(Node *));
        for (j = 0; j < NUMNODES; j++) {
            if (gra[j][i] != 0) {
                node->neighbors[k] = node_array[j];
                k++;
            }
        }
    }


    return node_array;
}

// /*------------¶¨Òå¶ÓÁÐº¯Êý£¬Èë¶Ó£¬³ö¶Ó----------------*/
// // To Enqueue an integer
// void Enqueue(Node inflnNode) {//Èë¶Ó
//     QNode* front;
//     QNode* rear;
//     QNode* temp;
//     //temp = (QNode*)calloc( NUMNODES, sizeof(QNode));

//     (*temp)->Quenode = inflnNode;
//     (*temp)->next = NULL;
//     if (front == NULL && rear == NULL) {
//         front = rear = temp;
//         //return;
//     }
//     (*rear)->next = temp;
//     (*rear) = temp;
// }

// // To Dequeue an integer.
// void Dequeue() {//³ö¶Ó
//     QNode* front;
//     QNode* rear;
//     QNode* temp = front;
//     QNode* pretemp;
//     if (front == NULL) {
//         printf("Queue is Empty\n");
//         //return;
//     }
//     if (front == rear) {
//         front = rear = NULL;
//     }
//     else {
//         pretemp = temp;
//         temp = (*temp)->next;
//     }
//     free(pretemp);
// }
// /*
// int Front() {
//     if(front == NULL) {
//         printf("Queue is empty\n");
//         return;
//     }
//     return front->data;
// }
// */
// /*
// void Print() {
//     struct QNode* temp = front;
//     while(temp != NULL) {
//         printf("%d ",temp->data);
//         temp = temp->next;
//     }
//     printf("\n");
// }
// */
// //7.4ÒÔÏÂÊÇÒ»¸ö²»·µ»ØÈÎºÎÖµµÄ¹ý³Ì£¬ÒÑÖªÆðÊ¼ÆÆ²úµãµÄID£¬ÇóÓÉËüÒýÆðµÄÆÆ²úËùµÃµ½µÄÐÂµÄÍøÂç×´Ì¬
// int influenceOne(Node graphnodes, int id)
// {
//     Node infln_node, neib_node;
//     QNode* front = NULL;
//     QNode* rear = NULL;
//     QNode* temp;
//     int i, j;
//     int count = 0;
//     double tempThrhld;

//     infln_node = getNodepointer(graphnodes, id);//»ñÈ¡ÆðÊ¼µã
//     //if(infln_node->infln == 1)
//     //  printf("ddnError: Have been influenced!\n");
//     infln_node->infln = 1;
//     count++;
//     printf("¼ÆÊý£º%d\n", count);
//     infln_node->beta = (infln_node->mV) * (1 - ThrHld); //¼ÆËãÆÆ²ú³É±¾


//     for (i = 0; i < infln_node->numNeib; i++) //±éÀúÁÚ¾Ó,½«ÆÆ²úµÄÁÚ¾ÓÈë¶Ó
//     {
//         neib_node = infln_node->neighbor[i];
//         //j = infln_node->whereNeibPutMe[i];
//         //neib_node->neibInfln[j] = 1;
//         tempThrhld = ThrHld * (neib_node->mV);
//         neib_node->mV -= infln_node->beta;
//         if (neib_node->mV < tempThrhld)
//         {
//             neib_node->infln = 1;
//             infln_node->neibInfln[i] = 1;
//             Enqueue(neib_node);//ÆÆ²úµÄÁÚ¾Ó½ÚµãÈë¶Ó
//         } else
//         {
//             neib_node->infln = 0;
//             infln_node->neibInfln[i] = 0;
//         }
//     }
//     while (front > rear)
//     {
//         temp = front; //¿ªÊ¼´¦Àí¶ÓÁÐÖÐÍ·½áµã£¬¸³ÖµÎªtemp
//         for (j = 0; j < ((*(*temp)->Quenode)->numNeib); j++) //¶Ôtemp½áµãµÄÁÚ¾Ó±éÀú²¢ÅÐ¶ÏÊÇ·ñÆÆ²ú£¬·ÅÈë¶ÓÁÐÖÐ£¬Ê×ÏÈÅÐ¶Ï
//         {
//             if ((*(*temp)->Quenode)->neibInfln[j] == 0)
//             {
//                 tempThrhld = ThrHld * ((*(*temp)->Quenode)->neighbor[j]->mV);
//                 (*(*temp)->Quenode)->neighbor[j]->mV -= (*(*temp)->Quenode)->beta;
//                 if ((*(*temp)->Quenode)->neighbor[j]->mV < tempThrhld)
//                 {
//                     (*(*temp)->Quenode)->neibInfln[j] = 1;
//                     count++;
//                     printf("¼ÆÊý£º%d\n", count);
//                     Enqueue((*(*temp)->Quenode)->neighbor[j]);
//                 }
//             } else continue;
//             temp = (*front)->next;
//             Dequeue;//É¾È¥Í·½áµã

//         }

//     }
//     return count;
//     printf("×îÖÕ¼ÆÊý£º%d\n", count);
// }

// /*----6.7ÍøÂç´«²¥¹ý³Ì£¬one by one£¬·µ»ØÒ»¸öÊý×é£¬Ã¿¸öÊý×é´ú±íÒ»¸ö½áµãÆÆ²úµ¼ÖÂµÄ×ÜÌåÆÆ²ú¸öÊý------------*/
// /*
// void influenceOne(Node graphnodes, int failNum[NUMNODES])//ÓÉÒ»¸ö³õÊ¼½ÚµãÆÆ²úµ¼ÖÂµÄ£¨·µ»ØµÄ£©ÕûÌåÆÆ²úÊýÁ¿
// {
//     Node infln_node, neib_node;
//     QNode* front = NULL;
//     QNode* rear = NULL;
//     QNode* temp;
//     int i,j;
//     int count;

//     for(i=0;i<NUMNODES;i++)
//     {
//         count=0;
//         infln_node = getNodepointer(graphnodes,i+1);//ÕÒµ½Ê×´ÎÆÆ²úµÄÖ÷Ìå½áµã
//         //infln_node->infln = 1;//ÔÚº¯ÊýÍâµÄ±ê×¢±¾½áµãÒÑÆÆ²ú
//         infln_node->infln=1;
//         count++;
//         infln_node->beta = (infln_node->mV)*(1-ThrHld);//¼ÆËãÆÆ²ú³É±¾
//         infln_node->mV *= ThrHld;//ÆÆ²úºóÖ÷ÌåµÄÊÐ³¡¼ÛÖµ
//         //Enqueue(infln_node);//³õÊ¼ÆÆ²úµÄ½áµã·ÅÈë¶ÓÁÐÖÐ

//         for(j=0;j<infln_node->numNeib;j++)
//         {

//         }
//         while(front>rear)
//         {
//             temp =front;//¶ÓÁÐÖÐÍ·½áµãµÄÁÚ¾Ó±éÀú
//             Dequeue;
//             for(j=0;j<temp->Quenode->numNeib;j++)
//             {
//                 temp->Quenode->neighbor[j]->mV=temp->Quenode->weight[j]*infln_node->beta;
//                 if(temp->Quenode->neighbor[j]->mV<=(temp->Quenode->neighbor[j]->mV)*ThrHld)
//                 {
//                     temp->Quenode->neibInfln[j]=1;
//                     count++;
//                     Enqueue(temp->Quenode->neighbor[j]);
//                 }else
//                 {
//                 //blablabla
//                 }
//             }
//         }
//         failNum[i]=count;
//     }
// }
// */

// /*---to do list,¸ø³öÒ»Èº³õÊ¼ÆÆ²úµÄµãµÄ¼¯ºÏ£¬ÅÐ¶ÏÓÉÆäÒýÆðµÄ×ÜÌåÆÆ²ú¸öÊý-------*/
// /*
// void influenceOne(Node graphnodes, int id)
// {
//     Node infln_node, neib_node;
//     int i,j;

//     infln_node = getNodepointer(graphnodes, id);//ÕÒµ½Ê×´ÎÆÆ²úµÄÖ÷Ìå½áµã


//     if(infln_node->infln == 1)
//         printf("ddnError: Have been influenced!\n");
//     infln_node->infln = 1;

//     for(i=0; i<infln_node->numNeib; i++)
//     {
//         neib_node = infln_node->neighbor[i];
//         j = infln_node->whereNeibPutMe[i];
//         neib_node->neibInfln[j] = 1;

//         neib_node->thrhld += neib_node->weight[j] / neib_node->sumweight;   //printf("\nsum=%f\n", neib_node->thrhld);
//     }
// }
// */
// //¶à¸öÖ÷ÌåÊ×´ÎÍ¬Ê±ÆÆ²ú£¬ÍøÂç´«²¥¹ý³Ì


// /*-------------------5/18³É¹¦Ö´ÐÐ²¢´òÓ¡---------------------*/
// //ÐÞ¸ÄÖÐ¡­¡­
// //double initializeGraph()
// double **initializeGraph()
// {
//     double **gra;
//     //int i,j,t=0;
//     printf("1.initializing Graph...\n");
//     gra = initial_graph();
//     connection_remedy(gra);

//     printf("1.initializeGraph is done\n");


//     return gra;
//     /*
//         printf("print out the graph\n");

//         for(i=0; i<NUMNODES; i++){
//             for(j=0; j<NUMNODES; j++,t++)
//                 printf("%4.2f\t",gra[i][j]);
//             if(t%5==0){
//                 printf("\n");}

//         }
//     */
// }












// /*
// Node ori_graph;

// double **dissim_graph;
// int dissim_type = DISSIMTYPE;




// Node newnode()  //ÐÂ½¨½Úµã
// {
//     Node node;

//     node = (Node)malloc(sizeof(*node));

//     node->id = 0;
//     node->numNeib = 0;
//     node->neighbor = NULL;
//     node->neibID = NULL;
//     node->weight = NULL;
//     node->sumweight = 0.0;
//     //+++++++++
//     node->whereNeibPutMe = NULL;
//     //+++++++++
//     node->infln = 0;
//     node->neibInfln = NULL;
//     node->thrhld = 0.0;
//     node->next = NULL;

//     return node;
// }

// Node getNodepointer(Node graphnodes, int id)
// {
//     Node node;
//     int i;

//     node = graphnodes;

// //  if(id>1)
//     for(i=1; i<id; i++)
//         node = node->next;

//     return node;
// }



// void influenceOne(Node graphnodes, int id)
// {
//     Node infln_node, neib_node;
//     int i,j;

//     infln_node = getNodepointer(graphnodes, id);
//     if(infln_node->infln == 1)
//         printf("ddnError: Have been influenced!\n");
//     infln_node->infln = 1;

//     for(i=0; i<infln_node->numNeib; i++)
//     {
//         neib_node = infln_node->neighbor[i];
//         j = infln_node->whereNeibPutMe[i];
//         neib_node->neibInfln[j] = 1;

//         neib_node->thrhld += neib_node->weight[j] / neib_node->sumweight;   //printf("\nsum=%f\n", neib_node->thrhld);
//     }
// }


// void destroy_graph_nodes(Node graphnodes)
// {
//     Node node, temp_node;
//     int i;

//     temp_node = graphnodes;
//     for(i=0; i<NUMNODES; i++)
//     {
//         free(temp_node->neighbor);
//         free(temp_node->neibID);
//         free(temp_node->weight);
//         free(temp_node->neibInfln);
//         //++++++++
//         free(temp_node->whereNeibPutMe);
//         //++++++++
//         temp_node = temp_node->next;
//     }

//     node = graphnodes;
//     temp_node = node;
//     for(i=0; i<NUMNODES; i++)
//     {
//         node = temp_node->next;
//         free(temp_node);
//         temp_node = node;
//     }
// }

// Node copynode(Node ori_node)    // do not copy "->next" or "->neighbor[i]"
// {
//     Node node;
//     int i;

//     node = newnode();

//     node->id = ori_node->id;
//     node->numNeib = ori_node->numNeib;
//     node->sumweight = ori_node->sumweight;
//     node->infln = ori_node->infln;
//     node->thrhld = ori_node->thrhld;

//     node->neighbor = (Node*)calloc(node->numNeib, sizeof(Node));
//     node->neibID = (int*)calloc(node->numNeib, sizeof(int));
//     node->weight = (double*)calloc(node->numNeib, sizeof(double));
//     node->neibInfln = (int*)calloc(node->numNeib, sizeof(int));

//     node->whereNeibPutMe = (int*)calloc(node->numNeib, sizeof(int));

//     for(i=0; i<node->numNeib; i++)
//     {
// //      node->neighbor[i] = ori_node->neighbor[i];
//         node->neibID[i] = ori_node->neibID[i];
//         node->weight[i] = ori_node->weight[i];
//         node->neibInfln[i] = ori_node->neibInfln[i];

//         node->whereNeibPutMe[i] = ori_node->whereNeibPutMe[i];
//     }

//     return node;
// }

// Node copy_graph_nodes(Node graphnodes)
// {
//     Node retgraphnodes, node, ori_node, old_node;
//     int i;

//     ori_node = graphnodes;
//     retgraphnodes = copynode(ori_node);
//     node = retgraphnodes;
//     old_node = node;

//     ori_node = ori_node->next;

//     while(ori_node != NULL)
//     {
//         node = copynode(ori_node);
//         old_node->next = node;
//         old_node = node;
//         ori_node = ori_node->next;
//     }

//     node = retgraphnodes;
//     while(node != NULL)
//     {
//         for(i=0; i<node->numNeib; i++)
//             node->neighbor[i] = getNodepointer(retgraphnodes, node->neibID[i]);
//         node = node->next;
//     }

//     return retgraphnodes;
// }

// double dissimilarity(int dtype, double **gra, int i, int j) //¶¨ÒåÈ¨ÖØ·½·¨£¬½«È¨ÖØ£¨±ßµÄÊýÁ¿£©×ª»»³É½Úµã¶ÔÁÙ½ÚµãµÄÓ°Ïì×÷ÓÃ
// {
//     double sum;
//     int k;

//     if(gra[i][j]==0.0)
//         return( (double)LARGE_NUM );
//     if(dtype == 1)
//         return( 1/gra[i][j] );
//     if(dtype == 2)
//     {
//         sum = 0.0;
//         for(k=0; k<NUMNODES; k++)
//             sum += gra[k][j];
//         return( sum/gra[i][j] );
//     }
//     if(dtype == 3)
//         return( UPPER_WEIGHT -gra[i][j] );

//     printf("Wrong Dissimilarity Type!\n");
//     return(-1*(double)LARGE_NUM);
// }


// double **initial_dissim_graph( double **gra, int dtype )
// {
//     double **gra_temp;
//     int i,j;

//     if(dtype==0)  return NULL;

//     gra_temp = (double**) calloc( NUMNODES, sizeof(*gra_temp) );
//     for(i=0; i<NUMNODES; i++)
//         gra_temp[i] = (double*) calloc( NUMNODES, sizeof(**gra_temp) );


//     for(i=0; i<NUMNODES; i++)
//         for(j=0; j<NUMNODES; j++)
//             gra_temp[i][j] = dissimilarity(dtype, gra, i, j);

//     for(i=0; i<NUMNODES; i++)
//         gra_temp[i][i] = 0.0;

//     return gra_temp;
// }





// //  #ifdef RECORD_GRAPH
// //  record_graph(gra);
// //  #endif

// //  ori_graph = transform_graph(gra);

// //  dissim_graph = initial_dissim_graph(gra, dissim_type);

// //  destroy_graph(gra);
// }


// void loadGraph()
// {
//     FILE *file;
//     int i,j;
//     double **gra;

//     gra = (double**) calloc( NUMNODES, sizeof(*gra) );
//     for(i=0; i<NUMNODES; i++)
//         gra[i] = (double*) calloc( NUMNODES, sizeof(**gra) );

//     file = fopen("recordGraph", "r");

//     for(i=0; i<NUMNODES; i++)
//         for(j=0; j<NUMNODES; j++)
//             fscanf(file, "  %lf", &gra[i][j]);

//     fclose(file);

//     ori_graph = transform_graph(gra);

//     dissim_graph = initial_dissim_graph(gra, dissim_type);

//     destroy_graph(gra);
// }

//*/
