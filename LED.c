#include "LED.h"



void LED_Init(void)
{
    GPIO_EnablePortClock(GPIO_PORTB);          // activate clock for Port B
	
    //TIMER_INIT( 2,40,"Seconds","periodic","32");                                                                     
	GPIO_UnlockPort(GPIO_PORTB);  // unlock GPIO of PORTB
  GPIO_EnableDigital(GPIO_PORTB,  GPIO_PIN_2| GPIO_PIN_5);   
	GPIO_SetPinDirection(GPIO_PORTB, GPIO_PIN_2, OUTPUT);
	GPIO_SetPinDirection(GPIO_PORTB, GPIO_PIN_5, OUTPUT);
	GPIO_SetAnalogFunction(GPIO_PORTB, 0x00);       // Disable analog function
	GPIO_DisableAlternateFunction(GPIO_PORTB, 0x00); // Disable alternate function
}


GPIO_Status LED_On(GPIO_Port port, GPIO_Pin pin){
		
	 switch (port) {
        case GPIO_PORTA:
            GPIO_PORTA_DATA_R|=pin;
            break;
        case GPIO_PORTB:
            GPIO_PORTB_DATA_R|=pin ;
            break;
        case GPIO_PORTC:
            GPIO_PORTC_DATA_R|=pin ;
            break;
        case GPIO_PORTD:
            GPIO_PORTD_DATA_R|=pin ;
            break;
        case GPIO_PORTE:
            GPIO_PORTE_DATA_R|=pin ;
            break;
        case GPIO_PORTF:
            GPIO_PORTF_DATA_R|=pin ;
            break;
        default:
            return GPIO_INVALID_PORT; // Invalid port
    }

	return  GPIO_SUCCESS;
}
GPIO_Status LED_Off(GPIO_Port port, GPIO_Pin pin){
		
	 switch (port) {
        case GPIO_PORTA:
            GPIO_PORTA_DATA_R&=~pin;
            break;
        case GPIO_PORTB:
            GPIO_PORTB_DATA_R&=~pin ;
            break;
        case GPIO_PORTC:
            GPIO_PORTC_DATA_R&=~pin ;
            break;
        case GPIO_PORTD:
            GPIO_PORTD_DATA_R&=~pin ;
            break;
        case GPIO_PORTE:
            GPIO_PORTE_DATA_R&=~pin ;
            break;
        case GPIO_PORTF:
            GPIO_PORTF_DATA_R&=~pin ;
            break;
        default:
            return GPIO_INVALID_PORT; // Invalid port
    }

	return  GPIO_SUCCESS;
}

