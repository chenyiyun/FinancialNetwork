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
    
    //int maxTimes = 0;
    int localmax = 0;
    //int maxFitness = 0;
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
    while (count < 100) {
        printf(" ***** generation %d *****\n",count);
        int **newGen = (int **)malloc(sizeof(int *)*POPULATION_SIZE);
        int *sum_array = getSumArray(fitness);
        
        int i = 0;
        //cross
        srand((unsigned int)time(NULL));
        for (; i < POPULATION_SIZE; i += 2) {
            int **couple = getCouple(sum_array, originGen);
            int *chromosome1 = couple[0];
            int *chromosome2 = couple[1];
            double dice=rand()%100*1.0/100;
            if(dice>CROSS_RATE){
                newGen[i]=chromosome1;
                newGen[i+1]=chromosome2;
            }else{
                cross(chromosome1, chromosome2, node_array);
                newGen[i] = chromosome1;
                newGen[i + 1] = chromosome2;
            }
            mutation(chromosome1,node_array);
            mutation(chromosome2,node_array);
            free(couple);
        }

        //free old generation
        for(i=0;i<POPULATION_SIZE;i++){
            free(originGen[i]);           
        }
        free(originGen);
        free(sum_array);
        //judge if it triggers the terminating condition.
        int *tmp_fitness = fitness;
        fitness = getFitness(node_array, newGen);
        free(tmp_fitness);
        
        localmax=0;
        for (i = 0; i < POPULATION_SIZE; i++) {
            if (localmax < fitness[i]) {
                localmax = fitness[i];
                //if(localmax>=maxFitness){
                    bestsolution = newGen[i];
                //}
            }
        }
        printf("localmax is %d\n",localmax);
        //printf("maxFitness is %d\n",maxFitness);
        //print_solution(bestsolution, node_array);
        
        //if (localmax > maxFitness && localmax - maxFitness < 2) {
        //    maxTimes++;
        //} else if (localmax < maxFitness && maxFitness - localmax < 2) {
        //    maxTimes++;
        //} else if(localmax==maxFitness){
        //    maxTimes++;
        //} else {
        //    maxTimes = 0;
        //    maxFitness=localmax;
        //}
        
        //if (maxFitness < localmax) {
        //    maxFitness = localmax;
        //}
        //print_solution(bestsolution, node_array);
        originGen=newGen;
        count++;
        //printf("maxTimes is %d\n",maxTimes);
    }

    printf("best solution is below\n");
    print_solution(bestsolution, node_array);
    //int max=localmax > maxFitness? localmax: maxFitness;
    printf("max fitness is %d\n",localmax);
    printf("it used %d genenration\n",count-1);
    
    return 0;
}





