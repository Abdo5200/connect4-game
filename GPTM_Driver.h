#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



#ifndef GPTM_Driver_h
#define GPTM_Driver_h


extern void Timer0_Number_Option(char option[10]);                                //To select timer option from 16, 32, or RTC
extern void Timer0_Number_Mode(char mode[10]);                                    //To select timer option from one shot, periodic, or capture
extern void Timer0_Number_Ticks(int ticks);                                       //To load Number of ticks
extern void GPTM0_Delay_Unit(int Counts);                           //Delay using GPTM  Timer
extern void TIMER0_INIT(int Counts,char mode[10],char option[10]);  //To use Timer with delay
#endif
