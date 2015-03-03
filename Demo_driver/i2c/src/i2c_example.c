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



/* Main Program */
#if I2C_MODE == I2C_MASTER
int main (void)
{
    uint32_t j;
    uint8_t buffer[100];
    uint8_t rxbuffer[100];
    
    SystemInit();
    
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
        // Read 1byte data from register(address is 0x06 and 1byte) of device(address is 0x21)
        buffer[0] = I2C_BYTE_READ(0x21, 0x06);    

        // Write 1byte data(0x11) to register(address is 0x00 and 2byte) of device(address is 0x50)
        I2C_BYTE_WRITE2(0x50, 0x00, 0x11);        
        // Read 1byte data from register(address is 0x00 and 2byte) of device(address is 0x50)
        buffer[0] = I2C_BYTE_READ2(0x50, 0x00);    

        // Write nbyte data(50byte) to register(address is 0x00 and 2byte) of device(address is 0x50)
        I2C_nBYTE_WRITE2(0x50, 0x00, buffer, 50);        
        // Read nbyte data(60byte) from register(address is 0x00 and 2byte) of device(address is 0x50)
        I2C_nBYTE_READ2(0x50, 0x00, rxbuffer, 60); 

    }
}
#else // I2C_SLAVE
int main (void)
{

    SystemInit();

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

