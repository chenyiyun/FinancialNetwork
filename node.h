#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "param.h"
#include "random.h"
#include "node.h"
#include "matrixCalculator.h"


typedef struct NodeType Node;//declaration of Structtype Node
typedef struct QueueNode *QNode;//declaration of Queue Node

struct NodeType //definition of Structtype Node
{
    //***** node characteristics *****//*
    int id;			  // id of the node
    double origin_mV;        // original market value of the node
    double mV;          //current market value of the node
    int infln;		// 0 or 1, if this node has bankruppted.
    double beta;    //bankcrupting cost, which equals to origin_mV-mV, if mV is negative, then beta equals origin_mV
    double bailout;  //the money the node need to stay away from bankruptcy, which equals theta(in param.h) * origin_mV
    
    /***** neighbor attributes *****/
    
    int numNeib;		// number of neighbors of the node
    Node **neighbors;		// Node pointers of neighbors
    int *neibIDs;		// ids of neighbors
    double *weightA;		// the weights of the neighbors
};



void initial_graph();
void printmatrix(double matrix[NUMNODES][NUMNODES]);
Node **copy_graph(Node **node_array);
void free_graph(Node **node_array);
void printGraph(Node **node_array);
Node *newnode();
Node **transform_graph();
void printGraph(Node **node_array);
void getMatrixA();

#endif
