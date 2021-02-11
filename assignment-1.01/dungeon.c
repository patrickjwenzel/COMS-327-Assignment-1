#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_X_MAX 80
#define MAP_Y_MAX 21

void dungeon();
void fill_dungeon(char dungeon[MAP_X_MAX][MAP_Y_MAX]);
void print_dungeon(char dungeon[MAP_X_MAX][MAP_Y_MAX]);
struct room newRoom(int x_pos, int y_pos, int x_size, int y_size);

struct room{
	int x_pos; //uper left corrner
	int y_pos;
	int x_size;
	int y_size;
};

int main(){
	srand(time(NULL));
	dungeon();
	return 0;
}



void dungeon() {
	int num_rooms = (rand() % (11 - 6 + 1)) + 6;
	int num_stairs = (rand() % (9 - 1 + 1)) + 1;
	int i, j, k, x, y, count, room_size_x, room_size_y, room_x_pos, room_y_pos, connect_room, x_move, y_move, num_rooms_placed = 0, num_cycles = 0;
	char dungeon[MAP_X_MAX][MAP_Y_MAX];
	int rooms[num_rooms][2];
	struct room room_array[num_rooms];

	fill_dungeon(dungeon);

  	while(num_cycles < num_rooms || num_rooms_placed < num_rooms){
  		for(i = 0; i < num_rooms; i++){
  			rooms[i][0]=(rand() % 5) + 4;
  			rooms[i][1]=(rand() % 5) + 3;
  		}
  	
  		for(i = 0; i < num_rooms - num_rooms_placed; i++){
  			room_x_pos = (rand() % MAP_X_MAX) + 1;
  			room_y_pos = (rand() % MAP_Y_MAX) + 1;
  			if(rooms[i][0] + room_x_pos < MAP_X_MAX && rooms[i][1] + room_y_pos < MAP_Y_MAX){
  				int is_room = 0;
				for(x = room_x_pos - 1; x < room_x_pos + rooms[i][0] + 1; x++){
					for(y = room_y_pos - 1; y < room_y_pos + rooms[i][1] + 1; y++){
						if(dungeon[x][y] == '.') is_room = 1;
					}
				}
  			
  				if(!is_room){
	  				room_array[num_rooms_placed] = newRoom(room_x_pos, room_y_pos, rooms[i][0], rooms[i][1]);
					for(x = room_x_pos; x < room_x_pos + rooms[i][0]; x++){
						for(y = room_y_pos; y < room_y_pos + rooms[i][1]; y++){
							dungeon[x][y]='.';
						}
					}
	  				num_rooms_placed++;
  				}
  			}
  		}
		num_cycles++;
  	}

//   	for(count = 0; count < num_rooms; count++){

// 	  	connect_room = connect_room == num_rooms ? 0 : count + 1;
	
// 	    x_move = room_array[connect_room].x_pos - room_array[count].x_pos;
// 	  	y_move = room_array[connect_room].y_pos - room_array[count].y_pos;

// 	  	if(x_move > 0){ // The room to connect to is to the right
// 			for(i = room_array[count].x_pos; i <= room_array[connect_room].x_pos; i++){
// 				if(dungeon[i][room_array[count].y_pos] != '.') dungeon[i][room_array[count].y_pos] = '#';
// 			}
// 			if(y_move>0){ // up
// 				for(i=room_array[count].x_pos; i<= room_array[connect_room].x_pos; i++){
// 					if(dungeon[room_array[connect_room].y_pos][i] != '.') dungeon[room_array[connect_room].y_pos][i] = '#';
// 				}
// 			}
// 			else if (y_move<0) {//down
// 				for(i=room_array[connect_room].y_pos; i<= room_array[count].y_pos; i++){
// 					if(dungeon[room_array[connect_room].x_pos][i] != '.') dungeon[room_array[connect_room].x_pos][i] = '#';
// 				}
// 			}
// 		}
//   		else if (x_move<0){ 
// 			for(i=room_array[connect_room].x_pos; i<=room_array[count].x_pos; i++){
// 				if(dungeon[i][room_array[count].y_pos] != '.') dungeon[i][room_array[count].y_pos] = '#';
// 			}
// 			if(y_move<0){ //down
// 				for(i=room_array[connect_room].y_pos; i<= room_array[count].y_pos; i++){
// 					if(dungeon[room_array[connect_room].x_pos][i] != '.') dungeon[room_array[connect_room].x_pos][i] = '#';
// 				}
// 			}
// 			else if (y_move>0) { // up
// 				for(i=room_array[count].y_pos; i<= room_array[connect_room].y_pos; i++){
// 					if(dungeon[room_array[connect_room].x_pos][i] != '.') dungeon[room_array[connect_room].x_pos][i] = '#';
// 				}
// 			}
// 		}
//   	else if(y_move>0){
// 			for(i=room_array[count].y_pos; i<=room_array[connect_room].y_pos; i++){
// 				if(dungeon[room_array[count].x_pos][i] != '.') dungeon[room_array[count].x_pos][i] = '#';
// 			}
// 		}
//   	else if (y_move<0){
// 			for(i=room_array[connect_room].y_pos; i<=room_array[count].y_pos; i++){
// 				if(dungeon[room_array[count].x_pos][i] != '.')dungeon[room_array[count].x_pos][i] = '#';
// 			}
// 		}
//   }
  
	print_dungeon(dungeon);
}

void fill_dungeon(char dungeon[MAP_X_MAX][MAP_Y_MAX]){
	int j, i;
	for(j = 0; j < MAP_X_MAX; j++){
		for(i = 0; i < MAP_Y_MAX; i++){
			dungeon[j][i] = '*';
		}
	}
}

void print_dungeon(char dungeon[MAP_X_MAX][MAP_Y_MAX]){
	int j, i;
	for(j = 0; j < MAP_Y_MAX; j++){
		for(i = 0; i < MAP_X_MAX; i++){
			if(dungeon[i][j] == '*') printf("%1c", ' ');
			else printf("%1c", dungeon[i][j]);			
	  }
	  printf("\n");
	}
}

struct room newRoom(int x_pos, int y_pos, int x_size, int y_size){
	struct room new_room;
	new_room.x_pos = x_pos;
	new_room.y_pos = y_pos;
	new_room.x_size = x_size;
	new_room.y_size = y_size;
	return new_room;
}