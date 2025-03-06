#include "SOC.h"

typedef struct {
    float buffer[50];
    float soma;
    int index;
} MovingAverageState;
typedef float (*filter)(MovingAverageState *, float);

static SOC_t soc_conf = {0};
static float moving_average(MovingAverageState *state, float new);

SOC_t *SOC_init()
{
    soc_conf.soc_channel    = SOC_ADC_CHANNEL;
    soc_conf.adc_resolution = (1UL << SOC_WIDTH) - 1;

    soc_conf.init_config.unit_id = ADC_UNIT;
    
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&soc_conf.init_config, &soc_conf.adc1_handle));

    soc_conf.config_canal.bitwidth = SOC_WIDTH;
    soc_conf.config_canal.atten    = SOC_ATTENUATION;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(soc_conf.adc1_handle, soc_conf.soc_channel, &soc_conf.config_canal));

    return &soc_conf;
};

esp_err_t destroy_SOC(SOC_t *soc)
{
    return adc_oneshot_del_unit(soc->adc1_handle);
}

float read_StateOfCharge(SOC_t *soc)
{
    filter test_read[4] = {moving_average, moving_average, moving_average, moving_average};
    MovingAverageState st[4] = {0};
    const int SAMPLES = 50;
    
    adc_oneshot_read(soc->adc1_handle, soc->soc_channel, &soc->adc_value);

    int val = 0, out = 0;
    for (int i = 0; i < SAMPLES; i++)
    {
        adc_oneshot_read(soc->adc1_handle, soc->soc_channel, &val);
        out += val;
    }
    out /= SAMPLES;

    /* Outro meio de fazer a conversão de tensão para porcentagem */
    float t0 = ((float)soc->adc_value / soc->adc_resolution) * VREF;
    t0 = t0 * ((R1 + R2) / R2);
    int p0 = t0 > VREF ? 100 : t0 < VREF_MIN ? 0 : (int)((t0 - VREF_MIN) / (VREF - VREF_MIN)) * 100;

    float t2 = ((float)out / soc->adc_resolution) * VREF;
    t2 = t2 * ((R1 + R2) / R2);
    int p2 = t2 > VREF ? 100 : t2 < VREF_MIN ? 0 : (int)((t2 - VREF_MIN) / (VREF - VREF_MIN)) * 100;

    float tt0 = ((float)soc->adc_value / soc->adc_resolution) * VREF;
    tt0 = tt0 * ((R1 + R2) / R2);
    tt0 = test_read[0](&st[0], tt0);
    int pp0 = tt0 > VREF ? 100 : tt0 < VREF_MIN ? 0 : (int)((tt0 - VREF_MIN) / (VREF - VREF_MIN)) * 100;

    float tt2 = ((float)out / soc->adc_resolution) * VREF;
    tt2 = tt2 * ((R1 + R2) / R2);
    tt2 = test_read[2](&st[1], tt2);
    int pp2 = tt2 > VREF ? 100 : tt2 < VREF_MIN ? 0 : (int)((tt2 - VREF_MIN) / (VREF - VREF_MIN)) * 100;

    /* Meio usado originalmente */
    float t1 = (float)((soc->adc_value * VREF) / soc->adc_resolution);
    int p1 = (t1 * 100) / (int)VREF;

    float t3 = (float)((out * VREF) / soc->adc_resolution);
    int p3 = (t3 * 100) / (int)VREF;

    float tt1 = (float)((soc->adc_value * VREF) / soc->adc_resolution);
    tt1 = test_read[1](&st[2], tt1);
    int pp1 = (tt1 * 100) / (int)VREF;

    float tt3 = (float)((out * VREF) / soc->adc_resolution);
    tt3 = test_read[3](&st[3], tt3);
    int pp3 = (tt3 * 100) / (int)VREF;

    printf("Valor lido: %d\r\n", soc->adc_value);
    printf("------------------ Método 1 (1 -> PURO) ------------------\r\n");
    printf("Tensão Pura: %.2f\r\n", t0);
    printf("Porcentagem Bruta: %d\r\n", p0);
    printf("------------------ Método 2 (2 -> PURO) ------------------\r\n");
    printf("Tensão Pura: %.2f\r\n", t1);
    printf("Porcentagem Bruta: %d\r\n", p1);
    println();
    printf("------------------ Método 3 (1 -> FILTRO NA LEITURA) ------------------\r\n");
    printf("Tensão Filtro 1: %.2f\r\n", t2);
    printf("Porcentagem Filtro 1: %d\r\n", p2);
    printf("------------------ Método 2 (2 -> FILTRO NA LEITURA) ------------------\r\n");
    printf("Tensão Filtro 1: %.2f\r\n", t3);
    printf("Porcentagem Filtro 1: %d\r\n", p3);
    println();
    printf("------------------ Método 1 (1 -> MV) ------------------\r\n");
    printf("Tensão MV: %.2f\r\n", tt0);
    printf("Porcentagem MV: %d\r\n", pp0);
    printf("------------------ Método 2 (2 -> MV) ------------------\r\n");
    printf("Tensão MV: %.2f\r\n", tt1);
    printf("Porcentagem MV: %d\r\n", pp1);
    println();
    printf("------------------ Método 1 (1 -> FILTRO NA LEITURA + MV) ------------------\r\n");
    printf("Tensão MV: %.2f\r\n", tt2);
    printf("Porcentagem MV: %d\r\n", pp2);
    printf("------------------ Método 2 (2 -> FILTRO NA LEITURA + MV) ------------------\r\n");
    printf("Tensão MV: %.2f\r\n", tt3);
    printf("Porcentagem MV: %d\r\n", pp3);
    println();

    return 0.0;
}

static float moving_average(MovingAverageState *state, float new)
{
    state->soma -= state->buffer[state->index];
    state->buffer[state->index] = new;
    state->soma += new;
    state->index = (state->index + 1) % 50;
    return state->soma / 50;
}
