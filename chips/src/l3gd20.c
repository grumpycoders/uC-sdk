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
    L3GD20_INT1_DURATION_REG = 0x38,
} l3gd20_registers_t;

enum{
    READ_BYTE = 0x80,
    MULTIBYTE_BYTE = 0x40,
};


void spi_read_registers(ssp_t ssp, uint8_t address, uint8_t *buffer, uint8_t nb) {
    //set the read bit
    address |= READ_BYTE;

    //set multiple byte bit if necessary
    if (nb > 1)
        address |= MULTIBYTE_BYTE;
    //send the request
    ssp_write(ssp, address);

    //get the value
    while(nb--)
        *buffer++ = ssp_read(ssp);
}

void spi_write_register(ssp_t ssp, uint8_t address, uint8_t value) {
    //reset the read bit
    address &= (0xff ^ READ_BYTE);

    //send the request
    ssp_write(ssp, address);

    //send the value
    ssp_write(ssp, value);
}

int l3gd20_init_ssp(l3gd20_t *l3gd20, ssp_port_t ssp_port) {
    pin_t cs = l3gd20->cs;
    ssp_t ssp = get_ssp(ssp_port);
    l3gd20->ssp = ssp;
    l3gd20->comm = L3GD20_SPI;

    gpio_set(cs, 1);

    ssp_config(ssp_port, 8 * 1000 * 1000);

    uint8_t b;

    // Let's read WHO_AM_I (0x0f) - should be 0xd4 for L3GD20.
    gpio_set(cs, 0);
    spi_read_registers(ssp, 0x0f, &b, 1);
    gpio_set(cs, 1);

    if (b != 0xd4) return 0;

    l3gd20_frequency(l3gd20, L3GD20_190HZ, L3GD20_BW1);
    l3gd20_power(l3gd20, 1);
    l3gd20_scale(l3gd20, L3GD20_250DPS);

    return 1;
}

/*
int l3gd20_init_i2c(l3gd20_t *l3gd20, i2c_port_t port){
    return NULL;
}
*/

void l3gd20_power(l3gd20_t *l3gd20, int power) {
    pin_t cs = l3gd20->cs;
    ssp_t ssp = l3gd20->ssp;

    gpio_set(cs, 1);

    uint8_t b;

    // Let's write to CTRL_REG1
    // 0x67 = 00000111
    // bit0: Enable X
    // bit1: Enable Y
    // bit2: Enable Z
    // bit3: Power Down
    // bit4-bit5: BW = bandwidth selection
    // bit6-bit7: ODR = sample frequency
    b = l3gd20->odr << 6 | l3gd20->bw << 4 | 0x07; // enable X, Y, Z
    if (power) b |= 0x08;
    l3gd20->power = power ? 1 : 0;

    gpio_set(cs, 0);
    spi_write_register(ssp, L3GD20_CTRL_REG1, b);
    gpio_set(cs, 1);
}

void l3gd20_scale(l3gd20_t *l3gd20, l3gd20_scale_t scale) {
    pin_t cs = l3gd20->cs;
    ssp_t ssp = l3gd20->ssp;

    gpio_set(cs, 1);

    uint8_t b;

    // Let's write to CTRL_REG4
    // Basically, setting defaults, just in case.
    // bit0: SPI mode: 4 wires
    // bit1-2: must be 0
    // bit3: nothing
    // bit4-bit5 : Full scale selection
    // bit6: little endian selection
    // bit7: block data update: continuous
    b = scale << 4;
    l3gd20->scale = scale;
    gpio_set(cs, 0);
    spi_write_register(ssp, L3GD20_CTRL_REG4, b);
    gpio_set(cs, 1);
}

void l3gd20_frequency(l3gd20_t *l3gd20, l3gd20_datarate_t odr, l3gd20_bandwidth_t bw) {
    l3gd20->odr = odr;
    l3gd20->bw = bw;
    l3gd20_power(l3gd20, l3gd20->power);
}

void l3gd20_read(l3gd20_t *l3gd20, float axis[3]) {
    pin_t cs = l3gd20->cs;
    ssp_t ssp = l3gd20->ssp;

    gpio_set(cs, 1);

    uint8_t registers[6];
    gpio_set(cs, 0);
    spi_read_registers(ssp, L3GD20_OUT_X_L, registers, 6);
    gpio_set(cs, 1);

    float sensitivity = 0.0f;

    // Reading scaling register from CTRL_REG5 (0x24)
    // That's bits 3 to 5.
    switch (l3gd20->scale) {
    case L3GD20_250DPS:
        sensitivity = 8.75f;
        break;
    case L3GD20_500DPS:
        sensitivity = 17.5f;
        break;
    case L3GD20_1000DPS:
        sensitivity = 35.0f;
        break;
    case L3GD20_2000DPS:
        sensitivity = 70.0f;
        break;
    }

    int i;
    // Now let's read X, Y, and Z.
    // That's registers 0x28 to 0x2d.
    // X is in 0x28 - 0x29.
    // Y is in 0x2a - 0x2b.
    // Z is in 0x2c - 0x2d.
    int16_t * axis_data = (int16_t *) registers;
    for (i = 0; i < 3; i++) {
        axis[i] = sensitivity * axis_data[i];
    }
}

void l3gd20_setup_filter(l3gd20_t *sensor, float cutoff){
}


