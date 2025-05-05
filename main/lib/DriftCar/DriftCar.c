#include "DriftCar.h"

static DriftCar_ctx_t ctx;

DriftCar_ctx_t *driftcar_create_app(void)
{
    /* Init the SOC feature */
    ctx.soc_handle = soc_create();
    soc_start_task(ctx.soc_handle);

    return &ctx;
}
