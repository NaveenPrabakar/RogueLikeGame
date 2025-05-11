#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "minheap.h"
#include "monsters.h"
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

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
int gameplay();
bool checkRoom();
void mon1();
void mon2();
void mon3();
void mon4();
void mon5();
void mon6();
void mon7();
void mon8();
void mon9();
void mon10();
void mon11();
void mon12();
void mon13();
void mon14();
void mon15();
void mon16();
void printmon();
void ncurse();
bool vmove();
void monlist();


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
    char tile2;
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
    char pr;
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

    srand(time(NULL));

    ncurse();


    if(argc == 1 || argc == 2){
        details(0, atoi(argv[1]));
    }

    if(argc == 3 && (strcmp(argv[1], "--load") == 0)){
        load(1); //load from disk and re-save
    }
    else if(argc == 3 && (strcmp(argv[1], "--save") == 0)){
        details(1);
    }

    else if(strcmp(argv[1], "--load") == 0){
        load(0); //load from disk only

    }
    else if(strcmp(argv[1], "--save") == 0){
        details(1); //save a randomly generated map
       
    }
    else{
        printf("Please pass in valid key (save or load)\n");

    }

    endwin();
}

void ncurse() {
    initscr();                           
    keypad(stdscr, TRUE);   
    noecho();                        
}

void monlist(struct monheap *h , int x, int y){

    clear();

    int offset = 0;


    while(1){

        clear();
    
    mvprintw(0, 1, "MONSTER LIST: [esc] to escape");
    mvprintw(1,1, "------------------------------");

    int j = 0;

    for(int i = offset; i < h->size && i < 30 + offset; i++){

        struct monster temp = h->hp[i];

        if(strcmp(temp.type, "P") == 0){
            continue;
        }
        int tempx = temp.mx - x;
        int tempy = temp.my - y;

        mvprintw(j + 2, 1, "Monster %c", grid[temp.my][temp.mx].tile2);

        if(tempx < 0){
            mvprintw(j + 2, 18, "West %d", abs(tempx));
        }
        else{
            mvprintw(j + 2, 18, "East %d", abs(tempx));
        }

        if(tempy < 0){
            mvprintw(j + 2, 27, "North %d", abs(tempy));
        }
        else{
            mvprintw(j + 2, 27, "South %d", abs(tempy));
        }

        j++;
    }

    refresh();

    int mov = getch();

    if(mov == KEY_DOWN){
        if(offset + 30 < h->size){
            offset++;
        }
    }
    else if(mov == KEY_UP){
        if(offset > 0){
            offset--;
        }
    }
    else if(mov == 27){
        break;
    }
}
}



void details(int key, int num_mon){

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

    int restart = gameplay(&p, num_mon, ranroom, r);

    if(key == 1){
        save(ranroom, r, ranstair, u, ranstairs, d, &p);
    }

    //free memory
    free(r);
    free(u);
    free(d);

    if(restart == 0){
        details(key, num_mon);
    }
    
}


//Build the intial set up of the map
void build(){

     for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAXR; j++) {
            grid[i][j].tile = ' ';  
            grid[i][j].tile2 = '+';
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

    int count = 0;

    while(count == 0){
        int x = rand() % 79 + 1;
        int y = rand() % 20 + 1;

        if(grid[y][x].tile == '#' || grid[y][x].tile == '.'){

            if(grid[y][x].tile == '#'){
                p->pr = '#';
            }
            else{
                p->pr = '.';
            }
            grid[y][x].tile = '@';
            p->px = x;
            p->py = y;
            count++;
        }
    }
}


//assign hardness to each cell
void assign_hardness(){
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
}


int gameplay(struct player *n, int num, int numr, struct room *r){


    //create the monster heap
    struct monheap h;
    create_mon(&h, num);
    

    //insert the player
    struct monster p;
    p.next = 0;
    p.speed = 10;
    p.type[0] = 'P';
    p.type[1] = '\0';
    p.pox = -1;
    p.poy = -1;
    p.mx = n->px;
    p.my = n->py;

    minsert(&h, p);

    grid[n->py][n->px + 1].tile = '>';

    int f = 0;

    int x, y;

    while(f != num){
        
        int count = 0;

        while(count == 0){
            x = rand() % 78 + 1;
            y = rand() % 19 + 1;
            
            if(grid[y][x].tile == '#' || grid[y][x].tile == '.'){  //make sure monsters are placed on different squares

                    count++;
            }
               
        }

    num_monsters(&h, x, y);

    f++;


    }

    // struct monster w;
    // w.next = 0;
    // w.speed = 10;
    // w.type[0] = '1';
    // w.type[1] = '0';
    // w.type[2] = '0';
    // w.type[3] = '1';
    // w.type[4] = '\0';
    // w.pox = -1;
    // w.poy = -1;
    // w.mx = n->px;
    // w.my = n->py;

    //minsert(&h, w);

    //place the monsters on the grid
    for(int i = 0; i < h.size; i++){
        if(strcmp(h.hp[i].type, "0000") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '0';
        }
        else if(strcmp(h.hp[i].type, "0001") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '1';
        }
        else if(strcmp(h.hp[i].type, "0010") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '2';
        }
        else if(strcmp(h.hp[i].type, "0011") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '3';
        }
        else if(strcmp(h.hp[i].type, "0100") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '4';
        }
        else if(strcmp(h.hp[i].type, "0101") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '5';
        }
        else if(strcmp(h.hp[i].type, "0110") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '6';
        }
        else if(strcmp(h.hp[i].type, "0111") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '7';
        }
        else if(strcmp(h.hp[i].type, "1000") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '8';
        }
        else if(strcmp(h.hp[i].type, "1001") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = '9';
        }
        else if(strcmp(h.hp[i].type, "1010") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = 'a';
        }
        else if(strcmp(h.hp[i].type, "1011") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = 'b';
        }
        else if(strcmp(h.hp[i].type, "1100") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = 'c';
        }
        else if(strcmp(h.hp[i].type, "1101") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = 'd';
        }
        else if(strcmp(h.hp[i].type, "1110") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = 'e';
        }
        else if(strcmp(h.hp[i].type, "1111") == 0){
            grid[h.hp[i].my][h.hp[i].mx].tile2 = 'f';
        }
    }
    printmon();

    int z = 0;


    printmon();

    while(h.size != 0){
        struct monster m = mextractmin(&h);

        if(strcmp(m.type, "P") == 0){ //skip for now, player doesn't move


            int mov = getch();

            if(mov == 'm'){
                while(1){
                    monlist(&h, n->px, n->py);
                    printmon();
                    mov = getch();

                    if(mov != 'm'){
                        break;
                    }
                }
            }

            if(mov == '7' || mov == 'y'){
                int n_y = n->py - 1;
                int n_x = n->px - 1;

                if(vmove(n_y, n_x)){
                    grid[n->py][n->px].tile = n->pr;
                    n->py = n->py - 1;
                    n->px = n->px - 1;
                    n->pr = grid[n->py][n->px].tile;
                    grid[n->py][n->px].tile = '@';
                }
            }
            else if(mov == '8' || mov == 'k'){
                int n_y = n->py - 1;
                int n_x = n->px;

                if(vmove(n_y, n_x)){
                    grid[n->py][n->px].tile = n->pr;
                    n->py = n->py - 1;
                    n->px = n->px;
                    n->pr = grid[n->py][n->px].tile;

                    grid[n->py][n->px].tile = '@';
                }
            }
            else if(mov == '9' || mov == 'u'){
                int n_y = n->py - 1;
                int n_x = n->px + 1;

                if(vmove(n_y, n_x)){
                    grid[n->py][n->px].tile = n->pr;
                    n->py = n->py - 1;
                    n->px = n->px + 1;

                    n->pr = grid[n->py][n->px].tile;

                    grid[n->py][n->px].tile = '@';
                }
            }
            else if(mov == '6' || mov == 'l'){
                int n_y = n->py;
                int n_x = n->px + 1;

                if(vmove(n_y, n_x)){
                    grid[n->py][n->px].tile = n->pr;
                    n->py = n->py;
                    n->px = n->px + 1;

                    n->pr = grid[n->py][n->px].tile;

                    grid[n->py][n->px].tile = '@';
                }
            }
            else if(mov == '3' || mov == 'n'){
                int n_y = n->py + 1;
                int n_x = n->px + 1;

                if(vmove(n_y, n_x)){
                    grid[n->py][n->px].tile = n->pr;
                    n->py = n->py + 1;
                    n->px = n->px + 1;

                    n->pr = grid[n->py][n->px].tile;

                    grid[n->py][n->px].tile = '@';
                }
            }
            else if(mov == '2' || mov == 'j'){
                int n_y = n->py + 1;
                int n_x = n->px;

                if(vmove(n_y, n_x)){
                    grid[n->py][n->px].tile = n->pr;
                    n->py = n->py + 1;
                    n->px = n->px;

                    n->pr = grid[n->py][n->px].tile;

                    grid[n->py][n->px].tile = '@';
                }
            }
            else if(mov == '1' || mov == 'b'){
                int n_y = n->py + 1;
                int n_x = n->px - 1;

                if(vmove(n_y, n_x)){
                    grid[n->py][n->px].tile = n->pr;
                    n->py = n->py + 1;
                    n->px = n->px - 1;
                    n->pr = grid[n->py][n->px].tile;
                    grid[n->py][n->px].tile = '@';
                }
            }
            else if(mov == '4' || mov == 'h'){
                int n_y = n->py;
                int n_x = n->px - 1;

                if(vmove(n_y, n_x)){
                    grid[n->py][n->px].tile = n->pr;
                    n->py = n->py;
                    n->px = n->px - 1;
                    n->pr = grid[n->py][n->px].tile;
                    grid[n->py][n->px].tile = '@';
                }
            }
            else if(mov == '5' || mov == ' ' || mov == '.'){ // do nothing
                int x;
            }
            else if(mov == '>'){

                if(n->pr == '>'){
                    return 0;
                }
            }
            else if(mov == '<'){
                if(n->pr == '<'){
                    return 0;
                }
            }
            else if(mov == 'q'){
                break;
            }
            
            
            printmon();
            usleep(500000);
            
        }
        else if(strcmp(m.type, "0000") == 0){
            mon1(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "0001") == 0){
            mon2(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "0010") == 0){
            mon3(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "0011") == 0){
            mon4(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "0100") == 0){
            mon5(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "0101") == 0){
            mon6(n->px, n->py, numr, r, &m);
            //printmon();
        }else if(strcmp(m.type, "0110") == 0){
            mon7(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "0111") == 0){
            mon8(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "1000") == 0){
            mon9(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "1001") == 0){
            mon10(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "1010") == 0){
            mon11(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "1011") == 0){
            mon12(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "1100") == 0){
            mon13(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "1101") == 0){
            mon14(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "1110") == 0){
            mon15(n->px, n->py, numr, r, &m);
            //printmon();
        }
        else if(strcmp(m.type, "1111") == 0){
            mon16(n->px, n->py, numr, r, &m);
            //printmon();
        }

        //printf("%s\n", m.type);
        
       

        if(m.my == n->py && m.mx == n->px && m.type[0] != 'P'){
            printmon();
            printw("\n You died ! You were killed by Monster %s\n", m.type);
            break;
        }

        for(int i = 0; i < h.size; i++){
            if(h.hp[i].mx == m.mx && h.hp[i].my == m.my){ //remove monster if it was thre
                mremove(&h, i);
            }
        }

        m.next = m.next + (1000/m.speed);
        minsert(&h, m);

        //printmon();

        

    }

    return 1;
}

void printmon(){
    //printf("\n");

    clear();

    for(int i = 0; i <MAX; i++){
        for(int j = 0; j < MAXR; j++){
            if(grid[i][j].tile2 != '+'){
                mvaddch(i, j, grid[i][j].tile2);
            }
            else{
                mvaddch(i, j, grid[i][j].tile);
            }
        }
        //printf("\n");
    }

    refresh();
}

bool vmove(int y, int x){
    if(grid[y][x].tile == '.' || grid[y][x].tile == '#' || grid[y][x].tile == '>' || grid[y][x].tile == '<'){
        return true;
    }

    return false;
}



bool checkRoom(int r_start, int c_start, int n, int m, int r_at, int c_at) {
    if (r_start <= r_at && r_at <= r_start + n - 1 &&
        c_start <= c_at && c_at <= c_start + m - 1) {
        return true;  
    }
    return false;  
}


//case: 0000, check if player is in room, if so move in straight line
void mon1(int x, int y, int numr, struct room *r, struct monster *m){

    for(int i = 0; i < numr; i++){
        if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, y, x)){
            if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, m->my, m->mx)){
                if(m->mx != x){
                    if(m->mx < x){
                        grid[m->my][m->mx++].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '0';
                    }
                    else{
                        grid[m->my][m->mx--].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '0';
                    }
                }
                else if(m->my != y){
                    if(m->my < y){
                        grid[m->my++][m->mx].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '0';
                    }
                    else{
                        grid[m->my--][m->mx].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '0';
                    }
            
                }
            }
        }
    }
}

//case: 0001, check if player is in room, monster remembers last postion (intelligence)
void mon2(int x, int y, int numr, struct room *r, struct monster *m){
    for(int i = 0; i < numr; i++){
        if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, y, x)){
            if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, m->my, m->mx)){
                m->pox = x;
                m->poy = y;
            }
        }
    }

    if(m->pox != -1){
        struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node t = directions(&min, i);

            if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].non){
                    min.distance = dist[t.y][t.x].non;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        if(grid[sy][sx].tile == '#'){
            m->pox = -1;
        }
        else{
            grid[m->my][m->mx].tile2 = '+';
            m->my = sy;
            m->mx = sx;
            grid[m->my][m->mx].tile2 = '1';
        }

    }
}

//case 0010, telepathy
void mon3(int x, int y, int numr, struct room *r, struct monster *m){
    struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i+=2){
            struct node t = directions(&min, i);
            if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].non){
                    min.distance = dist[t.y][t.x].non;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        grid[m->my][m->mx].tile2 = '+';

        m->my = sy;
        m->mx = sx;

        grid[m->my][m->mx].tile2 = '2';
}

// 0011
void mon4(int x, int y, int numr, struct room *r, struct monster *m){

    struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node t = directions(&min, i);

            if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].non){
                    min.distance = dist[t.y][t.x].non;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        grid[m->my][m->mx].tile2 = '+';

        m->my = sy;
        m->mx = sx;

        grid[m->my][m->mx].tile2 = '3';
}

//0100
void mon5(int x, int y, int numr, struct room *r, struct monster *m){
    for(int i = 0; i < numr; i++){
        if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, y, x)){
            if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, m->my, m->mx)){
                if(m->mx != x){
                    if(m->mx < x){
                        grid[m->my][m->mx++].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '4';
                    }
                    else{
                        grid[m->my][m->mx--].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '4';
                    }
                }
                else if(m->my != y){
                    if(m->my < y){
                        grid[m->my++][m->mx].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '4';
                    }
                    else{
                        grid[m->my--][m->mx].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '4';
                    }
            
                }
            }
        }
    }
}

//0101
void mon6(int x, int y, int numr, struct room *r, struct monster *m){
    for(int i = 0; i < numr; i++){
        if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, y, x)){
            if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, m->my, m->mx)){
                m->pox = x;
                m->poy = y;
            }
        }
    }

    if(m->pox != -1){
        struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node t = directions(&min, i);

            if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].non){
                    min.distance = dist[t.y][t.x].non;
                    sy = t.y;
                    sx = t.x;
                }
            }
        }

        grid[m->my][m->mx].tile2 = '+';

        m->my = sy;
        m->mx = sx;

        grid[m->my][m->mx].tile2 = '5';
    }
}

//0110
void mon7(int x, int y, int numr, struct room *r, struct monster *m){
    struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i+=2){//8 for the 8 directions
            struct node t = directions(&min, i);

            if(t.distance == 0){
                if (min.distance == dist[t.y][t.x].tunnel) {
                
                    double current_distance = sqrt(pow(sx - t.x, 2) + pow(sy - t.y, 2));
                    double new_distance = sqrt(pow(min.x - t.x, 2) + pow(min.y - t.y, 2));
        
                    if (new_distance < current_distance) {
                        sy = t.y;
                        sx = t.x;
                    }
                }
                else if(min.distance > dist[t.y][t.x].tunnel){
                    min.distance = dist[t.y][t.x].tunnel;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        if(grid[sy][sx].hardness != 0){
            if(grid[sy][sx].hardness <= 85){
                grid[sy][sx].hardness = 0;
                grid[sy][sx].tile = '#';
                struct player p;
                grid[m->my][m->mx].tile2 = '+';
                p.px = x;
                p.py = y;
                m->my = sy;
                m->mx = sx;
                grid[m->my][m->mx].tile2 = '6';

                dijkstra(&p);
                dijkstra_tunnel(&p);
            }
            else{
                grid[sy][sx].hardness-= 85;
            }
        }
        else{
            grid[m->my][m->mx].tile2 = '+';

            m->my = sy;
            m->mx = sx;
    
            grid[m->my][m->mx].tile2 = '6';
        }
}

//0111
void mon8(int x, int y, int numr, struct room *r, struct monster *m){
    struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node t = directions(&min, i);

            if (min.distance == dist[t.y][t.x].tunnel) {
                
                double current_distance = sqrt(pow(min.x - sx, 2) + pow(min.y - sy, 2));
                double new_distance = sqrt(pow(min.x - t.x, 2) + pow(min.y - t.y, 2));
    
                if (new_distance < current_distance) {
                    sy = t.y;
                    sx = t.x;
                }
            }
            else if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].tunnel){
                    min.distance = dist[t.y][t.x].tunnel;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        if(grid[sy][sx].hardness != 0){
            if(grid[sy][sx].hardness <= 85){
                grid[sy][sx].hardness = 0;
                grid[sy][sx].tile = '#';
                struct player p;
                grid[m->my][m->mx].tile2 = '+';
                p.px = x;
                p.py = y;
                m->my = sy;
                m->mx = sx;
                grid[m->my][m->mx].tile2 = '7';

                dijkstra(&p);
                dijkstra_tunnel(&p);
            }
            else{
                grid[sy][sx].hardness-= 85;
            }
        }
        else{
            grid[m->my][m->mx].tile2 = '+';

            m->my = sy;
            m->mx = sx;
    
            grid[m->my][m->mx].tile2 = '7';
        }
}

//1000
void mon9(int x, int y, int numr, struct room *r, struct monster *m){

    if(rand() % 2){
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m -> my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile != ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';
                    m->mx = sx;
                    m->my = sy;
                    grid[m->my][m->mx].tile2 = '8';
                   
                }
            }
        }
    
    else{


    for(int i = 0; i < numr; i++){
        if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, y, x)){
            if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, m->my, m->mx)){
                if(m->mx != x){
                    if(m->mx < x){
                        grid[m->my][m->mx++].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '8';
                    }
                    else{
                        grid[m->my][m->mx--].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '8';
                    }
                }
                else if(m->my != y){
                    if(m->my < y){
                        grid[m->my++][m->mx].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '8';
                    }
                    else{
                        grid[m->my--][m->mx].tile2 = '+';
                        grid[m->my][m->mx].tile2 = '8';
                    }
            
                }
            }
        }
    }
}
}

//1001
void mon10(int x, int y, int numr, struct room *r, struct monster *m){
    if(rand() % 2){
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile != ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';
                    m->mx = sx;
                    m->my = sy;
                    grid[m->my][m->mx].tile2 = '9';
                    
                }
            }
        
    }
    else{

        if(m->pox != -1){
            struct node min;
            min.distance = 10000;
            min.y = m->my;
            min.x = m->mx;
    
            int sx, sy;
            
            for(int i = 0; i < 8; i++){//8 for the 8 directions
                struct node t = directions(&min, i);
    
                if(t.distance == 0){
                    if(min.distance > dist[t.y][t.x].non){
                        min.distance = dist[t.y][t.x].non;
                        sy = t.y;
                        sx = t.x;
                    }
                }
    
            }
    
            grid[m->my][m->mx].tile2 = '+';
    
            m->my = sy;
            m->mx = sx;
    
            grid[m->my][m->mx].tile2 = '9';
        }
    }
}

void mon11(int x, int y, int numr, struct room *r, struct monster *m){

    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m -> my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile != ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';

                    m->mx = sx;
                    m->my = sy;

                    grid[m->my][m->mx].tile2 = 'a';
                    
                }
            }
        }
    
    else{
        struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i+=2){
            struct node t = directions(&min, i);
            if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].non){
                    min.distance = dist[t.y][t.x].non;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        grid[m->my][m->mx].tile2 = '+';

        m->my = sy;
        m->mx = sx;

        grid[m->my][m->mx].tile2 = 'a';
    }
}

void mon12(int x, int y, int numr, struct room *r, struct monster *m){
    if(rand() % 2){
       
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m -> my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile != ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';
                    m->mx = sx;
                    m->my = sy;
                    grid[m->my][m->mx].tile2 = 'b';
                   
                }
            }
        }
    
    else{
        struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node t = directions(&min, i);

            if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].non){
                    min.distance = dist[t.y][t.x].non;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        grid[m->my][m->mx].tile2 = '+';

        m->my = sy;
        m->mx = sx;

        grid[m->my][m->mx].tile2 = 'b';
    }
}

void mon13(int x, int y, int numr, struct room *r, struct monster *m){

    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m ->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile == ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    if(grid[sy][sx].hardness <= 85){
                        grid[sy][sx].hardness = 0;
                        grid[sy][sx].tile = '#';
                        struct player p;
                        grid[m->my][m->mx].tile2 = '+';
                        p.px = x;
                        p.py = y;
                        m->my = sy;
                        m->mx = sx;
                        grid[m->my][m->mx].tile2 = 'c';
        
                        dijkstra(&p);
                        dijkstra_tunnel(&p);
                    }
                    else{
                        grid[sy][sx].hardness-= 85;
                    }
                }
                else if(grid[y][x].tile == ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';
                    m->mx = sx;
                    m->my = sy;
                    grid[m->my][m->mx].tile2 = 'c';
                    
                }
            }
        }
    
        else{
    

    for(int i = 0; i < numr; i++){
        if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, y, x)){
            if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, m->my, m->mx)){
                if(m->mx != x){
                    if(m->mx < x){
                        grid[m->my][m->mx++].tile2 = '+';
                        grid[m->my][m->mx].tile2 = 'c';
                    }
                    else{
                        grid[m->my][m->mx--].tile2 = '+';
                        grid[m->my][m->mx].tile2 = 'c';
                    }
                }
                else if(m->my != y){
                    if(m->my < y){
                        grid[m->my++][m->mx].tile2 = '+';
                        grid[m->my][m->mx].tile2 = 'c';
                    }
                    else{
                        grid[m->my--][m->mx].tile2 = '+';
                        grid[m->my][m->mx].tile2 = 'c';
                    }
            
                }
            }
        }
    }
}
}

void mon14(int x, int y, int numr, struct room *r, struct monster *m){
    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m ->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile == ' '){
                    if(grid[sy][sx].hardness <= 85){
                        grid[sy][sx].hardness = 0;
                        grid[sy][sx].tile = '#';
                        struct player p;
                        grid[m->my][m->mx].tile2 = '+';
                        p.px = x;
                        p.py = y;
                        m->my = sy;
                        m->mx = sx;
                        grid[m->my][m->mx].tile2 = 'd';
        
                        dijkstra(&p);
                        dijkstra_tunnel(&p);
                    }
                    else{
                        grid[sy][sx].hardness-= 85;
                    }
                }
                else if(grid[sy][sx].tile == ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';
                    m->my = sy;
                    m->mx = sx;
                    grid[m->my][m->mx].tile2 = 'd';
                   
                }
            }
        
    }
    else{
        for(int i = 0; i < numr; i++){
            if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, y, x)){
                if(checkRoom(r[i].ly, r[i].lx, r[i].d2, r[i].d1, m->my, m->mx)){
                    m->pox = x;
                    m->poy = y;
                }
            }
        }
    
        if(m->pox != -1){
            struct node min;
            min.distance = 10000;
            min.y = m->my;
            min.x = m->mx;
    
            int sx, sy;
            
            for(int i = 0; i < 8; i++){//8 for the 8 directions
                struct node t = directions(&min, i);
    
                if(t.distance == 0){
                    if(min.distance > dist[t.y][t.x].non){
                        min.distance = dist[t.y][t.x].non;
                        sy = t.y;
                        sx = t.x;
                    }
                }
            }
    
            grid[m->my][m->mx].tile2 = '+';
    
            m->my = sy;
            m->mx = sx;
    
            grid[m->my][m->mx].tile2 = 'd';
        }
    }
}

void mon15(int x, int y, int numr, struct room *r, struct monster *m){
    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m ->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile == ' '){
                    if(grid[sy][sx].hardness <= 85){
                        grid[sy][sx].hardness = 0;
                        grid[sy][sx].tile = '#';
                        struct player p;
                        grid[m->my][m->mx].tile2 = '+';
                        p.px = x;
                        p.py = y;
                        m->my = sy;
                        m->mx = sx;
                        grid[m->my][m->mx].tile2 = 'e';
        
                        dijkstra(&p);
                        dijkstra_tunnel(&p);
                    }
                    else{
                        grid[sy][sx].hardness-= 85;
                    }
                }
                else if (grid[sy][sx].tile == ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';
                    m->my = sy;
                    m->mx = sx;
                    grid[m->my][m->mx].tile2 = 'e';
                    
                }
            }
        
    }
    else{
        struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i+=2){//8 for the 8 directions
            struct node t = directions(&min, i);

            if(t.distance == 0){
                if (min.distance == dist[t.y][t.x].tunnel) {
                
                    double current_distance = sqrt(pow(sx - t.x, 2) + pow(sy - t.y, 2));
                    double new_distance = sqrt(pow(min.x - t.x, 2) + pow(min.y - t.y, 2));
        
                    if (new_distance < current_distance) {
                        sy = t.y;
                        sx = t.x;
                    }
                }
                else if(min.distance > dist[t.y][t.x].tunnel){
                    min.distance = dist[t.y][t.x].tunnel;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        if(grid[sy][sx].hardness != 0){
            if(grid[sy][sx].hardness <= 85){
                grid[sy][sx].hardness = 0;
                grid[sy][sx].tile = '#';
                struct player p;
                grid[m->my][m->mx].tile2 = '+';
                p.px = x;
                p.py = y;
                m->my = sy;
                m->mx = sx;
                grid[m->my][m->mx].tile2 = 'e';

                dijkstra(&p);
                dijkstra_tunnel(&p);
            }
            else{
                grid[sy][sx].hardness-= 85;
            }
        }
        else{
            grid[m->my][m->mx].tile2 = '+';

            m->my = sy;
            m->mx = sx;
    
            grid[m->my][m->mx].tile2 = 'e';
        }
    }
}

void mon16(int x, int y, int numr, struct room *r, struct monster *m){
    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m ->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile == ' '){
                    if(grid[sy][sx].hardness <= 85){
                        grid[sy][sx].hardness = 0;
                        grid[sy][sx].tile = '#';
                        struct player p;
                        grid[m->my][m->mx].tile2 = '+';
                        p.px = x;
                        p.py = y;
                        m->my = sy;
                        m->mx = sx;
                        grid[m->my][m->mx].tile2 = 'f';
        
                        dijkstra(&p);
                        dijkstra_tunnel(&p);
                    }
                    else{
                        grid[sy][sx].hardness-= 85;
                    }
                }
                else if(grid[sy][sx].tile == ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';
                    m->my = sy;
                    m->mx = sx;
                    grid[m->my][m->mx].tile2 = 'f';
                    
                }
            }
        
    }
    else{
        struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node t = directions(&min, i);

            if (min.distance == dist[t.y][t.x].tunnel) {
                
                double current_distance = sqrt(pow(sx - t.x, 2) + pow(sy - t.y, 2));
                double new_distance = sqrt(pow(min.x - t.x, 2) + pow(min.y - t.y, 2));
    
                if (new_distance < current_distance) {
                    sy = t.y;
                    sx = t.x;
                }
            }

            else if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].tunnel){
                    min.distance = dist[t.y][t.x].tunnel;
                    sy = t.y;
                    sx = t.x;
                }
            }

        }

        if(grid[sy][sx].hardness != 0){
            if(grid[sy][sx].hardness <= 85){
                grid[sy][sx].hardness = 0;
                grid[sy][sx].tile = '#';
                struct player p;
                grid[m->my][m->mx].tile2 = '+';
                p.px = x;
                p.py = y;
                m->my = sy;
                m->mx = sx;
                grid[m->my][m->mx].tile2 = 'f';

                dijkstra(&p);
                dijkstra_tunnel(&p);
            }
            else{
                grid[sy][sx].hardness-= 85;
            }
        }
        else{
            grid[m->my][m->mx].tile2 = '+';

            m->my = sy;
            m->mx = sx;
    
            grid[m->my][m->mx].tile2 = 'f';
        }
    } 
}





