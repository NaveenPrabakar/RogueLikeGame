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

        std::string name;
        std::string symb;
        std::string color;
        std::string desc;
        std::string calc;
        std::string dam; 
        std::string hp;  
        int rrty; 
        std::vector<std::string> ability;
        int len;

        Monster() 
        : next(0), speed(0), type("P"), pox(-1), poy(-1), mx(0), my(0), 
          name("Goblin"), symb("G"), color("Green"), desc("This is you. What are you really?"), 
          dam("34"), hp("s"), rrty(1), calc("d"), len(0) {}
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