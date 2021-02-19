#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <endian.h>

#define MAP_X_MAX 80
#define MAP_Y_MAX 21
#define HARDNESS_MAX 255
#define HARDNESS_MIN 0
#define UP '<'
#define DOWN '>'
#define CORRIDOR '#'


typedef struct up_staircase{ //Up staircase
    uint8_t xPos;
    uint8_t yPos;
} up_t;

typedef struct down_staircase{ //Down Staircase
    uint8_t xPos;
    uint8_t yPos;
} down_t;

typedef struct room{ //Room
	uint8_t xPos;
    uint8_t yPos;
    uint8_t xSize;
    uint8_t ySize;
} room_t;

typedef struct player{ //Player character
    uint8_t xPos;
    uint8_t yPos;
} player_t;

#include "headers.h"

int main(int argc, char *argv[]){
	srand(time(NULL));
	char dungeon_map[MAP_Y_MAX][MAP_X_MAX];
	uint8_t hardness_map[MAP_Y_MAX][MAP_X_MAX];
    int numRooms = (rand() % (15 - 6 + 1)) + 6; //Generates between 6-15 rooms
    int numUp = (rand() % 5) + 1; //Will place 1-5 up staircases
    int numDown = (rand() % 5) + 1; //Will place 1-5 down stair cases
    int numStairs[2] = {0, 0};
    int rooms[numRooms][2];
    room_t room_array[numRooms];
    up_t up_stairs[numUp];
    down_t down_stairs[numDown];
    player_t player;
    
     
  
  for(i = 0; i<argc; i++){
    if(!(strcmp(argv[i], "--s"))){
      SAVE_FILE = 1;
    }
    else if(!(strcmp(argv[i], "--l"))){
      LOAD_FILE =1;
    }
  }
    
    if(LOAD_FILE){
      load_game(dungeon_map, hardness_map, numRooms, numUp, numDown, num_stairsplaced, rooms, room_array, up_stairs, down_stairs, &player, 1, 0);
    
    }
    else{
      fill_dungeon(dungeon_map, hardness_map);
      create_dungeon_map(dungeon_map, hardness_map, numRooms, numUp, numDown, numStairs, rooms, room_array, up_stairs, down_stairs, &player, 0, 0);
    }
    if(SAVE_FILE){
      create_dungeon_map(dungeon_map, hardness_map, numRooms, numUp, numDown, numStairs, rooms, room_array, up_stairs, down_stairs, &player, 1, 1);
    }
    
    print_dungeon(dungeon_map);
    
	return 0;
}

void create_dungeon_map(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int numRooms, int numUp, int numDown, int numStairs[2], int rooms[][2], room_t room_array[], up_t up_stairs[], down_t down_stairs[], player_t *player, int save, int bool) {
	int i, j = 0, x, y, room_xPos, room_yPos, numRooms_placed = 0, num_cycles = 0, player_placed = 0;
	if(!bool){
        while(num_cycles < numRooms || numRooms_placed < numRooms){ //Will loop until all rooms have been placed
            for(i = 0; i < numRooms; i++){
                rooms[i][0]=(rand() % 12) + 4; //Makes the room between 4 and 15 units in the x direction
                rooms[i][1]=(rand() % 12) + 3; //Makes the room between 3 and 15 units in the y direction
            }

            for(i = 0; i < numRooms - numRooms_placed; i++){
                room_xPos = (rand() % (MAP_X_MAX - 1)) + 1; //Makes the x coordinate between 1 and 70 (So a border can be placed)
                room_yPos = (rand() % (MAP_Y_MAX - 1)) + 1;//Makes the x coordinate between 1 and 70 (For Border reasons)

                if(rooms[i][0] + room_xPos < MAP_X_MAX && rooms[i][1] + room_yPos < MAP_Y_MAX){ //If the coordinates of the are on the grid

                    if(!is_room(dungeon, room_xPos, room_yPos, rooms, i)){ //If the room does not collide with another room
                        room_array[numRooms_placed] = newRoom(room_xPos, room_yPos, rooms[i][0], rooms[i][1]);

                        for(y = room_yPos; y < room_yPos + rooms[i][1]; y++){//Placing the room
                            for(x = room_xPos; x < room_xPos + rooms[i][0]; x++){
                                hardness[y][x] = 0;
                                int staircase = (rand() % (100)) + 1; //Gets a number from 1-100
                                int up_or_down;
                                if(!player_placed){
                                    dungeon[y][x] = '@'; //Place a player in the first room
                                    player->xPos = x;
                                    player->yPos = y;
                                    player_placed = 1;
                                }
                                else{
                                    if(staircase >= 1 && staircase <= 3){ //If random number is 1-3, place a staircase
                                        up_or_down = (rand() % 10) + 1; //Gets a random number 1-10
                                        if(up_or_down > 5 && numStairs[0] < numUp){ //If the number is 6-10 and the max up staircases have not been placed place an up staircase
                                            dungeon[y][x] = UP;
                                            up_t up;
                                            up.xPos = x;
                                            up.yPos = y;
                                            up_stairs[numStairs[0]++] = up;
                                        }
                                        else if(up_or_down <= 5 && numStairs[1]  < numDown){ //If the number is 1-5 and the max down staircases have not been placed place a down staircase
                                            dungeon[y][x] = DOWN;
                                            down_t down;
                                            down.yPos = y;
                                            down.xPos = x;
                                            down_stairs[numStairs[1]++] = down;
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
                        numRooms_placed++;
                    }
                }
            }
            num_cycles++;
        }

        for(i = 1; i < numRooms; i++){
            //room_array[i] = src
            //room_array[j] = dest
            shortest_path(dungeon, hardness, room_array[i].xPos, room_array[i].yPos, room_array[j].xPos, room_array[j].yPos, numUp, up_stairs, numDown, down_stairs, numStairs); //Make a path from one room to the next
            j = i;
        }
	}

  	if(save){ //If we are saving, write this dungeon to disk
  	    save_game(hardness, *player, numRooms, numStairs, up_stairs, down_stairs, room_array);
  	}
	return;
}

void fill_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX]){
	int j, i;
	for(j = 0; j < MAP_Y_MAX; j++){ //Fill the dungeon with ' 's and line borders with | or -. Make border hardness 255 and other hardness between 1-254
		for(i = 0; i < MAP_X_MAX; i++){
            if (i == 0 || i == MAP_X_MAX - 1) {
                hardness[j][i] = HARDNESS_MAX;
                dungeon[j][i] = '|';
            }
            else if (j == 0 || j == MAP_Y_MAX - 1) {
                hardness[j][i] = HARDNESS_MAX;
                dungeon[j][i] = '-';
            }
            else{
                dungeon[j][i] = ' ';
                hardness[j][i] = (rand() % (254 - 1)) + 1;
            }
		}
	}
}

room_t newRoom(int xPos, int yPos, int xSize, int ySize){ //Returns a new room struct with the rooms position and size
	struct room new_room;
	new_room.xPos = xPos;
	new_room.yPos = yPos;
	new_room.xSize = xSize;
	new_room.ySize = ySize;
	return new_room;
}

int is_room(char dungeon[MAP_Y_MAX][MAP_X_MAX], int room_xPos, int room_yPos, int rooms[][2], int i){ //Checks to see if a new room will collide with an existing room
	int x, y;
	for(y = room_yPos - 1; y < room_yPos + rooms[i][1] + 1; y++){
		for(x = room_xPos - 1; x < room_xPos + rooms[i][0] + 1; x++){
			if(dungeon[y][x] == '.') return 1;
		}
	}
	return 0;
}

void shortest_path(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int numUp, up_t up_stairs[], int numDown, down_t downstairs[], int numStairs[]){ //Start of the shortest path print for the corridor
	int i;
	int visited[MAP_X_MAX];
	for(i = 0; i < MAP_X_MAX; i++){ //Set visited array to all false
		visited[i] = 0;
	}

	modified_dfs(dungeon, hardness, src_x, src_y, dest_x, dest_y, visited, numUp, up_stairs, numDown, downstairs, numStairs);
}

/*
Modified version of a Depth First Search to get the shortest path from one room to the next
*/
void modified_dfs(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int numUp, up_t up_stairs[], int numDown, down_t downstairs[], int numStairs[]){
	int i;
	visited[src_x] = 1; //Set this coordinate to visited
	if(src_x - dest_x < 0){//Destination is to the right
		for(i = src_x; i <= dest_x; i++){ 
			if(!visited[i]){
				int all_up_placed = numStairs[0] == numUp ? 1 : 0; //Check if all up staircases have been placed
				int all_down_placed = numStairs[1] == numDown ? 1 : 0; //Check if all down staircases have been placed
				int staircase = (rand() % 100) + 1;
                dungeon[src_y][i] = dungeon[src_y][i] == '@' || dungeon[src_y][i] == '.' || dungeon[src_y][i] == DOWN || dungeon[src_y][i] == UP ? dungeon[src_y][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                hardness[src_y][i] = 0; //Set hardness to 0 since it is a room, corridor, or staircase
                if(dungeon[src_y][i] == CORRIDOR && staircase <= 3){ //Checking if a staircase should be placed
                    int up_or_down = (rand() % 10) + 1;
                    if(!all_down_placed && up_or_down <= 5){ //Make a down staircase if not all have been placed
                        dungeon[src_y][i] = DOWN;
                        down_t down;
                        down.xPos = src_x;
                        down.yPos = src_y;
                        downstairs[numStairs[1]++] = down;
                    }
                    else if(!all_up_placed && up_or_down > 5){ //Make an up staircase if not all have been placed
                        dungeon[src_y][i] = UP;
                        up_t up;
                        up.xPos = src_x;
                        up.yPos = src_y;
                        up_stairs[numStairs[0]++] = up;
                    }
                }
                int src_to_dest = src_y - dest_y;
                int up_or_down = (src_to_dest < 0) ? 1 : (src_to_dest > 0) ? -1 : 0;
                dungeon[src_y + up_or_down][i] = dungeon[src_y + up_or_down][i] == '@' || dungeon[src_y + up_or_down][i] == '.' || dungeon[src_y + up_or_down][i] == DOWN || dungeon[src_y + up_or_down][i] == UP ? dungeon[src_y + up_or_down][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                hardness[src_y + up_or_down][i] = 0;
                modified_dfs(dungeon, hardness, i, src_y + up_or_down, dest_x, dest_y, visited, numUp, up_stairs, numDown, downstairs, numStairs);
            }
        }
    }
    else if(src_x - dest_x > 0){ //Destination is to the left
        for(i = src_x; i >= dest_x; i--){
            if(!visited[i]){
                int all_up_placed = numStairs[0] == numUp ? 1 : 0;
                int all_down_placed = numStairs[1] == numDown ? 1 : 0;
                int staircase = (rand() % 100) + 1;
                dungeon[src_y][i] = dungeon[src_y][i] == '@' ||  dungeon[src_y][i] == '.' || dungeon[src_y][i] == DOWN || dungeon[src_y][i] == UP ? dungeon[src_y][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                hardness[src_y][i] = 0;
                if(dungeon[src_y][i] == CORRIDOR && staircase <= 3){
                    int up_or_down = (rand() % 10) + 1;
                    if(!all_down_placed && up_or_down <= 5){
                        dungeon[src_y][i] = DOWN;
                        down_t down;
                        down.xPos = src_x;
                        down.yPos = src_y;
                        downstairs[numStairs[1]++] = down;
                    }
                    else if(!all_up_placed && up_or_down > 5){
                        dungeon[src_y][i] = UP;
                        up_t up;
                        up.xPos = src_x;
                        up.yPos = src_y;
                        up_stairs[numStairs[0]++] = up;
                    }
                }
                int src_to_dest = src_y - dest_y;
                int up_or_down = (src_to_dest < 0) ? 1 : (src_to_dest > 0) ? -1 : 0;
                dungeon[src_y + up_or_down][i] = dungeon[src_y + up_or_down][i] == '@' ||  dungeon[src_y + up_or_down][i] == '.' || dungeon[src_y + up_or_down][i] == DOWN || dungeon[src_y + up_or_down][i] == UP ? dungeon[src_y + up_or_down][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
                hardness[src_y + up_or_down][i] = 0;
                modified_dfs(dungeon, hardness, i, src_y + up_or_down, dest_x, dest_y, visited, numUp, up_stairs, numDown, downstairs, numStairs);
			}
		}
	}
    else if(src_y - dest_y > 0){ //Destination is Up
        for(i = src_y + 1; i >= dest_y; i--){
            int all_up_placed = numStairs[0] == numUp ? 1 : 0;
            int all_down_placed = numStairs[1] == numDown ? 1 : 0;
            int staircase = (rand() % 100) + 1;
            dungeon[i][src_x] = (dungeon[i][src_x] == '@' ||  dungeon[i][src_x] == '.' || dungeon[i][src_x] == DOWN || dungeon[i][src_x] == UP) ? dungeon[i][src_x] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
            hardness[i][src_x] = 0;
            if(dungeon[i][src_x] == CORRIDOR && staircase <= 3){
                int up_or_down = (rand() % 10) + 1;
                if(!all_down_placed && up_or_down <= 5){
                    dungeon[i][src_x] = DOWN;
                    down_t down;
                    down.xPos = src_x;
                    down.yPos = src_y;
                    downstairs[numStairs[1]++] = down;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon[i][src_x] = UP;
                    up_t up;
                    up.xPos = src_x;
                    up.yPos = src_y;
                    up_stairs[numStairs[0]++] = up;
                }
            }
        }
        return;
    }
    else if(src_y - dest_y < 0){ //Destination is Down
        for(i = src_y + 1; i <= dest_y; i++){
            int all_up_placed = numStairs[0] == numUp ? 1 : 0;
            int all_down_placed = numStairs[1] == numDown ? 1 : 0;
            int staircase = (rand() % 100) + 1;
            dungeon[i][src_x] = (dungeon[i][src_x] == '@' ||  dungeon[i][src_x] == '.' || dungeon[i][src_x] == DOWN || dungeon[i][src_x] == UP) ? dungeon[i][src_x] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
            hardness[i][src_x] = 0;
            if(dungeon[i][src_x] == CORRIDOR && staircase <= 3){
                int up_or_down = (rand() % 10) + 1;
                if(!all_down_placed && up_or_down <= 5){
                    dungeon[i][src_x] = DOWN;
                    down_t down;
                    down.xPos = src_x;
                    down.yPos = src_y;
                    downstairs[numStairs[1]++] = down;
                }
                else if(!all_up_placed && up_or_down > 5){
                    dungeon[i][src_x] = UP;
                    up_t up;
                    up.xPos = src_x;
                    up.yPos = src_y;
                    up_stairs[numStairs[0]++] = up;
                }
            }
        }
        return;
	}
	else return;
}

void save_game(uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, int numRooms, int numStairs[], up_t up_stairs[], down_t down_stairs[], room_t rooms[]){
    int i;
    char *home = getenv("HOME");
    char *game_dir = ".rlg327";
    char *game_file = "dungeon";
    char *path = malloc((strlen(home) + strlen(game_dir) + strlen(game_file) + 2 + 1) * sizeof(char));
    sprintf(path, "%s/%s/%s", home, game_dir, game_file);
    int numUp = numStairs[0];
    int numDown = numStairs[1];

    FILE *file;
    char header[12] = "RLG327-S2021";
    int version = 0;
    int size = 1708 + (numUp * 2) + (numDown * 2) + (numRooms * 4); //1708 is the size of all bytes besides number of staircases and number of rooms
    if(!(file = fopen(path, "w"))){
        printf("Failed to open this file\n");
    }

    fwrite(&header, sizeof(header), 1, file);
    version = htobe32(version);
    fwrite(&version, 4, 1, file);

    size = htobe32(size);
    fwrite(&size, 4, 1, file);

    fwrite(&player.xPos, 1, 1, file);
    fwrite(&player.yPos, 1, 1, file);

    fwrite(hardness, 1, 1680, file); //DON'T DO AN & FOR THIS IT WILL BREAK THE PROGRAM
    numRooms = htobe16(numRooms);
    fwrite(&numRooms, 2, 1, file);
    numRooms = be16toh(numRooms);

    for(i = 0; i < numRooms; i++){
        fwrite(&rooms[i].xPos, sizeof(rooms[i].xPos), 1, file);
        fwrite(&rooms[i].yPos, sizeof(rooms[i].yPos), 1, file);
        fwrite(&rooms[i].xSize, sizeof(rooms[i].xSize), 1, file);
        fwrite(&rooms[i].ySize, sizeof(rooms[i].ySize), 1, file);
    }

    numUp = htobe16(numUp);
    fwrite(&numUp, 2, 1, file);

    for(i = 0; i < numStairs[0]; i++){
        fwrite(&up_stairs[i].xPos, sizeof(up_stairs[i].xPos), 1, file);
        fwrite(&up_stairs[i].yPos, sizeof(up_stairs[i].yPos), 1, file);
    }

    numDown = htobe16(numDown);
    fwrite(&numDown, 2, 1, file);

    for(i = 0; i < numStairs[1]; i++){
        fwrite(&down_stairs[i].xPos, sizeof(down_stairs[i].xPos), 1, file);
        fwrite(&down_stairs[i].yPos, sizeof(down_stairs[i].yPos), 1, file);
    }

	fclose(file);
}

void load_game(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int *numRooms, int numStairs[2], room_t room_array[], up_t up_stairs[], down_t down_stairs[], player_t *player){
    FILE *file;
    int i, version, size;
    char header[12];

    char *home = getenv("HOME");
    char *game_dir = ".rlg327";
    char *game_file = "dungeon";

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

    fread(&player->xPos, 1, 1, file);
    fread(&player->yPos, 1, 1, file);

    fread(hardness, 1, 1680, file);
    fread(numRooms, 2, 1, file);
    *numRooms = be16toh(*numRooms);

    room_array = malloc(*numRooms * sizeof(room_t));

    for(i = 0; i < *numRooms; i++){
        fread(&room_array[i].xPos, sizeof(room_array[i].xPos), 1, file);
        fread(&room_array[i].yPos, sizeof(room_array[i].yPos), 1, file);
        fread(&room_array[i].xSize, sizeof(room_array[i].xSize), 1, file);
        fread(&room_array[i].ySize, sizeof(room_array[i].ySize), 1, file);
    }

    fread(&numStairs[0], 2, 1, file);
    numStairs[0] = be16toh(numStairs[0]);
    up_stairs = malloc(numStairs[0] * sizeof(up_t));

    for(i = 0; i < numStairs[0]; i++){
        fread(&up_stairs[i].xPos, sizeof(up_stairs[i].xPos), 1, file);
        fread(&up_stairs[i].yPos, sizeof(up_stairs[i].yPos), 1, file);
    }

    fread(&numStairs[1], 2, 1, file);
    numStairs[1] = be16toh(numStairs[1]);
    down_stairs = malloc(numStairs[1] * sizeof(down_t));

    for(i = 0; i < numStairs[1]; i++){
        fread(&down_stairs[i].xPos, sizeof(down_stairs[i].xPos), 1, file);
        fread(&down_stairs[i].yPos, sizeof(down_stairs[i].yPos), 1, file);
    }
    fclose(file);
    
    
    
     for(j = 0; j < MAP_Y_MAX; j++){
        for(i = 0; i < MAP_X_MAX; i++){
            
            if(hardness[j][i] == 255){
            
              if(j == 0 || j == MAP_Y_MAX-1)
              {
                dungeon[j][i] = '-';
              }
              else
              {
                dungeon[j][i] = '|';
              }

            else if(hardness[j][i] == 0){
                dungeon[j][i] = '#';
            }
            else{ //Otherwise, print a space
                dungeon[j][i] = ' ';
            }
        }
    }
    
    
    
          

  for(k = 0; k<numRooms; k++){
    for(j=roomArray[k].yPos; j<roomArray[k].yPos+roomArray[k].ySize; j++){
      for(i=roomArray[k].xPos; i<roomArray[k].xPos+roomArray[k].xSize; i++){
     
	      dungeon[j][i] = '.';
      }
    }
  }
  
  for(i = 0; i<numUp; i++){
    dungeon[up_staircases[i].yPos][up_staircases[i].xPos] = '<';
  }

  for(i =0; i<numDown; i++){
     dungeon[down_staircases[i].yPos][down_staircases[i].xPos] = '>';
  }
    dungeon[player.yPos][player.xPos] = '@';
}

void make_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, room_t rooms[], int numRooms, up_t up_staircases[], int numUp, down_t down_staircases[], int numDown){
    int j, i, index;
    for(j = 0; j < MAP_Y_MAX; j++){
        for(i = 0; i < MAP_X_MAX; i++){
            if(player.xPos == i && player.yPos == j){ //If this is the player, set the player here
                dungeon[j][i] = '@';
            }
            else if(hardness[j][i] == 255){
                dungeon[j][i] = (j == 0 || j == MAP_Y_MAX - 1) ? '-' : '|'; //If it is a left/right wall make it |, otherwise -
            }
            else if(hardness[j][i] == 0){
                index = find_room(rooms, numRooms, i, j); //See if this is a room
                if(!(index == -1)){ //If so, see if there are any staircases
                    dungeon[j][i] = find_stairs(up_staircases, numUp, down_staircases, numDown, i, j, 0);
                }
                else{ //If not, it is a corridor, also check for staircases
                    dungeon[j][i] = find_stairs(up_staircases, numUp, down_staircases, numDown, i, j, 1);
                }
            }
            else{ //Otherwise, print a space
                dungeon[j][i] = ' ';
            }
        }
    }
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

