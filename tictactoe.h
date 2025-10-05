#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <stdio.h>

#define LOGFILE "game_log.txt"
#define MIN_N 3
#define MAX_N 10

/* Board management */
char **init_board(int n);
void free_board(char **board, int n);
void display_board(char **board, int n);

/* Game mechanics */
int is_valid_pos(int n, int r, int c);
int make_move(char **board, int n, int r, int c, char symbol);
int check_win(char **board, int n, char symbol);
int check_draw(char **board, int n);

/* Logging */
void log_state(FILE *fp, char **board, int n, int player_no, char symbol, int r, int c);

/* Utilities */
void print_instructions();
void play_human_vs_human(int n);
void play_human_vs_computer(int n);
void play_three_player(int n);

#endif
