/****************************************************************************
 *   $Id:: serialflash_example.c                                        $
 *   Project: QUINTIC QN9020 Flash Controller example
 *
 *   Description:
 *     This file contains Flash Controller driver usage.
 *
****************************************************************************/


#include "serialflash.h"
#include "system.h"


int main (void)
{
    uint32_t id, j;
    uint8_t buffer[256];
    uint8_t rxbuffer[256];

    // System initialization
    SystemInit();
    
    // Power on inside serial flash
    power_on_flash();
    
    // Reset value
    for (j = 0; j < 256; j++) {
        rxbuffer[j] = 0;
        buffer[j] = j;
    }

    // Read flash chip ID
    id = read_flash_id();
    
    // Erase 1 sector flash data from 0x4000, and 1 sector size is 4KB
    sector_erase_flash(0x4000, 1);
    
    /*
     *  The parameter "address" note:
     *       * When the address range is from 0x00 to 0x1000 (NVDS area), the address must be 4
     *         integer times.
     *       * When the address range is greater than or equal to 0x1000 (Code area), the
     *         address must be 256 integer times. (Encryption request)
     *  The parameter "size" note:
     *       * When the address range is from 0x00 to 0x1000 (NVDS area), the size must be 4
     *         integer times and less than or equal to 256.
     *       * When the address range is greater than or equal to 0x1000 (Code area), the
     *         size must be 256 bytes. (Encryption request)
     */
    write_flash(0x4000, (uint32_t *)buffer, 256);

    /*
     *  The parameter "address" note:
     *       * When the address range is from 0x00 to 0x1000 (NVDS area), the address must be 4
     *         integer times.
     *       * When the address range is greater than or equal to 0x1000 (Code area), the
     *         address must be 256 integer times. (Encryption request)
     *  The parameter "size" note:
     *       * When the address range is from 0x00 to 0x1000 (NVDS area), the size must be 4
     *         integer times and less than or equal to 256.
     *       * When the address range is greater than or equal to 0x1000 (Code area), the
     *         size must be 256 bytes integer times. (Encryption request)
     */
    read_flash(0x4000, (uint32_t *)rxbuffer, 256);

    
    for (j = 0; j < 256; j++) {
        if (rxbuffer[j] != j) {
            // read flash data error
        }
    }
        
    while (1)                                /* Loop forever */
    {

    }
}

