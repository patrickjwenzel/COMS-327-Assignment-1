#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <endian.h>
#include <math.h>
#include <unistd.h>
#include <ncurses.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "heap.c"
#include "dungeon.h"

int main(int argc, char *argv[]){
    if(DESC){
        return print_descriptions();
    }
    else{
        srand(time(NULL));
        Dungeon dungeon;
        dungeon.num_rooms = (rand() % 10) + 6; //Generates between 6-15 rooms
        dungeon.num_up = (rand() % 5) + 1; //Will place 1-5 up staircases
        dungeon.num_down = (rand() % 5) + 1; //Will place 1-5 down stair cases
        dungeon.num_mons = (rand() % 29) + 2; //Will place 2-30 monsters
        dungeon.up_stairs = (UpStairs*) (malloc(dungeon.num_up * sizeof(UpStairs)));
        dungeon.down_stairs = (DownStairs*) (malloc(dungeon.num_down * sizeof(DownStairs)));
        dungeon.rooms = (Room*) (malloc(dungeon.num_rooms * sizeof(Room)));
        dungeon.player.alive = 1;
        dungeon.player.speed = 10;
        int num_stairs_placed[2] = {0, 0};
        int init = 0;
        int num_characters = dungeon.num_mons + 1;
        Turn *turn_event;
        dungeon.quit = 0;
        dungeon.del = 0;
        dungeon.fow = 1;

        if(!(dungeon.file = fopen("./output.txt", "w"))){
            std::cout << "Failed to open this file\n";
            return -1;
        }
        if(argc == 1){ //This means that it was just normally ran and dungeon will not be saved
            turn_event = (Turn*) (malloc(num_characters* sizeof(Turn)));
            dungeon.mons = (Monster*) (malloc(dungeon.num_mons * sizeof(Monster)));
            create_monsters(&dungeon);
            fill_dungeon(&dungeon);
            create_dungeon_map(&dungeon, num_stairs_placed, 0);
            do_maps(&dungeon);
            place_monsters(&dungeon);
//        static void turn_decider(Dungeon *dungeon, Turn turn_event[], int *init, int num_characters)

//        turn_decider(&dungeon);

//char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int num_rooms, int num_up, int num_down, int num_stairs_placed[2], int rooms[2], Room room_array[], UpStairs up_stairs[],
// DownStairs down_stairs[], Player player, int save
        }
        else if(argc == 2){
            if(!strcmp(argv[1], "--save")){ //This means the dungeon will be generated and saved;
                turn_event = (Turn*) (malloc(num_characters* sizeof(Turn)));
                dungeon.mons = (Monster*) (malloc(dungeon.num_mons * sizeof(Monster)));
                create_monsters(&dungeon);
                fill_dungeon(&dungeon);
                create_dungeon_map(&dungeon, num_stairs_placed, 1);
                place_monsters(&dungeon);
                do_maps(&dungeon);
            }
            else if(!strcmp(argv[1], "--load")){ //This will load a dungeon
//        void load_game(Dungeon *dungeon, int num_stairs_placed[2], Room room_array[]);
                turn_event = (Turn*) (malloc(num_characters* sizeof(Turn)));
                dungeon.mons = (Monster*) (malloc(dungeon.num_mons * sizeof(Monster)));
                load_game(&dungeon, num_stairs_placed);
                create_monsters(&dungeon);
                place_monsters(&dungeon);
                do_maps(&dungeon);
            }
            else{
                std::cout << "Usage: " << argv[0] << " --save|--load|--nummon <number>\n";
                return -1;
            }
        }
        else if(argc == 3){
            if((!strcmp(argv[1], "--load") && !strcmp(argv[2], "--save")) || (!strcmp(argv[2], "--load") && !strcmp(argv[1], "--save"))){
                load_game(&dungeon, num_stairs_placed);
                save_game(&dungeon, num_stairs_placed);
                dungeon.mons = (Monster*) (malloc(dungeon.num_mons * sizeof(Monster)));
                num_characters = dungeon.num_mons + 1;
                turn_event = (Turn*) (malloc(num_characters* sizeof(Turn)));
                create_monsters(&dungeon);
                place_monsters(&dungeon);
                do_maps(&dungeon);
            }
            else if(!strcmp(argv[1], "--nummon")){ //This will load a dungeon
//        void load_game(Dungeon *dungeon, int num_stairs_placed[2], Room room_array[]);
                dungeon.num_mons = (uint16_t) atoi(argv[2]);
                if(dungeon.num_mons == 1){
                    printf("Must specify more than 1 monster\n");
                    return 1;
                }
                dungeon.mons = (Monster*) (malloc(dungeon.num_mons * sizeof(Monster)));
                num_characters = dungeon.num_mons + 1;
                turn_event = (Turn*) (malloc(num_characters* sizeof(Turn)));
                create_monsters(&dungeon);
                fill_dungeon(&dungeon);
                create_dungeon_map(&dungeon, num_stairs_placed, 0);
                place_monsters(&dungeon);
                do_maps(&dungeon);
            }
            else{
                std::cout << "Usage: " << argv[0] << " --save|--load|--nummon <number>\n";
                return -1;
            }
            do_maps(&dungeon);
        }
        else if(argc == 5){
            if((!strcmp(argv[1], "--load") && !strcmp(argv[2], "--save") && !strcmp(argv[3], "--nummon")) || (!strcmp(argv[2], "--load") && !strcmp(argv[1], "--save") && !strcmp(argv[3], "--nummon"))){
                load_game(&dungeon, num_stairs_placed);
                save_game(&dungeon, num_stairs_placed);
                dungeon.num_mons = (uint16_t) atoi(argv[4]);
                if(dungeon.num_mons == 1){
                    std::cout << "Must specify more than 1 monster\n";
                    return 1;
                }
                dungeon.mons = (Monster*) (malloc(dungeon.num_mons * sizeof(Monster)));
                num_characters = dungeon.num_mons + 1;
                turn_event = (Turn*) (malloc(num_characters* sizeof(Turn)));
                create_monsters(&dungeon);
                place_monsters(&dungeon);
                do_maps(&dungeon);
            }
            else if((!strcmp(argv[1], "--load") && !strcmp(argv[2], "--nummon") && !strcmp(argv[4], "--save")) || (!strcmp(argv[1], "--save") && !strcmp(argv[2], "--nummon") && !strcmp(argv[4], "--load")) ){
                load_game(&dungeon, num_stairs_placed);
                save_game(&dungeon, num_stairs_placed);
                dungeon.num_mons = (uint16_t) atoi(argv[3]);
                if(dungeon.num_mons == 1){
                    std::cout << "Must specify more than 1 monster\n";
                    return 1;
                }
                dungeon.mons = (Monster*) (malloc(dungeon.num_mons * sizeof(Monster)));
                num_characters = dungeon.num_mons + 1;
                turn_event = (Turn*) (malloc(num_characters* sizeof(Turn)));
                create_monsters(&dungeon);
                place_monsters(&dungeon);
                do_maps(&dungeon);
            }
            else if((!strcmp(argv[1], "--nummon") && !strcmp(argv[3], "--load") && !strcmp(argv[4], "--save")) || (!strcmp(argv[1], "--nummon") && !strcmp(argv[3], "--save") && !strcmp(argv[4], "--load"))){
                load_game(&dungeon, num_stairs_placed);
                save_game(&dungeon, num_stairs_placed);
                dungeon.num_mons = (uint16_t) atoi(argv[2]);
                if(dungeon.num_mons == 1){
                    std::cout << "Must specify more than 1 monster\n";
                    return 1;
                }
                dungeon.mons = (Monster*) (malloc(dungeon.num_mons * sizeof(Monster)));
                num_characters = dungeon.num_mons + 1;
                turn_event = (Turn*) (malloc(num_characters* sizeof(Turn)));
                create_monsters(&dungeon);
                place_monsters(&dungeon);
                do_maps(&dungeon);
            }
            else{
                std::cout << "Usage: " << argv[0] << " --save|--load|--nummon <number>\n";
                return -1;
            }
        }
        dungeon.total_monsters_faced = dungeon.num_mons;

        initscr();
        raw();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        refresh();
        mvprintw(0, 0, "This is where I'd put a message....IF I HAD ONE!");
        add_cells_to_fow(&dungeon);
        try{
            while(turn_decider(&dungeon, turn_event, &init, num_characters)){
                if(dungeon.del){
                    dungeon.del = 0;
                    delete_dungeon(&dungeon, turn_event, &init, &num_characters);
                }
                fflush(stdout);
                usleep(USLEEP_MAX/FPS);
            }
            endwin();
            fclose(dungeon.file);
            std::cout << (dungeon.player.alive && !dungeon.quit ? victory : dungeon.quit ? quit : tombstone);
            return 0;
        }
        catch(std::exception& e){
            endwin();
            std::cout << "Exception caught: " << e.what() << std::endl;
            return 1;
        }
    }
}

void reset_monster(Monster *monster){
    monster->x_pos = -1;
    monster->y_pos = -1;
    monster->alive = 1;
    monster->speed = 0;
    monster->type = 0;
    monster->rep = ' ';
    monster->map_made = 0;
    monster->name = "";
    monster->desc = "";
    monster->Speed.base = -1;
    monster->Speed.number = -1;
    monster->Speed.sides = -1;
    monster->health.number = -1;
    monster->health.base = -1;
    monster->health.sides = -1;
    monster->damage.base = -1;
    monster->damage.sides = -1;
    monster->damage.number = -1;
    monster->rrty = -1;
}

int print_descriptions(){
    int i;
    std::string path = getenv("HOME");
    path += "/.rlg327/descriptions/monster_desc.txt";

    std::ifstream file(path);
    std::string read;

    std::vector<Monster> mons;

    getline(file, read);;

    if(read != "RLG327 MONSTER DESCRIPTION 1"){
        std::cout << "File header is invalid." << std::endl;
        return 1;
    }

    else{
        while(!file.eof()){
            getline(file, read);
            if(read == "BEGIN MONSTER"){
                Monster mon;
                reset_monster(&mon);
                while(read != "END"){
                    file >> read;
                    if(read == "NAME"){
                        if(mon.name == ""){
                            getline(file, read);
                            mon.name = read.substr(1, std::string::npos);
                        }
                    }
                    else if(read == "DESC"){
                        getline(file, read);
                        while(read != "."){
                            mon.desc += read + "\n";
                            getline(file, read);
                        }
                    }
                    else if(read == "SYMB"){
                        file >> read;
                        mon.rep = read[0];
                    }
                    else if(read == "COLOR"){
                        std::string color = "";
                        getline(file, read);
                        read = read.substr(1, std::string::npos);
                        std::istringstream all_colors(read);
                        int j;

                        while(getline(all_colors, read, ' ')){
                            for(j = 0; j < 8; j++){
                                if(read == colors[j]){
                                    mon.color.push_back(j);
                                }
                            }
                        }
                    }
                    else if(read == "SPEED"){
                        file >> read;
                        int i;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                mon.Speed.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                mon.Speed.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                mon.Speed.sides = std::stoi(num);
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "HP"){
                        file >> read;
                        int i;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                mon.health.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                mon.health.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                mon.health.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "DAM"){
                        file >> read;
                        int i;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                mon.damage.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                mon.damage.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                mon.damage.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "ABIL"){
                        std::string color = "";
                        getline(file, read);
                        read = read.substr(1, std::string::npos);
                        std::istringstream all_abils(read);
                        uint8_t abil = 0b00000000;

                        while(getline(all_abils, read, ' ')){
                            if(read == std::string("SMART")){
                                abil |= SMART;
                            }
                            else if(read == std::string("TELE")){
                                abil |= TELEPATHIC;
                            }
                            else if(read == std::string("TUNNEL")){
                                abil |= TUNNEL;
                            }
                            else if(read == std::string("ERRATIC")){
                                abil |= ERRATIC;
                            }
                            else if(read == std::string("PASS")){
                                abil |= PASS;
                            }
                            else if(read == std::string("PICKUP")){
                                abil |= PICKUP;
                            }
                            else if(read == std::string("DESTROY")){
                                abil |= DESTROY;
                            }
                            else if(read == std::string("UNIQ")){
                                abil |= UNIQ;
                            }
                            mon.abil = abil;
                        }
                    }
                    else if(read == "RRTY"){
                        file >> read;
                        mon.rrty = std::stoi(read);
                    }
                    else if(read != "END"){
                        std::cout << read << " is an invalid format" << std::endl;
                        return 1;
                    }
                }
                mons.push_back(mon);
            }
        }
        for(i = 0; i < (int) (mons.size()); i++){
            std::cout << "Monster #" << i + 1 << ":" << std::endl;
            mons[i].print_monster();
        }
        return 0;
    }
}

void delete_dungeon(Dungeon *dungeon, Turn turn[], int *init, int *num_characters){
    free(dungeon->up_stairs);
    free(dungeon->down_stairs);
    free(dungeon->rooms);
    free(turn);
    free(dungeon->mons);

    dungeon->num_rooms = (rand() % 10) + 6; //Generates between 6-15 rooms
    dungeon->num_up = (rand() % 5) + 1; //Will place 1-5 up staircases
    dungeon->num_down = (rand() % 5) + 1;
    dungeon->up_stairs = (UpStairs*) (malloc(dungeon->num_up * sizeof(UpStairs)));
    dungeon->down_stairs = (DownStairs*) (malloc(dungeon->num_down * sizeof(DownStairs)));
    dungeon->rooms = (Room*) (malloc(dungeon->num_rooms * sizeof(Room)));
    dungeon->mons = (Monster*) (malloc(dungeon->num_mons * sizeof(Monster)));

    int num_stairs_placed[2] = {0, 0};
    *init = 0;
    *num_characters = dungeon->num_mons + 1;
    turn = (Turn*) (malloc(*num_characters * sizeof(Turn)));

    dungeon->total_monsters_faced += dungeon->num_mons;
    create_monsters(dungeon);
    fill_dungeon(dungeon);
    create_dungeon_map(dungeon, num_stairs_placed, 0);
    place_monsters(dungeon);
    do_maps(dungeon);
    add_cells_to_fow(dungeon);
}

//Moving Functions
int seen(int mons[2]){
    return mons[0] != -1 && mons[1] != -1;
}

static int turn_decider(Dungeon *dungeon, Turn turn_event[], int *init, int num_characters){
    heap_t heap;
    int i;
    Turn *t;
    int move, tport = 0;
    int next_pos[2];

    if(!*init){
        for(i = 0; i < num_characters; i++){
            if(!i){
                turn_event[i].seq = 0;
                turn_event[i].next_turn = 0;
                turn_event[i].symb = PLAYER;
                turn_event[i].speed = dungeon->player.speed;
            }
            else{
                turn_event[i].seq = i;
                turn_event[i].next_turn = 0;
                turn_event[i].symb = dungeon->mons[i - 1].rep;
                turn_event[i].speed = dungeon->mons[i - 1].speed;
            }
        }
        *init = 1;
    }

    heap_init(&heap, turn_cmp, NULL);
    for(i = 0; i < num_characters; i++){
        turn_event[i].heap_node = (!i || dungeon->mons[i - 1].alive) ? heap_insert(&heap, &turn_event[i]) : NULL;
    }

    while((t = (Turn*) heap_remove_min((&heap))) && get_num_alive_monsters(dungeon) > 0 && dungeon->player.alive){
//        printf("%c: %d %d %d\n", t->symb, t->next_turn, t->seq, t->speed);
        t->next_turn = t->next_turn + floor((double)(1000/t->speed));
        if(!t->seq){
            print_dungeon(dungeon);
            refresh();
            next_pos[0] = dungeon->player.y_pos;
            next_pos[1] = dungeon->player.x_pos;
            do{
                tport = 0;
                move = getch();
                if(move == 'Q'){
                    heap_delete(&heap);
                    dungeon->quit = 1;
                    return 0;
                }
                else if(move == '5' || move == ' ' || move == '.' || move == KEY_B2){ // Rest
                    
                }
                else if(move == 'm'){ //Display monster list
                    display_monsters(dungeon);
                }
                else if(move == '7' || move == 'y' || move == KEY_HOME){ //Up->left
                    next_pos[0] -= 1;
                    next_pos[1] -= 1;
                }
                else if(move == '8' || move == 'k' || move == KEY_UP){//Up
                    next_pos[0] -= 1;
                }
                else if(move == '9' || move == 'u' || move == KEY_PPAGE){//Up->Right
                    next_pos[0] -= 1;
                    next_pos[1] += 1;
                }
                else if(move == '6' || move == 'l' || move == KEY_RIGHT){//Right
                    next_pos[1] += 1;
                }
                else if(move == '3' || move == 'n' || move == KEY_NPAGE){//Down->Right
                    next_pos[0] += 1;
                    next_pos[1] += 1;
                }
                else if(move == '2' || move == 'j' || move == KEY_DOWN){ //Down
                    next_pos[0] += 1;
                }
                else if(move == '1' || move == 'b' || move == KEY_END){ //Down->left
                    next_pos[0] += 1;
                    next_pos[1] -= 1;
                }
                else if(move == '4' || move == 'h' || move == KEY_LEFT){ //Left
                    next_pos[1] -= 1;
                }
                else if(move == '?'){
                    print_help(dungeon);
                }
                else if((move == DOWN && find_stairs(dungeon, dungeon->player.x_pos, dungeon->player.y_pos, 0) == DOWN) || (move == UP && find_stairs(dungeon, dungeon->player.x_pos, dungeon->player.y_pos, 0) == UP)){
                    heap_delete(&heap);
                    dungeon->del = 1;
                    return 1;
                }
                else if(move  == 'g'){
                    mvprintw(22, 60, "Move %c       ", 'g');
                    refresh();
                    move = getch();
                    tport = 1;
                    next_pos[0] = dungeon->player.y_pos;
                    next_pos[1] = dungeon->player.x_pos;
                    if(move == 'r'){
                        while(1){
                            next_pos[1] = (rand() % (MAP_X_MAX - 2)) + 1;
                            next_pos[0] = (rand() % (MAP_Y_MAX - 2)) + 1;
                            if(is_open(dungeon, next_pos)){
                                break;
                            }
                        }
                    }
                    else{
                        teleport(dungeon, next_pos);
                    }
                }
                else if(move == 'f'){
                    dungeon->fow = !dungeon->fow;
                    print_dungeon(dungeon);
                    mvprintw(0, 0, dungeon->fow ? "The Fog of War map is now enabled. You can only see a 5 block radius                    " :
                                                "The Fog of War map is now disabled. Press f again to disable it.                           ");
                }
                else{
                    move = 'm';
                }

            }while(move == 'm' || move == '?' || move == 'f');

            if((next_pos[0] <= 0 || next_pos[0] >= MAP_Y_MAX - 1) || (next_pos[1] <= 0 || next_pos[1] >= MAP_X_MAX - 1) || (next_pos[0] == dungeon->player.y_pos && next_pos[1] == dungeon->player.x_pos)){
                continue;
            }
            if(dungeon->hardness[next_pos[0]][next_pos[1]] && !tport){
                mvprintw(0, 0, "You can't go through walls you silly goose! Stick to the dungeon.               ");
                continue;
            }
            mvprintw(22, 60, "Move %c     ", move == KEY_UP ? 'U' : move == KEY_DOWN ? 'D' : move == KEY_RIGHT ? 'R' : move == KEY_LEFT ? 'L' : move == KEY_PPAGE ? 'P' : move == KEY_NPAGE ? 'N' : move == KEY_HOME ? 'H' : move == KEY_END ? 'E' : move);
            move_character(dungeon, *t, next_pos);
            add_cells_to_fow(dungeon);
            print_dungeon(dungeon);
            refresh();
        }
        else{
            if(dungeon->mons[t->seq - 1].alive){
                move_character(dungeon, *t, next_pos);
                add_cells_to_fow(dungeon);
            }
        }

        t->heap_node = NULL;
        if(!get_num_alive_monsters(dungeon)){
            return 0;
        }
        if(dungeon->player.alive) continue;
        else break;
    }
    return dungeon->player.alive;
}

void teleport(Dungeon *dungeon, int next_pos[2]){
    int move;
    mvprintw(0, 0, "Move where you want to teleport with arrow keys. Cannot teleport onto a monster                ");
    mvaddch(dungeon->player.y_pos + 1, dungeon->player.x_pos, TELEPORT);
    refresh();
    next_pos[0] = dungeon->player.y_pos;
    next_pos[1] = dungeon->player.x_pos;
    while(1){
        move = getch();

        if(move == KEY_UP){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[0]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_DOWN){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_LEFT){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_RIGHT){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_PPAGE){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            next_pos[0]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_NPAGE){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_END){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]--;
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_HOME){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]--;
            next_pos[0]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }

        if(move == 'g'){
            if(is_open(dungeon, next_pos)){
                return;
            }
            else{
                mvprintw(0, 0, "Cannot teleport onto a monster                                                 ");
            }
        }
    }
}

int get_num_alive_monsters(Dungeon *dungeon){
    int i, count = 0;
    for(i = 0; i < int (dungeon->num_mons); i++){
        count += dungeon->mons[i].alive;
    }
    return count;
}

int is_open(Dungeon *dungeon, int next_pos[2]){
    int i;
    for(i = 0; i < int (dungeon->num_mons); i++){
        if((dungeon->mons[i].alive && (next_pos[0] == dungeon->mons[i].y_pos && next_pos[1] == dungeon->mons[i].x_pos)) || (next_pos[0] == dungeon->player.y_pos && next_pos[1] == dungeon->player.x_pos)){
            return 0;
        }
    }
    return 1;
}

int is_character(Dungeon *dungeon, Monster mons[], int next_pos[2], Turn turn){
    int i;
    if(!turn.seq){
        for(i = 0; i < int (dungeon->num_mons); i++){
            if(mons[i].alive && (next_pos[0] == mons[i].y_pos && next_pos[1] == mons[i].x_pos)){
                mons[i].alive = 0;
                mvprintw(0, 0, "You killed a monster, way to go!                                               ");
                refresh();
                return 1;
            }
        }
    }
    else{
        for(i = 0; i < int (dungeon->num_mons); i++){
            if(mons[i].alive && (next_pos[0] == dungeon->mons[i].y_pos && next_pos[1] == dungeon->mons[i].x_pos) && !(next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos)){ //If the next space is a monster, kill the monster
                mons[i].alive = uint8_t (0);
                return 1;
            }
            else if(next_pos[0] == dungeon->player.y_pos && next_pos[1] == dungeon->player.x_pos && turn.seq){ //If it is the player, kill the player
                dungeon->player.alive = uint8_t (0);
                return -1;
            }
        }
    }
    return 0;
}

void add_cells_to_fow(Dungeon *dungeon){
    int j, i;
    for(j = 1; j < MAP_Y_MAX - 1; j++){
        for(i = 1; i < MAP_X_MAX - 1; i++){
            if(((j >= dungeon->player.y_pos - 2) && (j <= dungeon->player.y_pos + 2)) && ((i >= dungeon->player.x_pos - 2) && (i <= dungeon->player.x_pos + 2))){
                dungeon->fmap[j][i] = dungeon->dmap[j][i];
            }
            else{
                if(dungeon->fmap[j][i] == ROCK){

                }
                else{
                    dungeon->fmap[j][i] = find_stairs(dungeon, i, j, find_room(dungeon, i, j) == -1);
                }
            }
        }
    }
}

void move_character(Dungeon *dungeon, Turn turn, int next_pos[2]){
    int character, is_room;
    if(!turn.seq){
        character = is_character(dungeon, dungeon->mons, next_pos, turn);
        is_room = find_room(dungeon, dungeon->player.x_pos, dungeon->player.y_pos);
        if(character == 1){ //If the next space is a monster
            dungeon->dmap[next_pos[0]][next_pos[1]] = PLAYER;
            dungeon->fmap[next_pos[0]][next_pos[1]] = PLAYER;
            dungeon->dmap[dungeon->player.y_pos][dungeon->player.x_pos] = find_stairs(dungeon, dungeon->player.x_pos, dungeon->player.y_pos, is_room == -1);
            dungeon->fmap[dungeon->player.y_pos][dungeon->player.x_pos] = find_stairs(dungeon, dungeon->player.x_pos, dungeon->player.y_pos, is_room == -1);
            dungeon->player.x_pos = next_pos[1]; //Move the monster
            dungeon->player.y_pos = next_pos[0];
        }
        else if(!character){ //If it is rock
            dungeon->dmap[next_pos[0]][next_pos[1]] = PLAYER;
            dungeon->dmap[dungeon->player.y_pos][dungeon->player.x_pos] = find_stairs(dungeon, dungeon->player.x_pos, dungeon->player.y_pos, is_room == -1);
            dungeon->player.x_pos = next_pos[1]; //Move the monster
            dungeon->player.y_pos = next_pos[0];
        }
        if(!dungeon->hardness[dungeon->player.y_pos][dungeon->player.x_pos]){
            dijkstra_map(dungeon, 0, NULL);
            dijkstra_map(dungeon, 1, NULL);
        }
        else{
            dijkstra_map(dungeon, 1, NULL);
        }
    }
    else{
        if(dungeon->mons[turn.seq - 1].type == 0 || dungeon->mons[turn.seq - 1].type == 8){
            while(1){
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                if((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->hardness[next_pos[0]][next_pos[1]] || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE) continue;
                else break;
            }
            character = is_character(dungeon, dungeon->mons, next_pos, turn);
            is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos);
            if(character == 1){
                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
//                char find_stairs(Dungeon *dungeon, int x, int y, int is_corridor)
                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon,dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
            }
            else if(!character){
                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
            }
            else{
                return;
            }
        }
        else if(dungeon->mons[turn.seq - 1].type == 4 || dungeon->mons[turn.seq - 1].type == 12){
            while(1){
                // Randomly generate a number [-1, 1] for x and y direction that the monster will move
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                if((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE) continue;
                else break;
            }
            character = is_character(dungeon, dungeon->mons, next_pos, turn);
            is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos);
            if(character == 1){ //If the next space is a monster
                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                if(is_room == -1){ //Checks if you are in a room
                    do_maps(dungeon); //Redo distance maps
                }
                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
            }
            else if(!character){ //If it is rock
                if(dungeon->hardness[next_pos[0]][next_pos[1]] - 85 <= 0){ //Can the monster destroy this rock
                    dungeon->hardness[next_pos[0]][next_pos[1]] = 0;
                    dungeon->hardness[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = 0;
                    dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                    if(is_room == -1){ //Checks if you are in a room
                        do_maps(dungeon); //Redo distance maps
                    }
                    dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                    dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                    dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                }
                else{
                    dungeon->hardness[next_pos[0]][next_pos[1]] -= 85; //Break the rock down by 85
                }
            }
            else{
                return;
            }
        }
        else {
            if (dungeon->mons[turn.seq - 1].type & TELEPATHIC) { //Monster is telepathic
                dungeon->mons[turn.seq - 1].pc_location[0] = dungeon->player.y_pos;
                dungeon->mons[turn.seq - 1].pc_location[1] = dungeon->player.x_pos;
                if (dungeon->mons[turn.seq - 1].type & SMART) { //Monster is telepathic and smart
                    if (dungeon->mons[turn.seq - 1].type & TUNNEL) { //Monster is telepathic, smart, and can tunnel
                        get_next_pos(dungeon, turn, next_pos, 1, 1, 1);
                        if (dungeon->mons[turn.seq - 1].type & ERRATIC && rand() & CHANCE) {
                            while (1) {
                                // Randomly generate a number [-1, 1] for x and y direction that the monster will move
                                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                                if ((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE)
                                    continue;
                                else break;
                            }
                        }
                        character = is_character(dungeon, dungeon->mons, next_pos, turn);
                        is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos);
                        if(character == 1){ //If the next space is a monster
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(!character){ //If it is rock
                            if(dungeon->hardness[next_pos[0]][next_pos[1]] - 85 <= 0){ //Can the monster destroy this rock
                                dungeon->hardness[next_pos[0]][next_pos[1]] = 0;
                                dungeon->hardness[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = 0;
                                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                                if(is_room == -1){ //Checks if you are in a room
                                    do_maps(dungeon); //Redo distance maps
                                }
                                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                                dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                            }
                            else{
                                dungeon->hardness[next_pos[0]][next_pos[1]] -= 85; //Break the rock down by 85
                            }
                        }
                        else {
                            return;
                        }
                    }
                    else { // Is Smart and Telepathic
                        get_next_pos(dungeon, turn, next_pos, 1, 0, 1);
                        if (dungeon->mons[turn.seq - 1].type & ERRATIC && rand() & CHANCE) {
                            while (1) {
                                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                                if ((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->dmap[next_pos[0]][next_pos[1]] == ROCK || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE)
                                    continue;
                                else break;
                            }
                        }
                        character = is_character(dungeon, dungeon->mons, next_pos, turn);
                        is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos,dungeon->mons[turn.seq - 1].y_pos);
                        if(character == 1){
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
//                char find_stairs(Dungeon *dungeon, int x, int y, int is_corridor)
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon,dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(!character){
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else {
                            return;
                        }
                    }
                }
                else { // Is Telepathic but not smart
                    if (dungeon->mons[turn.seq - 1].type & TUNNEL) { //If telepathic and can tunnel
                        get_next_pos(dungeon, turn, next_pos, 1, 1, 0);
                        if (dungeon->mons[turn.seq - 1].type & ERRATIC && rand() & CHANCE) {
                            while (1) {
                                // Randomly generate a number [-1, 1] for x and y direction that the monster will move
                                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                                if ((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE)
                                    continue;
                                else break;
                            }
                        }
                        character = is_character(dungeon, dungeon->mons, next_pos, turn);
                        is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos);
                        if(character == 1){ //If the next space is a monster
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(!character){ //If it is rock
                            if(dungeon->hardness[next_pos[0]][next_pos[1]] - 85 <= 0){ //Can the monster destroy this rock
                                dungeon->hardness[next_pos[0]][next_pos[1]] = 0;
                                dungeon->hardness[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = 0;
                                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                                if(is_room == -1){ //Checks if you are in a room
                                    do_maps(dungeon); //Redo distance maps
                                }
                                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                                dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                            }
                            else{
                                dungeon->hardness[next_pos[0]][next_pos[1]] -= 85; //Break the rock down by 85
                            }
                        }
                        else {
                            return;
                        }
                    }
                    else { //Is only telepathic and possibly erratic
                        get_next_pos(dungeon, turn, next_pos, 1, 0, 0);
                        if (dungeon->mons[turn.seq - 1].type & ERRATIC && rand() & CHANCE) {
                            while (1) {
                                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                                if ((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->hardness[next_pos[0]][next_pos[1]] || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE)
                                    continue;
                                else break;
                            }
                        }
                        character = is_character(dungeon, dungeon->mons, next_pos, turn);
                        is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos,dungeon->mons[turn.seq - 1].y_pos);
                        if(character == 1 && !(next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos)){
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
//                char find_stairs(Dungeon *dungeon, int x, int y, int is_corridor)
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon,dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(!character){
                            if (!(next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos)) {
                                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                                dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                            }
                        }
                        else {
                            return;
                        }
                    }
                }
            }
            else { //If not Telepathic
                if (dungeon->mons[turn.seq - 1].type & SMART) { //Monster is not telepathic and smart
                    if (dungeon->mons[turn.seq - 1].type & TUNNEL) {
                        if (can_be_seen(dungeon, dungeon->mons[turn.seq - 1], dungeon->player)) {
                            dungeon->mons[turn.seq - 1].pc_location[0] = dungeon->player.y_pos;
                            dungeon->mons[turn.seq - 1].pc_location[1] = dungeon->player.x_pos;
                            if (dungeon->player.y_pos != dungeon->mons[turn.seq - 1].pc_location[0] && dungeon->player.x_pos != dungeon->mons[turn.seq - 1].pc_location[1]) {
                                dijkstra_map(dungeon, dungeon->mons[turn.seq - 1].type & TUNNEL,&dungeon->mons[turn.seq - 1]);
                                dungeon->mons[turn.seq - 1].map_made = 1;
                            }
                        }
                        get_next_pos(dungeon, turn, next_pos, 0, 1, 1);
                        if ((dungeon->mons[turn.seq - 1].type & ERRATIC && rand() & CHANCE) || !dungeon->mons[turn.seq - 1].map_made) {
                            while (1) {
                                // Randomly generate a number [-1, 1] for x and y direction that the monster will move
                                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                                if ((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE)
                                    continue;
                                else break;
                            }
                        }
                        character = is_character(dungeon, dungeon->mons, next_pos, turn);
                        is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos);
                        if(character == 1){ //If the next space is a monster
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(!character){ //If it is rock
                            if(dungeon->hardness[next_pos[0]][next_pos[1]] - 85 <= 0){ //Can the monster destroy this rock
                                dungeon->hardness[next_pos[0]][next_pos[1]] = 0;
                                dungeon->hardness[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = 0;
                                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                                if(is_room == -1){ //Checks if you are in a room
                                    do_maps(dungeon); //Redo distance maps
                                }
                                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                                dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                            }
                            else{
                                dungeon->hardness[next_pos[0]][next_pos[1]] -= 85; //Break the rock down by 85
                            }
                        }
                        else {
                            return;
                        }
                    }
                    else { //Monster is smart
                        int mon_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos,dungeon->mons[turn.seq - 1].y_pos);
                        int player_room = find_room(dungeon, dungeon->player.x_pos, dungeon->player.y_pos);
                        int same_room = !(mon_room == -1 && player_room == -1) && mon_room == player_room;
                        if (same_room) {
                            dungeon->mons[turn.seq - 1].pc_location[0] = dungeon->player.y_pos;
                            dungeon->mons[turn.seq - 1].pc_location[1] = dungeon->player.x_pos;
                            if (dungeon->player.y_pos != dungeon->mons[turn.seq - 1].pc_location[0] && dungeon->player.x_pos != dungeon->mons[turn.seq - 1].pc_location[1]) {
                                dijkstra_map(dungeon, dungeon->mons[turn.seq - 1].type & TUNNEL,&dungeon->mons[turn.seq - 1]);
                                dungeon->mons[turn.seq - 1].map_made = 1;
                            }
                        }
                        get_next_pos(dungeon, turn, next_pos, 1, 0, 0);
                        if ((dungeon->mons[turn.seq - 1].type & ERRATIC && rand() & CHANCE) || !dungeon->mons[turn.seq - 1].map_made) {
                            while (1) {
                                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                                if ((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->hardness[next_pos[0]][next_pos[1]] || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE)
                                    continue;
                                else break;
                            }
                        }
                        character = is_character(dungeon, dungeon->mons, next_pos, turn);
                        is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos,dungeon->mons[turn.seq - 1].y_pos);
                        if(character == 1 && !(next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos)){
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
//                char find_stairs(Dungeon *dungeon, int x, int y, int is_corridor)
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon,dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(!character){
                            if (!(next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos)) {
                                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                                dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                            }
                        }
                        else {
                            return;
                        }
                    }
                }
            }
        }
    }
}

uint8_t can_be_seen(Dungeon *dungeon, Monster monster, Player player){
    int mon_room = find_room(dungeon, monster.x_pos, monster.y_pos);
    int player_room = find_room(dungeon, player.x_pos, player.y_pos);

    if(!(mon_room == -1 && player_room == -1) && mon_room == player_room){
        return 1;
    }

    int x, y;
    for(y = monster.y_pos - 5; (y <= monster.y_pos + 5) && (y > 0) && (y < MAP_Y_MAX - 1); y++){
        for(x = monster.x_pos - 5; (x <= monster.x_pos + 5) && (x > 0) && (x < MAP_X_MAX - 1); x++){
            if(x == player.x_pos && y == player.y_pos){
                return 1;
            }
        }
    }
    return 0;
}

void get_next_pos(Dungeon *dungeon, Turn turn, int next_pos[2], int telepathic, int tunneling, int smart){
    int min;
    if(!telepathic && smart && seen(dungeon->mons[turn.seq - 1].pc_location)){
        min = dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos]; //Set the min from the tunneling distance map to the cell above
        next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
        next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
        if(dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos - 1] < min){ //Up and left
            min = dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos - 1];
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
        }
        if(dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos - 1] < min){ //Left
            min = dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos - 1];
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
        }
        if(dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos - 1] < min){ //Down and left
            min = dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos - 1];
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
        }
        if(dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos] < min){ // Down
            min = dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos];
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
        }
        if(dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos + 1] < min){ //Down and right
            min = dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos + 1];
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
        }
        if(dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos + 1] < min){ //Right
            min = dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos + 1];
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
        }
        if(dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos + 1] < min){ //Up and right
            min = dungeon->mons[turn.seq - 1].distance[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos + 1];
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
        }
    }
    else if(tunneling){//If the monster tunnels
        if(telepathic && smart){ //If it is also smart
            min = dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos]; //Set the min from the tunneling distance map to the cell above
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
            if(dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos - 1] < min){ //Up and left
                min = dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos - 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
            }
            if(dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos - 1] < min){ //Left
                min = dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos - 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
            }
            if(dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos - 1] < min){ //Down and left
                min = dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos - 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
            }
            if(dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos] < min){ // Down
                min = dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
            }
            if(dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos + 1] < min){ //Down and right
                min = dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos + 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
            }
            if(dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos + 1] < min){ //Right
                min = dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos + 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
            }
            if(dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos + 1] < min){ //Up and right
                min = dungeon->t_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos + 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
            }
            /*
             * What I'm doing is checking the values in the distance maps and getting the direction with the smallest distance away and making those
             * coordinates the next position
             */
        }
        else if(!smart){ //If it can tunnel but it is not smart
            if(dungeon->mons[turn.seq - 1].y_pos - dungeon->player.y_pos < 0){ //Player is below
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
            }
            else if(dungeon->mons[turn.seq - 1].y_pos - dungeon->player.y_pos > 0){ // Player is above
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
            }
            else if(dungeon->mons[turn.seq - 1].x_pos - dungeon->player.x_pos < 0){ // Player is to the right
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
            }
            else{// Player is to the left
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
            }
        }
    }
    else{ //If it does not tunnel
        if(telepathic && smart){ //If it is smart
            //Do the same as before but with the non-tunneling distance maps
            min = dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos];
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
            if(dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos - 1] < min){
                min = dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos - 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
            }
            if(dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos - 1] < min){
                min = dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos - 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
            }
            if(dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos - 1] < min){
                min = dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos - 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
            }
            if(dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos] < min){
                min = dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
            }
            if(dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos + 1] < min){
                min = dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos + 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
            }
            if(dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos + 1] < min){
                min = dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos + 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
            }
            if(dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos + 1] < min){
                min = dungeon->nt_distances[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos + 1];
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
            }
        }
        else if(!smart){
            next_pos[1] = dungeon->mons[turn.seq - 1].x_pos;
            next_pos[0] = dungeon->mons[turn.seq - 1].y_pos;
            if(dungeon->mons[turn.seq - 1].y_pos - dungeon->player.y_pos < 0 && !dungeon->hardness[dungeon->mons[turn.seq - 1].y_pos + 1][dungeon->mons[turn.seq - 1].x_pos]){ //Player is below
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + 1;
            }
            else if(dungeon->mons[turn.seq - 1].y_pos - dungeon->player.y_pos > 0 && !dungeon->hardness[dungeon->mons[turn.seq - 1].y_pos - 1][dungeon->mons[turn.seq - 1].x_pos]){ // Player is above
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos - 1;
            }
            else if((dungeon->mons[turn.seq - 1].x_pos - dungeon->player.x_pos < 0) && !dungeon->hardness[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos + 1]){ // Player is to the right
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + 1;
            }
            else{// Player is to the left
                if(!dungeon->hardness[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos - 1]){
                    next_pos[1] = dungeon->mons[turn.seq - 1].x_pos - 1;
                }
            }
        }
    }
}

void print_help(Dungeon *dungeon){
    int i;
    int move;
    clear_dungeon(dungeon);
    do {
        for(i = 2; i < 21; i++){
            mvprintw(i, 0, keys[i - 2]);
        }
        move = getch();
    }while(move != 27);
    print_dungeon(dungeon);
}

void display_monsters(Dungeon *dungeon){
    int i, j;
    int move;
    int gt_index = 0;
    do{
        clear_dungeon(dungeon);
        if(dungeon->num_mons < 22){
            for(i = 0; i < int (dungeon->num_mons); i++){
                if(dungeon->mons[i].alive){
                    int north_south = dungeon->player.y_pos - dungeon->mons[i].y_pos;
                    int west_east = dungeon->player.x_pos - dungeon->mons[i].x_pos;
                    mvprintw(i + 1, 0, "Monster %d. %c, %d %s and %d %s", i + 1, dungeon->mons[i].rep, abs(north_south), north_south >= 0 ? "north" : "south", abs(west_east), west_east >= 0 ? "west" : "east");
                }
                else{
                    mvprintw(i + 1, 0, "Monster %d. %c is dead!", i + 1, dungeon->mons[i].rep);
                }
            }
        }
        else{
            for(i = gt_index, j = 0; j < 21 && i < int (dungeon->num_mons); j++, i++){
                if(dungeon->mons[i].alive){
                    int north_south = dungeon->player.y_pos - dungeon->mons[i].y_pos;
                    int west_east = dungeon->player.x_pos - dungeon->mons[i].x_pos;
                    mvprintw(!gt_index ? i + 1 : i - ((21 * (gt_index/21)) + ((gt_index % 21) - 1)), 0, "Monster %d. %c, %d %s and %d %s            ", i + 1, dungeon->mons[i].rep, abs(north_south), north_south >= 0 ? "north" : "south", abs(west_east), west_east >= 0 ? "west" : "east");
                }
                else{
                    mvprintw(!gt_index ? i + 1 : i - ((21 * (gt_index/21)) + ((gt_index % 21) - 1)), 0, "Monster %d. %c is dead!                 ", i + 1, dungeon->mons[i].rep);
                }
            }
        }
        mvprintw(22, 60, "%d            ", gt_index);
        mvprintw(23, 60, "Monster Move: %c            ", move == KEY_UP ? 'U' : move == KEY_DOWN ? 'D' : move == KEY_RIGHT ? 'R' : move == KEY_LEFT ? 'L' : move == KEY_PPAGE ? 'P' : move == KEY_NPAGE ? 'N' : move == KEY_HOME ? 'H' : move == KEY_END ? 'E' : move);
        refresh();
        move = getch();
        if(move == KEY_UP){
            if(gt_index - 21 < 0){
                
            }
            else{
                gt_index -= 21;
            }
        }
        else if(move == KEY_DOWN){
            if(dungeon->num_mons - gt_index < 21){
                
            }
            else{
                gt_index += 21;
            }
        }
    }while(move != 27);
    mvprintw(22, 60, "                           ");
    mvprintw(23, 60, "                           ");
    print_dungeon(dungeon);
    return;
}

void clear_dungeon(Dungeon *dungeon){
    int i, j;
    for(i = 1; i < 22; i++){
        for(j = 0; j < 81; j++){
            mvaddch(i, j, ' ');
        }
    }
}

static int32_t turn_cmp(const void *key, const void *with) {
    int32_t ret = ((Turn *) key)->next_turn - ((Turn *) with)->next_turn;
    if(!ret){
        ret = ((Turn *) key)->seq - ((Turn *) with)->seq;
    }
    return ret;
}

//Monster functions
void create_monsters(Dungeon *dungeon){
    int i, type;
    for(i = 0; i < int (dungeon->num_mons); i++){
        type = rand() & FIFTEEN; //Randomly get characteristics, returns a number [0, 15] or [0x0000, 0x1111]
        dungeon->mons[i].alive = 1;
        dungeon->mons[i].rep = monster_reps[type];
        dungeon->mons[i].speed = (rand() % 16) + 5; //Gets a number [5, 20]
        dungeon->mons[i].type = type;
        dungeon->mons[i].pc_location[0] = -1; // Manually set this until the monster sees PC and (possibly) remembers it
        dungeon->mons[i].pc_location[1] = -1;
    }
}

void place_monsters(Dungeon *dungeon){
    int i;

    for(i = 0; i < int (dungeon->num_mons); i++){
        int x, y;
        // If it is a tunneling monster
        if(dungeon->mons[i].type & TUNNEL){
            while(1){
                //Get a random number anywhere in the dungeon
                x = (rand() % (MAP_X_MAX - 2)) + 1;
                y = (rand() % (MAP_Y_MAX - 2)) + 1;
                if(dungeon->dmap[y][x] == PLAYER || !far_enough_away(dungeon, x, y)){ //Can't place on top of player. Later will add a distance factor to this
                    continue;
                }
                else{
                    dungeon->dmap[y][x] = dungeon->mons[i].rep;
                    dungeon->hardness[y][x] = 0;
                    dungeon->mons[i].x_pos = x;
                    dungeon->mons[i].y_pos = y;
                    break;
                }
            }
        }
            // If it is a non-tunneling monster
        else{
            while(1){
                x = (rand() % (MAP_X_MAX - 2)) + 1;
                y = (rand() % (MAP_Y_MAX - 2)) + 1;
                /*
                 * Don't place a non-tunneling monster if it is the player or is rock
                 */
                if (dungeon->dmap[y][x] == PLAYER || dungeon->hardness[y][x] || (!dungeon->hardness[y][x] && dungeon->dmap[y][x] == ROCK) || !far_enough_away(dungeon, x, y)) {
                    continue;
                } else {
                    dungeon->dmap[y][x] = dungeon->mons[i].rep;
                    dungeon->hardness[y][x] = 0;
                    dungeon->mons[i].x_pos = x;
                    dungeon->mons[i].y_pos = y;
                    break;
                }
            }
        }
    }
}

int far_enough_away(Dungeon *dungeon, int x, int y){
    return sqrt((pow((double) (x - dungeon->player.x_pos), 2.0)) + (pow((double) (y - dungeon->player.y_pos), 2.0))) > 6;
}

//Path Functions
void do_maps(Dungeon *dungeon){
//    printf("Non-tunneling monster map:\n");
    int i;
    for(i = 0; i <= int (dungeon->num_mons); i++){
        if(!i){
            dijkstra_map(dungeon, 0, NULL);
            dijkstra_map(dungeon, 1, NULL);
        }
        else{
            if(dungeon->mons[i - 1].alive && !(dungeon->mons[i - 1].type & TELEPATHIC) && dungeon->mons[i - 1].type & SMART && seen(dungeon->mons[i - 1].pc_location)){
                dijkstra_map(dungeon, dungeon->mons[i - 1].type & TUNNEL, &dungeon->mons[i - 1]);
            }
        }
    }

//    print_path_map(dungeon->nt_distances, dungeon, 0);
//    printf("Tunneling monster map:\n");
//    static void dijkstra_map(int distances[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], Player player, Room room_array[], int num_rooms, int tunneling);

//    print_path_map(dungeon->t_distances, dungeon, 1);
}

/*
 * From Professor's code
 */
static int32_t corridor_path_cmp(const void *key, const void *with) {
    return ((CorridorPath *) key)->cost - ((CorridorPath *) with)->cost;
}

/*
 * Edited version of Professor's code. Works for tunneling and non-tunneling monsters
 */
static void dijkstra_map(Dungeon *dungeon, int tunneling, Monster *monster){
    static CorridorPath path[MAP_Y_MAX][MAP_X_MAX], *p;
    static uint32_t init = 0;
    heap_t heap;
    uint32_t x, y;
    int weight;
    // pos[0] = y, pos[1] = x
    if(!init){
        for(y = 0; y < MAP_Y_MAX; y++){
            for(x = 0; x < MAP_X_MAX; x++){
                path[y][x].pos[0] = y;
                path[y][x].pos[1] = x;
            }
        }
        init = 1;
    }

    /*
     * Set costs to infinity and initialize distances array to 0's
     */
    for(y = 0; y < MAP_Y_MAX; y++){
        for(x = 0; x < MAP_X_MAX; x++){
            path[y][x].cost = INT_MAX;
            if(monster){
                if(tunneling){
                    monster->distance[y][x] = INT_MAX;
                }
                else{
                    monster->distance[y][x] = INT_MAX;
                }
            }
            else{
                if(tunneling){
                    dungeon->t_distances[y][x] = INT_MAX;
                }
                else{
                    dungeon->nt_distances[y][x] = INT_MAX;
                }
            }
        }
    }

    path[monster ? monster->pc_location[0] : dungeon->player.y_pos][monster ? monster->pc_location[1] : dungeon->player.x_pos].cost = 0;

    heap_init(&heap, corridor_path_cmp, NULL);

    for(y = 0; y < MAP_Y_MAX; y++){
        for(x = 0; x < MAP_X_MAX; x++){
            /*
             * If it is a tunneling monster, only check if it is not the border and if not, add to the heap
             */
            if(tunneling){
                path[y][x].heap_node = dungeon->hardness[y][x] != 255 ? heap_insert(&heap, &path[y][x]) : NULL;
            }
                /*
                 * If not a tunneling monster, only add to the heap if it is a corridor, room, or staircase
                 */
            else{
                path[y][x].heap_node = dungeon->hardness[y][x] == 0 ? heap_insert(&heap, &path[y][x]) : NULL;
            }
        }
    }

    while((p = (CorridorPath*) heap_remove_min((&heap)))){
        p->heap_node = NULL;
        weight = (1 + (dungeon->hardness[p->pos[0]][p->pos[1]] / 85)); // Set the weight so that if it is a room, staircase, or corridor the weight will be one and will be 1+dungeon->hardness/85 if not
        if(monster){
            if(tunneling){
                monster->distance[p->pos[0]][p->pos[1]] = p->cost;
            }
            else{
                monster->distance[p->pos[0]][p->pos[1]] = p->cost;
            }
        }
        else{
            if(tunneling){
                dungeon->t_distances[p->pos[0]][p->pos[1]] = p->cost;
            }
            else{
                dungeon->nt_distances[p->pos[0]][p->pos[1]] = p->cost;
            }
        }

        // Up
        if((path[p->pos[0] - 1][p->pos[1]].heap_node) &&
           (path[p->pos[0] - 1][p->pos[1]].cost > p->cost + weight)){
            path[p->pos[0] - 1][p->pos[1]].cost = p->cost + weight;
            heap_decrease_key_no_replace(&heap, path[p->pos[0] - 1][p->pos[1]].heap_node);
        }
        // Up to the left
        if((path[p->pos[0] - 1][p->pos[1] - 1].heap_node) &&
           (path[p->pos[0] - 1][p->pos[1] - 1].cost > p->cost + weight)) {
            path[p->pos[0] - 1][p->pos[1] - 1].cost = p->cost + weight;
            heap_decrease_key_no_replace(&heap, path[p->pos[0] - 1][p->pos[1] - 1].heap_node);
        }
        // Up to the right
        if((path[p->pos[0] - 1][p->pos[1] + 1].heap_node) &&
           (path[p->pos[0] - 1][p->pos[1] + 1].cost > p->cost + weight)){
            path[p->pos[0] - 1][p->pos[1] + 1].cost = p->cost + weight;
            heap_decrease_key_no_replace(&heap, path[p->pos[0] - 1][p->pos[1] + 1].heap_node);
        }
        // Left
        if((path[p->pos[0]][p->pos[1] - 1].heap_node) &&
           (path[p->pos[0]][p->pos[1] - 1].cost > p->cost + weight)){
            path[p->pos[0]][p->pos[1] - 1].cost = p->cost + weight;
            heap_decrease_key_no_replace(&heap, path[p->pos[0]][p->pos[1] - 1].heap_node);
        }
        // Right
        if((path[p->pos[0]][p->pos[1] + 1].heap_node) &&
           (path[p->pos[0]][p->pos[1] + 1].cost > p->cost + weight)){
            path[p->pos[0]][p->pos[1] + 1].cost = p->cost + weight;
            heap_decrease_key_no_replace(&heap, path[p->pos[0]][p->pos[1] + 1].heap_node);
        }
        // Down
        if((path[p->pos[0] + 1][p->pos[1]].heap_node) &&
           (path[p->pos[0] + 1][p->pos[1]].cost > p->cost + weight)){
            path[p->pos[0] + 1][p->pos[1]].cost = p->cost + weight;
            heap_decrease_key_no_replace(&heap, path[p->pos[0] + 1][p->pos[1]].heap_node);
        }
        //Down to the left
        if((path[p->pos[0] + 1][p->pos[1] - 1].heap_node) &&
           (path[p->pos[0] + 1][p->pos[1] - 1].cost > p->cost + weight)){
            path[p->pos[0] + 1][p->pos[1] - 1].cost = p->cost + weight;
            heap_decrease_key_no_replace(&heap, path[p->pos[0] + 1][p->pos[1] - 1].heap_node);
        }
        //Down to the right
        if((path[p->pos[0] + 1][p->pos[1] + 1].heap_node) &&
           (path[p->pos[0] + 1][p->pos[1] + 1].cost > p->cost + weight)){
            path[p->pos[0] + 1][p->pos[1] + 1].cost = p->cost + weight;
            heap_decrease_key_no_replace(&heap, path[p->pos[0] + 1][p->pos[1] + 1].heap_node);
        }
    }
}

/*
 * Prints the distances map, works for both tunneling and non-tunneling monsters
 */
void print_path_map(int distances[MAP_Y_MAX][MAP_X_MAX], Dungeon *dungeon, int tunneling){
    int j, i;
    for (j = 0; j < MAP_Y_MAX; j++) {
        for (i = 0; i < MAP_X_MAX; i++) {
            if(i == 0 || i == MAP_X_MAX - 1) mvaddch(j, i, SIDE);// putchar(SIDE);
            else if(j == 0 || j == MAP_Y_MAX - 1) mvaddch(j, i, TOP);// putchar(TOP);
            else if(i == dungeon->player.x_pos && j == dungeon->player.y_pos) mvaddch(j, i, PLAYER);// putchar(PLAYER);
            else{
                if(tunneling){
//                    printf("%d", distances[j][i] % 10);
                    mvaddch(j, i, distances[j][i] % 10);
                }
                else{
                    if(dungeon->hardness[j][i] != 0) mvaddch(j, i, ROCK);// putchar(ROCK);
                    else {
                        if(distances[j][i] < 0 || distances[j][i] == INT_MAX) mvaddch(j, i, 'X');  // putchar('X');
                        else mvaddch(j, i, distances[j][i] % 10); //printf("%d", distances[j][i] % 10);
                    }
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

//Dungeon Creation Functions
void create_dungeon_map(Dungeon *dungeon, int num_stairs_placed[2], int save) {
    int i, j = 0, room_x_pos, room_y_pos, num_rooms_placed = 0, num_cycles = 0, player_placed = 0;
    uint8_t x, y;
    int rooms[dungeon->num_rooms][2];
    while(num_cycles < dungeon->num_rooms || num_rooms_placed < dungeon->num_rooms){ //Will loop until all rooms have been placed
        for(i = 0; i < dungeon->num_rooms; i++){
            rooms[i][0]=(rand() % 12) + 4; //Makes the room between 4 and 15 units in the x direction
            rooms[i][1]=(rand() % 12) + 3; //Makes the room between 3 and 15 units in the y direction
        }

        for(i = 0; i < dungeon->num_rooms - num_rooms_placed; i++){
            room_x_pos = (rand() % (MAP_X_MAX - 1)) + 1; //Makes the x coordinate between 1 and 70 (So a border can be placed)
            room_y_pos = (rand() % (MAP_Y_MAX - 1)) + 1;//Makes the x coordinate between 1 and 70 (For Border reasons)

            if(rooms[i][0] + room_x_pos < MAP_X_MAX && rooms[i][1] + room_y_pos < MAP_Y_MAX){ //If the coordinates of the are on the grid

                if(!is_room(*dungeon, room_x_pos, room_y_pos, rooms, i)){ //If the room does not collide with another room
                    dungeon->rooms[num_rooms_placed] = newRoom(room_x_pos, room_y_pos, rooms[i][0], rooms[i][1]);
                    for(y = room_y_pos; y < room_y_pos + rooms[i][1]; y++){//Placing the room
                        for(x = room_x_pos; x < room_x_pos + rooms[i][0]; x++){
                            dungeon->hardness[y][x] = 0;
                            int staircase = (rand() % (100)) + 1; //Gets a number from 1-100
                            int up_or_down = (rand() % 10) + 1; //Gets a random number 1-10;
                            if(!player_placed){
                                dungeon->dmap[y][x] = PLAYER; //Place a player in the first room
                                dungeon->player.x_pos = x;
                                dungeon->player.y_pos = y;
                                player_placed = 1;

                            }
                            else{
                                if(staircase >= 1 && staircase <= 3){ //If random number is 1-3, place a staircase
                                    if(up_or_down > 5 && num_stairs_placed[0] < dungeon->num_up){ //If the number is 6-10 and the max up staircases have not been placed place an up staircase
                                        dungeon->dmap[y][x] = UP;
                                        UpStairs up(x, y);
                                        dungeon->up_stairs[num_stairs_placed[0]++] = up;
                                    }
                                    else if(up_or_down <= 5 && num_stairs_placed[1] < dungeon->num_down){ //If the number is 1-5 and the max down staircases have not been placed place a down staircase
                                        dungeon->dmap[y][x] = DOWN;
                                        DownStairs down(x, y);
                                        dungeon->down_stairs[num_stairs_placed[1]++] = down;
                                    }
                                    else{ //Otherwise, make it a room
                                        dungeon->dmap[y][x] = ROOM;
                                    }
                                }
                                else {
                                    dungeon->dmap[y][x] = ROOM;
                                }
                            }

                        }
                    }
                    num_rooms_placed++;
                }
            }
        }
        num_cycles++;
    }

    for(i = 1; i <= dungeon->num_rooms; i++){
        //room_array[i] = src
        //room_array[j] = dest
//            void shortest_path(Dungeon *dungeon, int src_x, int src_y, int dest_x, int dest_y, int num_stairs_placed[]);
        if(i == dungeon->num_rooms){
            shortest_path(dungeon, dungeon->rooms[i - 1].x_pos, dungeon->rooms[i - 1].y_pos, dungeon->rooms[0].x_pos, dungeon->rooms[0].y_pos, num_stairs_placed);
            break;
        }
        shortest_path(dungeon, dungeon->rooms[i].x_pos, dungeon->rooms[i].y_pos, dungeon->rooms[j].x_pos, dungeon->rooms[j].y_pos, num_stairs_placed); //Make a path from one room to the next
        j = i;
    }

    if(save){ //If we are saving, write this dungeon to disk
        save_game(dungeon, num_stairs_placed);
    }
    return;
}

void fill_dungeon(Dungeon *dungeon){
    int j, i;
    for(j = 0; j < MAP_Y_MAX; j++){ //Fill the dungeon with ROCKs and line borders with | or -. Make border hardness 255 and other hardness between 1-254
        for(i = 0; i < MAP_X_MAX; i++){
            if (i == 0 || i == MAP_X_MAX - 1) {
                dungeon->hardness[j][i] = HARDNESS_MAX;
                dungeon->dmap[j][i] = SIDE;
                dungeon->fmap[j][i] = SIDE;
            }
            else if (j == 0 || j == MAP_Y_MAX - 1) {
                dungeon->hardness[j][i] = HARDNESS_MAX;
                dungeon->dmap[j][i] = TOP;
                dungeon->fmap[j][i] = TOP;
            }
            else{
                dungeon->dmap[j][i] = ROCK;
                dungeon->fmap[j][i] = ROCK;
                dungeon->hardness[j][i] = (rand() % (254 - 1)) + 1;
            }
        }
    }
}

Room newRoom(int x_pos, int y_pos, int x_size, int y_size){ //Returns a new room struct with the rooms position and size
    Room new_room;
    new_room.x_pos = x_pos;
    new_room.y_pos = y_pos;
    new_room.x_size = x_size;
    new_room.y_size = y_size;
    return new_room;
}

int is_room(Dungeon dungeon, int room_x_pos, int room_y_pos, int rooms[][2], int i){ //Checks to see if a new room will collide with an existing room
    int x, y;
    for(y = room_y_pos - 1; y < room_y_pos + rooms[i][1] + 1; y++){
        for(x = room_x_pos - 1; x < room_x_pos + rooms[i][0] + 1; x++){
            if(dungeon.dmap[y][x] == ROOM || dungeon.dmap[y][x] == PLAYER) return 1;
        }
    }
    return 0;
}

void shortest_path(Dungeon *dungeon, int src_x, int src_y, int dest_x, int dest_y, int num_stairs_placed[]){ //Start of the shortest path print for the corridor
    int i;
    int visited[MAP_X_MAX];
    for(i = 0; i < MAP_X_MAX; i++){ //Set visited array to all false
        visited[i] = 0;
    }

    modified_dfs(dungeon, src_x, src_y, dest_x, dest_y, visited, num_stairs_placed);
}

/*
Modified version of a Depth First Search to get the shortest path from one room to the next
*/
void modified_dfs(Dungeon *dungeon, int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_stairs_placed[]){
    int i;
    visited[src_x] = 1; //Set this coordinate to visited
    int all_up_placed = num_stairs_placed[0] == dungeon->num_up ? 1 : 0; //Check if all up staircases have been placed
    int all_down_placed = num_stairs_placed[1] == dungeon->num_down ? 1 : 0; //Check if all down staircases have been placed
    int staircase = (rand() % 100) + 1;
    if(src_x - dest_x < 0){//Destination is to the right
        for(i = src_x; i <= dest_x; i++){
            if(!visited[i]){
                dungeon->dmap[src_y][i] = dungeon->dmap[src_y][i] == PLAYER || dungeon->dmap[src_y][i] == ROOM || dungeon->dmap[src_y][i] == DOWN || dungeon->dmap[src_y][i] == UP ? dungeon->dmap[src_y][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                dungeon->hardness[src_y][i] = 0; //Set dungeon->hardness to 0 since it is a room, corridor, or staircase
                if(dungeon->dmap[src_y][i] == CORRIDOR && staircase <= 3){ //Checking if a staircase should be placed
                    int up_or_down = (rand() % 10) + 1;
                    if(!all_down_placed && up_or_down <= 5){ //Make a down staircase if not all have been placed
                        dungeon->dmap[src_y][i] = DOWN;
                        DownStairs down(src_x, src_y);
//                        down.x_pos = src_x;
//                        down.y_pos = src_y;
                        dungeon->down_stairs[num_stairs_placed[1]++] = down;
                    }
                    else if(!all_up_placed && up_or_down > 5){ //Make an up staircase if not all have been placed
                        dungeon->dmap[src_y][i] = UP;
                        UpStairs up(src_x, src_y);
//                    up.x_pos = src_x;
//                    up.y_pos = src_y;
                        dungeon->up_stairs[num_stairs_placed[0]++] = up;
                    }
                }
                int src_to_dest = src_y - dest_y;
                int up_or_down = (src_to_dest < 0) ? 1 : (src_to_dest > 0) ? -1 : 0;
                dungeon->dmap[src_y + up_or_down][i] = dungeon->dmap[src_y + up_or_down][i] == PLAYER || dungeon->dmap[src_y + up_or_down][i] == ROOM || dungeon->dmap[src_y + up_or_down][i] == DOWN || dungeon->dmap[src_y + up_or_down][i] == UP ? dungeon->dmap[src_y + up_or_down][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                dungeon->hardness[src_y + up_or_down][i] = 0;
                modified_dfs(dungeon, i, src_y + up_or_down, dest_x, dest_y, visited, num_stairs_placed);
            }
        }
    }
    else if(src_x - dest_x > 0){ //Destination is to the left
        for(i = src_x; i >= dest_x; i--){
            if(!visited[i]){
                dungeon->dmap[src_y][i] = dungeon->dmap[src_y][i] == PLAYER ||  dungeon->dmap[src_y][i] == ROOM || dungeon->dmap[src_y][i] == DOWN || dungeon->dmap[src_y][i] == UP ? dungeon->dmap[src_y][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                dungeon->hardness[src_y][i] = 0;
                if(dungeon->dmap[src_y][i] == CORRIDOR && staircase <= 3){
                    int up_or_down = (rand() % 10) + 1;
                    if(!all_down_placed && up_or_down <= 5){
                        dungeon->dmap[src_y][i] = DOWN;
                        DownStairs down(src_x, src_y);
//                        down.x_pos = src_x;
//                        down.y_pos = src_y;
                        dungeon->down_stairs[num_stairs_placed[1]++] = down;
                    }
                    else if(!all_up_placed && up_or_down > 5){
                        dungeon->dmap[src_y][i] = UP;
                        UpStairs up(src_x, src_y);
//                    up.x_pos = src_x;
//                    up.y_pos = src_y;
                        dungeon->up_stairs[num_stairs_placed[0]++] = up;
                    }
                }
                int src_to_dest = src_y - dest_y;
                int up_or_down = (src_to_dest < 0) ? 1 : (src_to_dest > 0) ? -1 : 0;
                dungeon->dmap[src_y + up_or_down][i] = dungeon->dmap[src_y + up_or_down][i] == PLAYER ||  dungeon->dmap[src_y + up_or_down][i] == ROOM || dungeon->dmap[src_y + up_or_down][i] == DOWN || dungeon->dmap[src_y + up_or_down][i] == UP ? dungeon->dmap[src_y + up_or_down][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                dungeon->hardness[src_y + up_or_down][i] = 0;
                modified_dfs(dungeon, i, src_y + up_or_down, dest_x, dest_y, visited, num_stairs_placed);
            }
        }
    }
    else if(src_y - dest_y > 0){ //Destination is Up
        for(i = src_y + 1; i >= dest_y; i--){
            dungeon->dmap[i][src_x] = (dungeon->dmap[i][src_x] == PLAYER ||  dungeon->dmap[i][src_x] == ROOM || dungeon->dmap[i][src_x] == DOWN || dungeon->dmap[i][src_x] == UP) ? dungeon->dmap[i][src_x] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
            dungeon->hardness[i][src_x] = 0;
            if(dungeon->dmap[i][src_x] == CORRIDOR && staircase <= 3){
                int up_or_down = (rand() % 10) + 1;
                if(!all_down_placed && up_or_down <= 5){
                    dungeon->dmap[i][src_x] = DOWN;
                    DownStairs down(src_x, src_y);
//                        down.x_pos = src_x;
//                        down.y_pos = src_y;
                    dungeon->down_stairs[num_stairs_placed[1]++] = down;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon->dmap[i][src_x] = UP;
                    UpStairs up(src_x, src_y);
//                    up.x_pos = src_x;
//                    up.y_pos = src_y;;
                    dungeon->up_stairs[num_stairs_placed[0]++] = up;
                }
            }
        }
        return;
    }
    else if(src_y - dest_y < 0){ //Destination is Down
        for(i = src_y + 1; i <= dest_y; i++){
            dungeon->dmap[i][src_x] = (dungeon->dmap[i][src_x] == PLAYER ||  dungeon->dmap[i][src_x] == ROOM || dungeon->dmap[i][src_x] == DOWN || dungeon->dmap[i][src_x] == UP) ? dungeon->dmap[i][src_x] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
            dungeon->hardness[i][src_x] = 0;
            if(dungeon->dmap[i][src_x] == CORRIDOR && staircase <= 3){
                int up_or_down = (rand() % 10) + 1;
                if(!all_down_placed && up_or_down <= 5){
                    dungeon->dmap[i][src_x] = DOWN;
                    DownStairs down(src_x, src_y);
//                        down.x_pos = src_x;
//                        down.y_pos = src_y;
                    dungeon->down_stairs[num_stairs_placed[1]++] = down;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon->dmap[i][src_x] = UP;
                    UpStairs up(src_x, src_y);
//                    up.x_pos = src_x;
//                    up.y_pos = src_y;
                    dungeon->up_stairs[num_stairs_placed[0]++] = up;
                }
            }
        }
        return;
    }
    else return;
}

void print_dungeon(Dungeon *dungeon){
    int j, i;
    for (j = 0; j < MAP_Y_MAX; j++) {
        for (i = 0; i < MAP_X_MAX; i++) {
            mvaddch(j + 1, i, dungeon->fow ? dungeon->fmap[j][i] : dungeon->dmap[j][i]);//printf("%1c", dungeon[j][i]);
            fputc(dungeon->dmap[j][i], dungeon->file);
        }
        fputc('\n', dungeon->file);
    }
    mvprintw(j + 1, 0, "There are %d monsters, %d of which are alive      ", dungeon->num_mons, get_num_alive_monsters(dungeon));
    mvprintw(j + 2, 0, "PC Location is (%d, %d)        ", dungeon->player.x_pos, dungeon->player.y_pos);
    fputc('\n', dungeon->file);
    fputc('\n', dungeon->file);
}

//Save and load functions
void save_game(Dungeon *dungeon, int num_stairs_placed[]){
    int i;
    char *home = getenv("HOME");
    const char *game_dir = ".rlg327";
    const char *game_file = "dungeon";
    char *path = (char*) malloc((strlen(home) + strlen(game_dir) + strlen(game_file) + 2 + 1) * sizeof(char));
    sprintf(path, "%s/%s/%s", home, game_dir, game_file);
    int num_up = num_stairs_placed[0];
    int num_down = num_stairs_placed[1];

    FILE *file;
    const char header[13] = "RLG327-S2021";
    int version = 0;
    int size = 1708 + (num_up * 2) + (num_down * 2) + (dungeon->num_rooms * 4); //1708 is the size of all bytes besides number of staircases and number of dungeon->rooms
    if(!(file = fopen(path, "w"))){
        std::cout << "Failed to open this file\n";
    }

    fwrite(&header, sizeof(header), 1, file);
    version = htobe32(version);
    fwrite(&version, 4, 1, file);

    size = htobe32(size);
    fwrite(&size, 4, 1, file);

    fwrite(&dungeon->player.x_pos, 1, 1, file);
    fwrite(&dungeon->player.y_pos, 1, 1, file);

    fwrite(dungeon->hardness, 1, 1680, file); //DON'T DO AN & FOR THIS IT WILL BREAK THE PROGRAM
    dungeon->num_rooms = htobe16(dungeon->num_rooms);
    fwrite(&dungeon->num_rooms, 2, 1, file);
    dungeon->num_rooms = be16toh(dungeon->num_rooms);

    for(i = 0; i < dungeon->num_rooms; i++){
        fwrite(&dungeon->rooms[i].x_pos, sizeof(dungeon->rooms[i].x_pos), 1, file);
        fwrite(&dungeon->rooms[i].y_pos, sizeof(dungeon->rooms[i].y_pos), 1, file);
        fwrite(&dungeon->rooms[i].x_size, sizeof(dungeon->rooms[i].x_size), 1, file);
        fwrite(&dungeon->rooms[i].y_size, sizeof(dungeon->rooms[i].y_size), 1, file);
    }

    num_up = htobe16(num_up);
    fwrite(&num_up, 2, 1, file);

    for(i = 0; i < num_stairs_placed[0]; i++){
        fwrite(&dungeon->up_stairs[i].x_pos, sizeof(dungeon->up_stairs[i].x_pos), 1, file);
        fwrite(&dungeon->up_stairs[i].y_pos, sizeof(dungeon->up_stairs[i].y_pos), 1, file);
    }

    num_down = htobe16(num_down);
    fwrite(&num_down, 2, 1, file);

    for(i = 0; i < num_stairs_placed[1]; i++){
        fwrite(&dungeon->down_stairs[i].x_pos, sizeof(dungeon->down_stairs[i].x_pos), 1, file);
        fwrite(&dungeon->down_stairs[i].y_pos, sizeof(dungeon->down_stairs[i].y_pos), 1, file);
    }
    free(path);
    fclose(file);
}

void load_game(Dungeon *dungeon, int num_stairs_placed[2]){
    FILE *file;
    int i, version, size;
    char header[13];

    char *home = getenv("HOME");
    const char *game_dir = ".rlg327";
    const char *game_file = DUNGEON; //WELL_DONE HELLO DUNGEON ADVENTURE "00.rlg327";

    char *path = (char*) malloc((strlen(home) + strlen(game_dir) + strlen(game_file) + 2 + 1) * sizeof(char));
    sprintf(path, "%s/%s/%s", home, game_dir, game_file);

    if(!(file = fopen(path, "r"))){
        std::cout << "Failed to open this file\n";
    }
    fread(&header, sizeof(header), 1, file);
    fread(&version, 4, 1, file);
    version = be32toh(version);

    fread(&size, 4, 1, file);
    size = be32toh(size);

    fread(&dungeon->player.x_pos, 1, 1, file);
    fread(&dungeon->player.y_pos, 1, 1, file);

    fread(dungeon->hardness, 1, 1680, file);
    fread(&dungeon->num_rooms, 2, 1, file);
    dungeon->num_rooms = be16toh(dungeon->num_rooms);

    dungeon->rooms = (Room*) malloc(dungeon->num_rooms * sizeof(Room));

    for(i = 0; i < dungeon->num_rooms; i++){
        fread(&dungeon->rooms[i].x_pos, sizeof(dungeon->rooms[i].x_pos), 1, file);
        fread(&dungeon->rooms[i].y_pos, sizeof(dungeon->rooms[i].y_pos), 1, file);
        fread(&dungeon->rooms[i].x_size, sizeof(dungeon->rooms[i].x_size), 1, file);
        fread(&dungeon->rooms[i].y_size, sizeof(dungeon->rooms[i].y_size), 1, file);
    }

    fread(&num_stairs_placed[0], 2, 1, file);
    num_stairs_placed[0] = be16toh(num_stairs_placed[0]);
    dungeon->up_stairs = (UpStairs*) malloc(num_stairs_placed[0] * sizeof(UpStairs));

    for(i = 0; i < num_stairs_placed[0]; i++){
        fread(&dungeon->up_stairs[i].x_pos, sizeof(dungeon->up_stairs[i].x_pos), 1, file);
        fread(&dungeon->up_stairs[i].y_pos, sizeof(dungeon->up_stairs[i].y_pos), 1, file);
    }

    fread(&num_stairs_placed[1], 2, 1, file);
    num_stairs_placed[1] = be16toh(num_stairs_placed[1]);
    dungeon->down_stairs = (DownStairs*) malloc(num_stairs_placed[1] * sizeof(DownStairs));

    for(i = 0; i < num_stairs_placed[1]; i++){
        fread(&dungeon->down_stairs[i].x_pos, sizeof(dungeon->down_stairs[i].x_pos), 1, file);
        fread(&dungeon->down_stairs[i].y_pos, sizeof(dungeon->down_stairs[i].y_pos), 1, file);
    }
    fclose(file);
    free(path);
    make_dungeon(dungeon);
}

void make_dungeon(Dungeon *dungeon){
    int j, i, index;
    for(j = 0; j < MAP_Y_MAX; j++){
        for(i = 0; i < MAP_X_MAX; i++){
            if(dungeon->player.x_pos == i && dungeon->player.y_pos == j){ //If this is the dungeon->player, set the dungeon->player here
                dungeon->dmap[j][i] = PLAYER;
            }
            else if(dungeon->hardness[j][i] == 255){
                dungeon->dmap[j][i] = (j == 0 || j == MAP_Y_MAX - 1) ? TOP : SIDE; //If it is a left/right wall make it |, otherwise -
            }
            else if(dungeon->hardness[j][i] == 0){
                index = find_room(dungeon, i, j); //See if this is a room
                if(index >= 0){ //If so, see if there are any staircases
                    dungeon->dmap[j][i] = find_stairs(dungeon, i, j, 0);
                }
                else{ //If not, it is a corridor, also check for staircases
                    dungeon->dmap[j][i] = find_stairs(dungeon, i, j, 1);
                }
            }
            else{ //Otherwise, print a space
                dungeon->dmap[j][i] = ROCK;
            }
        }
    }
}

int find_room(Dungeon *dungeon, int x, int y){
    int i;
    for(i = 0; i < dungeon->num_rooms; i++){
        //If a room is located between these x and y coordinates return the index of that room array
        if(x >= dungeon->rooms[i].x_pos && x < (dungeon->rooms[i].x_pos + dungeon->rooms[i].x_size) && y >= dungeon->rooms[i].y_pos && (y < dungeon->rooms[i].y_pos + dungeon->rooms[i].y_size)) return i;
    }
    //Otherwise, return -1
    return -1;
}

char find_stairs(Dungeon *dungeon, int x, int y, int is_corridor){
    int i;
    for(i = 0; i < dungeon->num_up; i++){
        if(dungeon->up_stairs[i].x_pos == x && dungeon->up_stairs[i].y_pos == y) return UP; //If this is an up staircase
    }
    for(i = 0; i < dungeon->num_down; i++){
        if(dungeon->down_stairs[i].x_pos == x && dungeon->down_stairs[i].y_pos == y) return DOWN; //If this is a down staircase
    }
    return dungeon->hardness[y][x] ? ROCK : is_corridor ? CORRIDOR : ROOM; //If it is a corridor, return #, otherwise .
}

