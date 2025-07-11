//
// Created by labuser on 12/6/2024.
//

#ifndef NEW_TODOSIMULATION_H
#define NEW_TODOSIMULATION_H


#include "list.h"
#include "queue.h"


struct Packet {
    int arrivalTime;
    int transmissionTime;
    int transmissionStartTime;
    int switcherId;
    int priority;
    struct Packet *next;
};




void parseInput(char *argv[], int *num_Packet, int *num_Switches, int *maxArrivalTime, int *maxTransmissionTime);

void initialiseSimulator(struct Queue* ,int ,int*);
void createPacketList(struct List *,int ,int ,int ,int );
struct Packet* newPacket(struct Queue* , struct List *);
int availableSwitcher(int*, int );
void reportTransmissionCompletion(struct List *, int *, int );
int getNextEventTime(struct List *, struct List *,int );
struct Packet* transmitPacket(struct Queue *theQueue);
void reportStatistics(struct List* , int , int , int );

#endif //NEW_TODOSIMULATION_H
