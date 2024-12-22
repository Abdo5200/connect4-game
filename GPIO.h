#ifndef GPIO_H
#define GPIO_H
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#define INPUT 0
#define OUTPUT 1
typedef enum {
   GPIO_INVALID_PORT= 0,
    GPIO_SUCCESS = 1
} GPIO_Status;
typedef enum {
    GPIO_PORTA = 0,
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD,
    GPIO_PORTE,
    GPIO_PORTF
} GPIO_Port;

typedef enum {
    GPIO_PIN_0 = 0x01,
    GPIO_PIN_1 = 0x02,
    GPIO_PIN_2 = 0x04,
    GPIO_PIN_3 = 0x08,
    GPIO_PIN_4 = 0x10,
    GPIO_PIN_5 = 0x20,
    GPIO_PIN_6 = 0x40,
    GPIO_PIN_7 = 0x80
} GPIO_Pin;




typedef enum {
    GPIO_DISABLE_BOTH_EDGES = 0,
    GPIO_ENABLE_BOTH_EDGES = 1
} GPIO_InterruptBothEdges;

typedef enum {
    GPIO_EDGE_SENSITIVE = 0,
    GPIO_LEVEL_SENSITIVE = 1
} GPIO_InterruptSense;

typedef enum {
    GPIO_FALLING_EDGE_EVENT = 0,
    GPIO_RISING_EDGE_EVENT = 1
} GPIO_InterruptEvent;

void GPIO_Init(void);
GPIO_Status GPIO_EnablePortClock(GPIO_Port port);
GPIO_Status GPIO_UnlockPort(GPIO_Port port);
GPIO_Status GPIO_AllowChanges(GPIO_Port port, uint8_t pins);
GPIO_Status GPIO_SetAnalogFunction(GPIO_Port port, uint8_t pins);
GPIO_Status GPIO_ClearPinControl(GPIO_Port port, uint32_t pins);
GPIO_Status GPIO_SetPinDirection(GPIO_Port port, GPIO_Pin pin, uint8_t direction);
GPIO_Status GPIO_DisableAlternateFunction(GPIO_Port port, uint8_t pins);
GPIO_Status GPIO_EnablePullUp(GPIO_Port port, GPIO_Pin pin);
GPIO_Status GPIO_EnableDigital(GPIO_Port port, uint8_t pins);
GPIO_Status GPIO_SetPinControl(GPIO_Port port, uint32_t pins);
GPIO_Status GPIO_EnableAlternateFunction(GPIO_Port port, uint32_t pins);
GPIO_Status GPIO_DisableDigital(GPIO_Port port, uint8_t pins);
//for the interrupt handling
GPIO_Status GPIO_CheckInterruptStatus(GPIO_Port port, GPIO_Pin pin);
void GPIO_SetInterruptSense(GPIO_Port port, GPIO_Pin pin, GPIO_InterruptSense sense);
void GPIO_SetInterruptBothEdges(GPIO_Port port, GPIO_Pin pin, GPIO_InterruptBothEdges edges);
void GPIO_SetInterruptEvent(GPIO_Port port, GPIO_Pin pin, GPIO_InterruptEvent event);
void GPIO_ClearInterruptFlags(GPIO_Port port, GPIO_Pin pin);
void GPIO_EnableInterrupt(GPIO_Port port, GPIO_Pin pin);
void GPIO_SetInterruptPriority(uint8_t priority);
void GPIO_EnableInterruptInNVIC(void);
void PWM_init(void);
#endif /* GPIO_H */
