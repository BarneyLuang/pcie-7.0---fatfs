#ifndef _POWER_H__
#define _POWER_H__


#include "include.h"

#define ADC_BUFFER_SIZE 10                                /**< Size of buffer for ADC samples.  */

extern nrf_adc_value_t       adc_buffer[ADC_BUFFER_SIZE]; /**< ADC buffer. */
extern nrf_drv_adc_channel_t m_channel_config ; /**< Channel instance. Default configuration used. */


void adc_config(void);
void get_adc_vol(void);
void init_power_cfg(void);
void mainboard_wake_up(void);
void enter_sleep_mode(void);
#endif 
