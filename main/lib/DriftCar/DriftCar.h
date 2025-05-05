#ifndef __DRIFTCAR_H__
#define __DRIFTCAR_H__

#include "SOC.h"
#include "LEDs.h"

typedef soc_handle_t* soc_ctx_t;
typedef led_handle_t* led_ctx_t;

typedef struct
{
    soc_ctx_t soc_handle;
    led_ctx_t led_handle;
} DriftCar_ctx_t;

DriftCar_ctx_t *driftcar_create_app(void);

#endif // __DRIFTCAR_H__