/**
 ****************************************************************************************
 *
 * @file fw_func_addr.h
 *
 * @brief The address of code and data in the firmware.
 *
 * Copyright(C) 2015 NXP Semiconductors N.V.
 * All rights reserved.
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _FW_FUNC_ADDR_H_
#define _FW_FUNC_ADDR_H_


/** @addtogroup QN_ROM_DRIVER_ADDR QN9020 Rom Driver Address
  @{
 */


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "app_config.h"

#if (defined(QN_9020_B2))

#define _flash_write_enable                             0x0100113b
#define _calibration_init                               0x01000339
#define _seq_calibration                                0x010003ad
#define _freq_hop_calibration                           0x01000419
#define _clock_32k_calibration                          0x0100048d
#define _ref_pll_calibration                            0x01000563
#define _rco_calibration                                0x0100058d
#define _DMA_IRQHandler                                 0x01000745
#define _dma_init                                       0x01000765
#define _dma_abort                                      0x0100078b
#define _dma_memory_copy                                0x01000793
#define _dma_tx                                         0x010007ad
#define _dma_rx                                         0x010007d9
#define _dma_transfer                                   0x01000807
#define _GPIO_IRQHandler                                0x0100085d
#define _gpio_init                                      0x01000881
#define _gpio_read_pin                                  0x01000891
#define _gpio_write_pin                                 0x010008a5
#define _gpio_set_direction                             0x010008bb
#define _gpio_read_pin_field                            0x010008cb
#define _gpio_write_pin_field                           0x010008d5
#define _gpio_set_direction_field                       0x010008e7
#define _gpio_toggle_pin                                0x010008f5
#define _gpio_set_interrupt                             0x0100090d
#define _gpio_enable_interrupt                          0x01000939
#define _gpio_disable_interrupt                         0x01000943
#define _gpio_pull_set                                  0x0100094d
#define _gpio_wakeup_config                             0x010009b3
#define _I2C_IRQHandler                                 0x01000a11
#define _i2c_init                                       0x01000ad5
#define _i2c_bus_check                                  0x01000afd
#define _i2c_read                                       0x01000b17
#define _i2c_write                                      0x01000b79
#define _I2C_BYTE_READ                                  0x01000bb9
#define _I2C_BYTE_READ2                                 0x01000bdb
#define _I2C_nBYTE_READ2                                0x01000c03
#define _I2C_BYTE_WRITE                                 0x01000c47
#define _I2C_BYTE_WRITE2                                0x01000c63
#define _I2C_nBYTE_WRITE2                               0x01000c83
#define _PWM0_IRQHandler                                0x01000ccd
#define _pwm_init                                       0x01000cdb
#define _rf_enable_sw_set_freq                          0x01000d7d
#define _rf_set_freq                                    0x01000d97
#define _rf_enable                                      0x01000df5
#define _rf_tx_power_level_set                          0x01000e55
#define _rf_tx_power_level_get                          0x01000e69
#define _rf_init                                        0x01000ed1
#define _dec2bcd                                        0x01000f15
#define _bcd2dec                                        0x01000f25
#define _rtc_time_get                                   0x01000f61
#define _RTC_IRQHandler                                 0x01000f77
#define _RTC_CAP_IRQHandler                             0x01000f91
#define _rtc_init                                       0x01000fb1
#define _rtc_calibration                                0x01000fd1
#define _rtc_correction                                 0x01000ff7
#define _rtc_capture_enable                             0x01001023
#define _rtc_capture_disable                            0x0100105b
#define _rtc_time_set                                   0x01001075
#define _is_flash_busy                                  0x0100110f
#define _set_flash_clock                                0x010010fd
#define _read_flash_id                                  0x01001151
#define _sector_erase_flash                             0x0100116f
#define _block_erase_flash                              0x01001197
#define _chip_erase_flash                               0x010011bd
#define _read_flash                                     0x010011cf
#define _write_flash                                    0x010011f5
#define _power_on_or_off_flash                          0x01001217
#define _SPI0_TX_IRQHandler                             0x01001353
#define _SPI0_RX_IRQHandler                             0x01001399
#define _spi_init                                       0x01001403
#define _spi_read                                       0x01001475
#define _spi_write                                      0x010014c1
#define _timer_delay                                    0x01001549
#define _TIMER0_IRQHandler                              0x01001575
#define _TIMER1_IRQHandler                              0x010015b3
#define _TIMER2_IRQHandler                              0x010015f1
#define _TIMER3_IRQHandler                              0x0100162f
#define _timer_init                                     0x0100166d
#define _timer_config                                   0x010016ed
#define _timer_pwm_config                               0x01001729
#define _timer_capture_config                           0x0100176d
#define _UART0_TX_IRQHandler                            0x01001879
#define _UART0_RX_IRQHandler                            0x010018b3
#define _uart_init                                      0x01001915
#define _uart_read                                      0x01001985
#define _uart_write                                     0x010019b9
#define _uart_printf                                    0x010019ed
#define _uart_finish_transfers                          0x01001a01
#define _uart_check_tx_free                             0x01001a09
#define _uart_flow_on                                   0x01001a3d
#define _uart_flow_off                                  0x01001a67
#define _nvds_read                                      0x01001f59
#define _nvds_write                                     0x01001fd1
#define _nvds_erase                                     0x010020ad
#define _nvds_init_memory                               0x010020c3
#define _nvds_init                                      0x0100214f
#define _nvds_get                                       0x01002187
#define _nvds_del                                       0x010021c5
#define _nvds_lock                                      0x010021ff
#define _nvds_put                                       0x0100222b
#define _co_bt_bdaddr_compare                           0x01002ae5
#define _co_list_init                                   0x01002b8d
#define _co_list_push_back                              0x01002b95
#define _co_list_push_front                             0x01002c1b
#define _co_list_pop_front                              0x01002c33
#define _co_list_extract                                0x01002c41
#define _co_list_find                                   0x01002c63
#define _co_list_merge                                  0x01002c7d
#define _task_desc_register                             0x01002cfd
#define _ke_state_set                                   0x01002d61
#define _ke_state_get                                   0x01002d89
#define _ke_msg_discard                                 0x01002eaf
#define _ke_msg_save                                    0x01002eb3
#define _ke_timer_set                                   0x01002f3b
#define _ke_timer_clear                                 0x01002faf
#define _ke_timer_sleep_check                           0x01003079
#define _ke_accurate_timer_set                          0x010030d1
#define _ke_evt_set                                     0x01003159
#define _ke_evt_clear                                   0x01003173
#define _ke_evt_callback_set                            0x0100318d 
#define _ke_malloc                                      0x01003289
#define _ke_free                                        0x010032db
#define _ke_msg_alloc                                   0x0100335d
#define _ke_msg_send                                    0x0100338b
#define _ke_msg_send_front                              0x010033b1
#define _ke_msg_send_basic                              0x010033d7
#define _ke_msg_forward                                 0x010033e5
#define _ke_msg_free                                    0x010033ef
#define _ke_task_handler_get                            0x01002dc7
#define _aci_init                                       0x01003423
#define _aci_tx_done                                    0x01003441
#define _aci_enter_sleep                                0x0100348b
#define _aci_exit_sleep                                 0x010034d1
#define _hci_init                                       0x01003ec7
#define _hci_tx_done                                    0x01004875
#define _hci_enter_sleep                                0x010048b9
#define _hci_exit_sleep                                 0x010048d9
#define _lld_crypt_isr                                  0x010072e7
#define _lld_evt_rx_isr                                 0x010085b1
#define _lld_evt_start_isr                              0x0100858d
#define _lld_evt_end_isr                                0x0100859f
#define _lld_evt_timer_isr                              0x010085c3
#define _deep_sleep_init                                0x0100876f
#define _set_32k_ppm                                    0x010087df
#define _set_32k_freq                                   0x010087e5
#define _revise_wakeup_delay                            0x010087ff
#define _attm_get_mtu                                   0x0100ab1b
#define _attsdb_add_service                             0x0100ae61
#define _attsdb_get_service                             0x0100af2d
#define _attsdb_add_attribute                           0x0100af75
#define _attsdb_destroy                                 0x0100b09b
#define _attsdb_get_attribute                           0x0100b0b5
#define _attsdb_get_next_att                            0x0100b0d3
#define _attsdb_att_update_value                        0x0100b17b
#define _attsdb_att_set_value                           0x0100b1b5
#define _attsdb_att_partial_value_update                0x0100b1bb
#define _attsdb_att_get_value                           0x0100b1ed
#define _attsdb_att_get_uuid                            0x0100b20d
#define _attsdb_att_set_permission                      0x0100b281
#define _attsdb_att_get_permission                      0x0100b299
#define _attsdb_att_update_perm                         0x0100b2b3
#define _attsdb_svc_set_permission                      0x0100b2d3
#define _attsdb_svc_get_permission                      0x0100b2eb
#define _atts_write_no_resp                             0x0100c78d
#define _atts_write_rsp_send                            0x0100cb4f
#define _atts_svc_create_db                             0x0100cb7d
#define _gap_get_rec_idx                                0x0100d07d
#define _gap_get_lk_sec_status                          0x0100d0cd
#define _gap_get_enc_keysize                            0x0100d0d7
#define _gap_get_conhdl                                 0x0100d0e1
#define _gap_get_security                               0x0100d0fd
#define _gap_send_discon_cmp_evt                        0x0100d103
#define _gap_send_connect_req                           0x0100d123
#define _ble_prevent_sleep_set                          0x01015207
#define _ble_wakeup                                     0x01015221
#define _ble_wakeup_end                                 0x01015253
#define _ble_disable                                    0x01015275
#define _ble_enable                                     0x01015285
#define _ble_reset                                      0x01015331
#define _ble_version                                    0x01015375
#define _ble_schedule                                   0x010153a7
#define _ble_sleep                                      0x010153e3
#define _ble_isr                                        0x01015455
#define _reg_sleep_cb                                   0x010154fd
#define _qn_config_init                                 0x01015545
#define _config_work_mode                               0x01015611
#define _save_cal_setting                               0x0101564d
#define _save_ble_reg                                   0x0101568d
#define _save_sys_regs                                  0x010156d7
#define _save_ble_setting                               0x01015743
#define _restore_ble_setting                            0x0101574f
#define _enable_ble_sleep                               0x0101578f
#define _set_max_sleep_duration                         0x01015797
#define _sw_wakeup_ble_hw                               0x010157b1
#define _qn_plf_init                                    0x01015981
#define _qn_ble_init                                    0x010159a5
#define _prf_init_reg                                   0x01015a39
#define _prf_disp_disconnect_reg                        0x01015a3f
#define _rtc_capture_env                                0x1000cdb4
#define _timer0_env                                     0x1000cdc4
#define _timer1_env                                     0x1000cdcc
#define _timer2_env                                     0x1000cdd4
#define _timer3_env                                     0x1000cddc
#define _ke_evt_hdlr                                    0x1000cdf4
#define _rtc_env                                        0x1000d03c
#define _nvds_env                                       0x1000d0b0
#define _ke_env                                         0x1000d1fc
#define _sleep_api                                      0x1000dbf0
#define _gap_env                                        0x1000dd5c
#define _ble_env                                        0x1000f2e8
#define _ble_rf                                         0x1000f2f4
#define _config_api                                     0x1000f32c
#define _Table_CRC                                      0x0101636a
#define _aci_env                                        0x1000d21c

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

/// @} QN_ROM_DRIVER_ADDR
#endif
