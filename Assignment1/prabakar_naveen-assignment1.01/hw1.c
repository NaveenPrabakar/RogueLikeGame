#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 21 //MAX height
#define MAXR 80 //MAX Width


//prototypes for the methods
void build();
void place_rooms();
int check(int x, int y, int xm, int ym);
void connect_rooms();
void place_stairs();
void place_pc();
void assign_hardness();



//Rooms in the map, the center coordinate of the room and the top left corner of the grid
struct room{
    int cx; //center x
    int cy; // center y
    int lx; //top left x
    int ly; //top left y
    int d1; //width
    int d2; // height
};

//cell represents each coordinate in 2-d grid, has types which is #, . , <, >, ' '
//It also assigns hardness to the cell
struct cell{
    int hardness;
    char tile;
};


//struct room rooms[6]; //Array of the rooms (6 rooms at min)




struct cell grid[21][80]; //2-d board of cell


int main(int argc, char *argv[]){

    srand(time(NULL));
    build();

    //Changed from static to dynamic
    int ranroom = rand() % 4 + 6;
    struct room *r = (struct room *)malloc(ranroom * sizeof(struct room));

    place_rooms(r, ranroom);
    connect_rooms(r, ranroom);
    place_stairs();
    place_pc();
    assign_hardness();


    //print the board
    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            printf("%c", grid[i][j].tile);
        }

        printf("\n");
    }
}


//Build the intial set up of the map
void build(){

     for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAXR; j++) {
            grid[i][j].tile = ' ';  
        }
    }

    for(int i = 0; i < MAX; i++){
        grid[i][0].tile = '|';
        grid[i][79].tile = '|';
    }

    for(int i = 0; i < MAXR; i++){
        grid[0][i].tile = '-';
        grid[MAX-1][i].tile = '-';
    }
}


//randomly place six rooms, and place the rooms into an array
void place_rooms(struct room *rooms, int ranroom){
    srand(time(NULL));

    int z = 0;

    while(z != ranroom){
        int x = rand() % 79 + 1;
        int y = rand() % 20 + 1;
        int dim1 = rand() % 7 + 4;
        int dim2 = rand() % 4 + 3;

        if((x + dim1 < MAXR - 1) && (y + dim2 < MAX - 1)){

            
            if(check(x, y, dim2 + y, dim1 + x) == 1){
                for(int i = y; i < y + dim2; i++ ){
                for(int j = x; j < x + dim1; j++){
                    grid[i][j].tile = '.';
                }
            }
            
            //calculate the central coordinates of the rooms
            rooms[z].cy = y + (dim2/2);
            rooms[z].cx = x + (dim1/2);
            rooms[z].lx = x;
            rooms[z].ly = y;
            rooms[z].d1 = dim1;
            rooms[z].d2 = dim2;

            z++;
            }
        }
    }
}

//Check if there are other rooms around it or on it's dimension
int check(int x, int y, int xm, int ym){

    for(int i = y - 1; i < xm + 1; i++){
        for(int j = x - 1; j < ym + 1; j++){
            if(grid[i][j].tile == '.' || grid[i][j].tile == '-' || grid[i][j].tile == '|'){
                return 0;
            }
        }
    }

    return 1;
}



//carve the paths to the rooms
void connect_rooms(struct room *rooms, int ranroom) {
    srand(time(NULL)); 

    
    for (int i = 0; i < ranroom -1; i++) {
        int cx1 = rooms[i].cx;
        int cy1 = rooms[i].cy;
        int cx2 = rooms[i + 1].cx;
        int cy2 = rooms[i + 1].cy;

        int x = cx1;
        int y = cy1;

        //go to x coordinate first
        while (x != cx2) {

            if(x < cx2){
                x++;
            }
            else{
                x--;
            }
            
            if (grid[y][x].tile != '.' && grid[y][x].tile != '-' && grid[y][x].tile != '|') {
                grid[y][x].tile = '#';
            }
        }

       //go to y next
        while(y != cy2){
            if(y < cy2){
                y++;
            }
            else{
                y--;
            }
            
            if (grid[y][x].tile != '.' && grid[y][x].tile != '-' && grid[y][x].tile != '|') {
                grid[y][x].tile = '#';
            }
        }
    }
}

//place the stairs randomly in either a path or room
void place_stairs(){
    srand(time(NULL));

    int count = 0;

    while(count == 0){
        int x = rand() % 79 + 1;
        int y = rand() % 20 + 1;

        if(grid[y][x].tile == '.' || grid[y][x].tile == '#'){
            grid[y][x].tile = '<';
            count++;
        }
    } 

    while(count == 1){
        int x = rand() % 79 + 1;
        int y = rand() % 20 + 1;

        if(grid[y][x].tile == '.' || grid[y][x].tile == '#'){
            grid[y][x].tile = '>';
            count++;
        }
    }

}


//Place the player
void place_pc(){
    srand(time(NULL));

    int count = 0;

    while(count == 0){
        int x = rand() % 79 + 1;
        int y = rand() % 20 + 1;

        if(grid[y][x].tile == '#'){
            grid[y][x].tile = '@';
            count++;
        }
    }
}


//assign hardness to each cell
void assign_hardness(){

    srand(time(NULL));

    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            if(grid[i][j].tile == ' '){

                int h = rand() % 255;

                grid[i][j].hardness = h;
            }
            else if(grid[i][j].tile == '#' || grid[i][j].tile == '.' || grid[i][j].tile == '<' || grid[i][j].tile == '>'){
                grid[i][j].hardness = 0; //no hardness if it is a path or room
            }
            else if(grid[i][j].tile == '|' || grid[i][j].tile == '-'){
                grid[i][j].hardness = 255; //immutable wall
            }
        }
    }
}


