#include <stdio.h>
#include "Queue.h"


void single_traverse(Node *initial_node) {
    init_queue();
    offer(initial_node);
    initial_node->mV = ThrHld * (head->mV);
    initial_node->neibInfln = 1;

    //loop until queue is empty;
    while (!is_empty()) {
        Node *node = pop();
        //if node is bankrupt, then influence other nodes
        if (node->neibInfln) {
            int i = 0;
            Node **neighbors = node->neighbors;
            for (i = 0; i < node->numNeib; i++) {
                Node *neighbor_node = neighbors[i];
                //calculate the new value
                neighbor_node->mV = (neighbor_node->mV) - (node->beta) * (node->weightA[i]);
                //if it trigger the condition, mark this node as bankrupt
                if (neighbor_node->mV <= ThrHld * neighbor_node->origin_mV) {
                    neighbor_node->neibInfln = 1;
                    //offer this node into queue.
                    offer(neighbor_node);
                }
            }
        }
    }
}

void multiple_traverse(Node **initial_nodes, int nums) {
    init_queue();
    int i = 0;

    for (i = 0; i < nums; i++) {
        Node *initial_node = initial_nodes[i];
        offer(initial_node);
        initial_node->mV = ThrHld * (head->mV);
        initial_node->neibInfln = 1;
    }

    while (!is_empty) {
        Node *node = pop();
        //if node is bankrupt, then influence other nodes
        if (node->neibInfln) {
            int i = 0;
            Node **neighbors = node->neighbors;
            for (i = 0; i < node->numNeib; i++) {
                Node *neighbor_node = neighbors[i];
                //calculate the new value
                neighbor_node->mV = (neighbor_node->mV) - (node->beta) * (node->weightA[i]);
                //if it trigger the condition, mark this node as bankrupt
                if (neighbor_node->mV <= ThrHld * neighbor_node->origin_mV) {
                    neighbor_node->neibInfln = 1;
                    //offer this node into queue.
                    offer(neighbor_node);
                }
            }
        }
    }
}