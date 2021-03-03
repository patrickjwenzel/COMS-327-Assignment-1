#define MAP_X_MAX 80
#define MAP_Y_MAX 21
#define HARDNESS_MAX 255
#define HARDNESS_MIN 0
#define UP '<'
#define DOWN '>'
#define CORRIDOR '#'
#define PLAYER '@'
#define ROOM '.'
#define NON_TUNNELING 'N'
#define TUNNELING 'M'
#define ADVENTURE  "adventure.rlg327"
#define HELLO "hello.rlg327"
#define WELL_DONE "welldone.rlg327"
#define DUNGEON "dungeon"
#define SMART 0x1
#define TELEPATHIC 0x2
#define TUNNEL 0x4
#define ERRATIC 0x8

typedef struct corridor_path {
    heap_node_t *heap_node;
    uint8_t pos[2];
    uint8_t from[2];
    int32_t cost;
} corridor_path_t;

typedef struct turn {
    heap_node_t *heap_node;
    uint32_t next_turn;
    uint32_t seq;
    char symb;
    uint8_t speed;
} turn_t;

typedef struct up_staircase{ //Up staircase
    uint8_t x_pos;
    uint8_t y_pos;
} up_t;

typedef struct down_staircase{ //Down Staircase
    uint8_t x_pos;
    uint8_t y_pos;
} down_t;

typedef struct room{ //Room
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t x_size;
    uint8_t y_size;
} room_t;

typedef struct player{ //Player character
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t alive;
    uint8_t speed;
} player_t;

typedef struct monster{ //Tunneling Monster
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t alive;
    uint8_t speed;
    uint8_t type;
    char rep;
} mon_t;

typedef struct dungeon{ //Dungeon struct
    char dmap[MAP_Y_MAX][MAP_X_MAX];
    uint8_t hardness[MAP_Y_MAX][MAP_X_MAX];
    int num_rooms;
    int nt_distances[MAP_Y_MAX][MAP_X_MAX];
    int t_distances[MAP_Y_MAX][MAP_X_MAX];
    player_t player;
    room_t *rooms;
    mon_t *mons;
    uint32_t num_mons;
    uint16_t num_down;
    uint16_t num_up;
    up_t *up_stairs;
    down_t *down_stairs;
} dungeon_t;

const char monster_reps[16] = "0123456789abcdef";

void create_dungeon_map(dungeon_t *dungeon, int num_stairs_placed[2], int save);
void fill_dungeon(dungeon_t *dungeon);
room_t newRoom(int x_pos, int y_pos, int x_size, int y_size);
int is_room(dungeon_t dungeon, int room_x_pos, int room_y_pos, int rooms[][2], int i);
void shortest_path(dungeon_t *dungeon, int src_x, int src_y, int dest_x, int dest_y, int num_stairs_placed[]);
void modified_dfs(dungeon_t *dungeon, int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_stairs_placed[]);
void load_game(dungeon_t *dungeon, int num_stairs_placed[2]);
void save_game(dungeon_t *dungeon, int num_stairs_placed[]);
void make_dungeon(dungeon_t *dungeon);
int find_room(dungeon_t *dungeon, int x, int y);
char find_stairs(dungeon_t *dungeon, int x, int y, int is_corridor);
void print_dungeon(char dungeon[MAP_Y_MAX][MAP_X_MAX]);
static int32_t corridor_path_cmp(const void *key, const void *with);
static void dijkstra_map(dungeon_t *dungeon, int tunneling);
void print_path_map(int distances[MAP_Y_MAX][MAP_X_MAX], dungeon_t *dungeon, int tunneling);
void do_maps(dungeon_t *dungeon);
void create_monsters(dungeon_t *dungeon);
void place_monsters(dungeon_t *dungeon);
static void turn_decider(dungeon_t *dungeon, turn_t turn_event[], int *init);