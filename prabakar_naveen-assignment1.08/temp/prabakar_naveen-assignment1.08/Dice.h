#ifndef DICE_H
#define DICE_H

#include <string>
#include <cstdlib>
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>


class Dice{
    public:
      int base;
      int dice;
      int side;

    Dice(const std::string& parse){

        std:: string temp = "";

        for(char c : parse){
            if(c == '+'){
                base = std::stoi(temp);
                temp = "";
            }
            else if(c == 'd'){
                dice = std::stoi(temp);
                temp = "";
            }
            else{
                temp += c;
            }
        }

        side = std::stoi(temp);
    }

    int roll(){

        int total = 0;

        for(int i = 0; i < dice; i++){
            total += rand() % side + 1;
        }

        return total + base;
    }

    void setDice(const std::string& parse){
        std:: string temp = "";

        for(char c : parse){
            if(c == '+'){
                base = std::stoi(temp);
                temp = "";
            }
            else if(c == 'd'){
                dice = std::stoi(temp);
                temp = "";
            }
            else{
                temp += c;
            }
        }

        side = std::stoi(temp);
    }
};

#endif