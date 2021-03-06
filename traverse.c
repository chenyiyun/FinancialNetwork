#include "traverse.h"


void single_traverse(Node *initial_node)//contagion caused by a single node
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
                    //offer this node into queue.
                    offer(neighbor_node);
                }
            }
        }
    }
}


int multiple_traverse(Node **initial_nodes, int *array, int nums)//contagion caused by multiple nodes
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
    //printf("initial bankrupt %d nodes\n",count);
    //loop until queue is empty;
    //this queue only contains node that is bankrupted.
    while (!is_empty()) {
        Node *node = pop();
        //printf("node %d get out from queue\n",node->id);
        //if node is bankrupt, then influence other nodes
        int i = 0;
        Node **neighbors = node->neighbors;
        for (i = 0; i < node->numNeib; i++) {
            Node *neighbor_node = neighbors[i];
            //printf("get its neighbor %d\n",neighbor_node->id);
            if (neighbor_node->infln) {
                continue;
            }
            //printf("the old value is %f\n",neighbor_node->mV); 
            //calculate the new value
            neighbor_node->mV = (neighbor_node->mV) - (node->beta) * (node->weightA[i]);
            //printf("the new value is %f,with the beta value is %f\n",neighbor_node->mV,node->beta);
            //if it trigger the condition, mark this node as bankrupt
            if (neighbor_node->mV <= (BRThrHld * neighbor_node->origin_mV)) {
                neighbor_node->infln = 1;
                count++;
                //maintain the bankrupt cost, this is the base to influence others
                if (neighbor_node->mV < 0) {
                    neighbor_node->beta = neighbor_node->origin_mV;
                } else {
                    neighbor_node->beta = neighbor_node->origin_mV - neighbor_node->mV;
                }
                //offer this node into queue.
                offer(neighbor_node);
            }
        }
    }
    
    free_queue();
    return count;
}
