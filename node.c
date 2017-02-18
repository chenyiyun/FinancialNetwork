#include "node.h"
#include "param.h"
#include "random.h"
#include "matrixCalculator.h"
#include <time.h>

#define _CRT_SECURE_NO_DEPRECATE 1
#define RECORD_GRAPH

//initiate the node of the graph
double gra[NUMNODES][NUMNODES];
// Two global variables to store address of front and rear nodes.

/*initalize the graph*/
void initial_graph()
{
    int i, j;
    srand((unsigned int)time(NULL));
    
    /*initialize the number in 2d array to 0.0*/
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
            gra[i][j] = 0.0;
    
    /*generate the connection matrix*/
    for (i = 0; i < NUMNODES; i++)
        for (j = 0; j < NUMNODES; j++)
            if (check_threshold(CONNECT_PARAM) ) {
                gra[i][j] = 1.0;
            } else {
                gra[i][j] = 0.0;
            }
    
    /*initialize the weight*/
    for (i = 0; i < NUMNODES; i++){
        for (j = 0; j < NUMNODES; j++)
        {
            if (gra[i][j] == 1.0) {
                gra[i][j] = generate_weight();
            }
        }
    }
    
    normalize(gra);
}

void printmatrix(double matrix[NUMNODES][NUMNODES])
{
    int i = 0;
    int j = 0;
    for (i = 0; i < NUMNODES; i++) {
        for (j = 0; j < NUMNODES; j++) {
            printf("%f, ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printmv(double mv_array[NUMNODES]) {
    int i = 0;
    
    for (i = 0; i < NUMNODES; i++) {
        printf("%f, ", mv_array[i]);
    }
    printf("\n");
    
}

void printNeighbor(Node *node) {
    int j = 0;
    for (j = 0; j < (node->numNeib); j++) {
        printf("  id  %d", (node->neighbors[j])->id);
        printf("  this neighbor weight is %f\n", node->weightA[j]);
       
    }
}

void printGraph(Node **node_array)
{
    int i = 0;
    for (i = 0; i < NUMNODES; i++) {
        Node *node = node_array[i];
        printf("****************Node id is %d ***************\n", node->id);
        printf(" beta is %f,", node->beta);
        printf(" bailout money is %f,", node->bailout);
        printf(" numNeib is %d\n", node->numNeib);
        printNeighbor(node);
    }
}


//done
Node *newnode()
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->id = 0;
    node->mV = 0.0;
    node->infln = 0;
    node->origin_mV = 0.0;
    node->beta = 0.0;
    node->bailout = 0.0;
    node->numNeib = 0;
    return node;
}


Node **copy_graph(Node **node_array) {
    int i = 0, j = 0;
    Node **new_node_array = (Node **)malloc(NUMNODES * sizeof(Node *));
    Node **map=(Node **)malloc(NUMNODES * sizeof(Node *));
    for (i = 0; i < NUMNODES; i++) {
        Node *originNode = node_array[i];
        Node *node = (Node *)malloc(sizeof(Node));
        int count = originNode->numNeib;
        node->id = originNode->id;
        node->origin_mV = originNode->origin_mV;
        node->mV = originNode->mV;
        node->beta = originNode->beta;
        node->bailout = originNode->bailout;//Theta is the rate of the bailout money the node needs
        node->numNeib = count;
        node->neibIDs = (int *)malloc(count * sizeof(int));
        node->weightA = (double *)malloc(count * sizeof(double));
        //give values to this two memory block
        for (j = 0; j < count; j++) {
            node->neibIDs[j] = originNode->neibIDs[j];
            node->weightA[j] = originNode->weightA[j];
        }
        
        new_node_array[i] = node;
        map[node->id] = node;
    }
    
    //give value to **neighbors field
    for (i = 0; i < NUMNODES; i++) {
        Node *node = new_node_array[i];
        node->neighbors = (Node **)malloc((node->numNeib) * sizeof(Node *));
        for (j = 0; j < node->numNeib; j++) {
            node->neighbors[j]=map[node->neibIDs[j]];
        }
    }
    free(map);
    //printGraph(new_node_array);
    return new_node_array;
}

void free_graph(Node **node_array) {
    int i = 0;
    for (i = 0; i < NUMNODES; i++) {
        Node *node = node_array[i];
        free(node->neibIDs);
        node->neibIDs=NULL;
        free(node->weightA);
        node->weightA=NULL;
        free(node->neighbors);
        node->neighbors=NULL;
        free(node);
    }
    free(node_array);
}

void getMatrixA(){
    int i,j;
    double matrixA[NUMNODES][NUMNODES];
    double sum[NUMNODES];
    Matrix_WeightA(gra, matrixA);
    for(i=0;i<NUMNODES;i++){
        for(j=0;j<NUMNODES;j++){
           printf("%f ",matrixA[i][j]);
           sum[j]+=matrixA[i][j];
        }
        printf("\n");
    }
    printf("------------------------------------------------------\n");
    for(i=0;i<NUMNODES;i++){
        printf("%f ",sum[i]);
    }
}

/*--------------transform from graph to linked list----------------*/
Node **transform_graph()
{
    int i = 0, j = 0, k = 0;
    double mv_array[NUMNODES];
    double bv_array[NUMNODES];
    double matrixA[NUMNODES][NUMNODES];
    double *D = (double *)malloc(NUMNODES * sizeof(double));
    int count = 0;
    
    Node **node_array = (Node **)malloc(NUMNODES * sizeof(Node *));
    //get Nodes' mv
    generate_matrixD(D);
    Matrix_bV(gra, D, bv_array);
    Matrix_WeightA(gra, matrixA);
    Matrix_mV(matrixA, D, mv_array);
    
    for (i = 0; i < NUMNODES; i++) {
        Node *node = (Node *)malloc(sizeof(Node));
        count = 0;
        k=0;
        node->id = i;
        node->origin_mV = mv_array[i];
        node->mV = mv_array[i];
        //iterate through gra matrix to get size of neighbors firstly
        for (j = 0; j < NUMNODES; j++) {
            if (gra[j][i] != 0) {
                count++;//count the number of the neighbors
            }
        }
        node->beta = node->origin_mV * (1 - ThrHld);
        node->bailout = node->origin_mV * Theta;//Theta is the rate of the bailout money the node needs
        node->numNeib = count;
        //malloc memory for fixed size of neighbors
        node->neibIDs = (int *)malloc(count * sizeof(int));
        node->weightA = (double *)malloc(count * sizeof(double));
        
        //give values to this two memory block
        for (j = 0; j < NUMNODES; j++) {
            if (gra[j][i] != 0) {
                node->neibIDs[k] = j;
                node->weightA[k] = matrixA[j][i];
                k++;
            }
        }
        
        node_array[i] = node;
    }
    //give value to **neighbors field
    for (i = 0; i < NUMNODES; i++) {
        k=0;
        Node *node = node_array[i];
        node->neighbors = (Node **)malloc((node->numNeib) * sizeof(Node *));
        for (j = 0; j < NUMNODES; j++) {
            if (gra[j][i] != 0) {
                node->neighbors[k] = node_array[j];
                k++;
            }
        }
    }
    
    return node_array;
}
