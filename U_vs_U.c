#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 10
#define MIN_SIZE 3

char board[MAX_SIZE][MAX_SIZE][4];
FILE *logFile;

// Function prototypes
void setupBoard(int size);
void showBoard(int size);
void saveBoardState(int size);
int isCellAvailable(int row, int col, int size);
void promptPlayerMove(int size, char player);
int hasPlayerWon(int size, char player);
int isBoardFull(int size);
void togglePlayer(char *player);


int main() {
    int size, mode;
    char playerSymbol, opponentSymbol, currentPlayer;


    //  Board size selection
    printf("Welcome to Tic Tac Toe!\n");
    printf("Choose your board size (%d to %d): ", MIN_SIZE, MAX_SIZE);
    scanf("%d", &size);

    if (size < MIN_SIZE || size > MAX_SIZE) {
        printf("Oops! That size isn't supported. Try again next time.\n");
        return 1;
    }


    // Game mode selection
    printf("\nSelect game mode:\n");
    printf("1: Player vs Player\n");
    printf("2: Player vs Computer (coming soon!)\n");
    printf("3: Multiplayer (coming soon!)\n");
    scanf("%d", &mode);

    if (mode != 1) {
        printf("Sorry, only Player vs Player mode is available right now.\n");
        return 1;
    }



    // Player symbol selection
    do {
        printf("\nPick your symbol (X or O): ");
        scanf(" %c", &playerSymbol);
        playerSymbol = toupper(playerSymbol);
    } while (playerSymbol != 'X' && playerSymbol != 'O');

    opponentSymbol = (playerSymbol == 'X') ? 'O' : 'X';
    currentPlayer = playerSymbol;




    // Initialize board with numbered cells
    setupBoard(size);



    // Informing players about the win condition for larger boards
    if (size >= 4) {
        printf("Note: You only need to align 4 symbols in a row, column, or diagonal to win.\n");
    }



    // Opening a log file to write & save game progress
    logFile = fopen("game_log.txt", "w");
    if (!logFile) {
        printf("Couldn't open log file. Exiting.\n");
        return 1;
    }



    //  Main game loop (playing part)
    while (1) {
        showBoard(size);                      // Display current board
        promptPlayerMove(size, currentPlayer); // Ask current player for move
        saveBoardState(size);                 // Save board to log file

        if (hasPlayerWon(size, currentPlayer)) {
            showBoard(size);
            printf("Player %c wins!\n", currentPlayer);
            fprintf(logFile, "Player %c wins!\n", currentPlayer);
            break;
        }

        if (isBoardFull(size)) {
            showBoard(size);
            printf("It's a draw!\n");
            fprintf(logFile, "Game ended in a draw.\n");
            break;
        }

        togglePlayer(&currentPlayer); // Switch turns
    }

    fclose(logFile);
    printf("Thanks for playing!\n");
    return 0;
}



//  Filling the board with numbered strings 
void setupBoard(int size) {
    int count = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            sprintf(board[i][j], "%d", count++);
}



//  Displaying the current board
void showBoard(int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("   ");
        for (int j = 0; j < size; j++) {
            printf(" %2s ", board[i][j]);
            if (j < size - 1) printf("|");
        }
        printf("\n");
        if (i < size - 1) {
            printf("   ");
            for (int k = 0; k < size; k++) {
                printf("----");
                if (k < size - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}



//  Save the current board state to the log file
void saveBoardState(int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(logFile, "%s ", board[i][j]);
        }
        fprintf(logFile, "\n");
    }
    fprintf(logFile, "---------------------\n");
}



// Checking if a cell is available 
int isCellAvailable(int row, int col, int size) {
    return row >= 0 && row < size && col >= 0 && col < size &&
           strcmp(board[row][col], "X") != 0 && strcmp(board[row][col], "O") != 0;
}



//  Asking player for their move and update the board
void promptPlayerMove(int size, char player) {
    int move, row, col;
    char moveStr[4];
    while (1) {
        printf("Player %c, choose a cell number (0 to %d): ", player, size * size - 1);
        scanf("%d", &move);
        if (move < 0 || move >= size * size) {
            printf("Invalid number. Try again.\n");
            continue;
        }
        row = move / size;
        col = move % size;
        sprintf(moveStr, "%d", move);
        if (strcmp(board[row][col], moveStr) == 0) {
            sprintf(board[row][col], "%c", player);
            break;
        } else {
            printf("That spot's taken. Try again.\n");
        }
    }
}



// Check if the current player has won
int hasPlayerWon(int size, char player) {
    char pStr[2] = {player, '\0'};
    int winLength = (size >= 4) ? 4 : size;


    // Check rows
    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= size - winLength; j++) {
            int count = 0;
            for (int k = 0; k < winLength; k++) {
                if (strcmp(board[i][j + k], pStr) == 0)
                    count++;
            }
            if (count == winLength) return 1;
        }
    }


    // Check columns
    for (int j = 0; j < size; j++) {
        for (int i = 0; i <= size - winLength; i++) {
            int count = 0;
            for (int k = 0; k < winLength; k++) {
                if (strcmp(board[i + k][j], pStr) == 0)
                    count++;
            }
            if (count == winLength) return 1;
        }
    }


    // Check diagonals (top-left to bottom-right)
    for (int i = 0; i <= size - winLength; i++) {
        for (int j = 0; j <= size - winLength; j++) {
            int count = 0;
            for (int k = 0; k < winLength; k++) {
                if (strcmp(board[i + k][j + k], pStr) == 0)
                    count++;
            }
            if (count == winLength) return 1;
        }
    }


    // Check diagonals (top-right to bottom-left)
    for (int i = 0; i <= size - winLength; i++) {
        for (int j = winLength - 1; j < size; j++) {
            int count = 0;
            for (int k = 0; k < winLength; k++) {
                if (strcmp(board[i + k][j - k], pStr) == 0)
                    count++;
            }
            if (count == winLength) return 1;
        }
    }

    return 0;
}



// Checking if the board is full 
int isBoardFull(int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (strcmp(board[i][j], "X") != 0 && strcmp(board[i][j], "O") != 0)
                return 0;
    return 1;
}



// Switch turns between players
void togglePlayer(char *player) {
    *player = (*player == 'X') ? 'O' : 'X';
}

