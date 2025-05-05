#include "SOC.h"

static const char TAG[] = "State_Of_Charge";
static const char TASK_TAG[] = "SOC_TASK";

static soc_handle_t soc_config;
static const uint32_t adc_resolution = (1UL << SOC_WIDTH) - 1;
static TaskHandle_t soc_task_handle = NULL;

static void state_of_charge_task(void *arg);


soc_handle_t *soc_create()
{
    esp_log_level_set(TAG, ESP_LOG_VERBOSE);

    adc_oneshot_unit_init_cfg_t unit_config = {.unit_id = SOC_ADC_UNIT};

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config, &soc_config.adc1_handle));

    adc_oneshot_chan_cfg_t config_channel = {
        .bitwidth = SOC_WIDTH,
        .atten    = SOC_ATTENUATION,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(soc_config.adc1_handle, SOC_ADC_CHANNEL, &config_channel));

    soc_read(&soc_config);

    ESP_LOGV(TAG, "ADC CONFIGURATION SUCESS, CHANNEL: %d\r\n", SOC_ADC_CHANNEL);
    return &soc_config;
}

esp_err_t soc_destroy(soc_handle_t *handle)
{
    if (handle == NULL) 
        return ESP_ERR_NO_MEM;

    esp_err_t err = adc_oneshot_del_unit(handle->adc1_handle);
    // free(handle);
    return err;
}

uint8_t soc_read(soc_handle_t *handle)
{
    if (handle == NULL)
        return 0;

    int out = 0;
    for (int i = 0; i < CONFIG_DRIFT_CAR_ADC_SAMPLES; i++)
    {
        adc_oneshot_read(handle->adc1_handle, SOC_ADC_CHANNEL, &handle->adc_value);
        out += handle->adc_value;
    }
    out /= CONFIG_DRIFT_CAR_ADC_SAMPLES;

    handle->voltage_value = ((float)out / adc_resolution) * VREF;
    // t2 = t2 * ((R1 + R2) / R2);

    if (handle->voltage_value >= VREF)
        handle->percentage_value = 100;
    else if (handle->voltage_value <= VREF_MIN)
        handle->percentage_value = 0;
    else
        handle->percentage_value = (uint8_t)(((handle->voltage_value - VREF_MIN) / (VREF - VREF_MIN)) * 100);

    return handle->percentage_value;
}


esp_err_t soc_start_task(soc_handle_t *handle)
{    
    if (soc_task_handle != NULL) 
    {
        ESP_LOGW(TAG, "SOC task already running");
        return ESP_ERR_INVALID_STATE;
    }

    uint32_t stack_size;

#if CONFIG_DRIFT_CAR_ADC_DEBUG_VALUE
    stack_size = 2048;
#else
    stack_size = 800;
#endif

    if (xTaskCreatePinnedToCore(state_of_charge_task, TASK_TAG, stack_size, handle, 3, &soc_task_handle, 1) != pdPASS) 
    {
        ESP_LOGE(TAG, "Failed to create SOC task");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "SOC task started");
    return ESP_OK;
}

void soc_stop_task(void)
{
    if (soc_task_handle != NULL) 
    {
        vTaskDelete(soc_task_handle);
        soc_task_handle = NULL;
        ESP_LOGI(TAG, "SOC task stopped");
    }
}

TaskHandle_t soc_task_handler(void) 
{ 
    return soc_task_handle; 
};


void state_of_charge_task(void *arg)
{
    soc_handle_t *soc_handle = (soc_handle_t *)arg; 

    while (true)
    {
        soc_read(soc_handle);

#if CONFIG_DRIFT_CAR_ADC_DEBUG_VALUE
        ESP_LOGV(TAG, "VOLTAGE: %.2f", soc_handle->voltage_value);
        ESP_LOGV(TAG, "PERCENTAGE: %d", soc_handle->percentage_value);
        println();
#endif
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}
