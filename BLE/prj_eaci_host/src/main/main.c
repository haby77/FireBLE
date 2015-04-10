/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief Main loop of the application.
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include "usr_config.h"
#include "LPC17xx.h"
#include "uart.h"
#include "app_env.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_spi.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_nvic.h"

/*
 * FUNCTIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief App init function.
 *
 * This function is used to setup  and MENU UART.
 ****************************************************************************************
 */
#define BUFFER_SIZE 2
// PORT number that /CS pin assigned on
#define CS_PORT_NUM		0
// PIN number that  /CS pin assigned on
#define CS_PIN_NUM		16 
SPI_CFG_Type SPI_ConfigStruct;
SPI_DATA_SETUP_Type xferConfig;
//uint8_t Tx_Buf[BUFFER_SIZE];
//uint8_t Rx_Buf[BUFFER_SIZE];
/*********************************************************************//**
 * @brief 		Initialize CS pin as GPIO function to drive /CS pin
 * 				due to definition of CS_PORT_NUM and CS_PORT_NUM
 * @param		None
 * @return		None
 ***********************************************************************/
void CS_Init(void)
{
	GPIO_SetDir(CS_PORT_NUM, (1<<CS_PIN_NUM), 1);
	GPIO_SetValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
}


/*********************************************************************//**
 * @brief 		Drive CS output pin to low/high level to select slave device
 * 				via /CS pin state
 * @param[in]	state State of CS output pin that will be driven:
 * 				- 0: Drive CS pin to low level
 * 				- 1: Drive CS pin to high level
 * @return		None
 ***********************************************************************/
void CS_Force(int32_t state)
{
	if (state){
		GPIO_SetValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
	}else{
		GPIO_ClearValue(CS_PORT_NUM, (1<<CS_PIN_NUM));
	}
}

#if defined(CFG_HCI_SPI)
static void spi_gpio_init(void)
{
#if 0
    // DeInit NVIC and SCBNVIC
	NVIC_DeInit();
	NVIC_SCBDeInit();

	/* Configure the NVIC Preemption Priority Bits:
	 * two (2) bits of preemption priority, six (6) bits of sub-priority.
	 * Since the Number of Bits used for Priority Levels is five (5), so the
	 * actual bit number of sub-priority is three (3)
	 */
	NVIC_SetPriorityGrouping(0x05);

	//  Set Vector table offset value
    #if (__RAM_MODE__==1)
    NVIC_SetVTOR(0x10000000);
    #else
    NVIC_SetVTOR(0x00000000);
    #endif
#endif    
    
    PINSEL_CFG_Type PinCfg;
    //SPI_DATA_SETUP_Type xferConfig;
    PinCfg.Funcnum = 3;	 //0:GPIO,1:TXD,2:SSPI0,3:SPI
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;

    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 15;
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Pinnum = 17;
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Pinnum = 18;
    PINSEL_ConfigPin(&PinCfg);

    PinCfg.Pinnum = 16;
    PinCfg.Funcnum = 0;	//0:GPIO
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 03;
    PinCfg.Funcnum = 0;	//0:GPIO
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 02;
    PinCfg.Funcnum = 0;	//0:GPIO
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
    PINSEL_ConfigPin(&PinCfg);
    SPI_ConfigStruct.CPHA = SPI_CPHA_FIRST; 
    SPI_ConfigStruct.CPOL = SPI_CPOL_HI;
    SPI_ConfigStruct.ClockRate = 200000;
    SPI_ConfigStruct.DataOrder = SPI_DATA_MSB_FIRST;
    SPI_ConfigStruct.Databit = SPI_DATABIT_8;
    SPI_ConfigStruct.Mode = SPI_MASTER_MODE;
    SPI_Init(LPC_SPI, &SPI_ConfigStruct);
    // Initialize /CS pin to GPIO function
    CS_Init();
    //GPIO0.3 
    GPIO_SetDir(HCI_SPI_CTL_PORT_NUM, (1<<HCI_SPI_RD_PIN_NUM), 0);
    LPC_GPIOINT->IO0IntEnF = 0x08;
    NVIC_SetPriority(EINT3_IRQn, 4);
    NVIC_EnableIRQ(EINT3_IRQn);
      /* preemption = 0, sub-priority = 3 */
    NVIC_SetPriority(SPI_IRQn, 3);
    /* Enable SSP0 interrupt */
    NVIC_EnableIRQ(SPI_IRQn);
}
#endif

void EINT3_IRQHandler(void)
{
    #if defined(CFG_HCI_SPI)
        LPC_GPIOINT->IO0IntClr |= (1<<03);
        if (eaci_env.rx_state == EACI_STATE_RX_START)
        spi_read(QN_HCI_SPI, &eaci_env.msg_type, 1, eaci_uart_rx_done);
    #endif
}

static void AppInit(void)
{
    #if (defined(CFG_HCI_UART))
         // ACI UART
        uart_init(QN_HCI_UART, USARTx_CLK(0), UART_9600);
        uart_tx_enable(QN_HCI_UART, MASK_ENABLE);
        uart_rx_enable(QN_HCI_UART, MASK_ENABLE);
    #elif (defined(CFG_HCI_SPI))
        spi_gpio_init();
    #endif
    // Menu UART
    uart_init(QN_DEBUG_UART, USARTx_CLK(0), UART_9600);
    uart_tx_enable(QN_DEBUG_UART, MASK_ENABLE);
    uart_rx_enable(QN_DEBUG_UART, MASK_ENABLE);
    
    
    // Application
    app_init();
}

/**
 ****************************************************************************************
 * @brief Application schedule function.
 *
 * This function is called right after the booting process has completed.
 ****************************************************************************************
 */
static void AppSchedule(void)
{
    while (1)
    {
        // Check debug uart input
        if (app_env.input_len != 0)
        {
            app_menu_hdl();
            app_env.input_len = 0;
        }

        // Check aci uart input
        if (false == eaci_msg_que_is_empty(&app_env.msg_que))
        {
            struct eaci_msg *msg = (struct eaci_msg *)eaci_msg_que_pop(&app_env.msg_que);
            if (msg)
            {
                app_eaci_msg_hdl(msg->msg_type, msg->msg_id, msg->param_len, msg->param);
#ifndef MSG_BUFFER_EN
                MSG_FREE(msg);
#endif                
            }
        }
        
        // Check for sleep have to be done with interrupt disabled
        //GLOBAL_INT_DISABLE();
        // check sleep mode
        //GLOBAL_INT_RESTORE();
    }
}

/**
 ****************************************************************************************
 * @brief main function.
 *
 * This function is called right after the booting process has completed.
 ****************************************************************************************
 */
int main(void)
{
    // System
    SystemInit();

    // App
    AppInit();
    
    // Schedule
    GLOBAL_INT_START();
    AppSchedule();
    GLOBAL_INT_STOP();
}

