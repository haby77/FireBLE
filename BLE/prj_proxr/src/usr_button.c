#include "usr_button.h"
#include "button.h"
#include "sleep.h"
#include "pwm.h"
#include "adc.h"
#include "usr_task.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
 #define		KEY_SAMPLE_NUMBER		10
 
 #define		key_up				0x01
 #define		key_right			0x02
 #define		key_down			0x04
 #define		key_left			0x08
 #define		key_center		0x10
 
 /*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static  int16_t		adc_key_value[10];
static	int16_t adc_key_value_avg = 0;
static	int16_t	key_value0 = 0; 
static	uint8_t adc_done;

 static void adc_test_cb(void)
{
    adc_done = 1;
		ke_evt_set(1UL << EVENT_ADC_KEY_SAMPLE_CMP_ID);
}

button_env usr_button_env = {0,button_idle};

/**
 ****************************************************************************************
 * @brief Handles button press before key debounce.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

void app_event_button1_press_handler(void)
{
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
    if (sleep_env.deep_sleep)
    {
        sleep_env.deep_sleep = false;
        // start 32k xtal wakeup timer
        wakeup_32k_xtal_start_timer();
    }
#endif
		gpio_disable_interrupt(BUTTON1_PIN);
		ke_timer_set(USR_KEY_SCAN_TIMER,TASK_USR,10);
		ke_evt_clear(1UL << EVENT_BUTTON1_PRESS_ID);
    // delay 20ms to debounce
}

/**
 ****************************************************************************************
 * @brief   Button 1 click callback
 * @description
 *  Button 1 is used to enter adv mode.
 ****************************************************************************************
 */
void usr_button1_cb(void)
{
    // If BLE is in the sleep mode, wakeup it.
    if(ble_ext_wakeup_allow())
    {
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
        if (sleep_env.deep_sleep)
        {
            wakeup_32k_xtal_switch_clk();
        }
#endif

        sw_wakeup_ble_hw();

    }
		usr_button_env.button_st = button_press;
    // key debounce:
    // We can set a soft timer to debounce.
    // After wakeup BLE, the timer is not calibrated immediately and it is not precise.
    // So We set a event, in the event handle, set the soft timer.
    ke_evt_set(1UL << EVENT_BUTTON1_PRESS_ID);
}

int usr_key_scan_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
		adc_init(ADC_SINGLE_WITHOUT_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);
		
    // Read voltage. use interrupt
    adc_read_configuration read_cfg;
    read_cfg.trig_src = ADC_TRIG_SOFT;
    read_cfg.mode = BURST_MOD;
    read_cfg.start_ch = AIN0;
    read_cfg.end_ch = AIN0;
    adc_read(&read_cfg,adc_key_value, KEY_SAMPLE_NUMBER, adc_test_cb);
    while(adc_done == 0);	
		ke_timer_clear(USR_KEY_SCAN_TIMER,TASK_USR);
		return(KE_MSG_CONSUMED);
}

int usr_key_process_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
		ke_evt_clear(1UL << EVENT_ADC_KEY_SAMPLE_CMP_ID);
		switch(key_value0)
		{
			
			case	key_up:
								if(APP_IDLE == ke_state_get(TASK_APP))
                {
                        // start adv
											//QPRINTF("you press up key\r\n!");
                        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                                app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                                app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);

#if (QN_DEEP_SLEEP_EN)
                        // prevent entering into deep sleep mode
                        sleep_set_pm(PM_SLEEP);
#endif
                    //if APP_IDLE && KEY_LONG_PRESS && Server no working on,start adv.
                }
                else if(APP_ADV == ke_state_get(TASK_APP))
                {
                    // stop adv
                    app_gap_adv_stop_req();

#if (QN_DEEP_SLEEP_EN)
                    // allow entering into deep sleep mode
                    sleep_set_pm(PM_DEEP_SLEEP);
#endif
                }//if APP_ADV ,stop ADV and sleep.
								QPRINTF("\r\nkey_up!\r\n");
								key_value0 = 0;
								break;
								
			case	key_left:
								QPRINTF("\r\nkey_left!\r\n");
								key_value0 = 0;
						break;
			case	key_center:
								QPRINTF("\r\nkey_center!\r\n");
								key_value0 = 0;
						break;
			case	key_right:
								QPRINTF("\r\nkey_right!\r\n");
								key_value0 = 0;
						break;
			case	key_down:
								QPRINTF("\r\nkey_down!\r\n");
								key_value0 = 0;
						break;
			default	:	
								QPRINTF("\r\nkey_up!\r\n");
								key_value0 = 0;
					break;
		}
		usr_button_env.button_st = button_release;
    return(KE_MSG_CONSUMED);
}

void app_event_adc_key_sample_cmp_handler(void)
{		
		uint16_t i,sum = 0;
		ke_evt_clear(1UL << EVENT_ADC_KEY_SAMPLE_CMP_ID);
		adc_clock_off();
    adc_power_off();
		for (i = 0;i<10;i++)
				{
						//QPRINTF("array%d:  %d   result:%dmv\r\n",i,adc_key_value[i],ADC_RESULT_mV(adc_key_value[i]));
						sum +=  adc_key_value[i];
				}
		if (key_value0 != 0)
				key_value0 = 0;
		adc_key_value_avg = sum / 10;
		//QPRINTF("key:::%d   %dmv\r\n",adc_key_value_avg,ADC_RESULT_mV(adc_key_value_avg));
		if( (300 <	adc_key_value_avg)  && (adc_key_value_avg < 500))
		{
				key_value0 |= key_up;
				usr_button_env.joystick_dir = key_up;

		}
		else
		{
				if((800 <	adc_key_value_avg)  && (adc_key_value_avg < 1000))
				{
						usr_button_env.joystick_dir = key_right;
						key_value0 |= key_right;
				}
				else
				{
						if((1000 <	adc_key_value_avg)  && (adc_key_value_avg < 1400))
						{
								usr_button_env.joystick_dir = key_down;
								key_value0 |= key_down;
						}
						else
						{
									if((1600 <	adc_key_value_avg)  && (adc_key_value_avg < 1800))
									{
											usr_button_env.joystick_dir = key_left;
											key_value0 |= key_left;
									}
									else
									{
										if((1600 <	adc_key_value_avg)  && (adc_key_value_avg < 2048))
										{											
												usr_button_env.joystick_dir = key_center;
												key_value0 |= key_center;
										}
										else
										{
												key_value0 = 0;
										}
									}
						}
				}
		}
		//QPRINTF("key_value0 %d\r\n",key_value0);
		gpio_enable_interrupt(BUTTON1_PIN);
		ke_timer_set(USR_KEY_PROCESS_TIMER,TASK_USR,10);
}
///@USR END
