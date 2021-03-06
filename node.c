/*@Author: Xiaowei Li*/
/*@Copyright belong to Xiaowei Li and Yiyun Chen*/

#define _GNU_SOURCE
#include "node.h"
#include "param.h"
#include "random.h"
#include "matrixCalculator.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printGraph(Node **node_array)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < NUMNODES; i++) {
        Node *node = node_array[i];
        printf("%d\n", node->id);
        printf("%f\n", node->origin_mV);
        printf("%f\n", node->mV);
        printf("%d\n", node->infln);
        printf("%f\n", node->beta);
        printf("%f\n", node->bailout);
        printf("%d\n", node->numNeib);
        for(j=0;j<node->numNeib-1;j++){
            printf("%d,",node->neibIDs[j]);
        }
        printf("%d\n",node->neibIDs[j]);
        for(j=0;j<node->numNeib-1;j++){
            printf("%f,",node->weightA[j]); 
        }
        printf("%f\n",node->weightA[j]);
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
        node->infln=0;
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

Node** readGraph(){
    FILE *fp;
    char *line=NULL;
    size_t len=0;
    int i=0;
    int j=0;
    const char s[2]=",";
    Node **node_array = (Node **)malloc(NUMNODES * sizeof(Node *));
    
    fp=fopen("graph.cfg","r");
    if(fp==NULL) exit(EXIT_FAILURE);

    for(i=0;i<NUMNODES;i++){
        Node *node = (Node *)malloc(sizeof(Node));
        //get id
        getline(&line, &len, fp);
        node->id=atoi(line);
        //get originmv
        getline(&line,&len,fp);
        node->origin_mV=atof(line);
        //get mv
        getline(&line,&len,fp);
        node->mV=atof(line);
        //get infln
        getline(&line,&len,fp);
        node->infln=0;
        //get beta
        getline(&line,&len,fp);
        node->beta=atof(line);
        //get bailout
        getline(&line,&len,fp);
        node->bailout=atof(line);
        //get numNeib
        getline(&line, &len, fp);
        node->numNeib=atoi(line);
        //get neibIDs
        node->neibIDs = (int *)malloc(node->numNeib * sizeof(int));
        getline(&line,&len,fp);
        node->neibIDs[0]=atoi(strtok(line,s));
        for(j=1;j<node->numNeib;j++){
           node->neibIDs[j]=atoi(strtok(NULL,s));
        }
        //get weightA   
        node->weightA = (double *)malloc(node->numNeib * sizeof(double));
        getline(&line,&len,fp);
        node->weightA[0]=atof(strtok(line,s));
        for(j=1;j<node->numNeib;j++){
           node->weightA[j]=atof(strtok(NULL,s));
        }
        node_array[i]=node;
    }

    //give value to **neighbors field
    for (i = 0; i < NUMNODES; i++) {
        Node *node = node_array[i];
        node->neighbors = (Node **)malloc((node->numNeib) * sizeof(Node *));   
        for(j=0; j<node->numNeib;j++){
            node->neighbors[j]=node_array[node->neibIDs[j]];
        }
    }

    fclose(fp);
    if(line) free(line);
    return node_array;
}
