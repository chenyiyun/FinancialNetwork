#include "node.h"

#define _CRT_SECURE_NO_DEPRECATE 1
#define RECORD_GRAPH


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


// int main() {
//     initial_graph();
//     printmatrix();
//     Node **node_array = transform_graph();
//     printGraph(node_array);
//     return 0;
// }


//done
Node *newnode()
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->id = 0;
    node->mV = 0.0;
    node->infln = 0;
    node->origin_mV = 0.0;
    node->beta = 0.0;
    node->numNeib = 0;
    node->neighbors = NULL;
    node->neibIDs = NULL;
    node->weightA = NULL;
    node->neibInfln = NULL;
    node->next = NULL;

    return node;
}


/*--------------transform from graph to linked list----------------*/
Node **transform_graph()
{
    //head pointer for linkedlist
    Node *head, *pre, *cur;
    int i = 0, j = 0, k = 0;
    double mv_array[NUMNODES];
    double bv_array[NUMNODES];
    double weightA[NUMNODES][NUMNODES];
    int count = 0;

    Node **node_array = (Node **)malloc(NUMNODES * sizeof(Node *));

    //get Nodes' mv
    Matrix_mV(gra, bv_array, mv_array);
    Matrix_WeightA(gra, weightA);


    for (i = 0; i < NUMNODES; i++) {
        Node *node = newnode();
        count = 0;
        k = 0;
        node->id = i;
        node->origin_mV = mv_array[i];
        node->mV = mv_array[i];
        //iterate through gra matrix to get size of neighbors firstly
        for (j = 0; j < NUMNODES; j++) {
            //connection exists.
            //incoming node is the neighbor of this node
            if (gra[j][i] != 0) {
                count++;
            }
        }
        node->beta = node->mV * (1 - ThrHld);

        node->numNeib = count;
        //malloc memory for fixed size of neighbors
        node->neibIDs = (int *)malloc(count * sizeof(int));
        node->weightA = (double *)malloc(count * sizeof(double));
        node->neibInfln = (int *)malloc(count * sizeof(int));
        //give values to this two memory block
        for (j = 0; j < NUMNODES; j++) {
            if (gra[j][i] != 0) {
                node->neibIDs[k] = j;
                node->weightA[k] = weightA[j][i];
                node->neibInfln[k] = 0;
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