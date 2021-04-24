void create_dungeon_map(char dungeon[MAP_X_MAX][MAP_Y_MAX]);
void fill_dungeon(char dungeon[MAP_X_MAX][MAP_Y_MAX]);
void print_dungeon(char dungeon[MAP_X_MAX][MAP_Y_MAX]);
struct room newRoom(int x_pos, int y_pos, int x_size, int y_size);
int is_room(char dungeon[MAP_X_MAX][MAP_Y_MAX], int room_x_pos, int room_y_pos, int rooms[][2], int i);
void shortest_path(char dungeon[MAP_X_MAX][MAP_Y_MAX], int src_x, int src_y, int dest_x, int dest_y, int num_up, int num_up_placed, int num_down, int num_down_placed);
void modified_dfs(char dungeon[MAP_X_MAX][MAP_Y_MAX], int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_up, int num_up_placed, int num_down, int num_down_placed);
