#ifndef MONSTERHEAP
#define MONSTERHEAP
#include <string>
#include <vector>


class Monster {
    public:
        int next;
        int speed;
        char type[5];
        int pox;
        int poy;
        int mx;
        int my;

        Monster() : next(0), speed(0), type("0001"), pox(-1), poy(-1), mx(0), my(0) {}

    
    };


class MonsterHeap {
    public:
        std::vector<Monster*> hp;  
        int size;

        MonsterHeap() : size(0) {}
};

void create_mon(MonsterHeap &h, int num);
void num_monsters(MonsterHeap &h, int x, int y);
void minsert(MonsterHeap &h, Monster *n);
Monster* mextractmin(MonsterHeap &h);
void mheapifydown(MonsterHeap &h, int i);
void mheapifyup(MonsterHeap &h, int i);
void mremove(MonsterHeap &h, int i);
        
#endif