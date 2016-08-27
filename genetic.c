#include <stdio.h>
#include <node.h>
#include <stdlib.h>
#include <time.h>


//initialize the chromesome.
int** init(Node **node_array) {
    int count = 0;
    int start = 0;
    int dice = 0;
    int cur = 0;
    int rangestart = 0;
    int rangeend = NUMNODES - 1;
    bool stop = false;
    long remain = TOTAL_FUNDS;

    sort_node(node_array);

    int **solutions = (int **)malloc(sizeof(int *)*POPULATION_SIZE);

    srand((unsinged int)time(NULL));
    while (count < POPULATION_SIZE) {
        start = rand() % NUMNODES;
        cur = start;
        int *solution = calloc(NUMNODES, sizeof(int));

        //get a solution
        while (!stop) {

            dice = rand() % 10;
            //select cur;
            if (node_array[cur]->beta > remain) {
                rangeend = cur - 1;
            } else if (dice >= 5 && solution[cur] != 1) {
                if (remain > node_array[cur]->beta) {
                    remain -= node_array[cur]->beta;
                    solution[cur] = 1;
                    cur++;
                } else if (remain == node_array[cur]->beta) {
                    solution[cur] = 1;
                    stop = true;
                }
            } else {
                cur++;
            }

            //move rangestart
            while (solution[rangestart] == 1 && rangestart < rangeend) {
                rangestart++;
            }

            //move rangeend
            while (solution[rangeend] == 1 && rangeend > rangestart) {
                rangeend--;
            }

            if (rangestart > rangeend) {
                stop = true;
            }

            if (cur < rangestart || cur > rangeend) {
                cur = rangestart;
            }

        }

        solutions[count] = solution;
        count++;
        rangestart = 0;
        rangeend = NUMNODES - 1;
        remain = TOTAL_FUNDS;
    }

    return solutions;

}


//sort node according to beta
void sort_node(Node **node_array) {

    int i = 0, j = 0, minIndex = 0;
    Node *minnode = NULL;

    //select sort, time complexity(o(n^2))
    for (i = 0; i < NUMNODES; i++) {
        minnode = node_array[i];
        minIndex = i;
        for (j = i; j < NUMNODES; j++) {
            if (minnode->beta < node_array[j]->beta) {
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
int *getFintness(Node **node_array, int **solutions) {
    int i = 0;
    unsinged long size = sizeof(Node) * NUMNODES;

    int *fitness = (int *)malloc(sizeof(int) * POPULATION_SIZE);

    for (i = 0; i < POPULATION_SIZE; i++) {
        int *solution = solutions[i];
        Node **new_node_array = (Node **)malloc(size);
        memcpy(new_node_array, node_array, size);
        fitness[i] = multiple_traverse(new_node_array, solution, NUMNODES);
        free(new_node_array);
    }

    return fitness;
}


//roll the dice
int **getSubPopulation(int *fitness) {
    //get total fitness
    int i = 0;
    int sum = 0;
    int *rst = (int **)malloc(sizeof(int *) * SUB_POP_SIZE);
    int *flags = calloc(NUMNODES, sizeof(int));
    int index = 0;
    for (i = 0; i < NUMNODES; i++) {
        sum += fitness[i];
    }

    srand((unsinged int)time(NULL));

    while (index < SUB_POP_SIZE) {
        if (flags[i] == 1) {
            int dice = rand() % 100;
            double pro = fitness[i] / sum;
            if (dice < pro * 100) {
                rst[index] = solutions[i];
                index++;
                flags[i] = 1;
            }
        }

        if (i == NUMNODES) {
            i = 0;
        } else {
            i++;
        }
    }


    return rst;
}


//cross the chromesome
void cross(int *chromsome1, int *chromsome2, Node **node_array) {
    srand((unsinged int)time(NULL));
    int startIndex = rand() % NUMNODES;
    int i = 0;
    int part11 = 0, part12 = 0, part21 = 0, part22 = 0;

    for (i = 0; i < startIndex; i++) {
        if (chromsome1[i] == 1) {
            part11 += node_array->beta;
        }
        if (chromsome2[i] == 1) {
            part21 += node_array->beta;
        }
    }

    for (i = startIndex; i < NUMNODES; i++) {
        if (chromsome1[i] == 1) {
            part12 += node_array->beta;
        }
        if (chromsome2[i] == 1) {
            part22 += node_array->beta;
        }
    }

    //if conditions are satisfies even after the cross, then cross.
    if ((part11 + part22) < TOTAL_FUNDS && (part12 + part21) < TOTAL_FUNDS) {
        for (i = startIndex; i < NUMNODES; i++) {
            int tmp = chromsome1[i];
            chromsome1[i] = chromsome2[i];
            chromsome2[i] = tmp;
        }
    }
}


//variation on chromesome
void variation(int *chromsome, Node **node_array) {
    srand((unsinged int)time(NULL));
    int varIndex = rand() % NUMNODES;
    int i = 0;
    int cur = 0;
    for (i = 0; i < NUMNODES; i++) {
        if (chromsome[i] == 1) {
            cur += node_array[i]->beta;
        }
    }

    if (chromsome[varIndex] == 0) {
        if (cur + node_array[varIndex]->beta <= TOTAL_FUNDS) {
            chromsome[varIndex] = 1;
        }
    } else {
        chromsome[varIndex] = 0;
    }
}


//get couple combinations out of subPopulation
//randomized_subpop will store the random series data, and each neighbored
//value will be performed cross
//e.g. randomized_subpop will be like: 3,4,1,2,6,5 then 3 and 4 perform cross, 1 and 2 perform. etc
int *getCombinations() {
    int i = 0;
    int index = 0;
    int *randomized_subpop = (int *) calloc(SUB_POP_SIZE, sizeof(int));
    srand((unsinged int)time(NULL));
    for (i = 0; i < SUB_POP_SIZE; i++) {
        index = rand() % SUB_POP_SIZE;
        while (randomized_subpop[index]) {
            index = (index + 1) % SUB_POP_SIZE;
        }
        randomized_subpop[index] = i + 1;
    }

    return randomized_subpop;
}



//solutions    | pointer that pointed to a solution  |    |   |
//solution |1|0|1|0|
int main() {

    int maxTimes = 0;
    int localmax = 0;
    int maxFitness = 0;
    int* bestsolution = NULL;
    //intialize the gra matrix.
    initial_graph();
    //get node array from the matrix.
    Node **node_array = transform_graph();
    //intialize the chromsome population
    int **solutions = init(node_array);
    //get fitness of all chromsome
    int *fitness = getFintness(node_array, solutions);
    //get subpopulation
    int **subpop = getSubPopulation(fitness);
    //iterate through different generations until no more improve happens
    while (maxTimes < 5) {
        int i = 0;
        //cross
        int *combinations = getCombinations();
        for (; i < SUB_POP_SIZE; i += 2) {
            cross(subpop[combinations[i] - 1], subpop[combinations[i + 1] - 1], node_array);
        }
        variation(subpop[combinations[i] - 1]);
        variation(subpop[combinations[i + 1] - 1]);

        int *fitnesses = getFintness(node_array, subpop);
        for (i = 0; i < SUB_POP_SIZE; i++) {
            if (localmax < fitnesses[i]) {
                localmax = fitnesses[i];
                bestsolution = subpop[i];
            }
        }

        if (localmax > maxFitness && localmax - maxFitness < 2) {
            maxTimes++;
        } else if (localmax < maxFitness && maxFitness - localmax < 2) {
            maxTimes++;
        } else {
            maxTimes = 0;
        }

        if (maxFitness < localmax) {
            maxFitness = localmax;
        }
    }

    return bestsolution;
}








