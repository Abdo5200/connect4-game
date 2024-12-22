#include "ADC.h"
void Adc_init()
{
    /* Enable Clock to ADC0 and GPIO pins*/
    SYSCTL_RCGCGPIO_R |= (1<<4);   /* Enable Clock to GPIOE or PE3/AN0 */
    SYSCTL_RCGCADC_R  |= (1<<0);    /* AD0 clock enable*/
    
    /* initialize PE3 for \AIN0 input  */
    GPIO_EnableAlternateFunction(GPIO_PORTF, 0x08);
    GPIO_DisableDigital(GPIO_PORTF, 0x08);        /* disable digital function */
    GPIO_SetAnalogFunction(GPIO_PORTF, 0x08);       // Disable analog function
   
    /* initialize sample sequencer3 */
   ADC0_ACTSS_R &= ~(1<<3);        /* disable SS3 during configuration */
    ADC0_EMUX_R &= ~0xF000;    /* software trigger conversion */
    ADC0_SSMUX3_R  = 0;         /* get input from channel 0 */
    ADC0_SSCTL3_R  |= (1<<1)|(1<<2);        /* take one sample at a time, set flag at 1st sample */
   
	  /* Enable ADC Interrupt */
		ADC0_IM_R |= (1<<3); /* Unmask ADC0 sequence 3 interrupt*/
	NVIC_EN0_R |= (1<<17); /* enable IRQ17 for ADC0SS3*/
    ADC0_ACTSS_R |= (1<<3);         /* enable ADC0 sequencer 3 */
	 ADC0_PSSI_R |= (1<<3);        /* Enable SS3 conversion or start sampling data from AN0 */
	EnableInterrupts();           // (i) Clears the I bit
}