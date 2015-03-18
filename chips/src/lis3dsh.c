#include "gpio.h"
#include "ssp.h"
#include "lis3dsh.h"

// http://www.st.com/web/en/resource/technical/document/datasheet/DM00040962.pdf
// Disclaimer: that above documentation is abysmally wrong. Several pages of
// them even, so it may or may not be a copy/paste mistake. The SPI protocol
// this chip follows isn't the one described at all. More specifically, there is
// no such thing as a "multibyte" flag in the register address byte. If you set
// it, you'll actually end up reading or writing registers from 0x40 to 0x7f
// instead of doing multibytes operations on 0x00 to 0x3f. Instead, multibyte is
// controlled by the flag ADD_INC from the control register 6. It seems this
// flag is set to enabled by default, but we'll probably want to set it anyway,
// just to be sure.

static __inline__ void spi_read_registers(ssp_t ssp, uint8_t address, uint8_t * buffer, uint8_t size) {
    address |= 0x80;
    ssp_write(ssp, address++);
    while (size--) *buffer++ = ssp_readwrite(ssp, address++);
}

static __inline__ void spi_write_register(ssp_t ssp, uint8_t address, uint8_t value) {
    ssp_write(ssp, address);
    ssp_write(ssp, value);
}

int lis3dsh_init_ssp(lis3dsh_t * lis3dsh, ssp_port_t ssp_port, pin_t cs) {
    lis3dsh->cs = cs;
    ssp_t ssp = ssp_port.ssp;
    lis3dsh->ssp = ssp;
    lis3dsh->power = 0;

    gpio_config(cs, pin_dir_write, pull_up);
    gpio_set(cs, 1);

    // lis3dsh can talk at 10Mhz according to the docs, but I got
    // jitter at that speed. So 4Mhz it is for the moment.
    ssp_config(ssp_port, 4 * 1000 * 1000);
    ssp_write(ssp, 0xff);

    uint8_t b;

    // Reading register "WHO_AM_I", should return 0x3f for lis3dsh.
    gpio_set(cs, 0);
    spi_read_registers(ssp, 0x0f, &b, 1);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);
    if (b != 0x3f) return 0;

    lis3dsh_frequency(lis3dsh, LIS3DSH_ODR_100HZ);
    lis3dsh_scale(lis3dsh, LIS3DSH_SCALE_2G);
    lis3dsh_power(lis3dsh, LIS3DSH_POWER_ON);

    return 1;
}

void lis3dsh_power(lis3dsh_t * lis3dsh, lis3dsh_power_t power) {
    pin_t cs = lis3dsh->cs;
    ssp_t ssp = lis3dsh->ssp;

    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    uint8_t b;

    // Let's write to CTRL_REG4 (0x20)
    // bit0: Enable X
    // bit1: Enable Y
    // bit2: Enable Z
    // bit3: Continuous update
    // bit4-bit7: ODR = sample frequency
    // power off mode ==> ODR = 0
    b = 7; // enable X, Y, Z
    if (power) b |= lis3dsh->odr << 4;
    lis3dsh->power = power ? 1 : 0;

    gpio_set(cs, 0);
    spi_write_register(ssp, 0x20, b);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);
}

void lis3dsh_scale(lis3dsh_t * lis3dsh, lis3dsh_scale_t scale) {
    pin_t cs = lis3dsh->cs;
    ssp_t ssp = lis3dsh->ssp;

    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    uint8_t b;

    // Let's write to CTRL_REG5 (0x24)
    b = scale << 3;
    lis3dsh->scale = scale;
    gpio_set(cs, 0);
    spi_write_register(ssp, 0x24, b);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);
}

void lis3dsh_frequency(lis3dsh_t * lis3dsh, lis3dsh_odr_t odr) {
    lis3dsh->odr = odr;
    lis3dsh_power(lis3dsh, lis3dsh->power);
}

void lis3dsh_read(lis3dsh_t * lis3dsh, float axis[3]) {
    pin_t cs = lis3dsh->cs;
    ssp_t ssp = lis3dsh->ssp;

    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    uint8_t registers[6];
    gpio_set(cs, 0);
    spi_read_registers(ssp, 0x28, registers, 6);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    float sensitivity = 0.0f;

    switch (lis3dsh->scale) {
    case LIS3DSH_SCALE_2G:
        sensitivity = 0.06f;
        break;
    case LIS3DSH_SCALE_4G:
        sensitivity = 0.12f;
        break;
    case LIS3DSH_SCALE_6G:
        sensitivity = 0.18f;
        break;
    case LIS3DSH_SCALE_8G:
        sensitivity = 0.24f;
        break;
    case LIS3DSH_SCALE_16G:
        sensitivity = 0.73f;
        break;
    }

    int i;
    // Now let's read X, Y, and Z.
    // That's registers 0x28 to 0x2d.
    // X is in 0x28 - 0x29.
    // Y is in 0x2a - 0x2b.
    // Z is in 0x2c - 0x2d.
    // The data is in Little Endian.
#ifndef TARGET_LITTLE_ENDIAN
    for (i = 0; i < 3; i++) {
        uint8_t t;
        t = registers[i * 2 + 5];
        registers[i * 2 + 5] = registers[i * 2 + 4];
        registers[i * 2 + 4] = t;
    }
#endif
    // Yes, I know, pointer aliasing. But the whole code is filled with it
    // anyway, and that should be the least of our problems.
    int16_t * axis_data = (int16_t *) registers;
    for (i = 0; i < 3; i++) {
        axis[i] = sensitivity * axis_data[i];
    }
}
