#include <stdio.h>
#include "node.h"
#include "param.h"


void init_queue();
int is_empty();
void offer(Node *node);
Node *pop();
Node *peek();
int get_size();