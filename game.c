#include "Game.h"
volatile int col = 0;
volatile int g;
volatile int sw1_pressed = 0;
volatile int sw2_pressed = 0;
volatile int adc_val = 0;
void EnableInterrupts(); 


void startingScreen()
{
	int w; 
	Nokia5110_SetCursor(0,0);
	Nokia5110_ClearBuffer();
	
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2,2);
	Nokia5110_OutString("Welcome!  connect4");

		
	TIMER0_INIT( 100000,"periodic","32");
	for(w=0 ; w<7 ; w++)
	{
		Nokia5110_SetCursor(0,w);
		Nokia5110_OutString("           ");
	TIMER0_INIT( 500,"periodic","32");
	}

}
void printBoard(char *board){
   int row, col;
	Nokia5110_ClearBuffer();
   for(row = 0; row < BOARD_ROWS; row++){
      for(col = 0; col < BOARD_COLS; col++){
				Nokia5110_SetCursorChar( col ,row,board[BOARD_COLS*row+col]);
      }
   }
}

int changeBoard(char *board,int player , const char *PIECES , int col  )
{
	int row ;
   for(row = BOARD_ROWS - 1; row >= 0; row--){
      if(board[BOARD_COLS * row + col] == ' '){
         board[BOARD_COLS * row + col] = PIECES[player];
         return 1;
      }
   }
	 return 0 ;
}

int hasEmptyCol(char *board, int col  )
{
	int row ;
   for(row = BOARD_ROWS - 1; row >= 0; row--){
      if(board[BOARD_COLS * row + col] == ' '){
         return 1;
      }
   }
	 return 0 ;
}
int checkWin(char *board){
    return (horizontalCheck(board) || verticalCheck(board) || diagonalCheck(board));
}

int checkFour(char *board, int a, int b, int c, int d){
    return (board[a] == board[b] && board[b] == board[c] && board[c] == board[d] && board[a] != ' ');
}

int horizontalCheck(char *board){
    int row, col, idx;
    const int WIDTH = 1;

    for(row = 0; row < BOARD_ROWS; row++){
       for(col = 0; col < BOARD_COLS - 3; col++){
          idx = BOARD_COLS * row + col;
          if(checkFour(board, idx, idx + WIDTH, idx + WIDTH * 2, idx + WIDTH * 3)){
             return 1;
          }
       }
    }
    return 0;

}
int verticalCheck(char *board){
    int row, col, idx;
    const int HEIGHT = 7;

    for(row = 0; row < BOARD_ROWS - 3; row++){
       for(col = 0; col < BOARD_COLS; col++){
          idx = BOARD_COLS * row + col;
          if(checkFour(board, idx, idx + HEIGHT, idx + HEIGHT * 2, idx + HEIGHT * 3)){
              return 1;
          }
       }
    }
    return 0;

}

int diagonalCheck(char *board){
   int row, col, idx, count = 0;
   const int DIAG_RGT = 6, DIAG_LFT = 8;

   for(row = 0; row < BOARD_ROWS - 3; row++){
      for(col = 0; col < BOARD_COLS; col++){
         idx = BOARD_COLS * row + col;
         if((count <= 3 && checkFour(board, idx, idx + DIAG_LFT, idx + DIAG_LFT * 2, idx + DIAG_LFT * 3)) || (count >= 3 && checkFour(board, idx, idx + DIAG_RGT, idx + DIAG_RGT * 2, idx + DIAG_RGT * 3))){
            return 1;
         }
         count++;
      }
      count = 0;
   }
   return 0;
}
int takeTurn(char *board, int player, const char *PIECES)
{int prev_col=0;
    // Reset variables
    col = 0;
    sw1_pressed = 0;
    sw2_pressed = 0;
    // Set initial cursor position and display character
    Nokia5110_SetCursorChar(col, 0, PIECES[player]);
    while (1) { 
// Clear previous character's position
			if(prev_col!=col){
            Nokia5110_SetCursorChar(prev_col, 0, ' ');
				prev_col=col;
			}
			
        if( adc_val >= 0 && adc_val <= 585)
            col = 0;
            
        else if( adc_val >= 586 && adc_val <= 1170)
            col = 1;
          
     else if (adc_val >= 1171 && adc_val <= 1755)
            col = 2;
            
       else if( adc_val >= 1756 && adc_val <= 2340)
            col = 3;
            
         else if ( adc_val >= 2341 && adc_val <= 2925)
            col = 4;
         
       else if( adc_val >= 2926 && adc_val <= 3510)
            col = 5;
           
         else if (adc_val >= 3511 && adc_val <= 4095)
            col = 6;
        
       

            // Update new character's position
            Nokia5110_SetCursorChar(col, 0, PIECES[player]);
        if (sw2_pressed) {
						TIMER0_INIT( 500,"periodic","32");
            sw2_pressed = 0;
            break;
        }       
    }
    return changeBoard(board, player, PIECES, col);
}



