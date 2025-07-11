//
// Created by labuser on 12/6/2024.
//

#ifndef NEW_QUEUE_H
#define NEW_QUEUE_H

struct Queue{
    struct Packet *front;
    struct Packet *rear;
    int size;
};


void createEmptyQueue(struct Queue* );
int IsEmptyQueue(struct Queue * );


#endif //NEW_QUEUE_H
