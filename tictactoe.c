#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "tictactoe.h"

//helpers
static void clear_stdin();
static void wait_enter();
static void get_human_move(int n, char **board, int *r, int *c);
static void get_computer_move(int n, char **board, int *r, int *c);
static void write_board_to_file(FILE *fp, char **board, int n);

//functions
char **init_board(int n) {
    char **board = malloc(n * sizeof(char*));
    for (int i = 0; i < n; ++i) {
        board[i] = malloc(n * sizeof(char));
        for (int j = 0; j < n; ++j) board[i][j] = ' ';
    }
    return board;
}

void free_board(char **board, int n) {
    for (int i = 0; i < n; ++i) free(board[i]);
    free(board);
}

void display_board(char **board, int n) {
    printf("\n   ");
    for (int j = 0; j < n; ++j) printf(" %2d ", j+1);
    printf("\n");

    for (int i = 0; i < n; ++i) {
        printf("   ");
        for (int j = 0; j < n; ++j) printf("----");
        printf("-\n");

        printf("%2d ", i+1);
        for (int j = 0; j < n; ++j) {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
    }
    printf("   ");
    for (int j = 0; j < n; ++j) printf("----");
    printf("-\n");
}

//game mechanics
int is_valid_pos(int n, int r, int c) {
    return (r >= 1 && r <= n && c >= 1 && c <= n);
}

int make_move(char **board, int n, int r, int c, char symbol) {
    if (board[r-1][c-1] == ' ') {
        board[r-1][c-1] = symbol;
        return 1;
    }
    return 0;
}

int check_win(char **board, int n, char symbol) {
    int i, j, win;

    // Rows
    for (i = 0; i < n; ++i) {
        win = 1;
        for (j = 0; j < n; ++j) if (board[i][j] != symbol) win = 0;
        if (win) return 1;
    }
    // Columns
    for (j = 0; j < n; ++j) {
        win = 1;
        for (i = 0; i < n; ++i) if (board[i][j] != symbol) win = 0;
        if (win) return 1;
    }
    // Diagonals
    win = 1;
    for (i = 0; i < n; ++i) if (board[i][i] != symbol) win = 0;
    if (win) return 1;

    win = 1;
    for (i = 0; i < n; ++i) if (board[i][n-1-i] != symbol) win = 0;
    if (win) return 1;

    return 0;
}

int check_draw(char **board, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (board[i][j] == ' ') return 0;
    return 1;
}

// logging
static void write_board_to_file(FILE *fp, char **board, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) fprintf(fp, "[%c]", board[i][j]);
        fprintf(fp, "\n");
    }
}

void log_state(FILE *fp, char **board, int n, int player_no, char symbol, int r, int c) {
    if (!fp) return;
    fprintf(fp, "Player %d (%c) -> row %d col %d\n", player_no, symbol, r, c);
    write_board_to_file(fp, board, n);
    fflush(fp);
}

//utilities
static void clear_stdin() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

static void wait_enter() {
    printf("Press Enter to continue...");
    getchar();
}

void print_instructions() {
    printf("Tic-Tac-Toe (N x N)\n");
    printf("Win by filling a row, column or diagonal with your symbol.\n");
    printf("Game states saved to '%s'\n", LOGFILE);
}

//input helpers
static void get_human_move(int n, char **board, int *r, int *c) {
    int rr, cc;
    while (1) {
        printf("Enter row and column: ");
        if (scanf("%d %d", &rr, &cc) != 2) { clear_stdin(); 
            continue; }
        clear_stdin();

        if (!is_valid_pos(n, rr, cc)) { printf("Out of range.\n");
             continue; }
        if (board[rr-1][cc-1] != ' ') { printf("Occupied.\n"); 
            continue; }
        *r = rr; *c = cc; break;
    }
}

static void get_computer_move(int n, char **board, int *r, int *c) {
    int empties = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (board[i][j] == ' ') empties++;
    int target = rand() % empties, idx = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (board[i][j] == ' ') {
                if (idx == target) { *r=i+1; *c=j+1; return; }
                idx++;
            }
}

//game modes
void play_human_vs_human(int n) {
    char **board = init_board(n);
    FILE *fp = fopen(LOGFILE, "a");
    char symbols[2] = {'X','O'};
    int current=0,r,c;
    while (1) {
        display_board(board,n);
        printf("Player %d (%c)\n", current+1, symbols[current]);
        get_human_move(n,board,&r,&c);
        make_move(board,n,r,c,symbols[current]);
        log_state(fp,board,n,current+1,symbols[current],r,c);
        if (check_win(board,n,symbols[current])) { display_board(board,n); 
            printf("Player %d wins!\n", current+1); 
            break; }
        if (check_draw(board,n)) { display_board(board,n); 
            printf("Draw!\n"); 
            break; }
        current=1-current;
    }
    fclose(fp); free_board(board,n); wait_enter();
}

void play_human_vs_computer(int n) {
    char **board = init_board(n);
    FILE *fp = fopen(LOGFILE,"a");
    char symbols[2]={'X','O'}; 
    int current=0,r,c;
    while(1){
        display_board(board,n);
        if(current==0){ printf("Your move (X)\n"); 
            get_human_move(n,board,&r,&c);}
        else { printf("Computer (O)\n"); 
            get_computer_move(n,board,&r,&c); 
            printf("Computer chose %d %d\n",r,c);}

        make_move(board,n,r,c,symbols[current]);

        log_state(fp,board,n,current+1,symbols[current],r,c);

        if(check_win(board,n,symbols[current])){ display_board(board,n); 
            printf("%s wins!\n", current==0?"You":"Computer"); 
            break;}
        if(check_draw(board,n)){ display_board(board,n); 
            printf("Draw!\n"); 
            break;}
        current=1-current;
    }
    fclose(fp); free_board(board,n); wait_enter();
}

void play_three_player(int n) {
    char **board=init_board(n);
    FILE *fp=fopen(LOGFILE,"a");
    char symbols[3]={'X','O','Z'};
    int roles[3]={0,1,1}; // by default: Player1 human, others comp
    int r,c,current=0;
    while(1){
        display_board(board,n);
        printf("Player %d (%c) %s\n",current+1,symbols[current],roles[current]==0?"Human":"Computer");
        if(roles[current]==0) get_human_move(n,board,&r,&c);

        else { get_computer_move(n,board,&r,&c); 
            printf("Computer chose %d %d\n",r,c);}

        make_move(board,n,r,c,symbols[current]);

        log_state(fp,board,n,current+1,symbols[current],r,c);

        if(check_win(board,n,symbols[current])){ display_board(board,n); 
            printf("Player %d (%c) wins!\n",current+1,symbols[current]); 
            break;}
        if(check_draw(board,n)){ display_board(board,n); 
            printf("Draw!\n"); 
            break;}
        
        current=(current+1)%3;
    }
    fclose(fp); free_board(board,n); wait_enter();
}
