#ifndef Item_H
#define Item_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>  
#include <iostream>
#include <stdio.h>
#include <ncurses.h>



class Item{

    public:

    std::string name;
    std::string type;
    std::string color;
    std::string weight;
    std::string hit;
    std::string dam;
    std::string attr;
    std::string Val;
    std::string dodge;
    std::string defense;
    std::string speed;
    std::string art;
    int rrty;
    std::string desc;
    int len;

      Item():
        name("Goblin"), type("0000"), color("Green"), weight("0"), hit("0"),
        dam("0"), attr("0"), Val("0"), dodge("0"), defense("0"), speed("0"),
        art("0"), rrty(1), desc("A small, mischievous creature."), len(0) {}

};

class Item_parse{

    public:

    std::string filename;
    std::ifstream file;
    Item* item;

    std::vector<Item*> items;

        Item_parse(std::string filename) : filename(filename){
            file.open(filename);
        }

        void iparse(){
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
                    item->speed = i.substr(6);
                }
                else if(substrings2("END", i)){ // add the item to list
                    items.push_back(item); 
                }
                else if(substrings2("TYPE", i)){
                    item->type = i.substr(5);
                }
                else if(substrings2("WEIGHT", i)){
                    item->weight = i.substr(7);
                }
                else if(substrings2("HIT", i)){
                    item->hit = i.substr(4);
                }
                else if(substrings2("DAM", i)){
                    item->dam = i.substr(4);
                }
                else if(substrings2("ATTR", i)){
                    item->attr = i.substr(5);
                }
                else if(substrings2("VAL", i)){
                    item->Val = i.substr(4);
                }
                else if(substrings2("DODGE", i)){
                    item->dodge = i.substr(6);
                }
                else if(substrings2("DEF", i)){
                    item->defense = i.substr(4);
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
                mvprintw(j + 4, 1, "Weight: %s", temp->weight.c_str());
                mvprintw(j + 5, 1, "Hit: %s", temp->hit.c_str());
                mvprintw(j + 6, 1, "Dodge: %s", temp->dodge.c_str());
                mvprintw(j + 7, 1, "Defense: %s", temp->defense.c_str());
                mvprintw(j + 8, 1, "Speed: %s", temp->speed.c_str());
                mvprintw(j + 9, 1, "Art: %s", temp->art.c_str());
                mvprintw(j + 10, 1, "Val: %s", temp->Val.c_str());
                mvprintw(j + 11, 1, "Attr: %s", temp->attr.c_str());
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


};



#endif