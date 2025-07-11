#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "TODOSimulation.h"

void createEmptyQueue(struct Queue* theQueue){

    if(theQueue == NULL){
        printf("the queue is NULL.\n");
        exit(1);
    }
    struct Packet *dummyNode = (struct Packet *) malloc(sizeof(struct Packet));
    if(dummyNode == NULL){
        printf("The dummy Node for the priority Queue could not be allocated memory.\n");
        exit(1);
    }


    dummyNode->next = NULL;
    theQueue->front = dummyNode;
    theQueue->rear = dummyNode;
    theQueue->size = 0;


}

int IsEmptyQueue(struct Queue *theQueue){
    if(theQueue->size == 0)
        return 1;
    else
        return 0;
}