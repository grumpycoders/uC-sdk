#include <l3gd20.h>

#include <malloc.h>
enum{
    L3GD20_WHO_AM_I         = 0x0f,
    L3GD20_CTRL_REG1        = 0x20,
    L3GD20_CTRL_REG2        = 0x21,
    L3GD20_CTRL_REG3        = 0x22,
    L3GD20_CTRL_REG4        = 0x23,
    L3GD20_CTRL_REG5        = 0x24,
    L3GD20_REFERENCE        = 0x25,
    L3GD20_OUT_TEMP         = 0x26,
    L3GD20_STATUS_REG       = 0x27,
    L3GD20_OUT_X_L          = 0x28,
    L3GD20_OUT_X_H          = 0x29,
    L3GD20_OUT_Y_L          = 0x2a,
    L3GD20_OUT_Y_H          = 0x2b,
    L3GD20_OUT_Z_L          = 0x2c,
    L3GD20_OUT_Z_H          = 0x2d,
    L3GD20_FIFO_CTRL_REG    = 0x2e,
    L3GD20_FIFO_SRC_REG     = 0x2f,
    L3GD20_INT1_CFG_REG     = 0x30,
    L3GD20_INT1_SRC_REG     = 0x31,
    L3GD20_INT1_THS_XH_REG  = 0x32,
    L3GD20_INT1_THS_XL_REG  = 0x33,
    L3GD20_INT1_THS_YH_REG  = 0x34,
    L3GD20_INT1_THS_YL_REG  = 0x35,
    L3GD20_INT1_THS_ZH_REG  = 0x36,
    L3GD20_INT1_THS_ZL_REG  = 0x37,
    L3GD20_INT1_DURATION_REG = 0x38
} l3gd20_registers_t;

#define READ_BYTE 0x80
#define MULTIBYTE_BYTE 0x40

void spi_get_register(l3gd20_t *sensor, uint8_t address, uint8_t *buffer, uint8_t nb){
    //set the read bit
    address |= READ_BYTE;

    //set multiple byte bit if necessary
    if (nb > 1)
        address |= MULTIBYTE_BYTE;
    gpio_set(sensor->cs, 0);
    //send the request
    ssp_write(sensor->ssp, address);

    //get the value
    while(nb--)
        *buffer++ = ssp_read(sensor->ssp);
    gpio_set(sensor->cs, 1);
}

void spi_set_register(l3gd20_t *sensor, uint8_t address, uint8_t *buffer, uint8_t nb){
    //reset the read bit
    address &= (0xff ^ READ_BYTE);

    //set multiple byte bit if necessary
    if (nb > 1)
        address |= MULTIBYTE_BYTE;
    gpio_set(sensor->cs, 0);
    //send the request
    ssp_write(sensor->ssp, address);

    //send the value
    while(nb--)
        ssp_write(sensor->ssp, *buffer++);
    gpio_set(sensor->cs, 1);
}


l3gd20_t *l3gd20_init_ssp(ssp_port_t port, pin_t cs){
    l3gd20_t *handle = (l3gd20_t *)malloc(sizeof(l3gd20_t));

    handle-> comm = l3gd20_spi;
    handle->ssp = port;
    handle->cs = cs;

    uint8_t b;
    b = 0x08 | 0x00 | 0x01 | 0x02 | 0x10;
    spi_set_register(handle, 0x20, &b, 1);
    b = 0x00 | 0x04;
    spi_set_register(handle, 0x21, &b, 1);
    b = 0x00 | 0x00 | 0x00;
    spi_set_register(handle, 0x23, &b, 1);
    b = 0x40;
    spi_set_register(handle, 0x2e, &b, 1);

    handle->sensitivity = 8.75e-3;

    return handle;
}

l3gd20_t *l3gd20_init_i2c(ssp_port_t port, pin_t cs){
    return NULL;
}

void l3gd20_setup_filter(l3gd20_t *sensor, float cutoff){
}

void l3gd20_power(l3gd20_t *sensor, int power){
  uint8_t b;
  spi_get_register(sensor, L3GD20_CTRL_REG1, &b, 1);
  b &= 0xF7;

  if (power)
      b |= (uint8_t) 0x08;
  spi_set_register(sensor, L3GD20_CTRL_REG1, &b, 1);
}

uint8_t l3gd20_who_am_i(l3gd20_t *sensor){
    uint8_t b;
    spi_get_register(sensor, L3GD20_WHO_AM_I, &b, 1);
    return b;
}
void l3gd20_read(l3gd20_t *sensor, float axis[3]){
    uint8_t b[6];
    int16_t r;
    spi_get_register(sensor, L3GD20_OUT_X_L, b, 6);
    uint8_t i;
    for (i = 0 ; i < 3 ; i++){
        r = (int16_t)(((uint16_t) b[2*i+11]) << 8 | b[2*i]);
        axis[i] = r * sensor->sensitivity;
    }
}

void l3gd20_set_sensitivity(l3gd20_t *sensor){

}

void l3gd20_set_datarate(l3gd20_t *sensor){

}

