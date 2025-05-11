#ifndef MONSTERHEAP
#define MONSTERHEAP


struct monster{
    int next;
    int speed;
    char type[5];
    int pox;
    int poy;
    int mx;
    int my;
};


struct monheap{
    struct monster *hp;
    int size;
};

void create_mon(struct monheap *h, int num);


void num_monsters(struct monheap *h, int x, int y);

void minsert(struct monheap *h, struct monster n);

struct monster mextractmin(struct monheap *h);

void mheapifydown(struct monheap *h, int i);

void mheapifyup(struct monheap *h, int i);

void mremove(struct monheap *h, int i);

#endif