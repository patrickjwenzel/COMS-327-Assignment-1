#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <endian.h>
#include <math.h>
#include <unistd.h>

#include "heap.c"
#include "dungeon.h"

//typedef struct dungeon{
//    char dmap[MAP_Y_MAX][MAP_X_MAX];
//    uint8_t hardness[MAP_Y_MAX][MAP_X_MAX];
//    int num_rooms;
//    int nt_distances[MAP_Y_MAX][MAP_X_MAX];
//    int t_distances[MAP_Y_MAX][MAP_X_MAX];
//    player_t player;
//    room_t *rooms;
//    tmon_t *tmons;
//    ntmon_t *ntmons;
//    uint16_t num_tmons;
//    uint16_t num_ntmons;
//    uint16_t num_down;
//    uint16_t num_up;
//    up_t *up_stairs;
//    down_t *down_stairs;
//} dungeon_t;

int main(int argc, char *argv[]){
	srand(time(NULL));
	dungeon_t dungeon;
    dungeon.num_rooms = (rand() % (15 - 6 + 1)) + 6; //Generates between 6-15 rooms
    dungeon.num_up = (rand() % 5) + 1; //Will place 1-5 up staircases
    dungeon.num_down = (rand() % 5) + 1; //Will place 1-5 down stair cases
    dungeon.num_mons = (rand() % 30) + 1; //Will place 1-30 monsters
    dungeon.up_stairs = malloc(dungeon.num_up * sizeof(up_t));
    dungeon.down_stairs = malloc(dungeon.num_down * sizeof(down_t));
    dungeon.rooms = malloc(dungeon.num_rooms * sizeof(room_t));
    dungeon.player.alive = 1;
    dungeon.player.speed = 10;
    int num_stairs_placed[2] = {0, 0};

    if(argc == 1){ //This means that it was just normally ran and dungeon will not be saved
        dungeon.mons = malloc(dungeon.num_mons * sizeof(mon_t));
        create_monsters(&dungeon);
        fill_dungeon(&dungeon);
        create_dungeon_map(&dungeon, num_stairs_placed, 0);
        do_maps(&dungeon);
        place_monsters(&dungeon);
        print_dungeon(dungeon.dmap);

//char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int num_rooms, int num_up, int num_down, int num_stairs_placed[2], int rooms[2], room_t room_array[], up_t up_stairs[],
// down_t down_stairs[], player_t player, int save
    }
	else if(argc == 2){
        if(!strcmp(argv[1], "--save")){ //This means the dungeon will be generated and saved;
            fill_dungeon(&dungeon);
            create_dungeon_map(&dungeon, num_stairs_placed, 1);
            print_dungeon(dungeon.dmap);
            do_maps(&dungeon);
        }
        else if(!strcmp(argv[1], "--load")){ //This will load a dungeon
//        void load_game(dungeon_t *dungeon, int num_stairs_placed[2], room_t room_array[]);
            load_game(&dungeon, num_stairs_placed);
            print_dungeon(dungeon.dmap);
            do_maps(&dungeon);
        }
        else if(!strcmp(argv[1], "--nummon")){ //This will load a dungeon
//        void load_game(dungeon_t *dungeon, int num_stairs_placed[2], room_t room_array[]);
            dungeon.num_mons = (uint16_t) atoi(argv[1]);
            dungeon.mons = malloc(dungeon.num_mons * sizeof(mon_t));
            create_monsters(&dungeon);
            fill_dungeon(&dungeon);
            create_dungeon_map(&dungeon, num_stairs_placed, 1);
            place_monsters(&dungeon);
            print_dungeon(dungeon.dmap);
            do_maps(&dungeon);
        }
        else{
            printf("Usage: %s --save|--load\n", argv[0]);
            return -1;
        }
	}
	else if(argc == 3){
	    if(!strcmp(argv[1], "--load") || !strcmp(argv[2], "--load")){
            load_game(&dungeon, num_stairs_placed);
	    }
	    else if(!strcmp(argv[1], "--save") || !strcmp(argv[2], "--save")) {
            save_game(&dungeon, num_stairs_placed);
        }
	    else{
            printf("Usage: %s --save|--load\n", argv[0]);
            return -1;
	    }
        do_maps(&dungeon);
    }
	else if(argc == 4){
        if(!strcmp(argv[1], "--load") || !strcmp(argv[2], "--load") || !strcmp(argv[3], "--load")){
            load_game(&dungeon, num_stairs_placed);
        }
        else if(!strcmp(argv[1], "--save") || !strcmp(argv[2], "--save") || !strcmp(argv[3], "--save")) {
            save_game(&dungeon, num_stairs_placed);
        }
        else{
            printf("Usage: %s --save|--load\n", argv[0]);
            return -1;
        }
        do_maps(&dungeon);
	}

	printf("%s\n\nYou faced %d monsters.\n", dungeon.player.alive ? "You win!" : "You suck, you lost #luser!", dungeon.num_mons);

	return 0;
}

void create_monsters(dungeon_t *dungeon){
    int i, type;
    for(i = 0; i < dungeon->num_mons; i++){
        type = rand() & 0x0000000f;
        dungeon->mons[i].alive = 1;
        dungeon->mons[i].rep = monster_reps[type];
        dungeon->mons[i].speed = (rand() % 16) + 5;;
        dungeon->mons[i].type = type;
    }
}

void place_monsters(dungeon_t *dungeon){
//    typedef struct dungeon{ //Dungeon struct
//        char dmap[MAP_Y_MAX][MAP_X_MAX];
//        uint8_t hardness[MAP_Y_MAX][MAP_X_MAX];
//        int num_rooms;
//        int nt_distances[MAP_Y_MAX][MAP_X_MAX];
//        int t_distances[MAP_Y_MAX][MAP_X_MAX];
//        player_t player;
//        room_t *rooms;
//        mon_t *mons;
//        uint32_t num_mons;
//        uint8_t type;
//        uint16_t num_down;
//        uint16_t num_up;
//        up_t *up_stairs;
//        down_t *down_stairs;
//    } dungeon_t;

    int i;

    for(i = 0; i < dungeon->num_mons; i++){
        int x, y;
        // If it is a tunneling monster
        if(dungeon->mons[i].type & TUNNEL){
            while(1){
                x = (rand() % (MAP_X_MAX - 2)) + 1;
                y = (rand() % (MAP_Y_MAX - 2)) + 1;
                if(dungeon->dmap[y][x] == PLAYER){
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
                if (dungeon->dmap[y][x] == PLAYER || dungeon->hardness[y][x] || (!dungeon->hardness[y][x] && dungeon->dmap[y][x] == ' ')) {
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

void create_dungeon_map(dungeon_t *dungeon, int num_stairs_placed[2], int save) {
	int i, j = 0, x, y, room_x_pos, room_y_pos, num_rooms_placed = 0, num_cycles = 0, player_placed = 0;
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
                            int up_or_down;
                            if(!player_placed){
                                dungeon->dmap[y][x] = PLAYER; //Place a player in the first room
                                dungeon->player.x_pos = x;
                                dungeon->player.y_pos = y;
                                player_placed = 1;
                            }
                            else{
                                if(staircase >= 1 && staircase <= 3){ //If random number is 1-3, place a staircase
                                    up_or_down = (rand() % 10) + 1; //Gets a random number 1-10
                                    if(up_or_down > 5 && num_stairs_placed[0] < dungeon->num_up){ //If the number is 6-10 and the max up staircases have not been placed place an up staircase
                                        dungeon->dmap[y][x] = UP;
                                        up_t up;
                                        up.x_pos = x;
                                        up.y_pos = y;
                                        dungeon->up_stairs[num_stairs_placed[0]++] = up;
                                    }
                                    else if(up_or_down <= 5 && num_stairs_placed[1]  < dungeon->num_down){ //If the number is 1-5 and the max down staircases have not been placed place a down staircase
                                        dungeon->dmap[y][x] = DOWN;
                                        down_t down;
                                        down.y_pos = y;
                                        down.x_pos = x;
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

    for(i = 1; i < dungeon->num_rooms; i++){
        //room_array[i] = src
        //room_array[j] = dest
//            void shortest_path(dungeon_t *dungeon, int src_x, int src_y, int dest_x, int dest_y, int num_stairs_placed[]);
        shortest_path(dungeon, dungeon->rooms[i].x_pos, dungeon->rooms[i].y_pos, dungeon->rooms[j].x_pos, dungeon->rooms[j].y_pos, num_stairs_placed); //Make a path from one room to the next
        j = i;
    }

    if(save){ //If we are saving, write this dungeon to disk
        save_game(dungeon, num_stairs_placed);
    }
    return;
}

void fill_dungeon(dungeon_t *dungeon){
	int j, i;
	for(j = 0; j < MAP_Y_MAX; j++){ //Fill the dungeon with ' 's and line borders with | or -. Make border hardness 255 and other hardness between 1-254
		for(i = 0; i < MAP_X_MAX; i++){
            if (i == 0 || i == MAP_X_MAX - 1) {
                dungeon->hardness[j][i] = HARDNESS_MAX;
                dungeon->dmap[j][i] = '|';
            }
            else if (j == 0 || j == MAP_Y_MAX - 1) {
                dungeon->hardness[j][i] = HARDNESS_MAX;
                dungeon->dmap[j][i] = '-';
            }
            else{
                dungeon->dmap[j][i] = ' ';
                dungeon->hardness[j][i] = (rand() % (254 - 1)) + 1;
            }
		}
	}
}

room_t newRoom(int x_pos, int y_pos, int x_size, int y_size){ //Returns a new room struct with the rooms position and size
	struct room new_room;
	new_room.x_pos = x_pos;
	new_room.y_pos = y_pos;
	new_room.x_size = x_size;
	new_room.y_size = y_size;
	return new_room;
}

int is_room(dungeon_t dungeon, int room_x_pos, int room_y_pos, int rooms[][2], int i){ //Checks to see if a new room will collide with an existing room
	int x, y;
	for(y = room_y_pos - 1; y < room_y_pos + rooms[i][1] + 1; y++){
		for(x = room_x_pos - 1; x < room_x_pos + rooms[i][0] + 1; x++){
			if(dungeon.dmap[y][x] == ROOM || dungeon.dmap[y][x] == PLAYER) return 1;
		}
	}
	return 0;
}

void shortest_path(dungeon_t *dungeon, int src_x, int src_y, int dest_x, int dest_y, int num_stairs_placed[]){ //Start of the shortest path print for the corridor
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
void modified_dfs(dungeon_t *dungeon, int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_stairs_placed[]){
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
                        down_t down;
                        down.x_pos = src_x;
                        down.y_pos = src_y;
                        dungeon->down_stairs[num_stairs_placed[1]++] = down;
                    }
                    else if(!all_up_placed && up_or_down > 5){ //Make an up staircase if not all have been placed
                        dungeon->dmap[src_y][i] = UP;
                        up_t up;
                        up.x_pos = src_x;
                        up.y_pos = src_y;
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
                        down_t down;
                        down.x_pos = src_x;
                        down.y_pos = src_y;
                        dungeon->down_stairs[num_stairs_placed[1]++] = down;
                    }
                    else if(!all_up_placed && up_or_down > 5){
                        dungeon->dmap[src_y][i] = UP;
                        up_t up;
                        up.x_pos = src_x;
                        up.y_pos = src_y;
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
                    down_t down;
                    down.x_pos = src_x;
                    down.y_pos = src_y;
                    dungeon->down_stairs[num_stairs_placed[1]++] = down;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon->dmap[i][src_x] = UP;
                    up_t up;
                    up.x_pos = src_x;
                    up.y_pos = src_y;
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
                    down_t down;
                    down.x_pos = src_x;
                    down.y_pos = src_y;
                    dungeon->down_stairs[num_stairs_placed[1]++] = down;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon->dmap[i][src_x] = UP;
                    up_t up;
                    up.x_pos = src_x;
                    up.y_pos = src_y;
                    dungeon->up_stairs[num_stairs_placed[0]++] = up;
                }
            }
        }
        return;
    }
    else return;
}

void save_game(dungeon_t *dungeon, int num_stairs_placed[]){
    int i;
    char *home = getenv("HOME");
    char *game_dir = ".rlg327";
    char *game_file = "dungeon";
    char *path = malloc((strlen(home) + strlen(game_dir) + strlen(game_file) + 2 + 1) * sizeof(char));
    sprintf(path, "%s/%s/%s", home, game_dir, game_file);
    int num_up = num_stairs_placed[0];
    int num_down = num_stairs_placed[1];

    FILE *file;
    char header[12] = "RLG327-S2021";
    int version = 0;
    int size = 1708 + (num_up * 2) + (num_down * 2) + (dungeon->num_rooms * 4); //1708 is the size of all bytes besides number of staircases and number of dungeon->rooms
    if(!(file = fopen(path, "w"))){
        printf("Failed to open this file\n");
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

    fclose(file);
}

void load_game(dungeon_t *dungeon, int num_stairs_placed[2]){
    FILE *file;
    int i, version, size;
    char header[12];

    char *home = getenv("HOME");
    char *game_dir = ".rlg327";
    char *game_file = DUNGEON; //WELL_DONE HELLO DUNGEON ADVENTURE "00.rlg327";

    char *path = malloc((strlen(home) + strlen(game_dir) + strlen(game_file) + 2 + 1) * sizeof(char));
    sprintf(path, "%s/%s/%s", home, game_dir, game_file);

    if(!(file = fopen(path, "r"))){
        printf("Failed to open this file\n");
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

    dungeon->rooms = malloc(dungeon->num_rooms * sizeof(room_t));

    for(i = 0; i < dungeon->num_rooms; i++){
        fread(&dungeon->rooms[i].x_pos, sizeof(dungeon->rooms[i].x_pos), 1, file);
        fread(&dungeon->rooms[i].y_pos, sizeof(dungeon->rooms[i].y_pos), 1, file);
        fread(&dungeon->rooms[i].x_size, sizeof(dungeon->rooms[i].x_size), 1, file);
        fread(&dungeon->rooms[i].y_size, sizeof(dungeon->rooms[i].y_size), 1, file);
    }

    fread(&num_stairs_placed[0], 2, 1, file);
    num_stairs_placed[0] = be16toh(num_stairs_placed[0]);
    dungeon->up_stairs = malloc(num_stairs_placed[0] * sizeof(up_t));

    for(i = 0; i < num_stairs_placed[0]; i++){
        fread(&dungeon->up_stairs[i].x_pos, sizeof(dungeon->up_stairs[i].x_pos), 1, file);
        fread(&dungeon->up_stairs[i].y_pos, sizeof(dungeon->up_stairs[i].y_pos), 1, file);
    }

    fread(&num_stairs_placed[1], 2, 1, file);
    num_stairs_placed[1] = be16toh(num_stairs_placed[1]);
    dungeon->down_stairs = malloc(num_stairs_placed[1] * sizeof(down_t));

    for(i = 0; i < num_stairs_placed[1]; i++){
        fread(&dungeon->down_stairs[i].x_pos, sizeof(dungeon->down_stairs[i].x_pos), 1, file);
        fread(&dungeon->down_stairs[i].y_pos, sizeof(dungeon->down_stairs[i].y_pos), 1, file);
    }
    fclose(file);

    make_dungeon(dungeon);
}

void make_dungeon(dungeon_t *dungeon){
    int j, i, index;
    for(j = 0; j < MAP_Y_MAX; j++){
        for(i = 0; i < MAP_X_MAX; i++){
            if(dungeon->player.x_pos == i && dungeon->player.y_pos == j){ //If this is the dungeon->player, set the dungeon->player here
                dungeon->dmap[j][i] = PLAYER;
            }
            else if(dungeon->hardness[j][i] == 255){
                dungeon->dmap[j][i] = (j == 0 || j == MAP_Y_MAX - 1) ? '-' : '|'; //If it is a left/right wall make it |, otherwise -
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
                dungeon->dmap[j][i] = ' ';
            }
        }
    }
}

int find_room(dungeon_t *dungeon, int x, int y){
    int i;
    for(i = 0; i < dungeon->num_rooms; i++){
        //If a room is located between these x and y coordinates return the index of that room array
        if(x >= dungeon->rooms[i].x_pos && x < (dungeon->rooms[i].x_pos + dungeon->rooms[i].x_size) && y >= dungeon->rooms[i].y_pos && (y < dungeon->rooms[i].y_pos + dungeon->rooms[i].y_size)) return i;
    }
    //Otherwise, return -1
    return -1;
}

char find_stairs(dungeon_t *dungeon, int x, int y, int is_corridor){
    int i;
    for(i = 0; i < dungeon->num_up; i++){
        if(dungeon->up_stairs[i].x_pos == x && dungeon->up_stairs[i].y_pos == y) return UP; //If this is an up staircase
    }
    for(i = 0; i < dungeon->num_down; i++){
        if(dungeon->down_stairs[i].x_pos == x && dungeon->down_stairs[i].y_pos == y) return DOWN; //If this is a down staircase
    }
    return is_corridor ? CORRIDOR : ROOM; //If it is a corridor, return #, otherwise .
}

void print_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX]){
    int j, i;
    for (j = 0; j < MAP_Y_MAX; j++) {
        for (i = 0; i < MAP_X_MAX; i++) {
            printf("%1c", dungeon[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

void do_maps(dungeon_t *dungeon){
    printf("Non-tunneling monster map:\n");
    dijkstra_map(dungeon, 0);
    print_path_map(dungeon->nt_distances, dungeon, 0);
    printf("Tunneling monster map:\n");
//    static void dijkstra_map(int distances[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, room_t room_array[], int num_rooms, int tunneling);
    dijkstra_map(dungeon, 1);
    print_path_map(dungeon->t_distances, dungeon, 1);
}

/*
 * From Professor's code
 */
static int32_t corridor_path_cmp(const void *key, const void *with) {
    return ((corridor_path_t *) key)->cost - ((corridor_path_t *) with)->cost;
}

static int32_t turn_cmp(const void *key, const void *with) {
    int32_t ret = ((turn_t *) key)->next_turn - ((turn_t *) with)->next_turn;
    if(!ret){
        ret = ((turn_t *) key)->seq - ((turn_t *) with)->seq;
    }
    return ret;
}

static void turn_decider(dungeon_t *dungeon, turn_t turn_event[], int *init){
    int num_characters = dungeon->num_mons + 1;
    static turn_t *t;
    static uint32_t init = 0;
    heap_t heap;
    int i;

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
        turn_event[i].heap_node = heap_insert(&heap, &turn_event[i]);
    }

    while((t = heap_remove_min((&heap))) && dungeon->num_mons > 0 && dungeon->player.alive){
        turn_t temp_turn;

        t->next_turn = t->next_turn + floor((double)(1000/t->speed));
        temp_turn = *t;
        heap_node_delete(&heap, t->heap_node);
        heap_decrease_key_no_replace(&heap, temp_turn.heap_node);


    }
//        p->heap_node = NULL;
//        weight = (1 + (dungeon->hardness[p->pos[0]][p->pos[1]] / 85)); // Set the weight so that if it is a room, staircase, or corridor the weight will be one and will be 1+dungeon->hardness/85 if not
//        if(tunneling){
//            dungeon->t_distances[p->pos[0]][p->pos[1]] = p->cost;
//        }
//        else{
//            dungeon->nt_distances[p->pos[0]][p->pos[1]] = p->cost;
//        }
//
//        // Up
//        if((path[p->pos[0] - 1][p->pos[1]].heap_node) &&
//           (path[p->pos[0] - 1][p->pos[1]].cost > p->cost + weight)){
//            path[p->pos[0] - 1][p->pos[1]].cost = p->cost + weight;
//            heap_decrease_key_no_replace(&heap, path[p->pos[0] - 1][p->pos[1]].heap_node);
//        }
}

void move(dungeon_t *dungeon, turn_t turn){
    if(turn.symb == PLAYER){
        return;
    }
}

/*
 * Edited version of Professor's code. Works for tunneling and non-tunneling monsters
 */
static void dijkstra_map(dungeon_t *dungeon, int tunneling){
    static corridor_path_t path[MAP_Y_MAX][MAP_X_MAX], *p;
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
            if(tunneling){
                dungeon->t_distances[y][x] = INT_MAX;
            }
            else{
                dungeon->nt_distances[y][x] = INT_MAX;
            }
        }
    }

    path[dungeon->player.y_pos][dungeon->player.x_pos].cost = 0;

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

    while((p = heap_remove_min((&heap)))){
        p->heap_node = NULL;
        weight = (1 + (dungeon->hardness[p->pos[0]][p->pos[1]] / 85)); // Set the weight so that if it is a room, staircase, or corridor the weight will be one and will be 1+dungeon->hardness/85 if not
        if(tunneling){
            dungeon->t_distances[p->pos[0]][p->pos[1]] = p->cost;
        }
        else{
            dungeon->nt_distances[p->pos[0]][p->pos[1]] = p->cost;
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
void print_path_map(int distances[MAP_Y_MAX][MAP_X_MAX], dungeon_t *dungeon, int tunneling){
    int j, i;
    for (j = 0; j < MAP_Y_MAX; j++) {
        for (i = 0; i < MAP_X_MAX; i++) {
            if(i == 0 || i == MAP_X_MAX - 1) putchar('|');
            else if(j == 0 || j == MAP_Y_MAX - 1) putchar('-');
            else if(i == dungeon->player.x_pos && j == dungeon->player.y_pos) putchar(PLAYER);
            else{
                if(tunneling){
                    printf("%d", distances[j][i] % 10);
                }
                else{
                    if(dungeon->hardness[j][i] != 0) putchar(' ');
                    else {
                        if(distances[j][i] < 0 || distances[j][i] == INT_MAX) putchar('X');
                        else printf("%d", distances[j][i] % 10);
                    }
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}