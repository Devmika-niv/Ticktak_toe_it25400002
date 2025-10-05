#include<stdio.h>
#include<string.h>
#include <stdlib.h>

void clear_stdin(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

char **initialize_board(int n ){

    char **board=malloc(n*sizeof(char*));
    for(int i=0;i<n;i++){
        board[i]=malloc(n*sizeof(char));
        for( int j=0;j<n;j++){

            board[i][j]=' ';
        }
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

static void get_human_move(int n, char **board, int *r, int *c) {
    int rows , columns;
    while (1) {
        printf("Enter rows(1-3) and columns(1-3): ");
        if (scanf("%d %d", &rows, &columns) != 2) { clear_stdin(); continue; } //this is find if the user inputs are integers
        clear_stdin();
        if (!is_valid_pos(n, rows, columns)) { printf("Out of range.\n"); continue; } //to find if rows and columns are out of range
        if (board[rows-1][columns-1] != ' ') { printf("Occupied.\n"); continue; }
        *r = rows; *c = columns; 
        break;
    }
}


int main(){
    int n=3;

    char **board = initialize_board(n);
    
    
    char symbols[2] = {'X','O'};
    int current=0,r,c;
    while (1) {
        display_board(board,n);
        printf("Player %d (%c)\n", current+1, symbols[current]);
        get_human_move(n,board,&r,&c);
        make_move(board,n,r,c,symbols[current]);
        if (check_win(board,n,symbols[current])) { display_board(board,n); printf("Player %d wins!\n", current+1); break; }
        if (check_draw(board,n)) { display_board(board,n); printf("Draw!\n"); break; }
        current=1-current;
    }
     free_board(board,n);

        return 0;
}