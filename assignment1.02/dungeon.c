#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <endian.h>
#include "headers.h"


/*
 * Functionality in this code from assignment 1.01 is credited
 * to my teammate Patrick Wenzel, pwenzel@iastate.edu. All other
 * functionality specific to assignment 1.02 is my own.
 *
 */



/*

 * All I need to do is write the code for writing and saving.
 * ANd then integrate it into the main function 

 *for save, I just need to take the array of rooms, rock hardness array, and 
 */

#define MAP_X_MAX 80
#define MAP_Y_MAX 21
#define UP '<'
#define DOWN '>'
#define CORRIDOR '#'
#define MAX_HARD 255
#define MIN_HARD 0
#define LOAD_FILE 0
#define SAVE_FILE 0


struct room{
	uint_8 xPos;
	uint_8 yPos;
	uint_8 xSize;
	uint_8 ySize;
};

int main(){
  srand(time(NULL));
  int i;


  for(i = 0; i<argc; i++){
    if(!(strcmp(argv[i], "--s"))){
      SAVE_FILE = 1;
    }
    else if(!(strcmp(argv[i], "--l"))){
      LOAD_FILE =1;
    }
  }
	
	      


 
  char dungeon_map[MAP_X_MAX][MAP_Y_MAX];
  uint_8 hardnessMap[MAP_X_MAX][MAP_Y_MAX];
  
  int numRooms = (rand() % (15 - 6 + 1)) + 6; //Generates between 6-15 rooms
  int numUp = (rand() % 5) + 1; //Will place 1-5 up staircases
  int numDown = (rand() % 5) + 1; //Will place 1-5 down stair cases
  int i, j, k = 0, x, y, rxPos, roomyPos, numrPlaced = 0, numCycles = 0, numUp = 0, numDown = 0;
  int rooms[num_rooms][2];

  
  if(LOAD_FILE){
    loadDungeon(dungeonMap, hardnessMap, numRooms);
  }
  else{
    struct room room_array[numRooms];
    fill_dungeon(dungeon_map);
    create_dungeon_map(dungeon_map);
  }
  print_dungeon(dungeon_map);

  if(SAVE_FILE){

  }
  return 0;
}

int saveDungeon(){

  char*home = gentenv("HOME");
  char *gam_dir = ".rlg327";
  char *save_file = "dungeon";
  char *path = malloc((strlen(home)+strlen(game_dir)+strlen(save_file)+3)*sizeof(char));

  sprintf(path, "%s/%s/%s", home, game_dir, save_file);

  if(!(fopen(path, "w"))){
    fprintf(stderr, "Failed to open file for saving");
    return -1;
  }

  
  /*
   * Now I need to write the hardness array, the 
   *
   *
   */ 

}


int loadDungeon(){
  FILE *file;
  int i;
  int version;
  int size;
  char header[12];
  
  char*home = gentenv("HOME");
  char *gam_dir = ".rlg327";
  char *save_file = "dungeon";
  char *path = malloc((strlen(home)+strlen(game_dir)+strlen(save_file)+3)*sizeof(char));

  sprintf(path, "%s/%s/%s", home, game_dir, save_file);

  if(!(fopen(path, "r"))){
    fprintf(stderr, "Failed to open file for saving");
    return -1;
  }
  fread(&header, sizeof(header), 1, file);
  fread(&version, 4, 1, file);
  version = be32toh(version);

  fread(&size, 4, 1, file);
  size = be32toh(size);
  fread(&player->xPos, 1, 1, file);                            // redo
  fread(&player->y_pos, 1, 1, file);                            // redo
  fread(hardness, 1, 1680, file);
  fread(numRooms, 2, 1, file);
  *numRooms = be16toh(*numRooms);
  roomArray = malloc(*numRooms * sizeof(room_t));                // redo

  for(i = 0; i<*numROoms; i++){
    fread(&room_array[i].xPos, sizeof(room_array[i].xPos), 1, file);
    fread(&room_array[i].yPos, sizeof(room_array[i].yPos), 1, file);
    fread(&room_array[i].xSize, sizeof(room_array[i].xSize), 1, file);
    fread(&room_array[i].ySize, sizeof(room_array[i].ySize), 1, file);
  }

  fread(&numStairs[0],2,1,file);
  numStairs[0] = be16toh(num_stairs_placed[0]);
  upStairs = malloc(numStairs[0] * sizeof(up_t));

  for(i = 0; i<numStairs[0]; i++){
    fread(&upStairs[i].xPos, sizeof(upStairs[i].xPos), 1, file);
    fread(&upStairs[i].yPos, sizeof(upStairs[i].yPos), 1, file);
  }

  fread(&numStairs[1],2,1,file);
  numStairs[1] = be16toh(num_stairs_placed[0]);
  downStairs = malloc(numStairs[1] * sizeof(up_t));

  for(i = 0; i<numStairs[1]; i++){
    fread(&downStairs[i].xPos, sizeof(downStairs[i].xPos), 1, file);
    fread(&downStairs[i].yPos, sizeof(downStairs[i].yPos), 1, file);
  }
  
  
  
  fclose(file);


  for(i = 0; i<MAP_X_MAX; i++){
    for(j = 0; j<MAP_Y_MAX; j++){
      if(hardness[j][i] == 255){
	if(j == 0 || j == MAP_Y_MAX-1){
	  dungeon[j][i] = '-';
	}
	else{
	  dungeon[j][i] = '|';
	}
	  
      }

      else if(!hardness[j][i]){
	  dungeon[j][i] ='#';
      }
      else{
	dungeon[j][i] = ' ';	
      }
    }
  }

  for(k = 0; k<numRooms; k++){
    for(i=roomArray[k].xPos; i<roomAraay[k].xPos+roomArray[k].xSize; i++){
      for(j=roomArray[k].yPos; j<roomAraay[k].yPos+roomArray[k].ySize; j++){
      //   roomArra
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
}
  


/*
 * Need to change up create to take random and num rooms beforehand 

 *
 */

void create_dungeon_map(char dungeon[MAP_X_MAX][MAP_Y_MAX]) {


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

					for(x = room_x_pos; x < room_x_pos + rooms[i][0]; x++){//Placing the room
						for(y = room_y_pos; y < room_y_pos + rooms[i][1]; y++){
							int staircase = (rand() % (100)) + 1; //Gets a number from 1-100
							int up_or_down;
							if(staircase >= 1 && staircase <= 3){ //If random number is 1-3, place a staircase
								up_or_down = (rand() % 10) + 1; //Gets a random number 1-10
								if(up_or_down > 5 && num_up_placed < num_up){ //If the number is 6-10 and the max up staircases have not been placed place an up staircase
									dungeon[x][y] = UP;
									num_up_placed++;
								}
								else if(up_or_down <= 5 && num_down_placed < num_down){ //If the number is 1-5 and the max down staircases have not been placed place a down staircase
									dungeon[x][y] = DOWN;
									num_down_placed++;
								}
								else{ //Otherwise, make it a room
									dungeon[x][y] = '.';
								}
							}
							else {
								dungeon[x][y] = '.';
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
		shortest_path(dungeon, room_array[i].x_pos, room_array[i].y_pos, room_array[j].x_pos, room_array[j].y_pos, num_up, num_up_placed, num_down, num_down_placed); //Make a path from one room to the next
		j = i;
  	}  
	return;
}

void fill_dungeon(char dungeon[MAP_X_MAX][MAP_Y_MAX]){
	int j, i;
	for(j = 0; j < MAP_X_MAX; j++){ //Fill the dungeon with *'s to start
		for(i = 0; i < MAP_Y_MAX; i++){
			dungeon[j][i] = '*';
		}
	}
}

void print_dungeon(char dungeon[MAP_X_MAX][MAP_Y_MAX]){
	int j, i;
	for(j = 0; j < MAP_Y_MAX; j++){
		for(i = 0; i < MAP_X_MAX; i++){
			if(dungeon[i][j] == '*'){
				if(i == 0 || i == MAP_X_MAX - 1) printf("%1c", '|'); //If it is a side, print a wall
				else if(j == 0 || j == MAP_Y_MAX - 1) printf("%1c", '-'); //If it is a side, print a wall
				else printf("%1c", ' '); //If not a side, print a space
			}
			else printf("%1c", dungeon[i][j]);			
	  }
	  printf("\n");
	}
}

struct room newRoom(int x_pos, int y_pos, int x_size, int y_size){ //Returns a new room struct with the rooms position and size
	struct room new_room;
	new_room.x_pos = x_pos;
	new_room.y_pos = y_pos;
	new_room.x_size = x_size;
	new_room.y_size = y_size;
	return new_room;
}

int is_room(char dungeon[MAP_X_MAX][MAP_Y_MAX], int room_x_pos, int room_y_pos, int rooms[][2], int i){ //Checks to see if a new room will collide with an existing room
	int x, y;
	for(x = room_x_pos - 1; x < room_x_pos + rooms[i][0] + 1; x++){
		for(y = room_y_pos - 1; y < room_y_pos + rooms[i][1] + 1; y++){
			if(dungeon[x][y] == '.') return 1;
		}
	}
	return 0;
}

void shortest_path(char dungeon[MAP_X_MAX][MAP_Y_MAX], int src_x, int src_y, int dest_x, int dest_y, int num_up, int num_up_placed, int num_down, int num_down_placed){ //Start of the shortest path print for the corridor
	int i;
	int visited[MAP_X_MAX];
	for(i = 0; i < MAP_X_MAX; i++){ //Set visited array to all false
		visited[i] = 0;
	}

	modified_dfs(dungeon, src_x, src_y, dest_x, dest_y, visited, num_up, num_up_placed, num_down, num_down_placed);
}

/*
Modified version of a Depth First Search to get the shortest path from one room to the next
*/
void modified_dfs(char dungeon[MAP_X_MAX][MAP_Y_MAX], int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_up, int num_up_placed, int num_down, int num_down_placed){
	int i;
	visited[src_x] = 1; //Set this coordinate to visited
	if(src_x - dest_x < 0){//Destination is to the right
		for(i = src_x; i <= dest_x; i++){ 
			if(!visited[i]){
				int all_up_placed = num_up_placed == num_up ? 1 : 0; //Check if all up staircases have been placed
				int all_down_placed = num_down_placed == num_down ? 1 : 0; //Check if all down staircases have been placed
				int staircase = (rand() % 100) + 1;
				dungeon[i][src_y] = dungeon[i][src_y] == '.' || dungeon[i][src_y] == DOWN || dungeon[i][src_y] == UP ? dungeon[i][src_y] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
				if(dungeon[i][src_y] == CORRIDOR && staircase <= 3){ //Checking if a staircase should be placed
					int up_or_down = (rand() % 10) + 1;
					if(!all_down_placed && up_or_down <= 5){ //Make a down staircase if not all have been placed
						dungeon[i][src_y] = DOWN;
						num_down_placed++;
					}
					else if(!all_up_placed && up_or_down > 5){ //Make an up staircase if not all have been placed
						dungeon[i][src_y] = UP;
						num_up_placed++;
					}
				}
				int src_to_dest = src_y - dest_y;
				int up_or_down = (src_to_dest < 0) ? 1 : (src_to_dest > 0) ? -1 : 0;
				dungeon[i][src_y + up_or_down] = dungeon[i][src_y + up_or_down] == '.' || dungeon[i][src_y + up_or_down] == DOWN || dungeon[i][src_y + up_or_down] == UP ? dungeon[i][src_y + up_or_down] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
				modified_dfs(dungeon, i, src_y + up_or_down, dest_x, dest_y, visited, num_up, num_up_placed, num_down, num_down_placed);
			}
		}
	}
	else if(src_x - dest_x > 0){ //Desination is to the left
		for(i = src_x; i >= dest_x; i--){
			if(!visited[i]){
				int all_up_placed = num_up_placed == num_up ? 1 : 0;
				int all_down_placed = num_down_placed == num_down ? 1 : 0;
				int staircase = (rand() % 100) + 1;
				dungeon[i][src_y] = dungeon[i][src_y] == '.' || dungeon[i][src_y] == DOWN || dungeon[i][src_y] == UP ? dungeon[i][src_y] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
				if(dungeon[i][src_y] == CORRIDOR && staircase <= 3){
					int up_or_down = (rand() % 10) + 1;
					if(!all_down_placed && up_or_down <= 5){
						dungeon[i][src_y] = DOWN;
						num_down_placed++;
					}
					else if(!all_up_placed && up_or_down > 5){
						dungeon[i][src_y] = UP;
						num_up_placed++;
					}
				}
				int src_to_dest = src_y - dest_y;
				int up_or_down = (src_to_dest < 0) ? 1 : (src_to_dest > 0) ? -1 : 0;
				dungeon[i][src_y + up_or_down] = dungeon[i][src_y + up_or_down] == '.' || dungeon[i][src_y + up_or_down] == DOWN || dungeon[i][src_y + up_or_down] == UP ? dungeon[i][src_y + up_or_down] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
				modified_dfs(dungeon, i, src_y + up_or_down, dest_x, dest_y, visited, num_up, num_up_placed, num_down, num_down_placed);
			}
		}
	}
	else if(src_y - dest_y > 0){ //Destination is Up
		for(i = src_y + 1; i >= dest_y; i--){ 
			int all_up_placed = num_up_placed == num_up ? 1 : 0;
			int all_down_placed = num_down_placed == num_down ? 1 : 0;
			int staircase = (rand() % 100) + 1;
			dungeon[src_x][i] = (dungeon[src_x][i] == '.' || dungeon[src_x][i] == DOWN || dungeon[src_x][i] == UP) ? dungeon[src_x][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
			if(dungeon[src_x][i] == CORRIDOR && staircase <= 3){
				int up_or_down = (rand() % 10) + 1;
				if(!all_down_placed && up_or_down <= 5){
					dungeon[src_x][i] = DOWN;
					num_down_placed++;
				}
				else if(!all_up_placed && up_or_down > 5){
					dungeon[src_x][i] = UP;
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
			dungeon[src_x][i] = (dungeon[src_x][i] == '.' || dungeon[src_x][i] == DOWN || dungeon[src_x][i] == UP) ? dungeon[src_x][i] : CORRIDOR; //If the coordinate is not a room or staircase, print a #
			if(dungeon[src_x][i] == CORRIDOR && staircase <= 3){
				int up_or_down = (rand() % 10) + 1;
				if(!all_down_placed && up_or_down <= 5){
					dungeon[src_x][i] = DOWN;
					num_down_placed++;
				}
				else if(!all_up_placed && up_or_down > 5){
					dungeon[src_x][i] = UP;
					num_up_placed++;
				}
			}
		}
		return;
	}
	else return;
}
