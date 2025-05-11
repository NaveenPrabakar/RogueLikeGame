#ifndef PLAYER_H
#define PLAYER_H

#include "Item.h"
#include "Dice.h"
#include "monsters.h"



class Player {
    public:
        int px;
        int py;
        char pr;

        int hp;
        int attack;

        int speed;

        Item* weapon;
        Item* offhand;
        Item* ranged;
        Item* armor;
        Item* helmet;
        Item* cloak;
        Item* gloves;
        Item* boots;
        Item* amulet;
        Item* light;
        Item* ring1;
        Item* ring2;

        int exp;
        



        std::vector<Item*> inventory;
        std::unordered_set<std::string> deleted;

       
        Player() : px(0), py(0), pr('P'), hp(100), attack(0), speed(10), exp(0) {}

        bool pickup(Item* i){
            if(inventory.size() < 10){
                inventory.push_back(i);
                return true;
            }
            return false;
        }

        void swap_item(int position, Item*& swapped){
            Item* temp = swapped;
            swapped = inventory[position];
            inventory[position] = temp;
        }

        void equip(int position){

            Dice* d = new Dice("7+1d4");

            if(position >= 0 && position < inventory.size()){
                std::string temp = inventory[position]->type;

            if(temp == "WEAPON"){
                if(!weapon){
                    weapon = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    d->setDice(weapon->dam);
                    attack += d->roll();
                    return;
                }
                swap_item(position, weapon);
                attack = 0;
                d->setDice(weapon->dam);
                attack += d->roll();

                return;
            }
            else if(temp == "OFFHAND"){

                if(!offhand){
                    offhand = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    hp += offhand->defense;
                    return;
                }
                hp -= offhand->defense;
                swap_item(position, offhand);
                hp += offhand->defense;

                return;
            }
            else if(temp == "RANGED"){
                if(!ranged){
                    ranged = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    return;
                }
                swap_item(position, ranged);
            }
            else if(temp == "ARMOR"){
                if(!armor){
                    armor = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    hp += armor->defense;
                    return;
                }
                hp -= armor->defense;
                swap_item(position, armor);
                hp += armor->defense;
            }
            else if(temp == "HELMET"){
                if(!helmet){
                    helmet = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    hp += helmet->defense;
                    return;
                }
                hp -= helmet->defense;
                swap_item(position, helmet);
                hp += helmet->defense;
            }
            else if(temp == "CLOAK"){
                if(!cloak){
                    cloak = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    hp += cloak->defense;
                    return;
                }
                hp -= cloak->defense;
                swap_item(position, cloak);
                hp += cloak->defense;
            }
            else if(temp == "GLOVES"){
                if(!gloves){
                    gloves = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    hp += gloves->defense;
                    return;
                }
                hp -= gloves->defense;
                swap_item(position, gloves);
                hp += gloves->defense;
            }
            else if(temp == "BOOTS"){
                if(!boots){
                    boots = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    hp += boots->defense;
                    return;
                }
                hp -= boots->defense;
                swap_item(position, boots);
                hp += boots->defense;
            }
            else if(temp == "RING"){
                if(!ring1){
                    ring1 = inventory[position];
                    inventory.erase(inventory.begin() + position);

                    if(substrings3("damage", ring1->name)){
                        d->setDice(ring1->dam);
                        ring1->actual = d->roll();
                        attack += ring1->actual;
                    }
                    else{
                        speed += ring1->speed;
                    }
                    return;
                }
                else if(!ring2){
                    ring2 = inventory[position];
                    inventory.erase(inventory.begin() + position);

                    if(substrings3("damage", ring2->name)){
                        d->setDice(ring2->dam);
                        ring2->actual = d->roll();
                        attack += ring2->actual;
                    }
                    else{
                        speed += ring1->speed;
                    }

                    return; 
                }
                
                else{
                    
                    if(substrings3("damage", ring2->name)){
                        attack -= ring2->actual;
                    }
                    else{
                        speed -= ring2->speed;
                    }

                    swap_item(position, ring2);

                    if(substrings3("damage", ring2->name)){
                        d->setDice(ring2->dam);
                        ring2->actual = d->roll();
                        attack += ring2->actual;
                    }
                    else{
                        speed += ring2->speed;
                    }

                }
            }
            else if(temp == "AMULET"){
                if(!amulet){
                    amulet = inventory[position];
                    inventory.erase(inventory.begin() + position);
                    hp += amulet->defense;
                    return;
                }
                hp -= amulet->defense;
                swap_item(position, amulet);
                hp += amulet->defense;
            }
            else if(temp == "LIGHT"){
                if(!light){
                    light = inventory[position];
                    inventory.erase(inventory.begin() + position);
                }

                swap_item(position, light);
            }
            //inventory.erase(inventory.begin() + position);

            }
        }

        void print_inventory(std::vector<Item*> im){ //print the inventory, also give options to drop / equip
            clear();
            refresh();

            int offset = 0;

            while(1){
                mvprintw(0, 1, "Inventory: [esc] to escape [down arrow] to go down, [up arrow] for up");
                mvprintw(1, 1, "--------------------------------");

                for(int i = 0; i < inventory.size(); i++){
                    if(i == offset){
                        mvprintw(i + 2, 1, "%s  <-------", inventory[i]->name.c_str());
                    }
                    else{
                        mvprintw(i + 2, 1, "%s", inventory[i]->name.c_str());
                    }
                }

                int move = getch();

                if(move == KEY_DOWN){
                    if(offset != inventory.size() - 1){
                        offset++;
                    }
                }
                else if(move == KEY_UP){
                    if(offset != 0){
                        offset--;
                    }
                }
                else if(move == 'w'){
                    equip(offset);
                    offset = 0;
                }
                else  if(move == 'd'){
                    if(inventory.size() != 0){
                        inventory[offset]->ix = px;
                        inventory[offset]->iy = py;
                        im.push_back(inventory[offset]);
                        pr = inventory[offset]->symb;
                        inventory.erase(inventory.begin() + offset);
                    }
                }
                else if(move == 'I'){
                    clear();

                    while(1){
                        mvprintw(0, 1, "Description: [esc] to escape");
                        mvprintw(1, 1, "%s", inventory[offset]->name.c_str());
                        mvprintw(2, 1, "DAM: %s", inventory[offset]->dam.c_str());
                        mvprintw(3, 1, "Defense: %d", inventory[offset]->defense);
                        mvprintw(4, 1, "Speed: %d", inventory[offset]->speed);
                        mvprintw(6, 1, "%s", inventory[offset]->desc.c_str());

                        int ch = getch();

                        if(ch == 27){
                            break;
                        }
                    }


                }
                else if(move == 'x' && inventory.size() != 0){
                    deleted.insert(inventory[offset]->name);
                    inventory.erase(inventory.begin() + offset);
                }
                else if(move == 27){
                    break;
                }

                clear();
            }
        }

        void print_equip(){
            clear();
            refresh();

            int offset = 0;

            while(1){
                mvprintw(0, 1, "Equipment: [esc] to escape,[down arrow] to go down, [up arrow] for up ");
                if(!weapon){
                    mvprintw(1, 1, "WEAPON: FISTS");
                }
                else{
                    mvprintw(1, 1, "WEAPON: %s", weapon->name.c_str());
                }

                if(!offhand){
                    mvprintw(2, 1, "OFFHAND: NONE");
                } else {
                    mvprintw(2, 1, "OFFHAND: %s", offhand->name.c_str());
                }
        
                if(!ranged){
                    mvprintw(3, 1, "RANGED: NONE");
                } else {
                    mvprintw(3, 1, "RANGED: %s", ranged->name.c_str());
                }
        
                if(!armor){
                    mvprintw(4, 1, "ARMOR: NONE");
                } else {
                    mvprintw(4, 1, "ARMOR: %s", armor->name.c_str());
                }
        
                if(!helmet){
                    mvprintw(5, 1, "HELMET: NONE");
                } else {
                    mvprintw(5, 1, "HELMET: %s", helmet->name.c_str());
                }
        
                if(!cloak){
                    mvprintw(6, 1, "CLOAK: NONE");
                } else {
                    mvprintw(6, 1, "CLOAK: %s", cloak->name.c_str());
                }
        
                if(!gloves){
                    mvprintw(7, 1, "GLOVES: NONE");
                } else {
                    mvprintw(7, 1, "GLOVES: %s", gloves->name.c_str());
                }
        
                if(!boots){
                    mvprintw(8, 1, "BOOTS: NONE");
                } else {
                    mvprintw(8, 1, "BOOTS: %s", boots->name.c_str());
                }
        
                if(!amulet){
                    mvprintw(9, 1, "AMULET: NONE");
                } else {
                    mvprintw(9, 1, "AMULET: %s", amulet->name.c_str());
                }
        
                if(!light){
                    mvprintw(10, 1, "LIGHT: NONE");
                } else {
                    mvprintw(10, 1, "LIGHT: %s", light->name.c_str());
                }
        
                if(!ring1){
                    mvprintw(11, 1, "RING 1: NONE");
                } else {
                    mvprintw(11, 1, "RING 1: %s", ring1->name.c_str());
                }
        
                if(!ring2){
                    mvprintw(12, 1, "RING 2: NONE");
                } else {
                    mvprintw(12, 1, "RING 2: %s", ring2->name.c_str());
                }
        
                mvprintw(offset + 1, 40, "<-------");

                int move = getch();
                
                if(move == KEY_DOWN){
                    if(offset < 11) offset++;
                }
                else if(move == KEY_UP){
                    if(offset > 0) offset--;
                }
                else if(move == 27){
                    break;
                }
                else if(move == 't'){
                    if(inventory.size() >= 10){
                        mvprintw(13, 1, "Inventory is full, cannot take off item");
                    }
                    else{

                        if(offset == 0 && weapon){
                            Item* r = weapon;
                            inventory.push_back(r);
                            weapon = nullptr;
                            attack = 0;
                        }
                        else if(offset == 1 && offhand){
                            hp -= offhand->defense;
                            Item* r = offhand;
                            inventory.push_back(r);
                            offhand = nullptr;
                        }
                        else if(offset == 2 && ranged){
                            Item* r = ranged;
                            inventory.push_back(r);
                            ranged = nullptr;

                        }
                        else if(offset == 3 && armor){
                            hp -= armor->defense;
                            Item* r = armor;
                            inventory.push_back(r);
                            armor = nullptr;
                        }
                        else if(offset == 4 && helmet){
                            hp -= helmet->defense;
                            Item* r = helmet;
                            inventory.push_back(r);
                            helmet = nullptr;
                        }
                        else if(offset == 5 && cloak){
                            hp -= cloak->defense;
                            Item* r = cloak;
                            inventory.push_back(r);
                            cloak = nullptr;

                        }
                        else if(offset == 6 && gloves){
                            hp -= gloves->defense;
                            Item* r = gloves;
                            inventory.push_back(r);
                            gloves = nullptr;
                        }
                        else if(offset == 7 && boots){
                            hp -= boots->defense;
                            Item* r = boots;
                            inventory.push_back(r);
                            boots = nullptr;
                        }
                        else if(offset == 8 && amulet){
                            hp -= amulet->defense;
                            Item* r = amulet;
                            inventory.push_back(r);
                            amulet = nullptr;
                        }
                        else if(offset == 9 && light){
                            Item* r = light;
                            inventory.push_back(r);
                            light = nullptr;
                        }
                        else if(offset == 10 && ring1){

                            if(ring1->actual == 0){
                                speed -= ring1->speed;
                            }
                            else{
                                attack -= ring1->actual;
                            }
                            Item* r = ring1;
                            inventory.push_back(r);
                            ring1 = nullptr;
                        }
                        else if(offset == 11 && ring2){

                            if(ring2->actual == 0){
                                speed -= ring2->speed;
                            }
                            else{
                                attack -= ring2->actual;
                            }

                            Item* r = ring2;
                            inventory.push_back(r);
                            ring2 = nullptr;
                        }
                    }
                }
                
                clear();

            }
        }

        bool substrings3(const std::string& one, const std::string& two){
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

        void setSpeed(Monster* human){
            human->speed = speed;
        }

    };

#endif