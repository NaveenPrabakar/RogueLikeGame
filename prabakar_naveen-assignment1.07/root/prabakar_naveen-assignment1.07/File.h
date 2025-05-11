#ifndef FILE_H
#define FILE_H

#include <string>
#include <cstdlib>
#include "monsters.h"
#include <fstream>
#include <vector>  
#include <iostream>
#include <stdio.h>
#include <ncurses.h>

class File{
    public:
       std::string filename;
       std::ifstream file;
       Monster* monster; // Monster

       MonsterHeap h;
       

       File(std::string filename) : filename(filename) { //opens the fine
           file.open(filename);
           create_mon(h, 2);
       }

       //Parsing the file
       void parse(){

              std::string s;
              while (std::getline(file, s)) {

                if(substrings("RLG", s)){ //skip the first line
                    continue;
                }
                else if(substrings("BEGIN", s)){ //create a monster
                    monster = new Monster();
                }
                else if(substrings("END", s)){ // add the monster to list
                    minsert(h, monster);
                }
                else if(substrings("NAME", s)){// assign the name to the monster
                    monster->name = s.substr(5);
                }
                else if(substrings("SYMB", s)){//assign the symbol to the monster
                    monster->symb = s.substr(5);
                }
                else if(substrings("COLOR", s)){//assign the color to the monster
                    monster->color = s.substr(6);
                }
                else if(substrings("RRTY", s)){ //assign the rarity to the monster
                    monster->rrty = std::stoi(s.substr(5)); 
                }
                else if(substrings("SPEED", s)){ //assign the speed attribute to the monster
                    monster->calc = s.substr(6);
                }
                else if(substrings("HP", s)){// assign the hp
                    monster->hp = s.substr(3); 
                }
                else if(substrings("DAM", s)){// assign the damage to the monster
                    monster->dam = s.substr(4); 
                }
                else if(substrings("DESC", s)){
                    desc();
                }
                else if(substrings("ABIL", s)){
                    abil(s);
                }

              }
       }

       void desc(){
        std::string s;
        std::string total;
        int count = 0;
        while(std::getline(file, s)){
            if(s == "."){
                break;
            }
            total += s + "\n"; 
            count++;
       }

       monster->desc = total;
       monster->len = count;
    }

    void abil(std::string& t){

        std::string total;

       for(char c : t){
             if(c == ' '){
                 monster->ability.push_back(total);
                 
                 if(total == "SMART"){
                     monster->type[3] = '1';
                 }
                 else if(total == "TELE"){
                     monster->type[2] = '1';
                 }
                 else if(total == "TUNNEL"){
                     monster->type[1] = '1';
                 }
                 else if(total == "ERRATIC"){
                     monster->type[0] = '1';
                 }

                 total = "";
                 total.clear();
             }
             else{
                 total += c; 
             }
         }

        if(total == "SMART"){
            monster->type[3] = '1';
        }
        else if(total == "TELE"){
            monster->type[2] = '1';
        }
        else if(total == "TUNNEL"){
            monster->type[1] = '1';
        }
        else if(total == "ERRATIC"){
            monster->type[0] = '1';
        }
     }
 

    void print_file(){

        clear();

        int offset = 0;

        while(1){

            clear();
            refresh();
            
        mvprintw(0, 1, "MONSTER Description: [esc] to escape");
        mvprintw(1, 1, "--------------------------------");

        int j = 0;


            Monster* temp = h.hp[offset];

            mvprintw(j + 2, 1, "%s", temp->name.c_str());
            mvprintw(j + 3, 1, "%s", temp->desc.c_str());

            j = j + temp->len + 2;

            mvprintw(j + 1, 1, "%s", temp->symb.c_str());
            mvprintw(j + 2, 1, "%s", temp->color.c_str());
            mvprintw(j + 3, 1, "%s", temp->calc.c_str());
            print_ability(*temp, j + 4);
            mvprintw(j + 5, 1, "%s", temp->hp.c_str());
            mvprintw(j + 6, 1, "%s", temp->dam.c_str());
            mvprintw(j + 7, 1, "%d", temp->rrty);


        refresh();

        int mov = getch();

    if(mov == KEY_RIGHT){
        if(offset + 1 != h.size){
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

    void print_ability(Monster& temp, int k){
        std::string abilities = "";

        for (int i = 0; i < temp.ability.size(); i++) {
            abilities += temp.ability[i] + " ";
        }
        
        mvprintw(k, 1, "%s", abilities.c_str());
    }

    bool substrings(const std::string& one, const std::string& two){
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