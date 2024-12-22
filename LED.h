#ifndef LED
#define LED
#include "GPIO.h"
//****************** LED ****************************

void LED_Init(void);
GPIO_Status LED_On(GPIO_Port port,GPIO_Pin pin);
GPIO_Status LED_Off(GPIO_Port port, GPIO_Pin pin);


#endif
