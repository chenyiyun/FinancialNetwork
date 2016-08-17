#include "Queue.h"

int startpoint=0;
int endpoint=0;
struct NodeType *head;
int size=0;
int capacity=0;

int init_queue()
{
    head=malloc(sizeof(Node *)*NUMNODES);
    capacity=0;
    startpoint=0;
    endpoint=0;
}

bool is_empty()
{
    return startpoint==endpoint;
}


void offer(Node *node)
{
    //if it exceeds the capacity, copy old to new one and free previous memory
    if(size==capacity){
        int i=0;
        Node *new_head=malloc(sizeof(Node *)*capacity*2);
        for(i=0;i<capacity;i++){
            new_head[i]=head[i];
        }
        free(head);
        head=new_head;
    }
    int insert_position=endpoint;
    head[insert_position]=node;
    endpoint=(endpoint+1)%NUMNODES;
    size++;
}

Node *pop()
{
    Node *pop_node=head[startpoint];
    startpoint=(startpoint+1)%NUMNODES;
    return pop_node;
}

Node *peek()
{
    return head[startpoint];
}

Node *size()
{
    return size;
}
