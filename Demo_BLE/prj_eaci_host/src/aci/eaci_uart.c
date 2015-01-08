/**
 ****************************************************************************************
 *
 * @file eaci_uart.c
 *
 * @brief UART transport module functions for Easy Application Controller Interface.
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"
#include "uart.h"
#include "lpc17xx_spi.h"
#include "lpc17xx_gpio.h"

extern void CS_Force(int32_t state);

uint8_t payl_len =0 ;
uint8_t* payl_para= NULL;
uint8_t en_read_flag = 0;
#if defined(CFG_HCI_SPI)
static uint8_t wr_done = 0;
#endif

void tx_complete(void)
{
    wr_ongoing = 0;
    LPC_GPIO1->FIOSET    |= (1 << 17);
    #if defined(CFG_HCI_SPI)
    wr_done = 1;
    #endif
}
void myDelay (uint32_t ulTime)
{
    uint32_t i;

    i = 0;
    while (ulTime--) {
        for (i = 0; i < 5000; i++);
    }
}
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static void eaci_uart_read_start(void)
{
    // Initialize UART in reception mode state
    eaci_env.rx_state = EACI_STATE_RX_START;
    // Set the UART environment to message type 1 byte reception
    #if defined(CFG_HCI_UART)
        uart_read(QN_HCI_UART, &eaci_env.msg_type, 1, eaci_uart_rx_done);
    #endif
}

static void eaci_uart_read_hdr(void)
{
    // Change Rx state - wait for message id and parameter length
    eaci_env.rx_state = EACI_STATE_RX_HDR;
    // Set UART environment to header reception of EACI_MSG_HDR_LEN bytes
    #if defined(CFG_HCI_UART)
        uart_read(QN_HCI_UART, (uint8_t *)&eaci_env.msg_id, EACI_MSG_HDR_LEN, eaci_uart_rx_done);
    #elif defined(CFG_HCI_SPI) 
        spi_read(QN_HCI_SPI,(uint8_t *)&eaci_env.msg_id, EACI_MSG_HDR_LEN, eaci_uart_rx_done);
    #endif
}

static void eaci_uart_read_payl(uint8_t len, uint8_t *par)
{
    // Change rx state to payload reception
        eaci_env.rx_state = EACI_STATE_RX_PAYL;
    // Set UART environment to payload reception of len bytes
    #if defined(CFG_HCI_UART)
        uart_read(QN_HCI_UART, par, len, eaci_uart_rx_done);
    #elif defined(CFG_HCI_SPI) 
        spi_read(QN_HCI_SPI,par, len, eaci_uart_rx_done);   
    #endif
}

void eaci_uart_init(void)
{
    // Start uart reception
    eaci_uart_read_start();
}

void eaci_uart_write(uint8_t len, uint8_t *par)
{
    LPC_GPIO1->FIOCLR    |= (1 << 17);
    myDelay(4);
    // Send out the parameter with length
    #if defined(CFG_HCI_UART)    
        uart_write(QN_HCI_UART, par, len, tx_complete);
    #elif defined(CFG_HCI_SPI)
        wr_done = 0;
        spi_write(QN_HCI_SPI, par, len, tx_complete);
        while(wr_done == 0);
    #endif
}

#ifdef MSG_BUFFER_EN
uint8_t msg_buffer[10][32];
uint8_t msg_index = 0;
#endif

void eaci_uart_rx_done(void)
{
    rd_ongoing = 0;
    switch(eaci_env.rx_state)
    {
        // Message Type received
        case EACI_STATE_RX_START:
            if (eaci_env.msg_type == EACI_MSG_TYPE_EVT || eaci_env.msg_type == EACI_MSG_TYPE_DATA_IND)
                eaci_uart_read_hdr();
            else
            { 
                eaci_uart_init();
            }
            break;
        // Message ID and Parameter Length received
        case EACI_STATE_RX_HDR:
            // Allocate right length space for expected payload
        #ifndef MSG_BUFFER_EN
            eaci_env.msg = MSG_MALLOC(sizeof(struct eaci_msg) + eaci_env.param_len - sizeof(uint8_t));
        #else
            eaci_env.msg = (struct eaci_msg *)(msg_buffer[msg_index]);
            msg_index++;
            if (msg_index == 10) {
                msg_index = 0;
            }
        #endif
            // List header for chaining
            eaci_env.msg->elm.next  = NULL;
            // Message type
            eaci_env.msg->msg_type  = eaci_env.msg_type;
            // Message ID
            eaci_env.msg->msg_id    = eaci_env.msg_id;
            // Parameter length
            eaci_env.msg->param_len = eaci_env.param_len;
            // NO Parameters
            if (eaci_env.param_len == 0)
            {                   
                // Add to message queue
                eaci_msg_que_push(&app_env.msg_que, eaci_env.msg);
                // Change eaci rx state to message header reception
                eaci_uart_read_start();
            }
            else
            {
                eaci_uart_read_payl(eaci_env.param_len, eaci_env.msg->param);
            }
            
            break;
        // Parameter received
        case EACI_STATE_RX_PAYL:
            // Add to message queue with the received payload
            eaci_msg_que_push(&app_env.msg_que, eaci_env.msg);
            // Change eaci rx state to eaci message header reception
            eaci_uart_read_start();
            break;
        // Error
        case EACI_STATE_RX_ERR:
            // Start rx again
            eaci_uart_init();
            break;
    }
}
/// @} EACI_UART
