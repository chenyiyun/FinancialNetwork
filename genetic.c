/*@Author: Xiaowei Li*/
/*@Copyright belong to Xiaowei Li and Yiyun Chen*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "node.h"
#include "genetic.h"
#include "param.h"
#include "traverse.h"


void print_solution(int *solution, Node **node_array) {
    int i = 0;
    for (i = 0; i < NUMNODES; i++) {
        printf("%d:%d  ", node_array[i]->id, solution[i]);
    }
    printf("\n");
}

//initialize the chromosomes
int** init(Node **node_array) {
    int count = 0;
    double remain = TOTAL_FUNDS;

    int **solutions = (int **)malloc(sizeof(int *)*POPULATION_SIZE);
    
    while(count<POPULATION_SIZE){
        int *solution=(int *)calloc(NUMNODES,sizeof(int));
        int count2=0;
        int index=0;
        while(count2<10){
            index+=(rand() % (NUMNODES/10));
            if(index<NUMNODES){
              Node *node=node_array[index]; 
              if(node->bailout<=remain){
                 remain-=node->bailout;
                 solution[index]=1;
                 count2++;
              }
            }else{
                break;
            }
        }
        solutions[count]=solution;
        count++;
        remain=TOTAL_FUNDS;
    }
    
    return solutions;  
}

//get fitness of the population
int *getFitness(Node **node_array, int **solutions) {
    int i = 0;
    int *fitness = (int *)malloc(sizeof(int) * POPULATION_SIZE);
    
    for (i = 0; i < POPULATION_SIZE; i++) {
        int *solution = solutions[i];
        Node **new_node_array=copy_graph(node_array);
        fitness[i] = multiple_traverse(new_node_array, solution, NUMNODES);
        free_graph(new_node_array);
    }
    
    return fitness;
}

double *getFitnessStats(int *fitness){
    double *array=(double *) malloc(sizeof(double) * 2);
    double max=0;
    double sum=0;
    int i=0;
    for(i=0;i<POPULATION_SIZE;i++){
        if(fitness[i]>max) max=fitness[i];
        sum+=fitness[i];
    }

    array[0]=sum/POPULATION_SIZE;
    array[1]=max;
   
    return array;
}


int *getSumArray(int *fitness) {
    int sum = 0;
    int i = 0;
    int *sum_array = (int *)malloc(sizeof(int) * POPULATION_SIZE + 1);
    for (i = 0; i < POPULATION_SIZE; i++) {
        sum += fitness[i];
        sum_array[i] = sum;
    }
    sum_array[POPULATION_SIZE] = sum;
    return sum_array;
}

//roll the dice
int **getCouple(int* sum_array, int **solutions) {
    //get total fitness
    int **couple = (int **)malloc(sizeof(int *) * 2);
    int *flags = calloc(POPULATION_SIZE, sizeof(int));
    int count = 0;
    int sum = sum_array[POPULATION_SIZE];
    
    while (count < 2) {
        int dice = rand() % sum;
        int start = 0;
        int end = POPULATION_SIZE - 1;
        int index = -1;
        //binary search where this dice should be landed on.
        while (start + 1 < end) {
            int mid = start + (end - start) / 2;
            if (sum_array[mid] < dice) {
                start = mid;
            } else if (sum_array[mid] == dice) {
                index = mid;
                break;
            } else if (mid > 0 && sum_array[mid - 1] < dice) {
                index = mid;
                break;
            } else {
                end = mid;
            }
        }
        //avoid a special corner case when array contains 4,6 and program want to find 5.
        if (index == -1) {
            if (sum_array[start] < dice) {
                index = end;
            } else {
                index = start;
            }
        }
        
        if (flags[index] == 1) {continue;}
        else {
            int * newChromesome=(int *)calloc(NUMNODES,sizeof(int));
            memcpy(newChromesome,solutions[index],sizeof(int)*NUMNODES);
            couple[count] = newChromesome;
            flags[index] = 1;
            count++;
        }
    }
    free(flags);
    
    return couple;
}



//cross the chromesome
void cross(int *chromsome1, int *chromsome2, Node **node_array) {
    if ((rand() % 100) <= CROSS_RATE * 100) {
        int startIndex = rand() % NUMNODES;
        int i = 0;
        int part11 = 0, part12 = 0, part21 = 0, part22 = 0;
        
        for (i = 0; i < startIndex; i++) {
            if (chromsome1[i] == 1) {
                part11 += node_array[i]->bailout;
            }
            if (chromsome2[i] == 1) {
                part21 += node_array[i]->bailout;
            }
        }
        
        for (i = startIndex; i < NUMNODES; i++) {
            if (chromsome1[i] == 1) {
                part12 += node_array[i]->bailout;
            }
            if (chromsome2[i] == 1) {
                part22 += node_array[i]->bailout;
            }
        }
        //printf("preparing to cross\n");
        //if conditions are satisfies even after the cross, then cross.
        if ((part11 + part22) < TOTAL_FUNDS && (part12 + part21) < TOTAL_FUNDS) {
            //printf("these two chromosome is actually cross\n");
            for (i = startIndex; i < NUMNODES; i++) {
                int tmp = chromsome1[i];
                chromsome1[i] = chromsome2[i];
                chromsome2[i] = tmp;
            }
        }
    }
}

//variation on chromesome
void mutation(int *chromsome, Node **node_array) {
    double roll = rand() % 100 * 1.0 / 100;
    if (roll <= MUTATION_RATE) {
        int varIndex = rand() % NUMNODES;
        int i = 0;
        int cur = 0;
        for (i = 0; i < NUMNODES; i++) {
            if (chromsome[i] == 1) {
                cur += node_array[i]->bailout;
            }
        }
        
        if (chromsome[varIndex] == 0) {
            if (cur + node_array[varIndex]->bailout <= TOTAL_FUNDS) {
                chromsome[varIndex] = 1;
            }
        } else {
            chromsome[varIndex] = 0;
        }
    }
}

int **copy_generation(int **originGen){
    int **newGen=(int **)malloc(sizeof(int *)*POPULATION_SIZE);
    int i=0;
    
    for(;i<POPULATION_SIZE;i++){
        int *solution=(int *)calloc(NUMNODES,sizeof(int));
        size_t size=NUMNODES*sizeof(int);
        memcpy(solution,originGen[i],size);
        newGen[i]=solution;
    }
    
    return newGen;
}


int evolution_adaptive(int *fitness,int **originGen,Node **node_array){

    int localmax = 0;
    int globalmax=0;
    int count=1;
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

        
        //adaptive cross and mutation.

        //get favg and fmax, return array first value is favg, second value is fmax.
        double *array=getFitnessStats(fitness);
        double favg=array[0];
        double fmax=array[1];
        
        //self adaptive cross
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
        //printf("%d,%d\n",count,localmax);
        if(localmax>globalmax){
            globalmax=localmax;
        }
        originGen=newGen;
        count++;
    }
    return globalmax;

}


int evolution(int *fitness,int **originGen,Node **node_array){

    int localmax = 0;
    int globalmax=0;
    int count=1;
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

        
        //cross and mutation without adaptive algorithm.

        //self adaptive cross
        for(i=0;i<POPULATION_SIZE;i+=2){
            int *chromosome1=newGen[i];
            int *chromosome2=newGen[i+1];
            double dice=rand()%100*1.0/100;
            if(dice<=CROSS_RATE){
                cross(chromosome1, chromosome2, node_array);
                newGen[i] = chromosome1;
                newGen[i + 1] = chromosome2;
            } 
        }
        
        //mutation
        for(i=0;i<POPULATION_SIZE;i++){
            double dice=rand()%100*1.0/100;
            if(dice<=MUTATION_RATE){
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
            }
        }
        printf("%d,%d\n",count,localmax);
        if(localmax>globalmax){
            globalmax=localmax;
        }
        originGen=newGen;
        count++;
    }
    return globalmax;
}
