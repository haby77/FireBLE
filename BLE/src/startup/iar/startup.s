/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2009 IAR Systems. All rights reserved.
 *
 * $Revision: 47021 $
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

SYS_MODE_REG              EQU     0x40000080
REMAP_BIT                 EQU     0x40000000

_ADC_IRQHandler           EQU     0x0100035f
_COMPARATOR0_IRQHandler   EQU     0x010005e5
_COMPARATOR1_IRQHandler   EQU     0x010005f1
_CALIB_IRQHandler         EQU     0x010006d5
_DMA_IRQHandler           EQU     0x01000b91
_GPIO_IRQHandler          EQU     0x01000c85
_I2C_IRQHandler           EQU     0x01000ded
_PWM0_IRQHandler          EQU     0x01001099
_RTC_IRQHandler           EQU     0x0100133f
_RTC_CAP_IRQHandler       EQU     0x01001359
_SPI0_TX_IRQHandler       EQU     0x010016c3
_SPI0_RX_IRQHandler       EQU     0x01001709
_TIMER0_IRQHandler        EQU     0x010018e5
_TIMER1_IRQHandler        EQU     0x01001923
_TIMER2_IRQHandler        EQU     0x01001961
_TIMER3_IRQHandler        EQU     0x0100199f
_UART0_TX_IRQHandler      EQU     0x01001be9
_UART0_RX_IRQHandler      EQU     0x01001c23
_WDT_IRQHandler           EQU     0x01001ee7


        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        DATA
__vector_table
        DCD     sfe(CSTACK)                 ; Top of Stack
        DCD     Reset_Handler               ; Reset Handler
        DCD     NMI_Handler                 ; NMI Handler
        DCD     HardFault_Handler           ; Hard Fault Handler
        DCD     MemManage_Handler           ; MPU Fault Handler
        DCD     BusFault_Handler            ; Bus Fault Handler
        DCD     UsageFault_Handler          ; Usage Fault Handler
__vector_table_0x1c
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     SVC_Handler                 ; SVCall Handler
        DCD     DebugMon_Handler            ; Debug Monitor Handler
        DCD     0                           ; Reserved
        DCD     PendSV_Handler              ; PendSV Handler
        DCD     SysTick_Handler             ; SysTick Handler

        ; External Interrupts
#ifdef  __ROM_DRIVER_EN
        DCD     _GPIO_IRQHandler           ;  0:  GPIO Event
        DCD     _COMPARATOR0_IRQHandler   ;  1:  Comparator0
        DCD     _COMPARATOR1_IRQHandler   ;  2:  Comparator1
        DCD     BLE_IRQHandler            ;  3:  BLE
        DCD     _RTC_CAP_IRQHandler       ;  4:  RTC capture
        DCD     OSC_EN_IRQHandler         ;  5:  BLE IP OSC_EN output
        DCD     _RTC_IRQHandler           ;  6:  RTC
        DCD     _ADC_IRQHandler           ;  7:  ADC
        DCD     _DMA_IRQHandler           ;  8:  DMA
        DCD     0                         ;  9:  Reserved
        DCD     _UART0_TX_IRQHandler      ; 10:  UART0 TX
        DCD     _UART0_RX_IRQHandler      ; 11:  UART0 RX
        DCD     _SPI0_TX_IRQHandler       ; 12:  SPI0 TX
        DCD     _SPI0_RX_IRQHandler       ; 13:  SPI0 RX
        DCD     UART1_TX_IRQHandler       ; 14:  UART1 TX
        DCD     UART1_RX_IRQHandler       ; 15:  UART1 RX
        DCD     SPI1_TX_IRQHandler        ; 16:  SPI1 TX
        DCD     SPI1_RX_IRQHandler        ; 17:  SPI1 RX
        DCD     _I2C_IRQHandler           ; 18:  I2C
        DCD     _TIMER0_IRQHandler        ; 19:  Timer 0
        DCD     _TIMER1_IRQHandler        ; 20:  Timer 1
        DCD     _TIMER2_IRQHandler        ; 21:  Timer 2
        DCD     _TIMER3_IRQHandler        ; 22:  Timer 3
        DCD     _WDT_IRQHandler           ; 23:  Watch Dog
        DCD     _PWM0_IRQHandler          ; 24:  PWM CH0
        DCD     PWM1_IRQHandler           ; 25:  PWM CH1
        DCD     _CALIB_IRQHandler         ; 26:  Calibration
        DCD     0                         ; 27:  Reserved
        DCD     0                         ; 28:  Reserved
        DCD     TUNER_RX_IRQHandler       ; 29:  RF RX Setting
        DCD     TUNER_TX_IRQHandler       ; 30:  RF TX Setting
        DCD     0                         ; 31:  Reserved
#else
        DCD     GPIO_IRQHandler           ;  0:  GPIO Event
        DCD     COMPARATOR0_IRQHandler    ;  1:  Comparator0
        DCD     COMPARATOR1_IRQHandler    ;  2:  Comparator1
        DCD     BLE_IRQHandler            ;  3:  BLE
        DCD     RTC_CAP_IRQHandler        ;  4:  RTC capture
        DCD     OSC_EN_IRQHandler         ;  5:  BLE IP OSC_EN output
        DCD     RTC_IRQHandler            ;  6:  RTC
        DCD     ADC_IRQHandler            ;  7:  ADC
        DCD     DMA_IRQHandler            ;  8:  DMA
        DCD     0                         ;  9:  Reserved
        DCD     UART0_TX_IRQHandler       ; 10:  UART0 TX
        DCD     UART0_RX_IRQHandler       ; 11:  UART0 RX
        DCD     SPI0_TX_IRQHandler        ; 12:  SPI0 TX
        DCD     SPI0_RX_IRQHandler        ; 13:  SPI0 RX
        DCD     UART1_TX_IRQHandler       ; 14:  UART1 TX
        DCD     UART1_RX_IRQHandler       ; 15:  UART1 RX
        DCD     SPI1_TX_IRQHandler        ; 16:  SPI1 TX
        DCD     SPI1_RX_IRQHandler        ; 17:  SPI1 RX
        DCD     I2C_IRQHandler            ; 18:  I2C
        DCD     TIMER0_IRQHandler         ; 19:  Timer 0
        DCD     TIMER1_IRQHandler         ; 20:  Timer 1
        DCD     TIMER2_IRQHandler         ; 21:  Timer 2
        DCD     TIMER3_IRQHandler         ; 22:  Timer 3
        DCD     WDT_IRQHandler            ; 23:  Watch Dog
        DCD     PWM0_IRQHandler           ; 24:  PWM CH0
        DCD     PWM1_IRQHandler           ; 25:  PWM CH1
        DCD     CALIB_IRQHandler          ; 26:  Calibration
        DCD     0                         ; 27:  Reserved
        DCD     0                         ; 28:  Reserved
        DCD     TUNER_RX_IRQHandler       ; 29:  RF RX Setting
        DCD     TUNER_TX_IRQHandler       ; 30:  RF TX Setting
        DCD     TUNER_SETTING_IRQHandler  ; 31:  RF Setting

#endif


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;

        PUBWEAK Reset_Handler
        SECTION .RESET:CODE:REORDER(2)
	    THUMB
Reset_Handler
        ;; re-map
        LDR       R1, = SYS_MODE_REG
        LDR       R0, [R1]
        LDR       R2, = REMAP_BIT
        ORRS      R0, R0, R2
        STR       R0, [R1]

        LDR     R0, = sfe(CSTACK)
        MSR     MSP, R0

        LDR     R0, = __iar_program_start
        BX      R0

        SECTION .text:CODE:REORDER:NOROOT(2)
        THUMB
        PUBWEAK NMI_Handler
        PUBWEAK HardFault_Handler
        PUBWEAK MemManage_Handler
        PUBWEAK BusFault_Handler
        PUBWEAK UsageFault_Handler
        PUBWEAK SVC_Handler
        PUBWEAK DebugMon_Handler
        PUBWEAK PendSV_Handler
        PUBWEAK SysTick_Handler
        PUBWEAK GPIO_IRQHandler
        PUBWEAK COMPARATOR0_IRQHandler
        PUBWEAK COMPARATOR1_IRQHandler
        PUBWEAK BLE_IRQHandler
        PUBWEAK RTC_CAP_IRQHandler
        PUBWEAK OSC_EN_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK ADC_IRQHandler
        PUBWEAK DMA_IRQHandler
        PUBWEAK UART0_TX_IRQHandler
        PUBWEAK UART0_RX_IRQHandler
        PUBWEAK SPI0_TX_IRQHandler
        PUBWEAK SPI0_RX_IRQHandler
        PUBWEAK UART1_TX_IRQHandler
        PUBWEAK UART1_RX_IRQHandler
        PUBWEAK SPI1_TX_IRQHandler
        PUBWEAK SPI1_RX_IRQHandler
        PUBWEAK I2C_IRQHandler
        PUBWEAK TIMER0_IRQHandler
        PUBWEAK TIMER1_IRQHandler
        PUBWEAK TIMER2_IRQHandler
        PUBWEAK TIMER3_IRQHandler
        PUBWEAK WDT_IRQHandler
        PUBWEAK PWM0_IRQHandler
        PUBWEAK PWM1_IRQHandler
        PUBWEAK CALIB_IRQHandler
        PUBWEAK TUNER_RX_IRQHandler       ; 29:  RF RX Setting
        PUBWEAK TUNER_TX_IRQHandler       ; 30:  RF TX Setting
        PUBWEAK TUNER_SETTING_IRQHandler

NMI_Handler
        B NMI_Handler
HardFault_Handler
        B HardFault_Handler
MemManage_Handler
        B MemManage_Handler
BusFault_Handler
        B BusFault_Handler
UsageFault_Handler
        B UsageFault_Handler

SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
GPIO_IRQHandler
COMPARATOR0_IRQHandler
COMPARATOR1_IRQHandler
BLE_IRQHandler
RTC_CAP_IRQHandler
OSC_EN_IRQHandler
RTC_IRQHandler
ADC_IRQHandler
DMA_IRQHandler
UART0_TX_IRQHandler
UART0_RX_IRQHandler
SPI0_TX_IRQHandler
SPI0_RX_IRQHandler
UART1_TX_IRQHandler
UART1_RX_IRQHandler
SPI1_TX_IRQHandler
SPI1_RX_IRQHandler
I2C_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
WDT_IRQHandler
PWM0_IRQHandler
PWM1_IRQHandler
CALIB_IRQHandler
TUNER_RX_IRQHandler       ; 29:  RF RX Setting
TUNER_TX_IRQHandler       ; 30:  RF TX Setting
TUNER_SETTING_IRQHandler

Default_Handler
        B Default_Handler


        END
