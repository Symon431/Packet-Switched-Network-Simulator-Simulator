#ifndef NEW_LIST_H
#define NEW_LIST_H

struct List{
    struct Packet *head;
    struct Packet *tail;
    int size;
    int switcher_num;
};



void createEmptyList(struct List* );
void AddPacketToNewList(struct Packet *,struct List* );


#endif //NEW_LIST_H
