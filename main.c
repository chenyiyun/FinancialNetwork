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
    
    int localmax = 0;
    int globalmax=0;
    int count=1;
    //int* localbestsolution = NULL;
    //int* globalbestsolution = NULL;
    srand((unsigned int)time(NULL));
    //intialize the gra matrix.
    initial_graph();
    //get node array from the matrix.
    Node **node_array = transform_graph();
    int **originGen = init(node_array);
    //get initial fitness.
    int *fitness = getFitness(node_array, originGen);
    while (count < GENERATION_NUM) {
        
        int **newGen = (int **)malloc(sizeof(int *)*POPULATION_SIZE);
        int *sum_array = getSumArray(fitness);
        
        int i = 0;
        //get new generation by two
        srand((unsigned int)time(NULL));
        for (; i < POPULATION_SIZE; i += 2) {
            int **couple = getCouple(sum_array, originGen);
            int *chromosome1 = couple[0];
            int *chromosome2 = couple[1];
            free(couple);
            newGen[i]=chromosome1;
            newGen[i+1]=chromosome2;
        }
        
        //free old generation
        for(i=0;i<POPULATION_SIZE;i++){
            free(originGen[i]);           
        }
        free(originGen);
        free(sum_array);

        
        //get favg and fmax, return array first value is favg, second value is fmax.
        double *array=getFitnessStats(fitness);
        double favg=array[0];
        double fmax=array[1];

        //cross
        for(i=0;i<POPULATION_SIZE;i+=2){
            int fitness1=fitness[i];
            int fitness2=fitness[i+1];
            int *chromosome1=newGen[i];
            int *chromosome2=newGen[i+1];
            int fquote=fitness1>fitness2?fitness1:fitness2;
            double pc=0.0;
            if(fquote!=fmax){
                pc=k1*(fmax-fquote)/(fmax-favg);
            }else{
                pc=k3;
            }
            double dice=rand()%100*1.0/100;
            if(dice<=pc){
                cross(chromosome1, chromosome2, node_array);
                newGen[i] = chromosome1;
                newGen[i + 1] = chromosome2;
            } 
        }
        //calculate fitness after cross
        free(fitness);
        fitness=getFitness(node_array,newGen);
        
        //mutation
        for(i=0;i<POPULATION_SIZE;i++){
            double pm=0.0;
            int f=fitness[i];
            if(fmax>f){
                pm=k2*(fmax-f)/(fmax-favg);
            }else{
                pm=k3;
            }
            double dice=rand()%100*1.0/100;
            if(dice<=pm){
                mutation(newGen[i],node_array);
            }
        }   

        //get fitness after mutation
        free(fitness);
        fitness=getFitness(node_array,newGen);
        
        localmax=0;
        for (i = 0; i < POPULATION_SIZE; i++) {
            if (localmax < fitness[i]) {
                localmax = fitness[i];
                //localbestsolution = newGen[i];
            }
        }
        //print_solution(localbestsolution,node_array);
        printf("%d,%d\n",count,localmax);
        if(localmax>globalmax){
            globalmax=localmax;
        }
        originGen=newGen;
        count++;
    }

    //printf("best solution is below\n");
    //print_solution(globalbestsolution, node_array);
    //int max=localmax > maxFitness? localmax: maxFitness;
    printf("global max fitness is %d\n",globalmax);
    printf("it used %d genenration\n",count);
 
    return 0;
}







