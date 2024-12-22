#ifndef Game
#define Game

#include "Nokia5110.h"
#include "GPTM_Driver.h"


#define BOARD_ROWS 6
#define BOARD_COLS 7

extern volatile int col;
extern volatile int g;
extern volatile int sw1_pressed ;
extern volatile int sw2_pressed ;

extern volatile int adc_val ;
void Adc_init(void);

void printBoard(char *board);
int takeTurn(char *board, int player, const char*);
int hasEmptyCol(char *board, int col  ); 
int changeBoard(char *board,int player , const char *PIECES , int col );
int checkWin(char *board);
int checkFour(char *board, int, int, int, int);
int horizontalCheck(char *board);
int verticalCheck(char *board);
int diagonalCheck(char *board);
void startingScreen(void);



#endif

