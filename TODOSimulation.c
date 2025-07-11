#include <stdio.h>
#include <stdlib.h>
#include "TODOSimulation.h"
#include "queue.h"
#include "list.h"
#include <time.h>


int main(int argc, char *argv[]) {

    if (argc < 5) {
        fprintf(stderr, "Incorrect number of arguments\n");
        printf("Please make sure to add noOfPackets, noOfSwitchers, maxArrivalTime, maxTransmissionTime in that order to the program argument.\nGOODBYE!!!!\n");
        exit(1);
    }

    printf("=====================================\n");
    printf("  Packet Switched Network Simulator  \n");
    printf("=====================================\n");

    srand(time(NULL));

    int num_Packet, num_Switchers, maxArrivalTime, maxTransmissionTime;
    parseInput(argv, &num_Packet, &num_Switchers, &maxArrivalTime, &maxTransmissionTime);

    struct List *theList = malloc(sizeof(struct List));
    createEmptyList(theList);
    createPacketList(theList, num_Packet, maxArrivalTime, maxTransmissionTime, num_Switchers);

    struct Queue *theQueue = malloc(sizeof(struct Queue));
    createEmptyQueue(theQueue);

    int *switchers_arr = malloc(sizeof(int) * num_Switchers);
    initialiseSimulator(theQueue, num_Switchers, switchers_arr);

    struct List *TransmittedPacketList = malloc(sizeof(struct List));
    createEmptyList(TransmittedPacketList);

    int TIME = 0;

    while (theList->size != 0 || !IsEmptyQueue(theQueue)) {
        int nextEventTime = getNextEventTime(theList, TransmittedPacketList, TIME);
        if (nextEventTime == -1) break;
        if (nextEventTime <= TIME) {
            nextEventTime = TIME + 1;
        }
        TIME = nextEventTime;
        printf("\nCurrent TIME: %d\n", TIME);

        while (theList->head->next != NULL && theList->head->next->arrivalTime <= TIME) {
            newPacket(theQueue, theList);
        }

        int switcherID = availableSwitcher(switchers_arr, num_Switchers);
        while (switcherID != -1 && !IsEmptyQueue(theQueue)) {
            struct Packet *transmittedPacket = transmitPacket(theQueue);
            if (transmittedPacket != NULL) {
                transmittedPacket->switcherId = switcherID + 1;
                transmittedPacket->transmissionStartTime = TIME;
                AddPacketToNewList(transmittedPacket, TransmittedPacketList);
                switchers_arr[switcherID] = 0;
                printf("Switcher %d started transmission (Time: %d, Duration: %d, Priority: %d)\n",
                       switcherID + 1, TIME, transmittedPacket->transmissionTime, transmittedPacket->priority);
            }
            switcherID = availableSwitcher(switchers_arr, num_Switchers);
        }
        reportTransmissionCompletion(TransmittedPacketList, switchers_arr, TIME);
    }


    printf("\n\n=====================================\n");
    printf("Final statistics:\n");
    printf("=====================================\n");
    reportStatistics(TransmittedPacketList, num_Packet, num_Switchers, TIME);

    free(switchers_arr);
    free(theList);
    free(theQueue);
    free(TransmittedPacketList);

    return 0;
}




void parseInput(char *argv[], int *num_Packet, int *num_Switches, int *maxArrivalTime, int *maxTransmissionTime) {

    *num_Packet = atoi(argv[1]);
    *num_Switches = atoi(argv[2]);
    *maxArrivalTime = atoi(argv[3]);
    *maxTransmissionTime = atoi(argv[4]);
    printf("The num of packet:%d Num of switches:%d max arrival time:%d max transmission time:%d\n\n", *num_Packet,
           *num_Switches, *maxArrivalTime, *maxTransmissionTime);
}




void initialiseSimulator(struct Queue *theQueue, int num_Switches, int *switchers_arr) {
    createEmptyQueue(theQueue);

    // Initialize the switchers_arr passed from main (not a local array)
    for (int i = 0; i < num_Switches; ++i) {
        switchers_arr[i] = 1; // 1 = available
    }
}



int getNextEventTime(struct List *arrivalList, struct List *transmittedList, int currentTime) {
    int nextEventTime = INT_MAX;
    int foundEvent = 0;

    // Check next arrival time
    if (arrivalList->head->next != NULL && arrivalList->head->next->arrivalTime > currentTime) {
        nextEventTime = arrivalList->head->next->arrivalTime;
        foundEvent = 1;
    }

    // Check next transmission completion time in transmittedList
    struct Packet *temp = transmittedList->head->next;
    while (temp != NULL) {
        int completionTime = temp->transmissionStartTime + temp->transmissionTime;
        if (completionTime > currentTime && completionTime < nextEventTime) {
            nextEventTime = completionTime;
            foundEvent = 1;
        }
        temp = temp->next;
    }

    if (!foundEvent) {
        return -1;
    }
    return nextEventTime;
}


void createPacketList(struct List *packetList, int num_Packet, int maxArrivalTime, int maxTransmissionTime, int switcher_num) {
    packetList->switcher_num = switcher_num;

    for (int i = 0; i < num_Packet; ++i) {
        struct Packet *newPacket = (struct Packet *)malloc(sizeof(struct Packet));
        if (newPacket == NULL) {
            printf("New packet cannot be allocated memory.\n");
            exit(1);
        }

        newPacket->next = NULL;
        newPacket->arrivalTime = (rand() % maxArrivalTime) + 1;  // Ensures arrival time is at least 1
        newPacket->transmissionTime = (rand() % maxTransmissionTime) + 1;  // Ensures transmission time is at least 1
        newPacket->priority = (rand() % 4) + 1;
        newPacket->switcherId = 0;
        newPacket->transmissionStartTime = 0;

        struct Packet *temp = packetList->head;
        if (temp->next == NULL) {
            packetList->head->next = newPacket;
            packetList->tail = newPacket;
        } else {
            int inserted = 0;
            while (temp->next != NULL && !inserted) {
                if (newPacket->arrivalTime < temp->next->arrivalTime) {
                    newPacket->next = temp->next;
                    temp->next = newPacket;
                    inserted = 1;
                }
                temp = temp->next;
            }
            if (!inserted) {
                temp->next = newPacket;
                packetList->tail = newPacket;
            }
        }
        packetList->size++;
        printf("Created packet with Arrival Time: %d, Transmission Time: %d, Priority: %d\n",
               newPacket->arrivalTime, newPacket->transmissionTime, newPacket->priority);
    }
}


struct Packet* newPacket(struct Queue* theQueue, struct List *theList) {
    if (theList->size == 0 || theList->head->next == NULL) {
        printf("🚨 ERROR: The List is empty. No packets to add.\n");
        return NULL;
    }

    struct Packet *insertPacket = theList->head->next;
    theList->head->next = insertPacket->next;
    theList->size--;

    insertPacket->next = NULL;

    if (theQueue->size == 0) {
        theQueue->front->next = insertPacket;
        theQueue->rear = insertPacket;
    } else {
        struct Packet *temp = theQueue->front;
        int inserted = 0;
        while (temp->next != NULL && !inserted) {
            if (insertPacket->priority > temp->next->priority ||
                (insertPacket->priority == temp->next->priority && insertPacket->arrivalTime < temp->next->arrivalTime)) {
                insertPacket->next = temp->next;
                temp->next = insertPacket;
                inserted = 1;
            }
            temp = temp->next;
        }
        if (!inserted) {
            temp->next = insertPacket;
            theQueue->rear = insertPacket;
        }
    }

    theQueue->size++;

    printf("Packet added to queue: Arrival Time: %d, Transmission Time: %d, Priority: %d\n",
           insertPacket->arrivalTime, insertPacket->transmissionTime, insertPacket->priority);

    return insertPacket;
}






int availableSwitcher(int *arraySwitchers, int numOfSwitchers) {
    int availableSwitchers[numOfSwitchers];
    int count = 0;

    // Find available switchers
    for (int i = 0; i < numOfSwitchers; ++i) {
        if (arraySwitchers[i] == 1) {  // Switcher is available
            availableSwitchers[count++] = i;
        }
    }

    if (count == 0) {
        return -1;  // No switchers available
    }

    // Randomly select one of the available switchers
    int randomIndex = rand() % count;
    return availableSwitchers[randomIndex];
}


struct Packet* transmitPacket(struct Queue *theQueue) {
    if (IsEmptyQueue(theQueue)) {
        printf("🚨 ERROR: The Queue is empty, no packet to transmit.\n");
        return NULL;
    }

    struct Packet *thePacket = theQueue->front->next;
    theQueue->front->next = thePacket->next;

    // If the dequeued packet was the last one, update the rear pointer
    if (theQueue->rear == thePacket) {
        theQueue->rear = theQueue->front;
    }

    theQueue->size--;
    thePacket->next = NULL;  // Prevent dangling references

    printf("Transmitting packet (Transmission Time: %d, Priority: %d)\n",
           thePacket->transmissionTime, thePacket->priority);
    return thePacket;
}



void reportTransmissionCompletion(struct List *TransmittedPacketList, int *switchers_arr, int TIME) {
    struct Packet *temp = TransmittedPacketList->head->next;
    while (temp != NULL) {
        int transmissionCompletionTime = temp->transmissionStartTime + temp->transmissionTime;
        if (transmissionCompletionTime <= TIME) {
            printf(" Switcher %d is now available at TIME: %d\n", temp->switcherId, TIME);
            switchers_arr[temp->switcherId - 1] = 1;  // Mark switcher as available
        }
        temp = temp->next;
    }
}





void reportStatistics(struct List* theList, int numPackets, int numSwitchers, int Time) {
    int sumWaitingTime = 0, AverageWaitingTime;
    printf("The number of packets: %d\n", numPackets);
    printf("The number of switchers: %d\n", numSwitchers);
    printf("Completion Time: %d\n", Time);

    struct Packet *temp = theList->head->next;

    while (temp != NULL) {
        sumWaitingTime += (temp->transmissionStartTime - temp->arrivalTime);
        temp = temp->next;
    }

    AverageWaitingTime = sumWaitingTime / numPackets;
    printf("Average waiting time: %d\n", AverageWaitingTime);

    int SumTransmissionTime = 0, averageTransmissionTime;
    temp = theList->head->next;

    while (temp != NULL) {
        SumTransmissionTime += temp->transmissionTime;
        temp = temp->next;
    }

    averageTransmissionTime = SumTransmissionTime / numPackets;
    printf("Average transmission time per packet: %d\n", averageTransmissionTime);

    int *arraySwitchers = (int *)calloc(numSwitchers, sizeof(int));

    temp = theList->head->next;

    while (temp != NULL) {
        arraySwitchers[temp->switcherId - 1]++;
        temp = temp->next;
    }

    int largest = arraySwitchers[0], index = 0;
    for (int i = 1; i < numSwitchers; ++i) {
        if (arraySwitchers[i] > largest) {
            largest = arraySwitchers[i];
            index = i;
        }
    }

    for (int i = 0; i < numSwitchers; ++i) {
        printf("Switcher %d transmitted %d packets.\n", i + 1, arraySwitchers[i]);
    }

    printf("The busiest switcher was: %d.\n", index + 1);

    free(arraySwitchers);
}