#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAP_X_MAX 80
#define MAP_Y_MAX 21
#define UP '<'
#define DOWN '>'
#define CORRIDOR '#'

#include "headers.h"

struct room{
	int x_pos;
	int y_pos;
	int x_size;
	int y_size;
};

int main(){
	srand(time(NULL));
	char dungeon_map[MAP_X_MAX][MAP_Y_MAX];
	fill_dungeon(dungeon_map);
	create_dungeon_map(dungeon_map);
	print_dungeon(dungeon_map);
	return 0;
}

void create_dungeon_map(char dungeon[MAP_X_MAX][MAP_Y_MAX]) {
	int num_rooms = (rand() % (15 - 6 + 1)) + 6; //Generates between 6-15 rooms
	int num_up = (rand() % 5) + 1; //Will place 1-5 up staircases
	int num_down = (rand() % 5) + 1; //Will place 1-5 down stair cases
	int i, j = 0, x, y, room_x_pos, room_y_pos, num_rooms_placed = 0, num_cycles = 0, num_up_placed = 0, num_down_placed = 0;
	int rooms[num_rooms][2];
	struct room room_array[num_rooms];

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