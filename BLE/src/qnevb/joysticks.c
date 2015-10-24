#include "joysticks.h"
#include "button.h"
#include "sleep.h"
#include "pwm.h"
#include "adc.h"
#include "app_task.h"
#include "led.h"


#if	FB_JOYSTICKS
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


int app_key_scan_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
		if (gpio_read_pin(BUTTON1_PIN) == GPIO_LOW)
		{
				adc_init(ADC_SINGLE_WITHOUT_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);
				
				// Read voltage. use interrupt
				adc_read_configuration read_cfg;
				read_cfg.trig_src = ADC_TRIG_SOFT;
				read_cfg.mode = SINGLE_MOD;
				read_cfg.start_ch = AIN0;
				read_cfg.end_ch = AIN0;
				adc_read(&read_cfg,adc_key_value, KEY_SAMPLE_NUMBER, adc_test_cb);
				while(adc_done == 0);
		}	
		return(KE_MSG_CONSUMED);
}

int app_key_process_timer_handler(ke_msg_id_t const msgid, void const *param,
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
#if	(FB_OLED)
		ke_timer_set(APP_OLED_STATE_DISPlAY_TIMER,TASK_APP,20);
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
#if	FB_OLED
								OLED_ShowString(0,4, "                ");
								OLED_ShowString(0,4,"    key_up!     ");
#endif
								key_value0 = 0;
								break;
								
			case	key_left:
								QPRINTF("\r\nkey_left!\r\n");
#if	FB_OLED
								OLED_ShowString(0,4, "                ");
								OLED_ShowString(0,4,"    key_left!   ");
#endif
								led_set(2,LED_ON);
								key_value0 = 0;
						break;
			case	key_center:
								QPRINTF("\r\nkey_center!\r\n");
#if	FB_OLED
								OLED_ShowString(0,4,"                ");
								OLED_ShowString(0,4,"  key_center!   ");
#endif
								key_value0 = 0;
						break;
			case	key_right:
								QPRINTF("\r\nkey_right!\r\n");
#if	FB_OLED
								OLED_ShowString(0,4,"                ");
								OLED_ShowString(0,4,"   key_right!   ");
#endif
								led_set(3,LED_ON);
								key_value0 = 0;
						break;
			case	key_down:
								QPRINTF("\r\nkey_down!\r\n");
#if	FB_OLED
								OLED_ShowString(0,4,"                  ");
								OLED_ShowString(0,4,"    key_down!   ");
#endif
								led_set(2,LED_OFF);
								led_set(3,LED_OFF);
								key_value0 = 0;
						break;
			default	:	
								QPRINTF("\r\nkey_up!\r\n");
#if	FB_OLED
								OLED_ShowString(0,4,"                ");
								OLED_ShowString(0,4,"    key_up!     ");
#endif
								key_value0 = 0;
					break;
		}
#if FB_OLED
		ke_timer_set(APP_OLED_CLEAR_KEY_DISPLAY_TIMER,TASK_APP,60);
#endif
		usr_button_env.button_st = button_release;
		gpio_enable_interrupt(BUTTON1_PIN);
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
		if( (200 <	adc_key_value_avg)  && (adc_key_value_avg < 655))
		{
				key_value0 |= key_up;
				usr_button_env.joystick_dir = key_up;

		}
		else
		{
				if((656 <	adc_key_value_avg)  && (adc_key_value_avg < 1064))
				{
						usr_button_env.joystick_dir = key_right;
						key_value0 |= key_right;
				}
				else
				{
						if((1064 <	adc_key_value_avg)  && (adc_key_value_avg < 1474))
						{
								usr_button_env.joystick_dir = key_down;
								key_value0 |= key_down;
						}
						else
						{
									if((1475 <	adc_key_value_avg)  && (adc_key_value_avg < 1884))
									{
											usr_button_env.joystick_dir = key_left;
											key_value0 |= key_left;
									}
									else
									{
										if((1885 <	adc_key_value_avg)  && (adc_key_value_avg < 2048))
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
		ke_timer_set(APP_KEY_PROCESS_TIMER,TASK_APP,10);
}

#if FB_OLED
int app_oled_clear_key_display_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
		OLED_ShowString(0,4,"                ");		
		return(KE_MSG_CONSUMED);
}
#endif
#endif
///@USR END
