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
#include <string>
#include <vector>
#include "Player.h"
#include "Cell.h"
#include "Map.h"
#include "Room.h"
#include "up.h"
#include "Down.h"
#include "File.h"
#include <string>
#include <cstdlib>
#include "monsters.h"
#include <fstream>
#include <vector>  
#include "Item.h"
#include "Dice.h"
#include <unordered_set>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <chrono>
#include <iostream>

#define MAX 21 //MAX heights
#define MAXR 80 //MAX Width



//prototypes for the methods
void build();
void place_rooms(Room *r, int ranroom);
int check(int x, int y, int xm, int ym);
void connect_rooms(Room *r, int ranroom);
void place_stairs(int stairs_up, Up *u, int stairs_down, Down *d);
void place_pc(Player* p);
void assign_hardness();
void load(int x);
void save(int num_rooms, Room *r, int up_stairs, Up *u, int down_stairs, Down *d, Player* p);
int strcmp();
bool details(int key, int num_mon, Player* p);
void dijkstra(Player* p);
struct node directions(struct node *temp, int x);
void printmap();
void dijkstra_tunnel(Player* p);
void print_tunnel();
int gameplay(Player* p, int num_mon, int x, Room *r);
bool checkRoom();
void mon1(int x, int y, int numr, Room *r, Monster *m);
void mon2(int x, int y, int numr, Room *r, Monster *m);
void mon3(int x, int y, int numr, Room *r, Monster *m);
void mon4(int x, int y, int numr, Room *r, Monster *m);
void mon5(int x, int y, int numr, Room *r, Monster *m);
void mon6(int x, int y, int numr, Room *r, Monster *m);
void mon7(int x, int y, int numr, Room *r, Monster *m, char c);
void mon8(int x, int y, int numr, Room *r, Monster *m, char c);
void mon9(int x, int y, int numr, Room *r, Monster *m);
void mon10(int x, int y, int numr, Room *r, Monster *m);
void mon11(int x, int y, int numr, Room *r, Monster *m);
void mon12(int x, int y, int numr, Room *r, Monster *m);
void mon13(int x, int y, int numr, Room *r, Monster *m, char c);
void mon14(int x, int y, int numr, Room *r, Monster *m, char c);
void mon15(int x, int y, int numr, Room *r, Monster *m, char c);
void mon16(int x, int y, int numr, Room *r, Monster *m, char c);
void printmon(MonsterHeap &h, const std::vector<Item*> im, int j, int i, bool tele);
void ncurse();
bool vmove(int x, int y);
void monlist(MonsterHeap &h , int x, int y);
void explore(Player* p);
bool vmove2(int x, int y);
bool combat(Monster *m, Player* p);
void monster_description(int x, int y, MonsterHeap &h);
void play_walking_sound();
void open_exp(Player *p, Monster *m);
void controls();
void jukebox();




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




Cell grid[21][80]; //2-d board of cell

Map dist[21][80];


int main(int argc, char *argv[]){

    system("./mpg123 -q -f 8192 ./sounds/447_The_Murky_Depths.mp3 &");

    srand(time(NULL));

    //system("mpg123 -b 8192 -f 8192 ./sounds/450_Nightlands.mp3 &");

    ncurse();

    Player* p = NULL;

    bool t = false;

    if(argc == 1 || argc == 2){
        t = details(0, 0, p);
    }

    else if(argc == 3 && (strcmp(argv[1], "--load") == 0)){
        load(1); //load from disk and re-save
    }
    else if(argc == 3 && (strcmp(argv[1], "--save") == 0)){
        t = details(1, 2, p);
    }

    else if(strcmp(argv[1], "--load") == 0){
        load(0); //load from disk only

    }
    else if(strcmp(argv[1], "--save") == 0){
        t = details(1, 2, p); //save a randomly generated map
       
    }
    else{
        printf("Please pass in valid key (save or load)\n");
    }



    if(!t){

    system("./mpg123 -q -f 8192 ./sounds/game-over-160612.mp3 &");
    std::ifstream file;
    file.open("ASCII/death.txt");

    clear();

    int row = 1;

    mvprintw(0, 1, "q to quit");
    std::string s;

    while (std::getline(file, s)) {
        mvprintw(row++, 1, "%s", s.c_str());
    }

    while(1){
        int mov = getch();
        
        if(mov == 'q'){
            system("pkill mpg123");

            break;
        }
    }
}
else{
    std::ifstream file;
    file.open("ASCII/victory.txt");

    clear();

    int row = 1;

    mvprintw(0, 1, "q to quit");
    std::string s;

    while (std::getline(file, s)) {
        mvprintw(row++, 1, "%s", s.c_str());
    }

    while(1){
        int mov = getch();
        
        if(mov == 'q'){
            system("pkill mpg123");

            break;
        }
    }
}

    endwin();

    return 0;
}

void play_walking_sound(){
    //system("./mpg123 -q  ./sounds/walk-on-grass-1-291984.mp3 &");
}

void play_fighting_sound(){
    //system("./mpg123 -q  ./sounds/metal-sound-fighting-game-87507.mp3 &");
}

void jukebox(){

    int offset = 0;

    while(1){
        clear();
        mvprintw(0, 1, "[esc] to escape, [s] to select music");

        if(offset == 0){
            mvprintw(2, 1, "Ambient Dungeon (default) <----");
        }
        else{
            mvprintw(2, 1, "Ambient Dungeon (default)");
        }

        if(offset == 1){
            mvprintw(3, 1, "Dark Fantasy <----");
        }
        else{
            mvprintw(3,1, "Dark Fantasy");
        }

        if(offset == 2){
            mvprintw(4, 1, "Epic dungeon beat <----");
        }
        else{
            mvprintw(4, 1, "Epic dungeon beat");
        }

        if(offset == 3){
            mvprintw(5, 1, "Mute <----");
        }
        else{
            mvprintw(5, 1, "Mute");
        }

        int mov = getch();

        if(mov == 's'){
            if(offset == 0){
                system("pkill mpg123");
                system("./mpg123 -q -f 8192 ./sounds/447_The_Murky_Depths.mp3 &");
            }
            else if(offset == 1){
                system("pkill mpg123");
                system("./mpg123 -q -f 8192 ./sounds/dark-fantasy-ambient-dungeon-synth-248213.mp3 &");
            }
            else if(offset == 2){
                system("pkill mpg123");
                system("./mpg123 -q -f 8192 ./sounds/epic-dungeon-beat-259883.mp3 &");
            }
            else if(offset == 3){
                system("pkill mpg123");
            }
        }
        else if(mov == KEY_DOWN){
            if(offset != 3){
                offset++;
            }
        }
        else if(mov == KEY_UP){
            if(offset != 0){
                offset--;
            }
        }
        else if(mov == 27){
            break;
        }

    }
}

void controls(){

    int offset = 0;
    while(1){
        clear();
        mvprintw(0,1, "[esc] to leave the controls, [s] to select");


        if(offset == 0){
            mvprintw(2,1, "Movement   <---");
        }
        else{
            mvprintw(2,1, "Movement");
        }

        if(offset == 1){
            mvprintw(3,1 , "Attack   <---");
        }
        else{
            mvprintw(3,1,  "Attack");
        }

        if(offset == 2){
            mvprintw(4,1, "Inventory <---");
        }
        else{
            mvprintw(4,1, "Inventory");
        }

        if(offset == 3){
            mvprintw(5,1, "Equipment <---");
        }
        else{
            mvprintw(5,1, "Equipment");
        }

        if(offset == 4){
            mvprintw(6,1, "Experience <---");

        }
        else{
            mvprintw(6,1, "Experience");
        }

        int mov = getch();

        if(mov == 's'){
            if(offset == 0){
                while(1){
                    clear();
                    mvprintw(0,1, "[esc] to leave");

                    mvprintw(2, 1, "7 or y: upper-left");
                    mvprintw(3, 1, "8 or k: up");
                    mvprintw(4, 1, "9 or u: upper-right");
                    mvprintw(5, 1, "6 or l: right");
                    mvprintw(6, 1, "3 or n: lower-right");
                    mvprintw(7, 1, "2 or j: down");
                    mvprintw(8, 1, "1 or b: lower-left");
                    mvprintw(9, 1, "4 or h: left");

                    int mov = getch();

                    if(mov == 27){
                        break;
                    }
                }
            }
            else if(offset == 1){
                while(1){
                    clear();
                    mvprintw(0,1, "[esc] to leave");
                    
                    mvprintw(2, 1, "[a]: attack when in battle");
                    mvprintw(4, 1, " - It is turn based, you attack first. then the monster");
                    mvprintw(5, 1, "- Your health is restored if you win.");

                    int mov = getch();

                    if(mov == 27){
                        break;
                    }
                }
            }
            else if(offset == 2){
                while(1){
                    clear();
                    mvprintw(0,1, "[esc] to leave");

                    mvprintw(2, 1, "[i] to open inventory");
                    mvprintw(3, 1, "[d] to drop an item");
                    mvprintw(4, 1, "[x] to delete an item");
                    mvprintw(5, 1, "[I] to view the item");
                    mvprintw(6, 1, "[w] to equip the item");

                    int mov = getch();

                    if(mov == 27){
                        break;
                    }
                }
            }
            else if(offset == 3){
                while(1){
                    clear();
                    mvprintw(0,1, "[esc] to leave");

                    mvprintw(2, 1, "[e] to open equipment");
                    mvprintw(3, 1, "[t] to take off an equipment");

                    int mov = getch();

                    if(mov == 27){
                        break;
                    }
                    
                }
            }
            else if(offset == 4){
                while(1){
                clear();
                mvprintw(0,1, "[esc] to leave");

                mvprintw(2, 1, "[x] to open experience");
                mvprintw(3, 1, "[a] to add points to speed or health");

                int mov = getch();

                    if(mov == 27){
                        break;
                    }
            }
        }
        }
        else if(mov == KEY_DOWN){
            if(offset != 4){
                offset++;
            }
        }
        else if(mov == KEY_UP){
            if(offset != 0){
                offset--;
            }
        }
        else if(mov == 27){
            break;
        }
    }
}


void monster_description(int x, int y, MonsterHeap&h){
    Monster* temp;

    for(int i = 0; i < h.size; i++){
        if(h.hp[i]->mx == x && h.hp[i]->my){
            temp = h.hp[i];
            break;
        }
    }

    if(!temp){
        return;
    }
    clear();

    while(1){
        mvprintw(0, 1, "Name: %s      [q] to quit this menu", temp->name.c_str());
        mvprintw(3, 1, "%s", temp->desc.c_str());

        int mov = getch();

        if(mov == 'q'){
            break;
        }
    }
}

bool combat(Monster *m, Player *p){
    clear();
    refresh();
    mvprintw(0, 1, "YOU HAVE ENCOUNTERED: %s", m->name.c_str());
    std::ifstream file;

    if(m->name == "Junior Barbarian"){
        file.open("ASCII/Barbarian.txt");
    }
    else if(m->name == "Amazon Lich Queen"){
        file.open("ASCII/Queen.txt");
    }
    else if(m->name == "Durin's Bane"){
        file.open("ASCII/Bane.txt");
    }
    else if(m->name == "Titan"){
        file.open("ASCII/Titan.txt");
    }
    else if(m->name == "Slime"){
        file.open("ASCII/slime.txt");
    }
    else if(m->name == "Software Bug"){
        file.open("ASCII/Bug.txt");
    }
    else if(m->name == "Hardware Bug" ){
        file.open("ASCII/Hard.txt");
    }
    else if(m->name == "C Program"){
        file.open("ASCII/C.txt");
    }
    else if(m->name == "C++ Program"){
        file.open("ASCII/cpp.txt");
    }
    else if(m->name == "Rory Gilmore"){
        file.open("ASCII/Gilmore.txt");
    }
    else if(m->name == "Tsukino Usagi"){
        file.open("ASCII/anime.txt");
    }
    else if(m->name == "Casper the Friendly Ghost"){
        file.open("ASCII/Casper.txt");
    }
    else if(m->name == "Slimer"){
        file.open("ASCII/slimer.txt");
    }
    else if(m->name == "David Bowie"){
        file.open("ASCII/bowie.txt");
    }
    else if(m->name == "SpongeBob SquarePants"){
        file.open("ASCII/bob.txt");
    }
    else if(m->name == "Grass Mud Horse"){
        file.open("ASCII/horse.txt");
    }
    else if(m->name == "River Crab"){
        file.open("ASCII/crab.txt");
    }
    else if(m->name == "Overhead Projector"){
        file.open("ASCII/project.txt");
    }
    else if(m->name == "Donald Trump"){
        file.open("ASCII/Trump.txt");
    }
    else if(m->name == "Bruce Willis"){
        file.open("ASCII/willis.txt");
    }

    std::string s;
    int row = 1;

    Dice* d = new Dice(m->dam);

    int temphp = p->hp;
    int temphp2 = m->hp;

    system("./mpg123 -q -f 8192 ./sounds/mortal-kombat.mp3 &");

    while(p->hp > 0 && m->hp > 0){
        mvprintw(row++, 1, "You have %d Health. Monster has %d Health", p->hp, m->hp);
        mvprintw(row++, 1, "[a]ttack, you don't have a choice here");

        while (std::getline(file, s)) {
            mvprintw(row++, 1, "%s", s.c_str());
        }

        int move = getch();


        if(move == 'a'){

            play_fighting_sound();

            if(!p->weapon){
                Dice* s = new Dice("1+1d4");
                m->hp -= s->roll();
            }
            else{
                m->hp -= p->attack;
            }
            
            if(m->hp > 0){
                p->hp -= d->roll();
            }
            
            row = 1;
        }
    }

    if(p-> hp <= 0){
        return true;
    }
    else{
        system("./mpg123 -q -f 8192 ./sounds/fatality-mortal-kombat-sound-effect-hd.mp3 &");
        p->hp = temphp;

        if(temphp2 <= 200){
            p->exp += 20;
        }
        else if(temphp2 > 200 && temphp2 <= 500){
            p->exp += 40;
        }
        else if(temphp2 > 500 && temphp2 <= 1000){
            p->exp += 80;
        }
        else{
            p->exp += 150;
        }
        return false;
    }
}


void open_exp(Player *p, Monster *m){
    clear();

    int offset = 0;

    while(1){

        mvprintw(0, 1, "Base Stats: [esc] to escape,[down arrow] to go down, [up arrow] for up , [a] to add exp");
        mvprintw(1,1, "Experience points: %d", p->exp);

        if(offset == 0){
            mvprintw(3, 1, "Speed: %d (25 exp)    <----", p->speed);
        }
        else{
            mvprintw(3, 1, "Speed: %d (25 exp)", p->speed);
        }

        if(offset == 1){
            mvprintw(4,1, "Health: %d (25 exp)  <----", p->hp);
        }
        else{
            mvprintw(4,1, "Health: %d (25 exp)", p->hp);
        }

        int mov = getch();

        if(mov == 'a'){
            if(p->exp < 25){
                mvprintw(6, 1, "Not enough exp");
            }
            else if(offset == 0 && p->speed < 20){
                p->speed++;
                p->setSpeed(m);
                mvprintw(6, 1, "Speed increased by one!");
                p->exp -= 25;
            }
            else if(offset == 1){
                p->hp += 5;
                mvprintw(6,1, "Health increased by 5!");
                p->exp -= 25;
            }
            else{
                mvprintw(6, 1, "Speed cap is reached (MAX 20)");
            }
        }
        else if(mov == KEY_DOWN){
            if(offset != 1){
                offset++;
            }
            clear();
        }
        else if(mov == KEY_UP){
            if(offset != 0){
                offset--;
            }
            clear();
        }
        else if(mov == 27){
            break;
        }
    }
}

void explore(Player *p, int x){

    for(int i = p->py - x; i < p->py + x + 1; i++){
        for(int j = p->px - x; j < p->px + x + 1; j++){
            if(i >= 0 && i < MAX && j >= 0 && j < MAXR && grid[i][j].tile != ' ' && grid[i][j].tile != '|' && grid[i][j].tile != '-'){
                grid[i][j].explored = 1;
            }
        }
    }
}

void ncurse() {
    initscr();                               
    keypad(stdscr, TRUE);   
    noecho();  
    curs_set(0);
    start_color();
    init_pair(COLOR_BLACK,   COLOR_BLACK,   COLOR_BLACK);
    init_pair(COLOR_RED,     COLOR_RED,     COLOR_BLACK);
    init_pair(COLOR_GREEN,   COLOR_GREEN,   COLOR_BLACK);
    init_pair(COLOR_YELLOW,  COLOR_YELLOW,  COLOR_BLACK);
    init_pair(COLOR_BLUE,    COLOR_BLUE,    COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN,    COLOR_CYAN,    COLOR_BLACK);
    init_pair(COLOR_WHITE,   COLOR_WHITE,   COLOR_BLACK);      
         
}

void monlist(MonsterHeap &h , int x, int y){

    clear();

    int offset = 0;


    while(1){

        clear();
    
    mvprintw(0, 1, "MONSTER LIST: [esc] to escape");
    mvprintw(1,1, "--------------------------------");

    int j = 0;

    for(int i = offset; i < h.size && i < 30 + offset; i++){

        Monster* temp = h.hp[i];

        if(temp->type == "P"){
            continue;
        }
        int tempx = temp->mx - x;
        int tempy = temp->my - y;

        mvprintw(j + 2, 1, "Monster %c", grid[temp->my][temp->mx].tile2);

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
        if(offset + 30 < h.size){
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

refresh();
clear();
}



bool details(int key, int num_mon, Player *p){

        build();
        //Changed from static to dynamics
        int ranroom = rand() % 4 + 6;
        Room* r = new Room[ranroom];
        place_rooms(r, ranroom);
        connect_rooms(r, ranroom);

        int ranstair = rand() % 3 + 1;
        Up *u = new Up[ranstair];

        int ranstairs = rand() % 3 + 1;
        Down *d = new Down[ranstairs];



        place_stairs(ranstair, u, ranstairs, d);

        if(!p){
            p = new Player();
        }

        place_pc(p);
        assign_hardness();

    dijkstra(p);
    dijkstra_tunnel(p);

    int restart = gameplay(p, num_mon, ranroom, r);

    if(key == 1){
        save(ranroom, r, ranstair, u, ranstairs, d, p);
    }

    delete[] r; 
    delete[] u;  
    delete[] d;     

    if(restart == 0){
        details(key, num_mon, p);
    }

    if(restart == 2){
        return true;
    }

    return false;
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

    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAXR; j++){
            grid[i][j].explored = 0;
        }
    }

}


//randomly place six rooms, and place the rooms into an array
void place_rooms(Room *rooms, int ranroom){

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
            
            //calculate the central coordinates of the room
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

//Check if there are other rooms around it or on it's dimensions
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
void connect_rooms(Room *rooms, int ranroom) {

    
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
void place_stairs(int stairs_up, Up *u, int stairs_down, Down *d){

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
void place_pc(Player* p){

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
            grid[y][x].explored = 1;
            count++;
        }
    }



    
    for(int i = p->py - 2; i < p->py + 3; i++){
        for(int j = p->px - 2; j < p->px + 3; j++){
            if(i >= 0 && i < MAX && j >= 0 && j < MAXR && grid[i][j].tile != ' ' && grid[i][j].tile != '|' && grid[i][j].tile != '-'){
                grid[i][j].explored = 1;
            }
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
void save(int num_rooms, Room *r, int up_stairs, Up *u, int down_stairs, Down *d, Player* p){

    char *home = getenv("HOME");
    int len = strlen(home) + strlen("/.rlg327/dungeon") + 1;
    char *dungeon = (char *)malloc(len);
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
    char *dungeon = (char *)malloc(len);
    strcpy(dungeon, home);
    strcat(dungeon, "/.rlg327/dungeon");

    FILE *file = fopen(dungeon, "rb");

    char m[12]; //load in the RLG-COMS327
    fread(m, 1, 12, file); 

    uint32_t x; //load in the one byte value
    fread(&x, 4, 1, file);

    uint32_t total_size; // total size of the file
    fread(&total_size, 4, 1, file);

    Player* p = new Player();

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

    Room *r = new Room[num_rooms];

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

    Up *u = new Up[num_up];

    for(int i = 0; i < num_up; i++){
        fread(&u[i].ux, 1, 1, file);
        fread(&u[i].uy, 1, 1, file);

        grid[u[i].uy][u[i].ux].tile = '<';
    }


    uint16_t num_d;
    fread(&num_d, 2, 1, file);
    num_d = be16toh(num_up); //number of downstairs

    Down *d = new Down[num_d];

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
void dijkstra(Player* p){

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
void dijkstra_tunnel(Player* p){

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
}


int gameplay(Player* n, int num, int numr, Room *r){

    


    //create the monster heap
    //create_mon(h, num);
    

    //insert the player
    //Monster* p = new Monster();
    //p->next = 0;
    //p->speed = 10;
    //p->type[0] = 'P';
    //p->type[1] = '\0';
    //p->pox = -1;
    //p->poy = -1;
    //p->mx = n->px;
    //p->my = n->py;

    //minsert(h, p);

    //grid[n->py][n->px + 1].tile = '>';

    // int f = 0;

    // int x, y;

    // while(f != num){
        
    //      int count = 0;

    //     while(count == 0){
    //         x = rand() % 78 + 1;
    //         y = rand() % 19 + 1;
            
    //         if(grid[y][x].tile == '#' || grid[y][x].tile == '.'){  //make sure monsters are placed on different squaress

    //                 count++;
    //         }
               
    //     }

    // //num_monsters(h, x, y);

    // f++;


    // }



    // struct monster w;
    // w.next = 0;
    // w.speed = 10;
    // w.type[0] = '1';
    // w.type[1] = '0';
    // w.type[2] = '0';d
    // w.type[3] = '1';
    // w.type[4] = '\0';
    // w.pox = -1;;;;
    // w.poy = -1;
    // w.mx = n->px;
    // w.my = n->py;

    //minsert(&h, w);
//sdasdsa
    //place the monsters on the grid

    char *home = getenv("HOME");
    int len = strlen(home) + strlen("/.rlg327/monster_desc.txt") + 1;
    char *dungeon = (char *)malloc(len);
    strcpy(dungeon, home);
    strcat(dungeon, "/.rlg327/monster_desc.txt");

    File filsdae(dungeon, n->px, n->py);
    filsdae.parse();
    filsdae.place_monsters(grid);

    MonsterHeap h = filsdae.getHeap(); //get the created heap
    
    //filsdae.print_file(h);s

    char *homes = getenv("HOME");
    int lens = strlen(homes) + strlen("/.rlg327/object_desc.txt") + 1;
    char *dungeons = (char *)malloc(lens);
    strcpy(dungeons, homes);
    strcat(dungeons, "/.rlg327/object_desc.txt");

    Item_parse item(dungeons);
    item.iparse();


    for(int i = 0; i < h.size; i++){

        if(h.hp[i]->symb == "@"){
            continue;
        }
        
        grid[h.hp[i]->my][h.hp[i]->mx].tile2 = *(h.hp[i]->symb.c_str());
        
    }

    item.place_items(grid, n->deleted); //place the items down

    std::vector<Item*> im = item.get_list();

    printmon(h, im, n->px, n->py, false);


    int z = 0;

    while(h.size != 0){
        Monster *m = mextractmin(h);

        int mbit;
        

        if(strcmp(m->type, "P") != 0){
            mbit = std::stoi(m->type, nullptr, 2);
        }

        if(strcmp(m->type, "P") == 0){ //skip for now, player doesn't move

            printmon(h, im, n->px, n->py, false);

            n->setSpeed(m);

            int mov = getch();

            if(mov == 'm' || mov == 'f' || mov == 'g' || mov == 'i' || mov == 'o' || mov == 'e' || mov == ',' || mov == 'L' || mov == 'x' || mov == '?' || mov == '~'){

                int tempsx = n->px;
                int tempsy = n->py;
                char ts = n->pr;

                while(1){

                    if(mov == 'm'){
                        monlist(h, n->px, n->py);
                        printmon(h, im, n->px, n->py, false);
                    }
                    else if(mov == 'x'){
                        open_exp(n, m);
                        printmon(h, im, n->px, n->py, false);
                    }
                    else if(mov == 'e'){
                        n->print_equip();
                        printmon(h, im, n->px, n->py, false);
                    }
                    else if(mov == '~'){
                        jukebox();
                        printmon(h, im, n->px, n->py, false);
                    }
                    else if(mov == '?'){
                        controls();
                        printmon(h, im, n->px, n->py, false);
                    }
                    else if(mov == 'o'){
                        //monlist(h, n->px, n->py);s
                        filsdae.print_file();
                        printmon(h, im, n->px, n->py, false);
                    }
                    else if(mov == 'i'){
                        n->print_inventory(im);
                        printmon(h, im, n->px, n->py, false);
                    }
                    else if(mov == 'f'){
                        if(grid[0][0].on == -1){
                            grid[0][0].on = 0;
                        }
                        else{
                            grid[0][0].on = -1;
                        }
                        printmon(h, im, n->px, n->py, false);
                    }
                    else if(mov == ','){
                        char t = n->pr;
                        if(t == '|' || t == ')' || t == '}' || t == '[' || t == ']' || t == '(' || t == '{' || t == '\\' || t == '='
                        || t == '"' || t == '_' || t == '~' || t == '?' || t == '!' || t == '$' || t == '/' || t == ',' || t == '-' || t == '%'){
                            for(int i = 0; i < im.size(); i++){
                                if(im[i]->ix == n->px && im[i]->iy == n->py){
                                    n->pickup(im[i]);
                                    im.erase(im.begin() + i);
                                    break;
                                }
                            }

                            grid[n->py][n->px].tile = '.';
                            grid[n->py][n->px].tile2 = '+';
                            n->pr = '.';
                        }
                    }
                    else if(mov == 'g' || mov == 'L'){

                        int ter = mov;
                        int posx = n->px;
                        int posy = n->py;
                        printmon(h, im, n->px, n->py, true);

                        while(1){
                            mov = getch();

                            if(mov == '7' || mov == 'y'){
                                int n_y = n->py - 1;
                                int n_x = n->px - 1;
                
                                if(vmove2(n_y, n_x)){
                                    grid[n->py][n->px].tile = n->pr;
                                    n->py = n->py - 1;
                                    n->px = n->px - 1;
                                    n->pr = grid[n->py][n->px].tile;
                                    grid[n->py][n->px].tile = '*';
                                }
                            }
                            else if(mov == 't' && ter == 'L'){
                                monster_description(n->px, n->py, h);
                            }
                            else if(mov == '8' || mov == 'k'){
                                int n_y = n->py - 1;
                                int n_x = n->px;
                
                                if(vmove2(n_y, n_x)){
                                    grid[n->py][n->px].tile = n->pr;
                                    n->py = n->py - 1;
                                    n->px = n->px;
                                    n->pr = grid[n->py][n->px].tile;
                
                                    grid[n->py][n->px].tile = '*';
                                }
                            }
                            else if(mov == '9' || mov == 'u'){
                                int n_y = n->py - 1;
                                int n_x = n->px + 1;
                
                                if(vmove2(n_y, n_x)){
                                    grid[n->py][n->px].tile = n->pr;
                                    n->py = n->py - 1;
                                    n->px = n->px + 1;
                
                                    n->pr = grid[n->py][n->px].tile;
                                    grid[n->py][n->px].tile = '*';
                                }
                            }
                            else if(mov == '6' || mov == 'l'){
                                int n_y = n->py;
                                int n_x = n->px + 1;
                
                                if(vmove2(n_y, n_x)){
                                    grid[n->py][n->px].tile = n->pr;
                                    n->py = n->py;
                                    n->px = n->px + 1;
                
                                    n->pr = grid[n->py][n->px].tile;
                
                                    grid[n->py][n->px].tile = '*';
                                }
                            }
                            else if(mov == '3' || mov == 'n'){
                                int n_y = n->py + 1;
                                int n_x = n->px + 1;
                
                                if(vmove2(n_y, n_x)){
                                    grid[n->py][n->px].tile = n->pr;
                                    n->py = n->py + 1;
                                    n->px = n->px + 1;
                
                                    n->pr = grid[n->py][n->px].tile;
                
                                    grid[n->py][n->px].tile = '*';
                                }
                            }
                            else if(mov == '2' || mov == 'j'){
                                int n_y = n->py + 1;
                                int n_x = n->px;
                
                                if(vmove2(n_y, n_x)){
                                    grid[n->py][n->px].tile = n->pr;
                                    n->py = n->py + 1;
                                    n->px = n->px;
                
                                    n->pr = grid[n->py][n->px].tile;
                
                                    grid[n->py][n->px].tile = '*';
                                }
                            }
                            else if(mov == '1' || mov == 'b'){
                                int n_y = n->py + 1;
                                int n_x = n->px - 1;
                
                                if(vmove2(n_y, n_x)){
                                    grid[n->py][n->px].tile = n->pr;
                                    n->py = n->py + 1;
                                    n->px = n->px - 1;
                                    n->pr = grid[n->py][n->px].tile;
                                    grid[n->py][n->px].tile = '*';
                                }
                            }
                            else if(mov == '4' || mov == 'h'){
                                int n_y = n->py;
                                int n_x = n->px - 1;
                
                                if(vmove2(n_y, n_x)){
                                    grid[n->py][n->px].tile = n->pr;
                                    n->py = n->py;
                                    n->px = n->px - 1;
                                    n->pr = grid[n->py][n->px].tile;
                                    grid[n->py][n->px].tile = '*';
                                }
                            }
                            else if(mov == 'g' && ter != 'L'){

                                if(n->pr != ' '){
                                    grid[tempsy][tempsx].tile = ts;
                                    grid[n->py][n->px].tile = '@';
                                    tempsy = n->py;
                                    tempsx = n->px;
                                    ts = n->pr;
                                    dijkstra(n);
                                    dijkstra_tunnel(n);
                                    printmon(h, im, n->px, n->py, false);
                                    break;
                                }
                                else{
                                    mvprintw(0, 1, "Can't teleport there, it's a wall or monster");
                                }
                            }
                            else if(mov == 'r'){
                                int x = rand() % 79 + 1;
                                int y = rand() % 20 + 1;

                                grid[tempsy][tempsx].tile = ts;

                                grid[n->py][n->px].tile = n->pr;
                                n->py = y;
                                n->px = x;
                                n->pr = grid[n->py][n->px].tile;
                                grid[n->py][n->px].tile = '@';
                                printmon(h, im, n->px, n->py, false);
                                break;
                            }
                            else if(mov == 27 && ter == 'L'){
                                grid[n->py][n->px].tile = n->pr;
                                n->py = posy;
                                n->px = posx;
                                grid[n->py][n->px].tile = '@';
                                printmon(h, im, n->px, n->py, false);
                                break;
                            }

                            printmon(h, im, n->px, n->py, true);

                        }
                    }                    
                    mov = getch();

                    if(mov == 'g'){
                        grid[n->py][n->px].tile = '@';
                        printmon(h, im, n->px, n->py, true);
                    }

                    if(mov != 'm' && mov != 'f' && mov != 'g' && mov != 'i' && mov != 'o' && mov != 'e' && mov != ',' && mov != 'L' && mov != 'x' && mov != '?' && mov != '~'){
                        break;
                    }
                }
            }

            if(mov == '7' || mov == 'y'){
                int n_y = n->py - 1;
                int n_x = n->px - 1;

                if(vmove(n_y, n_x)){
                    play_walking_sound();
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
                    play_walking_sound();
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
                    play_walking_sound();
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
                    play_walking_sound();
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
                    play_walking_sound();
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
                    play_walking_sound();
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
                    play_walking_sound();
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
                    play_walking_sound();
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

            if(!n->light){
                explore(n, 2);
            }
            else{
                if(n->light->name == "a torch"){
                    explore(n, 4);
                }
                else{
                    explore(n, 7);
                }
            }
            
            
            printmon(h, im, n->px, n->py, false);
            //usleep(500000);
            
        }
        else if(mbit == 0b0000){
            mon1(n->px, n->py, numr, r, m);
            //printmon();
        }
        else if(mbit == 0b0001) {
            mon2(n->px, n->py, numr, r, m);
            //printmon();
        }
        else if(mbit == 0b0010){
            mon3(n->px, n->py, numr, r, m);
            //printmon();s
        }
        else if(mbit == 0b0011){
            mon4(n->px, n->py, numr, r, m);
            //printmon();
        }
        else if(mbit == 0b0100){
            mon5(n->px, n->py, numr, r, m);
            //printmon();
        }
        else if(mbit == 0b0101){
            mon6(n->px, n->py, numr, r, m);
            //printmon();
        }else if(mbit == 0b0110){
            mon7(n->px, n->py, numr, r, m, n->pr);
            //printmon();
        }
        else if(mbit == 0b0111){
            mon8(n->px, n->py, numr, r, m, n->pr);
            //printmon();
        }
        else if(mbit == 0b1000){
            mon9(n->px, n->py, numr, r, m);
            //printmon();
        }
        else if(mbit == 0b1001){
            mon10(n->px, n->py, numr, r, m);
            //printmon();
        }
        else if(mbit == 0b1010){
            mon11(n->px, n->py, numr, r, m);
            //printmon();
        }
        else if(mbit == 0b1011){
            mon12(n->px, n->py, numr, r, m);
            //printmon();
        }
        else if(mbit == 0b1100){
            mon13(n->px, n->py, numr, r, m, n->pr);
            //printmon();
        }
        else if(mbit == 0b1101){
            mon14(n->px, n->py, numr, r, m, n->pr);
            //printmon();
        }
        else if(mbit == 0b1110){
            mon15(n->px, n->py, numr, r, m, n->pr);
            //printmon();
        }
        else if(mbit == 0b1111){
            mon16(n->px, n->py, numr, r, m, n->pr);
            //printmon();
        }

        //printf("%s\n", m.type)s;

        bool temp = true;
       

        if(m->my == n->py && m->mx == n->px && m->name != "????"){

            if(combat(m, n)){
                break;
            }

            if(m->name == "SpongeBob SquarePants"){ //Game is over
                return 2;
            }

            grid[m->my][m->mx].tile2 = '+';

            if(grid[m->my][m->mx - 1].tile == '#'){
                grid[m->my][m->mx].tile = '#';
            }
            else{
                grid[m->my][m->mx].tile = '.';
            }
            
            temp = false;
            clear();

        }

        if(!n->cloak){
            dijkstra(n);
            dijkstra_tunnel(n);
        }

        if(temp){
            m->next = m->next + (1000/m->speed);
            minsert(h, m);
        }
    }

    return 1;
}

// void printmon(MonsterHeap &h){

//     if(grid[0][0].on == -1){

//         for(int i = 0; i <MAX; i++){
//             for(int j = 0; j < MAXR; j++){

//                 if(grid[i][j].explored == 1){
//                 if(grid[i][j].tile2 != '+'){
//                     mvaddch(i, j, grid[i][j].tile2);
//                 }
//                 else{
//                     mvaddch(i, j, grid[i][j].tile);
//                 }
//             }
//             else{
//                 mvaddch(i, j, ' ');

//             }
//             }
//             //printf("\n");
//         }

//     }
//     else{
//     //printf("\n");

//     //clear();

//     for(int i = 0; i <MAX; i++){
//         for(int j = 0; j < MAXR; j++){
//             if(grid[i][j].tile2 != '+'){
//                 mvaddch(i, j, grid[i][j].tile2);
//             }
//             else{
//                 mvaddch(i, j, grid[i][j].tile);
//             }
//         }
//         //printf("\n");
//     }
// }
//    add_color(h);

//     refresh();
// }

void printmon(MonsterHeap &h, const std::vector<Item*> im, int px, int py, bool tele) {

    bool placement = true;

    if(grid[0][0].on == - 1){
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAXR; j++) {
            bool drewMonster = false;
            for (int m = 0; m < h.size; m++) {


                if (h.hp[m]->mx == j && h.hp[m]->my == i && grid[i][j].explored == 1) {
                    std::string color = h.hp[m]->color.substr(0, h.hp[m]->color.find(' '));
                    int color_pair = COLOR_WHITE;

                    if (color == "GREEN")   color_pair = COLOR_GREEN;
                    else if (color == "YELLOW") color_pair = COLOR_YELLOW;
                    else if (color == "RED")    color_pair = COLOR_RED;
                    else if (color == "BLUE")   color_pair = COLOR_BLUE;
                    else if (color == "MAGENTA")color_pair = COLOR_MAGENTA;
                    else if (color == "CYAN")   color_pair = COLOR_CYAN;

                    attron(COLOR_PAIR(color_pair));
                    mvaddch(i, j, h.hp[m]->symb[0]); 
                    attroff(COLOR_PAIR(color_pair));

                    drewMonster = true;
                    break;
                }  
            }

            for(int k = 0; k < im.size(); k++){
                if(im[k]->ix == j && im[k]->iy == i && grid[i][j].explored == 1 && j != py && i != px){
                    std::string color = im[k]->color.substr(0, im[k]->color.find(' '));
                    int color_pair = COLOR_WHITE;

                    if (color == "GREEN")   color_pair = COLOR_GREEN;
                    else if (color == "YELLOW") color_pair = COLOR_YELLOW;
                    else if (color == "RED")    color_pair = COLOR_RED;
                    else if (color == "BLUE")   color_pair = COLOR_BLUE;
                    else if (color == "MAGENTA")color_pair = COLOR_MAGENTA;
                    else if (color == "CYAN")   color_pair = COLOR_CYAN;

                    attron(COLOR_PAIR(color_pair));
                    mvaddch(i, j, im[k]->symb); 
                    attroff(COLOR_PAIR(color_pair));

                    drewMonster = true;
                    break;
                }
            }

            if (!drewMonster) {
                if (grid[i][j].explored == 0) {
                    mvaddch(i, j, ' ');
                } else {
                    if (grid[i][j].tile2 != '+') {
                        mvaddch(i, j, grid[i][j].tile2);
                    } else {
                        mvaddch(i, j, grid[i][j].tile);
                    }
                }
            }

            if (i == py && j == px && tele) {
                mvaddch(i, j, '*'); 
            }
            else if(i == py && j == px && placement){
                mvaddch(i, j, '@'); 
                placement = false;
            }
        }
    }
}
else{
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAXR; j++) {
            bool drewMonster = false;

            for (int m = 0; m < h.size; m++) {

                if (h.hp[m]->mx == j && h.hp[m]->my == i && j != py && i != px) {
                    std::string color = h.hp[m]->color.substr(0, h.hp[m]->color.find(' '));
                    int color_pair = COLOR_WHITE;

                    if (color == "GREEN")   color_pair = COLOR_GREEN;
                    else if (color == "YELLOW") color_pair = COLOR_YELLOW;
                    else if (color == "RED")    color_pair = COLOR_RED;
                    else if (color == "BLUE")   color_pair = COLOR_BLUE;
                    else if (color == "MAGENTA")color_pair = COLOR_MAGENTA;
                    else if (color == "CYAN")   color_pair = COLOR_CYAN;

                    attron(COLOR_PAIR(color_pair));
                    mvaddch(i, j, h.hp[m]->symb[0]); 
                    attroff(COLOR_PAIR(color_pair));

                    drewMonster = true;
                    break;
                }  
            }

            for(int k = 0; k < im.size(); k++){
                if(im[k]->ix == j && im[k]->iy == i && j != py && i != px){
                    std::string color = im[k]->color.substr(0, im[k]->color.find(' '));
                    int color_pair = COLOR_WHITE;

                    if (color == "GREEN")   color_pair = COLOR_GREEN;
                    else if (color == "YELLOW") color_pair = COLOR_YELLOW;
                    else if (color == "RED")    color_pair = COLOR_RED;
                    else if (color == "BLUE")   color_pair = COLOR_BLUE;
                    else if (color == "MAGENTA")color_pair = COLOR_MAGENTA;
                    else if (color == "CYAN")   color_pair = COLOR_CYAN;

                    attron(COLOR_PAIR(color_pair));
                    mvaddch(i, j, im[k]->symb); 
                    attroff(COLOR_PAIR(color_pair));

                    drewMonster = true;
                    break;
                }
            }


            if (!drewMonster) {
                    if (grid[i][j].tile2 != '+') {
                        mvaddch(i, j, grid[i][j].tile2);
                    } else {
                        mvaddch(i, j, grid[i][j].tile);
                    }
                }

                if (i == py && j == px && tele) {
                    mvaddch(i, j, '*'); 
                }
                else if(i == py && j == px ){
                    mvaddch(i, j, '@'); 
                }
            }
        }
    }
    
    refresh();
}

bool vmove(int y, int x){
    if(y <= 19 && y >= 1 && x <= 78 && x >= 1  && grid[y][x].tile != ' '){
        return true;
    }

    return false;
}

bool vmove2(int y, int x){
    if(y <= 19 && y >= 1 && x <= 78 && x >= 1){
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
void mon1(int x, int y, int numr, Room *r, Monster *m){


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
void mon2(int x, int y, int numr, Room *r, Monster *m){
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
void mon3(int x, int y, int numr, Room *r, Monster *m){
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
void mon4(int x, int y, int numr, Room *r, Monster *m){



    struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx = m->mx;  
        int sy = m->my;
        bool found = false;
        
        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node t = directions(&min, i);

            if(t.distance == 0){
                if(min.distance > dist[t.y][t.x].non){
                    min.distance = dist[t.y][t.x].non;
                    sy = t.y;
                    sx = t.x;
                    found = true;
                }
            }

        }

        if(!found){
            return;
        }

        grid[m->my][m->mx].tile2 = '+';

        m->my = sy;
        m->mx = sx;

        grid[m->my][m->mx].tile2 = '3';
}

//0100
void mon5(int x, int y, int numr, Room *r, Monster *m){
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
void mon6(int x, int y, int numr, Room *r, Monster *m){
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
void mon7(int x, int y, int numr, Room *r, Monster *m, char c){
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
                Player* p = new Player();
                grid[m->my][m->mx].tile2 = '+';
                p->px = x;
                p->py = y;
                m->my = sy;
                m->mx = sx;
                grid[m->my][m->mx].tile2 = '6';
                
                dijkstra(p);
                dijkstra_tunnel(p);
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
void mon8(int x, int y, int numr, Room *r, Monster *m, char c){
    struct node min;
        min.distance = 10000;
        min.y = m->my;
        min.x = m->mx;

        int sx, sy;
        
        for(int i = 0; i < 8; i++){//8 for the 8 directions
            struct node t = directions(&min, i);
            if(t.distance == 0){
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
                Player* p = new Player();
                grid[m->my][m->mx].tile2 = '+';
                p->px = x;
                p->py = y;
                m->my = sy;
                m->mx = sx;
                grid[m->my][m->mx].tile2 = '7';

                
                dijkstra(p);
                
                dijkstra_tunnel(p);
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
void mon9(int x, int y, int numr, Room *r, Monster *m){

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
void mon10(int x, int y, int numr, Room *r, Monster *m){
    
    if(rand() % 2){
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile != ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    grid[m->my][m->mx].tile2 = '+';
                    m->mx = sx;
                    m->my = sy;
                    grid[m->my][m->mx].tile2 = '9';
                    return;
                }
            }
        
    }
    else{

        if(m->pox != -1){
            struct node min;
            min.distance = 10000;
            min.y = m->my;
            min.x = m->mx;
    
            int sx = m->mx;
            int sy = m->my;
            bool found = false;
            
            for(int i = 0; i < 8; i++){//8 for the 8 directions
                struct node t = directions(&min, i);
    
                if(t.distance == 0){
                    if(min.distance > dist[t.y][t.x].non){
                        min.distance = dist[t.y][t.x].non;
                        sy = t.y;
                        sx = t.x;
                        found = true;
                    }
                }
    
            }

            if (found) {
    
            grid[m->my][m->mx].tile2 = '+';
    
            m->my = sy;
            m->mx = sx;
    
            grid[m->my][m->mx].tile2 = '9';
            }
        }
    }
}

void mon11(int x, int y, int numr, Room *r, Monster *m){

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

void mon12(int x, int y, int numr, Room *r, Monster *m){
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

void mon13(int x, int y, int numr, Room *r, Monster *m, char c){

    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m ->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile == ' ' && grid[sy][sx].tile != '-' && grid[sy][sx].tile != '|'){
                    if(grid[sy][sx].hardness <= 85){
                        grid[sy][sx].hardness = 0;
                        grid[sy][sx].tile = '#';
                        Player* p = new Player();
                        grid[m->my][m->mx].tile2 = '+';
                        p->px = x;
                        p->py = y;
                        m->my = sy;
                        m->mx = sx;
                        grid[m->my][m->mx].tile2 = 'c';
        
                        
                        dijkstra(p);
                        
                        dijkstra_tunnel(p);
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

void mon14(int x, int y, int numr, Room *r, Monster *m, char c){
    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m ->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile == ' '){
                    if(grid[sy][sx].hardness <= 85){
                        grid[sy][sx].hardness = 0;
                        grid[sy][sx].tile = '#';
                        Player* p = new Player();
                        grid[m->my][m->mx].tile2 = '+';
                        p->px = x;
                        p->py = y;
                        m->my = sy;
                        m->mx = sx;
                        grid[m->my][m->mx].tile2 = 'd';
        
                        
                        dijkstra(p);
                        
                        dijkstra_tunnel(p);
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

void mon15(int x, int y, int numr, Room *r, Monster *m, char c){
    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m ->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile == ' '){
                    if(grid[sy][sx].hardness <= 85){
                        grid[sy][sx].hardness = 0;
                        grid[sy][sx].tile = '#';
                        Player* p = new Player();
                        grid[m->my][m->mx].tile2 = '+';
                        p->px = x;
                        p->py = y;
                        m->my = sy;
                        m->mx = sx;
                        grid[m->my][m->mx].tile2 = 'e';
        
                        
                        dijkstra(p);
                        
                        dijkstra_tunnel(p);
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
                Player* p = new Player();
                grid[m->my][m->mx].tile2 = '+';
                p->px = x;
                p->py = y;
                m->my = sy;
                m->mx = sx;
                grid[m->my][m->mx].tile2 = 'e';

                
                dijkstra(p);
                
                dijkstra_tunnel(p);
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

void mon16(int x, int y, int numr, Room *r, Monster *m, char c){
    if(rand() % 2){
        
            int sx = m->mx + (rand() % 2) * 2 - 1;
            int sy = m ->my + (rand() % 2) * 2 - 1;

            if(sy >= 0 && sy < MAX-1 && sx >= 0 && sx < MAXR - 1){
                if(grid[sy][sx].tile == ' '){
                    if(grid[sy][sx].hardness <= 85){
                        grid[sy][sx].hardness = 0;
                        grid[sy][sx].tile = '#';
                        Player* p = new Player();
                        grid[m->my][m->mx].tile2 = '+';
                        p->px = x;
                        p->py = y;
                        m->my = sy;
                        m->mx = sx;
                        grid[m->my][m->mx].tile2 = 'f';
        
                        
                        dijkstra(p);
                        
                        dijkstra_tunnel(p);
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
                Player* p = new Player();
                grid[m->my][m->mx].tile2 = '+';
                p->px = x;
                p->py = y;
                m->my = sy;
                m->mx = sx;
                grid[m->my][m->mx].tile2 = 'f';

                
                dijkstra(p);
                
                dijkstra_tunnel(p);
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





