#define S_GA
#define S_HG

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "param.h"
#include "node.h"
#include "random.h"
#include "genetic.h"
#include "matrixCalculator.h"
#include "traverse.h"
#include "Queue.h"



void print_gen(int **gen,Node **node_array){
    int i=0;
    for(;i<POPULATION_SIZE;i++){
        print_solution(gen[i],node_array);
    }
}

int main() {
    
   
    int globalmax_adaptive=0;
    int globalmax=0;
    int **originGen=NULL;
    int *fitness=NULL;
    int **originGen_adaptive=NULL;
    int *fitness_adaptive=NULL;

    srand((unsigned int)time(NULL));
    //intialize the gra matrix.
    initial_graph();
    //get node array from the matrix.
    Node **node_array = transform_graph();
    originGen = init(node_array);
    fitness = getFitness(node_array, originGen);
    originGen_adaptive=copy_generation(originGen);
    fitness_adaptive=getFitness(node_array,originGen_adaptive);
    
    printf("adaptive algorithm\n");
    globalmax_adaptive=evolution_adaptive(fitness_adaptive,originGen_adaptive,node_array);
    printf("normal algorithm\n");
    globalmax=evolution(fitness,originGen,node_array);
   
    printf("adaptive global max fitness is %d\n",globalmax_adaptive);
    printf("global max fitness is %d\n",globalmax); 
    return 0;
}







