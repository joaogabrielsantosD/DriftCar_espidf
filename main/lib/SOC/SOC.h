#ifndef __SOC_H__
#define __SOC_H__

#include "esp_adc/adc_oneshot.h"
#include "driver_defs.h"

#define VREF             8.40   // Volts
#define VREF_MIN         6.00   // Volts
#define R1               47     // K Ohms
#define R2               30     // K Ohms
#define SAMPLES          (25)   // Number of samples to average
/* ADC channel */
#define ADC_UNIT         ADC_UNIT_1
#define SOC_ADC_CHANNEL  ADC_CHANNEL_6
#define SOC_WIDTH        ADC_BITWIDTH_12
#define SOC_ATTENUATION  ADC_ATTEN_DB_12

typedef struct  {
    int adc_value;
    float voltage_value;
    uint8_t percentage_value;

    uint32_t adc_resolution;

    adc_channel_t soc_channel;

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config;
    adc_oneshot_chan_cfg_t config_canal;
} SOC_t;

SOC_t *SOC_init(void);
esp_err_t destroy_SOC(SOC_t *soc);
uint8_t read_StateOfCharge(SOC_t *soc);

#endif // __SOC_H__
