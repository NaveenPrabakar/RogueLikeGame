#ifndef CELL_H
#define CELL_H


class Cell {
    public:
        int hardness;
        char tile;
        char tile2;
        int on;
        int explored;
    
        
        Cell() : hardness(0), tile(' '), tile2(' '), on(-1), explored(-1) {}
    };
    
#endif 