/**
 ****************************************************************************************
 *
 * @file app_eaci_trans.c
 *
 * @brief UART transport module functions for Easy Application Controller Interface.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup EACI_TRANS
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"
#include "app_eaci_trans.h"
#include "uart.h"
#include "spi.h"

#if EACI_MSG_BUFFER_EN==TRUE
static uint8_t eaci_msg_idx = 0;
uint8_t eaci_msg_buf_used = 0;
static struct eaci_msg eaci_msg_buf[EACI_BUF_DEEP];
#endif

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void eaci_trans_send_error(uint8_t reason)
{
#if (defined(CFG_HCI_UART))
    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 1, 0};
    pdu[1] = EACI_MSG_TYPE_DATA_ERROR;
    pdu[3] = reason;
    eaci_pdu_send(sizeof(pdu), pdu);
#endif
}

static void eaci_trans_read_start(void)
{
    // Initialize UART in reception mode state
    eaci_env.rx_state = EACI_STATE_RX_START;
    // Set the UART environment to message type 1 byte reception
#if EACI_MSG_BUFFER_EN==TRUE
    #if (defined(CFG_HCI_UART))
    uart_read(QN_HCI_PORT, (uint8_t *)&eaci_env.msg_type, 1, eaci_trans_rx_done);
    #elif (defined(CFG_HCI_SPI))
    spi_read(QN_HCI_PORT, (uint8_t *)&eaci_env.msg_type, 1, eaci_trans_rx_done);
    #endif
#else
    #if (defined(CFG_HCI_UART))
    uart_read(QN_HCI_PORT, &eaci_env.msg_type, 1, eaci_trans_rx_done);
    #elif (defined(CFG_HCI_SPI))
    spi_read(QN_HCI_PORT, &eaci_env.msg_type, 1, eaci_trans_rx_done);
    #endif
#endif
}

static void eaci_trans_read_hdr(void)
{
    // Change Rx state - wait for message id and parameter length
    eaci_env.rx_state = EACI_STATE_RX_HDR;
#if (defined(CFG_HCI_UART)) 
    // Set UART environment to header reception of EACI_MSG_HDR_LEN bytes
    uart_read(QN_HCI_PORT, (uint8_t *)&eaci_env.msg_id, EACI_MSG_HDR_LEN, eaci_trans_rx_done);
#elif (defined(CFG_HCI_SPI))
    spi_read(QN_HCI_PORT, (uint8_t *)&eaci_env.msg_id, EACI_MSG_HDR_LEN, eaci_trans_rx_done);
#endif
}

static void eaci_trans_read_payl(uint8_t len)
{
    // Change rx state to payload reception
    eaci_env.rx_state = EACI_STATE_RX_PAYL;
#if EACI_MSG_BUFFER_EN==TRUE
    #if (defined(CFG_HCI_UART))
    // Set UART environment to payload reception of len bytes
    uart_read(QN_HCI_PORT, (uint8_t *)&eaci_msg_buf[eaci_msg_idx].param[1], len, eaci_trans_rx_done);
    #elif (defined(CFG_HCI_SPI))
    spi_read(QN_HCI_PORT, (uint8_t *)&eaci_msg_buf[eaci_msg_idx].param[1], len, eaci_trans_rx_done);
    #endif
#else
    #if (defined(CFG_HCI_UART))
    uart_read(QN_HCI_PORT, eaci_env.param + 1, len, eaci_trans_rx_done);
    #elif (defined(CFG_HCI_SPI))
    spi_read(QN_HCI_PORT, eaci_env.param + 1, len, eaci_trans_rx_done);
    #endif
#endif
}

void eaci_trans_init(void)
{
    // Start uart reception
    eaci_trans_read_start();
}

void eaci_trans_tx_done()
{
    // Defer the freeing of resources to ensure that it is done in background
    ke_evt_set(0x10000000);

#if (defined(QN_EACI_GPIO_WAKEUP_EX_MCU))
    eaci_wakeup_ex_mcu_stop();
#endif
}

void eaci_trans_write(struct ke_msg *msg)
{
    //go to start tx state
    eaci_env.tx_state = EACI_STATE_TX_ONGOING;

#if (defined(QN_EACI_GPIO_WAKEUP_EX_MCU))
    eaci_wakeup_ex_mcu_start();
#endif

#if (defined(CFG_HCI_UART))  
    uart_write(QN_HCI_PORT, ((uint8_t *)&msg->param), msg->param_len, eaci_trans_tx_done);
#elif (defined(CFG_HCI_SPI))
    spi_write(QN_HCI_PORT, ((uint8_t *)&msg->param), msg->param_len, eaci_trans_tx_done);
#endif
}

void eaci_trans_rx_done(void)
{
    switch(eaci_env.rx_state)
    {
        // Message Type received
        case EACI_STATE_RX_START:
            if (eaci_env.msg_type == EACI_MSG_TYPE_CMD || eaci_env.msg_type == EACI_MSG_TYPE_DATA_REQ)
            {
                eaci_env.error = TRUE;
                eaci_trans_read_hdr();
            }
            else
            {
                if (eaci_env.error == TRUE)
                {
                    eaci_env.error = FALSE;
                    eaci_trans_send_error(EACI_TYPE_ERROR);
                }

                eaci_trans_init();
            }
            break;

        // Message ID and Parameter Length received
        case EACI_STATE_RX_HDR:
            if (eaci_env.msg_id > EACI_MSG_REQ_MAX)
            {
                eaci_trans_send_error(EACI_MSG_OOR_ERROR);
                eaci_trans_init();
                break;
            }

#if EACI_MSG_BUFFER_EN==TRUE
            eaci_msg_buf[eaci_msg_idx].id = APP_SYS_UART_DATA_IND;
            eaci_msg_buf[eaci_msg_idx].dest_id = TASK_APP;
            eaci_msg_buf[eaci_msg_idx].src_id = (uint16_t)(eaci_env.msg_type << 8 | eaci_env.msg_id);
            eaci_msg_buf[eaci_msg_idx].param_len = eaci_env.param_len + 1;   
            eaci_msg_buf[eaci_msg_idx].param[0] = eaci_env.param_len;
            eaci_msg_buf_used++;
#else 
            // Encode msg_type and msg_id into src_id and allocate right length space for expected struct
            eaci_env.param = (uint8_t *)ke_msg_alloc(APP_SYS_UART_DATA_IND,
                                                     TASK_APP,
                                                     (uint16_t)(eaci_env.msg_type << 8 | eaci_env.msg_id),
                                                     eaci_env.param_len + 1);
            eaci_env.param[0] = eaci_env.param_len;
#endif
            // NO Parameters
            if (eaci_env.param_len == 0)
            {
                // Send message directly
#if EACI_MSG_BUFFER_EN==TRUE
                ke_msg_send(eaci_msg_buf[eaci_msg_idx].param);
                eaci_msg_idx++;
                if (eaci_msg_idx >= EACI_BUF_DEEP) {
                    eaci_msg_idx = 0;
                }
                if (eaci_msg_buf_used == EACI_BUF_DEEP) {
                    break;
                }
#else
                ke_msg_send(eaci_env.param);
#endif
                // Change eaci rx state to message header reception
                eaci_trans_read_start();
            }
            else
            {
                eaci_trans_read_payl(eaci_env.param_len);
            }
            break;

        // Parameter received
        case EACI_STATE_RX_PAYL:
            // Send received kernel message without other treatment
#if EACI_MSG_BUFFER_EN==TRUE
            ke_msg_send(eaci_msg_buf[eaci_msg_idx].param);
            eaci_msg_idx++;
            if (eaci_msg_idx >= EACI_BUF_DEEP) {
                eaci_msg_idx = 0;
            }
            if (eaci_msg_buf_used == EACI_BUF_DEEP) {
                break;
            }
#else
            ke_msg_send(eaci_env.param);
#endif
            // Change eaci rx state to ke message header reception
            eaci_trans_read_start();
            break;

        default:
            break;
    }
}
/// @} EACI_TRANS
