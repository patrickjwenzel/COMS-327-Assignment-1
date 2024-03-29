void create_dungeon_map(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], int num_rooms, int num_up, int num_down, int num_stairs_placed[2], int rooms[][2], room_t room_array[], up_t up_stairs[], down_t down_stairs[], player_t *player, int save, int skip);
void fill_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX]);
void print_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX]);
room_t newRoom(int x_pos, int y_pos, int x_size, int y_size);
int is_room(char dungeon[MAP_Y_MAX][MAP_X_MAX], int room_x_pos, int room_y_pos, int rooms[][2], int i);
void shortest_path(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int num_up, up_t up_stairs[], int num_down, down_t downstairs[], int num_stairs_placed[]);
void modified_dfs(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_up, up_t up_stairs[], int num_down, down_t downstairs[], int num_stairs_placed[]);
void load_game(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], int *num_rooms, int num_stairs_placed[2], room_t room_array[], up_t up_stairs[], down_t down_stairs[], player_t *player);
void save_game(u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, int num_rooms, int num_stairs_placed[], up_t up_stairs[], down_t down_stairs[], room_t rooms[]);
void make_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], u_int8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, room_t rooms[], int num_rooms, up_t up_staircases[], int num_up, down_t down_staircases[], int num_down);
int find_room(room_t rooms[], int num_rooms, int x, int y);
char find_stairs(up_t up_staircases[], int num_up, down_t down_staircases[], int num_down, int x, int y, int is_corridor);
static int32_t corridor_path_cmp(const void *key, const void *with);
static void dijkstra_map(int distances[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, room_t room_array[], int num_rooms, int tunneling);
void print_path_map(int distances[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, int tunneling);
int is_connected(uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], uint8_t found_rooms[], room_t room_array[], int num_rooms, int src_y, int src_x, player_t player);
void do_maps(int distances[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], room_t room_array[], int num_rooms, player_t player);