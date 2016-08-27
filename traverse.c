#include "traverse.h"


void single_traverse(Node *initial_node)
{
    init_queue();
    offer(initial_node);
    initial_node->beta = (1 - ThrHld) * (initial_node->mV);
    initial_node->mV = ThrHld * (initial_node->mV);
    initial_node->infln = 1;

    //loop until queue is empty;
    while (!is_empty()) {
        Node *node = pop();
        //if node is bankrupt, then influence other nodes
        if (node->infln) {
            int i = 0;
            Node **neighbors = node->neighbors;
            for (i = 0; i < node->numNeib; i++) {
                Node *neighbor_node = neighbors[i];
                if (neighbor_node->infln) continue;
                //calculate the new value
                neighbor_node->mV = (neighbor_node->mV) - (node->beta) * (node->weightA[i]);
                //if it trigger the condition, mark this node as bankrupt
                if (neighbor_node->mV <= ThrHld * neighbor_node->origin_mV) {
                    neighbor_node->infln = 1;
                    if (neighbor_node->mV < 0) {
                        neighbor_node->beta = neighbor_node->origin_mV;
                    } else {
                        neighbor_node->beta = neighbor_node->origin_mV - neighbor_node->mV;
                    }
                    //printf("node beta is %f\n", neighbor_node->beta);
                    //offer this node into queue.
                    offer(neighbor_node);
                }
            }
        }
    }
}

int multiple_traverse(Node **initial_nodes, int *array, int nums)
{
    init_queue();
    int i = 0;
    int count = 0;

    for (i = 0; i < nums; i++) {
        if (array[i]) {
            Node *initial_node = initial_nodes[i];
            offer(initial_node);
            initial_node->beta = (1 - ThrHld) * (initial_node->mV);
            initial_node->mV = ThrHld * (initial_node->mV);
            initial_node->infln = 1;
            count++;
        }
    }

    //loop until queue is empty;
    //this queue only contains node that is bankrupted.
    while (!is_empty()) {
        Node *node = pop();
        //if node is bankrupt, then influence other nodes
        int i = 0;
        Node **neighbors = node->neighbors;
        for (i = 0; i < node->numNeib; i++) {
            Node *neighbor_node = neighbors[i];
            if (neighbor_node->infln) continue;
            //calculate the new value
            neighbor_node->mV = (neighbor_node->mV) - (node->beta) * (node->weightA[i]);
            //if it trigger the condition, mark this node as bankrupt
            if (neighbor_node->mV <= ThrHld * neighbor_node->origin_mV) {
                neighbor_node->infln = 1;
                count++;
                //maintain the bankrupt cost, this is the base to influence others
                if (neighbor_node->mV < 0) {
                    neighbor_node->beta = neighbor_node->origin_mV;
                } else {
                    neighbor_node->beta = neighbor_node->origin_mV - neighbor_node->mV;
                }
                //printf("node beta is %f\n", neighbor_node->beta);
                //offer this node into queue.
                offer(neighbor_node);
            }
        }
    }

    return count;
}

int main() {
    int i = 0;

    Node **node_array = (Node **)malloc(6 * sizeof(Node *));

    for (i = 0; i < 6; i++) {
        node_array[i] = newnode();
    }

    /*Node 1*/
    Node *node = node_array[0];
    node->id = 1;
    node->mV = 150;
    node->origin_mV = 150;
    node->numNeib = 2;
    node->neibIDs = (int *)malloc(2 * sizeof(int));
    node->weightA = (double *)malloc(2 * sizeof(double));
    node->neighbors = (Node **)malloc(2 * sizeof(Node *));
    node->neibIDs[0] = 2;
    node->neibIDs[1] = 6;
    node->weightA[0] = 0.3;
    node->weightA[1] = 0.6;
    node->neighbors[0] = node_array[1];
    node->neighbors[1] = node_array[5];

    /*Node 2*/
    node = node_array[1];
    node->id = 2;
    node->mV = 80;
    node->origin_mV = 80;
    node->numNeib = 3;
    node->neibIDs = (int *)malloc(3 * sizeof(int));
    node->weightA = (double *)malloc(3 * sizeof(double));
    node->neighbors = (Node **)malloc(3 * sizeof(Node *));
    node->neibIDs[0] = 1;
    node->neibIDs[1] = 3;
    node->neibIDs[2] = 4;
    node->weightA[0] = 0.01;
    node->weightA[1] = 0.3;
    node->weightA[2] = 0.2;
    node->neighbors[0] = node_array[0];
    node->neighbors[1] = node_array[2];
    node->neighbors[2] = node_array[3];

    /*Node 3*/
    node = node_array[2];
    node->id = 3;
    node->mV = 70;
    node->origin_mV = 70;
    node->numNeib = 2;
    node->neibIDs = (int *)malloc(2 * sizeof(int));
    node->weightA = (double *)malloc(2 * sizeof(double));
    node->neighbors = (Node **)malloc(2 * sizeof(Node *));
    node->neibIDs[0] = 1;
    node->neibIDs[1] = 6;
    node->weightA[0] = 0.15;
    node->weightA[1] = 0.2;
    node->neighbors[0] = node_array[0];
    node->neighbors[1] = node_array[5];

    /*Node 4*/
    node = node_array[3];
    node->id = 4;
    node->mV = 60;
    node->origin_mV = 60;
    node->numNeib = 1;
    node->neibIDs = (int *)malloc(1 * sizeof(int));
    node->weightA = (double *)malloc(1 * sizeof(double));
    node->neighbors = (Node **)malloc(1 * sizeof(Node *));
    node->neibIDs[0] = 5;
    node->weightA[0] = 0.4;
    node->neighbors[0] = node_array[4];

    /*Node 5*/
    node = node_array[4];
    node->id = 5;
    node->mV = 50;
    node->origin_mV = 50;
    node->numNeib = 2;
    node->neibIDs = (int *)malloc(2 * sizeof(int));
    node->weightA = (double *)malloc(2 * sizeof(double));
    node->neighbors = (Node **)malloc(2 * sizeof(Node *));
    node->neibIDs[0] = 2;
    node->neibIDs[1] = 6;
    node->weightA[0] = 0.6;
    node->weightA[1] = 0.3;
    node->neighbors[0] = node_array[1];
    node->neighbors[1] = node_array[5];

    /*Node 6*/
    node = node_array[5];
    node->id = 6;
    node->mV = 90;
    node->origin_mV = 90;
    node->numNeib = 3;
    node->neibIDs = (int *)malloc(3 * sizeof(int));
    node->weightA = (double *)malloc(3 * sizeof(double));
    node->neighbors = (Node **)malloc(3 * sizeof(Node *));
    node->neibIDs[0] = 3;
    node->neibIDs[1] = 4;
    node->neibIDs[2] = 5;
    node->weightA[0] = 0.5;
    node->weightA[1] = 0.7;
    node->weightA[2] = 0.3;
    node->neighbors[0] = node_array[2];
    node->neighbors[1] = node_array[3];
    node->neighbors[2] = node_array[4];


    int bankrupt[6] = {1, 0, 1, 0, 0, 0};
    multiple_traverse(node_array, bankrupt, 6);

    int count = 0;

    for (i = 0; i < 6; i++) {
        if (node_array[i]->infln) {
            printf("company %d bankrupt!\n", i + 1);
            count++;
        }
    }

    printf("the total number of bankrupt companies is %d\n", count);

    return 1;
}

