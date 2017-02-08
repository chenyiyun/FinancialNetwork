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
    int start = 0;
    int dice = 0;
    int cur = 0;
    bool stop = false;
    double remain = TOTAL_FUNDS;
    //printf("total funds is %f\n",remain);
    
    
    sort_node(node_array);
    int **solutions = (int **)malloc(sizeof(int *)*POPULATION_SIZE);
    
    while (count < POPULATION_SIZE) {
        start = rand() % NUMNODES;
        cur = start;
        int *solution = (int *)calloc(NUMNODES, sizeof(int));
        int *flag = (int *)calloc(NUMNODES, sizeof(int));
        
        //get a solution
        while (!stop) {
            
            int count = 0;
            
            while (flag[cur] && count != NUMNODES) {
                cur = (cur + 1) % NUMNODES;
                count++;
            }
            
            //no suitable slot to allocate money.
            if (count == NUMNODES) {
                break;
            }
            
            dice = rand() % 10;
            //if current bailout is greater than remain, then no suitable slots for all slots after it. mark the flag as 1
            if (node_array[cur]->bailout > remain) {
                int i = cur;
                while (i < NUMNODES) {
                    flag[i] = 1;
                    i++;
                }
            }
            //current slot is suitable to allocate
            else if (node_array[cur]->bailout <= remain) {
                //dice is greater than five, then allocate money
                if (dice >= 5) {
                    remain -= node_array[cur]->bailout;
                    solution[cur] = 1;
                    flag[cur] = 1;
                    if (remain == 0) break;
                    else {
                        cur = (cur + 1) % NUMNODES;
                    }
                }
                //dice is less than five, then not allocate money
                else {
                    cur = (cur + 1) % NUMNODES;
                }
            }
        }
        // printf("solution %d:\n", count);
        // print_solution(solution, node_array);
        solutions[count] = solution;
        count++;
        remain = TOTAL_FUNDS;
        stop = false;
        
    }
    
    return solutions;
    
}


//sort node according to bailout
//increasing
void sort_node(Node **node_array) {
    
    int i = 0, j = 0, minIndex = 0;
    Node *minnode = NULL;
    
    //select sort, time complexity(o(n^2))
    for (i = 0; i < NUMNODES; i++) {
        minnode = node_array[i];
        minIndex = i;
        for (j = i; j < NUMNODES; j++) {
            if (minnode->bailout > node_array[j]->bailout) {//
                minnode = node_array[j];
                minIndex = j;
            }
        }
        
        //swap two pointer.
        Node *tmp = node_array[i];
        node_array[i] = node_array[minIndex];
        node_array[minIndex] = tmp;
    }
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
