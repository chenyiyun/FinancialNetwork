/*@Author: Xiaowei Li*/
/*@Copyright belong to Xiaowei Li and Yiyun Chen*/

#ifndef GENETIC_H_INCLUDED
#define GENETIC_H_INCLUDED

#include "node.h"

typedef enum
{
    false,
    true
}bool;


void sort_node(Node **node_array);
int** init(Node **node_array);
int *getFitness(Node **node_array, int **solutions);
double *getFitnessStats(int *fitness);
int *getSumArray(int *fitness);
int **getCouple(int* sum_array, int **solutions);
void cross(int *chromsome1, int *chromsome2, Node **node_array);
void mutation(int *chromsome, Node **node_array);
void print_solution(int *solution, Node **node_array);
void freeMem(Node **new_node_array);
void copyMem(Node **new_node_array, Node **node_array);
int **copy_generation(int **originGen);
int evolution_adaptive(int *fitness,int **originGen,Node **node_array);
int evolution(int *fitness,int **originGen,Node **node_array);


#endif
