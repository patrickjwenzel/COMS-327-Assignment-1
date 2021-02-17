void create_dungeon_map(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX]);
void fill_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX]);
void print_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX]);
room_t newRoom(int x_pos, int y_pos, int x_size, int y_size);
int is_room(char dungeon[MAP_Y_MAX][MAP_X_MAX], int room_x_pos, int room_y_pos, int rooms[][2], int i);
void shortest_path(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int num_up, int num_up_placed, int num_down, int num_down_placed);
void modified_dfs(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_up, int num_up_placed, int num_down, int num_down_placed);
void load_game();
void save_game(char *file_path, char dungeon[MAP_Y_MAX][MAP_X_MAX], int num_rooms, int num_up_placed, int num_down_placed);
void make_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, room_t rooms[], int num_rooms, up_t up_staircases[], int num_up, down_t down_staircases[], int num_down);
int find_room(room_t rooms[], int num_rooms, int x, int y);
char find_stairs(up_t up_staircases[], int num_up, down_t down_staircases[], int num_down, int x, int y, int is_corridor);

