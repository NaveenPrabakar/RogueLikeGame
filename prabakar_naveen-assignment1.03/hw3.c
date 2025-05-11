#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <stdint.h>
#include <string.h>
#include "minheap.h"

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
void load(int x);
void save();
int strcmp();
void details();
void dijkstra();
struct node directions();
void printmap();
void dijkstra_tunnel();
void print_tunnel();


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

// upstairs struct
struct up{
    int ux;
    int uy;
};

//downstairs struct
struct down{
    int dx;
    int dy;
};

//player
struct player{
    int px;
    int py;
};

//map of the shortests paths.
struct map{
    int tunnel;
    int non;
};


//struct room rooms[6]; //Array of the rooms (6 rooms at min)


enum Directions {
    Northy = -1,         
    Northx = 0,          
    NorthEasty = -1,     
    NorthEastx = 1,      
    Easty = 0,           
    Eastx = 1,           
    SouthEasty = 1,      
    SouthEastx = 1,      
    Southy = 1,          
    Southx = 0,         
    SouthWesty = 1,      
    SouthWestx = -1,     
    Westy = 0,           
    Westx = -1,          
    NorthWesty = -1,     
    NorthWestx = -1,    
};




struct cell grid[21][80]; //2-d board of cell

struct map dist[21][80];


int main(int argc, char *argv[]){


    if(argc == 1){
        details(0);
        return 0;
    }

    if(argc == 3 && (strcmp(argv[1], "--load") == 0)){
        load(1); //load from disk and re-save
    }
    else if(argc == 3 && (strcmp(argv[1], "--save") == 0)){
        details(1);
    }

    else if(strcmp(argv[1], "--load") == 0){
        load(0); //load from disk only

        return 0;
    }
    else if(strcmp(argv[1], "--save") == 0){
        details(1); //save a randomly generated map
       
    }
    else{
        printf("Please pass in valid key (save or load)\n");
        return 0;
    }
}


void details(int key){
    srand(time(NULL));
        build();
        //Changed from static to dynamic
        int ranroom = rand() % 4 + 6;
        struct room *r = (struct room *)malloc(ranroom * sizeof(struct room));
        place_rooms(r, ranroom);
        connect_rooms(r, ranroom);

        int ranstair = rand() % 3 + 1;
        struct up *u = (struct up  *)malloc(ranstair * sizeof(struct up));

        int ranstairs = rand() % 3 + 1;
        struct down *d = (struct down *)malloc(ranstairs * sizeof(struct down));



        place_stairs(ranstair, u, ranstairs, d);

        struct player p;

        place_pc(&p);
        assign_hardness();

        for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            printf("%c", grid[i][j].tile);
        }

        printf("\n");
    }

    dijkstra(&p);
    dijkstra_tunnel(&p);



    if(key == 1){
        save(ranroom, r, ranstair, u, ranstairs, d, &p);
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
void place_stairs(int stairs_up, struct up *u, int stairs_down, struct down *d){
    srand(time(NULL));

    int count = 0;

    while(count != stairs_up){
        int x = rand() % 79 + 1;
        int y = rand() % 20 + 1;

        if(grid[y][x].tile == '.' || grid[y][x].tile == '#'){
            grid[y][x].tile = '<';
            u[count].ux = x;
            u[count].uy = y; 
            count++;
        }
    }

    count = 0;

    while(count != stairs_down){
        int x = rand() % 79 + 1;
        int y = rand() % 20 + 1;

        if(grid[y][x].tile == '.' || grid[y][x].tile == '#'){
            grid[y][x].tile = '>';
            d[count].dx = x;
            d[count].dy = y;
            count++;
        }
    }

}


//Place the player
void place_pc(struct player *p){
    srand(time(NULL));

    int count = 0;

    while(count == 0){
        int x = rand() % 79 + 1;
        int y = rand() % 20 + 1;

        if(grid[y][x].tile == '#'){
            grid[y][x].tile = '@';
            p->px = x;
            p->py = y;
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

                int h = rand() % 254 + 1;

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


//save the dungeon to a hidden directory
void save(int num_rooms, struct room *r, int up_stairs, struct up *u, int down_stairs, struct down *d, struct player *p){

    char *home = getenv("HOME");
    int len = strlen(home) + strlen("/.rlg327/dungeon") + 1;
    char *dungeon = malloc(len);
    strcpy(dungeon, home);
    strcat(dungeon, "/.rlg327/dungeon");


    FILE *file = fopen(dungeon, "wb"); //open the dungeon file and write binary

    fwrite("RLG327-S2025", 1, 12, file); //write the marker

    uint32_t x = 0; //unsigned 32 bit integer
    x = htobe32(x); //big endian order

    fwrite(&x, 4, 1, file); //write the intial value

    uint32_t y = 12 + 4 + 4 + 2 + 1680 + 2 + (num_rooms * 4) + 2 + (up_stairs * 2) + 2 + (down_stairs * 2); //total size
    y = htobe32(y);

    fwrite(&y, 4, 1, file);

    uint8_t z = p->px;
    uint8_t a = p->py;
    
    fwrite(&z, 1, 1, file); //pc x
    fwrite(&a, 1, 1, file); //pc y

    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            fwrite(&grid[i][j].hardness, 1, 1, file); //write the hardness levels
        }
    }


    uint16_t rom = num_rooms; 

    rom = htobe16(rom);

    fwrite(&rom, 2, 1, file); //num rooms

    for(int i = 0; i < num_rooms; i++){
        fwrite(&r[i].lx, 1, 1, file); // write the room details
        fwrite(&r[i].ly, 1, 1, file);
        fwrite(&r[i].d1, 1, 1, file);
        fwrite(&r[i].d2, 1, 1, file);
    }

    uint16_t up_rom = up_stairs;
    up_rom = htobe16(up_rom);

    fwrite(&up_rom, 2, 1, file); //number of upstairs

    for(int i = 0; i < up_stairs; i++){// the postions of the upstairs
        fwrite(&u[i].ux, 1, 1, file);
        fwrite(&u[i].uy, 1, 1, file);
    }

    uint16_t down_rom = down_stairs; //number of down stairs
    down_rom = htobe16(down_rom);

    fwrite(&down_rom, 2, 1, file);

    for(int i = 0; i < down_stairs; i++){ //write the down stairs
        fwrite(&d[i].dx, 1, 1, file);
        fwrite(&d[i].dy, 1, 1, file);
    }

}

//load the dungeon from the directory
void load(int key){

    char *home = getenv("HOME");
    int len = strlen(home) + strlen("/.rlg327/dungeon") + 1;
    char *dungeon = malloc(len);
    strcpy(dungeon, home);
    strcat(dungeon, "/.rlg327/dungeon");

    FILE *file = fopen(dungeon, "rb");

    char m[12]; //load in the RLG-COMS327
    fread(m, 1, 12, file); 

    uint32_t x; //load in the one byte value
    fread(&x, 4, 1, file);

    uint32_t total_size; // total size of the file
    fread(&total_size, 4, 1, file);

    struct player *p = malloc(sizeof(struct player));

    uint8_t pcx, pcy; 
    fread(&pcx, 1, 1, file); //player positions
    fread(&pcy, 1, 1, file); 
    p->px = pcx;
    p->py = pcy;

    build();


    for(int i = 0; i < MAX; i++){ //assign the hardness
        for(int j =0; j < MAXR; j++){
            uint8_t hard;
            fread(&hard, 1, 1, file);
            grid[i][j].hardness = hard;
        }
    }

    uint16_t num_rooms;
    fread(&num_rooms, 2, 1, file); //number of the rooms
    num_rooms = be16toh(num_rooms);

    struct room *r = (struct room *)malloc(num_rooms * sizeof(struct room));

    for(int i = 0; i < num_rooms; i++){
        fread(&r[i].lx, 1, 1, file); // write the room details
        fread(&r[i].ly, 1, 1, file);
        fread(&r[i].d1, 1, 1, file);
        fread(&r[i].d2, 1, 1, file);


        for(int j = r[i].ly; j < r[i].ly + r[i].d2; j++){
            for(int k = r[i].lx; k < r[i].lx + r[i].d1; k++){
                grid[j][k].tile = '.';
            }
        }
    }


    uint16_t num_up;
    fread(&num_up, 2, 1, file);
    num_up = be16toh(num_up); //number of upstairs

    struct up *u = (struct up  *)malloc(num_up * sizeof(struct up)); //upstairs array

    for(int i = 0; i < num_up; i++){
        fread(&u[i].ux, 1, 1, file);
        fread(&u[i].uy, 1, 1, file);

        grid[u[i].uy][u[i].ux].tile = '<';
    }


    uint16_t num_d;
    fread(&num_d, 2, 1, file);
    num_d = be16toh(num_up); //number of downstairs

    struct down *d = (struct down  *)malloc(num_d * sizeof(struct down));

    for(int i = 0; i < num_d; i++){
        fread(&d[i].dx, 1, 1, file);
        fread(&d[i].dy, 1, 1, file);

        grid[d[i].dy][d[i].dx].tile = '>';
    }


    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            if(grid[i][j].hardness == 0 && grid[i][j].tile == ' '){
                grid[i][j].tile = '#';
            }
        }
    }

    grid[p->py][p->px].tile = '@';
    grid[0][0].tile = '-';


    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            printf("%c", grid[i][j].tile);
        }

        printf("\n");
    }

    if(key == 1){
        save(num_rooms, r, num_up, u, num_d, d, p);
    }

    dijkstra(p);
    dijkstra_tunnel(p);
}


//Took the string compare that we did in class
int strcmp(const char *s1, const char *s2){

    int i;

    for(i = 0; s1[i] && s1[i] == s2[i]; i++);


    return s1[i] - s2[i]; 
}


//dijkstra's on non-tunneling monsters
void dijkstra(struct player *p){

    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            dist[i][j].non = 10000;
        }
    }

    struct minheap h; //create the min heap
    create(&h);


    //intial node
    struct node n;
    n.distance = 0; 
    n.x = p->px;
    n.y = p->py;

    dist[p->py][p->px].non = 0;

    insert(&h, n);

    while(h.size > 0){

        struct node temp = extractmin(&h);

        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node travel = directions(&temp, i);

            if(grid[travel.y][travel.x].tile == ' '){ //can't be on spaces
                continue;
            }

            if(travel.y == p->py && travel.x == p->px){
                continue;
            }

            if(travel.distance == 0){

                int new_dist = dist[temp.y][temp.x].non + 1;

                if(dist[travel.y][travel.x].non > new_dist){
                    dist[travel.y][travel.x].non = new_dist;

                    struct node ne;
                    ne.x = travel.x;
                    ne.y = travel.y;
                    ne.distance = new_dist;

                    insert(&h, ne);
                }
            }
        }
    }

    printf("\n");
    printmap(p->px, p->py);
}


void printmap(int x, int y){
    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            if(i == y && j == x){
                printf("@");
            }
            else if(dist[i][j].non != 10000){
                printf("%d", dist[i][j].non % 10);
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

void print_tunnel(int x, int y){
    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            if(i == y && j == x){
                printf("@");
            }
            else if(dist[i][j].tunnel != 10000){
                printf("%d", dist[i][j].tunnel % 10);
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

struct node directions(struct node *temp, int x){

    struct node correct;

    if(x == 0){
        correct.x = temp->x + Northx;
        correct.y = temp->y + Northy; 
    }
    else if(x == 1){
        correct.x = temp->x + NorthEastx;
        correct.y = temp->y + NorthEasty; 
    }
    else if(x == 2){
        correct.x = temp->x + Eastx;
        correct.y = temp->y + Easty; 
    }
    else if(x == 3){
        correct.x = temp->x + SouthEastx;
        correct.y = temp->y + SouthEasty; 
    }
    else if(x == 4){
        correct.x = temp->x + Southx;
        correct.y = temp->y + Southy; 
    }
    else if(x == 5){
        correct.x = temp->x + SouthWestx;
        correct.y = temp->y + SouthWesty; 
    }
    else if(x == 6){
        correct.x = temp->x + Westx;
        correct.y = temp->y + Westy; 
    }
    else if(x == 7){
        correct.x = temp->x + NorthWestx;
        correct.y = temp->y + NorthWesty; 
    }

    if(correct.x >= 1 && correct.x < MAXR - 1 && correct.y >= 1 && correct.y < MAX - 1){
        correct.distance = 0; //true

        return correct;
    }
    else{
        correct.distance = -1;
        return correct;
    }
}



//dijkstra's on non-tunneling monsters
void dijkstra_tunnel(struct player *p){

    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            dist[i][j].tunnel = 10000;
        }
    }

    struct minheap h; //create the min heap
    create(&h);


    //intial node
    struct node n;
    n.distance = 0; 
    n.x = p->px;
    n.y = p->py;

    dist[p->py][p->px].tunnel = 0;

    insert(&h, n);

    while(h.size > 0){

        struct node temp = extractmin(&h);

        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node travel = directions(&temp, i);

            if(travel.y == p->py && travel.x == p->px){
                continue;
            }

            if(travel.distance == 0){
                int weight;
                char t = grid[travel.y][travel.x].tile;

               
                weight = 1 + (grid[temp.y][temp.x].hardness / 85);

                int new_dist = dist[temp.y][temp.x].tunnel + weight;

                if(dist[travel.y][travel.x].tunnel > new_dist){
                    dist[travel.y][travel.x].tunnel = new_dist;

                    struct node ne;
                    ne.x = travel.x;
                    ne.y = travel.y;
                    ne.distance = new_dist;

                    insert(&h, ne);
                }
            }
        }
    }

    printf("\n");
    print_tunnel(p->px, p->py);

}