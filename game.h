#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>

#define MAX_ROOMS 6
#define MIN_WIDTH 8
#define MIN_HEIGHT 8
#define MAX_HEIGHT 12
#define MAX_WIDTH 12
#define MAX_FOOD 5
#define HUNGER_MAX 10
#define BAR_WIDTH 10
#define HUNGER_INTERVAL 20
typedef struct stairs
{
    int room_index;
    int x,y;
}stairs;


typedef struct door
{
    //int index_room;
    int x,y;
    //int hidden;

}door;

typedef struct pillow
{
    //int index_room;
    int x,y;
}pillow;

typedef struct trap 
{
    int x,y;
}trap;


typedef struct window_room
{
   //int index_room;    
   int x,y;   
}window_room;


typedef struct room{
    int x,y;
    int width,height;
    int room_index;
    char theme[20];
    //int cor[MAX_ROOMS][5];
}room; 

typedef struct enemy
{
    char name;
    int current_x,current_y;
    int room_index;
    int heart;
    int alive;
}enemy;

typedef struct weopon
{
    char name;
    int x,y;
}weapon;

typedef struct Player{
    char user_name[100];
    int score;
    int gold;
    int num_of_games;
    int experience;
}Player;

typedef struct entry
{
    char user_name[100];
    time_t entry_time;
}entry;


//extern char user_name[100];
extern Player current_player;
extern struct room rooms_one[MAX_ROOMS];
extern struct room rooms_two[MAX_ROOMS];
extern struct room rooms_three[MAX_ROOMS];
extern struct room rooms_four[MAX_ROOMS];
extern struct room stair_room;                    
extern struct door doors_one[MAX_ROOMS][40];
extern struct door doors_two[MAX_ROOMS][40];
extern struct door doors_three[MAX_ROOMS][40];
extern struct door doors_four[MAX_ROOMS][40];
extern struct door hidden_door_one[MAX_ROOMS][40];
extern struct door hidden_door_two[MAX_ROOMS][40];
extern struct door hidden_door_three[MAX_ROOMS][40];
extern struct door hidden_door_four[MAX_ROOMS][40];
extern struct pillow pillows_one[MAX_ROOMS][3];
extern struct pillow pillows_two[MAX_ROOMS][3];
extern struct pillow pillows_three[MAX_ROOMS][3];
extern struct pillow pillows_four[MAX_ROOMS][3];
extern struct window_room windows_one[MAX_ROOMS][10];
extern struct window_room windows_two[MAX_ROOMS][10];
extern struct window_room windows_three[MAX_ROOMS][10];
extern struct window_room windows_four[MAX_ROOMS][10];
extern struct trap traps_one[MAX_ROOMS][10];
extern struct trap traps_two[MAX_ROOMS][10];
extern struct trap traps_three[MAX_ROOMS][10];
extern struct trap traps_four[MAX_ROOMS][10];
extern struct stairs stair;                        ////////////////
extern struct enemy deamon[4];                     /////////////
extern struct enemy fire_breathing[4];              //////////////
extern struct enemy giant[4];
extern struct enemy snake[4];                 
extern struct enemy undeed[4];          ////////////
extern char **floor_one;
extern char **floor_two;
extern char **floor_three;
extern char **floor_four;
extern const char obstacles[]; 
extern int visited_one[MAX_ROOMS];
extern int visited_two[MAX_ROOMS];
extern int visited_three[MAX_ROOMS];
extern int visited_four[MAX_ROOMS];
extern int **visit_map_one;
extern int **visit_map_two;
extern int **visit_map_three;
extern int **visit_map_four;
extern int enchanted[4][1];
extern int Heart;
extern int Gold;
extern int Health;
extern int hunger;
extern int food_collected;
extern int dagger_count;
extern int magic_wand_count;
extern int normal_arrow_count;
extern int sword_count;
extern int mace_damage;
extern int dagger_damage;
extern int magic_wand_damage;
extern int normal_arrow_damage;
extern int sword_damage;
extern char default_weapon;
extern char last_hit;
extern char spell;
extern int health_spell;
extern int speed_spell;
extern int damage_spell;
extern int health_count;
extern int speed_count;
extern int damage_count;
extern int enchanted_stay;
extern int win;
extern int end;
extern int which_floor_save;
extern int posx_save;
extern int posy_save;
extern int login_choice;
extern int color;
extern int choice_level;

void message_box(int message,int which_floor);
void create_map(char **floor_one,int which_floor,FILE *file_one,room rooms[MAX_ROOMS],window_room windows[MAX_ROOMS][10],door doors[MAX_ROOMS][40],pillow pillows[MAX_ROOMS][3],trap traps[MAX_ROOMS][10],int **visit);
void create_door(char **floor_one,int i,door doors[MAX_ROOMS][40],room rooms[MAX_ROOMS]);
void create_pillow(room rooms[MAX_ROOMS],pillow pillows[MAX_ROOMS][3]);
void create_window(room rooms[MAX_ROOMS],window_room windows[MAX_ROOMS][10]);
void create_corridor(int which_floor,char **floor_one,door doors[MAX_ROOMS][40]);
void create_stairs(void);
void create_traps(int which_floor,room rooms[MAX_ROOMS],trap traps[MAX_ROOMS][10]);
void save_floor(char **floor_one,FILE *file_one);
void initialize_floor(char **floor_one,int **visit);
void initialize(room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],door hidden[MAX_ROOMS][40],pillow pillows[MAX_ROOMS][3],window_room windows[MAX_ROOMS][10],trap traps[MAX_ROOMS][10],int visited[MAX_ROOMS]);
void move_character(int *position_x,int *position_y,int key,char **floor_one,int which_floor,room rooms[MAX_ROOMS]);
int find_room(int position_y,int position_x,room rooms[MAX_ROOMS]);
void room_visibility(int which_floor,int room_index,char **floor_one,room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],int **visit_map);
void corridor_visibility(int position_y,int position_x,int which_floor,char **floor,int **visit_map);
void reset_doors(char **floor_one,door doors[MAX_ROOMS][40],room rooms[MAX_ROOMS]);
void hidden_doors(int which_floor,char **floor,door doors[MAX_ROOMS][40],room rooms[MAX_ROOMS],int last_room);
void run(int which_floor,int *position_x,int *position_y,int dx,int dy,char **floor_one,room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],int visited[MAX_ROOMS],int **visit_map);
void status_box(void);
void game(int entering,int which_floor,char **floor,room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],trap traps[MAX_ROOMS][10],int visited[MAX_ROOMS],int **visit_map);
void watch_room(int which_floor,int room_index,char **floor_one,room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],int **visit_map);
void room_color(char **floor,room rooms[MAX_ROOMS],int room_index,int enchanted);
void create_gold(char **floor,room rooms[MAX_ROOMS]);
void create_food(char **floor,room rooms[MAX_ROOMS]);
void create_weapon(char **floor,room rooms[MAX_ROOMS]);
void create_spell(int which_floor,char **floor,room rooms[MAX_ROOMS]);
void create_monster(int which_floor,char **floor,room rooms[MAX_ROOMS]);
void draw_hunger_bar(int hunger);
void throw_weapon(char **floor,int which_floor,int position_x,int position_y,int dx,int dy);
void fight(char **floor,int which_floor,room rooms[MAX_ROOMS],int position_x,int position_y,int repeat);
void move_monster(enemy *monster, int player_x, int player_y, char **floor,int which_floor,room rooms[MAX_ROOMS]);
void update_monsters(int which_floor, int which_room,int player_x, int player_y, char **floor, room rooms[MAX_ROOMS]);
void display_monsters(int which_floor, char **floor,int visited[MAX_ROOMS]);
void save(const char *user_name);
int obstacle(char c);
void initial_menu(int highlight);
void game_menu(int highlight);
void game_choice(int choice);
void menu_choice(int choice);
void save_name(char *name);
void save_user(char *name,char *pass,char *reminder,char *email);
int check_name(char *name);
int check_pass(char *pass);
void signup(void);
void login(void);
void guest(void);
int check_email(char *email);
void new_game(void);
void resume_game(void);

#endif 