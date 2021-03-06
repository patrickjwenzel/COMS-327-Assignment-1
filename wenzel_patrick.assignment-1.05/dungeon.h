#define MAP_X_MAX 80
#define MAP_Y_MAX 21
#define HARDNESS_MAX 255
#define HARDNESS_MIN 0
#define UP '<'
#define DOWN '>'
#define CORRIDOR '#'
#define PLAYER '@'
#define ROOM '.'
#define ROCK ' '
#define TOP '-'
#define SIDE '|'
#define FIFTEEN 0x0000000f
#define ADVENTURE  "adventure.rlg327"
#define HELLO "hello.rlg327"
#define WELL_DONE "welldone.rlg327"
#define DUNGEON "dungeon"
#define CHANCE 0x00000001
#define SMART 0x1
#define TELEPATHIC 0x2
#define TUNNEL 0x4
#define ERRATIC 0x8
#define DEBUG 0
#define USLEEP_MAX 999999
#define FPS 1

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
    int pc_location[2];
} mon_t;

typedef struct dungeon{ //Dungeon struct
    char dmap[MAP_Y_MAX][MAP_X_MAX];
    uint8_t hardness[MAP_Y_MAX][MAP_X_MAX];
    int num_rooms;
    int nt_distances[MAP_Y_MAX][MAP_X_MAX]; // Non-tunneling distance map
    int t_distances[MAP_Y_MAX][MAP_X_MAX]; // Tunneling distance map
    player_t player;
    room_t *rooms; //Array of rooms
    mon_t *mons; //Array of monsters
    uint32_t num_mons; //Number of monsters in the dungeon
    uint32_t num_mons_alive; //Number of alive monsters in the dungeon
    uint16_t num_down;
    uint16_t num_up;
    up_t *up_stairs;
    down_t *down_stairs;
} dungeon_t;

const char monster_reps[16] = "0123456789abcdef"; //Different types of monsters

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
static int turn_decider(dungeon_t *dungeon, turn_t turn_event[], int *init, int num_characters);
static int32_t turn_cmp(const void *key, const void *with);
void move(dungeon_t *dungeon, turn_t turn);
int seen(int mons[2]);
void get_next_pos(dungeon_t *dungeon, turn_t turn, int next_pos[2], int telepathic, int tunneling, int smart);
int is_character(dungeon_t *dungeon, mon_t mons[], int next_pos[2], turn_t turn);


const char *victory =
        "\n                                       o\n"
        "                                      $\"\"$o\n"
        "                                     $\"  $$\n"
        "                                      $$$$\n"
        "                                      o \"$o\n"
        "                                     o\"  \"$\n"
        "                oo\"$$$\"  oo$\"$ooo   o$    \"$    ooo\"$oo  $$$\"o\n"
        "   o o o o    oo\"  o\"      \"o    $$o$\"     o o$\"\"  o$      \"$  "
        "\"oo   o o o o\n"
        "   \"$o   \"\"$$$\"   $$         $      \"   o   \"\"    o\"         $"
        "   \"o$$\"    o$$\n"
        "     \"\"o       o  $          $\"       $$$$$       o          $  ooo"
        "     o\"\"\n"
        "        \"o   $$$$o $o       o$        $$$$$\"       $o        \" $$$$"
        "   o\"\n"
        "         \"\"o $$$$o  oo o  o$\"         $$$$$\"        \"o o o o\"  "
        "\"$$$  $\n"
        "           \"\" \"$\"     \"\"\"\"\"            \"\"$\"            \""
        "\"\"      \"\"\" \"\n"
        "            \"oooooooooooooooooooooooooooooooooooooooooooooooooooooo$\n"
        "             \"$$$$\"$$$$\" $$$$$$$\"$$$$$$ \" \"$$$$$\"$$$$$$\"  $$$\""
        "\"$$$$\n"
        "              $$$oo$$$$   $$$$$$o$$$$$$o\" $$$$$$$$$$$$$$ o$$$$o$$$\"\n"
        "              $\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""
        "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"$\n"
        "              $\"                                                 \"$\n"
        "              $\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\""
        "$\"$\"$\"$\"$\"$\"$\"$\n"
        "                                   You win!\n\n";

const char *tombstone =
        "\n\n\n\n                /\"\"\"\"\"/\"\"\"\"\"\"\".\n"
        "               /     /         \\             __\n"
        "              /     /           \\            ||\n"
        "             /____ /   Rest in   \\           ||\n"
        "            |     |    Pieces     |          ||\n"
        "            |     |               |          ||\n"
        "            |     |   A. Luser    |          ||\n"
        "            |     |               |          ||\n"
        "            |     |     * *   * * |         _||_\n"
        "            |     |     *\\/* *\\/* |        | TT |\n"
        "            |     |     *_\\_  /   ...\"\"\"\"\"\"| |"
        "| |.\"\"....\"\"\"\"\"\"\"\".\"\"\n"
        "            |     |         \\/..\"\"\"\"\"...\"\"\""
        "\\ || /.\"\"\".......\"\"\"\"...\n"
        "            |     |....\"\"\"\"\"\"\"........\"\"\"\"\""
        "\"^^^^\".......\"\"\"\"\"\"\"\"..\"\n"
        "            |......\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"......"
        "..\"\"\"\"\"....\"\"\"\"\"..\"\"...\"\"\".\n\n"
        "            You're dead.  Better luck in the next life.\n\n\n";