#include <lsm303dlhc.h>

#include <malloc.h>
#include <stdio.h>

enum{
    // Accelerometer
    LSM303DLHC_A_CTRL_REG1        = 0x20,
    LSM303DLHC_A_CTRL_REG2        = 0x21,
    LSM303DLHC_A_CTRL_REG3        = 0x22,
    LSM303DLHC_A_CTRL_REG4        = 0x23,
    LSM303DLHC_A_CTRL_REG5        = 0x24,
    LSM303DLHC_A_CTRL_REG6        = 0x25,
    LSM303DLHC_A_REFERENCE        = 0x26,
    LSM303DLHC_A_STATUS_REG       = 0x27,
    LSM303DLHC_A_OUT_X_L          = 0x28,
    LSM303DLHC_A_OUT_X_H          = 0x29,
    LSM303DLHC_A_OUT_Y_L          = 0x2a,
    LSM303DLHC_A_OUT_Y_H          = 0x2b,
    LSM303DLHC_A_OUT_Z_L          = 0x2c,
    LSM303DLHC_A_OUT_Z_H          = 0x2d,
    LSM303DLHC_A_FIFO_CTRL_REG    = 0x2e,
    LSM303DLHC_A_FIFO_SRC_REG     = 0x2f,
    LSM303DLHC_A_INT1_CFG         = 0x30,
    LSM303DLHC_A_INT1_SRC         = 0x31,
    LSM303DLHC_A_INT1_THS         = 0x32,
    LSM303DLHC_A_INT1_DURATION    = 0x33,
    LSM303DLHC_A_INT2_CFG         = 0x34,
    LSM303DLHC_A_INT2_SRC         = 0x35,
    LSM303DLHC_A_INT2_THS         = 0x36,
    LSM303DLHC_A_INT2_DURATION    = 0x37,
    LSM303DLHC_A_CLICK_CFG        = 0x38,
    LSM303DLHC_A_CLICK_SRC        = 0x39,
    LSM303DLHC_A_CLICK_THS        = 0x3a,
    LSM303DLHC_A_TIME_LIMIT       = 0x3b,
    LSM303DLHC_A_TIME_LATENCY     = 0x3c,
    LSM303DLHC_A_TIME_WINDOW      = 0x3d,
    // Magnetometer
    LSM303DLHC_M_CRA_REG          = 0x00,
    LSM303DLHC_M_CRB_REG          = 0x01,
    LSM303DLHC_M_MR_REG           = 0x02,
    LSM303DLHC_M_OUT_X_H          = 0x03,
    LSM303DLHC_M_OUT_X_L          = 0x04,
    LSM303DLHC_M_OUT_Z_H          = 0x05,
    LSM303DLHC_M_OUT_Z_L          = 0x06,
    LSM303DLHC_M_OUT_Y_H          = 0x07,
    LSM303DLHC_M_OUT_Y_L          = 0x08,
    LSM303DLHC_M_SR_REG           = 0x09,
    LSM303DLHC_M_IRA_REG          = 0x0a,
    LSM303DLHC_M_IRB_REG          = 0x0b,
    LSM303DLHC_M_IRC_REG          = 0x0c,
    LSM303DLHC_M_TEMP_OUT_H       = 0x31,
    LSM303DLHC_M_TEMP_OUT_L       = 0x32,
} lsm303dlhc_registers_t;

enum {
    LSM303DLHC_A_I2C = 0x33,
    LSM303DLHC_M_I2C = 0x3D,
} lsm303dlhc_i2c_devices;

void i2c_read_register(i2c_t i2c, uint8_t device, uint8_t reg, uint8_t *buffer, uint8_t nb) {
    if(nb > 1)
        reg |= 0x80;
    i2c_wait(i2c);
    i2c_start_write(i2c, device);
    i2c_write(i2c, &reg, 1);
    i2c_start_read(i2c, device);
    i2c_read(i2c, buffer, nb);
    i2c_stop(i2c);
}

void i2c_write_register(i2c_t i2c, uint8_t device, uint8_t reg, uint8_t *buffer, uint8_t nb) {
    if(nb > 1)
        reg |= 0x80;
    i2c_start_write(i2c, device);
    i2c_write(i2c, &reg, 1);
    i2c_write(i2c, buffer, nb);
    i2c_stop(i2c);
}

void lsm303dlhc_accelero_init(lsm303dlhc_a_t *accelero, i2c_port_t i2c_port) {
    //init i2c port
    accelero->i2c = i2c_port.i2c;
    i2c_config(i2c_port, 400000);

    //init registers
     uint8_t c1 =
        (uint8_t)accelero->odr << 4 |
        LLSM303DLHC_A_ALL_AXES;
     uint8_t c4 =
        (uint8_t)accelero->en << 6 |
        (uint8_t)accelero->fs << 4 |
        (uint8_t)accelero->res << 3;
    i2c_write_register(accelero->i2c, LSM303DLHC_A_I2C, LSM303DLHC_A_CTRL_REG1, &c1, 1);
    i2c_write_register(accelero->i2c, LSM303DLHC_A_I2C, LSM303DLHC_A_CTRL_REG4, &c4, 1);
}

void lsm303dlhc_accelero_read(lsm303dlhc_a_t accelero, float *data) {
    uint8_t buffer[6];
    i2c_read_register(accelero.i2c, LSM303DLHC_A_I2C, LSM303DLHC_A_OUT_X_L, buffer, 6);

    float sensitivity = 0.0f;
    switch (accelero.fs) {
    case LSM303DLHC_A_2G:
        sensitivity = 1.0f;
        break;
    case LSM303DLHC_A_4G:
        sensitivity = 0.5f;
        break;
    case LSM303DLHC_A_8G:
        sensitivity = 0.25f;
        break;
    case LSM303DLHC_A_16G:
        sensitivity = 0.0834f;
        break;
    }

    if (accelero.res == LSM303DLHC_A_HIGH_RESOLUTION)
        sensitivity *= 4;

    uint8_t i;
    int16_t * axis_data = (int16_t *) buffer;
    for (i = 0 ; i < 3 ; i++)
        data[i] = (float)sensitivity * axis_data[i];
}

void lsm303dlhc_magneto_init(lsm303dlhc_m_t *magneto, i2c_port_t i2c_port) {
    //init i2c port
    magneto->i2c = i2c_port.i2c;
    i2c_config(i2c_port, 400000);

    //init registers
    uint8_t c[3];
    c[0] = (uint8_t)magneto->odr << 2;
    c[1] = (uint8_t)magneto->gain << 5;
    c[2] = 0; //Continuous mode
    i2c_write_register(magneto->i2c, LSM303DLHC_M_I2C, LSM303DLHC_M_CRA_REG, c, 3);
}

void lsm303dlhc_magneto_read(lsm303dlhc_m_t magneto, float *data) {
    uint8_t buffer[6];
    i2c_read_register(magneto.i2c, LSM303DLHC_M_I2C, LSM303DLHC_M_OUT_X_H, buffer, 6);

    float sensitivity_xy = 0.0f, sensitivity_z = 0.0f;
    switch (magneto.gain) {
    case LSM303DLHC_M_1_3GAUSS:
        sensitivity_xy = 1100.0f;
        sensitivity_z  = 980.0f;
        break;
    case LSM303DLHC_M_1_9GAUSS:
        sensitivity_xy = 855.0f;
        sensitivity_z  = 760.0f;
        break;
    case LSM303DLHC_M_2_5GAUSS:
        sensitivity_xy = 670.0f;
        sensitivity_z  = 600.0f;
        break;
    case LSM303DLHC_M_4_0GAUSS:
        sensitivity_xy = 450.0f;
        sensitivity_z  = 400.0f;
        break;
    case LSM303DLHC_M_4_7GAUSS:
        sensitivity_xy = 400.0f;
        sensitivity_z  = 355.0f;
        break;
    case LSM303DLHC_M_5_6GAUSS:
        sensitivity_xy = 330.0f;
        sensitivity_z  = 295.0f;
        break;
    case LSM303DLHC_M_8_1GAUSS:
        sensitivity_xy = 230.0f;
        sensitivity_z  = 205.0f;
        break;
    }

    data[0] = (float)(buffer[1] | ((uint16_t)buffer[0]) << 8) / sensitivity_xy;
    data[1] = (float)(buffer[5] | ((uint16_t)buffer[4]) << 8) / sensitivity_xy;
    data[2] = (float)(buffer[3] | ((uint16_t)buffer[2]) << 8) / sensitivity_z;
}


