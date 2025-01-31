#include "game.h"
#define MAX_PLAYERS 100


Player current_player;
int login_choice=0;
int quit=1;
int quit_from_setting=1;
int guest_call=1;
int color;
int choice_color=-1;
int choice_level=-1;
int choice_music=-1;
int music=1;

void initialize_game(){
     stair_room.height=0;
     stair_room.room_index=6;
     strcpy(stair_room.theme,"");
     stair_room.width=0;
     stair_room.x=0;
     stair_room.y=0;
     stair.room_index=6;
     stair.x=0;
     stair.y=0;
     for(int i=0;i<4;i++){
        deamon[i].alive=1;
        deamon[i].current_x=0;
        deamon[i].current_y=0;
        deamon[i].heart=5;
        deamon[i].name='A';
        deamon[i].room_index=6;
        fire_breathing[i].alive=1;
        fire_breathing[i].current_x=0;
        fire_breathing[i].current_y=0;
        fire_breathing[i].heart=10;
        fire_breathing[i].name='F';
        fire_breathing[i].room_index=6;
        giant[i].alive=1;
        giant[i].current_x=0;
        giant[i].current_y=0;
        giant[i].heart=15;
        giant[i].name='G';
        giant[i].room_index=6;
        snake[i].alive=1;
        snake[i].current_x=0;
        snake[i].current_y=0;
        snake[i].heart=20;
        snake[i].name='C';
        snake[i].room_index=6;
        undeed[i].alive=1;
        undeed[i].current_x=0;
        undeed[i].current_y=0;
        undeed[i].heart=30;
        undeed[i].name='U';
        undeed[i].room_index=6;
    }
    Heart=100,Health=10,Gold=0,hunger=0,food_collected=0,dagger_count=0,magic_wand_count=0,normal_arrow_count=0,sword_count=0,mace_damage=5,dagger_damage=12,magic_wand_damage=15,normal_arrow_damage=5,sword_damage=10,default_weapon='m',last_hit=' ',spell='n',health_spell=0,speed_spell=0,damage_spell=0,health_count=0,speed_count=0,damage_count=0,enchanted_stay=0,win=0,end=0;
}

void initial_menu(int highlight){
    clear();
    attron(COLOR_PAIR(5));
    mvprintw(0,0,"=== WELCOME! ===");
    attroff(COLOR_PAIR(5));
    char *choices[]={
         "Sign up",
         "Log in",
         "Guest",
         "Quit"
    };
    int n_choices = sizeof(choices) / sizeof(char *);
    int x=0,y=2;
    for(int i=0;i<n_choices;i++){
        if (i == highlight) {
            attron(A_REVERSE);
            mvprintw(y, x, "%s", choices[i]);
            attroff(A_REVERSE);
        } else {
            mvprintw(y, x, "%s", choices[i]);
        }
        y++;
    }
    refresh();
}

void setting_menu(int highlight){
    char *choices[]={
        "Color",
        "Level",
        "Song",
        "Back"
    };
    int n_choices = sizeof(choices) / sizeof(char *);
    int x=0,y=0;
    clear();
    for(int i=0;i<n_choices;i++){
        if(i==highlight){
            attron(A_REVERSE);
            mvprintw(y,x,"%s",choices[i]);
            attroff(A_REVERSE);
        }
        else{
            mvprintw(y,x,"%s",choices[i]);
        }
        y++; 
    }
}

void game_menu(int highlight){
    char *choices[]={
        "New Game",
        "Resume",
        "Scoreboard",
        "Setting",
        "Back"
    };
    int n_choices = sizeof(choices) / sizeof(char *);
    int x=0,y=0;
    clear();
    for(int i=0;i<n_choices;i++){
        if(i==highlight){
            attron(A_REVERSE);
            mvprintw(y,x,"%s",choices[i]);
            attroff(A_REVERSE);
        }
        else{
            mvprintw(y,x,"%s",choices[i]);
        }
        y++; 
    }
}

int compare_score(const void *a,const void *b){
    return((Player *)b)->gold-((Player *)a)->gold;
}

void setting_choice(int choice){
    switch (choice)
    {
    case 0:
    while(1){
    clear();
    mvprintw(0,0,"1.WHITE");
    mvprintw(1,0,"2.MAGENTA");
    if(choice_color!=-1){
    mvprintw(3,0,"ENTER YOUR CHOICE : %d" , choice_color);
    }
    else{
        mvprintw(3,0,"ENTER YOUR CHOICE :");
    }
    refresh();
    int ch=getch();
    if(ch=='1' || ch=='2'){
        choice_color=ch-'0';
    }
    else{
        if(choice_color==1 || choice_color==-1) color=0;
        else if(choice_color==2) color=1;
        break;
    }
    }
    break;
    case 1:
    while(1){
        clear();
        mvprintw(0,0,"1.EASY");
        mvprintw(1,0,"2.HARD");
        mvprintw(2,0,"3.SUPER-HARD");
        if(choice_level!=-1){
            mvprintw(4,0,"ENTER YOUR CHOICE : %d",choice_level);
        }
        else{
            mvprintw(4,0,"ENTER YOUR CHOICE :");
        }
        refresh();
        int ch=getch();
        if(ch=='1' || ch=='2' || ch=='3'){
            choice_level=ch-'0';
        }
        else{
            break;
        }
    }
    break;
    case 2:
    clear();
    while(1){
        clear();
        char *songs[2]={"starboy.mp3","save_your_tears.mp3"};
        attron(COLOR_PAIR(2));
        mvprintw(0,0,"=== MUSIC SETTING ===");
        attroff(COLOR_PAIR(2));
        mvprintw(1,0,"1.STARBOY");
        mvprintw(2,0,"2.SAVE YOUR TEARS");
        mvprintw(3,0,"3.NO MUSIC");
        if(choice_music!=-1){
            mvprintw(5,0,"ENTER YOUR CHOICE : %d",choice_music);
        }
        else{
            mvprintw(5,0,"ENTER YOUR CHOICE :");
        }
        refresh();
        int ch=getch();
        if(ch=='1'){
            choice_music=1;
            system("pkill mpg123");
            char command[100];
            snprintf(command,sizeof(command),"nohup mpg123 -q %s > /dev/null 2>&1 &",songs[0]);
            system(command);
            printf("\e[?251");
            fflush(stdout);
        }
        else if(ch=='2'){
            choice_music=2;
            system("pkill mpg123");
            char command[100];
            snprintf(command,sizeof(command),"nohup mpg123 -q %s > /dev/null 2>&1 &",songs[1]);
            system(command);
            printf("\e[?251");
            fflush(stdout);
        }
        else if(ch=='3'){
                choice_music=3;
                system("pkill mpg123");
                printf("\e[?251");
                fflush(stdout);
        }
        else{
            break;
        }
        printf("\e[?251");
        fflush(stdout);
    }
    break;  
    case 3:
    clear();
    quit_from_setting=0;
    break;  
    }
}


void game_choice(int choice){
     Player players[MAX_PLAYERS];
     int count=0;
     FILE *score_file;
     clear();
     switch(choice){
        case 0:
        clear();
        new_game();
        current_player.num_of_games++;
        current_player.gold+=Gold;
        if(win==1) current_player.score++;
        if(win==1 || win==-1) current_player.experience++;
        score_file=fopen("score.txt","r");
        if(score_file){
            while(fscanf(score_file,"%s %d %d %d %d",players[count].user_name,&players[count].gold,&players[count].score,&players[count].num_of_games,&players[count].experience)==5){
                count++;
            }
            fclose(score_file);
        }
        int player_found=0;
        for(int i=0;i<count;i++){
            if(strcmp(players[i].user_name,current_player.user_name)==0){
                players[i].gold+=Gold;
                players[i].num_of_games++;
                if(win==1) players[i].score++;
                if(win==1 || win==-1) players[i].experience++;
                player_found=1;
                break;
            }
        }
        if(!player_found){
            strcpy(players[count].user_name,current_player.user_name);
            players[count].score=current_player.score;
            players[count].gold=current_player.gold;
            players[count].num_of_games=current_player.num_of_games;
            players[count].experience=current_player.experience;
            count++;
        }
        score_file=fopen("score.txt","w");
        for(int i=0;i<count;i++){
            fprintf(score_file,"%s %d %d %d %d\n",players[i].user_name,players[i].gold,players[i].score,players[i].num_of_games,players[i].experience);
        }
        fclose(score_file);
        initialize_game();
        break;
        case 1:
        clear();
        if(login_choice){
           char file_name[105];
           snprintf(file_name,sizeof(file_name),"%s.dat",current_player.user_name);
           if(access(file_name,F_OK)==0){
            resume_game();
            current_player.gold+=Gold;
            if(win==1) current_player.score++;
            if(win==1 || win==-1) current_player.experience++;
            score_file=fopen("score.txt","r");
            if(score_file){
                while(fscanf(score_file,"%s %d %d %d %d",players[count].user_name,&players[count].gold,&players[count].score,&players[count].num_of_games,&players[count].experience)==5){
                    count++;
                }
                fclose(score_file);
            }
           for(int i=0;i<count;i++){
            if(strcmp(players[i].user_name,current_player.user_name)==0){
                players[i].gold+=Gold;
                if(win==1) players[i].score++;
                if(win==1 || win==-1) players[i].experience++;
                break;
            }
           }
           score_file=fopen("score.txt","w");
           for(int i=0;i<count;i++){
            fprintf(score_file,"%s %d %d %d %d\n",players[i].user_name,players[i].gold,players[i].score,players[i].num_of_games,players[i].experience);
           }
           fclose(score_file);
           }
        }
        break;
        case 2:
        clear();
        score_file=fopen("score.txt","r");
        if(score_file){
                while(fscanf(score_file,"%s %d %d %d %d",players[count].user_name,&players[count].gold,&players[count].score,&players[count].num_of_games,&players[count].experience)==5){
                    count++;
                }
                fclose(score_file);
        }
        qsort(players,count,sizeof(Player),compare_score);
        const wchar_t gold[]=L"\U0001F947";
        const wchar_t silver[]=L"\U0001F948";
        const wchar_t bronze[]=L"\U0001F949";
        attron(COLOR_PAIR(5));
        mvprintw(0,0,"=== SCORE BOARD ===");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(2));
        if(count){
        mvaddnwstr(1,0,gold,-1);
        if(strcmp(players[0].user_name,current_player.user_name)==0){
            attron(A_BOLD);
            mvprintw(1,3,"1.(GOAT) %s - GOLD:%d - SCORE:%d - NUM-OF-GAMES:%d - XP:%d",players[0].user_name,players[0].gold,players[0].score,players[0].num_of_games,players[0].experience);
            attroff(A_BOLD);
        }
        else mvprintw(1,3,"1.(GOAT) %s - GOLD:%d - SCORE:%d - NUM-OF-GAMES:%d - XP:%d",players[0].user_name,players[0].gold,players[0].score,players[0].num_of_games,players[0].experience);
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        if(count>1){
        mvaddnwstr(2,0,silver,-1);
        if(strcmp(players[1].user_name,current_player.user_name)==0){
            attron(A_BOLD);
            mvprintw(2,3,"2.(PRO) %s - GOLD:%d - SCORE:%d - NUM-OF-GAMES:%d - XP:%d",players[1].user_name,players[1].gold,players[1].score,players[1].num_of_games,players[1].experience);
            attroff(A_BOLD);
        }
        else mvprintw(2,3,"2.(PRO) %s - GOLD:%d - SCORE:%d - NUM-OF-GAMES:%d - XP:%d",players[1].user_name,players[1].gold,players[1].score,players[1].num_of_games,players[1].experience);
        }
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(4));
        if(count>2){
        mvaddnwstr(3,0,bronze,-1);
        if(strcmp(players[2].user_name,current_player.user_name)==0){
            attron(A_BOLD);
            mvprintw(3,3,"3.(NOOB+) %s - GOLD:%d - SCORE:%d - NUM-OF-GAMES:%d - XP:%d",players[2].user_name,players[2].gold,players[2].score,players[2].num_of_games,players[2].experience);
            attroff(A_BOLD);
        }
        else mvprintw(3,3,"3.(NOOB+) %s - GOLD:%d - SCORE:%d - NUM-OF-GAMES:%d - XP:%d",players[2].user_name,players[2].gold,players[2].score,players[2].num_of_games,players[2].experience);
        }
        attroff(COLOR_PAIR(4));
        for(int i=3;i<count;i++){
            if(strcmp(players[i].user_name,current_player.user_name)==0){
                attron(A_BOLD);
            }
            mvprintw(i+1,3,"%d. %s - GOLD:%d - SCORE:%d - NUM-OF-GAMES:%d - XP:%d",i+1,players[i].user_name,players[i].gold,players[i].score,players[i].num_of_games,players[i].experience);
            if(strcmp(players[i].user_name,current_player.user_name)==0){
                attroff(A_BOLD);
            }
        }
        }
        refresh();
        getch();
        break;
        case 3:
        quit_from_setting=1;
        clear();
        int highlight=0;
        int key;
        int choice_setting=-1;
        while(1){
        setting_menu(highlight);
        key=getch();
        switch(key){
            case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < 3) { 
                    highlight++;
                }
                break;
            case 10:
                choice_setting = highlight;
                setting_choice(choice_setting);
                break;
            default:
                break;
            }
            if(quit==0) break;
            if(quit_from_setting==0) break;
        }
        break;
        case 4:
        clear();
        highlight=0;
        key;
        int choice_menu=-1;
        while(1){
        initial_menu(highlight);
        key=getch();
        switch(key){
        case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < 3) { 
                    highlight++;
                }
                break;
            case 10:
                choice_menu = highlight;
                menu_choice(choice_menu);
                break;
            default:
                break;
            }
            if(quit==0) break;
        }
        break;
     }
}


void menu_choice(int choice){
    switch(choice){
        case 0:
        clear();
        login_choice=1;
        signup();
        break;
        case 1:
        clear();
        login_choice=1;
        login();
        break;
        case 2:
        clear();
        login_choice=0;
        guest();
        case 3:
        clear();
        quit=0;
        break;
    }
}


void save_name(char *name){
    FILE *fptr=fopen("name.txt","a");
    fprintf(fptr,"%s\n",name);
    fclose(fptr);
}


void save_user(char *name,char *pass,char *reminder,char *email){
    FILE *fptr=fopen("user.txt", "a");
    fprintf(fptr, "name:%s,pass:%s,reminder:%s,email:%s\n" , name,pass,reminder,email);
    fclose(fptr);
}


int check_name(char *name){
    //printw("in\n");
    //refresh();
    FILE *fpt=fopen("name.txt" , "r");
    if(fpt == NULL){
        //fopen("name.txt","a");
        //printw("file doesn't exist!\n");
        //refresh();
        return 1;
    }
    char line[2000];
    int line_num=0;
    while(fgets(line,sizeof(line),fpt)){
        line[strcspn(line,"\n")]='\0';
        if(strcmp(line,name)==0){
            printw("This username already exists!\n");
            fclose(fpt);
            return 0;
        }
    }
    fclose(fpt);
    return 1;
}


int check_pass(char *pass){
    int big_char=0;
    int small_char=0;
    int number=0;
    int pass_length=strlen(pass);
     while(*pass){
          if(*pass>='A' && *pass<='Z'){
            big_char=1;
          }
          else if(*pass>='a' && *pass<='z'){
            small_char=1;
          }
          else if(*pass>='0' && *pass<='9'){
            number=1;
          }
          pass++;
     }
     if(pass_length>=7 && big_char==1 && small_char==1 && number==1){
        return 1;
     }
     else{
        printw("Password should consist of at least 7 characters,a number,one uppercase letter and a lowercase one!\n");
        refresh();
        return 0;
     }
}

void create_password(char *pass,size_t length){
    const char *lowercase = "abcdefghijklmnopqrstuvwxyz";
    const char *uppercase="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *numbers="1234567890";
    const char *all="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    srand(time(NULL));
    pass[0]=lowercase[rand()%26];
    pass[1]=uppercase[rand()%26];
    pass[2]=numbers[rand()%10];
    for(size_t i=3;i<7;i++){
        pass[i]=all[rand()%62];
    }
    pass[7]='\0';
    for(size_t i=0;i<7;i++){
        size_t j=rand()%7;
        char temp=pass[i];
        pass[i]=pass[j];
        pass[j]=temp;
    }
}

void signup(void){
    choice_color=-1;
    choice_level=-1;
    choice_music=-1;
    echo();
    while(1){
    char name[100];
    char pass[100];
    char email[300];
    char reminder[100];
    while(1){
        printw("Enter your name :");
        refresh();
        getnstr(name,sizeof(name)-1);
        if(check_name(name)){
            save_name(name);
            break;
        }
        else continue;
    }
    while(1){
        printw("Enter your password (or type 'Y' for a suggestion):");
        refresh();
        getnstr(pass,sizeof(pass)-1);
        if(strcmp(pass,"Y")==0){
           create_password(pass,sizeof(pass)-1);
           printw("Suggested password is :%s\n" , pass);
           refresh();
           printw("Enter a reminder for your password:");
           refresh();
           getnstr(reminder,sizeof((reminder)-1));
           break;
        }
        else{
        if(check_pass(pass)){
            printw("Enter a reminder for your password:");
            refresh();
            getnstr(reminder,sizeof((reminder)-1));
            break;
        }
        else continue;
        } 
    }
    while(1){    
        printw("Enter your email :");
        refresh();
        getnstr(email,sizeof(email)-1);
        if(check_email(email)){
            break;
        }
        else continue;
    }
    save_user(name,pass,reminder,email);
    clear();
    printw("Your information have been saved.\n");
    refresh();
    printw("Press any key to exit.");
    refresh();
    getch();
    clear();
    strcpy(current_player.user_name,name);
    break;   
    }
    current_player.score=0;
    current_player.gold=0;
    current_player.num_of_games=0;
    current_player.experience=0;
    // FILE *score_file=fopen("score.dat","r+b");
    // fwrite(&current_player,sizeof(Player),1,score_file);
    // fclose(score_file);
    int highlight=0;
    int key;
    int choice=-1;
    while(1){
    game_menu(highlight);
    key=getch();
    switch(key){
        case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < 4) { 
                    highlight++;
                }
                break;
            case 10:
                choice = highlight;
                game_choice(choice);
                break;
            default:
                break;
    }
    if(quit==0) break;
    }
}


void login(void){
    char name[100];
    char pass[100];
    int name_found=0;
    choice_color=-1;
    choice_level=-1;
    choice_music=-1;
    echo();
    while(1){
    clear();    
    printw("Enter your name:");
    refresh();
    getnstr(name,sizeof(name)-1);
    FILE *fptr=fopen("name.txt" , "r");
    char line[2000];
    int line_num=0;
    while(fgets(line,sizeof(line),fptr)){
        line[strcspn(line,"\n")]='\0';
        if(strcmp(line,name)==0){
            name_found=1;
            fclose(fptr);
            break;
        }
    }
    if(name_found){
       strcpy(current_player.user_name,name);
       break;
    }
    else{
        clear();
        printw("Username is wrong!");
        refresh();
        getch();
        continue;
    }
    }
    while(1){
    clear();    
    printw("Enter your password:");
    refresh();
    getnstr(pass,sizeof(pass)-1);
    FILE *file=fopen("user.txt" , "r");
    char line[2000];
    char stored_name[100],stored_pass[100],stored_reminder[100],stored_email[300];
    int pass_found=0;
    while(fgets(line,sizeof(line),file)){
        sscanf(line,"name:%[^,],pass:%[^,],reminder:%[^,],email:%s",stored_name,stored_pass,stored_reminder,stored_email);
        if(strcmp(stored_name,name)==0){
            if(strcmp(stored_pass,pass)==0){
                pass_found=1;
            }
            else{
                printw("Password is wrong!\n");
                refresh();
                printw("This is your reminder:%s\n" , stored_reminder);
                refresh();
                getch();
            }
            break;
        }
    }
    fclose(file);
    if(pass_found){
        break;
    } 
    }
    current_player.experience=0;
    current_player.num_of_games=0;
    current_player.score=0;
    current_player.gold=0;
    int highlight=0;
    int key;
    int choice=-1;
    clear();
    printw("Hi %s!Welcome!" , current_player.user_name);
    refresh();
    getch();
    clear();
    while(1){
    game_menu(highlight);
    key=getch();
    switch(key){
        case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < 4) { 
                    highlight++;
                }
                break;
            case 10:
                choice = highlight;
                game_choice(choice);
                break;
            default:
                break;
    }
    if(quit==0) break;
    }
}


void guest(void){
    choice_color=-1;
    choice_level=-1;
    choice_music=-1;
    clear();
    int highlight=0;
    int key;
    int choice=-1;
    strcpy(current_player.user_name,"guest");
    current_player.experience=0;
    current_player.num_of_games=0;
    current_player.score=0;
    current_player.gold=0;
    while(1){
    game_menu(highlight);
    key=getch();
    switch(key){
        case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < 4) { 
                    highlight++;
                }
                break;
            case 10:
                choice = highlight;
                game_choice(choice);
                break;
            default:
                break;
    }
    if(quit==0) break;
    }
}
 

int check_email(char *email){
    int x=0;
    int atsign=0;
    int y=0;
    int dot=0;
    int z=0;
    
        while(*email!='\0' && ((*email>='a' && *email<='z') || (*email>='A' && *email<='Z'))){
            email++;
            x++;
        }
        if(*email!='\0' && *email=='@'){ 
            atsign=1;
            email++;
        }
        while(*email!='\0' && ((*email>='a' && *email<='z') || (*email>='A' && *email<='Z'))){
            email++;
            y++;
        }
        if(*email!='\0' && *email=='.'){
            dot=1;
            email++;
        }
        while(*email!='\0' && ((*email>='a' && *email<='z') || (*email>='A' && *email<='Z'))){
            z++;
            email++;
        }
        if(x && y && z && atsign && dot){
            return 1;
        }
        else{
            printw("Email should be in form of XXX@Y.ZZZ\n");
            refresh();
            return 0;
        }    
}

void new_game(){
    clear();
    refresh();
    int message=1;
    message_box(message,0);
    usleep(1000000);

    FILE *file_one=fopen("floor_one.txt" , "w");
    FILE *file_two=fopen("floor_two.txt" , "w");
    FILE *file_three=fopen("floor_three.txt" , "w");
    FILE *file_four=fopen("floor_four.txt" , "w");

    srand(time(NULL));
    initialize(rooms_one,doors_one,hidden_door_one,pillows_one,windows_one,traps_one,visited_one);
    floor_one=(char **)malloc(LINES * sizeof(char *));
    visit_map_one=(int **)malloc(LINES * sizeof(int *));
    for(int i=0;i<LINES;i++){
        floor_one[i]=(char *)malloc(COLS * sizeof(char));
        visit_map_one[i]=(int *)malloc(COLS * sizeof(int));
    }
    create_map(floor_one,0,file_one,rooms_one,windows_one,doors_one,pillows_one,traps_one,visit_map_one);
    initialize(rooms_two,doors_two,hidden_door_two,pillows_two,windows_two,traps_two,visited_two);
    floor_two=(char **)malloc(LINES * sizeof(char *));
    visit_map_two=(int **)malloc(LINES * sizeof(int *));
    for(int i=0;i<LINES;i++){
        floor_two[i]=(char *)malloc(COLS * sizeof(char));
        visit_map_two[i]=(int *)malloc(COLS * sizeof(int));
    }
    create_map(floor_two,1,file_two,rooms_two,windows_two,doors_two,pillows_two,traps_two,visit_map_two);
    initialize(rooms_three,doors_three,hidden_door_three,pillows_three,windows_three,traps_three,visited_three);
    visit_map_three=(int **)malloc(LINES * sizeof(int *));
    floor_three=(char **)malloc(LINES * sizeof(char *));
    for(int i=0;i<LINES;i++){
        floor_three[i]=(char *)malloc(COLS * sizeof(char));
        visit_map_three[i]=(int *)malloc(COLS * sizeof(int));
    }
    create_map(floor_three,2,file_three,rooms_three,windows_three,doors_three,pillows_three,traps_three,visit_map_three);
    initialize(rooms_four,doors_four,hidden_door_four,pillows_four,windows_four,traps_four,visited_four);
    visit_map_four=(int **)malloc(LINES * sizeof(int *));
    floor_four=(char **)malloc(LINES * sizeof(char *));
    for(int i=0;i<LINES;i++){
        floor_four[i]=(char *)malloc(COLS * sizeof(char));
        visit_map_four[i]=(int *)malloc(COLS * sizeof(int));
    }
    create_map(floor_four,3,file_four,rooms_four,windows_four,doors_four,pillows_four,traps_four,visit_map_four);
    

    for(int j=0;j<LINES;j++){
        for(int i=0;i<COLS;i++){
            visit_map_one[j][i]=0;
        }
    }
    for(int j=0;j<LINES;j++){
        for(int i=0;i<COLS;i++){
            visit_map_two[j][i]=0;
        }
    }
    for(int j=0;j<LINES;j++){
        for(int i=0;i<COLS;i++){
            visit_map_three[j][i]=0;
        }
    }
    for(int j=0;j<LINES;j++){
        for(int i=0;i<COLS;i++){
            visit_map_four[j][i]=0;
        }
    }
    noecho();
    end=0,win=0;
    game(0,0,floor_one,rooms_one,doors_one,traps_one,visited_one,visit_map_one);

    for(int i=0;i<LINES;i++){
        free(floor_one[i]);
        free(visit_map_one[i]);
    }
    free(floor_one);
    free(visit_map_one);
    for(int i=0;i<LINES;i++){
        free(floor_two[i]);
        free(visit_map_two[i]);
    }
    free(floor_two);
    free(visit_map_two);
    for(int i=0;i<LINES;i++){
        free(floor_three[i]);
        free(visit_map_three[i]);
    }
    free(floor_three);
    free(visit_map_three);
    for(int i=0;i<LINES;i++){
        free(floor_four[i]);
        free(visit_map_four[i]);
    }
    free(floor_four);
    free(visit_map_four);
}


void resume_game(){
    floor_one=(char **)malloc(LINES * sizeof(char *));
    visit_map_one=(int **)malloc(LINES * sizeof(int *));
    for(int i=0;i<LINES;i++){
        floor_one[i]=(char *)malloc(COLS * sizeof(char));
        visit_map_one[i]=(int *)malloc(COLS * sizeof(int));
    }
    
    floor_two=(char **)malloc(LINES * sizeof(char *));
    visit_map_two=(int **)malloc(LINES * sizeof(int *));
    for(int i=0;i<LINES;i++){
        floor_two[i]=(char *)malloc(COLS * sizeof(char));
        visit_map_two[i]=(int *)malloc(COLS * sizeof(int));
    }

    visit_map_three=(int **)malloc(LINES * sizeof(int *));
    floor_three=(char **)malloc(LINES * sizeof(char *));
    for(int i=0;i<LINES;i++){
        floor_three[i]=(char *)malloc(COLS * sizeof(char));
        visit_map_three[i]=(int *)malloc(COLS * sizeof(int));
    }
    
    visit_map_four=(int **)malloc(LINES * sizeof(int *));
    floor_four=(char **)malloc(LINES * sizeof(char *));
    for(int i=0;i<LINES;i++){
        floor_four[i]=(char *)malloc(COLS * sizeof(char));
        visit_map_four[i]=(int *)malloc(COLS * sizeof(int));
    }
    initialize(rooms_one,doors_one,hidden_door_one,pillows_one,windows_one,traps_one,visited_one);
    initialize(rooms_two,doors_two,hidden_door_two,pillows_two,windows_two,traps_two,visited_two);
    initialize(rooms_three,doors_three,hidden_door_three,pillows_three,windows_three,traps_three,visited_three);
    initialize(rooms_four,doors_four,hidden_door_four,pillows_four,windows_four,traps_four,visited_four);
    initialize_game();
    initialize_floor(floor_one,visit_map_one);
    initialize_floor(floor_two,visit_map_two);
    initialize_floor(floor_three,visit_map_three);
    initialize_floor(floor_four,visit_map_four);
    char file_name[105];
    snprintf(file_name, sizeof(file_name), "%s.dat", current_player.user_name);
    FILE *game_file = fopen(file_name, "rb");
    fread(rooms_one, sizeof(room), MAX_ROOMS, game_file);
    fread(rooms_two, sizeof(room), MAX_ROOMS, game_file);
    fread(rooms_three, sizeof(room), MAX_ROOMS, game_file);
    fread(rooms_four, sizeof(room), MAX_ROOMS, game_file);
    fread(&stair_room, sizeof(door), 1, game_file);
    fread(doors_one, sizeof(door), MAX_ROOMS * 40, game_file);
    fread(doors_two, sizeof(door), MAX_ROOMS * 40, game_file);
    fread(doors_three, sizeof(door), MAX_ROOMS * 40, game_file);
    fread(doors_four, sizeof(door), MAX_ROOMS * 40, game_file);
    fread(hidden_door_one, sizeof(door), MAX_ROOMS * 40, game_file);
    fread(hidden_door_two, sizeof(door), MAX_ROOMS * 40, game_file);
    fread(hidden_door_three, sizeof(door), MAX_ROOMS * 40, game_file);
    fread(hidden_door_four, sizeof(door), MAX_ROOMS * 40, game_file);
    fread(pillows_one, sizeof(pillow), MAX_ROOMS * 3, game_file);
    fread(pillows_two, sizeof(pillow), MAX_ROOMS * 3, game_file);
    fread(pillows_three, sizeof(pillow), MAX_ROOMS * 3, game_file);
    fread(pillows_four, sizeof(pillow), MAX_ROOMS * 3, game_file);
    fread(windows_one, sizeof(window_room), MAX_ROOMS * 10, game_file);
    fread(windows_two, sizeof(window_room), MAX_ROOMS * 10, game_file);
    fread(windows_three, sizeof(window_room), MAX_ROOMS * 10, game_file);
    fread(windows_four, sizeof(window_room), MAX_ROOMS * 10, game_file);
    fread(traps_one, sizeof(trap), MAX_ROOMS * 10, game_file);
    fread(traps_two, sizeof(trap), MAX_ROOMS * 10, game_file);
    fread(traps_three, sizeof(trap), MAX_ROOMS * 10, game_file);
    fread(traps_four, sizeof(trap), MAX_ROOMS * 10, game_file);
    fread(&stair, sizeof(stairs), 1, game_file);
    fread(deamon, sizeof(enemy), 4, game_file);
    fread(fire_breathing, sizeof(enemy), 4, game_file);
    fread(giant, sizeof(enemy), 4, game_file);
    fread(snake, sizeof(enemy), 4, game_file);
    fread(undeed, sizeof(enemy), 4, game_file);   
    for(int i=0;i<LINES;i++){
        fread(floor_one[i],sizeof(char),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fread(floor_two[i],sizeof(char),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fread(floor_three[i],sizeof(char),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fread(floor_four[i],sizeof(char),COLS,game_file);
    }
    fread(visited_one, sizeof(int), MAX_ROOMS, game_file);
    fread(visited_two, sizeof(int), MAX_ROOMS, game_file);  
    fread(visited_three, sizeof(int), MAX_ROOMS, game_file);
    fread(visited_four, sizeof(int), MAX_ROOMS, game_file);
    for(int i=0;i<LINES;i++){
        fread(visit_map_one[i],sizeof(int),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fread(visit_map_two[i],sizeof(int),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fread(visit_map_three[i],sizeof(int),COLS,game_file);
    }
    for(int i=0;i<LINES;i++){
        fread(visit_map_four[i],sizeof(int),COLS,game_file);
    }
    fread(enchanted, sizeof(int), 4 * 1, game_file);
    fread(&Heart, sizeof(int), 1, game_file);
    fread(&Gold, sizeof(int), 1, game_file);
    fread(&Health, sizeof(int), 1, game_file);
    fread(&hunger, sizeof(int), 1, game_file);
    fread(&food_collected, sizeof(int), 1, game_file);
    fread(&dagger_count, sizeof(int), 1, game_file);
    fread(&magic_wand_count, sizeof(int), 1, game_file);
    fread(&normal_arrow_count, sizeof(int), 1, game_file);
    fread(&sword_count, sizeof(int), 1, game_file);
    fread(&mace_damage, sizeof(int), 1, game_file);
    fread(&dagger_damage, sizeof(int), 1, game_file);
    fread(&magic_wand_damage, sizeof(int), 1, game_file);
    fread(&normal_arrow_damage, sizeof(int), 1, game_file);
    fread(&sword_damage, sizeof(int), 1, game_file);
    fread(&default_weapon, sizeof(char), 1, game_file);
    fread(&last_hit, sizeof(char), 1, game_file);
    fread(&spell, sizeof(char), 1, game_file);
    fread(&health_spell, sizeof(int), 1, game_file);
    fread(&speed_spell, sizeof(int), 1, game_file);
    fread(&damage_spell, sizeof(int), 1, game_file);
    fread(&health_count, sizeof(int), 1, game_file);
    fread(&speed_count, sizeof(int), 1, game_file);
    fread(&damage_count, sizeof(int), 1, game_file);
    fread(&enchanted_stay, sizeof(int), 1, game_file);
    fread(&which_floor_save, sizeof(int), 1, game_file);
    fread(&posx_save, sizeof(int), 1, game_file);
    fread(&posy_save, sizeof(int), 1, game_file);
    fread(&end,sizeof(int),1,game_file);
    fread(&win,sizeof(int),1,game_file);
    fclose(game_file);
    // FILE *file_one=fopen("floorcopy_one.txt" , "w");
    // save_floor(floor_one,file_one);
    // FILE *file_two=fopen("floorcopy_two.txt" , "w");
    // save_floor(floor_two,file_two);
    // FILE *file_three=fopen("floorcopy_three.txt" , "w");
    // save_floor(floor_three,file_three);
    // FILE *file_four=fopen("floorcopy_four.txt" , "w");
    // save_floor(floor_four,file_four);
    Gold=0;
    noecho();
    if(which_floor_save==0){
      game(2,which_floor_save,floor_one,rooms_one,doors_one,traps_one,visited_one,visit_map_one);
    } 
    else if(which_floor_save==1){
      game(2,which_floor_save,floor_two,rooms_two,doors_two,traps_two,visited_two,visit_map_two);
    }
    else if(which_floor_save==2){
      game(2,which_floor_save,floor_three,rooms_three,doors_three,traps_three,visited_three,visit_map_three);
    }
    else if(which_floor_save==3){
      game(2,which_floor_save,floor_four,rooms_four,doors_four,traps_four,visited_four,visit_map_four);
    }
    for(int i=0;i<LINES;i++){
        free(floor_one[i]);
        free(visit_map_one[i]);
    }
    free(floor_one);
    free(visit_map_one);
    for(int i=0;i<LINES;i++){
        free(floor_two[i]);
        free(visit_map_two[i]);
    }
    free(floor_two);
    free(visit_map_two);
    for(int i=0;i<LINES;i++){
        free(floor_three[i]);
        free(visit_map_three[i]);
    }
    free(floor_three);
    free(visit_map_three);
    for(int i=0;i<LINES;i++){
        free(floor_four[i]);
        free(visit_map_four[i]);
    }
    free(floor_four);
    free(visit_map_four);
}


int main(){
    initscr();
    keypad(stdscr,TRUE);
    setlocale(LC_ALL,"");
    cbreak();
    echo();
    start_color();
    if(has_colors()){
        init_color(COLOR_BLACK,49 * 1000/255,2*1000/255,54*1000/255);
        init_pair(1,COLOR_RED,COLOR_BLACK);
        init_pair(2,COLOR_YELLOW,COLOR_BLACK);
        init_pair(3,COLOR_GREEN,COLOR_BLACK);
        init_pair(4,COLOR_BLUE,COLOR_BLACK);
        init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
    }
    curs_set(FALSE);
    refresh();
    int highlight=0;
    int key;
    int choice=-1;
    while(1){
    initial_menu(highlight);
    key=getch();
    switch(key){
        case KEY_UP:
                if (highlight > 0) {
                    highlight--;
                }
                break;
            case KEY_DOWN:
                if (highlight < 3) { 
                    highlight++;
                }
                break;
            case 10:
                choice = highlight;
                menu_choice(choice);
                break;
            default:
                break;
    }
    if(quit==0) break;
    }
    curs_set(0);
    endwin();
    return 0;
}



