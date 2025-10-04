#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10
#define MIN_SIZE 3

char board[MAX_SIZE][MAX_SIZE][4];
FILE *logFile;

// Function prototypes
void setupBoard(int size);
void showBoard(int size);
void saveBoardState(int size);
void promptPlayerMove(int size, char player);
void computerMove(int size, char computerSymbol);
int hasPlayerWon(int size, char player);
int isBoardFull(int size);

int main() {
    int size, mode;
    char symbols[3] = {'X','O','Z'};
    int isComputer[3] = {0,0,0}; // 0 human, 1 computer
    int currentPlayerIndex = 0;

    printf("Welcome to Tic Tac Toe!\n");
    printf("Choose your board size (%d to %d): ", MIN_SIZE, MAX_SIZE);
    scanf("%d", &size);
    if (size < MIN_SIZE || size > MAX_SIZE) {
        printf("Oops! That size isn't supported. Try again next time.\n");
        return 1;
    }

    printf("\nSelect game mode:\n");
    printf("1: Player vs Player\n");
    printf("2: Player vs Computer\n");
    printf("3: Multi-Player (3 players)\n");
    scanf("%d", &mode);

    if (mode != 1 && mode != 2 && mode != 3) {
        printf("Sorry, only modes 1,2,3 are supported.\n");
        return 1;
    }

    setupBoard(size);

    if (size >= 4)
        printf("Note: You only need to align 4 symbols in a row, column, or diagonal to win.\n");

    logFile = fopen("game_log.txt", "w");
    if (!logFile) {
        printf("Couldn't open log file. Exiting.\n");
        return 1;
    }

    srand(time(NULL));

    // Configure multi-player roles
    if (mode == 3) {
        char ans;
        printf("Player X is always human.\n");
        printf("Should Player O be computer? (y/n): ");
        scanf(" %c", &ans);
        isComputer[1] = (ans == 'y' || ans == 'Y') ? 1 : 0;

        printf("Should Player Z be computer? (y/n): ");
        scanf(" %c", &ans);
        isComputer[2] = (ans == 'y' || ans == 'Y') ? 1 : 0;
    }

    while (1) {
        showBoard(size);

        if (mode == 1) {                                    // Player vs Player
            char player = (currentPlayerIndex==0)?'X':'O';
            promptPlayerMove(size, player);

            saveBoardState(size);
            if (hasPlayerWon(size, player)) {
                showBoard(size);
                printf("Player %c wins!\n", player);
                fprintf(logFile,"Player %c wins!\n",player);
                break;
            }

            if (isBoardFull(size)) {
                showBoard(size);
                printf("It's a draw!\n");
                fprintf(logFile,"Game ended in a draw.\n");
                break;
            }

            currentPlayerIndex = (currentPlayerIndex+1)%2;
        }

        else if (mode == 2) { // Player vs Computer
            char player = (currentPlayerIndex==0)?'X':'O';
            if (currentPlayerIndex==0)
                promptPlayerMove(size,player);
            else
                computerMove(size,player);

            saveBoardState(size);
            if (hasPlayerWon(size, player)) {
                showBoard(size);
                printf("Player %c wins!\n",player);
                fprintf(logFile,"Player %c wins!\n",player);
                break;
            }

            if (isBoardFull(size)) {
                showBoard(size);
                printf("It's a draw!\n");
                fprintf(logFile,"Game ended in a draw.\n");
                break;
            }

            currentPlayerIndex = (currentPlayerIndex+1)%2;
        }

        else if (mode == 3) { // Multi-Player (3 players)
            char player = symbols[currentPlayerIndex];
            printf("Player %c's turn.\n",player);
            if (isComputer[currentPlayerIndex]==0)
                promptPlayerMove(size,player);
            else
                computerMove(size,player);

            saveBoardState(size);
            if (hasPlayerWon(size, player)) {
                showBoard(size);
                printf("Player %c wins!\n",player);
                fprintf(logFile,"Player %c wins!\n",player);
                break;
            }

            if (isBoardFull(size)) {
                showBoard(size);
                printf("It's a draw!\n");
                fprintf(logFile,"Game ended in a draw.\n");
                break;
            }

            currentPlayerIndex = (currentPlayerIndex+1)%3;
        }
    }

    fclose(logFile);
    printf("Thanks for playing!\n");
    return 0;
}

// Fill the board with numbered strings
void setupBoard(int size) {
    int count = 0;
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++)
            sprintf(board[i][j],"%d",count++);
}

// Display the current board
void showBoard(int size) {
    printf("\n");
    for (int i=0;i<size;i++) {
        printf("   ");
        for (int j=0;j<size;j++) {
            printf(" %2s ",board[i][j]);
            if (j<size-1) printf("|");
        }
        printf("\n");
        if (i<size-1) {
            printf("   ");
            for (int k=0;k<size;k++) {
                printf("----");
                if (k<size-1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Save board state
void saveBoardState(int size) {
    for (int i=0;i<size;i++) {
        for (int j=0;j<size;j++)
            fprintf(logFile,"%s ",board[i][j]);
        fprintf(logFile,"\n");
    }
    fprintf(logFile,"---------------------\n");
}

// Prompt for human move
void promptPlayerMove(int size, char player) {
    int move,row,col;
    char moveStr[4];
    while (1) {
        printf("Player %c, choose a cell number (0 to %d): ",player,size*size-1);
        scanf("%d",&move);
        if (move<0||move>=size*size) {
            printf("Invalid number. Try again.\n");
            continue;
        }
        row = move/size;
        col = move%size;
        sprintf(moveStr,"%d",move);
        if (strcmp(board[row][col],moveStr)==0) {
            sprintf(board[row][col],"%c",player);
            break;
        } else {
            printf("That spot's taken. Try again.\n");
        }
    }
}

// Random computer move
void computerMove(int size, char computerSymbol) {
    int move,row,col;
    char moveStr[12];
    printf("Computer (%c) is making a move...\n",computerSymbol);
    while (1) {
        move = rand()%(size*size);
        row = move/size;
        col = move%size;
        sprintf(moveStr,"%d",move);
        if (strcmp(board[row][col],moveStr)==0) {
            sprintf(board[row][col],"%c",computerSymbol);
            break;
        }
    }
}

// Win check
int hasPlayerWon(int size, char player) {
    char pStr[2]={player,'\0'};
    int winLength=(size>=4)?4:size;

    // Rows
    for (int i=0;i<size;i++)
        for (int j=0;j<=size-winLength;j++) {
            int count=0;
            for (int k=0;k<winLength;k++)
                if (strcmp(board[i][j+k],pStr)==0) count++;
            if (count==winLength) return 1;
        }

    // Columns
    for (int j=0;j<size;j++)
        for (int i=0;i<=size-winLength;i++) {
            int count=0;
            for (int k=0;k<winLength;k++)
                if (strcmp(board[i+k][j],pStr)==0) count++;
            if (count==winLength) return 1;
        }

    // Diagonals TL-BR
    for (int i=0;i<=size-winLength;i++)
        for (int j=0;j<=size-winLength;j++) {
            int count=0;
            for (int k=0;k<winLength;k++)
                if (strcmp(board[i+k][j+k],pStr)==0) count++;
            if (count==winLength) return 1;
        }

    // Diagonals TR-BL
    for (int i=0;i<=size-winLength;i++)
        for (int j=winLength-1;j<size;j++) {
            int count=0;
            for (int k=0;k<winLength;k++)
                if (strcmp(board[i+k][j-k],pStr)==0) count++;
            if (count==winLength) return 1;
        }

    return 0;
}

// Board full?
int isBoardFull(int size) {
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++)
            if (strcmp(board[i][j],"X")!=0 &&
                strcmp(board[i][j],"O")!=0 &&
                strcmp(board[i][j],"Z")!=0)
                return 0;
    return 1;
}

