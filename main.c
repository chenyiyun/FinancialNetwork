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

int main() {
    
    int maxTimes = 0;
    int localmax = 0;
    int maxFitness = 0;
    int count=1;
    int* bestsolution = NULL;
    srand((unsigned int)time(NULL));
    //intialize the gra matrix.
    initial_graph();
    //get node array from the matrix.
    Node **node_array = transform_graph();
    int **originGen = init(node_array);
    //get initial fitness.
    int *fitness = getFitness(node_array, originGen);
    while (maxTimes < 5) {
        printf("count is %d\n",count);
        int **newGen = (int **)malloc(sizeof(int *)*POPULATION_SIZE);
        int *sum_array = getSumArray(fitness);
        
        int i = 0;
        //cross
        srand((unsigned int)time(NULL));
        for (; i < POPULATION_SIZE; i += 2) {
            int **couple = getCouple(sum_array, originGen);
            int *chromsome1 = couple[0];
            int *chromsome2 = couple[1];
            cross(chromsome1, chromsome2,node_array);
            mutation(chromsome1,node_array);
            mutation(chromsome2,node_array);
            newGen[i] = chromsome1;
            newGen[i + 1] = chromsome2;
            free(couple);
        }
        
        free(sum_array);
        //judge if it triggers the terminating condition.
        int *tmp_fitness = fitness;
        fitness = getFitness(node_array, newGen);
        free(tmp_fitness);
        
        localmax=0;
        for (i = 0; i < POPULATION_SIZE; i++) {
            if (localmax < fitness[i]) {
                localmax = fitness[i];
                if(localmax>=maxFitness){
                    bestsolution = newGen[i];
                }
            }
        }
        printf("localmax is %d\n",localmax);
        printf("maxFitness is %d\n",maxFitness);
        
        if (localmax > maxFitness && localmax - maxFitness < 2) {
            maxTimes++;
        } else if (localmax < maxFitness && maxFitness - localmax < 2) {
            maxTimes++;
        } else if(localmax==maxFitness){
            maxTimes++;
        } else {
            maxTimes = 0;
            maxFitness=localmax;
        }
        
        if (maxFitness < localmax) {
            maxFitness = localmax;
        }
        
        int **tmp_gen = originGen;
        originGen = newGen;
        free(tmp_gen);
        count++;
    }

    printf("best solution is below\n");
    print_solution(bestsolution, node_array);
    int max=localmax > maxFitness? localmax: maxFitness;
    printf("max fitness is %d\n",max);
    printf("it used %d genenration\n",count);
    
    return 0;
}





