#ifndef Item_H
#define Item_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>  
#include <iostream>
#include <stdio.h>
#include <ncurses.h>
#include "Cell.h"
#include <unordered_set>

#include "Dice.h"

#define MAX 21 //MAX heights
#define MAXR 80 //MAX Width



class Item{

    public:

    std::string name;
    std::string type;
    std::string color;
    int weight;
    int hit;
    std::string dam;
    int attr;
    int Val;
    int dodge;
    int defense;
    int speed;
    std::string art;
    int rrty;
    std::string desc;
    int len;
    int ix;
    int iy;

    int actual;

    char symb;

      Item():
        name("Goblin"), type("0000"), color("Green"), weight(1), hit(0),
        dam("0"), attr(0), Val(0), dodge(0), defense(0), speed(0),
        art("0"), rrty(1), desc("A small, mischievous creature."), len(0), symb('v'), actual(0) {}

};

class Item_parse{

    public:

    std::string filename;
    std::ifstream file;
    Item* item;

    std::vector<Item*> items;
    std::vector<Item*> map_items;

        Item_parse(std::string filename) : filename(filename){
            file.open(filename);
        }

        void iparse(){
            Dice* d = new Dice("7+1d4");
            std::string i;
            while(std::getline(file, i)){
                if(substrings2("RLG", i)){ //skip the first line
                    continue;
                }
                else if(substrings2("BEGIN", i)){ //create the item
                    item = new Item();
                }
                else if(substrings2("NAME", i)){ // assign the name to the item
                    item->name = i.substr(5);
                }
                else if(substrings2("COLOR", i)){//assign the color to the item
                    item->color = i.substr(6);
                }
                else if(substrings2("RRTY", i)){ //assign the rarity to the item
                    item->rrty = std::stoi(i.substr(5)); 
                }
                else if(substrings2("SPEED", i)){ //assign the speed attribute to the item
                    d->setDice(i.substr(6));
                    item->speed = d->roll();
                }
                else if(substrings2("END", i)){ // add the item to list
                    items.push_back(item); 
                }
                else if(substrings2("TYPE", i)){
                    item->type = i.substr(5);
                    item_type(item);
                }
                else if(substrings2("WEIGHT", i)){
                    d->setDice(i.substr(7));
                    item->weight = d->roll();
                }
                else if(substrings2("HIT", i)){
                    d->setDice(i.substr(4));
                    item->hit = d->roll();
                }
                else if(substrings2("DAM", i)){
                    item->dam = i.substr(4);
                }
                else if(substrings2("ATTR", i)){
                    d->setDice(i.substr(5));
                    item->attr = d->roll();
                }
                else if(substrings2("VAL", i)){
                    d->setDice(i.substr(4));
                    item->Val = d->roll();
                }
                else if(substrings2("DODGE", i)){
                    d->setDice(i.substr(6));
                    item->dodge = d->roll();
                }
                else if(substrings2("DEF", i)){
                    d->setDice(i.substr(4));
                    item->defense = d->roll();
                }
                else if(substrings2("ART", i)){
                    item->art = i.substr(4);
                }
                else if(substrings2("DESC", i)){
                    idesc();
                }
            }
        }

        void idesc(){
            std::string checks;
            std::string total;

            int count = 0;
            
            while(std::getline(file, checks)){
                if(checks == "."){
                    break;
                }
                total += checks + "\n"; 
                count++;
           }
    
           item->desc = total;
           item->len = count;
        }

        void print_item(){

            clear();
            refresh();
    
            int offset = 0;
    
            while(1){
                clear();
            mvprintw(0, 1, "ITEM LIST: [esc] to escape");
            mvprintw(1, 1, "--------------------------------");
    
            int j = 0;
    
    
                Item* temp = items[offset];
    
                mvprintw(j + 2, 1, "Name: %s", temp->name.c_str());
                mvprintw(j + 3, 1, "Description: %s", temp->desc.c_str());

                j = j + temp->len + 2;


                mvprintw(j + 1, 1, "Color: %s", temp->color.c_str());
                mvprintw(j + 2, 1, "Damage: %s", temp->dam.c_str());
                mvprintw(j + 3, 1, "Rarity: %d", temp->rrty);
                mvprintw(j + 4, 1, "Weight: %d", temp->weight);
                mvprintw(j + 5, 1, "Hit: %d", temp->hit);
                mvprintw(j + 6, 1, "Dodge: %d", temp->dodge);
                mvprintw(j + 7, 1, "Defense: %d", temp->defense);
                mvprintw(j + 8, 1, "Speed: %d", temp->speed);
                mvprintw(j + 9, 1, "Art: %s", temp->art.c_str());
                mvprintw(j + 10, 1, "Val: %d", temp->Val);
                mvprintw(j + 11, 1, "Attr: %d", temp->attr);
                mvprintw(j + 12, 1, "Type: %s", temp->type.c_str());

    
            refresh();
    
            int mov = getch();
    
        if(mov == KEY_RIGHT){
            if(offset + 1 != items.size()){
                offset++;
            }
        }
        else if(mov == KEY_LEFT){
            if(offset - 1 != -1){
                offset--;
            }
        }
        else if(mov == 27){
            break;
        }
        refresh();
        }
        refresh();
    clear();
        }

        bool substrings2(const std::string& one, const std::string& two){
            int count = 0;
    
            for(int i = 0; i < two.length() && i < one.length(); i++){
    
                if(two[i] == one[i]){
                    count++;
                }
            }
    
            if(count == one.length()){
                return true;
            }
    
            return false;
        }

        void item_type(Item* i){
            std::string temp = i->type;

            if(temp == "WEAPON"){
                i->symb = '|';
            }
            else if(temp == "OFFHAND"){
                i->symb = ')';
            }
            else if(temp == "RANGED"){
                i->symb = '}';
            }
            else if(temp == "ARMOR"){
                i->symb = '[';
            }
            else if(temp == "HELMET"){
                i->symb = ']';
            }
            else if(temp == "CLOAK"){
                i->symb = '(';
            }
            else if(temp == "GLOVES"){
                i->symb = '{';
            }
            else if(temp == "BOOTS"){
                i->symb = '\\';
            }
            else if(temp == "RING"){
                i-> symb = '=';
            }
            else if(temp == "AMULET"){
                i->symb = '"';
            }
            else if(temp == "LIGHT"){
                i->symb = '_';
            }
            else if(temp == "SCROLL"){
                i->symb = '~';
            }
            else if(temp == "BOOK"){
                i->symb = '?';
            }
            else if(temp == "FLASK"){
                i->symb = '!';
            }
            else if(temp == "GOLD"){
                i->symb = '$';
            }
            else if(temp == "AMMUNITION"){
                i->symb = '/';
            }
            else if(temp == "FOOD"){
                i->symb = ',';
            }
            else if(temp == "WAND"){
                i->symb = '-';
            }
            else if(temp == "CONTAINER"){
                i->symb = '%';
            }
        }

        void place_items(Cell grid[MAX][MAXR], std::unordered_set<std::string> deleted){

            int max = 0;

            std::unordered_set<std::string> unique; //uniqueness of the monsters

            while(max != 10){

                int i = rand() % items.size();
                Item* temp = items[i];

                if(unique.find(temp->name) != unique.end() || deleted.find(temp->name) != unique.end()){
                    continue;
                }

                int spawn = rand() % 100;

                if(spawn >= temp->rrty){
                    continue;
                }

                int count  = 0;
                int x = 0;
                int y = 0;

                while(count == 0){
                    x = rand() % 78 + 1;
                    y = rand() % 19 + 1;
                    
                    if(grid[y][x].tile == '.'){  
                            count++;
                    }
                       
                }

                grid[y][x].tile = items[i]->symb;
                items[i]->ix = x;
                items[i]->iy = y;

                max++;
                unique.insert(temp->name);
                map_items.push_back(temp);
            }
        }

        std::vector<Item*> get_list(){
            return map_items;
        }








};



#endif