#include "include.h"

#define SLEEP_DELAY     600   //10分钟

 nrf_adc_value_t       adc_buffer[ADC_BUFFER_SIZE]; /**< ADC buffer. */
 nrf_drv_adc_channel_t m_channel_2_config = NRF_DRV_ADC_DEFAULT_CHANNEL(NRF_ADC_CONFIG_INPUT_2); /**< Channel instance. Default configuration used. */
 nrf_drv_adc_channel_t m_channel_3_config = NRF_DRV_ADC_DEFAULT_CHANNEL(NRF_ADC_CONFIG_INPUT_3); /**< Channel instance. Default configuration used. */
int16_t vol_c2 ;
int16_t vol_c3 ;
static uint32_t sleep_delay_num ;
/**
 * @brief ADC interrupt handler.
 */
static void adc_event_handler(nrf_drv_adc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_ADC_EVT_DONE)
    {
        uint32_t i;
        for (i = 0; i < p_event->data.done.size; i++)
        {
            NRF_LOG_INFO("Current sample value: %d\r\n", p_event->data.done.p_buffer[i]);
        }
    }
}

/**
 * @brief ADC initialization.
 */
void adc_config(void)
{
	nrf_adc_config_t   adc_config ={NRF_ADC_CONFIG_RES_10BIT,NRF_ADC_CONFIG_SCALING_INPUT_FULL_SCALE,NRF_ADC_CONFIG_REF_VBG};
	nrf_adc_configure( &adc_config);
}


/*
 * 
*/
void get_adc_vol(void)
{
	uint16_t PwrAdcData_c3,PwrAdcData_c2;
	vol_c2 =	nrf_adc_convert_single(NRF_ADC_CONFIG_INPUT_2);//pin 1
	vol_c3 =	nrf_adc_convert_single(NRF_ADC_CONFIG_INPUT_3);//pin 2

  PwrAdcData_c2= vol_c2*300/1024;  
	PwrAdcData_c3= vol_c3*300/1024;  
//	NRF_LOG_INFO("vol_c2 ：%d  vol_c3 ：%d\r\n",vol_c2,vol_c3);	
//	NRF_LOG_INFO("PwrAdcData_c2 ：%d  PwrAdcData_c3 ：%d\r\n",PwrAdcData_c2,PwrAdcData_c3);	

	
	//check charging    
	if(nrf_gpio_pin_read(CHECK_CHARGING_PIN))
	{
			mainboard_wake_up();
			system_func_flag.flag_charging = 1;
	}
	else 
	{
			system_func_flag.flag_charging = 0;
	}
}


void init_power_cfg(void)
{
	nrf_gpio_cfg_input(CHECK_CHARGING_PIN,NRF_GPIO_PIN_PULLUP);
	
	nrf_gpio_cfg_output(MAINBOARD_POWER_EN_PIN);
	
	LED_WAKE_UP_OFF();
}


void mainboard_wake_up(void)
{
	sleep_delay_num =0;
	system_func_flag.flag_wakeup =1;
	nrf_gpio_pin_set(MAINBOARD_POWER_EN_PIN);
	
	NRF_LOG_INFO("mainboard_wake_up  唤醒\r\n");
}

void enter_sleep_mode(void)
{
	if(SLEEP_DELAY>sleep_delay_num)
	{
		sleep_delay_num++;
		system_func_flag.flag_boardsleep =0;
	}
	else 
	{
		system_func_flag.flag_wakeup = 0;
		system_func_flag.flag_boardsleep =1;
		nrf_gpio_pin_clear(MAINBOARD_POWER_EN_PIN);
		NRF_LOG_INFO("enter_sleep_mode 进入低功耗睡眠 \r\n");
	}
}