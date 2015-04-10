/**
 ****************************************************************************************
 *
 * @file spi.c
 *
 * @brief SPI driver for QN9020.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  SPI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "spi.h"
#if ((CONFIG_ENABLE_DRIVER_SPI0==TRUE || CONFIG_ENABLE_DRIVER_SPI1==TRUE) \
    && CONFIG_ENABLE_ROM_DRIVER_SPI==FALSE)
#if SPI_DMA_EN==TRUE
#include "dma.h"
#endif

/*
 * STRUCT DEFINITIONS
 *****************************************************************************************
 */

/// SPI channel parameters, holding data used for synchronous R/W data transactions
struct spi_txrxchannel
{
    int32_t  size;
    uint8_t  *bufptr;
    #if SPI_CALLBACK_EN==TRUE
    void     (*callback)(void);
    #endif
};

///Structure defining SPI environment parameters
struct spi_env_tag
{
    enum SPI_MODE mode;
    enum SPI_BUFFER_WIDTH width;
    struct spi_txrxchannel tx;
    struct spi_txrxchannel rx;
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
#if CONFIG_ENABLE_DRIVER_SPI0==TRUE
///SPI0 environment variable
static struct spi_env_tag spi0_env;
#endif
#if CONFIG_ENABLE_DRIVER_SPI1==TRUE
///SPI1 environment variable
static struct spi_env_tag spi1_env;
#endif

/*
 * LOCAL FUNCTION DECLARATION
 ****************************************************************************************
 */
#if ((CONFIG_ENABLE_DRIVER_SPI0==TRUE && CONFIG_SPI0_TX_ENABLE_INTERRUPT==FALSE) \
  || (CONFIG_ENABLE_DRIVER_SPI1==TRUE && CONFIG_SPI1_TX_ENABLE_INTERRUPT==FALSE))
/**
 ****************************************************************************************
 * @brief Transmit data to SPI TX FIFO.
 * @param[in]       SPI           QN_SPI0 or QN_SPI1
 * @param[in]       spi_env       Environment Variable of specified SPI port
 * @description
 * Start to transmit date to specified SPI port until expected tramsmitting data size is reduced to zero.
 ****************************************************************************************
 */
#if SPI_TX_DMA_EN==FALSE
static void spi_transmit_data(QN_SPI_TypeDef * SPI, struct spi_env_tag *spi_env)
{
    while ( spi_env->tx.size > 0 )
    {
        while ( !(spi_spi_GetSR(SPI) & SPI_MASK_TX_FIFO_NFUL_IF) );

        spi_tx_data(SPI, spi_env->width, spi_env->tx.bufptr, spi_env->tx.size);
#if (defined(CFG_HCI_SPI))
        gpio_write_pin(CFG_HCI_SPI_WR_CTRL_PIN, GPIO_LOW);
#endif
        
        spi_env->tx.bufptr++;
        spi_env->tx.size--;
        if (spi_env->width == SPI_32BIT) {
            spi_env->tx.bufptr += 3;
            spi_env->tx.size -= 3;
        }
    }
    
#if (defined(CFG_HCI_SPI))
    // Wait until the data is transfered.
    while (!( spi_spi_GetSR(SPI) & SPI_MASK_TX_FIFO_EMPT )); 
    
    // Wait until the Busy bit is cleared
    while ( spi_spi_GetSR(SPI) & SPI_MASK_BUSY );
    
    gpio_write_pin(CFG_HCI_SPI_WR_CTRL_PIN, GPIO_HIGH);
#endif

    #if SPI_CALLBACK_EN==TRUE
    // Call end of transmission callback
    if(spi_env->tx.callback != NULL)
    {
        spi_env->tx.callback();
    }
    #endif
}
#endif
#endif

#if ((CONFIG_ENABLE_DRIVER_SPI0==TRUE && CONFIG_SPI0_RX_ENABLE_INTERRUPT==FALSE) \
  || (CONFIG_ENABLE_DRIVER_SPI1==TRUE && CONFIG_SPI1_RX_ENABLE_INTERRUPT==FALSE))
/**
 ****************************************************************************************
 * @brief Receives data from SPI RX FIFO.
 * @param[in]       SPI           QN_SPI0 or QN_SPI1
 * @param[in]       spi_env       Environment Variable of specified SPI port
 * @description
 * Start to receive date from specified SPI port until expected receiving data size is reduced to zero.
 ****************************************************************************************
 */
#if (!(SPI_RX_DMA_EN==TRUE && SPI0_MOD_MST_EN==FALSE && SPI1_MOD_MST_EN==FALSE))
static void spi_receive_data(QN_SPI_TypeDef * SPI, struct spi_env_tag *spi_env)
{
    while ( spi_env->rx.size > 0 )
    {
        /* As long as Receive FIFO is not empty, I can always receive. */
        /* if it's a peer-to-peer communication, TXD needs to be written before a read can take place. */
        if ( spi_env->mode == SPI_MASTER_MOD ) {
            spi_spi_SetTXD(SPI, SPI_DUMMY_BYTE);
            /* Wait until the Busy bit is cleared */
            //while ( spi_spi_GetSR(SPI) & SPI_MASK_BUSY );
        }
        while ( !(spi_spi_GetSR(SPI) & SPI_MASK_RX_FIFO_NEMT_IF) );

        spi_rx_data(SPI, spi_env->width, spi_env->rx.bufptr, spi_env->rx.size);

        spi_env->rx.bufptr++;
        spi_env->rx.size--;
        if (spi_env->width == SPI_32BIT) {
            spi_env->rx.bufptr += 3;
            spi_env->rx.size -= 3;
        }
    }

    #if SPI_CALLBACK_EN==TRUE
    // Call end of reception callback
    if(spi_env->rx.callback != NULL)
    {
        spi_env->rx.callback();
    }
    #endif

}
#endif
#endif

/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#if (CONFIG_ENABLE_DRIVER_SPI0==TRUE && CONFIG_SPI0_TX_DEFAULT_IRQHANDLER==TRUE)
/**
 ****************************************************************************************
 * @brief SPI0 TX interrupt handler.
 * @description
 * If SPI0 TX FIFO is not full, it then generates interrupt. In this handler, data is transmited to port SPI0
 * until expected tramsmitting data size is reduced to zero. After last data transfer is finished, the callback function is called.
 ****************************************************************************************
 */
#if SPI_TX_DMA_EN==FALSE
void SPI0_TX_IRQHandler(void)
{
    uint32_t reg;

    reg = spi_spi_GetSR(QN_SPI0);
    if ( reg & SPI_MASK_TX_FIFO_NFUL_IF )   /* TX FIFO not full interrupt */
    {
        if (spi0_env.tx.size > 0) {

            spi_tx_data(QN_SPI0, spi0_env.width, spi0_env.tx.bufptr, spi0_env.tx.size);

            spi0_env.tx.bufptr++;
            spi0_env.tx.size--;
            if (spi0_env.width == SPI_32BIT) {
                spi0_env.tx.bufptr += 3;
                spi0_env.tx.size -= 3;
            }
        }
        else {
            spi_tx_int_enable(QN_SPI0, MASK_DISABLE);
            #if SPI_CALLBACK_EN==TRUE
            // Call end of transmission callback
            if (spi0_env.tx.callback != NULL)
            {
                spi0_env.tx.callback();
            }
            #endif
        }
    }
}
#endif
#endif

#if (CONFIG_ENABLE_DRIVER_SPI0==TRUE && CONFIG_SPI0_RX_DEFAULT_IRQHANDLER==TRUE)
/**
 ****************************************************************************************
 * @brief SPI0 RX interrupt handler.
 * @description
 * If SPI0 RX FIFO is not empty, it then generates interrupt. In this handler, data is received from
 * port SPI0 until expected receiving data size is reduced to zero. After last data received, the callback function is called.
 ****************************************************************************************
 */
#if (!(SPI_RX_DMA_EN==TRUE && SPI0_MOD_MST_EN==FALSE))
void SPI0_RX_IRQHandler(void)
{
    uint32_t reg;

    reg = spi_spi_GetSR(QN_SPI0);
    if ( reg & SPI_MASK_RX_FIFO_OVR_IF )  // RX FIFO overrun interrupt
    {
        spi_spi_ClrSR(QN_SPI0, SPI_MASK_RX_FIFO_OVR_IF); // clear interrupt
    }

    if ( reg & SPI_MASK_RX_FIFO_NEMT_IF ) { // RX FIFO not empty interrupt

        if (spi0_env.rx.size > 0) {

            spi_rx_data(QN_SPI0, spi0_env.width, spi0_env.rx.bufptr, spi0_env.rx.size);

            spi0_env.rx.bufptr++;
            spi0_env.rx.size--;
            if (spi0_env.width == SPI_32BIT) {
                spi0_env.rx.bufptr += 3;
                spi0_env.rx.size -= 3;
            }

            if ((spi0_env.mode == SPI_MASTER_MOD) && (spi0_env.rx.size > 0)) {
                spi_spi_SetTXD(QN_SPI0, SPI_DUMMY_BYTE);
                // Wait until the Busy bit is cleared
                //while ( spi_spi_GetSR(QN_SPI0) & SPI_MASK_BUSY );
            }

            if (spi0_env.rx.size <= 0) {
                spi_rx_int_enable(QN_SPI0, MASK_DISABLE);
                #if SPI_CALLBACK_EN==TRUE
                // Call end of reception callback
                if (spi0_env.rx.callback != NULL)
                {
                    spi0_env.rx.callback();
                }
                #endif
            }
        }
        else {
            reg = spi_spi_GetRXD(QN_SPI0);  // clear interrupt
        }
    }

}
#endif
#endif

#if (CONFIG_ENABLE_DRIVER_SPI1==TRUE && CONFIG_SPI1_TX_DEFAULT_IRQHANDLER==TRUE)
/**
 ****************************************************************************************
 * @brief SPI1 TX interrupt handler.
 * @description
 * If SPI1 TX FIFO is not full, it then generates interrupt. In this handler, data is transmited to port SPI1
 * until expected tramsmitting data size is reduced to zero. After last data transfer is finished, the callback function is called.
 ****************************************************************************************
 */
#if SPI_TX_DMA_EN==FALSE
void SPI1_TX_IRQHandler(void)
{
    uint32_t reg;

    reg = spi_spi_GetSR(QN_SPI1);
    if ( reg & SPI_MASK_TX_FIFO_NFUL_IF )   /* TX FIFO not full interrupt */
    {
        if (spi1_env.tx.size > 0) {

            spi_tx_data(QN_SPI1, spi1_env.width, spi1_env.tx.bufptr, spi1_env.tx.size);

            spi1_env.tx.bufptr++;
            spi1_env.tx.size--;
            if (spi1_env.width == SPI_32BIT) {
                spi1_env.tx.bufptr += 3;
                spi1_env.tx.size -= 3;
            }
        }
        else {
            spi_tx_int_enable(QN_SPI1, MASK_DISABLE);
            #if SPI_CALLBACK_EN==TRUE
            // Call end of transmission callback
            if (spi1_env.tx.callback != NULL)
            {
                spi1_env.tx.callback();
            }
            #endif
        }
    }
}
#endif
#endif

#if (CONFIG_ENABLE_DRIVER_SPI1==TRUE && CONFIG_SPI1_RX_DEFAULT_IRQHANDLER==TRUE)
/**
 ****************************************************************************************
 * @brief SPI1 RX interrupt handler.
 * @description
 * If SPI1 RX FIFO is not empty, it then generates interrupt. In this handler, data is received from port SPI1
 * until expected receiving data size is reduced to zero. After last data received, the callback function is called.
 ****************************************************************************************
 */
#if (!(SPI_RX_DMA_EN==TRUE && SPI1_MOD_MST_EN==FALSE))
void SPI1_RX_IRQHandler(void)
{
    uint32_t reg;

    reg = spi_spi_GetSR(QN_SPI1);
    if ( reg & SPI_MASK_RX_FIFO_OVR_IF )  // RX FIFO overrun interrupt
    {
        spi_spi_ClrSR(QN_SPI1, SPI_MASK_RX_FIFO_OVR_IF); // clear interrupt
    }

    if ( reg & SPI_MASK_RX_FIFO_NEMT_IF )  // RX FIFO not empty interrupt
    {
        if (spi1_env.rx.size > 0) {

            spi_rx_data(QN_SPI1, spi1_env.width, spi1_env.rx.bufptr, spi1_env.rx.size);

            spi1_env.rx.bufptr++;
            spi1_env.rx.size--;
            if (spi1_env.width == SPI_32BIT) {
                spi1_env.rx.bufptr += 3;
                spi1_env.rx.size -= 3;
            }

            if ((spi1_env.mode == SPI_MASTER_MOD) && (spi1_env.rx.size > 0)) {
                spi_spi_SetTXD(QN_SPI1, SPI_DUMMY_BYTE);
                // Wait until the Busy bit is cleared
                //while ( spi_spi_GetSR(QN_SPI1) & SPI_MASK_BUSY );
            }

            if (spi1_env.rx.size <= 0) {
                spi_rx_int_enable(QN_SPI1, MASK_DISABLE);
                #if SPI_CALLBACK_EN==TRUE
                // Call end of reception callback
                if (spi1_env.rx.callback != NULL)
                {
                    spi1_env.rx.callback();
                }
                #endif
            }
        }
        else {
            reg = spi_spi_GetRXD(QN_SPI1); // clear interrupt
        }
    }
}
#endif
#endif

/**
 ****************************************************************************************
 * @brief Initialize the SPI
 * @param[in]  SPI            QN_SPI0 or QN_SPI1
 * @param[in]  bitrate        sck speed: SPI_BITRATE(1000) means 1Kbps
 * @param[in]  width          32bits or 8bits
 * @param[in]  mode           master or slave
 * @description
 *  This function is used to initialize SPI. It consists of bit rate, transmit width, SPI mode, big/little endian,
 *  MSB/LSB first, master/salve. The function is also used to enable specified SPI interrupt, and
 *  enable NVIC SPI IRQ.
 *****************************************************************************************
 */
void spi_init(QN_SPI_TypeDef * SPI, uint32_t bitrate, enum SPI_BUFFER_WIDTH width, enum SPI_MODE mode)
{
    uint32_t reg = 0;
    struct spi_env_tag *spi_env = &spi0_env;

    spi_clock_on(SPI);

#if CONFIG_ENABLE_DRIVER_SPI0==TRUE
    if (SPI == QN_SPI0) {
        spi_env = &spi0_env;

        reg = SPI_MASK_SPI_IE
            | SPI_SSx_CFG
            | SPI_BIG_ENDIAN
            | width
            | SPI_BITORDER_CFG
            | mode
#if SPI0_MOD_3WIRE_EN==TRUE
            | SPI_MASK_DATA_IO_MODE
#endif
            | SPI_CPHA_1
            | SPI_CPOL_0;

        #if CONFIG_SPI0_TX_ENABLE_INTERRUPT==TRUE && SPI_TX_DMA_EN==FALSE
        // Enable the SPI0 TX Interrupt
        NVIC_EnableIRQ(SPI0_TX_IRQn);
        #endif

        #if CONFIG_SPI0_RX_ENABLE_INTERRUPT==TRUE && (!(SPI_RX_DMA_EN==TRUE && SPI0_MOD_MST_EN==FALSE))
        // Enable the SPI0 RX Interrupt
        NVIC_EnableIRQ(SPI0_RX_IRQn);
        #endif
    }
#endif

#if CONFIG_ENABLE_DRIVER_SPI1==TRUE
    if (SPI == QN_SPI1) {
        spi_env = &spi1_env;

        reg = SPI_MASK_SPI_IE
            | SPI_SSx_CFG
            | SPI_BIG_ENDIAN
            | width
            | SPI_BITORDER_CFG
            | mode
#if SPI1_MOD_3WIRE_EN==TRUE
            | SPI_MASK_DATA_IO_MODE
#endif
            | SPI_CPHA_1
            | SPI_CPOL_0;

        #if CONFIG_SPI1_TX_ENABLE_INTERRUPT==TRUE && SPI_TX_DMA_EN==FALSE
        // Enable the SPI1 TX Interrupt
        NVIC_EnableIRQ(SPI1_TX_IRQn);
        #endif

        #if CONFIG_SPI1_RX_ENABLE_INTERRUPT==TRUE && (!(SPI_RX_DMA_EN==TRUE && SPI1_MOD_MST_EN==FALSE))
        // Enable the SPI1 RX Interrupt
        NVIC_EnableIRQ(SPI1_RX_IRQn);
        #endif
    }
#endif

    reg |= bitrate;
    spi_spi_SetCR0(SPI, reg);

#if SPI_DMA_EN==TRUE
    dma_init();
#endif
    //Configure SPI environment
    spi_env->width = width;     // 8-bits or 32-bits
    spi_env->mode = mode;       // master or slave
    spi_env->rx.size = 0;
    spi_env->tx.size = 0;
    spi_env->rx.bufptr = NULL;
    spi_env->tx.bufptr = NULL;
    #if SPI_CALLBACK_EN==TRUE
    spi_env->rx.callback = NULL;
    spi_env->tx.callback = NULL;
    #endif
}

/**
 ****************************************************************************************
 * @brief Start a data reception.
 * @param[in]      SPI            QN_SPI0 or QN_SPI1
 * @param[in,out]  bufptr         Pointer to the RX data buffer
 * @param[in]      size           Size of the expected reception
 * @param[in]      rx_callback    Callback for end of reception
 * @description
 * This function is used to read Rx data from RX FIFO and the data will be stored in bufptr.
 * As soon as the end of the data transfer or a buffer overflow is detected, the callback function is called.
 *
 *****************************************************************************************
 */
void spi_read(QN_SPI_TypeDef * SPI, uint8_t *bufptr, int32_t size, void (*rx_callback)(void))
{
    #if (SPI_RX_DMA_EN==TRUE && (SPI0_MOD_MST_EN==FALSE || SPI1_MOD_MST_EN==FALSE))
    enum DMA_TRANS_MODE trans_mod;
    #endif
    // option: clear RX buffer
    //spi_spi_SetCR1(SPI, SPI_MASK_RX_FIFO_CLR);

#if CONFIG_ENABLE_DRIVER_SPI0==TRUE
    if (SPI == QN_SPI0) {
    #if (SPI_RX_DMA_EN==TRUE && SPI0_MOD_MST_EN==FALSE)
        if (spi0_env.width == SPI_8BIT) {
            trans_mod = DMA_TRANS_BYTE;
        }
        else {
            trans_mod = DMA_TRANS_WORD;
        }
        dma_rx(trans_mod, DMA_SPI0_RX, (uint32_t)bufptr, size, rx_callback);
    #else
        //Store environment parameters
        spi0_env.rx.bufptr = bufptr;
        spi0_env.rx.size = size;
        #if SPI_CALLBACK_EN==TRUE
        spi0_env.rx.callback = rx_callback;
        #endif
        
        #if CONFIG_SPI0_RX_ENABLE_INTERRUPT==TRUE
        spi_rx_int_enable(SPI, MASK_ENABLE);
        if ((spi0_env.mode == SPI_MASTER_MOD) && (spi0_env.rx.size != 0)) {
            spi_spi_SetTXD(SPI, SPI_DUMMY_BYTE);
        }
        #else
        // Start data reception
        spi_receive_data(SPI, &spi0_env);
        #endif
    #endif
    }
#endif

#if CONFIG_ENABLE_DRIVER_SPI1==TRUE
    if (SPI == QN_SPI1) {
    #if (SPI_RX_DMA_EN==TRUE && SPI1_MOD_MST_EN==FALSE)
        if (spi1_env.width == SPI_8BIT) {
            trans_mod = DMA_TRANS_BYTE;
        }
        else {
            trans_mod = DMA_TRANS_WORD;
        }
        dma_rx(trans_mod, DMA_SPI1_RX, (uint32_t)bufptr, size, rx_callback);
    #else        
        //Store environment parameters
        spi1_env.rx.bufptr = bufptr;
        spi1_env.rx.size = size;
        #if SPI_CALLBACK_EN==TRUE
        spi1_env.rx.callback = rx_callback;
        #endif
        
        #if CONFIG_SPI1_RX_ENABLE_INTERRUPT==TRUE
        spi_rx_int_enable(SPI, MASK_ENABLE);
        if ((spi1_env.mode == SPI_MASTER_MOD) && (spi1_env.rx.size != 0)) {
            spi_spi_SetTXD(SPI, SPI_DUMMY_BYTE);
        }
        #else
        // Start data reception
        spi_receive_data(SPI, &spi1_env);
        #endif
    #endif
    }
#endif
}

/**
 ****************************************************************************************
 * @brief Start a data transmission.
 * @param[in]  SPI            QN_SPI0 or QN_SPI1
 * @param[in]  bufptr         Pointer to the TX data buffer
 * @param[in]  size           Size of the transmission
 * @param[in]  tx_callback    Callback for end of transmission
 * @description
 * This function is used to write data into TX buffer to transmit data by SPI.
 * As soon as the end of the data transfer is detected, the callback function is called.
 *
 *****************************************************************************************
 */
void spi_write(QN_SPI_TypeDef * SPI, uint8_t *bufptr, int32_t size, void (*tx_callback)(void))
{
    #if SPI_TX_DMA_EN==TRUE
    enum DMA_TRANS_MODE trans_mod;
    #endif
    // option: clear TX buffer
    //spi_spi_SetCR1(SPI, SPI_MASK_TX_FIFO_CLR);

#if CONFIG_ENABLE_DRIVER_SPI0==TRUE
    if (SPI == QN_SPI0) {
    #if SPI_TX_DMA_EN==TRUE
        if (spi0_env.width == SPI_8BIT) {
            trans_mod = DMA_TRANS_BYTE;
        }
        else {
            trans_mod = DMA_TRANS_WORD;
        }
        dma_tx(trans_mod, (uint32_t)bufptr, DMA_SPI0_TX, size, tx_callback);
    #else        
        //Store environment parameters
        spi0_env.tx.bufptr = bufptr;
        spi0_env.tx.size = size;
        #if SPI_CALLBACK_EN==TRUE
        spi0_env.tx.callback = tx_callback;
        #endif
        
        #if CONFIG_SPI0_TX_ENABLE_INTERRUPT==TRUE
        spi_tx_int_enable(SPI, MASK_ENABLE);
        #else
        // Start data transmission
        spi_transmit_data(SPI, &spi0_env);
        #endif
    #endif
    }
#endif

#if CONFIG_ENABLE_DRIVER_SPI1==TRUE
    if (SPI == QN_SPI1) {
    #if SPI_TX_DMA_EN==TRUE
        if (spi1_env.width == SPI_8BIT) {
            trans_mod = DMA_TRANS_BYTE;
        }
        else {
            trans_mod = DMA_TRANS_WORD;
        }
        dma_tx(trans_mod, (uint32_t)bufptr, DMA_SPI1_TX, size, tx_callback);
    #else
        //Store environment parameters
        spi1_env.tx.bufptr = bufptr;
        spi1_env.tx.size = size;
        #if SPI_CALLBACK_EN==TRUE
        spi1_env.tx.callback = tx_callback;
        #endif

        #if CONFIG_SPI1_TX_ENABLE_INTERRUPT==TRUE
        spi_tx_int_enable(SPI, MASK_ENABLE);
        #else
        // Start data transmission
        spi_transmit_data(SPI, &spi1_env);
        #endif
    #endif
    }
#endif
}

/**
 ****************************************************************************************
 * @brief  Check if tx is ongoing
 * @return spi tx/rx status 
 * @param[in]       SPI          QN_SPI0 or QN_SPI1
 * @description
 *  This function is used to check SPI TX status
 *****************************************************************************************
 */
int spi_check_tx_free(QN_SPI_TypeDef *SPI)
{

#if SPI_TX_DMA_EN==TRUE
    // check DMA status
    if (dma_dma_GetIntStatus(QN_DMA) & DMA_MASK_BUSY)
        return SPI_TX_BUF_BUSY;
#else
    #if CONFIG_ENABLE_DRIVER_SPI0==TRUE
    if(SPI == QN_SPI0)
    {
        // check tx buffer
        if (spi0_env.tx.size)
            return SPI_TX_BUF_BUSY;
    }
    #endif    

    #if CONFIG_ENABLE_DRIVER_SPI1==TRUE
    if(SPI == QN_SPI1)
    {
        // check tx buffer
        if(spi1_env.tx.size)
            return SPI_TX_BUF_BUSY;
    }
    #endif
#endif

    // check tx busy
    if (!(spi_spi_GetSR(SPI) & SPI_MASK_TX_FIFO_EMPT))
        return SPI_LAST_BYTE_ONGOING;

    return SPI_TX_FREE;
}

#endif /* CONFIG_ENABLE_DRIVER_SPI==TRUE */
/// @} SPI
