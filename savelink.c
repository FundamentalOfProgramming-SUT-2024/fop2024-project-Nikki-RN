#include "game.h"

struct room rooms_one[MAX_ROOMS];
struct room rooms_two[MAX_ROOMS];
struct room rooms_three[MAX_ROOMS];
struct room rooms_four[MAX_ROOMS];
struct room stair_room;
struct door doors_one[MAX_ROOMS][40];
struct door doors_two[MAX_ROOMS][40];
struct door doors_three[MAX_ROOMS][40];
struct door doors_four[MAX_ROOMS][40];
struct door hidden_door_one[MAX_ROOMS][40];
struct door hidden_door_two[MAX_ROOMS][40];
struct door hidden_door_three[MAX_ROOMS][40];
struct door hidden_door_four[MAX_ROOMS][40];
struct pillow pillows_one[MAX_ROOMS][3];
struct pillow pillows_two[MAX_ROOMS][3];
struct pillow pillows_three[MAX_ROOMS][3];
struct pillow pillows_four[MAX_ROOMS][3];
struct window_room windows_one[MAX_ROOMS][10];
struct window_room windows_two[MAX_ROOMS][10];
struct window_room windows_three[MAX_ROOMS][10];
struct window_room windows_four[MAX_ROOMS][10];
struct trap traps_one[MAX_ROOMS][10];
struct trap traps_two[MAX_ROOMS][10];
struct trap traps_three[MAX_ROOMS][10];
struct trap traps_four[MAX_ROOMS][10];
struct stairs stair;
struct enemy deamon[4];
struct enemy fire_breathing[4];
struct enemy giant[4];
struct enemy snake[4];
struct enemy undeed[4];
char **floor_one;
char **floor_two;
char **floor_three;
char **floor_four;
const char obstacles[]="|_=O ";
int visited_one[MAX_ROOMS]={0};
int visited_two[MAX_ROOMS]={0};
int visited_three[MAX_ROOMS]={0};
int visited_four[MAX_ROOMS]={0};
int **visit_map_one;
int **visit_map_two;
int **visit_map_three;
int **visit_map_four;
int enchanted[4][1];
int Heart=100,Gold=0,Health=10;
int hunger=0;
int food_collected = 0;
int dagger_count=0,magic_wand_count=0,normal_arrow_count=0,sword_count=0;
int mace_damage=5,dagger_damage=12,magic_wand_damage=15,normal_arrow_damage=5,sword_damage=10;
char default_weapon='m';
char last_hit=' ';
char spell='n';
int health_spell=0,speed_spell=0,damage_spell=0;
int health_count=0,speed_count=0,damage_count=0;
int enchanted_stay=0; 
int win=0,end=0;
int which_floor_save,posx_save,posy_save;

void draw_hunger_bar(int hunger) {
    mvprintw(2, 0, "Hunger: [");
    for (int i = 0; i < BAR_WIDTH; i++) {
        if (i < hunger)
            addch('=');
        else
            addch(' ');
    }
    printw("]");
}

void throw_weapon(char **floor,int which_floor,int position_x,int position_y,int dx,int dy){
    int last_safe_row=position_x,last_safe_col=position_y;
    if(default_weapon=='d'){
       for(int i=1;i<=5;i++){
        int new_y=position_y+dy*(i);
        int new_x=position_x+dx*(i);
        if(new_x<0 || new_x>=COLS || new_y<0 || new_y>=LINES){
            break;
        }
        if(floor[position_y+dy*(i)][position_x+dx*(i)]!='.'){
            return;           
        }
        else if(floor[position_y+dy*(i)][position_x+dx*(i)]=='.'){
            if(!(last_safe_row==position_x && last_safe_col==position_y)) floor[last_safe_col][last_safe_row]='.';
            floor[position_y+dy*(i)][position_x+dx*(i)]='1';
        }
        last_safe_row=position_x+dx*(i);
        last_safe_col=position_y+dy*(i);
       }
    }
    else if(default_weapon=='n'){
        for(int i=1;i<=5;i++){
        int new_y=position_y+dy*(i);
        int new_x=position_x+dx*(i);
        if(new_x<0 || new_x>=COLS || new_y<0 || new_y>=LINES){
            break;
        }
        if(floor[position_y+dy*(i)][position_x+dx*(i)]!='.'){
            return;           
        }
        else if(floor[position_y+dy*(i)][position_x+dx*(i)]=='.'){
            if(!(last_safe_row==position_x && last_safe_col==position_y)) floor[last_safe_col][last_safe_row]='.';
            floor[position_y+dy*(i)][position_x+dx*(i)]='2';
        }
        last_safe_row=position_x+dx*(i);
        last_safe_col=position_y+dy*(i);
       }
    }
    else if(default_weapon=='a'){
        for(int i=1;i<=10;i++){
        int new_y=position_y+dy*(i);
        int new_x=position_x+dx*(i);
        if(new_x<0 || new_x>=COLS || new_y<0 || new_y>=LINES){
            break;
        }
        if(floor[position_y+dy*(i)][position_x+dx*(i)]!='.'){
            return;           
        }
        else if(floor[position_y+dy*(i)][position_x+dx*(i)]=='.'){
            if(!(last_safe_row==position_x && last_safe_col==position_y)) floor[last_safe_col][last_safe_row]='.';
            floor[position_y+dy*(i)][position_x+dx*(i)]='3';
        }
        last_safe_row=position_x+dx*(i);
        last_safe_col=position_y+dy*(i);
       }
    }
}

void fight(char **floor,int which_floor,room rooms[MAX_ROOMS],int position_x,int position_y,int repeat){
    char ch;
    if(default_weapon=='m' || default_weapon=='s'){
    int hit=1;
    if((position_x>0 && deamon[which_floor].current_x==position_x-1 && deamon[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && deamon[which_floor].current_x==position_x+1 && deamon[which_floor].current_y==position_y) ||
        (position_y>0 && deamon[which_floor].current_y==position_y-1 && deamon[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && deamon[which_floor].current_y==position_y+1 && deamon[which_floor].current_x==position_x) ||
        (position_x>0 && position_y>0 && deamon[which_floor].current_x==position_x-1 && deamon[which_floor].current_y==position_y-1) ||
        (position_x>0 && position_y<LINES-1 && deamon[which_floor].current_x==position_x-1 && deamon[which_floor].current_y==position_y+1) ||
        (position_x<COLS-1 && position_y>0 && deamon[which_floor].current_x==position_x+1 && deamon[which_floor].current_y==position_y-1) ||
        (position_x<COLS-1 && position_y<LINES-1 && deamon[which_floor].current_x==position_x+1 && deamon[which_floor].current_y==position_y+1)){
        if(default_weapon=='m') deamon[which_floor].heart-=mace_damage;
        if(default_weapon=='s') deamon[which_floor].heart-=sword_damage;
        message_box(34,which_floor);
        hit=0;
    }
    if((position_x>0 && fire_breathing[which_floor].current_x==position_x-1 && fire_breathing[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && fire_breathing[which_floor].current_x==position_x+1 && fire_breathing[which_floor].current_y==position_y) ||
        (position_y>0 && fire_breathing[which_floor].current_y==position_y-1 && fire_breathing[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && fire_breathing[which_floor].current_y==position_y+1 && fire_breathing[which_floor].current_x==position_x) ||
        (position_x>0 && position_y>0 && fire_breathing[which_floor].current_x==position_x-1 && fire_breathing[which_floor].current_y==position_y-1) ||
        (position_x>0 && position_y<LINES-1 && fire_breathing[which_floor].current_x==position_x-1 && fire_breathing[which_floor].current_y==position_y+1) ||
        (position_x<COLS-1 && position_y>0 && fire_breathing[which_floor].current_x==position_x+1 && fire_breathing[which_floor].current_y==position_y-1) ||
        (position_x<COLS-1 && position_y<LINES-1 && fire_breathing[which_floor].current_x==position_x+1 && fire_breathing[which_floor].current_y==position_y+1)){
        if(default_weapon=='m') fire_breathing[which_floor].heart-=mace_damage;
        if(default_weapon=='s') fire_breathing[which_floor].heart-=sword_damage;
        message_box(35,which_floor);
        hit=0;
    }
    if((position_x>0 && giant[which_floor].current_x==position_x-1 && giant[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && giant[which_floor].current_x==position_x+1 && giant[which_floor].current_y==position_y) ||
        (position_y>0 && giant[which_floor].current_y==position_y-1 && giant[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && giant[which_floor].current_y==position_y+1 && giant[which_floor].current_x==position_x) ||
        (position_x>0 && position_y>0 && giant[which_floor].current_x==position_x-1 && giant[which_floor].current_y==position_y-1) ||
        (position_x>0 && position_y<LINES-1 && giant[which_floor].current_x==position_x-1 && giant[which_floor].current_y==position_y+1) ||
        (position_x<COLS-1 && position_y>0 && giant[which_floor].current_x==position_x+1 && giant[which_floor].current_y==position_y-1) ||
        (position_x<COLS-1 && position_y<LINES-1 && giant[which_floor].current_x==position_x+1 && giant[which_floor].current_y==position_y+1)){
        if(default_weapon=='m') giant[which_floor].heart-=mace_damage;
        if(default_weapon=='s') giant[which_floor].heart-=sword_damage;
        message_box(36,which_floor);
        hit=0;
    }
    if((position_x>0 && snake[which_floor].current_x==position_x-1 && snake[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && snake[which_floor].current_x==position_x+1 && snake[which_floor].current_y==position_y) ||
        (position_y>0 && snake[which_floor].current_y==position_y-1 && snake[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && snake[which_floor].current_y==position_y+1 && snake[which_floor].current_x==position_x) ||
        (position_x>0 && position_y>0 && snake[which_floor].current_x==position_x-1 && snake[which_floor].current_y==position_y-1) ||
        (position_x>0 && position_y<LINES-1 && snake[which_floor].current_x==position_x-1 && snake[which_floor].current_y==position_y+1) ||
        (position_x<COLS-1 && position_y>0 && snake[which_floor].current_x==position_x+1 && snake[which_floor].current_y==position_y-1) ||
        (position_x<COLS-1 && position_y<LINES-1 && snake[which_floor].current_x==position_x+1 && snake[which_floor].current_y==position_y+1)){
        if(default_weapon=='m') snake[which_floor].heart-=mace_damage;
        if(default_weapon=='s') snake[which_floor].heart-=sword_damage;
        message_box(37,which_floor);
        hit=0;
    }
    if((position_x>0 && undeed[which_floor].current_x==position_x-1 && undeed[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && undeed[which_floor].current_x==position_x+1 && undeed[which_floor].current_y==position_y) ||
        (position_y>0 && undeed[which_floor].current_y==position_y-1 && undeed[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && undeed[which_floor].current_y==position_y+1 && undeed[which_floor].current_x==position_x) ||
        (position_x>0 && position_y>0 && undeed[which_floor].current_x==position_x-1 && undeed[which_floor].current_y==position_y-1) ||
        (position_x>0 && position_y<LINES-1 && undeed[which_floor].current_x==position_x-1 && undeed[which_floor].current_y==position_y+1) ||
        (position_x<COLS-1 && position_y>0 && undeed[which_floor].current_x==position_x+1 && undeed[which_floor].current_y==position_y-1) ||
        (position_x<COLS-1 && position_y<LINES-1 && undeed[which_floor].current_x==position_x+1 && undeed[which_floor].current_y==position_y+1)){
        if(default_weapon=='m') undeed[which_floor].heart-=mace_damage;
        if(default_weapon=='s') undeed[which_floor].heart-=sword_damage;
        message_box(38,which_floor);
        hit=0;
    } 
    if(hit){
        message_box(39,which_floor);
    }
    }
    else{
        if(repeat){
          ch=getchar();
          last_hit=ch;
        } 
        else{
            ch=last_hit;
        }
        int dx,dy;
        char enemy=' ';
        switch (ch)
        {
        case '1':
            dx=-1;
            dy=1;
        break;
        case '2':
            dx=0;
            dy=1;
        break;
        case '3':
            dx=1;
            dy=1;
        break;
        case '4':
            dx=-1;
            dy=0;
        break;
        case '6':
            dx=1;
            dy=0;
        break;
        case '7':
            dx=-1;
            dy=-1;
        break;
        case '8':
            dx=0;
            dy=-1;
        break;
        case '9':
            dx=1;
            dy=-1;
        break;
        }
        if(default_weapon=='d' || default_weapon=='n'){
            for(int i=1;i<=5;i++){
               if(deamon[which_floor].alive && deamon[which_floor].current_x==position_x+dx*(i) && deamon[which_floor].current_y==position_y+dy*(i)){
                enemy='d';
                break;
               }
               else if(fire_breathing[which_floor].alive && fire_breathing[which_floor].current_x==position_x+dx*(i) && fire_breathing[which_floor].current_y==position_y+dy*(i)){
                enemy='f';
                break;
               }
               else if(giant[which_floor].alive && giant[which_floor].current_x==position_x+dx*(i) && giant[which_floor].current_y==position_y+dy*(i)){
                enemy='g';
                break;
               }
               else if(snake[which_floor].alive && snake[which_floor].current_x==position_x+dx*(i) && snake[which_floor].current_y==position_y+dy*(i)){
                enemy='s';
                break;
               }
               else if(undeed[which_floor].alive && undeed[which_floor].current_x==position_x+dx*(i) && undeed[which_floor].current_y==position_y+dy*(i)){
                enemy='u';
                break;
               }
        }
        if(default_weapon=='d'){
            if(dagger_count>0){
            if(enemy=='d'){
            deamon[which_floor].heart-=dagger_damage;
            message_box(34,which_floor);
            } 
            else if(enemy=='f'){
            fire_breathing[which_floor].heart-=dagger_damage;
            message_box(35,which_floor);
            } 
            else if(enemy=='g'){
            giant[which_floor].heart-=dagger_damage;
            message_box(36,which_floor);
           } 
           else if(enemy=='s'){
            snake[which_floor].heart-=dagger_damage;
            message_box(37,which_floor);
           } 
           else if(enemy=='u'){
            undeed[which_floor].heart-=dagger_damage;
            message_box(38,which_floor);
           } 
           else{
            message_box(39,which_floor);
            throw_weapon(floor,which_floor,position_x,position_y,dx,dy);
           }
           dagger_count--;
        }
        else{
            default_weapon=' ';
            message_box(40,which_floor);
        }
        }
        else{
            if(normal_arrow_count>0){
            if(enemy=='d'){
            deamon[which_floor].heart-=normal_arrow_damage;
            message_box(34,which_floor);
           } 
           else if(enemy=='f'){
            fire_breathing[which_floor].heart-=normal_arrow_damage;
            message_box(35,which_floor);
           } 
           else if(enemy=='g'){
            giant[which_floor].heart-=normal_arrow_damage;
            message_box(36,which_floor);
           } 
           else if(enemy=='s'){
           snake[which_floor].heart-=normal_arrow_damage;
           message_box(37,which_floor);
           } 
           else if(enemy=='u'){
           undeed[which_floor].heart-=normal_arrow_damage;
           message_box(38,which_floor);
           } 
           else{
            message_box(39,which_floor);
            throw_weapon(floor,which_floor,position_x,position_y,dx,dy);
           }
           normal_arrow_count--;
        }
        else{
            default_weapon=' ';
            message_box(40,which_floor);
        }
        }
        }
        else if(default_weapon=='a'){
            for(int i=1;i<=10;i++){
               if(deamon[which_floor].alive && deamon[which_floor].current_x==position_x+dx*(i) && deamon[which_floor].current_y==position_y+dy*(i)){
                enemy='d';
                break;
               }
               else if(fire_breathing[which_floor].alive && fire_breathing[which_floor].current_x==position_x+dx*(i) && fire_breathing[which_floor].current_y==position_y+dy*(i)){
                enemy='f';
                break;
               }
               else if(giant[which_floor].alive && giant[which_floor].current_x==position_x+dx*(i) && giant[which_floor].current_y==position_y+dy*(i)){
                enemy='g';
                break;
               }
               else if(snake[which_floor].alive && snake[which_floor].current_x==position_x+dx*(i) && snake[which_floor].current_y==position_y+dy*(i)){
                enemy='s';
                break;
               }
               else if(undeed[which_floor].alive && undeed[which_floor].current_x==position_x+dx*(i) && undeed[which_floor].current_y==position_y+dy*(i)){
                enemy='u';
                break;
               }
            }
            if(magic_wand_count>0){
            if(enemy=='d'){
               deamon[which_floor].heart-=magic_wand_damage;
               message_box(34,which_floor);
            } 
            else if(enemy=='f'){
               fire_breathing[which_floor].heart-=magic_wand_damage;
               message_box(35,which_floor);
            } 
            else if(enemy=='g'){
               giant[which_floor].heart-=magic_wand_damage;
               message_box(36,which_floor);
            } 
            else if(enemy=='s'){
               snake[which_floor].heart-=magic_wand_damage;
               message_box(37,which_floor);
            } 
            else if(enemy=='u'){
               undeed[which_floor].heart-=magic_wand_damage;
               message_box(38,which_floor);
            }
            else{
                message_box(39,which_floor);
                throw_weapon(floor,which_floor,position_x,position_y,dx,dy);
            } 
            magic_wand_count--;
            }
            else{
                default_weapon=' ';
                message_box(40,which_floor);
            }
        }
    }
    if(deamon[which_floor].heart<=0){
      deamon[which_floor].alive=0;
      deamon[which_floor].current_x=0;
      deamon[which_floor].current_y=0;
    } 
    if(fire_breathing[which_floor].heart<=0){
      fire_breathing[which_floor].alive=0;
      fire_breathing[which_floor].current_x=0;
      fire_breathing[which_floor].current_y=0;
    } 
    if(giant[which_floor].heart<=0){
       giant[which_floor].alive=0;
       giant[which_floor].current_x=0;
       giant[which_floor].current_y=0;
    } 
    if(snake[which_floor].heart<=0){
      snake[which_floor].alive=0;
      snake[which_floor].current_x=0;
      snake[which_floor].current_y=0;
    } 
    if(undeed[which_floor].heart<=0){
      undeed[which_floor].alive=0;
      undeed[which_floor].current_x=0;
      undeed[which_floor].current_y=0;
    } 
}


void move_monster(enemy *monster, int player_x, int player_y, char **floor,int which_floor,room rooms[MAX_ROOMS]) {
    int dx = player_x - monster->current_x;
    int dy = player_y - monster->current_y;

    int next_x = monster->current_x;
    int next_y = monster->current_y;

    
    if (abs(dx) > abs(dy)) {
        next_x += (dx > 0) ? 1 : -1; 
    } else {
        next_y += (dy > 0) ? 1 : -1; 
    }

    int special_monsters=1;
    if(monster->name=='A' || monster->name=='F'){
        if(abs(dx)>2 || abs(dy)>2){
            special_monsters=0;
        }
    }
    if(special_monsters){
    if (floor[next_y][next_x] != '|' && 
        floor[next_y][next_x] != '_' &&
        floor[next_y][next_x] != '=' &&
        floor[next_y][next_x] != '+' &&
        !(next_y==player_y && next_x==player_x)) { 
        if(!(monster->name!=deamon[which_floor].name && next_y==deamon[which_floor].current_y && next_x==deamon[which_floor].current_x) &&
           !(monster->name!=fire_breathing[which_floor].name && next_y==fire_breathing[which_floor].current_y && next_x==fire_breathing[which_floor].current_x) &&
           !(monster->name!=giant[which_floor].name && next_y==giant[which_floor].current_y && next_x==giant[which_floor].current_x) &&
           !(monster->name!=snake[which_floor].name && next_y==snake[which_floor].current_y && next_x==snake[which_floor].current_x) &&
           !(monster->name!=undeed[which_floor].name && next_y==undeed[which_floor].current_y && next_x==undeed[which_floor].current_x)){
            mvaddch(monster->current_y, monster->current_x, floor[monster->current_y][monster->current_x]);
            monster->current_x = next_x;
            monster->current_y = next_y;
            mvaddch(monster->current_y,monster->current_x,monster->name);
        }    
    }
}
}

void update_monsters(int which_floor, int which_room,int player_x, int player_y, char **floor, room rooms[MAX_ROOMS]) {
    if(deamon[which_floor].room_index==which_room && deamon[which_floor].alive) move_monster(&deamon[which_floor], player_x, player_y, floor,which_floor, rooms);
    if(fire_breathing[which_floor].room_index==which_room && fire_breathing[which_floor].alive) move_monster(&fire_breathing[which_floor], player_x, player_y, floor,which_floor, rooms);
    if(giant[which_floor].room_index==which_room && giant[which_floor].alive) move_monster(&giant[which_floor], player_x, player_y, floor,which_floor,rooms);
    if(snake[which_floor].room_index==which_room && snake[which_floor].alive) move_monster(&snake[which_floor], player_x, player_y, floor,which_floor,rooms);
    if(undeed[which_floor].room_index==which_room && undeed[which_floor].alive) move_monster(&undeed[which_floor], player_x, player_y, floor,which_floor,rooms);
}

void display_monsters(int which_floor, char **floor,int visited[MAX_ROOMS]) {
    attron(COLOR_PAIR(1));
    if(visited[deamon[which_floor].room_index]==1 && deamon[which_floor].alive){
           mvaddch(deamon[which_floor].current_y, deamon[which_floor].current_x, ' ');  
           mvaddch(deamon[which_floor].current_y, deamon[which_floor].current_x, 'A');
    } 
    if(visited[fire_breathing[which_floor].room_index]==1 && fire_breathing[which_floor].alive){
      mvaddch(fire_breathing[which_floor].current_y, fire_breathing[which_floor].current_x, ' ');
      mvaddch(fire_breathing[which_floor].current_y, fire_breathing[which_floor].current_x, 'F');
    }  
    if(visited[giant[which_floor].room_index]==1 && giant[which_floor].alive){
      mvaddch(giant[which_floor].current_y, giant[which_floor].current_x, ' ');
      mvaddch(giant[which_floor].current_y, giant[which_floor].current_x, 'G');
    }  
    if(visited[snake[which_floor].room_index]==1 && snake[which_floor].alive){
       mvaddch(snake[which_floor].current_y, snake[which_floor].current_x, ' ');
       mvaddch(snake[which_floor].current_y, snake[which_floor].current_x, 'C');
    }  
    if(visited[undeed[which_floor].room_index]==1 && undeed[which_floor].alive){
       mvaddch(undeed[which_floor].current_y, undeed[which_floor].current_x, ' ');
       mvaddch(undeed[which_floor].current_y, undeed[which_floor].current_x, 'U');
    }  
    attroff(COLOR_PAIR(1));
}

void save(const char *user_name){
    end=0,win=0;
    char file_name[105];
    snprintf(file_name,sizeof(file_name),"%s.dat",user_name);
    FILE *game_file=fopen(file_name,"wb");
    fwrite(rooms_one,sizeof(room),MAX_ROOMS,game_file);
    fwrite(rooms_two,sizeof(room),MAX_ROOMS,game_file);
    fwrite(rooms_three,sizeof(room),MAX_ROOMS,game_file);
    fwrite(rooms_four,sizeof(room),MAX_ROOMS,game_file);
    fwrite(&stair_room,sizeof(door),1,game_file);
    fwrite(doors_one,sizeof(door),MAX_ROOMS * 40,game_file);
    fwrite(doors_two,sizeof(door),MAX_ROOMS * 40,game_file);
    fwrite(doors_three,sizeof(door),MAX_ROOMS * 40,game_file);
    fwrite(doors_four,sizeof(door),MAX_ROOMS * 40,game_file);
    fwrite(hidden_door_one,sizeof(door),MAX_ROOMS * 40,game_file);
    fwrite(hidden_door_two,sizeof(door),MAX_ROOMS * 40,game_file);
    fwrite(hidden_door_three,sizeof(door),MAX_ROOMS * 40,game_file);
    fwrite(hidden_door_four,sizeof(door),MAX_ROOMS * 40,game_file);
    fwrite(pillows_one,sizeof(pillow),MAX_ROOMS * 3,game_file);
    fwrite(pillows_two,sizeof(pillow),MAX_ROOMS * 3,game_file);
    fwrite(pillows_three,sizeof(pillow),MAX_ROOMS * 3,game_file);
    fwrite(pillows_four,sizeof(pillow),MAX_ROOMS * 3,game_file);
    fwrite(windows_one,sizeof(window_room),MAX_ROOMS * 10,game_file);
    fwrite(windows_two,sizeof(window_room),MAX_ROOMS * 10,game_file);
    fwrite(windows_three,sizeof(window_room),MAX_ROOMS * 10,game_file);
    fwrite(windows_four,sizeof(window_room),MAX_ROOMS * 10,game_file);
    fwrite(traps_one,sizeof(trap),MAX_ROOMS * 10,game_file);
    fwrite(traps_two,sizeof(trap),MAX_ROOMS * 10,game_file);
    fwrite(traps_three,sizeof(trap),MAX_ROOMS * 10,game_file);
    fwrite(traps_four,sizeof(trap),MAX_ROOMS * 10,game_file);
    fwrite(&stair,sizeof(stairs),1,game_file);
    fwrite(deamon,sizeof(enemy),4,game_file);
    fwrite(fire_breathing,sizeof(enemy),4,game_file);
    fwrite(giant,sizeof(enemy),4,game_file);
    fwrite(snake,sizeof(enemy),4,game_file);
    fwrite(undeed,sizeof(enemy),4,game_file);
    for(int i=0;i<LINES;i++){
        fwrite(floor_one[i],sizeof(char),COLS,game_file);
    }   
    for(int i=0;i<LINES;i++){
        fwrite(floor_two[i],sizeof(char),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fwrite(floor_three[i],sizeof(char),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fwrite(floor_four[i],sizeof(char),COLS,game_file);
    }
    fwrite(visited_one,sizeof(int),MAX_ROOMS,game_file);
    fwrite(visited_two,sizeof(int),MAX_ROOMS,game_file);  
    fwrite(visited_three,sizeof(int),MAX_ROOMS,game_file);
    fwrite(visited_four,sizeof(int),MAX_ROOMS,game_file);
    for(int i=0;i<LINES;i++){
        fwrite(visit_map_one[i],sizeof(int),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fwrite(visit_map_two[i],sizeof(int),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fwrite(visit_map_three[i],sizeof(int),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fwrite(visit_map_four[i],sizeof(int),COLS,game_file);
    }
    fwrite(enchanted,sizeof(int),4 * 1,game_file);
    fwrite(&Heart,sizeof(int),1,game_file);
    fwrite(&Gold,sizeof(int),1,game_file);
    fwrite(&Health,sizeof(int),1,game_file);
    fwrite(&hunger,sizeof(int),1,game_file);
    fwrite(&food_collected,sizeof(int),1,game_file);
    fwrite(&dagger_count,sizeof(int),1,game_file);
    fwrite(&magic_wand_count,sizeof(int),1,game_file);
    fwrite(&normal_arrow_count,sizeof(int),1,game_file);
    fwrite(&sword_count,sizeof(int),1,game_file);
    fwrite(&mace_damage,sizeof(int),1,game_file);
    fwrite(&dagger_damage,sizeof(int),1,game_file);
    fwrite(&magic_wand_damage,sizeof(int),1,game_file);
    fwrite(&normal_arrow_damage,sizeof(int),1,game_file);
    fwrite(&sword_damage,sizeof(int),1,game_file);
    fwrite(&default_weapon,sizeof(char),1,game_file);
    fwrite(&last_hit,sizeof(char),1,game_file);
    fwrite(&spell,sizeof(char),1,game_file);
    fwrite(&health_spell,sizeof(int),1,game_file);
    fwrite(&speed_spell,sizeof(int),1,game_file);
    fwrite(&damage_spell,sizeof(int),1,game_file);
    fwrite(&health_count,sizeof(int),1,game_file);
    fwrite(&speed_count,sizeof(int),1,game_file);
    fwrite(&damage_count,sizeof(int),1,game_file);
    fwrite(&enchanted_stay,sizeof(int),1,game_file);
    fwrite(&which_floor_save,sizeof(int),1,game_file);
    fwrite(&posx_save,sizeof(int),1,game_file);
    fwrite(&posy_save,sizeof(int),1,game_file);
    fwrite(&end,sizeof(int),1,game_file);
    fwrite(&win,sizeof(int),1,game_file);
    fclose(game_file);    
}



void game(int entering,int which_floor,char **floor,room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],trap traps[MAX_ROOMS][10],int visited[MAX_ROOMS],int **visit_map){
    int ch;
    int position_x;
    int position_y;
    int show_status = 0; // 0 = game page, 1 = food status, 2= weapon status, 3=spell status
    time_t last_hunger_update = time(NULL);
    time_t food_change=time(NULL);
    int change=0;
    time_t attack_time=0;
    double elapsed;
    if(entering==1){
        int visited_floor=0;
        for(int i=0;i<MAX_ROOMS;i++){
            if(visited[i]==1){
                visited_floor=1;
                break;
            }
        }
        int room_index=7;
        if(which_floor==3){
            int index=find_room(stair.y,stair.x,rooms);
            do{
               room_index=rand() % MAX_ROOMS;
            }
            while(room_index==index || room_index==1 || (visited_floor==1 && visited[room_index]==0));
            do{
            position_x=(rand() % (rooms[room_index].width-2))+rooms[room_index].x+1;
            position_y=(rand() % (rooms[room_index].height-2))+rooms[room_index].y+1;
            }
            while(floor[position_y][position_x]=='O');
        }
        else{
            int index=find_room(stair.y,stair.x,rooms);
            do{
               room_index=rand() % MAX_ROOMS;
            }
            while(room_index==index || (visited_floor==1 && visited[room_index]==0));
            do{
            position_x=(rand() % (rooms[room_index].width-2))+rooms[room_index].x+1;
            position_y=(rand() % (rooms[room_index].height-2))+rooms[room_index].y+1;
            }
            while(floor[position_y][position_x]=='O');
        }
    // position_x=stair.x;
    // position_y=stair.y;
    }
    else if(entering==0){
    if(choice_level==2) Heart=75;
    else if(choice_level==3) Heart=50;
    int room_index=15;
    do{
       room_index=rand() % MAX_ROOMS; 
    }
    while(room_index==stair.room_index);
    do{
        position_x=(rand() % (rooms[room_index].width-2))+rooms[room_index].x+1;
        position_y=(rand() % (rooms[room_index].height-2))+rooms[room_index].y+1;
    }
    while(floor[position_y][position_x]=='O');
    message_box(2,which_floor);
    usleep(1000000);
    }
    else if(entering==2){
      position_x=posx_save;
      position_y=posy_save;
      for(int j=3;j<LINES-3;j++){
        for(int i=0;i<COLS;i++){
            if(visit_map[j][i]==1){
                int which_room=find_room(j,i,rooms);
                if(which_room!=-1){
                if(strcmp(rooms[which_room].theme,"")!=0){
                    room_color(floor,rooms,which_room,0);    
                }
                else mvaddch(j,i,floor[j][i]);
                }
                else mvaddch(j,i,floor[j][i]);
            }
        }
    }
    refresh();
    for(int j=3;j<LINES-3;j++){
        for(int i=0;i<COLS;i++){
            if(visit_map[j][i]==1){
                if(floor[j][i]=='$'){
                    attron(COLOR_PAIR(2));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(2));
                }
                else if(floor[j][i]=='~'){
                    attron(COLOR_PAIR(1));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(1));
                }
                else if(floor[j][i]=='f' || floor[j][i]=='p' || floor[j][i]=='w'){
                    attron(COLOR_PAIR(4));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(4));
                }
                else if(floor[j][i]=='0'){
                    attron(COLOR_PAIR(4));
                    mvprintw(j,i,"%c",'f');
                    attroff(COLOR_PAIR(4));
                }
                else if(floor[j][i]=='d' || floor[j][i]=='1'){
                    const wchar_t Dagger[]= L"\U0001F5E1";
                    mvaddnwstr(j,i,Dagger,-1);
                }
                else if(floor[j][i]=='a' || floor[j][i]=='3'){
                    const wchar_t Magic_wand[] = L"\u2605";
                    mvaddnwstr(j,i,Magic_wand,-1);
                }
                else if(floor[j][i]=='n' || floor[j][i]=='2'){
                    const wchar_t Normal_Arrow[] = L"\u27B3";
                    mvaddnwstr(j,i,Normal_Arrow,-1);
                }
                else if(floor[j][i]=='s'){
                    const wchar_t Sword[] = L"\u2694";
                    mvaddnwstr(j,i,Sword,-1);
                }
                else if(floor[j][i]=='I'){
                    attron(COLOR_PAIR(2));
                    const wchar_t Treasure[] = L"\u2726";
                    mvaddnwstr(j,i,Treasure,-1);
                    attroff(COLOR_PAIR(2));
                }
            }
        }
    }
    refresh();
    }
    int prev_x=position_x;
    int prev_y=position_y;

    while(1){                                                                //GAME ON    
    /////////////////////////////////////////////////////////// if health=0 you lostttttt  
    if(end) break;


    if(show_status==0){  
    message_box(22,which_floor);       
    status_box();

    
    
    time_t current_time = time(NULL);
    if(difftime(current_time,food_change)>=20 && change==0){
        for(int j=0;j<LINES;j++){
            for(int i=0;i<COLS;i++){
                if(floor[j][i]=='p' || floor[j][i]=='w') floor[j][i]='f';
                else if(floor[j][i]=='f') floor[j][i]='0';
                change=1;
            }
        }
    }
    if(attack_time!=0){
        elapsed=difftime(current_time,attack_time);
        if(elapsed>=3 && hunger==0){
            Heart+=5;
            attack_time=0;
        }
    } 
    if(hunger>=HUNGER_MAX-1){
        message_box(23,which_floor);
        beep();
        ch=getch();
        if(ch=='E'){
            show_status=1;
        }
    }
    else if(hunger>(HUNGER_MAX/2)){
        message_box(25,which_floor);
        beep();
        Health--;
        if(Health<=0){
            win=-1;
            show_status=4;
            continue;
        }
        ch=getch();
        if(ch=='E'){
            show_status=1;
        }
    }
    if (current_time - last_hunger_update >= HUNGER_INTERVAL) {
            hunger++;
            if (hunger > HUNGER_MAX) hunger = HUNGER_MAX;
            last_hunger_update = current_time;
    }       
    mvaddch(prev_y,prev_x,floor[prev_y][prev_x]);
    visit_map[prev_y][prev_x]=1;
    refresh();
    int which_room=find_room(position_y,position_x,rooms);
    if(which_room!=-1){
        if(visited[which_room]==0) message_box(41,which_floor);
        visited[which_room]=1;
        room_visibility(which_floor,which_room,floor,rooms,doors,visit_map);
        if(strcmp(rooms[which_room].theme,"enchanted")!=0) enchanted_stay=0;
        if(enchanted_stay>30){
                    message_box(27,which_floor);
                    Heart--;
                    if(Heart<=0){
                       win=-1;
                       show_status=4;
                       continue;
                    }
        }
        if(strcmp(rooms[which_room].theme,"enchanted")==0 && (choice_music==1 || choice_music==2)){
           switch_song(song3);
        }
        else if(which_floor==3 && which_room==1 && (choice_music==1 || choice_music==2)){
           switch_song(song4);
        }
        else{
            if(choice_music==1) switch_song(song1);
            else if(choice_music==2) switch_song(song2);
        }
        //refresh();
        update_monsters(which_floor,which_room,position_x,position_y,floor,rooms);
    } 
    else{
        corridor_visibility(position_y,position_x,which_floor,floor,visit_map);
        enchanted_stay=0;
    }
    if(which_floor==3){
    if(undeed[which_floor].alive==0 && snake[which_floor].alive==0 && giant[which_floor].alive==0 && fire_breathing[which_floor].alive==0 && deamon[which_floor].alive==0){
            win=1;
            Gold+=100;
            show_status=4;
            continue;
    }}                   
    if(which_floor==3 && which_room==1){
        hidden_doors(3,floor,doors,rooms,0);
    }
    if((position_x>0 && deamon[which_floor].current_x==position_x-1 && deamon[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && deamon[which_floor].current_x==position_x+1 && deamon[which_floor].current_y==position_y) ||
        (position_y>0 && deamon[which_floor].current_y==position_y-1 && deamon[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && deamon[which_floor].current_y==position_y+1 && deamon[which_floor].current_x==position_x)){
        Heart--;
        if(Heart<=0){
                       win=-1;
                       show_status=4;
                       continue;
                    }
        message_box(28,which_floor);
        attack_time=time(NULL);
    }
    if((position_x>0 && fire_breathing[which_floor].current_x==position_x-1 && fire_breathing[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && fire_breathing[which_floor].current_x==position_x+1 && fire_breathing[which_floor].current_y==position_y) ||
        (position_y>0 && fire_breathing[which_floor].current_y==position_y-1 && fire_breathing[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && fire_breathing[which_floor].current_y==position_y+1 && fire_breathing[which_floor].current_x==position_x)){
        Heart-=2;
        if(Heart<=0){
                       win=-1;
                       show_status=4;
                       continue;
                    }
        message_box(29,which_floor);
        attack_time=time(NULL);
    }
    if((position_x>0 && giant[which_floor].current_x==position_x-1 && giant[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && giant[which_floor].current_x==position_x+1 && giant[which_floor].current_y==position_y) ||
        (position_y>0 && giant[which_floor].current_y==position_y-1 && giant[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && giant[which_floor].current_y==position_y+1 && giant[which_floor].current_x==position_x)){
        Heart-=3;
        if(Heart<=0){
                       win=-1;
                       show_status=4;
                       continue;
                    }
        message_box(30,which_floor);
        attack_time=time(NULL);
    }
    if((position_x>0 && snake[which_floor].current_x==position_x-1 && snake[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && snake[which_floor].current_x==position_x+1 && snake[which_floor].current_y==position_y) ||
        (position_y>0 && snake[which_floor].current_y==position_y-1 && snake[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && snake[which_floor].current_y==position_y+1 && snake[which_floor].current_x==position_x)){
        Heart-=4;
        if(Heart<=0){
                       win=-1;
                       show_status=4;
                       continue;
                    }
        message_box(31,which_floor);
        attack_time=time(NULL);
    }
    if((position_x>0 && undeed[which_floor].current_x==position_x-1 && undeed[which_floor].current_y==position_y) ||
        (position_x<COLS-1 && undeed[which_floor].current_x==position_x+1 && undeed[which_floor].current_y==position_y) ||
        (position_y>0 && undeed[which_floor].current_y==position_y-1 && undeed[which_floor].current_x==position_x) ||
        (position_y<LINES-1 && undeed[which_floor].current_y==position_y+1 && undeed[which_floor].current_x==position_x)){
        Heart-=5;
        if(Heart<=0){
                       win=-1;
                       show_status=4;
                       continue;
                    }
        message_box(32,which_floor);
        attack_time=time(NULL);
    }
    for(int j=3;j<LINES-3;j++){
        for(int i=0;i<COLS;i++){
            if(visit_map[j][i]==1){
                which_room=find_room(j,i,rooms);
                if(which_room!=-1){
                if(strcmp(rooms[which_room].theme,"")!=0){
                    room_color(floor,rooms,which_room,0);    
                }
                else mvaddch(j,i,floor[j][i]);
                }
                else mvaddch(j,i,floor[j][i]);
            }
        }
    }
    for(int j=3;j<LINES-3;j++){
        for(int i=0;i<COLS;i++){
            if(visit_map[j][i]==1){
                if(floor[j][i]=='$'){
                    attron(COLOR_PAIR(2));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(2));
                }
                else if(floor[j][i]=='~'){
                    attron(COLOR_PAIR(1));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(1));
                }
                else if(floor[j][i]=='f' ||  floor[j][i]=='p' || floor[j][i]=='w'){
                    attron(COLOR_PAIR(4));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(4));
                }
                else if(floor[j][i]=='0'){
                    attron(COLOR_PAIR(4));
                    mvprintw(j,i,"%c",'f');
                    attroff(COLOR_PAIR(4));
                }
                else if(floor[j][i]=='d' || floor[j][i]=='1'){
                    const wchar_t Dagger[]= L"\U0001F5E1";
                    mvaddnwstr(j,i,Dagger,-1);
                }
                else if(floor[j][i]=='a' || floor[j][i]=='3'){
                    const wchar_t Magic_wand[] = L"\u2605";
                    mvaddnwstr(j,i,Magic_wand,-1);
                }
                else if(floor[j][i]=='n' || floor[j][i]=='2'){
                    const wchar_t Normal_Arrow[] = L"\u27B3";
                    mvaddnwstr(j,i,Normal_Arrow,-1);
                }
                else if(floor[j][i]=='s'){
                    const wchar_t Sword[] = L"\u2694";
                    mvaddnwstr(j,i,Sword,-1);
                }
                else if(floor[j][i]=='I'){
                    attron(COLOR_PAIR(2));
                    const wchar_t Treasure[] = L"\u2726";
                    mvaddnwstr(j,i,Treasure,-1);
                    attroff(COLOR_PAIR(2));
                }
            }
        }
    }
    display_monsters(which_floor,floor,visited);
    //refresh();
    // for(int i=0;i<MAX_ROOMS;i++){
    //     if(visited[i]==1){
    //         room_visibility(i,floor,rooms,doors);
    //     }
    // }
    // refresh();
    door hidden[MAX_ROOMS][40];
     if(which_floor==0){
        for(int i=0;i<MAX_ROOMS;i++){
            for(int j=0;j<40;j++){
                hidden[i][j]=hidden_door_one[i][j];
            }
        }
     }
     else if(which_floor==1){
        for(int i=0;i<MAX_ROOMS;i++){
            for(int j=0;j<40;j++){
                hidden[i][j]=hidden_door_two[i][j];
            }
        }
     } 
     else if(which_floor==2){
        for(int i=0;i<MAX_ROOMS;i++){
            for(int j=0;j<40;j++){
                hidden[i][j]=hidden_door_three[i][j];
            }
        }

     } 
     else{
        for(int i=0;i<MAX_ROOMS;i++){
            for(int j=0;j<40;j++){
                hidden[i][j]=hidden_door_four[i][j];
            }
        }
     }
    for(int i=0;i<MAX_ROOMS;i++){
        for(int j=0;j<10;j++){
            if(traps[i][j].x  && traps[i][j].y && traps[i][j].x==position_x && traps[i][j].y==position_y){
                if(Heart>0){
                    Heart--;
                    if(Heart<=0){
                       win=-1;
                       show_status=4;
                       continue;
                    }
                    floor[position_y][position_x]='^';
                    message_box(3,which_floor);
                } 
            }
        }
    }
    for(int i=0;i<MAX_ROOMS;i++){
        for(int j=0;j<40;j++){
            if(hidden[i][j].x==position_x && hidden[i][j].y==position_y){
                floor[hidden[i][j].y][hidden[i][j].x]='?';
            }
        }
    }
    if(position_x==stair.x && position_y==stair.y){
        message_box(4,which_floor);
        //usleep(2000000);
    }
    else if(floor[position_y][position_x]=='$'){
       Gold++; 
       message_box(20,which_floor);
       floor[position_y][position_x]='.';
    }
    else if(floor[position_y][position_x]=='~'){
       Gold+=3;
       message_box(21,which_floor);
       floor[position_y][position_x]='.';
    }
    else if(floor[position_y][position_x]=='I'){
        position_x=rooms[1].x+(rand()%(rooms[1].width-2))+1;
        position_y=rooms[1].y+(rand()%(rooms[1].height-2))+1;
        room_visibility(which_floor,1,floor,rooms,doors,visit_map);
        refresh();
        continue;
    }
    refresh();
    if(color) attron(COLOR_PAIR(5));
    mvaddch(position_y,position_x,'P');
    if(color) attroff(COLOR_PAIR(5));
    refresh();
    ch=getch();
    if(ch=='q' && win!=1 && win!=-1){  
      clear(); 
      which_floor_save=which_floor;
      posx_save=position_x;
      posy_save=position_y;
      save(current_player.user_name);
      end=1;
      continue;
    } 
    prev_x=position_x;
    prev_y=position_y;
    if(ch=='f'){
        int dx=0,dy=0;
        ch=getch();
        if(ch == '1'){dx=-1; dy=1;} 
        else if(ch == '2') dy=1;
        else if(ch == '3'){dx=1; dy=1;}
        else if(ch == '4') dx=-1;
        else if(ch == '6') dx=1;
        else if(ch == '7'){dx=-1; dy=-1;}
        else if(ch == '8') dy=-1;
        else if(ch == '9'){dx=1; dy=-1;}
        if(dx != 0 || dy!=0){
            run(which_floor,&position_x,&position_y,dx,dy,floor,rooms,doors,visited,visit_map);
        }
    }
    else if(position_x==stair.x && position_y==stair.y && (ch=='u' || ch=='d')){
        if(!(which_floor==0 && ch=='d') && !(which_floor==3 && ch=='u')){
        for(int j=3;j<LINES-3;j++){
            for(int i=0;i<COLS;i++){
                mvaddch(j,i,' ');
            }
        }
        }
        if(ch=='u'){
            if(which_floor<3){
            which_floor++;
            if(which_floor==1){
               message_box(8,which_floor);
               usleep(2000000);
               game(1,1,floor_two,rooms_two,doors_two,traps_two,visited_two,visit_map_two);
            } 
            else if(which_floor==2){
               message_box(9,which_floor);
               usleep(2000000);
               game(1,2,floor_three,rooms_three,doors_three,traps_three,visited_three,visit_map_three);
            } 
            else if(which_floor==3){
               message_box(10,which_floor);
               usleep(2000000);
               game(1,3,floor_four,rooms_four,doors_four,traps_four,visited_four,visit_map_four);
            } 
           }
           else{
            message_box(5,which_floor);
            usleep(2000000);
           }
        }
        else{
            if(which_floor>0){
               which_floor--;
               if(which_floor==0){
                message_box(11,which_floor);
                usleep(2000000);
                game(1,0,floor_one,rooms_one,doors_one,traps_one,visited_one,visit_map_one);
               } 
               else if(which_floor==1){
                message_box(8,which_floor);
                usleep(2000000);
                game(1,1,floor_two,rooms_two,doors_two,traps_two,visited_two,visit_map_two);
               } 
               else if(which_floor==2){
                message_box(9,which_floor);
                usleep(2000000);
                game(1,2,floor_three,rooms_three,doors_three,traps_three,visited_three,visit_map_three);
               } 
            }
            else{
                message_box(6,which_floor);
                usleep(2000000);
            }
        }
    }
    else if(ch=='M'){
        for(int i=0;i<MAX_ROOMS;i++){
            watch_room(which_floor,i,floor,rooms,doors,visit_map);
            for(int j=3;j<LINES-3;j++){
                for(int i=0;i<COLS;i++){
                    if(floor[j][i]=='$'){
                    attron(COLOR_PAIR(2));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(2));
                    }
                    else if(floor[j][i]=='~'){
                    attron(COLOR_PAIR(1));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(1));
                    }
                    else if(floor[j][i]=='f' || floor[j][i]=='p' || floor[j][i]=='w'){
                    attron(COLOR_PAIR(4));
                    mvprintw(j,i,"%c",floor[j][i]);
                    attroff(COLOR_PAIR(4));
                    }
                    else if(floor[j][i]=='0'){
                    attron(COLOR_PAIR(4));
                    mvprintw(j,i,"%c",'f');
                    attroff(COLOR_PAIR(4));
                    }
                else if(floor[j][i]=='d' || floor[j][i]=='1'){
                    const wchar_t Dagger[]= L"\U0001F5E1";
                    mvaddnwstr(j,i,Dagger,-1);
                }
                else if(floor[j][i]=='a' || floor[j][i]=='3'){
                    const wchar_t Magic_wand[] = L"\u2605";
                    mvaddnwstr(j,i,Magic_wand,-1);
                }
                else if(floor[j][i]=='n' || floor[j][i]=='2'){
                    const wchar_t Normal_Arrow[] = L"\u27B3";
                    mvaddnwstr(j,i,Normal_Arrow,-1);
                }
                else if(floor[j][i]=='s'){
                    const wchar_t Sword[] = L"\u2694";
                    mvaddnwstr(j,i,Sword,-1);
                }
                else if(floor[j][i]=='I'){
                    attron(COLOR_PAIR(2));
                    const wchar_t Treasure[] = L"\u2726";
                    mvaddnwstr(j,i,Treasure,-1);
                    attroff(COLOR_PAIR(2));
                }
                }
            }
        }
        refresh();
        int ch_copy=getch();
        if(ch_copy=='M'){
            for(int i=0;i<MAX_ROOMS;i++){
                if(visited[i]==0){
                    for(int y=rooms[i].y;y<rooms[i].y+rooms[i].height;y++){
                        for(int x=rooms[i].x;x<rooms[i].x+rooms[i].width;x++){
                            mvaddch(y,x,' ');
                        }
                    }
                }
            }
            refresh();
        }
    }
    else if(ch=='s'){
        for(int i=0;i<MAX_ROOMS;i++){
        for(int j=0;j<10;j++){
            if(position_x>0 && traps[i][j].x==position_x-1 && traps[i][j].y==position_y){
                message_box(12,which_floor);
            }
            if(position_x<COLS-1 && traps[i][j].x==position_x+1 && traps[i][j].y==position_y){
                message_box(13,which_floor);
            }
            if(position_y>0 && traps[i][j].x==position_x && traps[i][j].y==position_y-1){
                message_box(14,which_floor);
            }
            if(position_y<LINES-1 && traps[i][j].x==position_x && traps[i][j].y==position_y+1){
                message_box(15,which_floor);
            }
        }
    }
    for(int i=0;i<MAX_ROOMS;i++){
        for(int j=0;j<40;j++){
            if(position_x>0 && hidden[i][j].x==position_x-1 && hidden[i][j].y==position_y){
                message_box(16,which_floor);
            }
            if(position_x<COLS-1 && hidden[i][j].x==position_x+1 && hidden[i][j].y==position_y){
                message_box(17,which_floor);
            }
            if(position_y>0 && hidden[i][j].x==position_x && hidden[i][j].y==position_y-1){
                message_box(18,which_floor);
            }
            if(position_y<LINES && hidden[i][j].x==position_x && hidden[i][j].y==position_y+1){
                message_box(19,which_floor);
            }
        }
    }
    }
    else if(ch=='c'){
        if(floor[position_y][position_x]=='f'){
            if(food_collected<MAX_FOOD){
            food_collected++;
            floor[position_y][position_x]='.';
            }
            else{
                message_box(24,which_floor);
            }
        }
        else if(floor[position_y][position_x]=='p'){
            if(food_collected<MAX_FOOD){
            food_collected++;
            spell='d';
            mace_damage=10;
            sword_damage=20;
            dagger_damage=24;
            magic_wand_damage=30;
            normal_arrow_damage=10;
            floor[position_y][position_x]='.';
            }
            else{
                message_box(24,which_floor);
            }
        }
        else if(floor[position_y][position_x]=='w'){
            if(food_collected<MAX_FOOD){
            food_collected++;
            spell='s';
            floor[position_y][position_x]='.';
            }
            else{
                message_box(24,which_floor);
            }
        }
        else if(floor[position_y][position_x]=='0'){
            Health--;
            floor[position_y][position_x]='.';
            message_box(42,which_floor);
        }
        else if(floor[position_y][position_x]=='d'){
            dagger_count+=10;
            floor[position_y][position_x]='.';
        }
        else if(floor[position_y][position_x]=='a'){
            magic_wand_count+=8;
            floor[position_y][position_x]='.';
        }
        else if(floor[position_y][position_x]=='n'){
            normal_arrow_count+=20;
            floor[position_y][position_x]='.';
        }
        else if(floor[position_y][position_x]=='s'){
            if(sword_count==0){
            sword_count++;
            floor[position_y][position_x]='.';
            }
            else {
                message_box(26,which_floor);
            }
        }
        else if(floor[position_y][position_x]=='H'){
            health_spell++;
            floor[position_y][position_x]='.';
        }   
        else if(floor[position_y][position_x]=='S'){
            speed_spell++;
            floor[position_y][position_x]='.';
        } 
        else if(floor[position_y][position_x]=='D'){
            damage_spell++;
            floor[position_y][position_x]='.';
        }
        else if(floor[position_y][position_x]=='1'){
            dagger_count++;
            floor[position_y][position_x]='.';
        }
        else if(floor[position_y][position_x]=='2'){
            normal_arrow_count++;
            floor[position_y][position_x]='.';
        }
        else if(floor[position_y][position_x]=='3'){
            magic_wand_count++;
            floor[position_y][position_x]='.';
        }
    }
    else if(ch=='E'){
        show_status=1;
    }
    else if(ch=='i'){
        show_status=2;
    }
    else if(ch=='S'){
        show_status=3;
    }
    else if(ch==' '){
        if(spell=='d' && damage_count<5) damage_count++;
        if(spell=='d' && damage_count>=5){
            spell='n';
            damage_count=0;
            mace_damage=5;
            sword_damage=10;
            dagger_damage=12;
            magic_wand_damage=15;
            normal_arrow_damage=5;
    }
        if(default_weapon==' '){
           message_box(33,which_floor);
        }
        else{
            fight(floor,which_floor,rooms,position_x,position_y,1);
        }
    }
    else if(ch=='a'){
         if(default_weapon==' '){
           message_box(33,which_floor);
        }
        else{
            fight(floor,which_floor,rooms,position_x,position_y,0);
        }
    }
    else{
        move_character(&position_x,&position_y,ch,floor,which_floor,rooms);
        if(spell=='s' && speed_count<10){
            move_character(&position_x,&position_y,ch,floor,which_floor,rooms);
            speed_count++;
        }
        else if(spell=='s' && speed_count>=10){
            spell='n';
            speed_count=0;
        }  
    }
    }
    else if(show_status==1){
        clear();
        attron(COLOR_PAIR(4));
        mvprintw(0, 0, "=== FOOD STATUS ===");
        attroff(COLOR_PAIR(4));
            draw_hunger_bar(hunger);
            mvprintw(4, 0, "Food collected: %d/%d", food_collected, MAX_FOOD);
            mvprintw(6, 0, "Press 'f' to eat and 'E' to return to the game");

            
            int ch = getch();
            if(ch=='f'){
                if (food_collected > 0 && hunger > 0) {
                    food_collected--;
                    hunger--;
                    if(spell=='h' && health_count<10){
                       Health+=2;
                       health_count++;
                    } 
                    else if(spell=='h' && health_count>=10){
                        spell='n';
                        health_count=0;
                    }
                    else Health++;
                }
            }
            else if (ch == 'E') {
                show_status = 0; 
                clear();
                message_box(22,which_floor);
                status_box();
                refresh();
            }
    }
    else if(show_status==2){
        clear();
        if(spell=='d' && damage_count<5){
            mace_damage=10;
            sword_damage=20;
            dagger_damage=24;
            magic_wand_damage=30;
            normal_arrow_damage=10;
        }
        else {
            if(spell=='d' && damage_count>=5){
                spell='n';
                damage_count=0;
            }
            mace_damage=5;
            sword_damage=10;
            dagger_damage=12;
            magic_wand_damage=15;
            normal_arrow_damage=5;
        }
        attron(COLOR_PAIR(1));
        mvprintw(0,0,"=== WEAPON STATUS ===");
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(2));
        mvprintw(4,0,"=== SHORT-RANGE ===");
        attroff(COLOR_PAIR(2));
        mvprintw(6,0,"MACE(m)          DAMAGE:%d    COUNT:1",mace_damage);
        mvprintw(8,0,"SWORD(s)         DAMAGE:%d    COUNT:%d" ,sword_damage,sword_count);
        attron(COLOR_PAIR(3));
        mvprintw(12,0,"=== LONG-RANGE ===");
        attroff(COLOR_PAIR(3));
        mvprintw(14,0,"DAGGER(d)        DAMAGE:%d  COUNT:%d  DISTANCE:5" ,dagger_damage, dagger_count);
        mvprintw(16,0,"MAGGIC WAND(a)   DAMAGE:%d  COUNT:%d  DISTANCE:10" ,magic_wand_damage, magic_wand_count);
        mvprintw(18,0,"NORMAL ARROW(n)  DAMAGE:%d  COUNT:%d  DISTANCE:5" ,normal_arrow_damage, normal_arrow_count);
        attron(COLOR_PAIR(1));
        mvprintw(20,0,"CURRENT WEAPON IS : %c" , default_weapon);
        attroff(COLOR_PAIR(1));
        mvprintw(22,0,"Press 'w' to place your weapon into the bag.");
        mvprintw(24,0,"Press the corresponding characters of the weapons to change them into default weapon.");
        mvprintw(26,0,"Press 'i' to return to the game");
        int ch=getch();
        if(default_weapon==' '){
        if(ch=='m') default_weapon='m';
        else if(ch=='s'){
        if(sword_count>0){
           default_weapon='s';
        } 
        else {
            mvprintw(2,0,"We're out of swords.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        } 
        else if(ch=='d'){
        if(dagger_count>0){
          default_weapon='d';
        } 
        else {
            mvprintw(2,0,"We're out of daggers.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        }
        else if(ch=='a'){
        if(magic_wand_count>0){
           default_weapon='a';
        } 
        else {
            mvprintw(2,0,"We're out of magic wands.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        }
        else if(ch=='n' && normal_arrow_count>0){
            if(normal_arrow_count>0){
              default_weapon='n';
            } 
            else{
            mvprintw(2,0,"We're out of normal arrows.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
            }
        } 
        else if(ch!='i' && ch!='m' && ch!='s' && ch!='d' && ch!='a' && ch!='n'){
            mvprintw(2,0,"Please press the keys that are listed here.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        }
        else{
        if(ch=='w'){
        default_weapon=' ';
        attron(COLOR_PAIR(1));
        mvprintw(20,0,"CURRENT WEAPON IS : %c" , default_weapon);
        attroff(COLOR_PAIR(1));
        ch=getch();
        if(ch=='m') default_weapon='m';
        else if(ch=='s'){
        if(sword_count>0){
          default_weapon='s';
        } 
        else {
            mvprintw(2,0,"We're out of swords.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        } 
        else if(ch=='d'){
        if(dagger_count>0){
           default_weapon='d';
        } 
        else {
            mvprintw(2,0,"We're out of daggers.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        }
        else if(ch=='a'){
        if(magic_wand_count>0){
           default_weapon='a';
        } 
        else {
            mvprintw(2,0,"We're out of magic wands.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        }
        else if(ch=='n' && normal_arrow_count>0){
            if(normal_arrow_count>0){
               default_weapon='n';
            } 
            else{
            mvprintw(2,0,"We're out of normal arrows.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
            }
        }
        else if(ch!='i' && ch!='m' && ch!='s' && ch!='d' && ch!='a' && ch!='n'){
            mvprintw(2,0,"Please press the keys that are listed here.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        } 
        else if(ch=='m' || ch=='s' || ch=='d' || ch=='a' || ch=='n'){
            mvprintw(2,0,"First put the current weapon in the bag.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                               ");
            refresh();
        }
        else if(ch!='i'){
            mvprintw(2,0,"Please press the keys that are listed here.");
            refresh();
            sleep(2);
            mvprintw(2,0,"                                                 ");
            refresh();
        }
        }
        attron(COLOR_PAIR(1));
        mvprintw(20,0,"CURRENT WEAPON IS : %c" , default_weapon);
        attroff(COLOR_PAIR(1));
        if(ch=='i'){
            show_status=0;
            clear();
            message_box(22,which_floor);
            status_box();
            refresh();
        }
    }
    else if(show_status==3){
        clear();
        attron(COLOR_PAIR(5));
        mvprintw(0,0,"=== SPELL STATUS ===");
        attroff(COLOR_PAIR(5));
        mvprintw(4,0,"HEALTH(h)   count:%d",health_spell);
        mvprintw(6,0,"SPEED(s)    count:%d",speed_spell);
        mvprintw(8,0,"DAMAGE(d)   count:%d",damage_spell);
        mvprintw(10,0,"CURRENT SPELL IS:%c" , spell);
        mvprintw(12,0,"Press 'S' to return to the game.");
        mvprintw(14,0,"Press 'h' , 's' and 'd' to use the spells.");
        mvprintw(16,0,"Press 'n' if you don't want to use any spell.");
        ch=getch();
        if(ch=='h' && health_spell>0){
          spell='h';
          health_spell--;
        }
        else if(ch=='s' && speed_spell>0){
          spell='s';
          speed_spell--;
        }
        else if(ch=='d' && damage_spell>0){
          spell='d';
          damage_spell--;
          mace_damage=10;
          dagger_damage=24;
          magic_wand_damage=30;
          normal_arrow_damage=10;
          sword_damage=20;
        }
        else if(ch=='n'){
            spell='n';
        }
        else if(ch=='S'){
            show_status=0;
            clear();
            message_box(22,which_floor);
            status_box();
            refresh();
        }
    }
    else if(show_status==4){
       clear();
       char file_name[105];
       snprintf(file_name,sizeof(file_name),"%s.dat",current_player.user_name);
       if(access(file_name,F_OK)==0){
        remove(file_name);
       }
       if(win==1){    
       printw("   ____     __   ,-----.      ___    _         .--.      .--.    ,-----.    ,---.   .--.         .---.  \n");
       printw("   \\   \\   /  /.'  .-,  '.  .'   |  | |        |  |_     |  |  .'  .-,  '.  |    \\  |  |         \\   /  \n");
       printw("    \\  _. /  '/ ,-.|  \\ _ \\ |   .'  | |        | _( )_   |  | / ,-.|  \\ _ \\ |  ,  \\ |  |         |   |  \n");
       printw("     _( )_ .';  \\  '_ /  | :.'  '_  | |        |(_ o _)  |  |;  \\  '_ /  | :|  |\\_ \\|  |          \\ /   \n");
       printw(" ___(_ o _)' |  _`,/ \\ _/  |'   ( \\.-.|        | (_,_) \\ |  ||  _`,/ \\ _/  ||  _( )_\\  |           v    \n");
       printw("|   |(_,_)'  : (  '\\_/ \\   ;' (`. _` /|        |  |/    \\|  |: (  '\\_/ \\   ;| (_ o _)  |          _ _   \n");
       printw("|   `-'  /    \\ `\"/  \\  ) / | (_ (_) _)        |  '  /\\  `  | \\ `\"/  \\  ) / |  (_,_)\\  |         (_I_)  \n");
       printw(" \\      /      '. \\_/``\".'   \\ /  . \\ /        |    /  \\    |  '. \\_/``\".'  |  |    |  |        (_(=)_) \n");
       printw("  `-..-'         '-----'      ``-'`-''         `---'    `---`    '-----'    '--'    '--'         (_I_)  \n");
    }
    else if(win==-1){
        printw("   ____     __   ,-----.      ___    _           .---.       ,-----.       .-'''-. ,---------.          .---.  \n");
        printw("   \\   \\   /  /.'  .-,  '.  .'   |  | |          | ,_|     .'  .-,  '.    / _     \\\\          \\         \\   /  \n");
        printw("    \\  _. /  '/ ,-.|  \\ _ \\ |   .'  | |        ,-./  )    / ,-.|  \\ _ \\  (`' )/`--' `--.  ,---'         |   |  \n");
        printw("     _( )_ .';  \\  '_ /  | :.'  '_  | |        \\  '_ '`) ;  \\  '_ /  | :(_ o _).       |   \\             \\ /   \n");
        printw(" ___(_ o _)' |  _`,/ \\ _/  |'   ( \\.-.|         > (_)  ) |  _`,/ \\ _/  | (_,_). '.     :_ _:              v    \n");
        printw("|   |(_,_)'  : (  '\\_/ \\   ;' (`. _` /|        (  .  .-' : (  '\\_/ \\   ;.---.  \\  :    (_I_)             _ _   \n");
        printw("|   `-'  /    \\ `\"/  \\  ) / | (_ (_) _)         `-'`-'|___\\ `\"/  \\  ) / \\    `-'  |   (_(=)_)           (_I_)  \n");
        printw(" \\      /      '. \\_/``\".'   \\ /  . \\ /          |        \\'. \\_/``\".'   \\       /     (_I_)           (_(=)_) \n");
        printw("  `-..-'         '-----'      ``-'`-''           `--------`  '-----'      `-...-'      '---'            (_I_)  \n");
    }
    refresh();
    char ch=getch();
    show_status=5;
    }
    else if(show_status==5){
        end=1;
        clear();
        attron(COLOR_PAIR(5));
        mvprintw(0,0,"=== Game Over ===");
        attroff(COLOR_PAIR(5));
        mvprintw(2,0,"You're score is %d",Gold);
        attron(COLOR_PAIR(1));
        if(win==1) mvprintw(4,0,"You have won!");
        else if(win==-1) mvprintw(4,0,"You have lost!");
        attroff(COLOR_PAIR(1));
        refresh();
        char ch=getch();
        break;
    }
    }
}

int obstacle(char c){
    return strchr(obstacles,c)!=NULL;
}

void run(int which_floor,int *position_x,int *position_y,int dx,int dy,char **floor_one,room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],int visited[MAX_ROOMS],int **visit_map){
    int prev_x=*position_x,prev_y=*position_y;
    trap traps[MAX_ROOMS][10];
    for(int i=0;i<MAX_ROOMS;i++){
        for(int j=0;j<10;j++){
        if(which_floor==0){
           traps[i][j]=traps_one[i][j];
        }
        else if(which_floor==1){
           traps[i][j]=traps_two[i][j];
        }
        else if(which_floor==2){
           traps[i][j]=traps_three[i][j];
        }
        else{
           traps[i][j]=traps_four[i][j];
        }
        }
    }
    while(*position_x + dx >=0 && *position_x + dx < COLS && *position_y + dy >=0 && *position_y + dy < LINES &&
           !obstacle(floor_one[*position_y+dy][*position_x+dx])){
            int monster=1;
            if((deamon[which_floor].current_x==*position_x+dx && deamon[which_floor].current_y==*position_y+dy) ||
               (fire_breathing[which_floor].current_x==*position_x+dx && fire_breathing[which_floor].current_y==*position_y+dy) ||
               (giant[which_floor].current_x==*position_x+dx && giant[which_floor].current_y==*position_y+dy) ||
               (snake[which_floor].current_x==*position_x+dx && snake[which_floor].current_y==*position_y+dy) ||
               (undeed[which_floor].current_x==*position_x+dx && undeed[which_floor].current_y==*position_y+dy)){
                monster=0;
                break;
            }
            int problem=0;
            for(int i=0;i<MAX_ROOMS;i++){
                for(int j=0;j<10;j++){
                    if(traps[i][j].x==*position_x+dx && traps[i][j].y==*position_y+dy){
                        problem=1;
                        if(Heart>0){
                            Heart--;
                            floor_one[*position_y+dy][*position_x+dx]='^';
                            message_box(3,which_floor);
                            refresh();
                        }
                        break;
                    }
                }
                if(problem) break; 
            }
            //if(problem) break;
            mvaddch(prev_y,prev_x,floor_one[prev_y][prev_x]);
            visit_map[prev_y][prev_x]=1;
            //refresh();
            int which_room=find_room(*position_y,*position_x,rooms);
            if(which_room!=-1){
            visited[which_room]=1;    
            room_visibility(which_floor,which_room,floor_one,rooms,doors,visit_map);
            refresh();
            } 
            if(which_floor==3 && which_room==1){
            hidden_doors(3,floor_one,doors,rooms,0);
            }
            refresh();
            mvaddch(*position_y,*position_x,'P');
            refresh();
            prev_x=*position_x,prev_y=*position_y;
            if(monster){
            *position_x+=dx;
            *position_y+=dy;
            usleep(900);
            }
          }
        //   mvaddch(prev_y,prev_x,floor_one[prev_y][prev_x]);
        //   visit_map[prev_y][prev_x]=1;
        //   refresh();
        //   mvaddch(*position_y,*position_x,'P');
        //   refresh();
}


void reset_doors(char **floor_one,door doors[MAX_ROOMS][40],room rooms[MAX_ROOMS]){
     for(int i=0;i<MAX_ROOMS;i++){
        for(int j=0;j<40;j++){
            doors[i][j].x=0;
            doors[i][j].y=0;
        }
    }
    int door_room[MAX_ROOMS]={0};
    for(int y=0;y<LINES;y++){
        for(int x=0;x<COLS;x++){
            if(floor_one[y][x]=='+'){
                int found=find_room(y,x,rooms);
                doors[found][door_room[found]].x=x;
                doors[found][door_room[found]].y=y;
                door_room[found]++;
            }
        }
    }
}

void hidden_doors(int which_floor,char **floor,door doors[MAX_ROOMS][40],room rooms[MAX_ROOMS],int last_room){
    int idx;
    if(last_room){
    idx=rand() % MAX_ROOMS;
    if(which_floor==3){
    while(idx==1){
        idx=rand() % MAX_ROOMS;
    }
    }
    }
    else idx=1;
   for(int y=rooms[idx].y;y<rooms[idx].y+rooms[idx].height;y++){
        for(int x=rooms[idx].x;x<rooms[idx].x+rooms[idx].width;x++){
            if(floor[y][x]=='+'){
                if((x>0 && (floor[y][x-1]=='.' || floor[y][x-1]=='O' || floor[y][x-1]=='*'  || floor[y][x-1]=='~' || floor[y][x-1]=='$' || floor[y][x-1]=='f' || floor[y][x-1]=='p' || floor[y][x-1]=='w' || floor[y][x-1]=='0' || floor[y][x-1]=='s' || floor[y][x-1]=='n' || floor[y][x-1]=='a' || floor[y][x-1]=='d' || floor[y][x-1]=='H' || floor[y][x-1]=='S' || floor[y][x-1]=='D' || floor[y][x-1]=='I' || floor[y][x-1]=='^')) ||
                (x<COLS-1 && (floor[y][x+1]=='.' || floor[y][x+1]=='O' || floor[y][x+1]=='*' || floor[y][x+1]=='~' || floor[y][x+1]=='$' || floor[y][x+1]=='f' || floor[y][x+1]=='p' || floor[y][x+1]=='w' || floor[y][x+1]=='0' || floor[y][x+1]=='s' || floor[y][x+1]=='n' || floor[y][x+1]=='a' || floor[y][x+1]=='d' || floor[y][x+1]=='H' || floor[y][x+1]=='S' || floor[y][x+1]=='D' || floor[y][x+1]=='I' || floor[y][x+1]=='^'))){
                    floor[y][x]='|';
                }
                else{
                    floor[y][x]='_';
                }
            }
        }
    }
    if(last_room){
    for(int i=0;i<40;i++){
        if(which_floor==0){
        if(doors[idx][i].x && doors[idx][i].y){
            hidden_door_one[idx][i].x=doors[idx][i].x;
            hidden_door_one[idx][i].y=doors[idx][i].y;
        }
        }
        else if(which_floor==1){
            if(doors[idx][i].x && doors[idx][i].y){
            hidden_door_two[idx][i].x=doors[idx][i].x;
            hidden_door_two[idx][i].y=doors[idx][i].y;
        }
        }
        else if(which_floor==2){
            if(doors[idx][i].x && doors[idx][i].y){
            hidden_door_three[idx][i].x=doors[idx][i].x;
            hidden_door_three[idx][i].y=doors[idx][i].y;
        }
        }
        else{
            if(doors[idx][i].x && doors[idx][i].y){
            hidden_door_four[idx][i].x=doors[idx][i].x;
            hidden_door_four[idx][i].y=doors[idx][i].y;
        }
        }
    }
    strcpy(rooms[idx].theme,"enchanted");
    enchanted[which_floor][0]=idx;
    int position_x,position_y;
    for(int j=0;j<2;j++){
            position_x=rooms[idx].x+(rand()%(rooms[idx].width-2))+1;
            position_y=rooms[idx].y+(rand()%(rooms[idx].height-2))+1;
            if(floor[position_y][position_x]!='*'){
                floor[position_y][position_x]='H';
            }
            position_x=rooms[idx].x+(rand()%(rooms[idx].width-2))+1;
            position_y=rooms[idx].y+(rand()%(rooms[idx].height-2))+1;
            if(floor[position_y][position_x]!='*'){
                floor[position_y][position_x]='S';
            }
            position_x=rooms[idx].x+(rand()%(rooms[idx].width-2))+1;
            position_y=rooms[idx].y+(rand()%(rooms[idx].height-2))+1;
            if(floor[position_y][position_x]!='*'){
                floor[position_y][position_x]='D';
            }
}
}
}


void watch_room(int which_floor,int room_index,char **floor_one,room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],int **visit_map){
    // pass pass_door[MAX_ROOMS]={0};
    // for(int i=0;i<MAX_ROOMS;i++){
    //     if(which_floor==0){
    //         pass_door[i]=pass_door_one[i];
    //     }
    //     if(which_floor==1){
    //         pass_door[i]=pass_door_two[i];
    //     }
    //     if(which_floor==2){
    //         pass_door[i]=pass_door_three[i];
    //     }
    //     if(which_floor==3){
    //         pass_door[i]=pass_door_four[i];
    //     }
    // }
    int start_y=(rooms[room_index].y > 3) ? rooms[room_index].y : 3;
    int end_y=(rooms[room_index].y+rooms[room_index].height < LINES) ? rooms[room_index].y+rooms[room_index].height : LINES-1;
    int start_x=(rooms[room_index].x > 0) ? rooms[room_index].x : 0 ;
    int end_x=(rooms[room_index].x+rooms[room_index].width < COLS) ?  rooms[room_index].x+rooms[room_index].width : COLS-1;
    for(int j=start_y; j<end_y;j++){
        for(int i=start_x;i<end_x;i++){
            if(j>=0 && j<LINES && i>=0 && i<COLS){
                if(which_floor==3 && room_index==1){
                   attron(COLOR_PAIR(2));
                   mvprintw(j,i,"%c",floor_one[j][i]);
                   attroff(COLOR_PAIR(2));
                } 
                else if(enchanted[which_floor][0]==room_index || strcmp(rooms[room_index].theme,"enchanted")==0){
                    attron(COLOR_PAIR(3));
                    mvprintw(j,i,"%c",floor_one[j][i]);
                    attroff(COLOR_PAIR(3));
                } 
                else mvprintw(j,i,"%c",floor_one[j][i]);
            }
        }
    }
}

void room_visibility(int which_floor,int room_index,char **floor_one,room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],int **visit_map){
    
    int start_y=(rooms[room_index].y-1 > 3) ? rooms[room_index].y-1 : 3;
    int end_y=(rooms[room_index].y+rooms[room_index].height+2 < LINES) ? rooms[room_index].y+rooms[room_index].height+1 : LINES-1;
    int start_x=(rooms[room_index].x-1 > 0) ? rooms[room_index].x-1 : 0 ;
    int end_x=(rooms[room_index].x+rooms[room_index].width  + 1< COLS) ?  rooms[room_index].x+rooms[room_index].width  + 1 : COLS-1;
    
    

    int room_colour=0;
    for(int j=start_y; j<end_y;j++){
        for(int i=start_x;i<end_x;i++){
            if(j>=0 && j<LINES && i>=0 && i<COLS){
                if(floor_one[j][i]=='#'){
                    for(int z=0;z<40;z++){
                        if((doors[room_index][z].x < COLS-1 && doors[room_index][z].x+1==i && doors[room_index][z].y==j) ||
                           (doors[room_index][z].x > 0 && doors[room_index][z].x-1==i && doors[room_index][z].y==j)||
                           (doors[room_index][z].y > 0 && doors[room_index][z].x==i && doors[room_index][z].y-1==j)||
                           (doors[room_index][z].y < LINES-1 && doors[room_index][z].x==i && doors[room_index][z].y+1==j)){
                                mvprintw(j,i,"%c",floor_one[j][i]);
                                refresh();
                                visit_map[j][i]=1;
                           }
                    }
                }
                else {
                if(which_floor==3 && room_index==1){
                   room_colour=2;
                   int cor=find_room(j,i,rooms);
                   if(cor==-1){
                    mvprintw(j,i,"%c",floor_one[j][i]);
                   } 
                   visit_map[j][i]=1;
                }
                else if(enchanted[which_floor][0]==room_index){
                   room_colour=3;
                   int cor=find_room(j,i,rooms);
                   if(cor==-1){
                    mvprintw(j,i,"%c",floor_one[j][i]);
                   } 
                   visit_map[j][i]=1;
                }   
                else{ 
                mvprintw(j,i,"%c",floor_one[j][i]);
                visit_map[j][i]=1;
                }
                }
            }
        }
    }
    if(room_colour==2){
        room_color(floor_one,rooms,room_index,1);
    } 
    else if(room_colour==3){
        room_color(floor_one,rooms,room_index,1);
    }
}

void room_color(char **floor,room rooms[MAX_ROOMS],int room_index,int enchanted){
    if(strcmp(rooms[room_index].theme,"treasure")==0){
       attron(COLOR_PAIR(2));
       for(int y=rooms[room_index].y;y<rooms[room_index].y+rooms[room_index].height;y++){
        for(int x=rooms[room_index].x;x<rooms[room_index].x+rooms[room_index].width;x++){
           mvprintw(y,x,"%c",floor[y][x]);
        }
       }
       attroff(COLOR_PAIR(2));
    }
    else if(strcmp(rooms[room_index].theme,"enchanted")==0){
        if(enchanted) enchanted_stay++;
        attron(COLOR_PAIR(3));
        for(int y=rooms[room_index].y;y<rooms[room_index].y+rooms[room_index].height;y++){
        for(int x=rooms[room_index].x;x<rooms[room_index].x+rooms[room_index].width;x++){
           mvprintw(y,x,"%c",floor[y][x]);
        }
       }
       attroff(COLOR_PAIR(3));
    }
}


void corridor_visibility(int position_y,int position_x,int which_floor,char **floor,int **visit_map){
     for(int y=position_y-1;y<=position_y+1;y++){
        for(int x=position_x-1;x<=position_x+1;x++){
            if(floor[y][x]=='#'){
               mvprintw(y,x,"%c",floor[y][x]);
               visit_map[y][x]=1;
            } 
        }
     }
     
}

int find_room(int position_y,int position_x,room rooms[MAX_ROOMS]){
    for(int i=0;i<MAX_ROOMS;i++){
        if(position_x>=rooms[i].x && position_x < rooms[i].x + rooms[i].width &&
           position_y>=rooms[i].y && position_y < rooms[i].y + rooms[i].height){
           return i;
           }
    }
    return -1;
}

void move_character(int *position_x,int *position_y,int key,char **floor_one,int which_floor,room rooms[MAX_ROOMS]){
    int which_room;
     door hidden[MAX_ROOMS][40];
     if(which_floor==0){
        for(int i=0;i<MAX_ROOMS;i++){
            for(int j=0;j<40;j++){
                hidden[i][j]=hidden_door_one[i][j];
            }
        }
     }
     else if(which_floor==1){
        for(int i=0;i<MAX_ROOMS;i++){
            for(int j=0;j<40;j++){
                hidden[i][j]=hidden_door_two[i][j];
            }
        }
     } 
     else if(which_floor==2){
        for(int i=0;i<MAX_ROOMS;i++){
            for(int j=0;j<40;j++){
                hidden[i][j]=hidden_door_three[i][j];
            }
        }

     } 
     else{
        for(int i=0;i<MAX_ROOMS;i++){
            for(int j=0;j<40;j++){
                hidden[i][j]=hidden_door_four[i][j];
            }
        }
     }
     switch (key)
     {   
        case '1': //down left
        case KEY_END:
            which_room=find_room(*position_y+1,*position_x-1,rooms);
            if(*position_y<LINES-1 && *position_x > 0 &&
                floor_one[*position_y+1][*position_x-1]!='|' &&
                floor_one[*position_y+1][*position_x-1]!='_' &&
                floor_one[*position_y+1][*position_x-1]!='O'&&
                floor_one[*position_y+1][*position_x-1]!='=' &&
                floor_one[*position_y+1][*position_x-1]!=' '){
                    if(!(deamon[which_floor].current_x==*position_x-1 && deamon[which_floor].current_y==*position_y+1) &&
                       !(fire_breathing[which_floor].current_x==*position_x-1 && fire_breathing[which_floor].current_y==*position_y+1) &&
                       !(giant[which_floor].current_x==*position_x-1 && giant[which_floor].current_y==*position_y+1) &&
                       !(snake[which_floor].current_x==*position_x-1 && snake[which_floor].current_y==*position_y+1) &&
                       !(undeed[which_floor].current_x==*position_x-1 && undeed[which_floor].current_y==*position_y+1)){
                        (*position_y)++;
                        (*position_x)--;
                    }
                }
                for(int i=0;i<MAX_ROOMS;i++){
                    for(int j=0;j<40;j++){
                        if(hidden[i][j].x && hidden[i][j].y && *position_x-1==hidden[i][j].x && *position_y+1==hidden[i][j].y){
                           (*position_x)--;
                           (*position_y)++;
                        } 
                    }
                }
            break; 
        case '2':
        case KEY_DOWN: //down
            which_room=find_room(*position_y+1,*position_x,rooms);
            if(*position_y < LINES - 1 &&
                floor_one[*position_y+1][*position_x]!='|' &&
                floor_one[*position_y+1][*position_x]!='_' &&
                floor_one[*position_y+1][*position_x]!='O'&&
                floor_one[*position_y+1][*position_x]!='=' &&
                floor_one[*position_y+1][*position_x]!=' '){
                    if(!(deamon[which_floor].current_x==*position_x && deamon[which_floor].current_y==*position_y+1) &&
                       !(fire_breathing[which_floor].current_x==*position_x && fire_breathing[which_floor].current_y==*position_y+1) &&
                       !(giant[which_floor].current_x==*position_x && giant[which_floor].current_y==*position_y+1) &&
                       !(snake[which_floor].current_x==*position_x && snake[which_floor].current_y==*position_y+1) &&
                       !(undeed[which_floor].current_x==*position_x && undeed[which_floor].current_y==*position_y+1)){
                        (*position_y)++;
                    }
                } 
                for(int i=0;i<MAX_ROOMS;i++){
                    for(int j=0;j<40;j++){
                        if(hidden[i][j].x && hidden[i][j].y && *position_x==hidden[i][j].x && *position_y+1==hidden[i][j].y){
                           (*position_y)++;
                        } 
                    }
                }
            break;
        case '3':  //down right
        case KEY_NPAGE:
            which_room=find_room(*position_y+1,*position_x+1,rooms);
            if(*position_x<COLS-1 && *position_y<LINES-1 &&
                floor_one[*position_y+1][*position_x+1]!='|' &&
                floor_one[*position_y+1][*position_x+1]!='_' &&
                floor_one[*position_y+1][*position_x+1]!='O'&&
                floor_one[*position_y+1][*position_x+1]!='=' &&
                floor_one[*position_y+1][*position_x+1]!=' '){
                    if(!(deamon[which_floor].current_x==*position_x +1&& deamon[which_floor].current_y==*position_y+1) &&
                       !(fire_breathing[which_floor].current_x==*position_x+1 && fire_breathing[which_floor].current_y==*position_y+1) &&
                       !(giant[which_floor].current_x==*position_x+1 && giant[which_floor].current_y==*position_y+1) &&
                       !(snake[which_floor].current_x==*position_x+1 && snake[which_floor].current_y==*position_y+1) &&
                       !(undeed[which_floor].current_x==*position_x+1 && undeed[which_floor].current_y==*position_y+1)){
                        (*position_y)++;
                        (*position_x)++;
                    }
                }
                for(int i=0;i<MAX_ROOMS;i++){
                    for(int j=0;j<40;j++){
                        if(hidden[i][j].x && hidden[i][j].y && *position_x+1==hidden[i][j].x && *position_y+1==hidden[i][j].y){
                           (*position_x)++;
                           (*position_y)++;
                        } 
                    }
                }
            break;
        case '4':  //left
        case KEY_LEFT:
            which_room=find_room(*position_y,*position_x-1,rooms);
            if(*position_x > 0 &&
                floor_one[*position_y][*position_x-1]!='|' &&
                floor_one[*position_y][*position_x-1]!='_' &&
                floor_one[*position_y][*position_x-1]!='O'&&
                floor_one[*position_y][*position_x-1]!='=' &&
                floor_one[*position_y][*position_x-1]!=' '){
                    if(!(deamon[which_floor].current_x==*position_x-1 && deamon[which_floor].current_y==*position_y) &&
                       !(fire_breathing[which_floor].current_x==*position_x-1 && fire_breathing[which_floor].current_y==*position_y) &&
                       !(giant[which_floor].current_x==*position_x-1 && giant[which_floor].current_y==*position_y) &&
                       !(snake[which_floor].current_x==*position_x-1 && snake[which_floor].current_y==*position_y) &&
                       !(undeed[which_floor].current_x==*position_x-1 && undeed[which_floor].current_y==*position_y)){
                        (*position_x)--;
                    }
                }
                for(int i=0;i<MAX_ROOMS;i++){
                    for(int j=0;j<40;j++){
                        if(hidden[i][j].x && hidden[i][j].y && *position_x-1==hidden[i][j].x && *position_y==hidden[i][j].y){
                           (*position_x)--;
                        } 
                    }
                }
            break;
        case '6':   //right
        case KEY_RIGHT:
            which_room=find_room(*position_y,*position_x+1,rooms);
            if(*position_x<COLS-1 &&
                floor_one[*position_y][*position_x+1]!='|' &&
                floor_one[*position_y][*position_x+1]!='_' &&
                floor_one[*position_y][*position_x+1]!='O'&&
                floor_one[*position_y][*position_x+1]!='=' &&
                floor_one[*position_y][*position_x+1]!=' '){
                    if(!(deamon[which_floor].current_x==*position_x+1 && deamon[which_floor].current_y==*position_y) &&
                       !(fire_breathing[which_floor].current_x==*position_x+1 && fire_breathing[which_floor].current_y==*position_y) &&
                       !(giant[which_floor].current_x==*position_x+1 && giant[which_floor].current_y==*position_y) &&
                       !(snake[which_floor].current_x==*position_x+1 && snake[which_floor].current_y==*position_y) &&
                       !(undeed[which_floor].current_x==*position_x+1 && undeed[which_floor].current_y==*position_y)){
                        (*position_x)++;
                    }
                }
                for(int i=0;i<MAX_ROOMS;i++){
                    for(int j=0;j<40;j++){
                        if(hidden[i][j].x && hidden[i][j].y && *position_x+1==hidden[i][j].x && *position_y==hidden[i][j].y){
                           (*position_x)++;
                        } 
                    }
                }
            break;
        case '7':   //up left
        case KEY_HOME:
            which_room=find_room(*position_y-1,*position_x-1,rooms);
            if(*position_x>0 && *position_y>0 &&
                floor_one[*position_y-1][*position_x-1]!='|' &&
                floor_one[*position_y-1][*position_x-1]!='_' &&
                floor_one[*position_y-1][*position_x-1]!='O'&&
                floor_one[*position_y-1][*position_x-1]!='=' &&
                floor_one[*position_y-1][*position_x-1]!=' '){
                    if(!(deamon[which_floor].current_x==*position_x-1 && deamon[which_floor].current_y==*position_y-1) &&
                       !(fire_breathing[which_floor].current_x==*position_x-1 && fire_breathing[which_floor].current_y==*position_y-1) &&
                       !(giant[which_floor].current_x==*position_x-1 && giant[which_floor].current_y==*position_y-1) &&
                       !(snake[which_floor].current_x==*position_x-1 && snake[which_floor].current_y==*position_y-1) &&
                       !(undeed[which_floor].current_x==*position_x-1 && undeed[which_floor].current_y==*position_y-1)){
                        (*position_x)--;
                        (*position_y)--;
                    }
                }
                for(int i=0;i<MAX_ROOMS;i++){
                    for(int j=0;j<40;j++){
                        if(hidden[i][j].x && hidden[i][j].y && *position_x-1==hidden[i][j].x && *position_y-1==hidden[i][j].y){
                           (*position_x)--;
                           (*position_y)--;
                        } 
                    }
                }
            break;                                        
        case '8':   //up
        case KEY_UP:
            which_room=find_room(*position_y-1,*position_x,rooms);
            if(*position_y>0 && floor_one[*position_y-1][*position_x]!='|' &&
                floor_one[*position_y-1][*position_x]!='_' &&
                floor_one[*position_y-1][*position_x]!='O' &&
                floor_one[*position_y-1][*position_x]!='=' &&
                floor_one[*position_y-1][*position_x]!=' '){
                    if(!(deamon[which_floor].current_x==*position_x && deamon[which_floor].current_y==*position_y-1) &&
                       !(fire_breathing[which_floor].current_x==*position_x && fire_breathing[which_floor].current_y==*position_y-1) &&
                       !(giant[which_floor].current_x==*position_x && giant[which_floor].current_y==*position_y-1) &&
                       !(snake[which_floor].current_x==*position_x && snake[which_floor].current_y==*position_y-1) &&
                       !(undeed[which_floor].current_x==*position_x && undeed[which_floor].current_y==*position_y-1)){
                        (*position_y)--;
                    }
                } 
                for(int i=0;i<MAX_ROOMS;i++){
                    for(int j=0;j<40;j++){
                        if(hidden[i][j].x && hidden[i][j].y && *position_x==hidden[i][j].x && *position_y-1==hidden[i][j].y){
                           (*position_y)--;
                        } 
                    }
                }
            break;
        case '9':  //up right
        case KEY_PPAGE:
            which_room=find_room(*position_y-1,*position_x+1,rooms);
            if(*position_x<COLS-1 && *position_y>0 &&
                floor_one[*position_y-1][*position_x+1]!='|' &&
                floor_one[*position_y-1][*position_x+1]!='_' &&
                floor_one[*position_y-1][*position_x+1]!='O'&&
                floor_one[*position_y-1][*position_x+1]!='=' &&
                floor_one[*position_y-1][*position_x+1]!=' '){
                    if(!(deamon[which_floor].current_x==*position_x+1 && deamon[which_floor].current_y==*position_y-1) &&
                       !(fire_breathing[which_floor].current_x==*position_x+1 && fire_breathing[which_floor].current_y==*position_y-1) &&
                       !(giant[which_floor].current_x==*position_x+1 && giant[which_floor].current_y==*position_y-1) &&
                       !(snake[which_floor].current_x==*position_x+1 && snake[which_floor].current_y==*position_y-1) &&
                       !(undeed[which_floor].current_x==*position_x+1 && undeed[which_floor].current_y==*position_y-1)){
                        (*position_x)++;
                        (*position_y)--;
                    }
                }
                for(int i=0;i<MAX_ROOMS;i++){
                    for(int j=0;j<40;j++){
                        if(hidden[i][j].x && hidden[i][j].y && *position_x+1==hidden[i][j].x && *position_y-1==hidden[i][j].y){
                           (*position_x)++;
                           (*position_y)--;
                        } 
                    }
                }
            break;
     }
}


void initialize(room rooms[MAX_ROOMS],door doors[MAX_ROOMS][40],door hidden[MAX_ROOMS][40],pillow pillows[MAX_ROOMS][3],window_room windows[MAX_ROOMS][10],trap traps[MAX_ROOMS][10],int visited[MAX_ROOMS]){
    for(int r=0;r<MAX_ROOMS;r++){
        rooms[r].x=0;
        rooms[r].y=0;
        rooms[r].height=0;
        rooms[r].width=0;
        strcpy(rooms[r].theme,"");
    }
    for(int d=0;d<MAX_ROOMS;d++){
        for(int i=0;i<40;i++){
            doors[d][i].x=0;
            doors[d][i].y=0;
        }
    }
    for(int p=0;p<MAX_ROOMS;p++){
        for(int i=0;i<5;i++){
            pillows[p][i].x=0;
            pillows[p][i].y=0;
        }
    }
    for(int w=0;w<MAX_ROOMS;w++){
        for(int i=0;i<10;i++){
            windows[w][i].x=0;
            windows[w][i].y=0;
        }
    }
    for(int t=0;t<MAX_ROOMS;t++){
        for(int i=0;i<10;i++){
            traps[t][i].x=0;
            traps[t][i].y=0;
        }
    }
    for(int h=0;h<MAX_ROOMS;h++){
        for(int i=0;i<40;i++){
            hidden[h][i].x=0;
            hidden[h][i].y=0;
        }
    }
    for(int v=0;v<MAX_ROOMS;v++){
        visited[v]=0;
    }

}


void initialize_floor(char **floor_one,int **visit){
     for(int y=0;y<LINES;y++){
        for(int x=0;x<COLS;x++){
            floor_one[y][x]=' ';
            visit[y][x]=0;
        }
     }
}

void save_floor(char **floor_one,FILE *file_one){
     //FILE *file=fopen("floor_one.txt" , "w");
     for(int y=0;y<LINES;y++){
        for(int x=0;x<COLS;x++){
            fputc(floor_one[y][x] , file_one);
        }
        fputc('\n' , file_one);
     }
     fclose(file_one);
}


void message_box(int message,int which_floor){
     WINDOW *message_win;
     message_win=newwin(3,COLS,0,0);
     switch(message){
           case 1:
           mvwprintw(message_win,1,1,"You have started the game");
           box(message_win,0,0);
           wrefresh(message_win);
           //getch();
           //delwin(message_win);
           //endwin();
           break;
           case 2:
           mvwprintw(message_win,1,1,"Use numpad to move.Press q to quit.");
           box(message_win,0,0);
           wrefresh(message_win);
           //getch();
           //delwin(message_win);
           //endwin();
           break;
           case 3:
           mvwprintw(message_win,1,1,"You have stepped on a trap!");
           box(message_win,0,0);
           wrefresh(message_win);
           //getch();
           //delwin(message_win);
           break;
           case 4:
           mvwprintw(message_win,1,1,"Press u to go upstair and d to go downstairs.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 5:
           mvwprintw(message_win,1,1,"You can't go up!");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 6:
           mvwprintw(message_win,1,1,"You can't go down!");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 7:
           mvwprintw(message_win,1,1,"Print c to generate password.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 8:
           mvwprintw(message_win,1,1,"Moving to the second floor.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 9:
           mvwprintw(message_win,1,1,"Moving to the third floor.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 10:
           mvwprintw(message_win,1,1,"Moving to the fourth floor.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;(message_win);
           break;
           case 11:
           mvwprintw(message_win,1,1,"Moving to the first floor.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 12:
           mvwprintw(message_win,1,1,"There is a trap on your left.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                     ");
           wrefresh(message_win);
           break;
           case 13:
           mvwprintw(message_win,1,1,"There is a trap on your right.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                     ");
           wrefresh(message_win);
           break;
           case 14:
           mvwprintw(message_win,1,1,"There is a trap right above you.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                      ");
           wrefresh(message_win); 
           break;
           case 15:
           mvwprintw(message_win,1,1,"There is a trap right beneaf you.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 16:
           mvwprintw(message_win,1,1,"There is a hidden door on your left.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 17:
           mvwprintw(message_win,1,1,"There is a hidden door on your right.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                       ");
           wrefresh(message_win);
           break;
           case 18:
           mvwprintw(message_win,1,1,"There is a hidden door right above you.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 19:
           mvwprintw(message_win,1,1,"There is a hidden door right beneaf you.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 20:
           mvwprintw(message_win,1,1,"You scored 1 Gold.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 21:
           mvwprintw(message_win,1,1,"You scored 3 Golds.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 22:
           mvwprintw(message_win,1,1,"You are in game's page.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 23:
           wbkgd(message_win,COLOR_PAIR(1));
           box(message_win,0,0);
           wattron(message_win,COLOR_PAIR(1));
           mvwprintw(message_win,1,1,"Press 'E' to eat or you'll die.");
           wattroff(message_win,COLOR_PAIR(1));
           wrefresh(message_win);
           break;
           case 24:
           mvwprintw(message_win,1,1,"You can't collect more food.Press 'E' to eat.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 25:
           mvwprintw(message_win,1,1,"You're getting dangerously hungry.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 26:
           mvwprintw(message_win,1,1,"You already have a sword.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 27:
           mvwprintw(message_win,1,1,"Staying in the wnchanted room is hurting you.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 28:
           mvwprintw(message_win,1,1,"The Deamon is damaging you.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 29:
           mvwprintw(message_win,1,1,"The Fire-Breathing-Monster is damaging you.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 30:
           mvwprintw(message_win,1,1,"The Giant is damaging you.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 31:
           mvwprintw(message_win,1,1,"The Snake is damaging you.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 32:
           mvwprintw(message_win,1,1,"The Undeed is damaging you.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 33:
           mvwprintw(message_win,1,1,"Press 'i' to select a weapon.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 34:
           mvwprintw(message_win,1,1,"You damaged the Deamon.It has %d hearts now.",deamon[which_floor].heart);
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 35:
           mvwprintw(message_win,1,1,"You damaged the Fire-Breathing-Monster.It has %d hearts now.",fire_breathing[which_floor].heart);
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 36:
           mvwprintw(message_win,1,1,"You damaged the Giant.It has %d hearts now.",giant[which_floor].heart);
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 37:
           mvwprintw(message_win,1,1,"You damaged the Snake.It has %d hearts now.",snake[which_floor].heart);
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 38:
           mvwprintw(message_win,1,1,"You damaged the Undeed.It has %d hearts now.",undeed[which_floor].heart);
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 39:
           mvwprintw(message_win,1,1,"The weapon didn't hit anything.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 40:
           mvwprintw(message_win,1,1,"We are out of weapon.Press 'i' to choose another weapon.");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
           case 41:
           mvwprintw(message_win,1,1,"You have entered a new room.");
           box(message_win,0,0);
           wrefresh(message_win);
           break;
           case 42:
           mvwprintw(message_win,1,1,"You have eaten a poisonous food!");
           box(message_win,0,0);
           wrefresh(message_win);
           sleep(1);
           mvwprintw(message_win,1,1,"                                        ");
           wrefresh(message_win);
           break;
     }
}

void status_box(){
   WINDOW *status_win;
   status_win=newwin(3,COLS,LINES-3,0);
   box(status_win,0,0);
   mvwprintw(status_win,1 ,1,"Hearts: %d" , Heart);
   mvwprintw(status_win,1 ,20,"Health: %d" , Health);
   mvwprintw(status_win,1 , 40,"Gold: %d" , Gold);
   mvwprintw(status_win,1,60,"Collected Food: %d/%d" , food_collected,MAX_FOOD);
   wrefresh(status_win); 
}


void create_map(char **floor_one,int which_floor,FILE *file_one,room rooms[MAX_ROOMS],window_room windows[MAX_ROOMS][10],door doors[MAX_ROOMS][40],pillow pillows[MAX_ROOMS][3],trap traps[MAX_ROOMS][10],int **visit){
    //int max_y=15, max_x=15;
    int room_count = 0;

    initscr();
    setlocale(LC_ALL,"");
    noecho();
    start_color();
    if(has_colors()){
        init_color(COLOR_BLACK,49 * 1000/255,2*1000/255,54*1000/255);
        init_pair(1,COLOR_RED,COLOR_BLACK);
        init_pair(2,COLOR_YELLOW,COLOR_BLACK);
        init_pair(3,COLOR_GREEN,COLOR_BLACK);
        init_pair(4,COLOR_BLUE,COLOR_BLACK);
        init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
    }
    //  init_pair(1,COLOR_RED,COLOR_BLACK);
    curs_set(FALSE);

    //srand(time(NULL));

    
    int maxcopy_x=(COLS*13)/15;
    int maxcopy_y=(LINES*13)/15;
    initialize_floor(floor_one,visit);
    if(which_floor==0){
    while (room_count < MAX_ROOMS) {
        struct room new_room;
                new_room.width = rand() % (MAX_WIDTH - MIN_WIDTH + 1) + MIN_WIDTH;
                new_room.height = rand() % (MAX_HEIGHT - MIN_HEIGHT + 1) + MIN_HEIGHT;
                new_room.x = rand() % (maxcopy_x - new_room.width);
                new_room.y = rand() % (maxcopy_y - new_room.height-4)+4;
    int overlap=1;
    for(int i=0;i<room_count;i++){
        struct room existing=rooms[i];
        if(new_room.x - 10<= existing.x + existing.width &&
           new_room.x + new_room.width + 10>=existing.x &&
           new_room.y - 10 <=existing.y + existing.height &&
           new_room.y + new_room.height + 10 > existing.y){
           overlap=0;
           break;
           }
    }
    if(overlap){
        rooms[room_count]=new_room;
        rooms[room_count].room_index=room_count;
        strcpy(rooms[room_count].theme,"");
        room_count++;
    }
    }
    }
    else{
        rooms[0].x=stair_room.x;
        rooms[0].y=stair_room.y;
        rooms[0].width=stair_room.width;
        rooms[0].height=stair_room.height;
        rooms[0].room_index=0;
        room_count++;
        while (room_count < MAX_ROOMS) {
        struct room new_room;
                new_room.width = rand() % (MAX_WIDTH - MIN_WIDTH + 1) + MIN_WIDTH;
                new_room.height = rand() % (MAX_HEIGHT - MIN_HEIGHT + 1) + MIN_HEIGHT;
                new_room.x = rand() % (maxcopy_x - new_room.width);
                new_room.y = rand() % (maxcopy_y - new_room.height-4)+4;
    int overlap=1;
    for(int i=0;i<room_count;i++){
        struct room existing=rooms[i];
        if(new_room.x -10<= existing.x + existing.width &&
           new_room.x + new_room.width +10>=existing.x &&
           new_room.y-10<=existing.y + existing.height &&
           new_room.y + new_room.height +10 > existing.y){
           overlap=0;
           break;
           }
    }
    if(overlap){
        rooms[room_count]=new_room;
        rooms[room_count].room_index=room_count;      ///
        strcpy(rooms[room_count].theme,"");         /// 
        room_count++;
    }
    }
    }
    create_pillow(rooms,pillows);
    create_window(rooms,windows);
    create_traps(which_floor,rooms,traps);
    if(which_floor==0) create_stairs();
    for (int i = 0; i < room_count; i++) {
        struct room r = rooms[i];
        for(int j=0;j<r.width;j++){
        //mvprintw(r.y , r.x + j , "_");
        floor_one[r.y][r.x+j]='_';
        }
        for (int y = 1; y < r.height - 1; y++) {
            //mvprintw(r.y + y, r.x, "|");
            floor_one[r.y+y][r.x]='|';
            for (int x = 1; x < r.width-1; x++) {
                //mvprintw(r.y + y, r.x + x, ".");
                floor_one[r.y+y][r.x+x]='.';
            }
            //mvprintw(r.y + y, r.x + r.width - 1, "|");
            floor_one[r.y+y][r.x+r.width-1]='|';
        }
        for(int j=0;j<r.width;j++){
        //mvprintw(r.y+r.height-1 , r.x + j , "_");
        floor_one[r.y+r.height-1][r.x + j]='_';
        }
        for(int j=0;j<10;j++){
            if(windows[i][j].x && windows[i][j].y){
                //mvprintw(windows[i][j].y,windows[i][j].x,"=");
                floor_one[windows[i][j].y][windows[i][j].x]='=';
            }
        }
        for(int j=0;j<3;j++){
            if(pillows[i][j].x && pillows[i][j].y){
                //mvprintw(pillows[i][j].y,pillows[i][j].x,"O");
                floor_one[pillows[i][j].y][pillows[i][j].x]='O';
            }
        }
        for(int j=0;j<10;j++){
            if(traps[i][j].x && traps[i][j].y){
                floor_one[traps[i][j].y][traps[i][j].x]='.';
            }
        }
        // if(!(which_floor==3 && i==1)) create_door(floor_one,i,doors,rooms);
        create_door(floor_one,i,doors,rooms);
        for(int j=0;j<3;j++){
            if(doors[i][j].x && doors[i][j].y){
                //mvprintw(doors[i][j].y,doors[i][j].x,"+");
                floor_one[doors[i][j].y][doors[i][j].x]='+';
            }
        }
    }
    //if(!which_floor){
    //mvprintw(stair.y,stair.x,"*");
    create_gold(floor_one,rooms);
    create_food(floor_one,rooms);
    create_weapon(floor_one,rooms);
    create_spell(which_floor,floor_one,rooms);
    if(which_floor==3){
        int pos_x,pos_y;
        do{
        pos_x=rooms[2].x+(rand()%(rooms[2].width-2))+1;
        pos_y=rooms[2].y+(rand()%(rooms[2].height-2))+1;
        }
        while(pos_x==stair.x && pos_y==stair.y);
        floor_one[pos_y][pos_x]='I';
    }
    for(int i=0;i<MAX_ROOMS;i++){
        for(int j=0;j<10;j++){
            if(floor_one[traps[i][j].y][traps[i][j].x]=='$' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='~' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='f' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='p' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='w' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='0' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='s' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='n' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='a' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='d' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='H' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='D' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='S' ||
               floor_one[traps[i][j].y][traps[i][j].x]=='I'){
                traps[i][j].x=0;
                traps[i][j].y=0;
            }
        }
    }
    floor_one[stair.y][stair.x]='*';
    //refresh();
    //getch();
    //endwin();
    create_corridor(which_floor,floor_one,doors);
    for(int y=0;y<LINES;y++){
        for(int x=0;x<COLS;x++){
            if(floor_one[y][x]=='+' && !((y<LINES-1 && floor_one[y+1][x]=='#') || (y>0 && floor_one[y-1][x]=='#') || (x<COLS-1 && floor_one[y][x+1]=='#') || (x>0 && floor_one[y][x-1]=='#') || (y==LINES-1 && ((x>0 &&floor_one[y][x-1]=='#') || (x<COLS-1 && floor_one[y][x+1]=='#'))) || (x==COLS-1 && ((y>0 &&floor_one[y-1][x]=='#') || (y<LINES-1 && floor_one[y+1][x]=='#'))) || (x==0 && ((y>0 && floor_one[y-1][x]=='#') || (y<LINES-1 && floor_one[y+1][x]=='#'))))){
                if((x>0 && (floor_one[y][x-1]=='.' || floor_one[y][x-1]=='O' || floor_one[y][x-1]=='*' || floor_one[y][x-1]=='~' || floor_one[y][x-1]=='$' || floor_one[y][x-1]=='f' || floor_one[y][x-1]=='p' || floor_one[y][x-1]=='w' || floor_one[y][x-1]=='0' || floor_one[y][x-1]=='s' || floor_one[y][x-1]=='n' || floor_one[y][x-1]=='a' || floor_one[y][x-1]=='d' || floor_one[y][x-1]=='H' || floor_one[y][x-1]=='S' || floor_one[y][x-1]=='D' || floor_one[y][x-1]=='I')) ||
                (x<COLS-1 && (floor_one[y][x+1]=='.' || floor_one[y][x+1]=='O' || floor_one[y][x+1]=='*'|| floor_one[y][x+1]=='$' || floor_one[y][x+1]=='~' || floor_one[y][x+1]=='f' || floor_one[y][x+1]=='p' || floor_one[y][x+1]=='w' || floor_one[y][x+1]=='0' || floor_one[y][x+1]=='s' || floor_one[y][x+1]=='n' || floor_one[y][x+1]=='a' || floor_one[y][x+1]=='d' || floor_one[y][x+1]=='H' || floor_one[y][x+1]=='S' || floor_one[y][x+1]=='D' || floor_one[y][x+1]=='I'))){
                    floor_one[y][x]='|';
                }
                else{
                    floor_one[y][x]='_';
                }
            }
        }
    }
    reset_doors(floor_one,doors,rooms);
    hidden_doors(which_floor,floor_one,doors,rooms,1);
    create_monster(which_floor,floor_one,rooms);
    save_floor(floor_one,file_one);
    //if(!which_floor){
    // for(int i=stair_room.y;i<stair_room.y+stair_room.height;i++){
    //     for(int j=stair_room.x;j<stair_room.x+stair_room.width;j++){
    //         char c=floor_one[i][j];
    //         mvprintw(i,j,"%c",c);
    //     }
    // }
    refresh();
    // game(floor_one);
    //getch();
    //endwin();
}

void create_door(char **floor_one,int i,door doors[MAX_ROOMS][40],room rooms[MAX_ROOMS]){
    //int i=0;
    //if(which_floor) i++;
     //for(int i=0;i<MAX_ROOMS;i++){
        int number_door = 1; // At least one door
        for (int j = 0; j < number_door; j++) {
            int valid = 0;
            while (!valid) {
                int side = rand() % 4; // Random side: 0=top, 1=bottom, 2=left, 3=right 
                if(rooms[i].x==0 && side==2) side=3;
                if(rooms[i].y==LINES-1 && side==1) side=0;
                if(rooms[i].x==COLS-1 && side==3) side=2; 
                int x, y;
                if (side == 0) { // Top
                    x = rooms[i].x + rand() % rooms[i].width;
                    y = rooms[i].y;
                } else if (side == 1) { // Bottom
                    x = rooms[i].x + rand() % rooms[i].width;
                    y = rooms[i].y + rooms[i].height - 1;
                } else if (side == 2) { // Left
                    x = rooms[i].x;
                    y = rooms[i].y + rand() % rooms[i].height;
                } else { // Right
                    x = rooms[i].x + rooms[i].width - 1;
                    y = rooms[i].y + rand() % rooms[i].height;
                }

                if (floor_one[y][x] == '|' || floor_one[y][x] == '_' || floor_one[y][x]=='=' || floor_one[y][x]=='+') {
                    doors[i][j].x = x;
                    doors[i][j].y = y;
                    floor_one[y][x] = '+'; // Mark the door
                    valid = 1;
                }
            }
        }
     //}
}


void create_pillow(room rooms[MAX_ROOMS],pillow pillows[MAX_ROOMS][3]){
     for(int i=0;i<MAX_ROOMS;i++){
        int number_pillow=rand() % 3;
        for(int j=0;j<number_pillow;j++){
            pillows[i][j].x=rooms[i].x+(rand() % (rooms[i].width - 4))+2;
            pillows[i][j].y=rooms[i].y+(rand() % (rooms[i].height - 4))+2;
        }
     }
}

void create_window(room rooms[MAX_ROOMS],window_room windows[MAX_ROOMS][10]){
     for(int i=0;i<MAX_ROOMS;i++){
        int number_window=rand() % 10;
        for(int j=0;j<number_window;j++){
            int x=rand()%(rooms[i].width);
            if(x==0 || x==rooms[i].width-1){
                int y=rand()%(rooms[i].height-1);
                windows[i][j].x=x+rooms[i].x;
                windows[i][j].y=y+rooms[i].y;
            }
            else{
                windows[i][j].x=x+rooms[i].x;
                windows[i][j].y=rooms[i].y;
            }
        }
     }
}

void create_stairs(){
    int room_stairs=rand()%MAX_ROOMS;
    stair.x=(rand()%(rooms_one[room_stairs].width-2)+1)+rooms_one[room_stairs].x;
    stair.y=(rand()%(rooms_one[room_stairs].height-2)+1)+rooms_one[room_stairs].y; 
    stair.room_index=room_stairs; 
    stair_room=rooms_one[room_stairs];
}


void create_corridor(int which_floor,char **floor_one,door doors[MAX_ROOMS][40]) {
    // Collect all doors
    door door_locations[MAX_ROOMS]; // Maximum 3 doors per room
    int door_count = 0;

    for (int i = 0; i < MAX_ROOMS; i++) {
        for (int j = 0; j < 3; j++) {
            if (doors[i][j].x && doors[i][j].y) {
                door_locations[door_count].x = doors[i][j].x;
                door_locations[door_count].y = doors[i][j].y;
                door_count++;
            }
        }
    }
    
    // Connect doors with paths
    for (int i = 0; i < door_count - 1; i++) {
        door start = door_locations[i];
        door end = door_locations[i + 1];

        // Pathfinding to connect doors
        int current_x = start.x;
        int current_y = start.y;

        while (current_x != end.x || current_y != end.y) {
            if (current_x < end.x) current_x++;
            else if (current_x > end.x) current_x--;
            else if (current_y < end.y) current_y++;
            else if (current_y > end.y) current_y--;
            
            // Place path character if not inside a room
            if (floor_one[current_y][current_x] == '.'||
                floor_one[current_y][current_x] == '*'||
                floor_one[current_y][current_x]== 'O' ||
                floor_one[current_y][current_x] == '+'||
                floor_one[current_y][current_x]== '$' ||
                floor_one[current_y][current_x]== '~' ||
                floor_one[current_y][current_x]=='f'  ||
                floor_one[current_y][current_x]=='p'  ||
                floor_one[current_y][current_x]=='w'  ||
                floor_one[current_y][current_x]=='0'  ||
                floor_one[current_y][current_x]=='s'  ||
                floor_one[current_y][current_x]=='n'  ||
                floor_one[current_y][current_x]=='a'  ||
                floor_one[current_y][current_x]=='d'  ||
                floor_one[current_y][current_x]=='H'  ||
                floor_one[current_y][current_x]=='S'  ||
                floor_one[current_y][current_x]=='D'  ||
                floor_one[current_y][current_x]=='I') {
                continue; // Skip room interiors
            }
            else if(floor_one[current_y][current_x] == '|' ||
            floor_one[current_y][current_x] == '_' ||
            floor_one[current_y][current_x]=='='){
                floor_one[current_y][current_x] = '+';

            }
            // Mark the path
            else floor_one[current_y][current_x] = '#';
        }
    }
}

void create_traps(int which_floor,room rooms[MAX_ROOMS],trap traps[MAX_ROOMS][10]){
        for(int i=0;i<MAX_ROOMS;i++){
        if(which_floor==3 && i==1){
           strcpy(rooms[1].theme,"treasure");
           for(int j=0;j<10;j++){
           traps[i][j].x=rooms[i].x+(rand() % (rooms[i].width - 2))+1;
           traps[i][j].y=rooms[i].y+(rand() % (rooms[i].height - 2))+1;
        }
        }  
        else{  
        int number_traps=rand() % 3;
        for(int j=0;j<number_traps && !(traps[i][j].x==stair.x && traps[i][j].y==stair.y);j++){
           traps[i][j].x=rooms[i].x+(rand() % (rooms[i].width - 2))+1;
           traps[i][j].y=rooms[i].y+(rand() % (rooms[i].height - 2))+1;
        }
        }
    } 
}

void create_gold(char **floor,room rooms[MAX_ROOMS]){
    int position_x;
    int position_y;
    for(int i=0;i<MAX_ROOMS;i++){
        int count=rand()%2;
        position_x=rooms[i].x+(rand()%(rooms[i].width-2))+1;
        position_y=rooms[i].y+(rand()%(rooms[i].height-2))+1;
        for(int j=0;j<count && floor[position_y][position_x]!='*';j++){
            floor[position_y][position_x]='$';
        }
    }
    int index_one=rand()%MAX_ROOMS;
    int index_two=rand()%MAX_ROOMS;
    position_x=rooms[index_one].x+(rand()%(rooms[index_one].width-2))+1;
    position_y=rooms[index_one].y+(rand()%(rooms[index_one].height-2))+1;
    if(floor[position_y][position_x]!='*') floor[position_y][position_x]='~';
    position_x=rooms[index_two].x+(rand()%(rooms[index_two].width-2))+1;
    position_y=rooms[index_two].y+(rand()%(rooms[index_two].height-2))+1;
    if(floor[position_y][position_x]!='*') floor[position_y][position_x]='~';
}

void create_food(char **floor,room rooms[MAX_ROOMS]){
    int position_x,position_y;
     for(int i=0;i<MAX_ROOMS;i++){
        int number=rand() % 3;
        for(int j=0;j<number;j++){
            position_x=rooms[i].x+(rand() % (rooms[i].width - 2))+1;
            position_y=rooms[i].y+(rand() % (rooms[i].height - 2))+1;
            floor[position_y][position_x]='f';
        }
        number=rand()%2;
        for(int j=0;j<number;j++){
            position_x=rooms[i].x+(rand() % (rooms[i].width - 2))+1;
            position_y=rooms[i].y+(rand() % (rooms[i].height - 2))+1;
            floor[position_y][position_x]='p';
        }
        number=rand()%2;
        for(int j=0;j<number;j++){
            position_x=rooms[i].x+(rand() % (rooms[i].width - 2))+1;
            position_y=rooms[i].y+(rand() % (rooms[i].height - 2))+1;
            floor[position_y][position_x]='w';
        }
        number=rand()%2;
        for(int j=0;j<number;j++){
            position_x=rooms[i].x+(rand() % (rooms[i].width - 2))+1;
            position_y=rooms[i].y+(rand() % (rooms[i].height - 2))+1;
            floor[position_y][position_x]='0';
        }
    }
}

void create_weapon(char **floor,room rooms[MAX_ROOMS]){
    int position_x,position_y;

    int index_Dagger=rand()%MAX_ROOMS;
    int index_Magic=rand()%MAX_ROOMS;
    int index_Normal=rand()%MAX_ROOMS;
    int index_Sword=rand()%MAX_ROOMS;

    position_x=rooms[index_Dagger].x+(rand()%(rooms[index_Dagger].width-2))+1;
    position_y=rooms[index_Dagger].y+(rand()%(rooms[index_Dagger].height-2))+1;
    if(floor[position_y][position_x]!='*' &&
       !(position_x>0 && floor[position_y][position_x-1]=='*') &&
       !(position_x<COLS-1 && floor[position_y][position_x+1]=='*')){
       floor[position_y][position_x]='d';
       if(position_x>0 && floor[position_y][position_x-1]!='|' && floor[position_y][position_x-1]!='+' && floor[position_y][position_x-1]!='=') floor[position_y][position_x-1]='.';
       if(position_x<COLS-1 && floor[position_y][position_x+1]!='|' && floor[position_y][position_x+1]!='+' && floor[position_y][position_x+1]!='=') floor[position_y][position_x+1]='.';
    }
    position_x=rooms[index_Magic].x+(rand()%(rooms[index_Magic].width-2))+1;
    position_y=rooms[index_Magic].y+(rand()%(rooms[index_Magic].height-2))+1;
    if(floor[position_y][position_x]!='*' &&
       !(position_x>0 && floor[position_y][position_x-1]=='*') &&
       !(position_x<COLS-1 && floor[position_y][position_x+1]=='*')){
        floor[position_y][position_x]='a';
        if(position_x>0 && floor[position_y][position_x-1]!='|' && floor[position_y][position_x-1]!='+' && floor[position_y][position_x-1]!='=') floor[position_y][position_x-1]='.';
        if(position_x<COLS-1 && floor[position_y][position_x+1]!='|' && floor[position_y][position_x+1]!='+' && floor[position_y][position_x+1]!='=') floor[position_y][position_x+1]='.';
    }   
    position_x=rooms[index_Normal].x+(rand()%(rooms[index_Normal].width-2))+1;
    position_y=rooms[index_Normal].y+(rand()%(rooms[index_Normal].height-2))+1;
    if(floor[position_y][position_x]!='*' &&
       !(position_x>0 && floor[position_y][position_x-1]=='*') &&
       !(position_x<COLS-1 && floor[position_y][position_x+1]=='*')){
        floor[position_y][position_x]='n';
        if(position_x>0 && floor[position_y][position_x-1]!='|' && floor[position_y][position_x-1]!='+' && floor[position_y][position_x-1]!='=') floor[position_y][position_x-1]='.';
        if(position_x<COLS-1 && floor[position_y][position_x+1]!='|' && floor[position_y][position_x+1]!='+' && floor[position_y][position_x+1]!='=') floor[position_y][position_x+1]='.';
    }   
    position_x=rooms[index_Sword].x+(rand()%(rooms[index_Sword].width-2))+1;
    position_y=rooms[index_Sword].y+(rand()%(rooms[index_Sword].height-2))+1;
    if(floor[position_y][position_x]!='*' &&
      !(position_x>0 && floor[position_y][position_x-1]=='*') &&
      !(position_x<COLS-1 && floor[position_y][position_x+1]=='*')){
       floor[position_y][position_x]='s';
       if(position_x>0 && floor[position_y][position_x-1]!='|' && floor[position_y][position_x-1]!='+' && floor[position_y][position_x-1]!='=') floor[position_y][position_x-1]='.';
       if(position_x<COLS-1 && floor[position_y][position_x+1]!='|' && floor[position_y][position_x+1]!='+' && floor[position_y][position_x+1]!='=') floor[position_y][position_x+1]='.';
    }    
}

void create_spell(int which_floor,char **floor,room rooms[MAX_ROOMS]){
    int index_health=rand()%MAX_ROOMS,index_speed=rand()%MAX_ROOMS,index_damage=rand()%MAX_ROOMS;
    int position_x,position_y;
    position_x=rooms[index_health].x+(rand()%(rooms[index_health].width-2))+1;
    position_y=rooms[index_health].y+(rand()%(rooms[index_health].height-2))+1;
    if(floor[position_y][position_x]!='*'){
       floor[position_y][position_x]='H';
    }
    position_x=rooms[index_speed].x+(rand()%(rooms[index_speed].width-2))+1;
    position_y=rooms[index_speed].y+(rand()%(rooms[index_speed].height-2))+1;
    if(floor[position_y][position_x]!='*'){
       floor[position_y][position_x]='S';
    }
    position_x=rooms[index_damage].x+(rand()%(rooms[index_damage].width-2))+1;
    position_y=rooms[index_damage].y+(rand()%(rooms[index_damage].height-2))+1;
    if(floor[position_y][position_x]!='*'){
       floor[position_y][position_x]='D';
    }
}

void create_monster(int which_floor,char **floor,room rooms[MAX_ROOMS]){
    int position_x,position_y;
    int index_deamon,index_fire,index_giant,index_snake,index_undeed;
     
    do{
        index_deamon=rand()%MAX_ROOMS;
    }
    while(strcmp(rooms[index_deamon].theme,"enchanted")==0);
    if(which_floor==3) index_deamon=1;
    do{
    position_x=rooms[index_deamon].x+(rand()%(rooms[index_deamon].width-2))+1;
    position_y=rooms[index_deamon].y+(rand()%(rooms[index_deamon].height-2))+1;
    }
    while(floor[position_y][position_x]=='*');
        deamon[which_floor].room_index=index_deamon;
        deamon[which_floor].current_x=position_x;
        deamon[which_floor].current_y=position_y;
        deamon[which_floor].heart=5;
        deamon[which_floor].name='A';
        deamon[which_floor].alive=1;
        //floor[position_y][position_x]='e';
     
    do{
        index_fire=rand()%MAX_ROOMS;
    }
    while(strcmp(rooms[index_fire].theme,"enchanted")==0);
    if(which_floor==3) index_fire=1;
    do{
    position_x=rooms[index_fire].x+(rand()%(rooms[index_fire].width-2))+1;
    position_y=rooms[index_fire].y+(rand()%(rooms[index_fire].height-2))+1;
    }
    while(floor[position_y][position_x]=='*');
        fire_breathing[which_floor].room_index=index_fire;
        fire_breathing[which_floor].current_x=position_x;
        fire_breathing[which_floor].current_y=position_y;
        fire_breathing[which_floor].heart=10;
        fire_breathing[which_floor].name='F';
        fire_breathing[which_floor].alive=1;
        //floor[position_y][position_x]='e';
    
    do{
        index_giant=rand()%MAX_ROOMS;
    }
    while(strcmp(rooms[index_giant].theme,"enchanted")==0);
    if(which_floor==3) index_giant=1;
    do{
    position_x=rooms[index_giant].x+(rand()%(rooms[index_giant].width-2))+1;
    position_y=rooms[index_giant].y+(rand()%(rooms[index_giant].height-2))+1;
    }
    while(floor[position_y][position_x]=='*');
        giant[which_floor].room_index=index_giant;
        giant[which_floor].current_x=position_x;
        giant[which_floor].current_y=position_y;
        giant[which_floor].heart=15;
        giant[which_floor].name='G';
        giant[which_floor].alive=1;
        //floor[position_y][position_x]='e';
    
    do{
        index_snake=rand()%MAX_ROOMS;
    }
    while(strcmp(rooms[index_snake].theme,"enchanted")==0);
    if(which_floor==3) index_snake=1;
    do{
    position_x=rooms[index_snake].x+(rand()%(rooms[index_snake].width-2))+1;
    position_y=rooms[index_snake].y+(rand()%(rooms[index_snake].height-2))+1;
    }
    while(floor[position_y][position_x]=='*');
        snake[which_floor].room_index=index_snake;
        snake[which_floor].current_x=position_x;
        snake[which_floor].current_y=position_y;
        snake[which_floor].heart=20;
        snake[which_floor].name='C';
        snake[which_floor].alive=1;
        //floor[position_y][position_x]='e';
    do{
        index_undeed=rand()%MAX_ROOMS;
    }
    while(strcmp(rooms[index_undeed].theme,"enchanted")==0);
    if(which_floor==3) index_undeed=1;
    do{
    position_x=rooms[index_undeed].x+(rand()%(rooms[index_undeed].width-2))+1;
    position_y=rooms[index_undeed].y+(rand()%(rooms[index_undeed].height-2))+1;
    }
    while(floor[position_y][position_x]=='*');
        undeed[which_floor].room_index=index_undeed;
        undeed[which_floor].current_x=position_x;
        undeed[which_floor].current_y=position_y;
        undeed[which_floor].heart=30;
        undeed[which_floor].name='U';
        undeed[which_floor].alive=1;
        //floor[position_y][position_x]='e';
    
}
