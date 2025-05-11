#ifndef MINHEAP
#define MINHEAP

struct node {
    int distance;
    int x;
    int y;
};

struct minheap {
    struct node heap[10000];
    int size;
};


void create(struct minheap *h);

void insert(struct minheap *h, struct node n);

struct node extractmin(struct minheap *h);

void heapifydown(struct minheap *h, int i);

void heapifyup(struct minheap *h, int i);

#endif