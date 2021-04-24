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
#include <cstdlib>
#include <ctime>
#include "heap.c"
#include "dungeon.h"


int main(int argc, char *argv[]){
    srand(time(NULL));
    Dungeon dungeon;
    dungeon.num_rooms = (rand() % 10) + 6; //Generates between 6-15 rooms
    dungeon.num_up = (rand() % 5) + 1; //Will place 1-5 up staircases
    dungeon.num_down = (rand() % 5) + 1; //Will place 1-5 down stair cases
    dungeon.num_mons = (rand() % 29) + 2; //Will place 2-30 monsters
    dungeon.num_items = (rand() % 11) + 10;
    dungeon.up_stairs = (UpStairs*) (malloc(dungeon.num_up * sizeof(UpStairs)));
    dungeon.down_stairs = (DownStairs*) (malloc(dungeon.num_down * sizeof(DownStairs)));
    dungeon.rooms = (Room*) (malloc(dungeon.num_rooms * sizeof(Room)));
    dungeon.player.alive = 1;
    dungeon.player.speed = 10;
    int num_stairs_placed[2] = {0, 0};
    int init = 0;
    int num_characters = dungeon.num_mons + 1;
    dungeon.quit = 0;
    dungeon.del = 0;
    dungeon.fow = 1;
    parse_descriptions(&dungeon);
    dungeon.boss_killed = 0;
    dungeon.player.hp = 1000;
    dungeon.player.damage.base = 0;
    dungeon.player.damage.number = 1;
    dungeon.player.damage.sides = 4;
    
    dungeon.player.is_infected = 0;
    dungeon.player.infect_amount = 0;

    if(!(dungeon.file = fopen("./output.txt", "w"))){
        std::cout << "Failed to open this file\n";
        return -1;
    }
    if(argc == 1){ //This means that it was just normally ran and dungeon will not be saved
        create_monsters(&dungeon);
        fill_dungeon(&dungeon);
        create_dungeon_map(&dungeon, num_stairs_placed, 0);
        do_maps(&dungeon);
        place_monsters(&dungeon);
        create_items(&dungeon);
        place_items(&dungeon);
    }
    else if(argc == 2){
        if(!strcmp(argv[1], "--save")){ //This means the dungeon will be generated and saved;
            create_monsters(&dungeon);
            fill_dungeon(&dungeon);
            create_dungeon_map(&dungeon, num_stairs_placed, 1);
            place_monsters(&dungeon);
            create_items(&dungeon);
            place_items(&dungeon);
            do_maps(&dungeon);
        }
        else if(!strcmp(argv[1], "--load")){ //This will load a dungeon
//        void load_game(Dungeon *dungeon, int num_stairs_placed[2], Room room_array[]);
            load_game(&dungeon, num_stairs_placed);
            create_monsters(&dungeon);
            place_monsters(&dungeon);
            create_items(&dungeon);
            place_items(&dungeon);
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
            num_characters = dungeon.num_mons + 1;
            create_monsters(&dungeon);
            place_monsters(&dungeon);
            create_items(&dungeon);
            place_items(&dungeon);
            do_maps(&dungeon);
        }
        else if(!strcmp(argv[1], "--nummon")){ //This will load a dungeon
            dungeon.num_mons = (uint32_t) atoi(argv[2]);
            
            if(dungeon.num_mons == 1){
                printf("Must specify more than 1 monster\n");
                return 1;
            }
            num_characters = dungeon.num_mons + 1;
            create_monsters(&dungeon);
            fill_dungeon(&dungeon);
            create_dungeon_map(&dungeon, num_stairs_placed, 0);
            place_monsters(&dungeon);
            create_items(&dungeon);
            place_items(&dungeon);
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
            dungeon.num_mons = (uint32_t) atoi(argv[4]);
            
            if(dungeon.num_mons == 1){
                std::cout << "Must specify more than 1 monster\n";
                return 1;
            }
            num_characters = dungeon.num_mons + 1;
            create_monsters(&dungeon);
            create_items(&dungeon);
            place_monsters(&dungeon);
            place_items(&dungeon);
            do_maps(&dungeon);
        }
        else if((!strcmp(argv[1], "--load") && !strcmp(argv[2], "--nummon") && !strcmp(argv[4], "--save")) || (!strcmp(argv[1], "--save") && !strcmp(argv[2], "--nummon") && !strcmp(argv[4], "--load")) ){
            load_game(&dungeon, num_stairs_placed);
            save_game(&dungeon, num_stairs_placed);
            dungeon.num_mons = (uint32_t) atoi(argv[3]);
            
            if(dungeon.num_mons == 1){
                std::cout << "Must specify more than 1 monster\n";
                return 1;
            }
            num_characters = dungeon.num_mons + 1;
            create_monsters(&dungeon);
            create_items(&dungeon);
            place_monsters(&dungeon);
            place_items(&dungeon);
            do_maps(&dungeon);
        }
        else if((!strcmp(argv[1], "--nummon") && !strcmp(argv[3], "--load") && !strcmp(argv[4], "--save")) || (!strcmp(argv[1], "--nummon") && !strcmp(argv[3], "--save") && !strcmp(argv[4], "--load"))){
            load_game(&dungeon, num_stairs_placed);
            save_game(&dungeon, num_stairs_placed);
            dungeon.num_mons = (uint32_t) atoi(argv[2]);
            
            if(dungeon.num_mons == 1){
                std::cout << "Must specify more than 1 monster\n";
                return 1;
            }
            num_characters = dungeon.num_mons + 1;
            create_monsters(&dungeon);
            create_items(&dungeon);
            place_monsters(&dungeon);
            place_items(&dungeon);
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
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    clear();
    refresh();
    add_cells_to_fow(&dungeon);
    int ij;
    for(ij = 0; ij < 10; ij++){
        dungeon.player.carrying[ij] = get_blank_item(dungeon, 1);
    }
    for(ij = 0; ij < 12; ij++){
        dungeon.player.equipped[ij] = get_blank_item(dungeon, 1);
    }
    
    try{
        while(turn_decider(&dungeon, &init, num_characters)){
            if(dungeon.del){
                dungeon.del = 0;
                delete_dungeon(&dungeon, &init, &num_characters);
            }

            fflush(stdout);
            usleep(USLEEP_MAX/FPS);
        }
        endwin();
        fclose(dungeon.file);
        std::cout << (((dungeon.player.alive && !dungeon.quit) || dungeon.boss_killed) ? victory : dungeon.quit ? quit : tombstone);
        return 0;
    }
    catch(std::exception& e){
        endwin();
        std::cout << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
}

Item get_blank_item(Dungeon dungeon, int blank){
    Item blank_item;
    if(blank){
        blank_item.rrty = 1000;
        blank_item.speed = 0;
        blank_item.damage_bonus.sides = 1;
        blank_item.damage_bonus.number = 0;
        blank_item.damage_bonus.base = 0;
    }
    else{
        int rand_num = rand() % dungeon.descriptions.item_list.size();
        blank_item = dungeon.descriptions.item_list[rand_num];
    }
    return blank_item;
}

void reset_monster(Monster *monster){
    monster->x_pos = -1;
    monster->y_pos = -1;
    monster->alive = 1;
    monster->speed = 0;
    monster->type = 0;
    monster->rep = ' ';
    monster->map_made = 0;
    if(!(monster->name.compare(""))){
        monster->name.clear();
    }
    if(!(monster->desc.compare(""))){
        monster->desc.clear();
    }
    monster->Speed.base = -1;
    monster->Speed.number = -1;
    monster->Speed.sides = -1;
    monster->Health.number = -1;
    monster->Health.base = -1;
    monster->Health.sides = -1;
    monster->Damage.base = -1;
    monster->Damage.sides = -1;
    monster->Damage.number = -1;
    monster->rrty = -1;
}

void reset_item(Item *item){
    item->x_pos = -1;
    item->y_pos = -1;
    if(!(item->name.compare(""))){
        item->name.clear();
    }
    if(!(item->desc.compare(""))){
        item->desc.clear();
    }
    if(!(item->type.compare(""))){
        item->type.clear();
    }
    item->color = -1;
    item->hit_bonus.base = -1;
    item->hit_bonus.number = -1;
    item->hit_bonus.sides =-1;
    item->damage_bonus.base = -1;
    item->damage_bonus.number = -1;
    item->damage_bonus.sides =-1;
    item->dodge_bonus.base = -1;
    item->dodge_bonus.number = -1;
    item->dodge_bonus.sides =-1;
    item->speed_bonus.base = -1;
    item->speed_bonus.number = -1;
    item->speed_bonus.sides =-1;
    item->spec_attr.base = -1;
    item->spec_attr.number = -1;
    item->spec_attr.sides =-1;
    item->value.base = -1;
    item->value.number = -1;
    item->value.sides =-1;
    item->weight.base = -1;
    item->weight.number = -1;
    item->weight.sides =-1;
    item->defense_bonus.base = -1;
    item->defense_bonus.number = -1;
    item->defense_bonus.sides =-1;
    item->artifact = -1;
    item->rrty = -1;
}

void parse_descriptions(Dungeon *dungeon){
    int i, j;
    std::string path = getenv("HOME");
    path += "/.rlg327/monster_desc.txt";

    std::ifstream file(path);
    std::string read;

    getline(file, read);

    if(read != "RLG327 MONSTER DESCRIPTION 1"){
        std::cout << "File header is invalid." << std::endl;
        exit(1);
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

                        while(getline(all_colors, read, ' ')){
                            for(j = 0; j < 8; j++){
                                if(read == colors[j]){
                                    mon.color.push_back(j + 1);
                                }
                            }
                        }
                    }
                    else if(read == "SPEED"){
                        file >> read;
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
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                mon.Health.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                mon.Health.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                mon.Health.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "DAM"){
                        file >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                mon.Damage.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                mon.Damage.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                mon.Damage.sides = std::stoi(read.substr(i + 1, std::string::npos));
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
                        uint16_t abil = 0b0000000000000000;

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
                            else if(read == std::string("BOSS")){
                                abil |= BOSS;
                            }
                            else if(read == std::string("INFECT")){
                                abil |= INFECT;
                            }
                            else{
                                std::cout << "Invalid Monster Ability: " << read << std::endl;
                                exit(1);
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
                        exit(1);
                    }
                }
                dungeon->descriptions.monster_list.push_back(mon);
            }
        }

        file.close();
    }

    std::string path2 = getenv("HOME");
    path2 += "/.rlg327/object_desc.txt";

    std::ifstream file2(path2);

    getline(file2, read);

    if(read != "RLG327 OBJECT DESCRIPTION 1"){
        std::cout << "file2 header is invalid." << std::endl;
        exit(1);
    }

    else{
        while(!file2.eof()) {
            getline(file2, read);
            if (read == "BEGIN OBJECT") {
                Item item;
                while (read != "END") {
                    file2 >> read;
                    if (read == "NAME") {
                        getline(file2, read);
                        item.name = read.substr(1, std::string::npos);
                    }
                    else if (read == "DESC") {
                        getline(file2, read);
                        while (read != ".") {
                            item.desc += read + "\n";
                            getline(file2, read);
                        }
                    }
                    else if(read == "TYPE"){
                        file2 >> read;
                        item.type = read;
                        if(item.type == "WEAPON"){
                            item.rep = '|';
                        }
                        else if(item.type == "OFFHAND"){
                            item.rep = ')';
                        }
                        else if(item.type == "RANGED"){
                            item.rep = '}';
                        }
                        else if(item.type == "ARMOR"){
                            item.rep = '[';
                        }
                        else if(item.type == "HELMET"){
                            item.rep = ']';
                        }
                        else if(item.type == "CLOAK"){
                            item.rep = '(';
                        }
                        else if(item.type == "GLOVES"){
                            item.rep = '{';
                        }
                        else if(item.type == "BOOTS"){
                            item.rep = '\\';
                        }
                        else if(item.type == "RING"){
                            item.rep = '=';
                        }
                        else if(item.type == "AMULET"){
                            item.rep = '"';
                        }
                        else if(item.type == "LIGHT"){
                            item.rep = '_';
                        }
                        else if(item.type == "SCROLL"){
                            item.rep = '~';
                        }
                        else if(item.type == "BOOK"){
                            item.rep = '?';
                        }
                        else if(item.type == "FLASK"){
                            item.rep = '!';
                        }
                        else if(item.type == "GOLD"){
                            item.rep = '$';
                        }
                        else if(item.type == "AMMUNITION"){
                            item.rep = '/';
                        }
                        else if(item.type == "FOOD"){
                            item.rep = ',';
                        }
                        else if(item.type == "WAND"){
                            item.rep = '-';
                        }
                        else if(item.type == "CONTAINER"){
                            item.rep = '%';
                        }
                        else if(item.type == "STACK"){
                            item.rep = '&';
                        }
                        else{
                            item.rep = '*';
                        }
                    }
                    else if(read == "COLOR"){
                        file2 >> read;
                        for(j = 0; j < 8; j++){
                            if(read == colors[j]){
                                item.color = j;
                                break;
                            }
                        }
                    }
                    else if(read == "HIT"){
                        file2 >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                item.hit_bonus.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                item.hit_bonus.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                item.hit_bonus.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "DAM"){
                        file2 >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                item.damage_bonus.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                item.damage_bonus.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                item.damage_bonus.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "DODGE"){
                        file2 >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                item.dodge_bonus.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                item.dodge_bonus.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                item.dodge_bonus.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "DEF"){
                        file2 >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                item.defense_bonus.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                item.defense_bonus.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                item.defense_bonus.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "WEIGHT"){
                        file2 >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                item.weight.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                item.weight.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                item.weight.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "SPEED"){
                        file2 >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                item.speed_bonus.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                item.speed_bonus.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                item.speed_bonus.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "ATTR"){
                        file2 >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                item.spec_attr.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                item.spec_attr.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                item.spec_attr.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "VAL"){
                        file2 >> read;
                        std::string num = "";
                        for(i = 0; i < (int) read.length(); i++){
                            if(read[i] == '+'){
                                item.value.base = std::stoi(num);
                                num = "";
                            }
                            else if(read[i] == 'd'){
                                item.value.number = std::stoi(num);
                                num = "";
                                num += read[i + 1];
                                item.value.sides = std::stoi(read.substr(i + 1, std::string::npos));
                                break;
                            }
                            else{
                                num += read[i];
                            }
                        }
                    }
                    else if(read == "ART"){
                        file2 >> read;
                        item.artifact = (read == "FALSE") ? 0 : 1;
                    }
                    else if(read == "RRTY"){
                        file2 >> read;
                        item.rrty = std::stoi(read);
                    }
                    else if(read != "END"){
                        std::cout << "Invalid Item Attribute: " << read << std::endl;
                        exit(1);
                    }
                }
                dungeon->descriptions.item_list.push_back(item);
            }
        }
        file2.close();
    }
}

void delete_dungeon(Dungeon *dungeon, int *init, int *num_characters){
    free(dungeon->up_stairs);
    free(dungeon->down_stairs);
    free(dungeon->rooms);
    dungeon->turn.clear();
    dungeon->items.clear();
    dungeon->mons.clear();

    dungeon->num_rooms = (rand() % 10) + 6; //Generates between 6-15 rooms
    dungeon->num_up = (rand() % 5) + 1; //Will place 1-5 up staircases
    dungeon->num_down = (rand() % 5) + 1;
    dungeon->up_stairs = (UpStairs*) (malloc(dungeon->num_up * sizeof(UpStairs)));
    dungeon->down_stairs = (DownStairs*) (malloc(dungeon->num_down * sizeof(DownStairs)));
    dungeon->rooms = (Room*) (malloc(dungeon->num_rooms * sizeof(Room)));

    int num_stairs_placed[2] = {0, 0};
    *init = 0;
    *num_characters = dungeon->num_mons + 1;

    dungeon->total_monsters_faced += dungeon->num_mons;
    create_monsters(dungeon);
    fill_dungeon(dungeon);
    create_dungeon_map(dungeon, num_stairs_placed, 0);
    place_monsters(dungeon);
    create_items(dungeon);
    place_items(dungeon);
    do_maps(dungeon);
    add_cells_to_fow(dungeon);
}

//Moving Functions
int seen(int mons[2]){
    return mons[0] != -1 && mons[1] != -1;
}

static int turn_decider(Dungeon *dungeon, int *init, int num_characters){
    heap_t heap;
    int i;
    Turn *t;
    int move, tport = 0;
    int next_pos[2];

    if(!*init){
        Turn turn_event;
        for(i = 0; i < num_characters; i++){
            if(!i){
                turn_event.seq = 0;
                turn_event.next_turn = 0;
                turn_event.symb = PLAYER;
                turn_event.speed = dungeon->player.speed;
            }
            else{
                turn_event.seq = i;
                turn_event.next_turn = 0;
                turn_event.symb = dungeon->mons[i - 1].rep;
                turn_event.speed = dungeon->mons[i - 1].speed;
            }
            dungeon->turn.push_back(turn_event);
        }
        *init = 1;
    }

    heap_init(&heap, turn_cmp, NULL);
    for(i = 0; i < num_characters; i++){
        dungeon->turn[i].heap_node = (!i || dungeon->mons[i - 1].alive) ? heap_insert(&heap, &dungeon->turn[i]) : NULL;
    }

    while(((t = (Turn*) heap_remove_min(&heap))) && ((dungeon->player.alive && get_num_alive_monsters(dungeon)) || (!get_num_alive_monsters(dungeon) && !dungeon->boss_killed && dungeon->player.alive))){
        t->next_turn = t->next_turn + floor((double)(1000/t->speed));
        refresh();
        if(!t->seq){
            print_dungeon(dungeon);
            refresh();
            if(dungeon->player.is_infected){
                attron(COLOR_PAIR(GREEN));
                mvprintw(0, 0, "%s", spaces);
                mvprintw(0, 0, "You are infected! Find a vaccine before you die");
                attroff(COLOR_PAIR(GREEN));
                refresh();
            }
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
                }
                else if(move == 'i'){
                    print_inventory(dungeon);
                    getch();
                    print_dungeon(dungeon);
                    refresh();
                }
                else if(move == 'e'){
                    print_equipped(dungeon);
                    getch();
                    print_dungeon(dungeon);
                    refresh();
                }
                else if(move == 'I'){
                    clear_dungeon(dungeon);
                    print_inventory(dungeon);
                    int inspected = 0;
                    while(!inspected){
                        int item_to_display = getch() - '0';
                        if(item_to_display < 10 && item_to_display >= 0){
                            clear_dungeon(dungeon);
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "You selected the item in slot %d of your inventory", item_to_display);
                            mvprintw(2, 0, "This is the description for %s:", dungeon->player.carrying[item_to_display].name.c_str());
                            mvprintw(3, 0, "%s", dungeon->player.carrying[item_to_display].desc.c_str());
                            mvprintw(15, 0, "Hit any key to return to the map");
                            refresh();
                            item_to_display = getch();
                            clear_dungeon(dungeon);
                            print_dungeon(dungeon);
                            inspected = 1;
                        }
                        else{
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "Invalid slot number. Please enter a number between 0-9", item_to_display);
                        }
                    }
                }
                else if(move == 'w'){
                    clear_dungeon(dungeon);
                    print_inventory(dungeon);
                    mvprintw(0, 0, "%s", spaces);
                    mvprintw(0, 0, "Select an item to wear. Press Escape to return");
                    mvprintw(13, 0, spaces);
                    refresh();
                    int swapped = 0;
                    int item_to_wear, input;
                    do{
                        input = getch();
                        item_to_wear = input - '0';
                        refresh();
                        if(item_to_wear < 10 && item_to_wear >= 0){
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "You selected to wear the item in slot %d", item_to_wear);
                            refresh();
                            Item temp_item = dungeon->player.carrying[item_to_wear];
                            if(temp_item.type == "WEAPON"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[0];
                                dungeon->player.equipped[0] = temp_item;
                            }
                            else if(temp_item.type == "OFFHAND"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[1];
                                dungeon->player.equipped[1] = temp_item;
                            }
                            else if(temp_item.type == "RANGED"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[2];
                                dungeon->player.equipped[2] = temp_item;
                            }
                            else if(temp_item.type == "LIGHT"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[3];
                                dungeon->player.equipped[3] = temp_item;
                            }
                            else if(temp_item.type == "ARMOR"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[4];
                                dungeon->player.equipped[4] = temp_item;
                            }
                            else if(temp_item.type == "HELMET"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[5];
                                dungeon->player.equipped[5] = temp_item;
                            }
                            else if(temp_item.type == "CLOAK"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[6];
                                dungeon->player.equipped[6] = temp_item;
                            }
                            else if(temp_item.type == "GLOVES"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[7];
                                dungeon->player.equipped[7] = temp_item;
                            }
                            else if(temp_item.type == "BOOTS"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[8];
                                dungeon->player.equipped[8] = temp_item;
                            }
                            else if(temp_item.type == "AMULET"){
                                dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[9];
                                dungeon->player.equipped[9] = temp_item;
                            }
                            else if(temp_item.type == "RING"){
                                if(dungeon->player.equipped[10].rrty == 1000){
                                    dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[10];
                                    dungeon->player.equipped[10] = temp_item;
                                }
                                else if(dungeon->player.equipped[11].rrty == 1000){
                                    dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[11];
                                    dungeon->player.equipped[11] = temp_item;
                                }
                                else{
                                    mvprintw(0, 0, "%s", spaces);
                                    mvprintw(13, 0, "Enter a 0 for your left hand ring or a 1 for right hand ring");
                                    mvprintw(14, 0, "Which would you like to swap?");
                                    mvprintw(16, 0, "Left Hand Ring: %s (sp: %d, dm: %d+%dd%d)", dungeon->player.equipped[10].name.c_str(), dungeon->player.equipped[10].speed, dungeon->player.equipped[10].damage_bonus.base, dungeon->player.equipped[10].damage_bonus.number, dungeon->player.equipped[10].damage_bonus.sides);
                                    mvprintw(17, 0, "Right Hand Ring: %s (sp: %d, dm: %d+%dd%d)", dungeon->player.equipped[11].name.c_str(), dungeon->player.equipped[11].speed, dungeon->player.equipped[11].damage_bonus.base, dungeon->player.equipped[11].damage_bonus.number, dungeon->player.equipped[11].damage_bonus.sides);
                                    refresh();
                                    int index_of_equip;
                                    while(1){
                                        index_of_equip = (getch() - '0') + 10;
                                        if(index_of_equip == 37){
                                            input = ESC;
                                            break;
                                        }
                                        else if(index_of_equip == 10){
                                            dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[10];
                                            dungeon->player.equipped[10] = temp_item;
                                            break;
                                        }
                                        else if(index_of_equip == 11){
                                            dungeon->player.carrying[item_to_wear] = dungeon->player.equipped[11];
                                            dungeon->player.equipped[11] = temp_item;
                                            break;
                                        }
                                        else{
                                            continue;
                                        }
                                    }
                                }
                            }
                            else if(temp_item.type == "FLASK"){
                                if(!dungeon->player.is_infected){
                                    clear_dungeon(dungeon);
                                    attron(COLOR_PAIR(5));
                                    mvprintw(5, 0, "YOU AREN'T INFECTED, DO YOU REALLY WANT TO USE THIS?");
                                    mvprintw(6, 0, "ENTER y for yes, n for no. CASE SENSITIVE");
                                    attroff(COLOR_PAIR(5));
                                    int decision;
                                    while(1){
                                        decision = getch();
                                        if(decision != 'y' && decision != 'n'){
                                            mvprintw(0, 0, "Please Enter y, n. (y's are for yes, n's are no");
                                        }
                                        else{
                                            break;
                                        }
                                    }
                                    if(decision == 'y'){
                                        dungeon->player.carrying[item_to_wear] = get_blank_item(*dungeon, 1);
                                    }
                                }
                                else{
                                    dungeon->player.is_infected = 0;
                                    dungeon->player.carrying[item_to_wear] = get_blank_item(*dungeon, 1);
                                    mvprintw(0, 0, "%s", spaces);
                                    mvprintw(0, 0, "You are cured!");
                                }
                            }
                            else if(temp_item.type == "FOOD"){
                                clear_dungeon(dungeon);
                                attron(COLOR_PAIR(5));
                                mvprintw(5, 0, "ARE YOU SURE YOU NEED THIS, DO YOU REALLY WANT TO USE IT?");
                                mvprintw(6, 0, "ENTER y for yes, n for no. CASE SENSITIVE");
                                attroff(COLOR_PAIR(5));
                                int decision;
                                while(1){
                                    decision = getch();
                                    if(decision != 'y' && decision != 'n'){
                                        mvprintw(0, 0, "Please Enter y, n. (y's are for yes, n's are no");
                                    }
                                    else{
                                        break;
                                    }
                                }
                                if(decision == 'y'){
                                    dungeon->player.turns_immune = roll(dungeon->player.carrying[item_to_wear].hit_bonus);
                                    dungeon->player.carrying[item_to_wear] = get_blank_item(*dungeon, 1);
                                    dungeon->player.immune = 1;
                                    dungeon->player.immune_count = -1;
                                    mvprintw(0, 0, "%s");
                                    mvprintw(0, 0, "You have %d turns left being immune to the Virtual Semester                   ", dungeon->player.turns_immune);
                                    refresh();
                                }
                            }
                            else{
                                mvprintw(0, 0, "%s", spaces);
                                mvprintw(0, 0, "This inventory slot is empty. Try again and choose a non-empty slot");
                                swapped = 1;
                            }
                            swapped = 1;
                        }
                        else{
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "You must enter a number between 0-9");
                            refresh();
                        }
                    }while(!swapped && input != ESC);
                    clear_dungeon(dungeon);
                    print_dungeon(dungeon);
                }
                else if(move == 't'){
                    clear_dungeon(dungeon);
                    print_equipped(dungeon);
                    mvprintw(0, 0, "%s", spaces);
                    mvprintw(0, 0, "Select an item to take off. Press Escape to return");
                    refresh();
                    int taken_off = 0;
                    int item_to_take_off, input;
                    do{
                        input = getch();
                        item_to_take_off = get_equip_index((char) input);
                        if(item_to_take_off < 12){
                            int ij;
                            if(dungeon->player.equipped[item_to_take_off].rrty == 1000){
                                taken_off = 1;
                            }
                            else{
                                for(ij = 0; ij < 10; ij++){
                                    if(dungeon->player.carrying[ij].rrty == 1000){
                                        dungeon->player.carrying[ij] = dungeon->player.equipped[item_to_take_off];
                                        dungeon->player.equipped[item_to_take_off] = get_blank_item(*dungeon, 1);
                                        taken_off = 1;
                                        break;
                                    }
                                }
                                if(!taken_off){
                                    mvprintw(0, 0, "%s", spaces);
                                    mvprintw(0, 0, "Carry inventory full. Please drop an item and try again");
                                    refresh();
                                    taken_off = 1;
                                }
                            }
                        }
                        else{
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "Please try again but enter a valid equipment slot.");
                            taken_off = 1;
                            refresh();
                        }
                    }while(!taken_off && input != ESC);
                    clear_dungeon(dungeon);
                    print_dungeon(dungeon);
                    refresh();
                }
                else if(move == 'd'){
                    clear_dungeon(dungeon);
                    print_inventory(dungeon);
                    mvprintw(0, 0, "%s", spaces);
                    mvprintw(13, 0, "%s", spaces);
                    mvprintw(0, 0, "Select an item to drop. Press Escape to return");
                    refresh();
                    int dropped = 0;
                    int item_to_drop, input;
                    do{
                        input = getch();
                        item_to_drop = input - '0';
                        if(item_to_drop < 10 && item_to_drop >= 0){
                            int ij;
                            for(ij = 0; ij < 8; ij++){
                                int new_x = dungeon->player.x_pos + dirs[ij][1];
                                int new_y = dungeon->player.y_pos + dirs[ij][0];
                                int next_pos[2] = {new_y, new_x};
                                char stairs = find_stairs(dungeon, new_x, new_y, 1);
                                if(!((is_item(dungeon, new_x, new_y) >= 0) || dungeon->hardness[new_y][new_x] || !is_open(dungeon, next_pos) || stairs != CORRIDOR)){
                                    dungeon->dmap[new_y][new_x] = dungeon->player.carrying[item_to_drop].rep;
                                    dungeon->items.push_back(dungeon->player.carrying[item_to_drop]);
                                    dungeon->items[dungeon->items.size() - 1].x_pos = new_x;
                                    dungeon->items[dungeon->items.size() - 1].y_pos = new_y;
                                    dungeon->player.carrying[item_to_drop] = get_blank_item(*dungeon, 1);
                                    mvprintw(0, 0, "%s", spaces);
                                    mvprintw(0, 0, "%s", colors[dungeon->items[dungeon->items.size() - 1].color].c_str());
                                    refresh();
                                    dropped = 1;
                                    break;
                                }
                            }
                            if(!dropped){
                                mvprintw(0, 0, "%s", spaces);
                                mvprintw(0, 0, "Unable to drop item: surrounded by objects and/or rocks");
                                dropped = 1;
                            }
                        }
                        else{
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "Please try again but enter a valid carry slot.");
                            dropped = 1;
                            refresh();
                        }
                    }while(!dropped && input != ESC);
                    clear_dungeon(dungeon);
                    print_dungeon(dungeon);
                    refresh();
                }
                else if(move == 'x') {
                    clear_dungeon(dungeon);
                    print_inventory(dungeon);
                    mvprintw(0, 0, "%s", spaces);
                    mvprintw(13, 0, "%s", spaces);
                    mvprintw(0, 0, "Select an item to permanently delete. Press Escape to return");
                    refresh();
                    int deleted = 0;
                    int item_to_delete, input;
                    do{
                        input = getch();
                        item_to_delete = input - '0';
                        if (item_to_delete < 10 && item_to_delete >= 0) {
                            clear_dungeon(dungeon);
                            attron(COLOR_PAIR(5));
                            mvprintw(5, 0, "ARE YOU SURE YOU WANT TO DELETE %s?", dungeon->player.carrying[item_to_delete].name.c_str());
                            mvprintw(6, 0, "ENTER y for yes, n for no. CASE SENSITIVE");
                            attroff(COLOR_PAIR(5));
                            int decision;
                            while(1){
                                decision = getch();
                                if(decision != 'y' && decision != 'n'){
                                    mvprintw(0, 0, "Please Enter y, n. (y's are for yes, n's are no");
                                }
                                else{
                                    break;
                                }
                            }
                            if(decision == 'y'){
                                dungeon->player.carrying[item_to_delete] = get_blank_item(*dungeon, 1);
                                deleted = 1;
                            }

                        } else {
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "Please try again but enter a valid carry slot.");
                            deleted = 1;
                            refresh();
                        }
                    }while (!deleted && input != ESC);
                    clear_dungeon(dungeon);
                    print_dungeon(dungeon);
                    refresh();
                }
                else if(move == ','){
                    int item = is_item(dungeon, dungeon->player.x_pos, dungeon->player.y_pos);
                    if(item >= 0){
                        int ij, full = 1;
                        for(ij = 0; ij < 10; ij++){
                            if(dungeon->player.carrying[ij].rrty == 1000){
                                dungeon->player.carrying[ij] = dungeon->items[item];
                                dungeon->items.erase(dungeon->items.begin() + item);
                                full = 0;
                                break;
                            }
                        }
                        if(full){
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "Unable to pick up due to full inventory");
                        }
                    }
                    else{
                        mvprintw(0, 0, "%s", spaces);
                        mvprintw(0, 0, "You are not over an item");
                    }
                }
                else if(move == 'L'){
                    int np[2] = {0, 0};
                    select_monster(dungeon, np);
                    int i;
                    for(i = 0; i < (int) dungeon->mons.size(); i++){
                        if(dungeon->mons[i].alive && (dungeon->mons[i].y_pos == np[0] && dungeon->mons[i].x_pos == np[1])){
                            clear_dungeon(dungeon);
                            mvprintw(0, 0, "%s", spaces);
                            mvprintw(0, 0, "Press the escape key to return to the game");
                            mvprintw(2, 0, "The monster you selected is %s:", dungeon->mons[i].name.c_str());
                            mvprintw(4, 0, "%s", dungeon->mons[i].desc.c_str());
                            refresh();
                            int temp;
                            do{
                                temp = getch();
                            }while(temp != ESC);
                            break;
                        }
                    }
                    clear_dungeon(dungeon);
                    print_dungeon(dungeon);
                }
                else{
                    move = 'm';
                }
                if(!(move == 'm' || move == '?' || move == 'f' || move == 'i' || move == 'e' || move == 'I' || move == 'w' || move == 't' || move == 'd' || move == 'x' || move == ',')){
                    if(dungeon->player.infect_amount){
                        dungeon->player.hp -= dungeon->player.is_infected * dungeon->player.infect_amount;
                        if(dungeon->player.hp <= 0){
                            dungeon->player.alive = 0;
                            return 0;
                        }
                    }
                    else if(dungeon->player.immune){
                        dungeon->player.immune_count++;
                        mvprintw(0, 0, "%s");
                        mvprintw(0, 0, "You have %d turns left being immune to the Virtual Semester                   ", dungeon->player.turns_immune - dungeon->player.immune_count);
                        if(!(dungeon->player.turns_immune - dungeon->player.immune_count)){
                            mvprintw(0, 0, "%s");
                            attron(COLOR_PAIR(RED));
                            mvprintw(0, 0, "You are no longer immune to the Virtual Semester!                           ");
                            attroff(COLOR_PAIR(RED));
                            dungeon->player.immune = 0;
                        }
                        refresh();
                    }
                }
            }while(move == 'm' || move == '?' || move == 'f' || move == 'i' || move == 'e' || move == 'I' || move == 'w' || move == 't' || move == 'd' || move == 'x' || move == ',');

            dungeon->player.speed = update_speed(dungeon);

            if((next_pos[0] <= 0 || next_pos[0] >= MAP_Y_MAX - 1) || (next_pos[1] <= 0 || next_pos[1] >= MAP_X_MAX - 1) || (next_pos[0] == dungeon->player.y_pos && next_pos[1] == dungeon->player.x_pos)){
                continue;
            }
            if(dungeon->hardness[next_pos[0]][next_pos[1]] && !tport){
                mvprintw(0, 0, "You can't go through walls you silly goose! Stick to the dungeon.               ");
                continue;
            }
            attron(COLOR_PAIR(dungeon->player.is_infected ? GREEN : WHITE));
            mvprintw(22, 60, "Move %c     ", move == KEY_UP ? 'U' : move == KEY_DOWN ? 'D' : move == KEY_RIGHT ? 'R' : move == KEY_LEFT ? 'L' : move == KEY_PPAGE ? 'P' : move == KEY_NPAGE ? 'N' : move == KEY_HOME ? 'H' : move == KEY_END ? 'E' : move);
            attroff(COLOR_PAIR(dungeon->player.is_infected ? GREEN : WHITE));
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
        if(dungeon->boss_killed){
            return 0;
        }
        if(dungeon->player.alive) continue;
        else break;
    }
    return dungeon->player.alive;
}

void select_monster(Dungeon *dungeon, int next_pos[2]){
    int move;
    mvprintw(0, 0, "%s", spaces);
    mvprintw(0, 0, "Move to the monster you want to inspect.");
    mvaddch(dungeon->player.y_pos + 1, dungeon->player.x_pos, TELEPORT);
    refresh();
    next_pos[0] = dungeon->player.y_pos;
    next_pos[1] = dungeon->player.x_pos;
    while(1){
        move = getch();

        if(move == KEY_UP && (next_pos[0] - 1 > 0)){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[0]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_DOWN && (next_pos[0] + 1 < (MAP_Y_MAX - 1))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_LEFT && (next_pos[1] - 1 > 0)){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_RIGHT && (next_pos[1] + 1 < (MAP_X_MAX - 1))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_PPAGE && ((next_pos[1] + 1 < (MAP_X_MAX - 1)) && (next_pos[0] - 1 > 0))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            next_pos[0]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_NPAGE && ((next_pos[1] + 1 < (MAP_X_MAX - 1)) && (next_pos[0] + 1 < (MAP_Y_MAX - 1)))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_END && ((next_pos[1] - 1 > 0) && (next_pos[0] + 1 < (MAP_Y_MAX - 1)))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]--;
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_HOME && ((next_pos[1] - 1 > 0) && (next_pos[0] - 1 > 0))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]--;
            next_pos[0]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }

        if(move == 't'){
            if(!is_open(dungeon, next_pos) && (next_pos[0] != dungeon->player.y_pos && next_pos[1] != dungeon->player.y_pos)){
                return;
            }
            else{
                mvprintw(0, 0, "%s", spaces);
                mvprintw(0, 0, "Must select a monster");
                refresh();
            }
        }
    }
}

int update_speed(Dungeon *dungeon){
    int ret_speed = 10;
    int i;
    for(i = 0; i < 12; i++){
        ret_speed += dungeon->player.equipped[i].speed;
    }

    return ret_speed;
}

int get_equip_index(char input){
    if(input == 'a') return 0;
    else if(input == 'b') return 1;
    else if(input == 'c') return 2;
    else if(input == 'd') return 3;
    else if(input == 'e') return 4;
    else if(input == 'f') return 5;
    else if(input == 'g') return 6;
    else if(input == 'h') return 7;
    else if(input == 'i') return 8;
    else if(input == 'j') return 9;
    else if(input == 'k') return 10;
    else if(input == 'l') return 11;
    else return 13;
}

void print_equipped(Dungeon *dungeon){
    clear_dungeon(dungeon);
    mvprintw(0, 0, "%s", spaces);
    mvprintw(0, 0, "This is your equipped inventory. Blank indicates empty");
    refresh();
    int i,j;
    for(i = 0; i < 12; i++){
        Item item = dungeon->player.equipped[i];
        mvprintw(i + 2, 0, "%c [", equip_slots[i]);
        for(j = 3; j < equip_cats[i][j - 3]; j++){
            mvprintw(i + 2, j, "%c", tolower(equip_cats[i][j - 3]));
        }
        mvprintw(i + 2, j, "]");
        int len = strlen(equip_cats[i]) + 4;

        for(j = len; j < 11; j++){
            mvprintw(i + 2, j, " ");
        }
        mvprintw(i + 2, j, ")");
        if(item.rrty != 1000){
            mvprintw(i + 2, 13, "%s (sp: %d, dm: %d+%dd%d)", item.name.c_str(), item.speed, item.damage_bonus.base, item.damage_bonus.number, item.damage_bonus.sides);
        }
    }
    mvprintw(i + 3, 0, "Hit any key to continue");
    refresh();
}

void print_inventory(Dungeon *dungeon){
    clear_dungeon(dungeon);
    mvprintw(0, 0, "%s", spaces);
    mvprintw(0, 0, "This is your carrying inventory. Blank indicates empty");

    refresh();
    int i;
    for(i = 0; i < 10; i++){
        Item item = dungeon->player.carrying[i];
        mvprintw(i + 2, 0, "%d) ", i);
        if(item.rrty != 1000){
            mvprintw(i + 2, 3, "%s (sp: %d, dm: %d+%dd%d)", item.name.c_str(), item.speed, item.damage_bonus.base, item.damage_bonus.number, item.damage_bonus.sides);
        }
    }
    mvprintw(i + 3, 0, "Hit any key to continue");
    refresh();
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

        if(move == KEY_UP && (next_pos[0] - 1 > 0)){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[0]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_DOWN && (next_pos[0] + 1 < (MAP_Y_MAX - 1))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_LEFT && (next_pos[1] - 1 > 0)){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_RIGHT && (next_pos[1] + 1 < (MAP_X_MAX - 1))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_PPAGE && ((next_pos[1] + 1 < (MAP_X_MAX - 1)) && (next_pos[0] - 1 > 0))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            next_pos[0]--;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_NPAGE && ((next_pos[1] + 1 < (MAP_X_MAX - 1)) && (next_pos[0] + 1 < (MAP_Y_MAX - 1)))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]++;
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_END && ((next_pos[1] - 1 > 0) && (next_pos[0] + 1 < (MAP_Y_MAX - 1)))){
            mvaddch(next_pos[0] + 1, next_pos[1], dungeon->fow ? dungeon->fmap[next_pos[0]][next_pos[1]] : dungeon->dmap[next_pos[0]][next_pos[1]]);
            next_pos[1]--;
            next_pos[0]++;
            mvaddch(next_pos[0] + 1, next_pos[1], TELEPORT);
        }
        else if(move == KEY_HOME && ((next_pos[1] - 1 > 0) && (next_pos[0] - 1 > 0))){
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

int get_damage(Dungeon *dungeon){
    int total_damage = roll(dungeon->player.damage);
    int i;

    for(i = 0; i < 12; i++){
        total_damage += roll(dungeon->player.equipped[i].damage_bonus);
    }

    return total_damage;
}

int do_combat(Dungeon *dungeon, int mon_index, int player_attack){
    int dam;
    if(player_attack){
        dam = get_damage(dungeon);
        mvprintw(0, 0, "%s", spaces);
        mvprintw(0, 0, "Monster HP: %d     Damage Done: %d     Monster HP Remaining: %d", dungeon->mons[mon_index].health, dam, dungeon->mons[mon_index].health - dam);
        refresh();
        if(dungeon->mons[mon_index].type & INFECT){
            if((rand() & CHANCE) && !dungeon->player.immune && !dungeon->player.is_infected){
                dam = roll(dungeon->mons[mon_index].Damage);
                dungeon->player.is_infected = 1;
                dungeon->player.infect_amount = dam;
                dungeon->mons[mon_index].alive = 0;
                int is_room = find_room(dungeon, dungeon->mons[mon_index].x_pos, dungeon->mons[mon_index].y_pos);
                dungeon->dmap[dungeon->mons[mon_index].y_pos][dungeon->mons[mon_index].x_pos] = find_stairs(dungeon, dungeon->mons[mon_index].x_pos, dungeon->mons[mon_index].y_pos,is_room == -1);
                dungeon->player.hp -= dam;
                clear_dungeon(dungeon);
                clock_t timer;
                timer = clock();
                attron(COLOR_PAIR(GREEN));
                mvprintw(10, 14, "You are infected. You will lose %d health every turn.", dam);
                mvprintw(11, 14, "Find or use a vaccine to cure yourself before you die.");
                while(((float)(clock() - timer) / CLOCKS_PER_SEC) < 7){
                    mvprintw(12, 14, "This screen will close in %f seconds   ", 7 - ((float)(clock() - timer) / CLOCKS_PER_SEC));
                    refresh();
                }
                attroff(COLOR_PAIR(GREEN));
                clear_dungeon(dungeon);
                print_dungeon(dungeon);
                return 0;
            }
        }
        dungeon->mons[mon_index].health -= dam;
        dungeon->mons[mon_index].alive = dungeon->mons[mon_index].health > 0 ? 1 : 0;
        return dungeon->mons[mon_index].alive;
    }
    else{
        dam = roll(dungeon->mons[mon_index].Damage);
        if((dungeon->mons[mon_index].type & INFECT) && !dungeon->player.immune && !dungeon->player.is_infected) {
            dungeon->player.is_infected = 1;
            dungeon->player.infect_amount = dam;
            dungeon->mons[mon_index].alive = 0;
            int is_room = find_room(dungeon, dungeon->mons[mon_index].x_pos, dungeon->mons[mon_index].y_pos);
            dungeon->dmap[dungeon->mons[mon_index].y_pos][dungeon->mons[mon_index].x_pos] = find_stairs(dungeon, dungeon->mons[mon_index].x_pos, dungeon->mons[mon_index].y_pos,is_room == -1);
            clear_dungeon(dungeon);
            clock_t timer;
            timer = clock();
            attron(COLOR_PAIR(GREEN));
            mvprintw(10, 14, "You are infected. You will lose %d health every turn.", dam);
            mvprintw(11, 14, "Find or use a vaccine to cure yourself before you die.");
            while(((float)(clock() - timer) / CLOCKS_PER_SEC) < 7){
                mvprintw(12, 14, "This screen will close in %f seconds   ", 7 - ((float)(clock() - timer) / CLOCKS_PER_SEC));
                refresh();
            }
            attroff(COLOR_PAIR(GREEN));
            clear_dungeon(dungeon);
            print_dungeon(dungeon);
        }
        dungeon->player.hp -= dam;
        dungeon->player.alive = dungeon->player.hp > 0 ? 1 : 0;
        return dungeon->player.alive;
    }
}

int is_character(Dungeon *dungeon, std::vector<Monster> mons, int next_pos[2], Turn turn){
    int i, combat;
    if(!turn.seq){
        for(i = 0; i < int (dungeon->num_mons); i++){
            if(dungeon->mons[i].alive && (next_pos[0] == dungeon->mons[i].y_pos && next_pos[1] == dungeon->mons[i].x_pos)){
                combat = do_combat(dungeon, i, 1);
                if(!combat){
                    mvprintw(0, 0, "%s", spaces);
                    mvprintw(0, 0, "You killed a monster, way to go!");
                    refresh();
                    if(dungeon->mons[i].type & BOSS){
                        dungeon->boss_killed = 1;
                    }
                    return 0;
                }
                return 1;
            }
        }
    }
    else{
        for(i = 0; i < int (dungeon->num_mons); i++){
            if(dungeon->mons[i].alive && (next_pos[0] == dungeon->mons[i].y_pos && next_pos[1] == dungeon->mons[i].x_pos) && !(next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos)){ //If the next space is a monster, kill the monster
                return i;
            }
            else if(next_pos[0] == dungeon->player.y_pos && next_pos[1] == dungeon->player.x_pos && (dungeon->mons[i].y_pos == dungeon->mons[turn.seq - 1].y_pos && dungeon->mons[i].x_pos == dungeon->mons[turn.seq - 1].x_pos)){ //If it is the player, kill the player
                combat = do_combat(dungeon, i, 0);
                return combat ? -3 : -1;
            }
        }
    }
    return -2;
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
        if(!character){ //If the next space is a monster
            dungeon->dmap[next_pos[0]][next_pos[1]] = PLAYER;
            dungeon->fmap[next_pos[0]][next_pos[1]] = PLAYER;
            dungeon->dmap[dungeon->player.y_pos][dungeon->player.x_pos] = find_stairs(dungeon, dungeon->player.x_pos, dungeon->player.y_pos, is_room == -1);
            dungeon->fmap[dungeon->player.y_pos][dungeon->player.x_pos] = find_stairs(dungeon, dungeon->player.x_pos, dungeon->player.y_pos, is_room == -1);
            dungeon->player.x_pos = next_pos[1]; //Move the monster
            dungeon->player.y_pos = next_pos[0];
        }
        else if(character == -2){ //If it is rock
            dungeon->dmap[next_pos[0]][next_pos[1]] = PLAYER;
            dungeon->dmap[dungeon->player.y_pos][dungeon->player.x_pos] = find_stairs(dungeon, dungeon->player.x_pos, dungeon->player.y_pos, is_room == -1);
            dungeon->player.x_pos = next_pos[1]; //Move the player
            dungeon->player.y_pos = next_pos[0];
        }
        do_maps(dungeon);
    }
    else{
        if(dungeon->mons[turn.seq - 1].type & INFECT){
            get_next_pos(dungeon, turn, next_pos, 1, 0, 1);
            character = is_character(dungeon, dungeon->mons, next_pos, turn);
            is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos);
            if(character >= 0){ //If the next space is a monster
                //Do nothing
            }
            else if(character == -2){ //If it is rock
                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                if(is_room == -1){ //Checks if you are in a room
                    // Do nothing
                }
                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
            }
            else {
                return;
            }
        }
        else if(dungeon->mons[turn.seq - 1].type == 0 || dungeon->mons[turn.seq - 1].type == 8){
            while(1){
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                if((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->hardness[next_pos[0]][next_pos[1]] || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE) continue;
                else break;
            }
            character = is_character(dungeon, dungeon->mons, next_pos, turn);
            is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos);
            if(character >= 0){
                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon,dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
            }
            else if(character == -2){
                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = find_stairs(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos, is_room == -1);
                dungeon->mons[turn.seq - 1].x_pos = next_pos[1];
                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
            }
            else{
                return;
            }
        }
        else if(dungeon->mons[turn.seq - 1].type == 4 || dungeon->mons[turn.seq - 1].type == 12 || dungeon->mons[turn.seq - 1].type & BOSS){
            while(1){
                // Randomly generate a number [-1, 1] for x and y direction that the monster will move
                next_pos[0] = dungeon->mons[turn.seq - 1].y_pos + ((rand() % 3) + 1) - 2;
                next_pos[1] = dungeon->mons[turn.seq - 1].x_pos + ((rand() % 3) + 1) - 2;
                if((next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos) || dungeon->dmap[next_pos[0]][next_pos[1]] == TOP || dungeon->dmap[next_pos[0]][next_pos[1]] == SIDE) continue;
                else break;
            }
            character = is_character(dungeon, dungeon->mons, next_pos, turn);
            is_room = find_room(dungeon, dungeon->mons[turn.seq - 1].x_pos, dungeon->mons[turn.seq - 1].y_pos);
            if(character >= 0){ //If the next space is a monster
                dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                if(is_room == -1){ //Checks if you are in a room
                    do_maps(dungeon); //Redo distance maps
                }
                dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = dungeon->mons[character].rep;
                dungeon->mons[character].x_pos = dungeon->mons[turn.seq - 1].x_pos;
                dungeon->mons[character].y_pos = dungeon->mons[turn.seq - 1].y_pos;
                dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
            }
            else if(character == -2){ //If it is rock
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
                        if(character >= 0){ //If the next space is a monster
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = dungeon->mons[character].rep;
                            dungeon->mons[character].x_pos = dungeon->mons[turn.seq - 1].x_pos;
                            dungeon->mons[character].y_pos = dungeon->mons[turn.seq - 1].y_pos;
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(character == -2){ //If it is rock
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
                        if(character >= 0){
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = dungeon->mons[character].rep;
                            dungeon->mons[character].x_pos = dungeon->mons[turn.seq - 1].x_pos;
                            dungeon->mons[character].y_pos = dungeon->mons[turn.seq - 1].y_pos;
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(character == -2){
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
                        if(character >= 0){ //If the next space is a monster
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = dungeon->mons[character].rep;
                            dungeon->mons[character].x_pos = dungeon->mons[turn.seq - 1].x_pos;
                            dungeon->mons[character].y_pos = dungeon->mons[turn.seq - 1].y_pos;
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(character == -2){ //If it is rock
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
                        if(character >= 0 && !(next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos)){
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = dungeon->mons[character].rep;
                            dungeon->mons[character].x_pos = dungeon->mons[turn.seq - 1].x_pos;
                            dungeon->mons[character].y_pos = dungeon->mons[turn.seq - 1].y_pos;
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(character == -2){
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
                        if(character >= 0){ //If the next space is a monster
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = dungeon->mons[character].rep;
                            dungeon->mons[character].x_pos = dungeon->mons[turn.seq - 1].x_pos;
                            dungeon->mons[character].y_pos = dungeon->mons[turn.seq - 1].y_pos;
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(character == -2){ //If it is rock
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
                        if(character >= 0 && !(next_pos[0] == dungeon->mons[turn.seq - 1].y_pos && next_pos[1] == dungeon->mons[turn.seq - 1].x_pos)){
                            dungeon->dmap[next_pos[0]][next_pos[1]] = dungeon->mons[turn.seq - 1].rep;
                            if(is_room == -1){ //Checks if you are in a room
                                do_maps(dungeon); //Redo distance maps
                            }
                            dungeon->dmap[dungeon->mons[turn.seq - 1].y_pos][dungeon->mons[turn.seq - 1].x_pos] = dungeon->mons[character].rep;
                            dungeon->mons[character].x_pos = dungeon->mons[turn.seq - 1].x_pos;
                            dungeon->mons[character].y_pos = dungeon->mons[turn.seq - 1].y_pos;
                            dungeon->mons[turn.seq - 1].x_pos = next_pos[1]; //Move the monster
                            dungeon->mons[turn.seq - 1].y_pos = next_pos[0];
                        }
                        else if(character == -2){
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
    if(dungeon->mons[turn.seq - 1].type & INFECT){
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
    }
    else if(!telepathic && smart && seen(dungeon->mons[turn.seq - 1].pc_location)){
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
    }while(move != ESC);
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

void create_items(Dungeon *dungeon){
    int i, j, rand_num, rrty, placed;
    for(i = 0; i < int (dungeon->num_items); i++){
        placed = 0;
        while(!placed){
            rand_num = rand() % dungeon->descriptions.item_list.size();
            Item item = dungeon->descriptions.item_list[rand_num];
            if(item.artifact){
                int cont = 0;
                for(j = 0; j < (int) dungeon->items.size(); j++){
                    if(!dungeon->items[j].desc.compare(item.desc)){
                        cont = 1;
                        break;
                    }
                }
                if(cont){
                    continue;
                }
            }
            rrty = rand() % 100;
            if(rrty >= item.rrty){
                continue;
            }
            else{
                item.hit = roll(item.hit_bonus);
                item.dodge = roll(item.dodge_bonus);
                item.speed = roll(item.speed_bonus);
                item.sattr = roll(item.spec_attr);
                item.val = roll(item.value);
                item.w = roll(item.weight);
                item.defense = roll(item.defense_bonus);
                item.x_pos = -1;
                item.y_pos = -1;
                dungeon->items.push_back(item);
                placed = 1;
            }
        }
    }
}

int roll(Dice dice){
    int total = 0, i;
    total += dice.base;
    if(dice.sides){
        for(i = 0; i < dice.number; i++){
            total += (rand() % (((dice.sides + 1) - 1))) + 1;
        }
    }

    return total;
}

//Monster functions
void create_monsters(Dungeon *dungeon){
    int i, rand_num, j, rrty, placed;
    for(i = 0; i < int (dungeon->num_mons); i++){
        placed = 0;
        while(!placed){
            rand_num = rand() % dungeon->descriptions.monster_list.size();
            Monster m = dungeon->descriptions.monster_list[rand_num];
            m.type = m.abil;
            if(m.type & UNIQ){
                int cont = 0;
                for(j = 0; j < (int) dungeon->mons.size(); j++){
                    if(dungeon->mons[j].type == m.type){
                        cont = 1;
                        break;
                    }
                }
                if(cont){
                    continue;
                }
            }
            rrty = rand() % 100;
            if(rrty >= m.rrty){
                continue;
            }
            else{
                m.speed = roll(m.Speed);
                m.health = roll(m.Health);
                m.alive = 1;
                m.pc_location[0] = -1;
                m.pc_location[1] = -1;
                dungeon->mons.push_back(m);
                placed = 1;
            }
        }
    }
}

void place_items(Dungeon *dungeon){
    int i;

    for(i = 0; i < (int) dungeon->num_items; i++){
        int x, y;
        while(1){
            x = (rand() % (MAP_X_MAX - 2)) + 1;
            y = (rand() % (MAP_Y_MAX - 2)) + 1;
            int room = find_room(dungeon, x, y);
            int stairs = find_stairs(dungeon, x, y, ((int) (room == -1)));
            if(dungeon->dmap[y][x] == PLAYER || is_monster_or_item(dungeon, x, y) != -999 || dungeon->hardness[y][x] || (!dungeon->hardness[y][x] && dungeon->dmap[y][x] == ROCK) || stairs == UP || stairs == DOWN){
                continue;
            }
            else{
                dungeon->dmap[y][x] = dungeon->items[i].rep;
                dungeon->items[i].x_pos = x;
                dungeon->items[i].y_pos = y;
                break;
            }
        }
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
            if(i == 0 || i == MAP_X_MAX - 1) std::cout << SIDE;// mvaddch(j, i, SIDE);// putchar(SIDE);
            else if(j == 0 || j == MAP_Y_MAX - 1) std::cout << TOP;// mvaddch(j, i, TOP);// putchar(TOP);
            else if(i == dungeon->player.x_pos && j == dungeon->player.y_pos) std::cout << PLAYER;// mvaddch(j, i, PLAYER);// putchar(PLAYER);
            else{
                if(tunneling){
//                    printf("%d", distances[j][i] % 10);
                    std::cout << (distances[j][i] % 10);
                }
                else{
                    if(dungeon->hardness[j][i] != 0) std::cout << ROCK;// mvaddch(j, i, ROCK);// putchar(ROCK);
                    else {
                        if(distances[j][i] < 0 || distances[j][i] == INT_MAX) std::cout << 'X';// mvaddch(j, i, 'X');  // putchar('X');
                        else std::cout << distances[j][i] % 10;// mvaddch(j, i, distances[j][i] % 10); //printf("%d", distances[j][i] % 10);
                    }
                }
            }
        }
        std::cout << std::endl;//printf("\n");
    }
    std::cout << std::endl;//printf("\n");
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
                        dungeon->down_stairs[num_stairs_placed[1]++] = down;
                    }
                    else if(!all_up_placed && up_or_down > 5){ //Make an up staircase if not all have been placed
                        dungeon->dmap[src_y][i] = UP;
                        UpStairs up(src_x, src_y);
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
                        dungeon->down_stairs[num_stairs_placed[1]++] = down;
                    }
                    else if(!all_up_placed && up_or_down > 5){
                        dungeon->dmap[src_y][i] = UP;
                        UpStairs up(src_x, src_y);
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
                    dungeon->down_stairs[num_stairs_placed[1]++] = down;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon->dmap[i][src_x] = UP;
                    UpStairs up(src_x, src_y);
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
                    dungeon->down_stairs[num_stairs_placed[1]++] = down;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon->dmap[i][src_x] = UP;
                    UpStairs up(src_x, src_y);
                    dungeon->up_stairs[num_stairs_placed[0]++] = up;
                }
            }
        }
        return;
    }
    else return;
}

int is_monster_or_item(Dungeon *dungeon, int x, int y){
    int i;
    for(i = 0; i < (int) dungeon->num_mons; i++){
        if(dungeon->mons[i].alive && ((x == dungeon->mons[i].x_pos && y == dungeon->mons[i].y_pos) && (!dungeon->fow || can_be_seen(dungeon, dungeon->mons[i], dungeon->player)))){
            return i;
        }
    }
    for(i = 0; i < (int) dungeon->num_items; i++){
        if((x == dungeon->items[i].x_pos && y == dungeon->items[i].y_pos) && (!dungeon->fow || item_visible(dungeon, dungeon->items[i], dungeon->player))){
            return ((-1 * i) - 1);
        }
    }
    return -999;
}

uint8_t item_visible(Dungeon *dungeon, Item item, Player player){
    int mon_room = find_room(dungeon, item.x_pos, item.y_pos);
    int player_room = find_room(dungeon, player.x_pos, player.y_pos);

    if(!(mon_room == -1 && player_room == -1) && mon_room == player_room){
        return 1;
    }

    int x, y;
    for(y = item.y_pos - 5; (y <= item.y_pos + 5) && (y > 0) && (y < MAP_Y_MAX - 1); y++){
        for(x = item.x_pos - 5; (x <= item.x_pos + 5) && (x > 0) && (x < MAP_X_MAX - 1); x++){
            if(x == player.x_pos && y == player.y_pos){
                return 1;
            }
        }
    }
    return 0;
}

void print_dungeon(Dungeon *dungeon){
    int j, i;
    for (j = 0; j < MAP_Y_MAX; j++) {
        for (i = 0; i < MAP_X_MAX; i++) {
            int mon = is_monster_or_item(dungeon, i, j);
            if(mon == -999){
                attron(COLOR_PAIR(dungeon->player.is_infected ? GREEN : WHITE));
                mvaddch(j + 1, i, dungeon->fow ? dungeon->fmap[j][i] : dungeon->dmap[j][i]);//printf("%1c", dungeon[j][i]);
                attroff(COLOR_PAIR(dungeon->player.is_infected ? GREEN : WHITE));
            }
            else{
                if(mon >= 0){
                    int col = rand() % dungeon->mons[mon].color.size();
                    int shade = dungeon->mons[mon].color[col];
                    attron(COLOR_PAIR(dungeon->player.is_infected ? GREEN : shade));
                    mvaddch(j + 1, i, dungeon->fow ? dungeon->fmap[j][i] : dungeon->dmap[j][i]);
                    attroff(COLOR_PAIR(dungeon->player.is_infected ? GREEN : shade));
                }
                else{
                    int shade = dungeon->items[(mon + 1) * -1].color;
                    attron(COLOR_PAIR(dungeon->player.is_infected ? GREEN : shade));
                    mvaddch(j + 1, i, dungeon->fow ? dungeon->fmap[j][i] : dungeon->dmap[j][i]);
                    attroff(COLOR_PAIR(dungeon->player.is_infected ? GREEN : shade));
                }
            }
            fputc(dungeon->dmap[j][i], dungeon->file);
        }
        fputc('\n', dungeon->file);
    }
    attron(COLOR_PAIR(dungeon->player.is_infected ? GREEN : WHITE));
    mvprintw(j + 1, 0, "There are %d monsters, %d of which are alive      ", dungeon->num_mons, get_num_alive_monsters(dungeon));
    mvprintw(j + 2, 0, "PC Location is (%d, %d)        ", dungeon->player.x_pos, dungeon->player.y_pos);
    mvprintw(j + 2, 24, "Health: %d    Speed %d   ", dungeon->player.hp, dungeon->player.speed);
    attroff(COLOR_PAIR(dungeon->player.is_infected ? GREEN : WHITE));

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
    int item = is_item(dungeon, x, y);
    return dungeon->hardness[y][x] ? ROCK : item >= 0 ? dungeon->items[item].rep :  is_corridor ? CORRIDOR : ROOM; //If it is a corridor, return #, otherwise .
}

int is_item(Dungeon *dungeon, int x, int y){
    int i;
    for(i = 0; i < (int) dungeon->num_items; i++){
        if(x == dungeon->items[i].x_pos && y == dungeon->items[i].y_pos){
            return i;
        }
    }
    return -1;
}
