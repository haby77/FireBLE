/****************************************************************************
 *   $Id:: i2c_example.c                                                   $
 *   Project: QUINTIC QN9020 I2C example
 *
 *   Description:
 *     This file contains I2C driver usage.
 *
****************************************************************************/

#include "i2c.h"
#include "system.h"



/****************************************************************************
*               I2C driver code example
*I2C driver has the master and slave mode.
*The master mode function is as below:
*@I2C_BYTE_WRITE: register address is 8 bits
*@I2C_BYTE_WRITE2: register address is 16 bits
*@I2C_nBYTE_WRITE: write n bytes data
*the read function is similar
*
*The slave mode function is as below:
*The read/write is done in the interrupt handler:I2C_IRQHandler
*
*Note: the Device Address is 7-bit without read/write bit.
*
****************************************************************************/


uint8_t i2cbuffer[104];

void i2c_io_config(void)
{
    // pin mux
    syscon_SetPMCR0(QN_SYSCON, P07_SW_CLK_PIN_CTRL       
                             | P06_SW_DAT_PIN_CTRL
                             );
    syscon_SetPMCR1(QN_SYSCON, P24_I2C_SCL_PIN_CTRL  //P2.4 i2c scl
                             | P23_I2C_SDA_PIN_CTRL  //P2.3 i2c sda     
                             );
    // pin select
    syscon_SetPMCR2(QN_SYSCON, 0);   

    // pin pull ( 00 : High-Z,  01 : Pull-down,  10 : Pull-up,  11 : Reserved )
    syscon_SetPPCR0(QN_SYSCON, 0xAAAA5AAA);
    syscon_SetPPCR1(QN_SYSCON, 0x2AAAAAAA);
}

/* Main Program */
#if I2C_MODE == I2C_MASTER
int main (void)
{
    uint32_t j;
    uint8_t buffer[100];
    uint8_t rxbuffer[100];
    
    SystemInit();
        
    //i2c io configurate
    i2c_io_config();
    
    //Initialize I2C master
    i2c_init(I2C_SCL_RATIO(100000), i2cbuffer, 104);


    for (j = 0; j < 100; j++) {
        buffer[j] = 0x55 + j;
        rxbuffer[j] = 0;
    }    
    
    while (1)                                /* Loop forever */
    {
        // Write 1byte data(0x80) to register(address is 0x00 and 1byte) of device(address is 0x21)
        I2C_BYTE_WRITE(0x21, 0x00, 0x80);
        for (j = 0; j < 10000; j++);
        // Read 1byte data from register(address is 0x06 and 1byte) of device(address is 0x21)
        buffer[0] = I2C_BYTE_READ(0x21, 0x06);    
        for (j = 0; j < 10000; j++);

        // Write 1byte data(0x11) to register(address is 0x00 and 2byte) of device(address is 0x50)
        I2C_BYTE_WRITE2(0x50, 0x00, 0x11);        
        for (j = 0; j < 10000; j++);
        // Read 1byte data from register(address is 0x00 and 2byte) of device(address is 0x50)
        buffer[0] = I2C_BYTE_READ2(0x50, 0x00);    
        for (j = 0; j < 10000; j++);

        // Write nbyte data(50byte) to register(address is 0x00 and 2byte) of device(address is 0x50)
        I2C_nBYTE_WRITE2(0x50, 0x00, buffer, 50);        
        for (j = 0; j < 10000; j++);
        // Read nbyte data(60byte) from register(address is 0x00 and 2byte) of device(address is 0x50)
        I2C_nBYTE_READ2(0x50, 0x00, rxbuffer, 60); 
        for (j = 0; j < 10000; j++);

    }
}
#else // I2C_SLAVE
int main (void)
{

    SystemInit();

    //i2c io configurate.
    i2c_io_config();

    /*
     * Initialize I2C slave.
     * This i2c slave address is QN9020_I2C_ADDR.
     * I2c master can read n byte data from this slave, and write n byte data to this slave.
     */
    i2c_init(i2cbuffer, 104);

    while (1)                                /* Loop forever */
    {
    
    }
}
#endif /* I2C_MODE == I2C_MASTER */

