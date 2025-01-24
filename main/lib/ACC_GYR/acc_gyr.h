#ifndef ACC_GYR_H
#define ACC_GYR_H

// MPU9250 module
#include "driver/i2c.h"
#include "ahrs.h"
#include "mpu9250.h"
#include "calibrate.h"
#include "common.h"

/**
 * Transformation:
 *  - Rotate around Z axis 180 degrees
 *  - Rotate around X axis -90 degrees
 * @param  {object} s {x,y,z} sensor
 * @return {object}   {x,y,z} transformed
 */
void transform_accel_gyro(vector_t *v);

/**
 * Transformation: to get magnetometer aligned
 * @param  {object} s {x,y,z} sensor
 * @return {object}   {x,y,z} transformed
 */
void transform_mag(vector_t *v);

#endif
