#include <stdio.h>
#include "monsters.h"
#include <stdlib.h>
#include <time.h>


void create_mon(struct monheap *h, int num){
    h->size = 0;
    h->hp = (struct monster*)malloc(sizeof(struct monster) * (num + 1));

    if (h->hp == NULL) {
        printf("Memory allocation failed.\n");
        exit(1); 
    }
}


//intialize the heap with monsters
void num_monsters(struct monheap *h, int x, int y){
    struct monster m;

    m.mx = x;
    m.my = y;

    int temp = rand() % 2;

    //intelligence
    if(temp){
        m.type[0] = '1';
    }
    else{
        m.type[0] =  '0';
    }

    temp = rand() % 2;

        //telepathic ability
    if(temp){
        m.type[1] = '1';
    }
    else{
        m.type[1] = '0';
    }
        
    temp = rand() % 2;

        //tunnel ability
    if(temp){
        m.type[2] = '1';
    }
    else{
        m.type[2] = '0';
    }

    temp = rand() % 2;

    if(temp){
        m.type[3] = '1';
    }
    else{
        m.type[3] = '0';
    }

    m.next = 0;
    m.speed = (rand() % 16) + 5; 
    m.pox = -1;
    m.poy = -1;
    m.type[4] = '\0';

    minsert(h, m);
}

void minsert(struct monheap *h, struct monster n){
    h->hp[h->size] = n;
    mheapifyup(h, h->size);
    h->size++;
}

// heapify up 
void mheapifyup(struct monheap *h, int i) {

    int x = 0;
    while (i > 0 && x == 0) {

        int j = (i - 1) / 2;

        if (h->hp[j].next > h->hp[i].next) {

            struct monster temp = h->hp[j];
            h->hp[j] = h->hp[i];
            h->hp[i] = temp;
            i = j;

        } else {
            x = 1;
        }
    }
}

//extract min
struct monster mextractmin(struct monheap *h) {
    
    struct monster minNode = h->hp[0];
    int i = --h->size;
    h->hp[0] = h->hp[i];

    mheapifydown(h, 0);
    return minNode;
}

//heapify down
void mheapifydown(struct monheap *h, int i) {

    int x = 0;

    while (x == 0) {
        int l = (2 * i) + 1;
        int r = (2 * i) + 2;
        int s = i;

        if (l < h->size && h->hp[l].next < h->hp[s].next){
            s = l;
        }
            
        if (r < h->size && h->hp[r].next < h->hp[s].next){
            s = r;
        }

        if (s != i) {
            struct monster temp = h->hp[i];
            h->hp[i] = h->hp[s];
            h->hp[s] = temp;
            i = s;
        } else {
            x = 1;
        }
    }
}


//remove monsters that are killed from heap
void mremove(struct monheap *h, int i){

    h->hp[i] = h->hp[--h->size];
    mheapifyup(h, i);
    mheapifydown(h, i);
}