// oled_i2c.c
#include "oled_i2c.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
int result=0;


void OLED_Write_Float(float f)
{
    char* buf11;
   // int status;
    sprintf( buf11, "%f", f );
    OLED_Write_String(buf11);
    OLED_Write_String( "     " );
}

//int main(void)
//{ 
	
	/* int count = 0;
   // float dec = 0.0;
	 I2C3_Init();
	 OLED_Init();	
	 OLED_Clear();
    
    // variables for counting
        while ( 1 ) {
            
            /////////////////////
            // Strings
            ///////////////////
            
             OLED_YX( 0, 0 );
             OLED_Write_String( "OLED SSD1306" );
                             Delay_ms(1000);

             OLED_YX(1, 0);
             OLED_Write_String ("TM4C123");
                         Delay_ms(1000);
            
            /////////////////////
            // Integer Count
            ////////////////////
            
            for (count = 0; count <= 100; count++){
                OLED_YX(2, 0 );
                OLED_Write_String( "Count:" );
                OLED_YX(3, 2);
                OLED_Write_Integer(count);
                Delay_ms(100);
            }
            
            OLED_Clear();
            Delay_ms(100);
            
          
   }*/
	 
// }

// I2C intialization and GPIO alternate function configuration
void I2C3_Init ( void )
{
 SYSCTL_RCGCGPIO_R    |= 0x00000008 ; // Enable the clock for port D
SYSCTL_RCGCI2C_R     |= 0x00000008 ; // Enable the clock for I2C 3
GPIO_PORTD_DEN_R   |= 0x03; // Assert DEN for port D
// Configure Port D pins 0 and 1 as I2C 3
GPIO_PORTD_AFSEL_R  |= 0x00000003 ;
GPIO_PORTD_PCTL_R|= 0x00000033 ;
GPIO_PORTD_ODR_R |= 0x00000002 ; // SDA (PD1 ) pin as open darin
I2C3_MCR_R    = 0x0010 ; // Enable I2C 3 master function
/* Configure I2C 3 clock frequency
(1 + TIME_PERIOD ) = SYS_CLK /(2*
( SCL_LP + SCL_HP ) * I2C_CLK_Freq )
TIME_PERIOD = 16 ,000 ,000/(2(6+4) *100000) - 1 = 7 */
I2C3_MTPR_R   = 0x07 ;
}
/* wait untill I2C Master module is busy */
/*  and if not busy and no error return 0 */
static int I2C_wait_till_done(void)
{
    while(I2C3_MCS_R    & 1);   /* wait until I2C master is not busy */
    return I2C3_MCS_R     & 0xE; /* return I2C error code, 0 if no error*/
}
char I2C3_Wr(int slaveAddr, char memAddr, uint8_t data)
{

    char error;

    /* send slave address and starting address */
   I2C3_MSA_R    = slaveAddr << 1;
    I2C3_MDR_R    = memAddr;
    I2C3_MCS_R  = 3;                      /* S-(saddr+w)-ACK-maddr-ACK */

    error = I2C_wait_till_done();       /* wait until write is complete */
    if (error) return error;

    /* send data */
    I2C3_MDR_R    = data;
    I2C3_MCS_R  = 5;                      /* -data-ACK-P */
    error = I2C_wait_till_done();       /* wait until write is complete */
    while(I2C3_MCS_R  & 0x40);            /* wait until bus is not busy */
    error = I2C3_MCS_R  & 0xE;
    if (error) return error;

    return 0;       /* no error */
}
// Receive one byte of data from I2C slave device
char I2C3_Write_Multiple(int slave_address, char slave_memory_address, int bytes_count, uint8_t* data)
{   
    char error;
    if (bytes_count <= 0)
        return -1;                  /* no write was performed */
    /* send slave address and starting address */
    I2C3_MSA_R    = slave_address << 1;
    I2C3_MDR_R    = slave_memory_address;
    I2C3_MCS_R  = 3;                  /* S-(saddr+w)-ACK-maddr-ACK */

    error = I2C_wait_till_done();   /* wait until write is complete */
    if (error) return error;

    /* send data one byte at a time */
    while (bytes_count > 1)
    {
        I2C3_MDR_R    = *data++;             /* write the next byte */
      I2C3_MCS_R = 1;                   /* -data-ACK- */
        error = I2C_wait_till_done();
        if (error) return error;
        bytes_count--;
    }
    
    /* send last byte and a STOP */
    I2C3_MDR_R    = *data++;                 /* write the last byte */
    I2C3_MCS_R  = 5;                       /* -data-ACK-P */
    error = I2C_wait_till_done();
    while(I2C3_MCS_R  & 0x40);             /* wait until bus is not busy */
    if (error) return error;
    return 0;       /* no error */
}

//OLED
void OLED_Command( uint8_t temp){
    
	  I2C3_Wr(0x3C,0x00,temp);
}

/*******************************************************************************
 * Function: void OLED_Data ( uint8_t temp)
 *
 * Returns: Nothing
 *
 * Description: sends data to the OLED
 * 
 ******************************************************************************/

void OLED_Data( uint8_t temp){
 
	  I2C3_Wr(0x3C,0x40,temp);
	  
}

/*******************************************************************************
 * Function: void OLED_Init ()
 *
 * Returns: Nothing
 *
 * Description: Initializes OLED
 * 
 ******************************************************************************/

void OLED_Init() {
    
    OLED_Command(OLED_DISPLAYOFF);         // 0xAE
    OLED_Command(OLED_SETDISPLAYCLOCKDIV); // 0xD5
    OLED_Command(0x80);                    // the suggested ratio 0x80
    OLED_Command(OLED_SETMULTIPLEX);       // 0xA8
    OLED_Command(0x1F);
    OLED_Command(OLED_SETDISPLAYOFFSET);   // 0xD3
    OLED_Command(0x0);                        // no offset
    OLED_Command(OLED_SETSTARTLINE | 0x0); // line #0
    OLED_Command(OLED_CHARGEPUMP);         // 0x8D
    OLED_Command(0xAF);
    OLED_Command(OLED_MEMORYMODE);         // 0x20
    OLED_Command(0x00);                    // 0x0 act like ks0108
    OLED_Command(OLED_SEGREMAP | 0x1);
    OLED_Command(OLED_COMSCANDEC);
    OLED_Command(OLED_SETCOMPINS);         // 0xDA
    OLED_Command(0x02);
    OLED_Command(OLED_SETCONTRAST);        // 0x81
    OLED_Command(0x8F);
    OLED_Command(OLED_SETPRECHARGE);       // 0xd9
    OLED_Command(0xF1);
    OLED_Command(OLED_SETVCOMDETECT);      // 0xDB
    OLED_Command(0x40);
    OLED_Command(OLED_DISPLAYALLON_RESUME);// 0xA4
    OLED_Command(OLED_NORMALDISPLAY);      // 0xA6
    OLED_Command(OLED_DISPLAYON);          //--turn on oled panel

}

/*******************************************************************************
 * Function: void OLED_YX(unsigned char Row, unsigned char Column)
 *
 * Returns: Nothing
 *
 * Description: Sets the X and Y coordinates
 * 
 ******************************************************************************/

void OLED_YX(unsigned char Row, unsigned char Column)
{
    OLED_Command( 0xB0 + Row);
    OLED_Command( 0x00 + (8*Column & 0x0F) );
    OLED_Command( 0x10 + ((8*Column>>4)&0x0F) );
}

/*******************************************************************************
 * Function: void OLED_PutChar(char ch)
 *
 * Returns: Nothing
 *
 * Description: Writes a character to the OLED
 * 
 ******************************************************************************/

void OLED_PutChar(char ch )
{
	 uint8_t bytes[9];
	   const uint8_t *base = &OledFont[ch - 32][0];
    if ( ( ch < 32 ) || ( ch > 127 ) ){
        ch = ' ';
    }



   
    //bytes[0] = 0x40;
    memmove( bytes + 1, base, 8 );
       
		I2C3_Write_Multiple(0x3C,0x40,9,bytes);
    
}


/*******************************************************************************
 * Function: void OLED_Clear()
 *
 * Returns: Nothing
 *
 * Description: Clears the OLED
 * 
 ******************************************************************************/

void OLED_Clear()
{
	uint16_t row;
	uint16_t col ;
    for ( row = 0; row < 8; row++ ) {
        for ( col = 0; col < 16; col++ ) {
            OLED_YX( row, col );
            OLED_PutChar(' ');
        }
    }
}


/*******************************************************************************
 * Function:  void OLED_Write_String( char *s )
 * 
 * Returns: Nothing
 *
 * Description: Writes a string to the OLED
 * 
 ******************************************************************************/

void OLED_Write_String( char *s )
{
    while (*s) OLED_PutChar( *s++);
}


/*******************************************************************************
 * Function:  void OLED_Write_Integer ( uint8_t i )
 * 
 * Returns: Nothing
 *
 * Description: Writes an integer to the OLED
 * 
 ******************************************************************************/

void OLED_Write_Integer(uint8_t  i)
{
     char s[20];
     sprintf( s, "%d", i );
     OLED_Write_String( s );
     OLED_Write_String( "     " );
}

/*******************************************************************************
 * Function:  void OLED_Write_Float( float f )
 * 
 * Returns: Nothing
 *
 * Description: Writes a float to the OLED
 * 
 ******************************************************************************/
int x;
