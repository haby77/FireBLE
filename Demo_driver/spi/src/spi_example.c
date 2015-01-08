/****************************************************************************
 *   $Id:: spi_example.c                                                   $
 *   Project: QUINTIC QN9020 SPI example
 *
 *   Description:
 *     This file contains SPI driver usage.
 *
****************************************************************************/

#include "system.h"
#include "spi.h"


/****************************************************************************
*               SPI driver code example
*SPI driver has the master and slave mode, which are use the same API.
*
*
*Note: 
*   The max speed of SPI should no more than (__USART_CLK/4)
*   The size of transmission should be multiple of 4 at 32 bits mode.
*
****************************************************************************/

volatile uint8_t rx_flag = 0;
volatile uint8_t tx_flag = 0;


void led_blink_left(void)
{
    rx_flag = 0;
}

void led_blink_right(void)
{
    tx_flag = 0;
}

void spi_io_config(void)
{
    // pin mux
    syscon_SetPMCR0(QN_SYSCON, P07_SW_CLK_PIN_CTRL       
                             | P06_SW_DAT_PIN_CTRL       
                             | P13_SPI1_CLK_PIN_CTRL      //P1.3 spi1 clk
                             | P12_SPI1_CS0_PIN_CTRL      //P1.2 spi1 cs
                             | P11_SPI1_DAT_PIN_CTRL      //P1.1 spi1 data out 
                             | P10_SPI1_DIN_PIN_CTRL      //P1.0 spi1 data in
                             );
    syscon_SetPMCR1(QN_SYSCON, P35_SPI0_CS0_PIN_CTRL      //P3.5 spi0 cs 
                             | P34_SPI0_CLK_PIN_CTRL      //P3.4 spi0 clk
                             | P33_SPI0_DAT_PIN_CTRL      //P3.3 spi0 data out
                             | P32_SPI0_DIN_PIN_CTRL      //P3.2 spi0 data in
                             );
    
    // pin select
    syscon_SetPMCR2(QN_SYSCON, SYSCON_MASK_SPI0_PIN_SEL);   

    // pin pull ( 00 : High-Z,  01 : Pull-down,  10 : Pull-up,  11 : Reserved )
    syscon_SetPPCR0(QN_SYSCON, 0xAAAA5AAA);
    syscon_SetPPCR1(QN_SYSCON, 0x2AAAAAAA);	     
}

/*  
 * Main Program
 */
int main (void)
{
    uint32_t j;
    uint8_t buffer[40];
    uint8_t rxbuffer[40];
    
    SystemInit();
    
    //SPI io configurate
    spi_io_config();
    
    //Initialize SPI
//    spi_init(QN_SPI0, SPI_BITRATE(1000000), SPI_32BIT,  SPI_MASTER_MOD);
    spi_init(QN_SPI1, SPI_BITRATE(1000000), SPI_8BIT, SPI_MASTER_MOD);

    for (j = 0; j < 40; j++) {
        rxbuffer[j] = 0;
        buffer[j] = j;
    }

    //Write 4byte data
    tx_flag = 1;
//    spi_write(QN_SPI0, buffer, 4, led_blink_right);
    spi_write(QN_SPI1, buffer, 4, led_blink_right);
    while(tx_flag);
    
    //Read 4byte data
    rx_flag = 1;
//    spi_read(QN_SPI0,  rxbuffer, 4, led_blink_left);
    spi_read(QN_SPI1,  rxbuffer, 4, led_blink_left);
    while(rx_flag);
    
    
    while (1)                             /* Loop forever */
    {
        tx_flag = 1;
        spi_write(QN_SPI1, buffer, 8, led_blink_right);
        while(tx_flag);
        delay(1000);
        
        rx_flag = 1;
        spi_read(QN_SPI1,  rxbuffer, 8, led_blink_left);
        while(rx_flag);
        delay(1000);
    }
}

