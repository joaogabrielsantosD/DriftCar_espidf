#include "SOC.h"

static SOC_t soc_conf = {0};

SOC_t *SOC_init()
{
    soc_conf.soc_channel    = SOC_ADC_CHANNEL;
    soc_conf.adc_resolution = (1UL << SOC_WIDTH) - 1;

    soc_conf.init_config.unit_id = ADC_UNIT;
    
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&soc_conf.init_config, &soc_conf.adc1_handle));

    soc_conf.config_canal.bitwidth = SOC_WIDTH;
    soc_conf.config_canal.atten    = SOC_ATTENUATION;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(soc_conf.adc1_handle, soc_conf.soc_channel, &soc_conf.config_canal));

    read_StateOfCharge(&soc_conf);

    return &soc_conf;
}

esp_err_t destroy_SOC(SOC_t *soc)
{
    return adc_oneshot_del_unit(soc->adc1_handle);
}

uint8_t read_StateOfCharge(SOC_t *soc)
{
    int out = 0;
    for (int i = 0; i < SAMPLES; i++)
    {
        adc_oneshot_read(soc->adc1_handle, soc->soc_channel, &soc->adc_value);
        out += soc->adc_value;
    }
    out /= SAMPLES;

    soc->voltage_value = ((float)out / soc->adc_resolution) * VREF;
    // t2 = t2 * ((R1 + R2) / R2);

    if (soc->voltage_value >= VREF)
        soc->percentage_value = 100;
    else if (soc->voltage_value <= VREF_MIN)
        soc->percentage_value = 0;
    else
        soc->percentage_value = (uint8_t)(((soc->voltage_value - VREF_MIN) / (VREF - VREF_MIN)) * 100);

    return soc->percentage_value;
}
