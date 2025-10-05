#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tictactoe.h"

int main() {
    srand((unsigned) time(NULL));
    int choice, n;

    print_instructions();

    while (1) {
        printf("\n1) Human vs Human\n");
        printf("2) Human vs Computer\n");
        printf("3) Three Player\n");
        printf("4) Exit\n");
        printf("Choice: ");
        if (scanf("%d",&choice)!=1){ while(getchar()!='\n'); continue; }
        if (choice==4) break;
        printf("Enter board size N (3-10): ");
        if (scanf("%d",&n)!=1 || n<MIN_N || n>MAX_N){ while(getchar()!='\n'); continue; }
        if (choice==1) play_human_vs_human(n);
        else if (choice==2) play_human_vs_computer(n);
        else if (choice==3) play_three_player(n);
    }
    return 0;
}
