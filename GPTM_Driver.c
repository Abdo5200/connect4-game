#include "GPTM_Driver.h"
#include <math.h>
#include <stdlib.h>
int k = 0;

void Timer0_Number_Option(char option[10])//To select timer option from 16, 32, or RTC
{
    if ((strcmp(option, "16")) == 0)
        TIMER0_CFG_R = 0x4;//16 bits option
    else if ((strcmp(option, "32")) == 0)
        TIMER0_CFG_R = 0x0;//32 bits option
}

void Timer0_Number_Mode(char mode[10])//To select timer option from one shot, periodic, or capture
{
    if ((strcmp(mode, "one_shot")) == 0)
        TIMER0_TAMR_R |= 0x1;//One Shot mode
    else if ((strcmp(mode, "periodic")) == 0)
        TIMER0_TAMR_R |= 0x2;//Periodic mode
    else if ((strcmp(mode, "capture")) == 0)
        TIMER0_TAMR_R |= 0x3;//Capture mode
}


void Timer0_Number_Ticks(int ticks)//To load Number of ticks
{

    TIMER0_TAILR_R = ticks - 1;
    for (k = 0; k < 500; k++) {
        __asm__("nop");
    }//Load Ticks
    TIMER0_ICR_R = 1;
    for (k = 0; k < 500; k++) {
        __asm__("nop");
    }//clear timer flag
}

void GPTM0_Delay_Unit(int Counts)//Delay using GPTM  Timer
{
    Timer0_Number_Ticks(2000);//ticks for one milliseconds
    TIMER0_CTL_R |= 1UL << 0;
    for (k = 0; k < Counts; k++) {
        while ((TIMER0_RIS_R & 0x1) == 0);//Number of milliseconds
        TIMER0_ICR_R = 0x01;
    }
}

void TIMER0_INIT(int Counts, char mode[10], char option[10]) {
    SYSCTL_RCGCTIMER_R |= (1UL << 0);
    for (k = 0; k < 4; k++) {
        __asm__("nop");
    }
    TIMER0_CTL_R &= ~(1UL << 0); //Disable Timer
   
		Timer0_Number_Option(option);//32 bits Option Timer
    Timer0_Number_Mode(mode);    //Periodic Mode For Timer Module 1

    GPTM0_Delay_Unit(Counts);

    TIMER0_CTL_R &= ~(1UL << 0);//Disable Timer
}
