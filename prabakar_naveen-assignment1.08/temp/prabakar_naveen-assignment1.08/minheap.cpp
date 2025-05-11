#include <stdio.h>
#include "minheap.h"



//intialize the heap
void create(struct minheap *h){
    h->size = 0;
}

// heapify up 
void heapifyup(struct minheap *h, int i) {

    int x = 0;
    while (i > 0 && x == 0) {

        int j = (i - 1) / 2;

        if (h->heap[j].distance > h->heap[i].distance) {

            struct node temp = h->heap[j];
            h->heap[j] = h->heap[i];
            h->heap[i] = temp;
            i = j;

        } else {
            x = 1;
        }
    }
}

//heapify down
void heapifydown(struct minheap *h, int i) {

    int x = 0;

    while (x == 0) {
        int l = (2 * i) + 1;
        int r = (2 * i) + 2;
        int s = i;

        if (l < h->size && h->heap[l].distance < h->heap[s].distance){
            s = l;
        }
            
        if (r < h->size && h->heap[r].distance < h->heap[s].distance){
            s = r;
        }

        if (s != i) {
            struct node temp = h->heap[i];
            h->heap[i] = h->heap[s];
            h->heap[s] = temp;
            i = s;
        } else {
            x = 1;
        }
    }
}

//Insert a node to the heap
void insert(struct minheap *h, struct node n){

    h->heap[h->size] = n;
    heapifyup(h, h->size);
    h->size++;
}

//extract min
struct node extractmin(struct minheap *h) {
    
    struct node minNode = h->heap[0];
    int i = --h->size;
    h->heap[0] = h->heap[i];

    heapifydown(h, 0);
    return minNode;
}

