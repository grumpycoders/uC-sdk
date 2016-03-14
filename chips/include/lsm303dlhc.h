/*
    LSM303DLHC is 3D accelerometer and 3D magnetometer module
    http://www.st.com/web/catalog/sense_power/FM89/SC1449/PF251940

    It can communicate with I2C
*/
#pragma once

#include <decl.h>
#include <gpio.h>
#include <i2c.h>

// Accelerometer data rate
// use LSM303DLHC_A_POWERDOWN to turn accelerometer off
typedef enum {
    LSM303DLHC_A_POWERDOWN  = 0x00,
    LSM303DLHC_A_1HZ        = 0x01,
    LSM303DLHC_A_10HZ       = 0x02,
    LSM303DLHC_A_25HZ       = 0x03,
    LSM303DLHC_A_50HZ       = 0x04,
    LSM303DLHC_A_100HZ      = 0x05,
    LSM303DLHC_A_200HZ      = 0x06,
    LSM303DLHC_A_400HZ      = 0x07,
    LSM303DLHC_A_1620HZ     = 0x08, //low power mode
    LSM303DLHC_A_1344HZ     = 0x09, //1.344kHz in normal mode, 5.376kHz in low-power mode
} lsm303dlhc_a_odr_t;

// Accelerometer axis used
typedef enum {
    LLSM303DLHC_A_X_AXIS    = 0x01,
    LLSM303DLHC_A_Y_AXIS    = 0x02,
    LLSM303DLHC_A_Z_AXIS    = 0x04,
    LLSM303DLHC_A_ALL_AXES  = 0x07,
} lsm303dlhc_a_axis_t;

// Accelerometer full-scale selection
typedef enum {
    LSM303DLHC_A_2G         = 0x00,
    LSM303DLHC_A_4G         = 0x01,
    LSM303DLHC_A_8G         = 0x02,
    LSM303DLHC_A_16G        = 0x03,
} lsm303dlhc_a_fs_t;

// Accelerometer resolution
typedef enum {
    LSM303DLHC_A_LOW_RESOLUTION     = 0x00,
    LSM303DLHC_A_HIGH_RESOLUTION    = 0x01,
} lsm303dlhc_a_res_t;

// Accelerometer Endianness
typedef enum {
    LSM303DLHC_A_LITTLE_ENDIAN      = 0x00,
    LSM303DLHC_A_BIG_ENDIAN         = 0x01,
} lsm303dlhc_a_en_t;

/*
    i2c: i2c port, will be configured in lsm303dlhc_accelero_init
    odr: output data rate (see lsm303dlhc_a_odr_t)
    fs: full-scale selection (see lsm303dlhc_a_fs_t)
    res: resolution (see lsm303dlhc_a_res_t)
    en: endianness (see lsm303dlhc_a_en_t)
*/
typedef struct {
    i2c_t i2c;
    uint8_t odr:4, fs:2, res:1, en:1;
} lsm303dlhc_a_t;


// Magnetometer data rate
typedef enum
{
    LSM303DLHC_M_0_75HZ     = 0x00,
    LSM303DLHC_M_1_5HZ      = 0x01,
    LSM303DLHC_M_3_0HZ      = 0x02,
    LSM303DLHC_M_7_5HZ      = 0x03,
    LSM303DLHC_M_15HZ       = 0x04,
    LSM303DLHC_M_30HZ       = 0x05,
    LSM303DLHC_M_75HZ       = 0x06,
    LSM303DLHC_M_220HZ      = 0x07,
} lsm303dlhc_m_odr_t;

// Magnetometer gain
typedef enum {
    LSM303DLHC_M_1_3GAUSS = 0x01,
    LSM303DLHC_M_1_9GAUSS = 0x02,
    LSM303DLHC_M_2_5GAUSS = 0x03,
    LSM303DLHC_M_4_0GAUSS = 0x04,
    LSM303DLHC_M_4_7GAUSS = 0x05,
    LSM303DLHC_M_5_6GAUSS = 0x06,
    LSM303DLHC_M_8_1GAUSS = 0x07,
} lsm303dlhc_m_gain_t;

typedef struct {
    i2c_t i2c;
    uint8_t odr:3, gain:3;
} lsm303dlhc_m_t;

BEGIN_DECL

void lsm303dlhc_accelero_init(lsm303dlhc_a_t *accelero, i2c_port_t i2c_port);
void lsm303dlhc_accelero_read(lsm303dlhc_a_t accelero, float *data);

void lsm303dlhc_magneto_init(lsm303dlhc_m_t *magneto, i2c_port_t i2c_port);
void lsm303dlhc_magneto_read(lsm303dlhc_m_t magneto, float *data);

END_DECL
