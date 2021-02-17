#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <endian.h>

/* Valid switches to main are:
 * --save Save dungeon
 * --load Load dungeon
*/

#define MAP_X_MAX 80
#define MAP_Y_MAX 21
#define HARDNESS_MAX 255
#define HARDNESS_MIN 0
#define UP '<'
#define DOWN '>'
#define CORRIDOR '#'
#define PLAYER '@'

typedef struct up_staircase{
    u_int8_t x_pos;
    u_int8_t y_pos;
} up_t;

typedef struct down_staircase{
    u_int8_t x_pos;
    u_int8_t y_pos;
} down_t;

typedef struct room{
	u_int8_t x_pos;
    u_int8_t y_pos;
    u_int8_t x_size;
    u_int8_t y_size;
} room_t;

typedef struct player{
    u_int8_t x_pos;
    u_int8_t y_pos;
} player_t;

#include "headers.h"

int main(int argc, char *argv[]){
	srand(time(NULL));
	char dungeon_map[MAP_Y_MAX][MAP_X_MAX];
	u_int8_t hardness_map[MAP_Y_MAX][MAP_X_MAX];

	if(argc > 2){
	    printf("Usage: %s --save|--load\n", argv[0]);
	    return -1;
	}
	if(argc == 1){
        printf("Hello\n");
        fill_dungeon(dungeon_map, hardness_map);
        create_dungeon_map(dungeon_map, hardness_map);
        print_dungeon(dungeon_map);
    }
	else if(!strcmp(argv[1], "--save")){
	    //save_game();
	}
	else if(!strcmp(argv[1], "--load")){
	    load_game();
	}
	else if(strcmp(argv[1], "--save") || strcmp(argv[1], "--load")){
        printf("Usage: %s --save|--load\n", argv[0]);
        return -1;
	}
	return 0;
}

void create_dungeon_map(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX]) {
	int num_rooms = (rand() % (15 - 6 + 1)) + 6; //Generates between 6-15 rooms
	int num_up = (rand() % 5) + 1; //Will place 1-5 up staircases
	int num_down = (rand() % 5) + 1; //Will place 1-5 down stair cases
	int i, j = 0, x, y, room_x_pos, room_y_pos, num_rooms_placed = 0, num_cycles = 0, num_up_placed = 0, num_down_placed = 0, player_placed = 0;
	int rooms[num_rooms][2];
    room_t room_array[num_rooms];

  	while(num_cycles < num_rooms || num_rooms_placed < num_rooms){ //Will loop until all rooms have been placed
  		for(i = 0; i < num_rooms; i++){
  			rooms[i][0]=(rand() % 12) + 4; //Makes the room between 4 and 15 units in the x direction
  			rooms[i][1]=(rand() % 12) + 3; //Makes the room between 3 and 15 units in the y direction
  		}
  	
  		for(i = 0; i < num_rooms - num_rooms_placed; i++){
  			room_x_pos = (rand() % (MAP_X_MAX - 1)) + 1; //Makes the x coordinate between 1 and 70 (So a border can be placed)
  			room_y_pos = (rand() % (MAP_Y_MAX - 1)) + 1;//Makes the x coordinate between 1 and 70 (For Border reasons)

  			if(rooms[i][0] + room_x_pos < MAP_X_MAX && rooms[i][1] + room_y_pos < MAP_Y_MAX){ //If the coordinates of the are on the grid

  				if(!is_room(dungeon, room_x_pos, room_y_pos, rooms, i)){ //If the room does not collide with another room
	  				room_array[num_rooms_placed] = newRoom(room_x_pos, room_y_pos, rooms[i][0], rooms[i][1]);

					for(y = room_y_pos; y < room_y_pos + rooms[i][1]; y++){//Placing the room
						for(x = room_x_pos; x < room_x_pos + rooms[i][0]; x++){
						    hardness[y][x] = 0;
							int staircase = (rand() % (100)) + 1; //Gets a number from 1-100
							int up_or_down;
							if(!player_placed){
							    dungeon[y][x] = PLAYER;
							    player_placed = 1;
							}
							else{
                                if(staircase >= 1 && staircase <= 3){ //If random number is 1-3, place a staircase
                                    up_or_down = (rand() % 10) + 1; //Gets a random number 1-10
                                    if(up_or_down > 5 && num_up_placed < num_up){ //If the number is 6-10 and the max up staircases have not been placed place an up staircase
                                        dungeon[y][x] = UP;
                                        num_up_placed++;
                                    }
                                    else if(up_or_down <= 5 && num_down_placed < num_down){ //If the number is 1-5 and the max down staircases have not been placed place a down staircase
                                        dungeon[y][x] = DOWN;
                                        num_down_placed++;
                                    }
                                    else{ //Otherwise, make it a room
                                        dungeon[y][x] = '.';
                                    }
                                }
                                else {
                                    dungeon[y][x] = '.';
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

  	for(i = 1; i < num_rooms; i++){
		//room_array[i] = src
		//room_array[j] = dest
		shortest_path(dungeon, hardness, room_array[i].x_pos, room_array[i].y_pos, room_array[j].x_pos, room_array[j].y_pos, num_up, num_up_placed, num_down, num_down_placed); //Make a path from one room to the next
		j = i;
  	}  
	return;
}

void fill_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX]){
	int j, i;
	for(j = 0; j < MAP_Y_MAX; j++){ //Fill the dungeon with *'s to start and all hardness values of 255
		for(i = 0; i < MAP_X_MAX; i++){
			dungeon[j][i] = '*';
			hardness[j][i] = 255;
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

int is_room(char dungeon[MAP_Y_MAX][MAP_X_MAX], int room_x_pos, int room_y_pos, int rooms[][2], int i){ //Checks to see if a new room will collide with an existing room
	int x, y;
	for(y = room_y_pos - 1; y < room_y_pos + rooms[i][1] + 1; y++){
		for(x = room_x_pos - 1; x < room_x_pos + rooms[i][0] + 1; x++){
			if(dungeon[y][x] == '.') return 1;
		}
	}
	return 0;
}

void shortest_path(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int num_up, int num_up_placed, int num_down, int num_down_placed){ //Start of the shortest path print for the corridor
	int i;
	int visited[MAP_X_MAX];
	for(i = 0; i < MAP_X_MAX; i++){ //Set visited array to all false
		visited[i] = 0;
	}

	modified_dfs(dungeon, hardness, src_x, src_y, dest_x, dest_y, visited, num_up, num_up_placed, num_down, num_down_placed);
}

/*
Modified version of a Depth First Search to get the shortest path from one room to the next
*/
void modified_dfs(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_up, int num_up_placed, int num_down, int num_down_placed){
	int i;
	visited[src_x] = 1; //Set this coordinate to visited
	hardness[src_y][src_x] = 0; //Since it is a corrdior or staircase, set hardness to 0
	if(src_x - dest_x < 0){//Destination is to the right
		for(i = src_x; i <= dest_x; i++){ 
			if(!visited[i]){
				int all_up_placed = num_up_placed == num_up ? 1 : 0; //Check if all up staircases have been placed
				int all_down_placed = num_down_placed == num_down ? 1 : 0; //Check if all down staircases have been placed
				int staircase = (rand() % 100) + 1;
                dungeon[src_y][i] = dungeon[src_y][i] == PLAYER || dungeon[src_y][i] == '.' || dungeon[src_y][i] == DOWN || dungeon[src_y][i] == UP ? dungeon[src_y][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                if(dungeon[src_y][i] == CORRIDOR && staircase <= 3){ //Checking if a staircase should be placed
                    int up_or_down = (rand() % 10) + 1;
                    if(!all_down_placed && up_or_down <= 5){ //Make a down staircase if not all have been placed
                        dungeon[src_y][i] = DOWN;
                        num_down_placed++;
                    }
                    else if(!all_up_placed && up_or_down > 5){ //Make an up staircase if not all have been placed
                        dungeon[src_y][i] = UP;
                        num_up_placed++;
                    }
                }
                int src_to_dest = src_y - dest_y;
                int up_or_down = (src_to_dest < 0) ? 1 : (src_to_dest > 0) ? -1 : 0;
                dungeon[src_y + up_or_down][i] = dungeon[src_y + up_or_down][i] == PLAYER || dungeon[src_y + up_or_down][i] == '.' || dungeon[src_y + up_or_down][i] == DOWN || dungeon[src_y + up_or_down][i] == UP ? dungeon[src_y + up_or_down][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                modified_dfs(dungeon, hardness, i, src_y + up_or_down, dest_x, dest_y, visited, num_up, num_up_placed, num_down, num_down_placed);
            }
        }
    }
    else if(src_x - dest_x > 0){ //Desination is to the left
        for(i = src_x; i >= dest_x; i--){
            if(!visited[i]){
                int all_up_placed = num_up_placed == num_up ? 1 : 0;
                int all_down_placed = num_down_placed == num_down ? 1 : 0;
                int staircase = (rand() % 100) + 1;
                dungeon[src_y][i] = dungeon[src_y][i] == PLAYER ||  dungeon[src_y][i] == '.' || dungeon[src_y][i] == DOWN || dungeon[src_y][i] == UP ? dungeon[src_y][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                if(dungeon[src_y][i] == CORRIDOR && staircase <= 3){
                    int up_or_down = (rand() % 10) + 1;
                    if(!all_down_placed && up_or_down <= 5){
                        dungeon[src_y][i] = DOWN;
                        num_down_placed++;
                    }
                    else if(!all_up_placed && up_or_down > 5){
                        dungeon[src_y][i] = UP;
                        num_up_placed++;
                    }
                }
                int src_to_dest = src_y - dest_y;
                int up_or_down = (src_to_dest < 0) ? 1 : (src_to_dest > 0) ? -1 : 0;
                dungeon[src_y + up_or_down][i] = dungeon[src_y + up_or_down][i] == PLAYER ||  dungeon[src_y + up_or_down][i] == '.' || dungeon[src_y + up_or_down][i] == DOWN || dungeon[src_y + up_or_down][i] == UP ? dungeon[src_y + up_or_down][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                modified_dfs(dungeon, hardness, i, src_y + up_or_down, dest_x, dest_y, visited, num_up, num_up_placed, num_down, num_down_placed);
			}
		}
	}
    else if(src_y - dest_y > 0){ //Destination is Up
        for(i = src_y + 1; i >= dest_y; i--){
            int all_up_placed = num_up_placed == num_up ? 1 : 0;
            int all_down_placed = num_down_placed == num_down ? 1 : 0;
            int staircase = (rand() % 100) + 1;
            dungeon[i][src_x] = (dungeon[i][src_x] == PLAYER ||  dungeon[i][src_x] == '.' || dungeon[i][src_x] == DOWN || dungeon[i][src_x] == UP) ? dungeon[i][src_x] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
            if(dungeon[i][src_x] == CORRIDOR && staircase <= 3){
                int up_or_down = (rand() % 10) + 1;
                if(!all_down_placed && up_or_down <= 5){
                    dungeon[i][src_x] = DOWN;
                    num_down_placed++;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon[i][src_x] = UP;
                    num_up_placed++;
                }
            }
        }
        return;
    }
    else if(src_y - dest_y < 0){ //Destination is Down
        for(i = src_y + 1; i <= dest_y; i++){
            int all_up_placed = num_up_placed == num_up ? 1 : 0;
            int all_down_placed = num_down_placed == num_down ? 1 : 0;
            int staircase = (rand() % 100) + 1;
            dungeon[i][src_x] = (dungeon[i][src_x] == PLAYER ||  dungeon[i][src_x] == '.' || dungeon[i][src_x] == DOWN || dungeon[i][src_x] == UP) ? dungeon[i][src_x] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
            if(dungeon[i][src_x] == CORRIDOR && staircase <= 3){
                int up_or_down = (rand() % 10) + 1;
                if(!all_down_placed && up_or_down <= 5){
                    dungeon[i][src_x] = DOWN;
                    num_down_placed++;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon[i][src_x] = UP;
                    num_up_placed++;
                }
            }
        }
        return;
	}
	else return;
}

void save_game(char *file_path, char dungeon[MAP_Y_MAX][MAP_X_MAX], int num_rooms, int num_up_placed, int num_down_placed){
    FILE *f = fopen(file_path, "w");
	fclose(f);
}

void load_game(){
    FILE *file;
    char dungeon[MAP_Y_MAX][MAP_X_MAX];
    u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX];
    int num_rooms, i, version, size, num_up, num_down;
    player_t player;
    char header[12];

    char *home = getenv("HOME");
    char *game_dir = ".rlg327";
    char *game_file = "welldone.rlg327";

    char *path = malloc((strlen(home) + strlen(game_dir) + strlen(game_file) + 2 + 1) * sizeof(char));
    sprintf(path, "%s/%s/%s", home, game_dir, game_file);

    if(!(file = fopen(path, "r"))){
        printf("Failed to open this file");
    }
    fread(&header, sizeof(header), 1, file);
    fread(&version, 4, 1, file);
    version = be32toh(version);

    fread(&size, 4, 1, file);
    size = be32toh(size);

    fread(&player.x_pos, 1, 1, file);
    fread(&player.y_pos, 1, 1, file);

    fread(&hardness, 1, 1680, file);

    fread(&num_rooms, 2, 1, file);
    num_rooms = be16toh(num_rooms);

    room_t rooms[num_rooms];

    for(i = 0; i < num_rooms; i++){
        fread(&rooms[i].x_pos, sizeof(rooms[i].x_pos), 1, file);
        fread(&rooms[i].y_pos, sizeof(rooms[i].y_pos), 1, file);
        fread(&rooms[i].x_size, sizeof(rooms[i].x_size), 1, file);
        fread(&rooms[i].y_size, sizeof(rooms[i].y_size), 1, file);
    }

    fread(&num_up, 2, 1, file);
    num_up = be16toh(num_up);
    up_t up_staircases[num_up];

    for(i = 0; i < num_up; i++){
        fread(&up_staircases[i].x_pos, sizeof(up_staircases[i].x_pos), 1, file);
        fread(&up_staircases[i].y_pos, sizeof(up_staircases[i].y_pos), 1, file);
    }

    fread(&num_down, 2, 1, file);
    num_down = be16toh(num_down);
    down_t down_staircases[num_down];

    for(i = 0; i < num_down; i++){
        fread(&down_staircases[i].x_pos, sizeof(down_staircases[i].x_pos), 1, file);
        fread(&down_staircases[i].y_pos, sizeof(down_staircases[i].y_pos), 1, file);
    }
    fclose(file);

    make_dungeon(dungeon, hardness, player, rooms, num_rooms, up_staircases, num_up, down_staircases, num_down);
    print_dungeon(dungeon);
}

void make_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, room_t rooms[], int num_rooms, up_t up_staircases[], int num_up, down_t down_staircases[], int num_down){
    int j, i, index;
    for(j = 0; j < MAP_Y_MAX; j++){
        for(i = 0; i < MAP_X_MAX; i++){
            if(player.x_pos == i && player.y_pos == j){
                dungeon[j][i] = PLAYER;
            }
            else if(hardness[j][i] == 255){
                dungeon[j][i] = '*';
            }
            else if(hardness[j][i] == 0){
                index = find_room(rooms, num_rooms, i, j);
                if(!(index == -1)){
                    dungeon[j][i] = find_stairs(up_staircases, num_up, down_staircases, num_down, i, j, 0);
                }
                else{
                    dungeon[j][i] = find_stairs(up_staircases, num_up, down_staircases, num_down, i, j, 1);
                }
            }
            else{
                dungeon[j][i] = ' ';
            }
        }
    }
}

char find_stairs(up_t up_staircases[], int num_up, down_t down_staircases[], int num_down, int x, int y, int is_corridor){
    int i;
    for(i = 0; i < num_up; i++){
        if(up_staircases[i].x_pos == x && up_staircases[i].y_pos == y) return UP;
    }
    for(i = 0; i < num_down; i++){
        if(down_staircases[i].x_pos == x && down_staircases[i].y_pos == y) return DOWN;
    }
    return is_corridor ? CORRIDOR : '.';
}

int find_room(room_t rooms[], int num_rooms, int x, int y){
    int i;
    for(i = 0; i < num_rooms; i++){
        if(x >= rooms[i].x_pos && x < (rooms[i].x_pos + rooms[i].x_size) && y >= rooms[i].y_pos && (y < rooms[i].y_pos + rooms[i].y_size)) return i;
    }
    return -1;
}

void print_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX]){
    int j, i;
    for (j = 0; j < MAP_Y_MAX; j++) {
        for (i = 0; i < MAP_X_MAX; i++) {
            if (dungeon[j][i] == '*') {
                if (i == 0 || i == MAP_X_MAX - 1) printf("%1c", '|'); //If it is a side, print a wall
                else if (j == 0 || j == MAP_Y_MAX - 1) printf("%1c", '-'); //If it is a side, print a wall
                else printf("%1c", ' '); //If not a side, print a space
            }
            else printf("%1c", dungeon[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}