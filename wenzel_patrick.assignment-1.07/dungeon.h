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
#define TELEPORT '*'
#define FIFTEEN 0x0000000f
#define ADVENTURE  "adventure.rlg327"
#define HELLO "hello.rlg327"
#define WELL_DONE "welldone.rlg327"
#define DUNGEON "dungeon"
#define CHANCE 0x000000000000001
#define SMART 0b000000000000001
#define TELEPATHIC 0b000000000000010
#define TUNNEL 0b000000000000100
#define ERRATIC 0b000000000001000
#define PASS 0b000000000010000
#define PICKUP 0b000000000100000
#define DESTROY 0b000000001000000
#define UNIQ 0b0000000010000000
#define BOSS 0b0000000100000000
#define DEBUG 0
#define USLEEP_MAX 999999
#define FPS 3
#define MONSTER_TYPE 10
#define DESC 1

std::vector<std::string> colors = {"BLACK", "BLUE", "GREEN", "CYAN", "RED", "MAGENTA", "YELLOW", "WHITE"};

class CorridorPath{
public:
    heap_node_t *heap_node;
    uint8_t pos[2];
    uint8_t from[2];
    int32_t cost;
};

class Turn{
public:
    heap_node_t *heap_node;
    uint32_t next_turn;
    uint32_t seq;
    char symb;
    uint8_t speed;
};

class UpStairs{
public:
    uint8_t x_pos;
    uint8_t y_pos;
    UpStairs(uint8_t x, uint8_t y){
        x_pos = x;
        y_pos = y;
    }
};

class DownStairs{
public:
    uint8_t x_pos;
    uint8_t y_pos;
    DownStairs(uint8_t x, uint8_t y){
        x_pos = x;
        y_pos = y;
    }
};

class Room{
public:
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t x_size;
    uint8_t y_size;
};

class Dice{
public:
    int base;
    int number;
    int sides;
};

class Monster{
public:
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t alive;
    uint8_t speed;
    uint8_t type;
    char rep;
    int pc_location[2];
    int distance[MAP_Y_MAX][MAP_X_MAX];
    uint8_t map_made;
    std::string name;
    std::string desc;
    std::vector<uint8_t> color;
    uint16_t abil;
    Dice Speed;
    Dice health;
    Dice damage;
    int rrty;
    std::string abils;

    void print_monster(){
        std::cout << "Name: " << name << std::endl;
        std::cout << "Description: " << desc;
        std::cout << "Symbol: " << rep << std::endl << "Colors: ";

        abils = "";
        if(abil & SMART){
            abils += "SMART ";
        }
        if(abil & TELEPATHIC){
            abils += "TELE ";
        }
        if(abil & TUNNEL){
            abils += "TUNNEL ";
        }
        if(abil & ERRATIC){
            abils += "ERRATIC ";
        }
        if(abil & PASS){
            abils += "PASS ";
        }
        if(abil & PICKUP){
            abils += "PICKUP ";
        }
        if(abil & DESTROY){
            abils += "DESTROY ";
        }
        if(abil & UNIQ){
            abils += "UNIQ ";
        }
        if(abil & BOSS){
            abils += "BOSS";
        }
        for(int i = 0; i < (int) color.size(); i++){
            std::cout << colors[color[i]] << " ";
        }
        std::cout << std::endl;
        std::cout << "Speed: " << Speed.base << "+" << Speed.number << "d" << Speed.sides << std::endl;
        std::cout << "Abilities: " << abils << std::endl;
        std::cout << "Health: " << health.base << "+" << health.number << "d" << health.sides << std::endl;
        std::cout << "Damage: " << damage.base << "+" << damage.number << "d" << damage.sides << std::endl;
        std::cout << "Rarity: " << rrty << std::endl;
        std::cout << std::endl << std::endl;
    }
};

class Player{
public:
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t alive;
    uint8_t speed;
};

class Dungeon{
public:
    char dmap[MAP_Y_MAX][MAP_X_MAX];
    char fmap[MAP_Y_MAX][MAP_X_MAX];
    uint8_t hardness[MAP_Y_MAX][MAP_X_MAX];
    int num_rooms;
    int nt_distances[MAP_Y_MAX][MAP_X_MAX]; // Non-tunneling distance map
    int t_distances[MAP_Y_MAX][MAP_X_MAX]; // Tunneling distance map
    Player player;
    Room *rooms; //Array of rooms
    Monster *mons; //Array of monsters
    uint32_t num_mons; //Number of monsters in the dungeon
    uint16_t num_down;
    uint16_t num_up;
    UpStairs *up_stairs;
    DownStairs *down_stairs;
    FILE *file;
    uint8_t quit;
    uint8_t del;
    int total_monsters_faced;
    uint8_t fow;
};

const char monster_reps[17] = "0123456789abcdef"; //Different types of monsters

void create_dungeon_map(Dungeon *dungeon, int num_stairs_placed[2], int save);
void fill_dungeon(Dungeon *dungeon);
Room newRoom(int x_pos, int y_pos, int x_size, int y_size);
int is_room(Dungeon dungeon, int room_x_pos, int room_y_pos, int rooms[][2], int i);
void shortest_path(Dungeon *dungeon, int src_x, int src_y, int dest_x, int dest_y, int num_stairs_placed[]);
void modified_dfs(Dungeon *dungeon, int src_x, int src_y, int dest_x, int dest_y, int visited[MAP_X_MAX], int num_stairs_placed[]);
void load_game(Dungeon *dungeon, int num_stairs_placed[2]);
void save_game(Dungeon *dungeon, int num_stairs_placed[]);
void make_dungeon(Dungeon *dungeon);
int find_room(Dungeon *dungeon, int x, int y);
char find_stairs(Dungeon *dungeon, int x, int y, int is_corridor);
void print_dungeon(Dungeon *dungeon);
void save_game(Dungeon *dungeon, int num_stairs_placed[]);
void load_game(Dungeon *dungeon, int num_stairs_placed[2]);
void make_dungeon(Dungeon *dungeon);
int find_room(Dungeon *dungeon, int x, int y);
char find_stairs(Dungeon *dungeon, int x, int y, int is_corridor);
static int32_t corridor_path_cmp(const void *key, const void *with);
static void dijkstra_map(Dungeon *dungeon, int tunneling, Monster *monster);
void print_path_map(int distances[MAP_Y_MAX][MAP_X_MAX], Dungeon *dungeon, int tunneling);
void do_maps(Dungeon *dungeon);
void create_monsters(Dungeon *dungeon);
void place_monsters(Dungeon *dungeon);
static int turn_decider(Dungeon *dungeon, Turn turn_event[], int *init, int num_characters);
static int32_t turn_cmp(const void *key, const void *with);
void move_character(Dungeon *dungeon, Turn turn, int next_pos[2]);
int seen(int mons[2]);
void get_next_pos(Dungeon *dungeon, Turn turn, int next_pos[2], int telepathic, int tunneling, int smart);
int is_character(Dungeon *dungeon, Monster mons[], int next_pos[2], Turn turn);
uint8_t can_be_seen(Dungeon *dungeon, Monster monster, Player player);
int far_enough_away(Dungeon *dungeon, int x, int y);
void display_monsters(Dungeon *dungeon);
void clear_dungeon(Dungeon *dungeon);
void print_help(Dungeon *dungeon);
void delete_dungeon(Dungeon *dungeon, Turn turn[], int *init, int *num_characters);
int get_num_alive_monsters(Dungeon *dungeon);
int is_open(Dungeon *dungeon, int next_pos[2]);
void teleport(Dungeon *dungeon, int next_pos[2]);
void add_cells_to_fow(Dungeon *dungeon);
int print_descriptions();
void reset_monster(Monster *monster);

const char *keys[19] = {
        "7/y/Home moves your character up one and one left",
        "8/k/Up Arrow moves your character one up",
        "9/u/Page Up moves your character one up and one right",
        "6/l/Right Arrow moves your character one right",
        "3/n/Dwn Page moves your character one down and one right",
        "2/j/Down Arrow moves your character one down",
        "1/b/End moves your character one down and one left",
        "4/h/Left Arrow moves your character one left",
        "> Attempts to go down stairs. Only works if on a down staircase",
        "< Attempts to go up stairs. Only works if on an up staircase",
        "5, space, or . Rests for a turn",
        "m displays a list of all the monsters",
        "Up arrow scrolls up on the monster list (if able to)",
        "Down arrow scrolls down on the monster list (if able to",
        "Escape exits the monster list or the help screen",
        "Q quits the game",
        "? Displays all the valid inputs and what they do",
        "g teleports. r for random or use arrows to choose location",
        "f Displays the whole map until f is pressed again"
};

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

const char *quit =
        "\n\n\n"
        "  ___    ___  ________   ___  ___   ________   _______           ________\n"
        "  |\\  \\  /  /||\\   __  \\ |\\  \\|\\  \\ |\\   __  \\ |\\  ___ \\         |\\   __  \\   \n"
        "  \\ \\  \\/  / /\\ \\  \\|\\  \\\\ \\  \\\\\\  \\\\ \\  \\|\\  \\\\ \\   __/|        \\ \\  \\|\\  \\ \n"
        "   \\ \\    / /  \\ \\  \\\\\\  \\\\ \\  \\\\\\  \\\\ \\   _  _\\\\ \\  \\_|/__       \\ \\   __  \\ \n"
        "    \\/  /  /    \\ \\  \\\\\\  \\\\ \\  \\\\\\  \\\\ \\  \\\\  \\|\\ \\  \\_|\\ \\       \\ \\  \\ \\  \\    \n"
        "  __/  / /       \\ \\_______\\\\ \\_______\\\\ \\__\\\\ _\\ \\ \\_______\\       \\ \\__\\ \\__\\ \n"
        " |\\___/ /         \\|_______| \\|_______| \\|__|\\|__| \\|_______|        \\|__|\\|__|\n"
        " \\|___|/\n"
        "________   ___  ___   ___   _________   _________   _______    ________\n"
        "|\\   __  \\ |\\  \\|\\  \\ |\\  \\ |\\___   ___\\|\\___   ___\\|\\  ___ \\  |\\   __  \\   \n"
        "\\ \\  \\|\\  \\\\ \\  \\\\\\  \\\\ \\  \\\\|___ \\  \\_|\\|___ \\  \\_|\\ \\   __/| \\ \\  \\|\\  \\ \n"
        " \\ \\  \\\\\\  \\\\ \\  \\\\\\  \\\\ \\  \\    \\ \\  \\      \\ \\  \\  \\ \\  \\_|/__\\ \\   _  _\\ \n"
        "  \\ \\  \\\\\\  \\\\ \\  \\\\\\  \\\\ \\  \\    \\ \\  \\      \\ \\  \\  \\ \\  \\_|\\ \\\\ \\  \\\\  \\|\n"
        "   \\ \\_____  \\\\ \\_______\\\\ \\__\\    \\ \\__\\      \\ \\__\\  \\ \\_______\\\\ \\__\\\\ _\\ \n"
        "    \\|___| \\__\\\\|_______| \\|__|     \\|__|       \\|__|   \\|_______| \\|__|\\|__|\n"
        "          \\|__|\n"
        "   ___    ___        ___        ________   _____ ______    _______\n"
        "   |\\  \\  |\\  \\      |\\  \\      |\\   __  \\ |\\   _ \\  _   \\ |\\  ___ \\ \n"
        " __\\_\\  \\_\\_\\  \\_____\\ \\  \\     \\ \\  \\|\\  \\\\ \\  \\\\\\__\\ \\  \\\\ \\   __/|\n"
        "|\\____    ___    ____\\\\ \\  \\     \\ \\   __  \\\\ \\  \\\\|__| \\  \\\\ \\  \\_|/__\n"
        "\\|___| \\  \\__|\\  \\___| \\ \\  \\____ \\ \\  \\ \\  \\\\ \\  \\    \\ \\  \\\\ \\  \\_|\\ \\ \n"
        "    __\\_\\  \\_\\_\\  \\_____\\ \\_______\\\\ \\__\\ \\__\\\\ \\__\\    \\ \\__\\\\ \\_______\\ \n"
        "   |\\____    ____   ____\\\\|_______| \\|__|\\|__| \\|__|     \\|__| \\|_______| \n"
        "   \\|___| \\  \\__|\\  \\___| \n"
        "         \\ \\__\\ \\ \\__\\ \n"
        "          \\|__|  \\|__|\n"
        "\n\nQuitting's lame.\n\n\n";