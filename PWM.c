#include "PWM.h"
void PWM_init()
{/* Clock setting for PWM and GPIO PORT */
    SYSCTL_RCGCPWM_R |= 2;        /* Enable clock to PWM1 module */
    SYSCTL_RCGCGPIO_R |= 0x20;    /* Enable system clock to PORTF */
    SYSCTL_RCC_R &= ~0x00100000;  /* Directly feed clock to PWM1 module without pre-divider */
    /* Fixed Duty Cycle PWM (10 kHz, 50% duty cycle) */
    PWM1_3_CTL_R &= ~(1 << 0);    /* Disable Generator 3 counter */
    PWM1_3_CTL_R &= ~(1 << 1);    /* Select down count mode of counter 3 */
    PWM1_3_GENA_R = 0x0000008C;   /* Set PWM output when counter reloaded and clear when matches PWMCMPA */
    PWM1_3_LOAD_R = 16000;        /* Set load value for 10 kHz (16 MHz / 16000) */
    PWM1_3_CMPA_R = 8000 - 1;     /* Set duty cycle to 50% (16000 / 2 = 8000) */
    PWM1_3_CTL_R = 1;             /* Enable Generator 3 counter */
    PWM1_ENABLE_R = 0x40;         /* Enable PWM1 channel 6 output */

    /* Variable Duty Cycle PWM (50 Hz, LED Fading) */
     SYSCTL_RCC_R |= (1 << 20);    /* Enable System Clock Divisor function */
     SYSCTL_RCC_R |= 0x000E0000;   /* Use pre-divider value of 64, then feed clock to PWM1 module */
    /* Setting of PF2 pin for M1PWM6 channel output pin */
     GPIO_EnableAlternateFunction(GPIO_PORTF, 0x04); // Disable alternate function
     GPIO_ClearPinControl(GPIO_PORTF, 0x00000F00);         /* Set PF2 as output pin */
     GPIO_SetPinControl(GPIO_PORTF, 0x00000500);    /* Make PF2 PWM output pin */
     GPIO_EnableDigital(GPIO_PORTF, 0x04);     /* Set PF2 as a digital pin */
 
    /* PWM1 channel 6 setting */
    PWM1_3_CTL_R &= ~(1 << 0);    /* Disable Generator 3 counter */
    PWM1_3_CTL_R &= ~(1 << 1);    /* Select down count mode of counter 3 */
    PWM1_3_GENA_R = 0x0000008C;   /* Set PWM output when counter reloaded and clear when matches PWMCMPA */
    PWM1_3_LOAD_R = 5000;         /* Set load value for 50 Hz (16 MHz / 64 = 250 kHz and (250 kHz / 5000) = 50 Hz) */
    PWM1_3_CMPA_R = 4999;         /* Set duty cycle to minimum value (initially LED off) */
    PWM1_3_CTL_R = 1;             /* Enable Generator 3 counter */
    PWM1_ENABLE_R = 0x40;         /* Enable PWM1 channel 6 output */
}