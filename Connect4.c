
// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total


#include "ADC.h"
#include "gg.h"
#include "LED.h"
#include "PWM.h"
#include "TExaS.h"
#include "game.h"
#include <stdlib.h>
//#include "oled_i2c.h"
int i    = 0;
int turn = 0, done = 0;
const char *PIECES = "XO";
char board1[(BOARD_ROWS * BOARD_COLS) + 1];

int main(void) {
		
    int duty_cycle = 4999;
    GPIO_Init();
    Adc_init();// Call initialization of port PF4, PF3, PF2, PF1, PF0
I2C3_Init();
	   OLED_Init();
	   OLED_Clear();
    while (1) {
        for (i = 0; i < BOARD_COLS * BOARD_ROWS + 1; i++) {
            board1[i] = ' ';
        }
     
	
        Nokia5110_Init();
        Nokia5110_Clear();
        startingScreen();
        Nokia5110_Clear();
        printBoard(board1);

        for (turn = 0; turn < (BOARD_ROWS * BOARD_COLS) && !done;) {// 42


            do {
                printBoard(board1);
							
            } while (!takeTurn(board1, (turn) % 2, PIECES));
            done = checkWin(board1);
            turn++;
            if (done) {
                LED_Init();
                
                LED_On(GPIO_PORTB, GPIO_PIN_2);
                break;
            }
            do {
                printBoard(board1);
            } while (!takeTurn(board1, (turn) % 2, PIECES));
            done = checkWin(board1);
            turn++;
            if (done) {
                LED_Init();
                LED_On(GPIO_PORTB, GPIO_PIN_5);
                break;
            }
        }
        printBoard(board1);
        if (turn == BOARD_ROWS * BOARD_COLS && !done) {
            Nokia5110_OutString("It's a tie!");
        } else {
            turn--;
            Nokia5110_Clear();
            Nokia5110_SetCursor(1, 1);
            Nokia5110_OutString("Player");
            Nokia5110_SetCursor(8, 1);
            Nokia5110_OutString(turn % 2 == 0 ? "X" : "O");
            Nokia5110_SetCursor(3, 2);
            Nokia5110_OutString("wins!");
            Nokia5110_SetCursor(1, 4);
            Nokia5110_OutString("GAME OVER");
        }
        PWM_init();
        while (!sw1_pressed) {
					    

					OLED_YX(3,1);
					OLED_Write_String("RESTART!!");
            duty_cycle = duty_cycle - 10; /* Decrease duty cycle */
            if (duty_cycle <= 0) {
                duty_cycle = 5000; /* Reset duty cycle to maximum */
            }
            PWM1_3_CMPA_R = duty_cycle; /* Update the duty cycle */
          //  TIMER0_INIT(1, "periodic", "32");
        }
        sw1_pressed = 0;
    done=0;
				OLED_Clear();
				LED_Off(GPIO_PORTB, GPIO_PIN_2);
				LED_Off(GPIO_PORTB, GPIO_PIN_5);

		}
}

void GPIOPortF_Handler(void) {
    if (GPIO_PORTF_RIS_R & GPIO_PIN_4) {
        // Handle SW1 interrupt
        GPIO_PORTF_ICR_R = GPIO_PIN_4;
        sw1_pressed      = 1;
        TIMER0_INIT(500, "periodic", "32");
    }

    if (GPIO_PORTF_RIS_R & GPIO_PIN_0) {
        // Handle SW2 interrupt
        GPIO_PORTF_ICR_R = GPIO_PIN_0;
        sw2_pressed      = 1;
        TIMER0_INIT(500, "periodic", "32");
    }
}
void ADC0Seq3_Handler(void) {

    adc_val    = ADC0_SSFIFO3_R; /* read adc coversion result from SS3 FIFO*/
    ADC0_ISC_R = 8;              /* clear coversion clear flag bit*/
    ADC0_PSSI_R |= (1 << 3);     /* Enable SS3 conversion or start sampling data from AN0 data from AN0 */
}
