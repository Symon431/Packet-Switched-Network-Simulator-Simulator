#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "TODOSimulation.h"

void createEmptyList(struct List* packetList) {
    if(packetList == NULL){
        printf("packetlist is NULL.\n");
        exit(1);
    }
    struct Packet *dummyNode = (struct Packet* ) malloc(sizeof(struct Packet));

    if (dummyNode == NULL) {
        printf("The dummy Node could not be allocated memory.\n");
        exit(1);
    }


    packetList->head = dummyNode;
    packetList->tail = dummyNode;
    packetList->size = 0;
    dummyNode->next = NULL;
}

void AddPacketToNewList(struct Packet *transmittedPacket, struct List *newList) {
    transmittedPacket->next = NULL;
    newList->tail->next = transmittedPacket;
    newList->tail = transmittedPacket; // Update tail
    newList->size++;
}

