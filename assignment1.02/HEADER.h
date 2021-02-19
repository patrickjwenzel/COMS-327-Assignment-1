void create_dungeon_map(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int num_rooms, int numUp, int numDown, int num_stairs_placed[2], int rooms[][2], room_t room_array[], up_t up_stairs[], down_t down_stairs[], player_t *player, int save, int skip);
void fill_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX]);
void print_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX]);
room_t newRoom(int xPos, int yPos, int xSize, int ySize);
void shortest_path(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int numUp, up_t up_stairs[], int numDown, down_t downstairs[], int num_stairs_placed[]);
void modified_dfs(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int numUp, up_t up_stairs[], int numDown, down_t downstairs[], int num_stairs_placed[]);
void load_game(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], int *num_rooms, int num_stairs_placed[2], room_t room_array[], up_t up_stairs[], down_t down_stairs[], player_t *player);
void save_game(uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, int num_rooms, int num_stairs_placed[], up_t up_stairs[], down_t down_stairs[], room_t rooms[]);
void make_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX], uint8_t hardness[MAP_Y_MAX][MAP_X_MAX], player_t player, room_t rooms[], int num_rooms, up_t up_staircases[], int numUp, down_t down_staircases[], int numDown);
