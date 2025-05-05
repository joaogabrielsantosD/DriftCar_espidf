#include "DriftCar.h"

static DriftCar_ctx_t ctx;

DriftCar_ctx_t *driftcar_create_app(void)
{
    /* Init the SOC feature */
    ctx.soc_handle = soc_create();
    soc_start_task(ctx.soc_handle);

    /* Init the LED feature */
    ctx.led_handle = led_create();
    led_brake_off(ctx.led_handle);

    return &ctx;
}
