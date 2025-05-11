#include <stdio.h>
#include "monsters.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>


void create_mon(MonsterHeap &h, int num) {
    h.size = 0;  
}


void minsert(MonsterHeap &h, Monster* n) {

    h.hp.push_back(n);  
    h.size++;
    mheapifyup(h, h.size - 1);  
}

void mheapifyup(MonsterHeap &h, int i) {
    int x = 0;
    while (i > 0 && x == 0) {
        int j = (i - 1) / 2;
        if (h.hp[j]->next > h.hp[i]->next) {
            Monster* temp = h.hp[i];
            h.hp[i] = h.hp[j];
            h.hp[j] = temp;
            i = j;
        } else {
            x = 1;
        }
    }
}

Monster* mextractmin(MonsterHeap &h) {

    Monster* minNode = h.hp[0];  
    h.hp[0] = h.hp[h.size - 1];  
    h.hp.pop_back();
    h.size--;
    mheapifydown(h, 0); 

    return minNode;
}

void mheapifydown(MonsterHeap &h, int i) {
    int x = 0;
    while (x == 0) {
        int l = (2 * i) + 1;
        int r = (2 * i) + 2;
        int s = i;

        if (l < h.size && h.hp[l]->next < h.hp[s]->next) {
            s = l;
        }
        if (r < h.size && h.hp[r]->next < h.hp[s]->next) {
            s = r;
        }

        if (s != i) {
            Monster* temp = h.hp[i];
            h.hp[i] = h.hp[s];
            h.hp[s] = temp; 
            i = s;
        } else {
            x = 1;
        }
    }
}


void mremove(MonsterHeap &h, int i) {

    h.hp[i] = h.hp[h.size - 1];  
    h.hp.pop_back();
    h.size--;
    mheapifyup(h, i);
    mheapifydown(h, i);
}


void num_monsters(MonsterHeap &h, int x, int y) {

    Monster* m = new Monster();  
    m->mx = x;
    m->my = y;

    
    int temp = rand() % 2;
    if(temp){
        m->type[0] = '1';
    }
    else{
        m->type[0] = '0';
    }

    temp = rand() % 2;

    if(temp){
        m->type[1] = '1';
    }
    else{
        m->type[1] = '0';
    }

    temp = rand() % 2;

    if(temp){
        m->type[2] = '1';
    }
    else{
        m->type[2] = '0';
    }

    temp = rand() % 2;

    if(temp){
        m->type[3] = '1';
    }
    else{
        m->type[3] = '0';
    }

    m->next = 0;
    m->speed = (rand() % 16) + 5;
    m->pox = -1;
    m->poy = -1;
    m->type[4] = '\0';

    minsert(h, m);  
}
