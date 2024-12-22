#include "GPIO.h"


void GPIO_Init(void)
{
    GPIO_EnablePortClock(GPIO_PORTF);   // Enable Port F clock
    GPIO_UnlockPort(GPIO_PORTF);        // Unlock PortF PF0
    //GPIO_AllowChanges(GPIO_PORTF, 0x1F); // Allow changes to PF4-0
    GPIO_AllowChanges(GPIO_PORTF, GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);
    GPIO_SetAnalogFunction(GPIO_PORTF, 0x00);       // Disable analog function
    GPIO_ClearPinControl(GPIO_PORTF, 0x1F);          // Clear GPIO PCTL
    
    GPIO_SetPinDirection(GPIO_PORTF, GPIO_PIN_4,INPUT); // PF4 input
    GPIO_SetPinDirection(GPIO_PORTF, GPIO_PIN_0, INPUT); // PF0 input
    GPIO_SetPinDirection(GPIO_PORTF, GPIO_PIN_3, OUTPUT); // PF3 output
    GPIO_SetPinDirection(GPIO_PORTF, GPIO_PIN_2, OUTPUT); // PF2 output
    GPIO_SetPinDirection(GPIO_PORTF, GPIO_PIN_1, OUTPUT); // PF1 output

    GPIO_DisableAlternateFunction(GPIO_PORTF, 0x00); // Disable alternate function
    GPIO_EnablePullUp(GPIO_PORTF, GPIO_PIN_4);       // Enable pull-up resistor on PF4
    GPIO_EnablePullUp(GPIO_PORTF, GPIO_PIN_0);       // Enable pull-up resistor on PF0
    GPIO_EnableDigital(GPIO_PORTF, 0x1F);            // Enable digital pins PF4-PF0
    // Configure interrupts for PF4 (SW1) and PF0 (SW2)
    GPIO_SetInterruptSense(GPIO_PORTF, GPIO_PIN_4, GPIO_EDGE_SENSITIVE); // Edge-sensitive interrupt
    GPIO_SetInterruptSense(GPIO_PORTF, GPIO_PIN_0, GPIO_EDGE_SENSITIVE); // Edge-sensitive interrupt
    GPIO_SetInterruptBothEdges(GPIO_PORTF, GPIO_PIN_4, GPIO_DISABLE_BOTH_EDGES); // Not both edges trigger
    GPIO_SetInterruptBothEdges(GPIO_PORTF, GPIO_PIN_0, GPIO_DISABLE_BOTH_EDGES); // Not both edges trigger
    GPIO_SetInterruptEvent(GPIO_PORTF, GPIO_PIN_4, GPIO_FALLING_EDGE_EVENT); // Falling edge event
    GPIO_SetInterruptEvent(GPIO_PORTF, GPIO_PIN_0, GPIO_FALLING_EDGE_EVENT); // Falling edge event
    GPIO_ClearInterruptFlags(GPIO_PORTF, GPIO_PIN_4); // Clear interrupt flag for PF4
    GPIO_ClearInterruptFlags(GPIO_PORTF, GPIO_PIN_0); // Clear interrupt flag for PF0
    GPIO_EnableInterrupt(GPIO_PORTF, GPIO_PIN_4);    // Enable interrupt for PF4 (SW1)
    GPIO_EnableInterrupt(GPIO_PORTF, GPIO_PIN_0);    // Enable interrupt for PF0
    // Set interrupt priority to 5
    GPIO_SetInterruptPriority(5);
    // Enable interrupt 30 in NVIC (IRQ = 30)
   GPIO_EnableInterruptInNVIC();
}

//add the magic number for specific port 
GPIO_Status GPIO_EnablePortClock(GPIO_Port port)
{
    volatile unsigned long delay;
    volatile unsigned long* sysctlRCGCGPIOReg;
    volatile unsigned long* sysctlRCGC2Reg;

    switch (port) {
        case GPIO_PORTA:
            sysctlRCGCGPIOReg = &SYSCTL_RCGCGPIO_R;
            sysctlRCGC2Reg = &SYSCTL_RCGC2_R;
            break;
        case GPIO_PORTB:
            sysctlRCGCGPIOReg = &SYSCTL_RCGCGPIO_R;
            sysctlRCGC2Reg = &SYSCTL_RCGC2_R;
            break;
        case GPIO_PORTC:
            sysctlRCGCGPIOReg = &SYSCTL_RCGCGPIO_R;
            sysctlRCGC2Reg = &SYSCTL_RCGC2_R;
            break;
        case GPIO_PORTD:
            sysctlRCGCGPIOReg = &SYSCTL_RCGCGPIO_R;
            sysctlRCGC2Reg = &SYSCTL_RCGC2_R;
            break;
        case GPIO_PORTE:
            sysctlRCGCGPIOReg = &SYSCTL_RCGCGPIO_R;
            sysctlRCGC2Reg = &SYSCTL_RCGC2_R;
            break;
        case GPIO_PORTF:
            sysctlRCGCGPIOReg = &SYSCTL_RCGCGPIO_R;
            sysctlRCGC2Reg = &SYSCTL_RCGC2_R;
            break;
        default:
            return GPIO_INVALID_PORT; // Invalid port
    }

    *sysctlRCGCGPIOReg |= (1 << port);  // Enable the clock for the specified port
    delay = *sysctlRCGCGPIOReg;         // Delay
    *sysctlRCGC2Reg |= (1 << port);     // Enable the corresponding bit in RCGC2
    delay = *sysctlRCGC2Reg;            // Delay to make sure of the connection 
    return GPIO_SUCCESS; // Clock enabled successfully
}



//add the magic number for specific port 
GPIO_Status GPIO_UnlockPort(GPIO_Port port)
{
    volatile unsigned long* gpioPortLockReg;
    unsigned long unlockValue = 0x4C4F434B; // Common unlock value for all ports

    switch (port) {
        case GPIO_PORTA:
            gpioPortLockReg = &GPIO_PORTA_LOCK_R;
            break;
        case GPIO_PORTB:
            gpioPortLockReg = &GPIO_PORTB_LOCK_R;
            break;
        case GPIO_PORTC:
            gpioPortLockReg = &GPIO_PORTC_LOCK_R;
            break;
        case GPIO_PORTD:
            gpioPortLockReg = &GPIO_PORTD_LOCK_R;
            break;
        case GPIO_PORTE:
            gpioPortLockReg = &GPIO_PORTE_LOCK_R;
            break;
        case GPIO_PORTF:
            gpioPortLockReg = &GPIO_PORTF_LOCK_R;
            break;
        default:
            return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioPortLockReg = unlockValue; // Unlock the specified port

    if ((*gpioPortLockReg & 0x1F) != 0) {
        return GPIO_INVALID_PORT; // Error unlocking port
    }

    return GPIO_SUCCESS; // Port unlocked successfully
}

// ta2ked 3ala el lock by3mel access to certain pins in a port  
GPIO_Status GPIO_AllowChanges(GPIO_Port port, uint8_t pins)
{
    volatile unsigned long* gpioPortCRReg;

    switch (port) {
        case GPIO_PORTA:
            gpioPortCRReg = &GPIO_PORTA_CR_R;
            break;
        case GPIO_PORTB:
            gpioPortCRReg = &GPIO_PORTB_CR_R;
            break;
        case GPIO_PORTC:
            gpioPortCRReg = &GPIO_PORTC_CR_R;
            break;
        case GPIO_PORTD:
            gpioPortCRReg = &GPIO_PORTD_CR_R;
            break;
        case GPIO_PORTE:
            gpioPortCRReg = &GPIO_PORTE_CR_R;
            break;
        case GPIO_PORTF:
            gpioPortCRReg = &GPIO_PORTF_CR_R;
            break;
        default:
            return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioPortCRReg = pins; // Allow changes to specified pins

    return GPIO_SUCCESS; // Changes allowed successfully
}


GPIO_Status GPIO_SetAnalogFunction(GPIO_Port port, uint8_t pins)
{
    volatile unsigned long *gpioAMSELReg;

    switch (port) {
        case GPIO_PORTA:
            gpioAMSELReg = &GPIO_PORTA_AMSEL_R;
            break;
        case GPIO_PORTB:
            gpioAMSELReg = &GPIO_PORTB_AMSEL_R;
            break;
        case GPIO_PORTC:
            gpioAMSELReg = &GPIO_PORTC_AMSEL_R;
            break;
        case GPIO_PORTD:
            gpioAMSELReg = &GPIO_PORTD_AMSEL_R;
            break;
        case GPIO_PORTE:
            gpioAMSELReg = &GPIO_PORTE_AMSEL_R;
            break;
        case GPIO_PORTF:
            gpioAMSELReg = &GPIO_PORTF_AMSEL_R;
            break;
        default:
           return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioAMSELReg = pins; // Set analog function for specified pins
    return GPIO_SUCCESS; // Analog function set successfully
}

GPIO_Status GPIO_ClearPinControl(GPIO_Port port, uint32_t pins)
{
    volatile unsigned long *gpioPCTLReg;

    switch (port) { 
        case GPIO_PORTA:
            gpioPCTLReg = &GPIO_PORTA_PCTL_R;
            break;
        case GPIO_PORTB:
            gpioPCTLReg = &GPIO_PORTB_PCTL_R;
            break;
        case GPIO_PORTC:
            gpioPCTLReg = &GPIO_PORTC_PCTL_R;
            break;
        case GPIO_PORTD:
            gpioPCTLReg = &GPIO_PORTD_PCTL_R;
            break;
        case GPIO_PORTE:
            gpioPCTLReg = &GPIO_PORTE_PCTL_R;
            break;
        case GPIO_PORTF:
            gpioPCTLReg = &GPIO_PORTF_PCTL_R;
            break;
        default:
            return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioPCTLReg &= ~pins; // Clear pin control for specified pins
    return GPIO_SUCCESS; // Pin control cleared successfully
}
GPIO_Status GPIO_SetPinControl(GPIO_Port port, uint32_t pins)
{
    volatile unsigned long *gpioPCTLReg;

    switch (port) {
        case GPIO_PORTA:
            gpioPCTLReg = &GPIO_PORTA_PCTL_R;
            break;
        case GPIO_PORTB:
            gpioPCTLReg = &GPIO_PORTB_PCTL_R;
            break;
        case GPIO_PORTC:
            gpioPCTLReg = &GPIO_PORTC_PCTL_R;
            break;
        case GPIO_PORTD:
            gpioPCTLReg = &GPIO_PORTD_PCTL_R;
            break;
        case GPIO_PORTE:
            gpioPCTLReg = &GPIO_PORTE_PCTL_R;
            break;
        case GPIO_PORTF:
            gpioPCTLReg = &GPIO_PORTF_PCTL_R;
            break;
        default:
            return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioPCTLReg = pins; // Clear pin control for specified pins
    return GPIO_SUCCESS; // Pin control cleared successfully
}

// output or input 
GPIO_Status GPIO_SetPinDirection(GPIO_Port port, GPIO_Pin pin, uint8_t direction)
{
    volatile unsigned long* gpioDirReg;

    
    switch (port) {
        case GPIO_PORTA:
            gpioDirReg = &GPIO_PORTA_DIR_R;
            if ((pin < GPIO_PIN_0) || (pin > GPIO_PIN_7)) {
                return GPIO_INVALID_PORT; // Invalid pin for port A
            }
            break;
        case GPIO_PORTB:
            gpioDirReg = &GPIO_PORTB_DIR_R;
            if ((pin < GPIO_PIN_0) || (pin > GPIO_PIN_7)) {
                return GPIO_INVALID_PORT; // Invalid pin for port B
            }
            break;
        case GPIO_PORTC:
            gpioDirReg = &GPIO_PORTC_DIR_R;
            if ((pin < GPIO_PIN_0) || (pin > GPIO_PIN_7)) {
                return GPIO_INVALID_PORT; // Invalid pin for port C
            }
            break;
        case GPIO_PORTD:
            gpioDirReg = &GPIO_PORTD_DIR_R;
            if ((pin < GPIO_PIN_0) || (pin > GPIO_PIN_7)) {
                return GPIO_INVALID_PORT; // Invalid pin for port D
            }
            break;
        case GPIO_PORTE:
            gpioDirReg = &GPIO_PORTE_DIR_R;
            if ((pin < GPIO_PIN_0) || (pin > GPIO_PIN_7)) {
                return GPIO_INVALID_PORT; // Invalid pin for port E
            }
            break;
        case GPIO_PORTF:
            gpioDirReg = &GPIO_PORTF_DIR_R;
            if ((pin < GPIO_PIN_0) || (pin > GPIO_PIN_4)) {
                return GPIO_INVALID_PORT; // Invalid pin for port F
            }
            break;
				
        default:
            return GPIO_INVALID_PORT; // Invalid port

			}

    if (direction == 0) {
        *gpioDirReg &= ~pin; // Set pin as input
    } else {
        *gpioDirReg |= pin; // Set pin as output
    }

    return GPIO_SUCCESS; // Pin direction set successfully
}


GPIO_Status GPIO_DisableAlternateFunction(GPIO_Port port, uint8_t pins)
{
    volatile unsigned long *gpioAFSELReg;

    switch (port) {
        case GPIO_PORTA:
            gpioAFSELReg = &GPIO_PORTA_AFSEL_R;
            break;
        case GPIO_PORTB:
            gpioAFSELReg = &GPIO_PORTB_AFSEL_R;
            break;
        case GPIO_PORTC:
            gpioAFSELReg = &GPIO_PORTC_AFSEL_R;
            break;
        case GPIO_PORTD:
            gpioAFSELReg = &GPIO_PORTD_AFSEL_R;
            break;
        case GPIO_PORTE:
            gpioAFSELReg = &GPIO_PORTE_AFSEL_R;
            break;
        case GPIO_PORTF:
            gpioAFSELReg = &GPIO_PORTF_AFSEL_R;
            break;
        default:
           return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioAFSELReg &= ~pins; // Disable alternate function for specified pins
    return GPIO_SUCCESS; 
}
GPIO_Status GPIO_EnableAlternateFunction(GPIO_Port port, uint32_t pins)
{
    volatile unsigned long *gpioAFSELReg;

    switch (port) {
        case GPIO_PORTA:
            gpioAFSELReg = &GPIO_PORTA_AFSEL_R;
            break;
        case GPIO_PORTB:
            gpioAFSELReg = &GPIO_PORTB_AFSEL_R;
            break;
        case GPIO_PORTC:
            gpioAFSELReg = &GPIO_PORTC_AFSEL_R;
            break;
        case GPIO_PORTD:
            gpioAFSELReg = &GPIO_PORTD_AFSEL_R;
            break;
        case GPIO_PORTE:
            gpioAFSELReg = &GPIO_PORTE_AFSEL_R;
            break;
        case GPIO_PORTF:
            gpioAFSELReg = &GPIO_PORTF_AFSEL_R;
            break;
        default:
           return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioAFSELReg |= pins; // Disable alternate function for specified pins
    return GPIO_SUCCESS; 
}

GPIO_Status GPIO_EnablePullUp(GPIO_Port port, GPIO_Pin pin)
{
    volatile unsigned long *gpioPURReg;

    switch (port) {
        case GPIO_PORTA:
            gpioPURReg = &GPIO_PORTA_PUR_R;
            break;
        case GPIO_PORTB:
            gpioPURReg = &GPIO_PORTB_PUR_R;
            break;
        case GPIO_PORTC:
            gpioPURReg = &GPIO_PORTC_PUR_R;
            break;
        case GPIO_PORTD:
            gpioPURReg = &GPIO_PORTD_PUR_R;
            break;
        case GPIO_PORTE:
            gpioPURReg = &GPIO_PORTE_PUR_R;
            break;
        case GPIO_PORTF:
            gpioPURReg = &GPIO_PORTF_PUR_R;
            break;
        default:
            return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioPURReg |= pin; // Enable pull-up resistor for specified pin
     return GPIO_SUCCESS; 
}

GPIO_Status GPIO_EnableDigital(GPIO_Port port, uint8_t pins)
{
    volatile unsigned long *gpioDENReg;

    switch (port) {
        case GPIO_PORTA:
            gpioDENReg = &GPIO_PORTA_DEN_R;
            break;
        case GPIO_PORTB:
            gpioDENReg = &GPIO_PORTB_DEN_R;
            break;
        case GPIO_PORTC:
            gpioDENReg = &GPIO_PORTC_DEN_R;
            break;
        case GPIO_PORTD:
            gpioDENReg = &GPIO_PORTD_DEN_R;
            break;
        case GPIO_PORTE:
            gpioDENReg = &GPIO_PORTE_DEN_R;
            break;
        case GPIO_PORTF:
            gpioDENReg = &GPIO_PORTF_DEN_R;
            break;
        default:
             return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioDENReg |= pins; // Enable digital pins for specified pins
     return GPIO_SUCCESS; 
}
GPIO_Status GPIO_DisableDigital(GPIO_Port port, uint8_t pins)
{
    volatile unsigned long *gpioDENReg;

    switch (port) {
        case GPIO_PORTA:
            gpioDENReg = &GPIO_PORTA_DEN_R;
            break;
        case GPIO_PORTB:
            gpioDENReg = &GPIO_PORTB_DEN_R;
            break;
        case GPIO_PORTC:
            gpioDENReg = &GPIO_PORTC_DEN_R;
            break;
        case GPIO_PORTD:
            gpioDENReg = &GPIO_PORTD_DEN_R;
            break;
        case GPIO_PORTE:
            gpioDENReg = &GPIO_PORTE_DEN_R;
            break;
        case GPIO_PORTF:
            gpioDENReg = &GPIO_PORTF_DEN_R;
            break;
        default:
             return GPIO_INVALID_PORT; // Invalid port
    }

    *gpioDENReg &=~ pins; // Enable digital pins for specified pins
     return GPIO_SUCCESS; 
}

//**************** INTERRUPT ***************************************************************
void GPIO_SetInterruptSense(GPIO_Port port, GPIO_Pin pin, GPIO_InterruptSense sense)
{
    volatile unsigned long *gpioISReg;

    switch (port) {
        case GPIO_PORTA:
            gpioISReg = &GPIO_PORTA_IS_R;
            break;
        case GPIO_PORTB:
            gpioISReg = &GPIO_PORTB_IS_R;
            break;
        case GPIO_PORTC:
            gpioISReg = &GPIO_PORTC_IS_R;
            break;
        case GPIO_PORTD:
            gpioISReg = &GPIO_PORTD_IS_R;
            break;
        case GPIO_PORTE:
            gpioISReg = &GPIO_PORTE_IS_R;
            break;
        case GPIO_PORTF:
            gpioISReg = &GPIO_PORTF_IS_R;
            break;
        default:
            return; // Invalid port
    }

    if (sense == GPIO_LEVEL_SENSITIVE) {
        *gpioISReg |= pin; // Set level-sensitive interrupt for specified pin
    } else {
        *gpioISReg &= ~pin; // Set edge-sensitive interrupt for specified pin
    }
}

void GPIO_SetInterruptBothEdges(GPIO_Port port, GPIO_Pin pin, GPIO_InterruptBothEdges edges)
{
    volatile unsigned long *gpioIBEReg;

    switch (port) {
        case GPIO_PORTA:
            gpioIBEReg = &GPIO_PORTA_IBE_R;
            break;
        case GPIO_PORTB:
            gpioIBEReg = &GPIO_PORTB_IBE_R;
            break;
        case GPIO_PORTC:
            gpioIBEReg = &GPIO_PORTC_IBE_R;
            break;
        case GPIO_PORTD:
            gpioIBEReg = &GPIO_PORTD_IBE_R;
            break;
        case GPIO_PORTE:
            gpioIBEReg = &GPIO_PORTE_IBE_R;
            break;
        case GPIO_PORTF:
            gpioIBEReg = &GPIO_PORTF_IBE_R;
            break;
        default:
            return; // Invalid port
    }

    if (edges == GPIO_ENABLE_BOTH_EDGES) {
        *gpioIBEReg |= pin; // Enable both edges trigger for specified pin
    } else {
        *gpioIBEReg &= ~pin; // Disable both edges trigger for specified pin
    }
}

void GPIO_SetInterruptEvent(GPIO_Port port, GPIO_Pin pin, GPIO_InterruptEvent event)
{
    volatile unsigned long *gpioIEVReg;

    switch (port) {
        case GPIO_PORTA:
            gpioIEVReg = &GPIO_PORTA_IEV_R;
            break;
        case GPIO_PORTB:
            gpioIEVReg = &GPIO_PORTB_IEV_R;
            break;
        case GPIO_PORTC:
            gpioIEVReg = &GPIO_PORTC_IEV_R;
            break;
        case GPIO_PORTD:
            gpioIEVReg = &GPIO_PORTD_IEV_R;
            break;
        case GPIO_PORTE:
            gpioIEVReg = &GPIO_PORTE_IEV_R;
            break;
        case GPIO_PORTF:
            gpioIEVReg = &GPIO_PORTF_IEV_R;
            break;
        default:
            return; // Invalid port
    }

    if (event == GPIO_RISING_EDGE_EVENT) {
        *gpioIEVReg |= pin; // Set rising edge trigger for specified pin
    } else {
        *gpioIEVReg &= ~pin; // Set falling edge trigger for specified pin
    }
}

void GPIO_ClearInterruptFlags(GPIO_Port port, GPIO_Pin pin)
{
    volatile unsigned long *gpioICRReg;

    switch (port) {
        case GPIO_PORTA:
            gpioICRReg = &GPIO_PORTA_ICR_R;
            break;
        case GPIO_PORTB:
            gpioICRReg = &GPIO_PORTB_ICR_R;
            break;
        case GPIO_PORTC:
            gpioICRReg = &GPIO_PORTC_ICR_R;
            break;
        case GPIO_PORTD:
            gpioICRReg = &GPIO_PORTD_ICR_R;
            break;
        case GPIO_PORTE:
            gpioICRReg = &GPIO_PORTE_ICR_R;
            break;
        case GPIO_PORTF:
            gpioICRReg = &GPIO_PORTF_ICR_R;
            break;
        default:
            return; // Invalid port
    }

    *gpioICRReg = pin; // Clear interrupt flags for specified pin
}

void GPIO_EnableInterrupt(GPIO_Port port, GPIO_Pin pin)
{
    volatile unsigned long *gpioIMReg;

    switch (port) {
        case GPIO_PORTA:
            gpioIMReg = &GPIO_PORTA_IM_R;
            break;
        case GPIO_PORTB:
            gpioIMReg = &GPIO_PORTB_IM_R;
            break;
        case GPIO_PORTC:
            gpioIMReg = &GPIO_PORTC_IM_R;
            break;
        case GPIO_PORTD:
            gpioIMReg = &GPIO_PORTD_IM_R;
            break;
        case GPIO_PORTE:
            gpioIMReg = &GPIO_PORTE_IM_R;
            break;
        case GPIO_PORTF:
            gpioIMReg = &GPIO_PORTF_IM_R;
            break;
        default:
            return; // Invalid port
    }

    *gpioIMReg |= pin; // Enable interrupt for specified pin
}

void GPIO_SetInterruptPriority(uint8_t priority)
{
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | (priority << 21); // Set interrupt priority
}

void GPIO_EnableInterruptInNVIC()
{
    NVIC_EN0_R |= 0x40000000; // Enable interrupt in NVIC (IRQ = 30)
}

